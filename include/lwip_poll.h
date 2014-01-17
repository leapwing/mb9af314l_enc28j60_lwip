#ifndef _LWIP_POLL_H
#define _LWIP_POLL_H

/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "mcu.h"

/*---------------------------------------------------------------------------*/
/* constants and macros                                                      */
/*---------------------------------------------------------------------------*/
extern volatile uint32_t g_u32SystemTime;

/* application demos selection */

//demo for dhcp
#define DHCP_ENABLE 0 

//#define TCPUDP_ECHO_DEMO                 //demo for TCP/UDP echo server
//#define DNS_TCP_CLIENT_DEMO           //demo for TCP client and DNS client

/*throughput testing enable*/
#define PERFORMANCE_TEST   //throughput test enable

//#define NETIO_SERVER

#if defined PERFORMANCE_TEST
#define IPERF_CLIENT    //Send throughput test using Iperf
#define IPERF_SERVER    //Receive throughput test using Iperf
#define TCP_RXTX
#define NETIO_SERVER
#endif


/*---------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*---------------------------------------------------------------------------*/
void LwIP_Init(void);
void LwIP_Input_Handle(void);
void LwIP_Tmr_Handle(void);
#endif /* _LWIP_POLL_H */
/*****************************************************************************/
/* END OF FILE */
/*****************************************************************************/
