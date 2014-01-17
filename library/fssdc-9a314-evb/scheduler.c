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
 ** \file   scheduler.c
 **
 ** History:
 ** 2012-01-17 V0.1 CNo first version
 ** 2012-09-03 V0.2 CNo added SystemTime, Scheduler_DeleteTask(), fixed style,
 **                 improved documentation
 ** 2012-10-04 V0.3 CNo added g_u32SystemTime
 ******************************************************************************/

#include "scheduler.h"

/**
 ******************************************************************************
 ** \addtogroup SchedGroup
 ******************************************************************************/
//@{

/******************************************************************************/
/* Local pre-processor symbols/macros ('#define')                             */
/******************************************************************************/

/******************************************************************************/
/* Global variable definitions (declared in header file with 'extern')        */
/******************************************************************************/
/// A global time reference incremented every 1ms
volatile uint32_t g_u32SystemTime = 0; 

/******************************************************************************/
/* Local type definitions ('typedef')                                         */
/******************************************************************************/

/******************************************************************************/
/* Local function prototypes ('static')                                       */
/******************************************************************************/

/******************************************************************************/
/* Local variable definitions ('static')                                      */
/******************************************************************************/
static volatile en_SchedulerErrorStatus_t m_enStatus;

static stc_SchedulerTask_t astcTaskTable[SCHEDULER_MAX_TASKS];

/******************************************************************************/
/* Function implementation - global ('extern') and local ('static')           */
/******************************************************************************/

/**
 ******************************************************************************
 ** \brief Initialize Scheduler
 **
 ** This function must be called before any other Scheduler related function
 ** can be used. It initializes the task table.
 **
 *****************************************************************************/
void Scheduler_Init(void)
{
  uint32_t u32TaskNumber;
  for (u32TaskNumber = 0; u32TaskNumber < SCHEDULER_MAX_TASKS; u32TaskNumber++)
  {
    astcTaskTable[u32TaskNumber].pfnTaskCb = NULL;
    astcTaskTable[u32TaskNumber].u32PeriodicalDelay = 0;
    astcTaskTable[u32TaskNumber].u32DelayToNextRun = 0;
    astcTaskTable[u32TaskNumber].u32TaskRunning = 0;
  }
} // Scheduler_Init

/**
 ******************************************************************************
 ** \brief Start scheduler
 **
 ** This function will activate the SysTick interrupt that will trigger
 ** SysTick_Handler(), which contains the scheduler code.
 **
 *****************************************************************************/
void Scheduler_Start(void)
{
  // Configure and start SysTick timer to fire every msec
  SysTick_Config(__HCLK / 1000);
} // Scheduler_Start

/**
 ******************************************************************************
 ** \brief Scheduler
 **
 ** This function is triggered by the SysTick interrupt.
 **
 ** It updates the task table and marks Tasks active that are due
 ** If u32PeriodicalDelay == 0, task is treated as a one-shot event,
 ** i.e. after one invocation, task is removed from task table.
 **
 ** \note If the user want to use and own Systick Handler, this function
 **       must be renamed and called in the user handler.
 *****************************************************************************/
void SysTick_Handler(void) // Scheduler(void)
{
  uint32_t u32TaskNumber;
	
  g_u32SystemTime++;  
  for (u32TaskNumber = 0; u32TaskNumber < SCHEDULER_MAX_TASKS; u32TaskNumber++)
  {
    if (NULL != astcTaskTable[u32TaskNumber].pfnTaskCb)
    {
      if (0 == astcTaskTable[u32TaskNumber].u32DelayToNextRun)
			{
        astcTaskTable[u32TaskNumber].u32TaskRunning += 1;
        if (astcTaskTable[u32TaskNumber].u32PeriodicalDelay > 0)
        {
          astcTaskTable[u32TaskNumber].u32DelayToNextRun = astcTaskTable[u32TaskNumber].u32PeriodicalDelay;
        }
        else
        {
          astcTaskTable[u32TaskNumber].pfnTaskCb = NULL;
        }
      }
      else
      {
        astcTaskTable[u32TaskNumber].u32DelayToNextRun -= 1;
      }
    }
  }
} // SysTick_Handler

