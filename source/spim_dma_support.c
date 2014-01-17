/********************************************************************************/
/*!
	@file			spim_dma_support.c
	@author         
    @version        1.00
    @date           2013.07.01
	@brief          

    @section HISTORY
		2013.12.01	V1.00	Start Here.

    @section LICENSE
		BSD License. See Copyright.txt
*/
/********************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "spim_dma_support.h"
#include "system.h"

/* Defines -------------------------------------------------------------------*/
typedef enum
{
  CSIOIDLE = 0,
  CSIOSTART,
  CSIOCOMM,
  CSIOEND,
  CSIOERR,
}CSIO_CommState;
/* Variables -----------------------------------------------------------------*/
/* Structs of SPI */
/*
 *  Data Information Struct
 */
typedef struct {
    uint32_t  Size;
    uint32_t  Count;
    uint32_t  IntState;
    uint8_t   *pData;
} Sio_DATA;

/* @brief SPI Device . */
typedef struct 
{
    /* @brief Transmit data. */
	 Sio_DATA *pDataTx;
	/* @brief Receive data. */
	 Sio_DATA *pDataRx;
	/* @brief FIFO Depth       */
	 int32_t Fifo;       
} SPI_Dev_t,*pSPI_Dev_t;

static Sio_DATA SioDataTx,SioDataRx;
static SPI_Dev_t SPI_Dev;
static pSPI_Dev_t pSPI_Dev;

/* Constants -----------------------------------------------------------------*/
typedef enum
{
    DMAC_SW_TRIG = 0,          //!< Software trigger
    DAMC_USB0_EP1_INT = 32,    //!< IDREQ0
    DAMC_USB0_EP2_INT,         //!< IDREQ1
    DAMC_USB0_EP3_INT,         //!< IDREQ2
    DAMC_USB0_EP4_INT,         //!< IDREQ3
    DAMC_USB0_EP5_INT,         //!< IDREQ4
    DMAC_ADC0_SCAN_INT,        //!< IDREQ5
    DMAC_ADC1_SCAN_INT,        //!< IDREQ6
    DMAC_ADC2_SCAN_INT,        //!< IDREQ7
    DMAC_BT0_IRQ0,             //!< IDREQ8
    DMAC_BT2_IRQ0,             //!< IDREQ9
    DMAC_BT4_IRQ0,             //!< IDREQ10
    DMAC_BT6_IRQ0,             //!< IDREQ11
    DMAC_MFS0_RX_INT,          //!< IDREQ12
    DMAC_MFS0_TX_INT,          //!< IDREQ13
    DMAC_MFS1_RX_INT,          //!< IDREQ14
    DMAC_MFS1_TX_INT,          //!< IDREQ15
    DMAC_MFS2_RX_INT,          //!< IDREQ16
    DMAC_MFS2_TX_INT,          //!< IDREQ17
    DMAC_MFS3_RX_INT,          //!< IDREQ18
    DMAC_MFS3_TX_INT,          //!< IDREQ19
    DMAC_MFS4_RX_INT,          //!< IDREQ20
    DMAC_MFS4_TX_INT,          //!< IDREQ21
    DMAC_MFS5_RX_INT,          //!< IDREQ22
    DMAC_MFS5_TX_INT,          //!< IDREQ23
    DMAC_MFS6_RX_INT,          //!< IDREQ24
    DMAC_MFS6_TX_INT,          //!< IDREQ25
    DMAC_MFS7_RX_INT,          //!< IDREQ26
    DMAC_MFS7_TX_INT,          //!< IDREQ27
    DMAC_EXT_INT_0,            //!< IDREQ28
    DMAC_EXT_INT_1,            //!< IDREQ29
    DMAC_EXT_INT_2,            //!< IDREQ30
    DMAC_EXT_INT_3,            //!< IDREQ31
}DMAC_InputSource_t;

typedef enum
{
    DMAC_BLOCK_TRSF_MODE = 0,  //!< DMAC block transfer mode
    DMAC_BURST_TRSF_MODE,      //!< DMAC burst transfer mode
    DMAC_DEMAND_TRSF_MODE,     //!< DMAC demand transfer mode
  
}DMAC_Mode_t;

