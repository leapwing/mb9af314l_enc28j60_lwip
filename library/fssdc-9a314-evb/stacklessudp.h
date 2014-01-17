/************************************************************************/
/* $Revision$                                                           */
/* $Date$                                                               */
/************************************************************************/
/*               (C) Fujitsu Semiconductor Europe GmbH (FSEU)           */
/*                                                                      */
/* The following software deliverable is intended for and must only be  */
/* used for reference and in an evaluation laboratory environment.      */
/* It is provided on an as-is basis without charge and is subject to    */
/* alterations.                                                         */
/* It is the user's obligation to fully test the software in its        */
/* environment and to ensure proper functionality, qualification and    */
/* compliance with component specifications.                            */
/*                                                                      */
/* In the event the software deliverable includes the use of open       */
/* source components, the provisions of the governing open source       */
/* license agreement shall apply with respect to such software          */
/* deliverable.                                                         */
/* FSEU does not warrant that the deliverables do not infringe any      */
/* third party intellectual property right (IPR). In the event that     */
/* the deliverables infringe a third party IPR it is the sole           */
/* responsibility of the customer to obtain necessary licenses to       */
/* continue the usage of the deliverable.                               */
/*                                                                      */
/* To the maximum extent permitted by applicable law FSEU disclaims all */
/* warranties, whether express or implied, in particular, but not       */
/* limited to, warranties of merchantability and fitness for a          */
/* particular purpose for which the deliverable is not designated.      */
/*                                                                      */
/* To the maximum extent permitted by applicable law, FSEU's liability  */
/* is restricted to intentional misconduct and gross negligence.        */
/* FSEU is not liable for consequential damages.                        */
/*                                                                      */
/* (V1.5)                                                               */
/************************************************************************/

/**
 ******************************************************************************
 ** \file   stacklessudp.h
 **
 ** Definitions for stacklessudp.c functions
 **
 ** History:
 ** 2012-02-14 V0.01 CNo first version
 ** 2012-09-04 V0.02 MWi Doxygen comments added
 **
 ******************************************************************************/

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include <stdint.h>

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
 ******************************************************************************
 ** \defgroup SlUdpGroup Stackless UDP
 **
 ** Provided functions of Stackless UDP module:
 ** - TxBufferUDPFill()
 ** - ReceiveUDPData()
 **
 ******************************************************************************/ 
//@{

/******************************************************************************/
/* Global pre-processor symbols/macros ('#define')                            */
/******************************************************************************/
#define MAC0HWADDR0 (0x00)
#define MAC0HWADDR1 (0x01)
#define MAC0HWADDR2 (0x01)
#define MAC0HWADDR3 (0x66)
#define MAC0HWADDR4 (0x73)
#define MAC0HWADDR5 (0x42)

#define MAC1HWADDR0 (0x00)
#define MAC1HWADDR1 (0x01)
#define MAC1HWADDR2 (0x01)
#define MAC1HWADDR3 (0x66)
#define MAC1HWADDR4 (0x73)
#define MAC1HWADDR5 (0x38)

// Source HW Address
#define SRCMACADDR0 MAC0HWADDR0;
#define SRCMACADDR1 MAC0HWADDR1;
#define SRCMACADDR2 MAC0HWADDR2;
#define SRCMACADDR3 MAC0HWADDR3;
#define SRCMACADDR4 MAC0HWADDR4;
#define SRCMACADDR5 MAC0HWADDR5;

// Destination HW Address
#define DESTMACADDR0 255;
#define DESTMACADDR1 255;
#define DESTMACADDR2 255;
#define DESTMACADDR3 255;
#define DESTMACADDR4 255;
#define DESTMACADDR5 255;

// Source IP Address
#define SRCIPADDR0 192;
#define SRCIPADDR1 168;
#define SRCIPADDR2   1;
#define SRCIPADDR3  42;

// Destination IP Address
#define DESTIPADDR0 255;
#define DESTIPADDR1 255;
#define DESTIPADDR2 255;
#define DESTIPADDR3 255;

/******************************************************************************
 * Global type definitions
 ******************************************************************************/
 
/******************************************************************************/
/* Global variable definitions ('extern')                                     */
/******************************************************************************/

/******************************************************************************/
/* Global function prototypes (definition in C source)                        */
/******************************************************************************/
void TxBufferUDPFill(uint8_t *pu8TxBuffer, uint8_t * pu8UdpPayload, uint32_t u32PayloadLength);
void ReceiveUDPData(void);

//@} // SlUdpGroup

#ifdef __cplusplus
}
#endif

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
