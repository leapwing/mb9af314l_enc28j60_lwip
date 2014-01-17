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
 ** \file   stacklessudp.c
 **
 ** Send and receive UDP packets without a full featured TCP/IP stack
 **
 ** History:
 ** 2012-02-14 V0.01 CNo first version
 ** 2012-09-04 V0.02 MWi Doxygen comments added
 **
 ******************************************************************************/
 
/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "stacklessudp.h"

/**
 ******************************************************************************
 ** \addtogroup SlUdpGroup
 ******************************************************************************/
//@{

/******************************************************************************/
/* Local pre-processor symbols/macros ('#define')                             */
/******************************************************************************/

/******************************************************************************/
/* Global variable definitions (declared in header file with 'extern')        */
/******************************************************************************/

/******************************************************************************/
/* Local type definitions ('typedef')                                         */
/******************************************************************************/

/******************************************************************************/
/* Local function prototypes ('static')                                       */
/******************************************************************************/

/******************************************************************************/
/* Local variable definitions ('static')                                      */
/******************************************************************************/

/******************************************************************************/
/* Function implementation - global ('extern') and local ('static')           */
/******************************************************************************/

/**
 ******************************************************************************
 ** \brief With this routine you can send an UDP packet without a full-fledged
 **        TCP/IP stack
 **
 ** Here you can write an Ethernet frame into pu8TxBuffer that is sent by MAC
 ** unit with  EMAC_Send() function.
 **
 ** \param  pu8TxBuffer       Buffer containing the complete Ethernet frame
 ** \param  pu8UdpPayload     only the data to be sent
 ** \param  u32PayloadLength  Length of payload data in bytes
 **
 ******************************************************************************/
void TxBufferUDPFill(uint8_t *pu8TxBuffer, uint8_t * pu8UdpPayload, uint32_t u32PayloadLength)
{
	uint32_t u32Index;
	
  // Ethernet header//////////////////////////////////////////////////
  // Destination Address -- now: broadcast
  pu8TxBuffer[0]  = DESTMACADDR0;
  pu8TxBuffer[1]  = DESTMACADDR1;
  pu8TxBuffer[2]  = DESTMACADDR2;
  pu8TxBuffer[3]  = DESTMACADDR3;
  pu8TxBuffer[4]  = DESTMACADDR4;
  pu8TxBuffer[5]  = DESTMACADDR5;

  // Source Address
  pu8TxBuffer[6]  = SRCMACADDR0;
  pu8TxBuffer[7]  = SRCMACADDR1;
  pu8TxBuffer[8]  = SRCMACADDR2;
  pu8TxBuffer[9]  = SRCMACADDR3;
  pu8TxBuffer[10] = SRCMACADDR4;
  pu8TxBuffer[11] = SRCMACADDR5;

  // Type/Length
  pu8TxBuffer[12] = 0x08; // Internet Protocol
  pu8TxBuffer[13] = 0x00;

  // IP header ////////////////////////////////////////////////////////
  pu8TxBuffer[14] = (4 << 4) | 5; // IPv4, Headerlength = 5 32-bit words
  pu8TxBuffer[15] = 0; // Type of Service - pointless as ignored by any router under the sun
  pu8TxBuffer[16] = (20 + 8 + u32PayloadLength) / 255; // total length high
  pu8TxBuffer[17] = (20 + 8 + u32PayloadLength) % 255; // total length low

  pu8TxBuffer[18] = 0; // Identification (fragmentation)
  pu8TxBuffer[19] = 0; // Identification (fragmentation)
  pu8TxBuffer[20] = (1 << 6); // fragmentation: don't fragment bit
  pu8TxBuffer[21] = 0; // fragment offset

  pu8TxBuffer[22] = 255; // time to live
  pu8TxBuffer[23] = 17; // protocol: UDP
  pu8TxBuffer[24] = 0x00; // header checksum, handled by COE (Checksum Offload Engine)
  pu8TxBuffer[25] = 0x00; // header checksum, handled by COE (Checksum Offload Engine)

  pu8TxBuffer[26] = SRCIPADDR0;  // source address
  pu8TxBuffer[27] = SRCIPADDR1;
  pu8TxBuffer[28] = SRCIPADDR2;
  pu8TxBuffer[29] = SRCIPADDR3;

  pu8TxBuffer[30] = DESTIPADDR0;  // destination address
  pu8TxBuffer[31] = DESTIPADDR1;
  pu8TxBuffer[32] = DESTIPADDR2;
  pu8TxBuffer[33] = DESTIPADDR3;

  pu8TxBuffer[24] = 0; // header checksum, handled by COE (Checksum Offload Engine)
  pu8TxBuffer[25] = 0; // header checksum, handled by COE (Checksum Offload Engine)


  // UDP header ///////////////////////////////////////////////////////
  pu8TxBuffer[34] = 0xFF; // source port
  pu8TxBuffer[35] = 0xBB;
  pu8TxBuffer[36] = 0xFF; // destination port
  pu8TxBuffer[37] = 0xAA;

  pu8TxBuffer[38] = 0;    // UDP length high \todo
  pu8TxBuffer[39] = 8 + u32PayloadLength; // UDP length low \todo
  pu8TxBuffer[40] = 0x00; // header checksum \todo
  pu8TxBuffer[41] = 0x00; // header checksum \todo

  // Copy payload
  for (u32Index = 0; u32Index < u32PayloadLength; ++u32Index)
  {
    pu8TxBuffer[42 + u32Index] = pu8UdpPayload[u32Index];
  }
} // TxBufferUDPFill

//@} // SlUdpGroup

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
