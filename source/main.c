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

#include "mcu.h"

#include "lwip_poll.h"
#include "dhcp.h"
#include "etharp.h"
#include "ip.h"
#include "ip_frag.h"
#include "udp.h"
#include "snmp_msg.h"
#include "tcp_impl.h"
#include "dns.h"
#include "stats.h"

#include "board.h"
#include "joystick.h"
#include "scheduler.h"
#include "tasks.h"
#include "adc.h"
#include "dualtimer_delay.h"

#include "uart.h"
#include <stdio.h>

/**
 ******************************************************************************
 ** \brief  Main function of project for MB9A310 series.
 **
 ** \param  none
 ** \return uint32_t return value, if needed
 ******************************************************************************/

int32_t main(void)
{	
	Board_Init();
	Uart4_Init();
	_delay_init();
	
	LwIP_Init();
	
	// Setup scheduler and tasks
	Scheduler_Init();
	
	Scheduler_AddTask(etharp_tmr, 400, ARP_TMR_INTERVAL);
#if LWIP_TCP
	Scheduler_AddTask(tcp_tmr, 500, TCP_TMR_INTERVAL);
#endif
#if IP_FRAG
	Scheduler_AddTask(ip_reass_tmr, 400, IP_TMR_INTERVAL);
#endif
#if LWIP_DHCP
	Scheduler_AddTask(Task_RefreshDHCP, 1000, 1000);
	Scheduler_AddTask(dhcp_fine_tmr, 100, DHCP_FINE_TIMER_MSECS);
	Scheduler_AddTask(dhcp_coarse_tmr, 100, DHCP_COARSE_TIMER_MSECS);
#endif
#if LWIP_DNS
	Scheduler_AddTask(dns_tmr, 500, DNS_TMR_INTERVAL);
#endif

	Scheduler_AddTask(Task_SetLEDs, 0, 100);
	Scheduler_AddTask(Task_ProcessSwitches, 500, 100);
	Scheduler_AddTask(Task_Adconverter, 500, 100);
	Scheduler_AddTask(Task_SerialTerminalOutput, 500, 1000);
//	Scheduler_AddTask(stats_display, 5300, 10000);

	Scheduler_Start();
	
	while(1){
		LwIP_Input_Handle();
		Scheduler_DispatchTasks();
	}
}

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