typedef enum
{
    DMAC_TRST_WIDTH_8BIT = 0,  //!< 8 bit
    DMAC_TRST_WIDTH_16BIT,     //!< 16 bit
    DMAC_TRST_WIDTH_32BIT,     //!< 32 bit
    
}DMAC_TrsfWidth_t;

typedef enum
{
    DMAC_STOP_STAT_INIT = 0,       //!< initialization
    DMAC_STOP_STAT_TRSF_ERR,       //!< transfer error
    DMAC_STOP_STAT_FORCE_STOP,     //!< stop by peripheral or EB/DE bit
    DMAC_STOP_STAT_SRC_ERR,        //!< source address access error
    DMAC_STOP_STAT_DST_ERR,        //!< source address access error
    DMAC_STOP_STAT_TRSF_SUCCESS,   //!< transfer successfully
    DMAC_STOP_STAT_TRSF_PAUSE,     //!< transfer pause
    
}DMAC_StopStat_t;

#define SPI_DMACA_EB (0x80000000)
#define SPI_DMACA_PB (0x40000000)
#define SPI_DMACA_ST (0x20000000)
#define SPI_DMACA_BC(v) ((v << 16) & 0x000F0000)
#define SPI_DMACA_TC(v) ((v) & 0x0000FFFF)
#define SPI_DMACA_TXIS (DMAC_MFS7_TX_INT << 23)
#define SPI_DMACA_RXIS (DMAC_MFS7_RX_INT << 23)

#define SPI_DMACB_MS(v) ((v << 28) & 0x30000000)
#define SPI_DMACB_TW(v) ((v << 26) & 0x0C000000)
#define SPI_DMACB_FS (0x02000000)
#define SPI_DMACB_FD (0x01000000)
#define SPI_DMACB_RC (0x00800000)
#define SPI_DMACB_RS (0x00400000)
#define SPI_DMACB_RD (0x00200000)
#define SPI_DMACB_EI (0x00100000)
#define SPI_DMACB_CI (0x00080000)
#define SPI_DMACB_SS_MASK (0x00070000)
#define SPI_DMACB_EM (0x00000001)

static uint32_t dummy = 0;
/* Function prototypes -------------------------------------------------------*/

/**************************************************************************/
/*! 
    Initialize spi master.
*/
/**************************************************************************/

