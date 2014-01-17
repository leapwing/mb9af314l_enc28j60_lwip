#include "system.h"
#include "spim_dma_support.h"
#include "dualtimer_delay.h"
#include "enc28j60.h"
#include <stdio.h>

static uint8_t Enc28j60Bank;
static uint32_t NextPacketPtr;

static int32_t enc28j60_retx(void);

uint8_t enc28j60ReadOp(uint8_t op, uint8_t address)
{
	uint8_t r_tx, r_rx;
	uint32_t size = 1;
	ENC28J60_CSL();
	r_tx = op | (address & ADDR_MASK);		// Send the Read Control Register opcode and address.
	SPIDev_BufTxRx(&r_tx, &r_rx,size);
	if(address & 0x80){
		SPIDev_DataRx(&r_rx, &size);
	}
    ENC28J60_CSH();
	return r_rx;
}

void enc28j60WriteOp(uint8_t op, uint8_t address, uint8_t data)
{
	uint8_t r_tx[2];
	uint32_t size = 2;
	ENC28J60_CSL();
	r_tx[0] = op | (address & ADDR_MASK);		// Send the Read Control Register opcode and address.
	r_tx[1] = data;
	SPIDev_DataTx(&r_tx, &size);
	
    ENC28J60_CSH();
}

void enc28j60SetBank(uint8_t address)
{
	// set the bank (if needed)
	if((address & BANK_MASK) != Enc28j60Bank)
	{
		// set the bank
		enc28j60WriteOp(ENC28J60_BIT_FIELD_CLR, ECON1, (ECON1_BSEL1|ECON1_BSEL0));
		enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, ECON1, (address & BANK_MASK)>>5);
		Enc28j60Bank = (address & BANK_MASK);
	}
}

uint8_t enc28j60Read(uint8_t address)
{
	// select bank to read
	enc28j60SetBank(address);
	// do the read
	return enc28j60ReadOp(ENC28J60_READ_CTRL_REG, address);	
}

void enc28j60Write(uint8_t address, uint8_t data)
{
	// select bank to write
	enc28j60SetBank(address);
	// do the write
	enc28j60WriteOp(ENC28J60_WRITE_CTRL_REG, address, data);
}

uint32_t enc28j60_read_phyreg(uint8_t address)
{
	uint32_t data;
	// set the PHY register address
	enc28j60Write(MIREGADR, address);
	enc28j60Write(MICMD, MICMD_MIIRD);
	// Loop to wait until the PHY register has been read through the MII
	// This requires 10.24us
	while( (enc28j60Read(MISTAT) & MISTAT_BUSY) );
	// Stop reading
	enc28j60Write(MICMD, 0);
	// Obtain results and return
	data = enc28j60Read ( MIRDL );
	data |= enc28j60Read ( MIRDH ) << 8;	
	return data;
}

void enc28j60PhyWrite(uint8_t address, uint16_t data)
{
	// set the PHY register address
	enc28j60Write(MIREGADR, address);
	// write the PHY data
	enc28j60Write(MIWRL, data & 0xFF);
	enc28j60Write(MIWRH, data >> 8);
	// wait until the PHY write completes
	while(enc28j60Read(MISTAT) & MISTAT_BUSY);
}

void enc28j60_soft_reset(void)
{
	// Note: The power save feature may prevent the reset from executing, so
    // we must make sure that the device is not in power save before issuing
    // a reset.
	enc28j60WriteOp(ENC28J60_BIT_FIELD_CLR, ECON2, ECON2_PWRSV);
	// Give some opportunity for the regulator to reach normal regulation and
    // have all clocks running
    _delay_ms(1);
	// Execute the System Reset command
	enc28j60WriteOp(ENC28J60_SOFT_RESET, 0, ENC28J60_SOFT_RESET);
	// Wait for the oscillator start up timer and PHY to become ready
    _delay_ms(1);
}

void enc28j60_powerdown(void)
{
	// Disable packet reception
	enc28j60WriteOp(ENC28J60_BIT_FIELD_CLR, ECON1, ECON1_RXEN);
	// Make sure any last packet which was in-progress when RXEN was cleared is completed
	while( (enc28j60Read(ESTAT) & ESTAT_RXBUSY) );
	// If a packet is being transmitted, wait for it to finish
	while( (enc28j60Read(ECON1) & ECON1_TXRST) );
	// Enter sleep mode
	enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, ECON2, ECON2_PWRSV);
}

