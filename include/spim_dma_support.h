/********************************************************************************/
/*!
	@file			spim_dma_support.h
	@author         
    @version        1.00
    @date           2013.07.01
	@brief          

    @section HISTORY
		2013.07.01	V1.00	Start Here.

    @section LICENSE
		BSD License. See Copyright.txt
*/
/********************************************************************************/
#ifndef __SPI_DMA_SUPPORT_H
#define __SPI_DMA_SUPPORT_H

#ifdef __cplusplus
 extern "C" {
#endif

/* General Inclusion */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>

/* uC Specific Inclusion */
#include "mcu.h"

/* SPI Definition */

#define SPI_POLARITY			1			/* SPI polarity high : 0 ,low : 1 */
#define SPI_BITODER				1         	/* SPI bit oder LSB : 0 ,MSB : 1 */
#define SPI_BAUDLATE			4000000UL	/* SPI Baudrate */
#define SPI_MFS					7			/* MFS channel to be used as SPI (0-3,4-7) */
#define SPI_LOC					1			/* Pin relocation number (0-2) */
/* comment dma function */
#define SPI_DMA_TXCH            3           /* SPI TX DMA Channel */
#define SPI_DMA_RXCH            2           /* SPI RX DMA Channel */

#if SPI_MFS >= 4
#define SPI_FIFO_MODE 1
#if SPI_FIFO_MODE == 1
#define HW_FIFO 8
#endif
#endif

#if   SPI_MFS == 0
#define MFS_SPI_RX_IRQHandler 	MFS0RX_IRQHandler
#define MFS_SPI_TX_IRQHandler 	MFS0TX_IRQHandler
#define MFS_SPI_RX_IRQn 		MFS0RX_IRQn
#define MFS_SPI_TX_IRQn 		MFS0TX_IRQn
#define MFS_SPI_SMR   			FM3_MFS0_CSIO->SMR
#define MFS_SPI_SCR  		 	FM3_MFS0_CSIO->SCR
#define MFS_SPI_ESCR  			FM3_MFS0_CSIO->ESCR
#define MFS_SPI_SSR   			FM3_MFS0_CSIO->SSR
#define MFS_SPI_TDR   			FM3_MFS0_CSIO->TDR
#define MFS_SPI_RDR   			FM3_MFS0_CSIO->RDR
#define MFS_SPI_BGR   			FM3_MFS0_CSIO->BGR
#if   SPI_LOC == 0
#define ATTACH_MFS_SPI() { FM3_GPIO->EPFR07 = (FM3_GPIO->EPFR07 & ~(0x3F << 4)) | (0x14 << 4);  FM3_GPIO->PFR2 |= 0x07 << 1; }
#elif SPI_LOC == 1
#define ATTACH_MFS_SPI() { FM3_GPIO->EPFR07 = (FM3_GPIO->EPFR07 & ~(0x3F << 4)) | (0x2A << 4); FM3_GPIO->PFR1 |= 0x07 << 4; FM3_GPIO->ADE &= ~(0x07 << 4);}
#elif SPI_LOC == 2
#define ATTACH_MFS_SPI() { FM3_GPIO->EPFR07 = (FM3_GPIO->EPFR07 & ~(0x3F << 4)) | (0x3F << 4); FM3_GPIO->PFRB |= 0x07 << 4; }
#endif

#elif SPI_MFS == 1
#define MFS_SPI_RX_IRQHandler	MFS1RX_IRQHandler
#define MFS_SPI_TX_IRQHandler	MFS1TX_IRQHandler
#define MFS_SPI_RX_IRQn 		MFS1RX_IRQn
#define MFS_SPI_TX_IRQn 		MFS1TX_IRQn
#define MFS_SPI_SMR   			FM3_MFS1_CSIO->SMR
#define MFS_SPI_SCR   			FM3_MFS1_CSIO->SCR
#define MFS_SPI_ESCR  			FM3_MFS1_CSIO->ESCR
#define MFS_SPI_SSR   			FM3_MFS1_CSIO->SSR
#define MFS_SPI_TDR   			FM3_MFS1_CSIO->TDR
#define MFS_SPI_RDR   			FM3_MFS1_CSIO->RDR
#define MFS_SPI_BGR   			FM3_MFS1_CSIO->BGR
#if   SPI_LOC == 0
#define ATTACH_MFS_SPI() { FM3_GPIO->EPFR07 = (FM3_GPIO->EPFR07 & ~(0x3F << 10)) | (0x14 << 10);  FM3_GPIO->PFR5 |= 0x07 << 6; }
#elif SPI_LOC == 1
#define ATTACH_MFS_SPI() { FM3_GPIO->EPFR07 = (FM3_GPIO->EPFR07 & ~(0x3F << 10)) | (0x2A << 10); FM3_GPIO->PFR1 |= 0x07 << 1; FM3_GPIO->ADE &= ~(0x07 << 1);}
#elif SPI_LOC == 2
#define ATTACH_MFS_SPI() { FM3_GPIO->EPFR07 = (FM3_GPIO->EPFR07 & ~(0x3F << 10)) | (0x3F << 10); FM3_GPIO->PFRF |= 0x07 << 0; }
#endif

#elif SPI_MFS == 2
#define MFS_SPI_RX_IRQHandler	MFS2RX_IRQHandler
#define MFS_SPI_TX_IRQHandler	MFS2TX_IRQHandler
#define MFS_SPI_RX_IRQn 		MFS2RX_IRQn
#define MFS_SPI_TX_IRQn 		MFS2TX_IRQn
#define MFS_SPI_SMR   			FM3_MFS2_CSIO->SMR
#define MFS_SPI_SCR   			FM3_MFS2_CSIO->SCR
#define MFS_SPI_ESCR  			FM3_MFS2_CSIO->ESCR
#define MFS_SPI_SSR   			FM3_MFS2_CSIO->SSR
#define MFS_SPI_TDR   			FM3_MFS2_CSIO->TDR
#define MFS_SPI_RDR   			FM3_MFS2_CSIO->RDR
#define MFS_SPI_BGR   			FM3_MFS2_CSIO->BGR
#if   SPI_LOC == 0
#define ATTACH_MFS_SPI() { FM3_GPIO->EPFR07 = (FM3_GPIO->EPFR07 & ~(0x3F << 16)) | (0x14 << 16);  FM3_GPIO->PFR7 |= 0x07 << 2; }
#elif SPI_LOC == 1
#define ATTACH_MFS_SPI() { FM3_GPIO->EPFR07 = (FM3_GPIO->EPFR07 & ~(0x3F << 16)) | (0x2A << 16); FM3_GPIO->PFR2 |= 0x07 << 4; }
#elif SPI_LOC == 2
#define ATTACH_MFS_SPI() { FM3_GPIO->EPFR07 = (FM3_GPIO->EPFR07 & ~(0x3F << 16)) | (0x3F << 16); FM3_GPIO->PFR1 |= 0x07 << 7; FM3_GPIO->ADE &= ~(0x07 << 7);}
#endif

#elif SPI_MFS == 3
#define MFS_SPI_RX_IRQHandler	MFS3RX_IRQHandler
#define MFS_SPI_TX_IRQHandler	MFS3TX_IRQHandler
#define MFS_SPI_RX_IRQn 		MFS3RX_IRQn
#define MFS_SPI_TX_IRQn 		MFS3TX_IRQn
#define MFS_SPI_SMR   			FM3_MFS3_CSIO->SMR
#define MFS_SPI_SCR   			FM3_MFS3_CSIO->SCR
#define MFS_SPI_ESCR  			FM3_MFS3_CSIO->ESCR
#define MFS_SPI_SSR   			FM3_MFS3_CSIO->SSR
#define MFS_SPI_TDR   			FM3_MFS3_CSIO->TDR
#define MFS_SPI_RDR   			FM3_MFS3_CSIO->RDR
#define MFS_SPI_BGR   			FM3_MFS3_CSIO->BGR
#if   SPI_LOC == 0
#define ATTACH_MFS_SPI() { FM3_GPIO->EPFR07 = (FM3_GPIO->EPFR07 & ~(0x3F << 22)) | (0x14 << 22);  FM3_GPIO->PFR7 |= 0x07 << 5; }
#elif SPI_LOC == 1
#define ATTACH_MFS_SPI() { FM3_GPIO->EPFR07 = (FM3_GPIO->EPFR07 & ~(0x3F << 22)) | (0x2A << 22); FM3_GPIO->PFR5 |= 0x07 << 0; }
#elif SPI_LOC == 2
#define ATTACH_MFS_SPI() { FM3_GPIO->EPFR07 = (FM3_GPIO->EPFR07 & ~(0x3F << 22)) | (0x3F << 22); FM3_GPIO->PFR4 |= 0x07 << 8; }
#endif

#elif SPI_MFS == 4
#define MFS_SPI_RX_IRQHandler	MFS4RX_IRQHandler
#define MFS_SPI_TX_IRQHandler	MFS4TX_IRQHandler
#define MFS_SPI_RX_IRQn 		MFS4RX_IRQn
#define MFS_SPI_TX_IRQn 		MFS4TX_IRQn
#define MFS_SPI_SMR   			FM3_MFS4_CSIO->SMR
#define MFS_SPI_SCR   			FM3_MFS4_CSIO->SCR
#define MFS_SPI_ESCR  			FM3_MFS4_CSIO->ESCR
#define MFS_SPI_SSR   			FM3_MFS4_CSIO->SSR
#define MFS_SPI_TDR   			FM3_MFS4_CSIO->TDR
#define MFS_SPI_RDR   			FM3_MFS4_CSIO->RDR
#define MFS_SPI_BGR   			FM3_MFS4_CSIO->BGR
#define MFS_SPI_FCR   			FM3_MFS4_CSIO->FCR
#define MFS_SPI_FCR0  			FM3_MFS4_CSIO->FCR0
#define MFS_SPI_FCR1  			FM3_MFS4_CSIO->FCR1
#define MFS_SPI_FBYTE 			FM3_MFS4_CSIO->FBYTE
#define MFS_SPI_FBYTE1 			FM3_MFS4_CSIO->FBYTE1
#define MFS_SPI_FBYTE2 			FM3_MFS4_CSIO->FBYTE2
#if   SPI_LOC == 0
#define ATTACH_MFS_SPI() { FM3_GPIO->EPFR08 = (FM3_GPIO->EPFR08 & ~(0x3F << 4)) | (0x14 << 4);  FM3_GPIO->PFR0 |= 0x07 << 10; }
#elif SPI_LOC == 1
#define ATTACH_MFS_SPI() { FM3_GPIO->EPFR08 = (FM3_GPIO->EPFR08 & ~(0x3F << 4)) | (0x2A << 4); FM3_GPIO->PFR1 |= 0x07 << 10; FM3_GPIO->ADE &= ~(0x07 << 10);}
#elif SPI_LOC == 2
#define ATTACH_MFS_SPI() { FM3_GPIO->EPFR08 = (FM3_GPIO->EPFR08 & ~(0x3F << 4)) | (0x3F << 4); FM3_GPIO->PFR0 |= 0x07 << 5; }
#endif

#elif SPI_MFS == 5
#define MFS_SPI_RX_IRQHandler	MFS5RX_IRQHandler
#define MFS_SPI_TX_IRQHandler	MFS5TX_IRQHandler
#define MFS_SPI_RX_IRQn 		MFS5RX_IRQn
#define MFS_SPI_TX_IRQn 		MFS5TX_IRQn
#define MFS_SPI_SMR   			FM3_MFS5_CSIO->SMR
#define MFS_SPI_SCR   			FM3_MFS5_CSIO->SCR
#define MFS_SPI_ESCR 			FM3_MFS5_CSIO->ESCR
#define MFS_SPI_SSR  			FM3_MFS5_CSIO->SSR
#define MFS_SPI_TDR  			FM3_MFS5_CSIO->TDR
#define MFS_SPI_RDR   			FM3_MFS5_CSIO->RDR
#define MFS_SPI_BGR   			FM3_MFS5_CSIO->BGR
#define MFS_SPI_FCR   			FM3_MFS5_CSIO->FCR
#define MFS_SPI_FCR0  			FM3_MFS5_CSIO->FCR0
#define MFS_SPI_FCR1  			FM3_MFS5_CSIO->FCR1
#define MFS_SPI_FBYTE 			FM3_MFS5_CSIO->FBYTE
#define MFS_SPI_FBYTE1 			FM3_MFS5_CSIO->FBYTE1
#define MFS_SPI_FBYTE2 			FM3_MFS5_CSIO->FBYTE2
#if   SPI_LOC == 0
#define ATTACH_MFS_SPI() { FM3_GPIO->EPFR08 = (FM3_GPIO->EPFR08 & ~(0x3F << 10)) | (0x14 << 10);  FM3_GPIO->PFR6 |= 0x07 << 0; }
#elif SPI_LOC == 1
#define ATTACH_MFS_SPI() { FM3_GPIO->EPFR08 = (FM3_GPIO->EPFR08 & ~(0x3F << 10)) | (0x2A << 10); FM3_GPIO->PFR9 |= 0x07 << 2; }
#elif SPI_LOC == 2
#define ATTACH_MFS_SPI() { FM3_GPIO->EPFR08 = (FM3_GPIO->EPFR08 & ~(0x3F << 10)) | (0x3F << 10); FM3_GPIO->PFR3 |= 0x07 << 6; }
#endif

#elif SPI_MFS == 6
#define MFS_SPI_RX_IRQHandler	MFS6RX_IRQHandler
#define MFS_SPI_TX_IRQHandler	MFS6TX_IRQHandler
#define MFS_SPI_RX_IRQn 		MFS6RX_IRQn
#define MFS_SPI_TX_IRQn 		MFS6TX_IRQn
#define MFS_SPI_SMR   			FM3_MFS6_CSIO->SMR
#define MFS_SPI_SCR   			FM3_MFS6_CSIO->SCR
#define MFS_SPI_ESCR  			FM3_MFS6_CSIO->ESCR
#define MFS_SPI_SSR   			FM3_MFS6_CSIO->SSR
#define MFS_SPI_TDR   			FM3_MFS6_CSIO->TDR
#define MFS_SPI_RDR   			FM3_MFS6_CSIO->RDR
#define MFS_SPI_BGR   			FM3_MFS6_CSIO->BGR
#define MFS_SPI_FCR   			FM3_MFS6_CSIO->FCR
#define MFS_SPI_FCR0  			FM3_MFS6_CSIO->FCR0
#define MFS_SPI_FCR1  			FM3_MFS6_CSIO->FCR1
#define MFS_SPI_FBYTE 			FM3_MFS6_CSIO->FBYTE
#define MFS_SPI_FBYTE1 			FM3_MFS6_CSIO->FBYTE1
#define MFS_SPI_FBYTE2 			FM3_MFS6_CSIO->FBYTE2
#if   SPI_LOC == 0
#define ATTACH_MFS_SPI() { FM3_GPIO->EPFR08 = (FM3_GPIO->EPFR08 & ~(0x3F << 16)) | (0x14 << 16);  FM3_GPIO->PFR5 |= 0x07 << 3; }
#elif SPI_LOC == 1
#define ATTACH_MFS_SPI() { FM3_GPIO->EPFR08 = (FM3_GPIO->EPFR08 & ~(0x3F << 16)) | (0x2A << 16); FM3_GPIO->PFR3 |= 0x07 << 2; }
#elif SPI_LOC == 2
#define ATTACH_MFS_SPI() { FM3_GPIO->EPFR08 = (FM3_GPIO->EPFR08 & ~(0x3F << 16)) | (0x3F << 16); FM3_GPIO->PFRF |= 0x07 << 3; }
#endif

#elif SPI_MFS == 7
#define MFS_SPI_RX_IRQHandler	MFS7RX_IRQHandler
#define MFS_SPI_TX_IRQHandler	MFS7TX_IRQHandler
#define MFS_SPI_RX_IRQn 		MFS7RX_IRQn
#define MFS_SPI_TX_IRQn 		MFS7TX_IRQn
#define MFS_SPI_SMR   			FM3_MFS7_CSIO->SMR
#define MFS_SPI_SCR  			FM3_MFS7_CSIO->SCR
#define MFS_SPI_ESCR  			FM3_MFS7_CSIO->ESCR
#define MFS_SPI_SSR   			FM3_MFS7_CSIO->SSR
#define MFS_SPI_TDR   			FM3_MFS7_CSIO->TDR
#define MFS_SPI_RDR   			FM3_MFS7_CSIO->RDR
#define MFS_SPI_BGR   			FM3_MFS7_CSIO->BGR
#define MFS_SPI_FCR   			FM3_MFS7_CSIO->FCR
#define MFS_SPI_FCR0  			FM3_MFS7_CSIO->FCR0
#define MFS_SPI_FCR1  			FM3_MFS7_CSIO->FCR1
#define MFS_SPI_FBYTE 			FM3_MFS7_CSIO->FBYTE
#define MFS_SPI_FBYTE1 			FM3_MFS7_CSIO->FBYTE1
#define MFS_SPI_FBYTE2 			FM3_MFS7_CSIO->FBYTE2
#if   SPI_LOC == 0
#define ATTACH_MFS_SPI() { FM3_GPIO->EPFR08 = (FM3_GPIO->EPFR08 & ~(0x3F << 22)) | (0x14 << 22);  FM3_GPIO->PFR5 |= 0x07 << 9; }
#elif SPI_LOC == 1
#define ATTACH_MFS_SPI() { FM3_GPIO->EPFR08 = (FM3_GPIO->EPFR08 & ~(0x3F << 22)) | (0x2A << 22); FM3_GPIO->PFR4 |= 0x07 << 12; }
#elif SPI_LOC == 2
#define ATTACH_MFS_SPI() { FM3_GPIO->EPFR08 = (FM3_GPIO->EPFR08 & ~(0x3F << 22)) | (0x3F << 22); FM3_GPIO->PFRB |= 0x07 << 0; }
#endif

#endif


#ifdef SPI_DMA_TXCH
#if SPI_DMA_TXCH == 0
#define MFS_SPI_TX_DMA_IRQHander   DMAC0_Handler 
#define MFS_SPI_TX_DMA_IRQn        DMAC0_IRQn
#define MFS_SPI_TX_DMACA           FM3_DMAC->DMACA0
#define MFS_SPI_TX_DMACB           FM3_DMAC->DMACB0
#define MFS_SPI_TX_DMACSA          FM3_DMAC->DMACSA0
#define MFS_SPI_TX_DMACDA          FM3_DMAC->DMACDA0
#elif SPI_DMA_TXCH == 1
#define MFS_SPI_TX_DMA_IRQHander   DMAC1_Handler 
#define MFS_SPI_TX_DMA_IRQn        DMAC1_IRQn
#define MFS_SPI_TX_DMACA           FM3_DMAC->DMACA1
#define MFS_SPI_TX_DMACB           FM3_DMAC->DMACB1
#define MFS_SPI_TX_DMACSA          FM3_DMAC->DMACSA1
#define MFS_SPI_TX_DMACDA          FM3_DMAC->DMACDA1
#elif SPI_DMA_TXCH == 2
#define MFS_SPI_TX_DMA_IRQHander   DMAC2_Handler 
#define MFS_SPI_TX_DMA_IRQn        DMAC2_IRQn
#define MFS_SPI_TX_DMACA           FM3_DMAC->DMACA2
#define MFS_SPI_TX_DMACB           FM3_DMAC->DMACB2
#define MFS_SPI_TX_DMACSA          FM3_DMAC->DMACSA2
#define MFS_SPI_TX_DMACDA          FM3_DMAC->DMACDA2
#elif SPI_DMA_TXCH == 3
#define MFS_SPI_TX_DMA_IRQHander   DMAC3_Handler 
#define MFS_SPI_TX_DMA_IRQn        DMAC3_IRQn
#define MFS_SPI_TX_DMACA           FM3_DMAC->DMACA3
#define MFS_SPI_TX_DMACB           FM3_DMAC->DMACB3
#define MFS_SPI_TX_DMACSA          FM3_DMAC->DMACSA3
#define MFS_SPI_TX_DMACDA          FM3_DMAC->DMACDA3
#elif SPI_DMA_TXCH == 4
#define MFS_SPI_TX_DMA_IRQHander   DMAC4_Handler 
#define MFS_SPI_TX_DMA_IRQn        DMAC4_IRQn
#define MFS_SPI_TX_DMACA           FM3_DMAC->DMACA4
#define MFS_SPI_TX_DMACB           FM3_DMAC->DMACB4
#define MFS_SPI_TX_DMACSA          FM3_DMAC->DMACSA4
#define MFS_SPI_TX_DMACDA          FM3_DMAC->DMACDA4
#elif SPI_DMA_TXCH == 5
#define MFS_SPI_TX_DMA_IRQHander   DMAC5_Handler 
#define MFS_SPI_TX_DMA_IRQn        DMAC5_IRQn
#define MFS_SPI_TX_DMACA           FM3_DMAC->DMACA5
#define MFS_SPI_TX_DMACB           FM3_DMAC->DMACB5
#define MFS_SPI_TX_DMACSA          FM3_DMAC->DMACSA5
#define MFS_SPI_TX_DMACDA          FM3_DMAC->DMACDA5
#elif SPI_DMA_TXCH == 6
#define MFS_SPI_TX_DMA_IRQHander   DMAC6_Handler 
#define MFS_SPI_TX_DMA_IRQn        DMAC6_IRQn
#define MFS_SPI_TX_DMACA           FM3_DMAC->DMACA6
#define MFS_SPI_TX_DMACB           FM3_DMAC->DMACB6
#define MFS_SPI_TX_DMACSA          FM3_DMAC->DMACSA6
#define MFS_SPI_TX_DMACDA          FM3_DMAC->DMACDA6
#elif SPI_DMA_TXCH == 7
#define MFS_SPI_TX_DMA_IRQHander   DMAC7_Handler 
#define MFS_SPI_TX_DMA_IRQn        DMAC7_IRQn
#define MFS_SPI_TX_DMACA           FM3_DMAC->DMACA7
#define MFS_SPI_TX_DMACB           FM3_DMAC->DMACB7
#define MFS_SPI_TX_DMACSA          FM3_DMAC->DMACSA7
#define MFS_SPI_TX_DMACDA          FM3_DMAC->DMACDA7
#endif
   
#if SPI_MFS == 0
#define MFS_SPI_TX_DRQSEL()		  {FM3_INTREQ->DRQSEL = (FM3_INTREQ->DRQSEL & ~(1 << 13)) | (1 << 13);}      
#elif SPI_MFS == 1
#define MFS_SPI_TX_DRQSEL()       {FM3_INTREQ->DRQSEL = (FM3_INTREQ->DRQSEL & ~(1 << 15)) | (1 << 15);} 
#elif SPI_MFS == 2
#define MFS_SPI_TX_DRQSEL()       {FM3_INTREQ->DRQSEL = (FM3_INTREQ->DRQSEL & ~(1 << 17)) | (1 << 17);} 
#elif SPI_MFS == 3
#define MFS_SPI_TX_DRQSEL()       {FM3_INTREQ->DRQSEL = (FM3_INTREQ->DRQSEL & ~(1 << 19)) | (1 << 19);} 
#elif SPI_MFS == 4
#define MFS_SPI_TX_DRQSEL()       {FM3_INTREQ->DRQSEL = (FM3_INTREQ->DRQSEL & ~(1 << 21)) | (1 << 21);} 
#elif SPI_MFS == 5
#define MFS_SPI_TX_DRQSEL()       {FM3_INTREQ->DRQSEL = (FM3_INTREQ->DRQSEL & ~(1 << 23)) | (1 << 23);} 
#elif SPI_MFS == 6
#define MFS_SPI_TX_DRQSEL()       {FM3_INTREQ->DRQSEL = (FM3_INTREQ->DRQSEL & ~(1 << 25)) | (1 << 25);} 
#elif SPI_MFS == 7
#define MFS_SPI_TX_DRQSEL()       {FM3_INTREQ->DRQSEL = (FM3_INTREQ->DRQSEL & ~(1 << 27)) | (1 << 27);} 
#endif
   
#endif
   
#ifdef SPI_DMA_RXCH
#if SPI_DMA_RXCH == 0
#define MFS_SPI_RX_DMA_IRQHander   DMAC0_Handler 
#define MFS_SPI_RX_DMA_IRQn        DMAC0_IRQn
#define MFS_SPI_RX_DMACA           FM3_DMAC->DMACA0
#define MFS_SPI_RX_DMACB           FM3_DMAC->DMACB0
#define MFS_SPI_RX_DMACSA          FM3_DMAC->DMACSA0
#define MFS_SPI_RX_DMACDA          FM3_DMAC->DMACDA0
#elif SPI_DMA_RXCH == 1
#define MFS_SPI_RX_DMA_IRQHander   DMAC1_Handler 
#define MFS_SPI_RX_DMA_IRQn        DMAC1_IRQn
#define MFS_SPI_RX_DMACA           FM3_DMAC->DMACA1
#define MFS_SPI_RX_DMACB           FM3_DMAC->DMACB1
#define MFS_SPI_RX_DMACSA          FM3_DMAC->DMACSA1
#define MFS_SPI_RX_DMACDA          FM3_DMAC->DMACDA1
#elif SPI_DMA_RXCH == 2
#define MFS_SPI_RX_DMA_IRQHander   DMAC2_Handler 
#define MFS_SPI_RX_DMA_IRQn        DMAC2_IRQn
#define MFS_SPI_RX_DMACA           FM3_DMAC->DMACA2
#define MFS_SPI_RX_DMACB           FM3_DMAC->DMACB2
#define MFS_SPI_RX_DMACSA          FM3_DMAC->DMACSA2
#define MFS_SPI_RX_DMACDA          FM3_DMAC->DMACDA2
#elif SPI_DMA_RXCH == 3
#define MFS_SPI_RX_DMA_IRQHander   DMAC3_Handler 
#define MFS_SPI_RX_DMA_IRQn        DMAC3_IRQn
#define MFS_SPI_RX_DMACA           FM3_DMAC->DMACA3
#define MFS_SPI_RX_DMACB           FM3_DMAC->DMACB3
#define MFS_SPI_RX_DMACSA          FM3_DMAC->DMACSA3
#define MFS_SPI_RX_DMACDA          FM3_DMAC->DMACDA3
#elif SPI_DMA_RXCH == 4
#define MFS_SPI_RX_DMA_IRQHander   DMAC4_Handler 
#define MFS_SPI_RX_DMA_IRQn        DMAC4_IRQn
#define MFS_SPI_RX_DMACA           FM3_DMAC->DMACA4
#define MFS_SPI_RX_DMACB           FM3_DMAC->DMACB4
#define MFS_SPI_RX_DMACSA          FM3_DMAC->DMACSA4
#define MFS_SPI_RX_DMACDA          FM3_DMAC->DMACDA4
#elif SPI_DMA_RXCH == 5
#define MFS_SPI_RX_DMA_IRQHander   DMAC5_Handler 
#define MFS_SPI_RX_DMA_IRQn        DMAC5_IRQn
#define MFS_SPI_RX_DMACA           FM3_DMAC->DMACA5
#define MFS_SPI_RX_DMACB           FM3_DMAC->DMACB5
#define MFS_SPI_RX_DMACSA          FM3_DMAC->DMACSA5
#define MFS_SPI_RX_DMACDA          FM3_DMAC->DMACDA5
#elif SPI_DMA_RXCH == 6
#define MFS_SPI_RX_DMA_IRQHander   DMAC6_Handler 
#define MFS_SPI_RX_DMA_IRQn        DMAC6_IRQn
#define MFS_SPI_RX_DMACA           FM3_DMAC->DMACA6
#define MFS_SPI_RX_DMACB           FM3_DMAC->DMACB6
#define MFS_SPI_RX_DMACSA          FM3_DMAC->DMACSA6
#define MFS_SPI_RX_DMACDA          FM3_DMAC->DMACDA6
#elif SPI_DMA_RXCH == 7
#define MFS_SPI_RX_DMA_IRQHander   DMAC7_Handler 
#define MFS_SPI_RX_DMA_IRQn        DMAC7_IRQn
#define MFS_SPI_RX_DMACA           FM3_DMAC->DMACA7
#define MFS_SPI_RX_DMACB           FM3_DMAC->DMACB7
#define MFS_SPI_RX_DMACSA          FM3_DMAC->DMACSA7
#define MFS_SPI_RX_DMACDA          FM3_DMAC->DMACDA7
#endif

#if SPI_MFS == 0
#define MFS_SPI_RX_DRQSEL()       {FM3_INTREQ->DRQSEL = (FM3_INTREQ->DRQSEL & ~(1 << 12)) | (1 << 12);}      
#elif SPI_MFS == 1
#define MFS_SPI_RX_DRQSEL()       {FM3_INTREQ->DRQSEL = (FM3_INTREQ->DRQSEL & ~(1 << 14)) | (1 << 14);} 
#elif SPI_MFS == 2
#define MFS_SPI_RX_DRQSEL()       {FM3_INTREQ->DRQSEL = (FM3_INTREQ->DRQSEL & ~(1 << 16)) | (1 << 16);} 
#elif SPI_MFS == 3
#define MFS_SPI_RX_DRQSEL()       {FM3_INTREQ->DRQSEL = (FM3_INTREQ->DRQSEL & ~(1 << 18)) | (1 << 18);} 
#elif SPI_MFS == 4
#define MFS_SPI_RX_DRQSEL()       {FM3_INTREQ->DRQSEL = (FM3_INTREQ->DRQSEL & ~(1 << 20)) | (1 << 20);} 
#elif SPI_MFS == 5
#define MFS_SPI_RX_DRQSEL()       {FM3_INTREQ->DRQSEL = (FM3_INTREQ->DRQSEL & ~(1 << 22)) | (1 << 22);} 
#elif SPI_MFS == 6
#define MFS_SPI_RX_DRQSEL()       {FM3_INTREQ->DRQSEL = (FM3_INTREQ->DRQSEL & ~(1 << 24)) | (1 << 24);} 
#elif SPI_MFS == 7
#define MFS_SPI_RX_DRQSEL()       {FM3_INTREQ->DRQSEL = (FM3_INTREQ->DRQSEL & ~(1 << 26)) | (1 << 26);} 
#endif
   
#endif

/*
 *  Register Bit Definition
 */
#define SMR_SOE          0x01U
#define SMR_SCKE         0x02U
#define SMR_BDS          0x04U
#define SMR_SCINV        0x08U
#define SMR_WUCR         0x10U
#define SMR_MD_UART      0x00U
#define SMR_MD_UART_MP   0x20U
#define SMR_MD_SIO       0x40U
#define SMR_MD_LIN       0x60U
#define SMR_MD_I2C       0x80U

#define SCR_TXE          0x01U
#define SCR_RXE          0x02U
#define SCR_TBIE         0x04U
#define SCR_TIE          0x08U
#define SCR_RIE          0x10U
#define SCR_SPI          0x20U
#define SCR_MS           0x40U
#define SCR_UPGL         0x80U

#define SSR_TBI          0x01U
#define SSR_TDRE         0x02U
#define SSR_RDRF         0x04U
#define SSR_ORE          0x08U
#define SSR_REC          0x80U

#define ESCR_SOP         0x80U
#define ESCR_DATABITS_8  0x00U
#define ESCR_DATABITS_5  0x01U
#define ESCR_DATABITS_6  0x02U
#define ESCR_DATABITS_7  0x03U
#define ESCR_DATABITS_9  0x04U
#define ESCR_WTBITS_0    0x00U
#define ESCR_WTBITS_1    0x08U
#define ESCR_WTBITS_2    0x10U
#define ESCR_WTBITS_3    0x18U

#define FCR1_FSEL        0x01U
#define FCR1_FTIE        0x02U
#define FCR1_FDRQ        0x04U
#define FCR1_FRIIE       0x08U
#define FCR1_FLSTE       0x10U

#define FCR0_FE1         0x01U
#define FCR0_FE2         0x02U
#define FCR0_FCL1        0x04U
#define FCR0_FCL2        0x08U
#define FCR0_FSET        0x10U
#define FCR0_FLD         0x20U
#define FCR0_FLST        0x40U

/* General Definition */

/* Funcion Prototypes */
extern void SPIDev_Init(void);
extern void SPIDev_Uninit(void);
extern int32_t  SPIDev_SetBaudrate(int32_t Baudrate);
extern int32_t  SPIDev_BufTxRx(void *pDataTx, void *pDataRx, uint32_t Size);
extern int32_t  SPIDev_DataTx(void *pData, uint32_t *pSize);
extern int32_t  SPIDev_DataRx(void *pData, uint32_t *pSize);
extern int32_t  SPIDev_DMA_DataTx(void *pData, uint32_t Size);
extern int32_t  SPIDev_DMA_DataRx(void *pData, uint32_t Size);

/* Externs */


#ifdef __cplusplus
}
#endif

#endif	/* __SPI_DMA_SUPPORT_H */