void SPIDev_Init(void)
{ 
	pSPI_Dev = &SPI_Dev;
	pSPI_Dev->pDataTx = &SioDataTx;
	pSPI_Dev->pDataRx = &SioDataRx;
	
    /* Bus reset sequence may be required here */
    NVIC_DisableIRQ(MFS_SPI_RX_IRQn);
    NVIC_DisableIRQ(MFS_SPI_TX_IRQn);
    
    /* Attach MFS module to I/O pads */
	ATTACH_MFS_SPI();  
    
	/* Initialize MFS as SPI */
	MFS_SPI_SCR = 0x80;		/* Disable MFS */
	MFS_SPI_SMR = 0x4F;		/* SPI mode 0, Enable SCK/SO output */
	MFS_SPI_ESCR = 0x08;		/* wait 1bit */
	SPIDev_SetBaudrate(SPI_BAUDLATE);
	MFS_SPI_SCR = 0x21;		/* Enable MFS: Set SPI/TXE */
	
#ifdef HW_FIFO
	pSPI_Dev->Fifo = HW_FIFO;
	MFS_SPI_FCR1 = 0x1C;
	MFS_SPI_FBYTE1 = 0;
	MFS_SPI_FBYTE2 = HW_FIFO;
	MFS_SPI_FCR0 = 0x0F;
#else
	pSPI_Dev->Fifo = 0;
#endif 

#ifdef SPI_DMA_RXCH
	/* Enable Rx/Error interrupts */
    //NVIC_ClearPendingIRQ(MFS_SPI_RX_IRQn);
    //NVIC_EnableIRQ(MFS_SPI_RX_IRQn);
    //NVIC_SetPriority(MFS_SPI_RX_IRQn,NVIC_EncodePriority(NVIC_PRIORITYGROUP2,2,2));
	NVIC_DisableIRQ(MFS_SPI_RX_DMA_IRQn);
	MFS_SPI_RX_DRQSEL();
	MFS_SPI_RX_DMACSA = (uint32_t)&(MFS_SPI_RDR);
	MFS_SPI_RX_DMACDA = NULL;
	MFS_SPI_RX_DMACB = SPI_DMACB_MS(DMAC_DEMAND_TRSF_MODE) | SPI_DMACB_TW(DMAC_TRST_WIDTH_8BIT);
	MFS_SPI_RX_DMACA = (SPI_DMACA_RXIS); 
	NVIC_ClearPendingIRQ(MFS_SPI_RX_DMA_IRQn);
	NVIC_EnableIRQ(MFS_SPI_RX_DMA_IRQn);
    NVIC_SetPriority(MFS_SPI_RX_DMA_IRQn,NVIC_EncodePriority(NVIC_PRIORITYGROUP2,2,2));
#endif

#ifdef SPI_DMA_TXCH   
	/* Enable Tx interrupts */
	//NVIC_ClearPendingIRQ(MFS_SPI_TX_IRQn);
	//NVIC_EnableIRQ(MFS_SPI_TX_IRQn);
	//NVIC_SetPriority(MFS_SPI_TX_IRQn,NVIC_EncodePriority(NVIC_PRIORITYGROUP2,2,3));
    NVIC_DisableIRQ(MFS_SPI_TX_DMA_IRQn);
    MFS_SPI_TX_DRQSEL();
	MFS_SPI_TX_DMACSA = NULL;
	MFS_SPI_TX_DMACDA = (uint32_t)&MFS_SPI_TDR;
    MFS_SPI_TX_DMACA = (SPI_DMACA_TXIS); 
    MFS_SPI_TX_DMACB = SPI_DMACB_MS(DMAC_DEMAND_TRSF_MODE) | SPI_DMACB_TW(DMAC_TRST_WIDTH_8BIT);
    NVIC_ClearPendingIRQ(MFS_SPI_TX_DMA_IRQn);
	NVIC_EnableIRQ(MFS_SPI_TX_DMA_IRQn);
    NVIC_SetPriority(MFS_SPI_TX_DMA_IRQn,NVIC_EncodePriority(NVIC_PRIORITYGROUP2,2,3));
#endif
    
#if defined(SPI_DMA_RXCH) || defined(SPI_DMA_TXCH)
    FM3_DMAC->DMACR   = 0x80000000; // Globally enable DMA 
#endif

}

/**************************************************************************/
/*! 
    Uninitialize spi.
*/
/**************************************************************************/

void SPIDev_Uninit(void)
{
#ifdef HW_FIFO
	MFS_SPI_FCR0 = 0x00;
#endif 
    MFS_SPI_SCR = 0x80;	 /* Disable MFS */   
    NVIC_ClearPendingIRQ(MFS_SPI_RX_IRQn);
    NVIC_ClearPendingIRQ(MFS_SPI_TX_IRQn);
    NVIC_DisableIRQ(MFS_SPI_RX_IRQn);
    NVIC_DisableIRQ(MFS_SPI_TX_IRQn);
    NVIC_SetPriority(MFS_SPI_RX_IRQn,DEFAULT_INTERRUPT_LEVEL);
    NVIC_SetPriority(MFS_SPI_TX_IRQn,DEFAULT_INTERRUPT_LEVEL);
    
	/* uninitialize data information */
    pSPI_Dev->pDataRx->Size     = 0;
    pSPI_Dev->pDataRx->Count    = 0;
    pSPI_Dev->pDataRx->IntState = 0;
    pSPI_Dev->pDataRx->pData    = NULL;
    
    pSPI_Dev->pDataTx->Size     = 0;
    pSPI_Dev->pDataTx->Count    = 0;
    pSPI_Dev->pDataTx->IntState = 0;
    pSPI_Dev->pDataTx->pData    = NULL;
	pSPI_Dev->Fifo = 0;
}