void enc28j60_powerup(void)
{
	// Leave power down mode
	enc28j60WriteOp(ENC28J60_BIT_FIELD_CLR, ECON2, ECON2_PWRSV);
    // Wait for the 300us Oscillator Startup Timer (OST) to time out.  This
    // delay is required for the PHY module to return to an operational state.
	while(!(enc28j60Read(ESTAT) & ESTAT_CLKRDY) );
    // Enable packet reception
	enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_RXEN);
}

void enc28j60_clkout_set(uint8_t clk)
{
	// 0x00: CLKOUT disabled (pin driven low)
    // 0x01: Divide by 1 (25 MHz)
    // 0x02: Divide by 2 (12.5 MHz)
    // 0x03: Divide by 3 (8.333333 MHz)
    // 0x04: Divide by 4 (6.25 MHz, POR default)
    // 0x05: Divide by 8 (3.125 MHz)
	// 0x06: Reserved
	// 0x07: Reserved
	enc28j60Write(ECOCON, clk & 0x7);
}

uint8_t enc28j60_clkout_get(void)
{
	uint8_t data;
	data = enc28j60Read(ECOCON) & 0x07;
	return data;
}

int32_t enc28j60_check_int(void)
{
	volatile uint16_t isr;
	int32_t res = 0;
	isr = enc28j60Read(EIR);
	
	if(isr & EIR_PKTIF){
		res |= EIR_PKTIF;
	}
	if(isr & EIR_LINKIF){
		res |= EIR_LINKIF;
	}
    /*
    if(isr & EIR_TXIF){
        res |= EIR_TXIF;
    }
    if(isr & EIR_TXERIF){
		res |= EIR_TXERIF;
	}
    */
	if(isr & EIR_RXERIF){
		res |= EIR_RXERIF;
	}
	return res;
}

int32_t enc28j60_clear_int(uint16_t isr)
{
	uint16_t isr_mask = isr & (EIR_TXIF|EIR_TXERIF);
	if(isr_mask) enc28j60WriteOp(ENC28J60_BIT_FIELD_CLR, EIR, isr_mask);
	return 0;
}

int32_t enc28j60_clear_rxer(void)
{
    if(enc28j60Read(EPKTCNT) != 0){ 
        do{
        enc28j60Write(ERDPTL, NextPacketPtr & 0xFF);
        enc28j60Write(ERDPTH, (NextPacketPtr)>>8);
        NextPacketPtr  = enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0);
        NextPacketPtr |= enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0)<<8;
        enc28j60Write(ERXRDPTL, NextPacketPtr & 0xFF);
        enc28j60Write(ERXRDPTH, (NextPacketPtr)>>8);
        enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, ECON2, ECON2_PKTDEC);
        }while(enc28j60Read(EPKTCNT) != 0);
    }
    enc28j60WriteOp(ENC28J60_BIT_FIELD_CLR, EIR, EIR_RXERIF);
    //enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_RXRST);
    //enc28j60WriteOp(ENC28J60_BIT_FIELD_CLR, ECON1, ECON1_RXRST);
	return 0;
}

