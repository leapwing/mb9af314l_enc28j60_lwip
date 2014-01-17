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
 ** \file   board.c
 **
 ** Quick routine to initialize ADC
 **
 ** History:
 ** 2012-10-01 V1.0  CNo first version
 **
 ******************************************************************************/
#include "board.h"
#include "joystick.h"
#include "adc.h"
#include "led.h"


void Board_Init(void)
{
    // Initialize ADC to read out pot
    ADCinit();
    bFM3_ADC0_SCCR_SSTR = 1; // Start ADC operation
	LED_Init();
    Joystick_Init();
}

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