/*
 *  SpiDev_SetBaudrate
 */
int32_t  SPIDev_SetBaudrate(int32_t Baudrate)
{
    int32_t ret = 0;
	uint16_t reg_bgr;
    
    /*
     *  SIO's BGR register
     */
    /* BaudRate : round off reg_bgr to the whole number */
    reg_bgr = (uint16_t)(APB2_Clock/Baudrate) - 1;
	if(reg_bgr < 3) MFS_SPI_BGR = 3;
	else	MFS_SPI_BGR = reg_bgr;
    
    return ret;
}

/*
 *  SpiDev_BufTxRx
 */
int32_t  SPIDev_BufTxRx(void *pDataTx, void *pDataRx, uint32_t Size)
{
    int32_t ret;
    uint32_t count;
    uint8_t *p_tx;
    uint8_t *p_rx;
	uint8_t reg_ssr;
    
    /* check parameter */
    if (Size == 0) {
        return -1; 
    }
    
    ret   = 0;
    count = 0;
    p_tx  = (uint8_t*)pDataTx;
    p_rx  = (uint8_t*)pDataRx;    
	
    while (count < Size) {
        
        /* tx */
		while (!(MFS_SPI_SSR & SSR_TDRE));
		MFS_SPI_TDR = p_tx[count]; 
		//while (!(MFS_SPI_SSR & SSR_RDRF));
		//MFS_SPI_RDR;	
		do{
			reg_ssr = MFS_SPI_SSR;
		}while(!(reg_ssr & (SSR_TDRE)) || (!(reg_ssr & (SSR_TBI))));
		/* rx */
		MFS_SPI_SCR |= SCR_RXE;
		while (!(MFS_SPI_SSR & SSR_TDRE));
		MFS_SPI_TDR = 0; 
		while (!(MFS_SPI_SSR & SSR_RDRF));
		p_rx[count] = MFS_SPI_RDR;
		MFS_SPI_SCR &= ~SCR_RXE;
		count++;
    }
	
	/*
	do{
		reg_ssr = MFS_SPI_SSR;
	}while(!(reg_ssr & (SSR_TDRE)) || (!(reg_ssr & (SSR_TBI))));
    */
    return ret;
}

/*
 *  SioDev_DataTx
 */
int32_t  SPIDev_DataTx(void *pData, uint32_t *pSize)
{
    int32_t ret = 0;
#ifdef HW_FIFO
	uint32_t i;
	uint32_t num;
	uint8_t reg_fcr1;
#endif
	uint8_t reg_ssr;
	
    /* check NULL pointer */
    if ((pData == NULL) || (pSize == NULL)) {
        return -1;
    }
    
    /* check parameter */
    if (*pSize <= 0) {
        return -1; 
    }
    
	/* initialized data information */
	pSPI_Dev->pDataTx->Size     = (uint32_t)(*pSize);
	pSPI_Dev->pDataTx->Count    = 0;
	pSPI_Dev->pDataTx->IntState = 0;
	pSPI_Dev->pDataTx->pData    = pData;
	
	while(pSPI_Dev->pDataTx->Count < pSPI_Dev->pDataTx->Size){
		if (pSPI_Dev->Fifo > 0) {
#ifdef HW_FIFO
			num = pSPI_Dev->pDataTx->Size - pSPI_Dev->pDataTx->Count;
			if (num > (uint32_t)pSPI_Dev->Fifo) {
				num = (uint32_t)pSPI_Dev->Fifo;
			}
			//while (!(MFS_SPI_SSR & SSR_TDRE));
			for (i = 0; i < num; i++) {
				/* tx data to tx FIFO */
				MFS_SPI_TDR = pSPI_Dev->pDataTx->pData[pSPI_Dev->pDataTx->Count];
				pSPI_Dev->pDataTx->Count += 1;
			}		

			reg_fcr1 = MFS_SPI_FCR1 & ~(FCR1_FDRQ);
			MFS_SPI_FCR1 = reg_fcr1;

			while(!(MFS_SPI_FCR1 & FCR1_FDRQ));
			/*
			while (!(MFS_SPI_SSR & SSR_RDRF));
			while ((MFS_SPI_SSR & (SSR_RDRF))) {
				MFS_SPI_RDR;
			}
			*/
#endif
		}else{
			while (!(MFS_SPI_SSR & SSR_TDRE));
			MFS_SPI_TDR = pSPI_Dev->pDataTx->pData[pSPI_Dev->pDataTx->Count];
			pSPI_Dev->pDataTx->Count += 1; 
			//while (!(MFS_SPI_SSR & SSR_RDRF));
			//MFS_SPI_RDR;
		}	
	}
	
	/* tx is completed */ 
	do{
		reg_ssr = MFS_SPI_SSR;
	}while((!(reg_ssr & (SSR_TDRE)) || (!(reg_ssr & (SSR_TBI)))));
	
	/* check Count */
	if (pSPI_Dev->pDataTx->Count != 0) {
		/* return size */
		*pSize = (int32_t)(pSPI_Dev->pDataTx->Count);
		ret = 0;
		
	} else { /* Count == 0 */
		*pSize = 0;
		ret = -1;
		
	}
    
    return ret;
}