uint8_t enc28j60_init(uint8_t *macaddr)
{
	uint8_t version;
	// Set up the SPI module on the FM3 for communications with the ENC28J60
	ENC28J60_RST_INIT();
	ENC28J60_CS_INIT();
	ENC28J60_INT_INIT();
	//Exint_EnableChannel(EXTINT3_CH);
	// If the RESET pin is connected, take the chip out of reset
	ENC28J60_RST_OUT(0);
	_delay_ms(10);
	ENC28J60_RST_OUT(1);
	
	// Set up SPI
   	SPIDev_Init();

	// RESET the entire ENC28J60, clearing all registers
    // Also wait for CLKRDY to become set.
    // Bit 3 in ESTAT is an unimplemented bit.  If it reads out as '1' that
    // means the part is in RESET or there is something wrong with the SPI
    // connection.  This loop makes sure that we can communicate with the
    // ENC28J60 before proceeding.
    do {
        enc28j60_soft_reset();
    } while(!(enc28j60Read(ESTAT) & ESTAT_CLKRDY)); 
	
	// do bank 0 stuff
	// initialize receive buffer
	// 16-bit transfers, must write low byte first
	// set receive buffer start address
	NextPacketPtr = RXSTART_INIT;
	
	// Rx start
	enc28j60Write(ERXSTL, RXSTART_INIT&0xFF);
	enc28j60Write(ERXSTH, RXSTART_INIT>>8);
	// set receive pointer address
	enc28j60Write(ERXRDPTL, RXSTART_INIT&0xFF);
	enc28j60Write(ERXRDPTH, RXSTART_INIT>>8);
	// RX end
	enc28j60Write(ERXNDL, RXSTOP_INIT&0xFF);
	enc28j60Write(ERXNDH, RXSTOP_INIT>>8);
	// TX start
	enc28j60Write(ETXSTL, TXSTART_INIT&0xFF);
	enc28j60Write(ETXSTH, TXSTART_INIT>>8);
	// TX end
	enc28j60Write(ETXNDL, TXSTOP_INIT&0xFF);
	enc28j60Write(ETXNDH, TXSTOP_INIT>>8);
	
	// do bank 2 stuff
	// enable MAC receive
	enc28j60Write(MACON1, MACON1_MARXEN|MACON1_TXPAUS|MACON1_RXPAUS);
	
	// enable automatic padding to 60bytes and CRC operations
	enc28j60Write(MACON3, MACON3_PADCFG0|MACON3_TXCRCEN|MACON3_FRMLNEN|MACON3_FULDPX);
	
	// Allow infinite deferals if the medium is continuously busy 
    // (do not time out a transmission if the half duplex medium is 
    // completely saturated with other people's data)
    enc28j60Write(MACON4, MACON4_DEFER);

	// Late collisions occur beyond 63+8 bytes (8 bytes for preamble/start of frame delimiter)
	// 55 is all that is needed for IEEE 802.3, but ENC28J60 B5 errata for improper link pulse 
	// collisions will occur less often with a larger number.
    enc28j60Write(MACLCON2, 63);
	
	// Set non-back-to-back inter-packet gap to 9.6us.  The back-to-back 
	// inter-packet gap (MABBIPG) is set by MACSetDuplex() which is called 
	// later.
	enc28j60Write(MAIPGL, 0x12);
	enc28j60Write(MAIPGH, 0x0C);
	
	// Set the maximum packet size which the controller will accept
    // Do not send packets longer than MAX_PACKETSIZE:
	enc28j60Write(MAMXFLL, MAX_PACKETSIZE&0xFF);	
	enc28j60Write(MAMXFLH, MAX_PACKETSIZE>>8);
	
	// do bank 3 stuff
    // write MAC address
	// NOTE: MAC address in ENC28J60 is byte-backward
	// ENC28J60 is big-endian arm gcc is little-endian
	enc28j60Write(MAADR5, macaddr[0]);	
	enc28j60Write(MAADR4, macaddr[1]);
	enc28j60Write(MAADR3, macaddr[2]);
	enc28j60Write(MAADR2, macaddr[3]);
	enc28j60Write(MAADR1, macaddr[4]);
	enc28j60Write(MAADR0, macaddr[5]);
	
	enc28j60PhyWrite(PHCON1, PHCON1_PDPXMD);
	
	// no loopback of transmitted frames
	enc28j60PhyWrite(PHCON2, PHCON2_HDLDIS);
	
	
	// Disable the CLKOUT output to reduce EMI generation
	enc28j60_clkout_set(0);
	
	// Get the Rev ID so that we can implement the correct errata workarounds
	version = enc28j60getrev();
	
	// Configure LEDA to display LINK status, LEDB to display TX/RX activity
	enc28j60PhyWrite(PHLCON,0x3472);
	
	// do bank 1 stuff, packet filter:
	// For broadcast packets we allow only ARP packtets
	// All other packets should be unicast only for our mac (MAADR)
	//
	// The pattern to match on is therefore
	// Type     ETH.DST
	// ARP      BROADCAST
 	// 06 08 -- ff ff ff ff ff ff -> ip checksum for theses bytes=f7f9
	// in binary these poitions are:11 0000 0011 1111
	// This is hex 303F->EPMM0=0x3f,EPMM1=0x30
	enc28j60Write(ERXFCON, ERXFCON_UCEN|ERXFCON_CRCEN|ERXFCON_PMEN);
	
	enc28j60Write(EPMM0, 0x3f);
	enc28j60Write(EPMM1, 0x30);
	enc28j60Write(EPMCSL, 0xf9);
	enc28j60Write(EPMCSH, 0xf7);
	
	// set inter-frame gap (back-to-back)
	enc28j60Write(MABBIPG, 0x15);
	
	// switch to bank 0
	enc28j60SetBank(ECON1);

	// enable interrutps
	enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, EIE, EIE_INTIE|EIE_PKTIE|EIE_RXERIE|EIE_LINKIE);
    enc28j60PhyWrite(PHIE,0x12);  // link phy interrupt
	// enable packet reception
	enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_RXEN);
	
	return version;
}

