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
 ** \file   scheduler.h
 **
 ** Interface definition for simple cooperative scheduler
 **
 **
 ** History:
 ** 2012-01-17 V0.1 CNo first version
 ** 2012-09-03 V1.0 CNo added SystemTime, Scheduler_DeleteTask(), fixed style, improved documentation
 **
 ******************************************************************************/

#ifndef SCHEDULER_H
#define SCHEDULER_H

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "mcu.h"

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
 ******************************************************************************
 ** \defgroup SchedGroup Scheduler
 **
 ** Provided functions of Scheduler module:
 ** - Scheduler_Init()
 ** - Scheduler_Start()
 ** - Scheduler_AddTask()
 ** - Scheduler_DispatchTasks()
 ** - Scheduler_GetSystemTime()
 ** - Scheduler_DeleteTask()
 ** - Scheduler_AddTask()
 **
 ** Source code for simple cooperative scheduler
 **
 ** Most embedded systems require specific actions to occur periodicaly.
 ** ARP, DHCP, TCP protocols are examples for protocols that define timeouts.
 ** If for some reason interrupts are not used, the respective polling has to be done by software.
 **
 ** This simple scheduler is designed to meet those requirements.
 ** It is triggered every millisecond by the SysTick timer and decreases a variable
 ** for every task. If this variable has reached zero, the respective task is marked active.
 ** In the main loop, a dispatcher will execute all active tasks.
 **
 ** \note This module uses the Systick Interrupt Handler SysTick_Handler().
 **       If the user want to reuse it, it must be renamed and called in the
 **       user handler.
 **
 ******************************************************************************/
//@{

/******************************************************************************/
/* Global pre-processor symbols/macros ('#define')                            */
/******************************************************************************/
 /// Size of task table. Only as many as SCHEDULER_MAX_TASKS can be created, not more.
#define SCHEDULER_MAX_TASKS (12)

/// Value to be returned by Scheduler_AddTask() if task table is full (value must be larger than SCHEDULER_MAX_TASKS!)
#define SCHEDULER_ADD_FAILURE (9999)

//#define SCHEDULER_TICKS (5) ///< If defined, scheduler will be executed any x msec, instead of every single msec.

/******************************************************************************
 * Global type definitions
 ******************************************************************************/
 
 /**
 ******************************************************************************
 ** \brief Scheduler Error Status
 ******************************************************************************/
typedef enum en_SchedulerErrorStatus
{
  SchedulerTaskOK            = 0,
  SchedulerTaskFailed        = 1,
  SchedulerInvalidTaskNumber = 2
} en_SchedulerErrorStatus_t;

/**
 ******************************************************************************
 ** \brief Scheduler Task Structure
 ******************************************************************************/
typedef struct stc_SchedulerTask
{
  void (*pfnTaskCb)(void);              ///< Pointer to the function to be run as the task
  volatile uint32_t u32PeriodicalDelay; ///< Time between repetitions in msec. 0 for one-shot tasks, i.e. no repetition
  volatile uint32_t u32DelayToNextRun;  ///< Value is decreased every msec. When 0, set to u32PeriodicalDelay
  volatile uint32_t u32TaskRunning;     ///< Task is ready to be activated by dispatcher if >0.
} stc_SchedulerTask_t;

/******************************************************************************/
/* Global variable definitions ('extern')                                     */
/******************************************************************************/
extern volatile uint32_t g_u32SystemTime; 

/******************************************************************************/
/* Global function prototypes (definition in C source)                        */
/******************************************************************************/
void Scheduler_Init(void);
uint32_t Scheduler_AddTask(void(*pfnTaskCb)(void),
                           uint32_t u32Delay,
                           uint32_t u32Period
                          );
void Scheduler_Start(void);
en_SchedulerErrorStatus_t Scheduler_DeleteTask(uint32_t u32TaskNumber);
void Scheduler_DispatchTasks(void);

uint32_t Scheduler_GetSystemTime(void);

//@} // SchedGroup

#ifdef __cplusplus
}
#endif

#endif //SCHEDULER_H
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