/*
 *  SioDev_DataRx
 */
int32_t  SPIDev_DataRx(void *pData, uint32_t *pSize)
{
    int32_t ret = 0;
	uint8_t tmp;
#ifdef HW_FIFO
	uint8_t reg_fbyte1,reg_fcr1;
#endif
    
    /* check NULL pointer */
    if ((pData == NULL) || (pSize == NULL)) {
        return -1; 
    }
    
    /* check parameter */
    if (*pSize <= 0) {
        return -1; 
    }
   
	/* initialized data information */
	pSPI_Dev->pDataRx->Size     = (uint32_t)(*pSize);
	pSPI_Dev->pDataRx->Count    = 0;
	pSPI_Dev->pDataRx->IntState = 0;
	pSPI_Dev->pDataRx->pData    = pData;
        
	tmp = (uint8_t)MFS_SPI_RDR;
	tmp = MFS_SPI_SSR | SSR_REC;
	MFS_SPI_SSR = tmp;
	
	MFS_SPI_SCR |= SCR_RXE;
	MFS_SPI_SMR &= ~SMR_SOE;
	
	while(pSPI_Dev->pDataRx->Count < pSPI_Dev->pDataRx->Size){	
		if (pSPI_Dev->Fifo > 0) {
#ifdef HW_FIFO
			//while (!(MFS_SPI_SSR & SSR_TDRE));

			if (pSPI_Dev->pDataRx->Size - pSPI_Dev->pDataRx->Count >= (uint32_t)pSPI_Dev->Fifo) {
				reg_fbyte1 = (uint8_t)(pSPI_Dev->Fifo);
			} else {
				reg_fbyte1 = (uint8_t)(pSPI_Dev->pDataRx->Size - pSPI_Dev->pDataRx->Count);
			}
			MFS_SPI_FBYTE1 = reg_fbyte1;
	
			/* FDRQ = 0 */
			reg_fcr1 = MFS_SPI_FCR1 & ~(FCR1_FDRQ);
			MFS_SPI_FCR1 = reg_fcr1;
       
			while(!(MFS_SPI_FCR1 & FCR1_FDRQ));
 
			while (!(MFS_SPI_SSR & SSR_RDRF));
			while ((MFS_SPI_SSR & (SSR_RDRF))) {
				/* rx data from the FIFO */
				pSPI_Dev->pDataRx->pData[pSPI_Dev->pDataRx->Count] = (uint8_t)MFS_SPI_RDR;
				pSPI_Dev->pDataRx->Count += 1;
			}
#endif 
		}else{
			while (!(MFS_SPI_SSR & SSR_TDRE));
			MFS_SPI_TDR = 0x00;
			while (!(MFS_SPI_SSR & SSR_RDRF));
			pSPI_Dev->pDataRx->pData[pSPI_Dev->pDataRx->Count] = (uint8_t)MFS_SPI_RDR;
			pSPI_Dev->pDataRx->Count += 1;
		}
	}
	
	MFS_SPI_SCR &= ~SCR_RXE;
	MFS_SPI_SMR |= SMR_SOE;
        
	/* check Count */
	if (pSPI_Dev->pDataRx->Count != 0) {
		/* return size */
		*pSize = (int32_t)(pSPI_Dev->pDataRx->Count);
		ret = 0;
		
	} else { /* Count == 0 */
		*pSize = 0;
		ret = -1;
		
	}
  
    return ret;
}