uint8_t enc28j60getrev(void)
{
	return(enc28j60Read(EREVID));
}

uint8_t enc28j60_mac_is_txReady(void)
{
	if(!(enc28j60Read(ECON1) & ECON1_TXRTS ))
		return 1;
	else
		return 0;
}

uint8_t enc28j60_mac_is_linked(void)
{
	if((enc28j60_read_phyreg(PHSTAT1) & PHSTAT1_LLSTAT ))
		return 1;
	else
		return 0;
}

int32_t enc28j60_retx(void)
{
	uint8_t r_tx;
	uint8_t rx_status[7];
	uint16_t txend;
	uint32_t size;
	int32_t i, res = 0;
	
	enc28j60WriteOp(ENC28J60_BIT_FIELD_CLR, ECON1, ECON1_TXRTS);
	
	txend = enc28j60Read(ETXNDL) & 0xFF;
	txend |= enc28j60Read(ETXNDH) << 8;
	txend++;
	
	enc28j60Write(ERDPTL, txend & 0xFF);
	enc28j60Write(ERDPTH, txend >> 8);
	
	r_tx = ENC28J60_READ_BUF_MEM;
	size = 1;
	ENC28J60_CSL();
	SPIDev_DataTx(&r_tx, &size);
		
	// Send the data, one byte at a time
	size = 7;
	SPIDev_DataRx(rx_status, &size);

	// Terminate the burst operation
	ENC28J60_CSH();
	
	if((rx_status[3] & 0xA0) == 0x20){
		for(i = 0; i < 3; i++){
			enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_TXRST);
			enc28j60WriteOp(ENC28J60_BIT_FIELD_CLR, ECON1, ECON1_TXRST);
			enc28j60WriteOp(ENC28J60_BIT_FIELD_CLR, EIR, EIR_TXERIF|EIR_TXIF);
			enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_TXRTS);
			while(!(enc28j60Read(EIR) & (EIR_TXERIF | EIR_TXIF)));
			enc28j60WriteOp(ENC28J60_BIT_FIELD_CLR, ECON1, ECON1_TXRST);
			enc28j60Write(ERDPTL, txend & 0xFF);
			enc28j60Write(ERDPTH, txend >> 8);
			r_tx = ENC28J60_READ_BUF_MEM;
			size = 1;
			ENC28J60_CSL();
			SPIDev_DataTx(&r_tx, &size);
			size = 7;
			SPIDev_DataRx(rx_status, &size);
			ENC28J60_CSH();
			if((enc28j60Read(EIR) & (EIR_TXERIF)) && ((rx_status[3] & 0xA0) == 0x20)){
				res = 0;
			}else{
				res = 1;
				break;
			}
		}
	}else{
		enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_TXRST);
		enc28j60WriteOp(ENC28J60_BIT_FIELD_CLR, ECON1, ECON1_TXRST);
		enc28j60WriteOp(ENC28J60_BIT_FIELD_CLR, EIR, EIR_TXERIF);
	}
	return res;
}


