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
 ** \file   tasks.h
 **
 ** Just some glue logic to be used with scheduler
 **
 **
 ** History:
 ** 2012-01-23 V0.01 CNo first version
 ** 2012-09-04 V0.02 MWi Doxygen comments added
 **
 ******************************************************************************/

#ifndef ETHERNET_TASKS_H
#define ETHERNET_TASKS__H

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

#include "mcu.h"
/**
 ******************************************************************************
 ** \defgroup TasksGroup Tasks
 **
 ** Provided functions of Tasks module:
 ** - SetLEDs()
 ** - Eth_AutonegotiateHandler()
 ** - ProcessSwitches()
 ** - SerialTerminalOutput()
 ** - AcquireRotarySwitchData()
 ** - IncreaseLEDValue()
 **
 ******************************************************************************/
//@{

/******************************************************************************/
/* Global pre-processor symbols/macros ('#define')                            */
/******************************************************************************/

/******************************************************************************
 * Global type definitions
 ******************************************************************************/

/******************************************************************************/
/* Global variable definitions ('extern')                                     */
/******************************************************************************/
extern uint16_t u16ADCData;
extern uint32_t u32KeyData;
extern uint16_t u16LedVal;
/******************************************************************************/
/* Global function prototypes (definition in C source)                        */
/******************************************************************************/

void Task_RefreshDHCP(void);
void Task_SetLEDs(void);
void Task_ProcessSwitches(void);
void Task_SerialTerminalOutput(void);
void Task_Adconverter(void);

//@} // TasksGroup

#ifdef __cplusplus
}
#endif

#endif //ETHERNET_TASKS_H
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