#ifdef SPI_DMA_RXCH
void MFS_SPI_RX_IRQHandler(void)
{

}

void MFS_SPI_RX_DMA_IRQHander(void)
{    
	uint8_t stat;
	stat = (MFS_SPI_RX_DMACB & SPI_DMACB_SS_MASK) >> 16;
    switch (stat)
    {
        case DMAC_STOP_STAT_TRSF_ERR:    
        case DMAC_STOP_STAT_FORCE_STOP:
        case DMAC_STOP_STAT_SRC_ERR:
        case DMAC_STOP_STAT_DST_ERR:
			/* transfer err */
            pSPI_Dev->pDataRx->IntState = 2;
            break;
        case DMAC_STOP_STAT_TRSF_SUCCESS:
			/* transfer complete */
            pSPI_Dev->pDataRx->IntState = 1;
            break;
        default:
            break;
        
    }
    MFS_SPI_RX_DMACB &= ~(7ul<<16); /* Clear interrupt flag */
}

int32_t  SPIDev_DMA_DataRx(void *pData, uint32_t Size)
{
	int32_t ret = 0;
	uint8_t tmp;
    
    /* check NULL pointer & parameter */
    if ((pData == NULL) || (Size <= 0)) {
        return -1;
    }
   
	/* initialized data information */
	pSPI_Dev->pDataRx->Size     = (uint32_t)(Size);
	pSPI_Dev->pDataRx->Count    = 0;
	pSPI_Dev->pDataRx->IntState = 0;
	pSPI_Dev->pDataRx->pData    = pData;
	
	pSPI_Dev->pDataTx->Size     = (uint32_t)(Size);
	pSPI_Dev->pDataTx->Count    = 0;
	pSPI_Dev->pDataTx->IntState = 0;
	pSPI_Dev->pDataTx->pData    = (uint8_t *)&dummy;
        
	tmp = (uint8_t)MFS_SPI_RDR;
	tmp = MFS_SPI_SSR | SSR_REC;
	MFS_SPI_SSR = tmp;

#ifdef HW_FIFO	
	MFS_SPI_FCR0 &= ~0x0F;
#endif
	MFS_SPI_SMR &= ~SMR_SOE;
	MFS_SPI_SCR |= SCR_RXE;
		
	MFS_SPI_RX_DMACSA = (uint32_t)&(MFS_SPI_RDR);
	MFS_SPI_RX_DMACDA = (uint32_t)(pSPI_Dev->pDataRx->pData);
	MFS_SPI_RX_DMACB = SPI_DMACB_MS(DMAC_DEMAND_TRSF_MODE) | SPI_DMACB_TW(DMAC_TRST_WIDTH_8BIT) | SPI_DMACB_FS | SPI_DMACB_RS | SPI_DMACB_EI | SPI_DMACB_CI;
	MFS_SPI_RX_DMACA = (SPI_DMACA_EB | SPI_DMACA_RXIS | SPI_DMACA_BC(0) | SPI_DMACA_TC(pSPI_Dev->pDataRx->Size - 1)); 
	
	MFS_SPI_TX_DMACSA = (uint32_t)(pSPI_Dev->pDataTx->pData);
	MFS_SPI_TX_DMACDA = (uint32_t)&MFS_SPI_TDR;
	MFS_SPI_TX_DMACB = SPI_DMACB_MS(DMAC_DEMAND_TRSF_MODE) | SPI_DMACB_TW(DMAC_TRST_WIDTH_8BIT) | SPI_DMACB_FS |  SPI_DMACB_RS | SPI_DMACB_FD |  SPI_DMACB_RD;
	MFS_SPI_TX_DMACA = (SPI_DMACA_EB | SPI_DMACA_TXIS | SPI_DMACA_BC(0) | SPI_DMACA_TC(pSPI_Dev->pDataTx->Size - 1)); 
	
	MFS_SPI_SCR |= SCR_RIE | SCR_TIE;
	
	while(pSPI_Dev->pDataRx->IntState == 0);
	
	MFS_SPI_SCR &= ~(SCR_RXE | SCR_RIE | SCR_TIE);
	
	MFS_SPI_SMR |= SMR_SOE;
#ifdef HW_FIFO
	MFS_SPI_FCR0 |= 0x0F;
#endif	
        
	/* check int state */
	if(pSPI_Dev->pDataRx->IntState == 1){
		ret = 0;	
	}else{
		ret = -1;
	}
	
    return ret;
}
#endif