/**
 ******************************************************************************
 ** \brief Create a task.
 **
 ** This function can be called to create new tasks.
 **
 ** \param  pfnTaskCb  Pointer to function that shall be called ba scheduler
 ** \param  u32Delay   Time in msec between call of Scheduler_Start() and first
 **                    run
 ** \param  u32Period  Time in msec between two invocations
 **
 ** \note Check with Scheduler_GetStatus() if adding succeded if you want to
 **       use Scheduler_DeleteTask()
 **
 ** \retval u32TaskNumber  Place in task table, SCHEDULER_ADD_FAILURE if task
 **                        table full
 **
 *****************************************************************************/
uint32_t Scheduler_AddTask(void(*pfnTaskCb)(void),
                           uint32_t u32Delay,
                           uint32_t u32Period)
{
  uint32_t u32TaskNumber;
	
  for (u32TaskNumber = 0; u32TaskNumber < SCHEDULER_MAX_TASKS; u32TaskNumber++)
  {
    if (NULL == astcTaskTable[u32TaskNumber].pfnTaskCb)
    {
        astcTaskTable[u32TaskNumber].pfnTaskCb = pfnTaskCb;
        astcTaskTable[u32TaskNumber].u32PeriodicalDelay = u32Period;
        astcTaskTable[u32TaskNumber].u32DelayToNextRun = u32Delay;
        astcTaskTable[u32TaskNumber].u32TaskRunning = 0;
        m_enStatus = SchedulerTaskOK;
        break;
    }
  }

  if (SCHEDULER_MAX_TASKS == u32TaskNumber)
  {
      m_enStatus = SchedulerTaskFailed;
      u32TaskNumber = SCHEDULER_ADD_FAILURE;
  }

  return u32TaskNumber;
} // Scheduler_AddTask

/**
 ******************************************************************************
 ** \brief Run all due tasks
 **
 ** This function should be called as often as possible, i.e. in a while loop
 ** in main(). It will check the property u32TaskRunning of every task in the
 ** tasktable and will run it if that value is > 0.
 **
 *****************************************************************************/
void Scheduler_DispatchTasks(void)
{
  uint32_t u32TaskNumber;
	
  for (u32TaskNumber = 0; u32TaskNumber < SCHEDULER_MAX_TASKS; u32TaskNumber++)
  {
    if (astcTaskTable[u32TaskNumber].u32TaskRunning > 0)
    {
      astcTaskTable[u32TaskNumber].u32TaskRunning -= 1;
      astcTaskTable[u32TaskNumber].pfnTaskCb();
    }
  }
} // Scheduler_DispatchTasks

/**
 ******************************************************************************
 ** \brief Return time in msec since Scheduler_Start() was invoked
 **
 ** \retval uint32_t Time since call of Scheduler_Start()
 **
 *****************************************************************************/
uint32_t Scheduler_GetSystemTime(void)
{
  return g_u32SystemTime;
} // Scheduler_GetSystemTime

/**
 ******************************************************************************
 ** \brief Delete a task.
 **
 ** This function will remove a task from the task table.
 **
 ** \param  u32TaskNumber  Designates which task shall be removed. Obtained
 **                        as return value from Scheduler_AddTask()
 **
 ** \retval ErrorStatus    SchedulerInvalidTaskNumber if u32TaskNumber exceeds
 **                        maximum number of tasks (SCHEDULER_MAX_TASKS - 1)
 **                        SchedulerTaskOK else
 **
 *****************************************************************************/
en_SchedulerErrorStatus_t Scheduler_DeleteTask(uint32_t u32TaskNumber)
{
  if (u32TaskNumber >= SCHEDULER_MAX_TASKS)
  {
      return SchedulerInvalidTaskNumber;
  }
  else
  {
    astcTaskTable[u32TaskNumber].pfnTaskCb = NULL;
    astcTaskTable[u32TaskNumber].u32PeriodicalDelay = 0;
    astcTaskTable[u32TaskNumber].u32DelayToNextRun = 0;
    astcTaskTable[u32TaskNumber].u32TaskRunning = 0;
  }
  return SchedulerTaskOK;
} // Scheduler_DeleteTask

//@} // SchedGroup

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
