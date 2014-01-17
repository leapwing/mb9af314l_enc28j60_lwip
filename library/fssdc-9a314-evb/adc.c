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
 ** \file   adc.c
 **
 ** Quick routine to initialize ADC
 **
 ** History:
 ** 2012-03-15 V1.0  CNo first version
 **
 ******************************************************************************/
#include "adc.h"


void ADCinit(void){
  FM3_GPIO->ADE   = 0x0002;  // Select analogue pin -> AN01

  FM3_ADC0->SCIS3 = 0x00; // Scan channel select AN31-AN24
  FM3_ADC0->SCIS2 = 0x00; // Scan channel select AN23-AN16
  FM3_ADC0->SCIS1 = 0x00; // Scan channel select AN15-AN08
  FM3_ADC0->SCIS0 = 0x02; // Scan channel select AN07-AN00 -> AN00

  FM3_ADC0->ADST1 = 0x2F; // Sampling Time 1
  FM3_ADC0->ADST0 = 0x2F; // Sampling Time 0

  FM3_ADC0->ADSS3 = 0x00; // Sampling Time Select AN31-AN24
  FM3_ADC0->ADSS2 = 0x00; // Sampling Time Select AN23-AN16
  FM3_ADC0->ADSS1 = 0x00; // Sampling Time Select AN15-AN08
  FM3_ADC0->ADSS0 = 0x00; // Sampling Time Select AN07-AN00 -> Use Sampling Time 0 for AN0

  FM3_ADC0->ADCT  = 0x00; // Comparison Time = 14 x (ADCT + 2) / HCLK

  FM3_ADC0->ADCEN = 0x01; // Enable ADC

  while (3 != FM3_ADC0->ADCEN); // wait until ADC operation is enabled

  FM3_ADC0->ADSR  = (1<<6); // Stop ADC, Places conversion result on the LSB side.

  FM3_ADC0->CMPCR = 0x00; // No comparsion
  FM3_ADC0->CMPD  = 0x00; // No comparsion value

  FM3_ADC0->SFNS  = 0x00; // Set Fifo Stage Count Interrupt
  FM3_ADC0->ADCR  = 0x00; // Disable ADC interrupts

  bFM3_ADC0_SCCR_RPT = 0; //start single channel, single conversion mode
  FM3_ADC0->SCCR = 0x11; // FIFO clear, start ADC Single Conversion
}