void enc28j60_packet_send(uint8_t *buffer, uint32_t length)
{
	int32_t res;
	
	res = enc28j60_BeginPacketSend(length);
	if(res != 0){
		return;
	}
	enc28j60_PacketSend(buffer, length);
	
	enc28j60_EndPacketSend();
}

uint32_t enc28j60_packet_receive(uint8_t *buffer, uint32_t max_length)
{
	uint16_t pkt_cnt;
	uint16_t total_len;
	
	pkt_cnt = enc28j60_CheckPacketReceive();
	if(pkt_cnt <= 0){
		return 0;
	}	
	total_len = enc28j60_BeginPacketReceive();
	if (total_len > max_length-1){
		total_len = max_length;
   	}
	if(total_len != 0){
		enc28j60_PacketReceive(buffer, total_len);
	}
	enc28j60_EndPacketReceive();
	return total_len;
}

int32_t enc28j60_CheckPacketReceive(void)
{
	uint16_t pkt_cnt;

	pkt_cnt = enc28j60Read(EPKTCNT);
	if(pkt_cnt == 0){
		return 0;
	}
	return pkt_cnt;
}

int32_t enc28j60_BeginPacketReceive(void)
{
	uint16_t rx_status, data_length;
	enc28j60Write(ERDPTL, NextPacketPtr & 0xFF);
	enc28j60Write(ERDPTH, (NextPacketPtr)>>8);
	NextPacketPtr  = enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0);
	NextPacketPtr |= enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0)<<8;
	data_length  = enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0);
	data_length |= enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0)<<8;
	data_length -=4; //remove the CRC count
	rx_status  = enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0);
	rx_status |= enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0)<<8;
	
	if((rx_status & 0x80) == 0){
        enc28j60_EndPacketReceive();
		data_length = 0;
	}
	return data_length;
}

int32_t enc28j60_PacketReceive(uint8_t *buffer,uint32_t length)
{
	uint8_t r_tx = ENC28J60_READ_BUF_MEM;
	uint32_t size = 1;
	ENC28J60_CSL();
	SPIDev_DataTx(&r_tx, &size);	
	size = length;
#ifndef SPI_DMA_RXCH
	SPIDev_DataRx(buffer, &size);
#else
	if(size < 64){
		SPIDev_DataRx(buffer, &size);
	}else{
		SPIDev_DMA_DataRx(buffer, size);
	}
#endif
	ENC28J60_CSH();
	return 0;
}

int32_t enc28j60_EndPacketReceive(void)
{
	enc28j60Write(ERXRDPTL, NextPacketPtr & 0xFF);
	enc28j60Write(ERXRDPTH, (NextPacketPtr)>>8);
	enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, ECON2, ECON2_PKTDEC);
	return 0;
}

int32_t enc28j60_BeginPacketSend(uint32_t length)
{
    uint32_t timeout = 100;
	
	while(!enc28j60_mac_is_txReady() && --timeout);
	
	if(timeout == 0) 
      return -1;
    
	enc28j60Write(EWRPTL, TXSTART_INIT & 0xFF );
	enc28j60Write(EWRPTH, TXSTART_INIT >> 8 );

	enc28j60Write(ETXNDL, (TXSTART_INIT+length) & 0xFF );
	enc28j60Write(ETXNDH, (TXSTART_INIT+length) >> 8);

	enc28j60WriteOp(ENC28J60_WRITE_BUF_MEM, 0, 0x00);
	return 0;
}

int32_t enc28j60_PacketSend(uint8_t *buffer,uint32_t length)
{
	uint8_t r_tx = ENC28J60_WRITE_BUF_MEM;
	uint32_t size = 1;
	ENC28J60_CSL();
    SPIDev_DataTx(&r_tx, &size);
	size = length;
#ifndef SPI_DMA_TXCH
	SPIDev_DataTx(buffer, &size);
#else
	if(size < 64){
		SPIDev_DataTx(buffer, &size);
	}else{
		SPIDev_DMA_DataTx(buffer, size);
	}
#endif
    ENC28J60_CSH();
	return 0;
}

int32_t enc28j60_EndPacketSend(void)
{
	enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_TXRTS);
	if((enc28j60Read(EIR) & EIR_TXERIF)){
		if(!enc28j60_retx()) 
          return -1;
	}
	return 0;
}