#ifdef SPI_DMA_TXCH
void MFS_SPI_TX_IRQHandler(void)
{

}

void MFS_SPI_TX_DMA_IRQHander(void)
{    
	uint8_t stat;
	stat = (MFS_SPI_TX_DMACB & SPI_DMACB_SS_MASK) >> 16;
    switch (stat)
    {
        case DMAC_STOP_STAT_TRSF_ERR:    
        case DMAC_STOP_STAT_FORCE_STOP:
        case DMAC_STOP_STAT_SRC_ERR:
        case DMAC_STOP_STAT_DST_ERR:
			/* transfer err */
            pSPI_Dev->pDataTx->IntState = 2;
            break;
        case DMAC_STOP_STAT_TRSF_SUCCESS:
			/* transfer complete */
            pSPI_Dev->pDataTx->IntState = 1;
            break;
        default:
            break;
        
    }
    MFS_SPI_TX_DMACB &= ~(7ul<<16); /* Clear interrupt flag */
}

int32_t  SPIDev_DMA_DataTx(void *pData, uint32_t Size)
{
	int32_t ret = 0;
	
    /* check NULL pointer & parameter */
    if ((pData == NULL) || (Size <= 0)) {
        return -1;
    }
    
	/* initialized data information */
	pSPI_Dev->pDataTx->Size     = (uint32_t)(Size);
	pSPI_Dev->pDataTx->Count    = 0;
	pSPI_Dev->pDataTx->IntState = 0;
	pSPI_Dev->pDataTx->pData    = pData;
	
	MFS_SPI_TX_DMACSA = (uint32_t)(pSPI_Dev->pDataTx->pData);
	MFS_SPI_TX_DMACDA = (uint32_t)&MFS_SPI_TDR;
	MFS_SPI_TX_DMACB = SPI_DMACB_MS(DMAC_DEMAND_TRSF_MODE) | SPI_DMACB_TW(DMAC_TRST_WIDTH_8BIT) | SPI_DMACB_FD |  SPI_DMACB_RD | SPI_DMACB_EI | SPI_DMACB_CI;
	MFS_SPI_TX_DMACA = (SPI_DMACA_EB | SPI_DMACA_TXIS | SPI_DMACA_BC(0) |SPI_DMACA_TC(pSPI_Dev->pDataTx->Size - 1)); 
	
	/* wait tx completed */ 

	MFS_SPI_SCR |= SCR_TIE ;

	//while(!(MFS_SPI_TX_DMACB & SPI_DMACB_SS_MASK));
	while(pSPI_Dev->pDataTx->IntState == 0);
	
	MFS_SPI_SCR &= ~(SCR_TIE);
	
	/* check int state */
	if(pSPI_Dev->pDataTx->IntState == 1){
		ret = 0;	
	}else{
		ret = -1;
	}
   
    return ret;
}
#endif


/* End Of File ---------------------------------------------------------------*/
