/************************************************************************/
/*               (C) Fujitsu Semiconductor Europe GmbH                  */
/*                                                                      */
/* The following software deliverable is intended for and must only be  */
/* used for reference and in an evaluation laboratory environment.      */
/* It is provided on an as-is basis without charge and is subject to    */
/* alterations.                                                         */
/* It is the user?s obligation to fully test the software in its        */
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
/* To the maximum extent permitted by applicable law, FSEU?s liability  */
/* is restricted to intention and gross negligence.                     */
/* FSEU is not liable for consequential damages.                        */
/*                                                                      */
/* (V1.4)                                                               */
/************************************************************************/
/** \file joystick.h
 **
 ** MCU description file
 **
 ** History:
 **   2012-09-17  1.0  MSc  First version
 *****************************************************************************/

#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/

#include "mcu.h"

#define _BUTTON_API_MSC_
#define API_NUMBEROFBUTTONS 4
/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define')                           */
/*****************************************************************************/

#define JOYSTICK_PDR     FM3_GPIO->PDIR3
#define JOYSTICK_PDR_P0  bFM3_GPIO_PDIR3_PA
#define JOYSTICK_PDR_P1  bFM3_GPIO_PDIR3_PC
#define JOYSTICK_PDR_P2  bFM3_GPIO_PDIR3_PD
#define JOYSTICK_PDR_P3  bFM3_GPIO_PDIR3_PE
#define JOYSTICK_PDR_P4  bFM3_GPIO_PDIR3_PF

#define JOYSTICK_DDR     FM3_GPIO->DDR3
#define JOYSTICK_DDR_D0  bFM3_GPIO_DDR3_PA
#define JOYSTICK_DDR_D1  bFM3_GPIO_DDR3_PC
#define JOYSTICK_DDR_D2  bFM3_GPIO_DDR3_PD
#define JOYSTICK_DDR_D3  bFM3_GPIO_DDR3_PE
#define JOYSTICK_DDR_D4  bFM3_GPIO_DDR3_PF

#define JOYSTICK_MASK    ((1 << 0xA) | (1 << 0xC) | (1 << 0xD) | (1 << 0xE) | (1 << 0xF))

#define JOYSTICK_CENTER  (1 << 0xA)
#define JOYSTICK_RIGHT   (1 << 0xD)
#define JOYSTICK_DOWN    (1 << 0xF)
#define JOYSTICK_LEFT    (1 << 0xC)
#define JOYSTICK_UP      (1 << 0xE)

#define JOYSTICK_PRESSED()         (JOYSTICK_PDR & JOYSTICK_MASK)
#define JOYSTICK_CENTER_PRESSED()  ((JOYSTICK_PDR & JOYSTICK_CENTER) != JOYSTICK_CENTER)
#define JOYSTICK_RIGHT_PRESSED()   ((JOYSTICK_PDR & JOYSTICK_RIGHT) != JOYSTICK_RIGHT)
#define JOYSTICK_DOWN_PRESSED()    ((JOYSTICK_PDR & JOYSTICK_DOWN) != JOYSTICK_DOWN)
#define JOYSTICK_LEFT_PRESSED()    ((JOYSTICK_PDR & JOYSTICK_LEFT) != JOYSTICK_LEFT)
#define JOYSTICK_UP_PRESSED()      ((JOYSTICK_PDR & JOYSTICK_UP) != JOYSTICK_UP)

#define BUTTON_LEFT_PRESSED()     JOYSTICK_LEFT_PRESSED()    
#define BUTTON_RIGHT_PRESSED()    JOYSTICK_RIGHT_PRESSED()   
#define BUTTON_UP_PRESSED()       JOYSTICK_UP_PRESSED()      
#define BUTTON_DOWN_PRESSED()     JOYSTICK_DOWN_PRESSED()    
#define BUTTON_CENTER_PRESSED()   JOYSTICK_CENTER_PRESSED()

/*****************************************************************************/
/* Global type definitions ('typedef')                                       */
/*****************************************************************************/

/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source)           */
/*****************************************************************************/

/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source)             */
/*****************************************************************************/

void Joystick_Init(void);

#endif /* __MCU_H__ */
