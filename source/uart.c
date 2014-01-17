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
 ** \file   uart.c
 **
 ** Enables printf() in stdio.h to output on UART0
 **
 ** History:
 ** 2012-01-12 V0.01 CNo first version
 ** 2012-09-04 V0.02 MWi Doxygen comments added
 **
 ******************************************************************************/

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "mcu.h"
#include "uart.h"       /* include all UART-Functions */

/**
 ******************************************************************************
 ** \addtogroup UartGroup
 ******************************************************************************/
//@{

/**
 ******************************************************************************
 ** \brief  Initializes UART0     (for IAR UART4)
 ******************************************************************************/
void Uart4_Init( void )
{
  // Initialize ports for UART0
  FM3_GPIO->PFR0 |= (1 << 0x0A) | (1 << 0x0B);  // P0A->SIN4_0, P0B->SOT4_0
  FM3_GPIO->EPFR08 = (FM3_GPIO->EPFR08 & ~(15 << 4)) | (4 << 4); // Use SIN4_0, SOT4_0

//  UART0->SMR =  0x01;
  FM3_MFS4_UART->SCR =  (1<<7); // UPCL (UART reset)
  FM3_MFS4_UART->SMR =  (1<<0)|(1<<5); // enable SOE

  FM3_MFS4_UART->SSR = 0;
  FM3_MFS4_UART->ESCR = 0;
  FM3_MFS4_UART->BGR = 40000000 / 2 / 115200 - 1;  // 115.2kbaud @ 40 MHz

  FM3_MFS4_UART->SCR =  (1<<0)|(1<<1); // enable RXE, TXE
}

/**
******************************************************************************
** \brief  Overloads system function to let printf() write to UART0
**
** \param  handle  Standard interface
** \param  pu8Buf  Print buffer
** \param  u8Size  Print buffer size
** \return u8Size  Size of actual print
******************************************************************************/
uint8_t __write(int handle, const unsigned char *pu8Buf, uint8_t u8Size)
{
  int32_t i32CharToWrite = 0;
  for(i32CharToWrite = 0; i32CharToWrite < u8Size; i32CharToWrite++)
  {
    while (!(FM3_MFS4_UART->SSR & (1 << 1)))
      ;                                     // wait for transmit buffer to be empty
	  FM3_MFS4_UART->RDR = pu8Buf[i32CharToWrite];  // put ch into tx buffer
  }
  return u8Size;
}


/* *
******************************************************************************
** \brief  some systems need __putchar() to be overloaded instead of __write()
** to enable printf() to output strings to UART0. In that case, you can use this code.
**
** \param  standard interface
** \return size
****************************************************************************** /
int __putchar(char ch)
{
   while (!(FM3_MFS0_UART->SSR & (1 << 1)))
      ;                                     // wait for transmit buffer to be empty
	  FM3_MFS0_UART->RDR = ch;                // put ch into tx buffer
    return ch;
}
*/

/**
******************************************************************************
** \brief  Overloads system function to let scanf() read from UART0
**
** \param  handle   Standard interface
** \param  pu8Buf   Print buffer
** \param  u8Size   Print buffer size
** \return u8Size   Size of actual print
******************************************************************************/
uint8_t __read(int handle, unsigned char *pu8Buf, uint8_t u8Size)
{
  uint8_t u8Index;
  for (u8Index = 0; u8Index < u8Size; ++u8Index)
  {
    while(!(FM3_MFS4_UART->SSR & (1 << 2)));			/* wait for data in buffer */
      pu8Buf[u8Index] = FM3_MFS4_UART->RDR;
  }
  return u8Size;
}

#if  __CC_ARM
// If Keil uVision is used with standard library files like stdio.h and strings.h,
// a special features by the name of "semihosting" must be considered.
// See http://infocenter.arm.com/help/topic/com.arm.doc.dui0349c/Ciheeaja.html for more information.
// This example is based on C:\Keil\ARM\Examples\Hello
#pragma import(__use_no_semihosting)

typedef struct __FILE {int handle;} FILE;
FILE __stdout;

int fputc(int ch, FILE *f) {
  return (__write(0, (const unsigned char*)&ch, 1));
}

int ferror(FILE *f) {
  /* Your implementation of ferror */
  return 0;
}

void _ttywrch(int ch) {
  __write(0, (const unsigned char*)&ch, 1);
}

void _sys_exit(int return_code) {
while(1);  /* endless loop */
}
#endif  //__CC_ARM

//@} // UartGroup

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
