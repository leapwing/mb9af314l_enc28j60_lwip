  /*!
 ******************************************************************************
 **
 ** \file udp_echoserver.c
 **
 ** \brief UDP echo server demo
 **
 ** \author FSLA AE Team 
 **
 ** \version V0.1
 **
 ** \date 2012-7-09
 **
 ** \attention THIS SAMPLE CODE IS PROVIDED AS IS. FUJITSU SEMICONDUCTOR
 **            ACCEPTS NO RESPONSIBILITY OR LIABILITY FOR ANY ERRORS OR
 **            OMMISSIONS.
 **
 ** (C) Copyright 200x-201x by Fujitsu Semiconductor(Shanghai) Co.,Ltd.
 **
 ******************************************************************************
 **
 ** \note Other information.
 **
 ******************************************************************************
 */

/*---------------------------------------------------------------------------*/
/* include files                                                             								  */
/*---------------------------------------------------------------------------*/

#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"
#include <string.h>
#include <stdio.h>


/*---------------------------------------------------------------------------*/
/* local defines                                                                                                                 */
/*---------------------------------------------------------------------------*/
#define UDP_SERVER_PORT    7   /* define the UDP local connection port */

/*---------------------------------------------------------------------------*/
/* local functions prototypes                                                                                              */
/*---------------------------------------------------------------------------*/

void echo_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, u16_t port);

/*!
 ******************************************************************************
 ** \brief  Initialize the server application.
 **
 ** \param None
 **
 ******************************************************************************
 */
void udp_echo_init(void)
{
   struct udp_pcb *upcb;
   err_t err;
   
   /* Create a new UDP control block  */
   upcb = udp_new();
   
   if (upcb)
   {
     /* Bind the upcb to the UDP_PORT port */
     /* Using IP_ADDR_ANY allow the upcb to be used by any local interface */
      err = udp_bind(upcb, IP_ADDR_ANY, UDP_SERVER_PORT);
      
      if(err == ERR_OK)
      {
        /* Set a receive callback for the upcb */
        udp_recv(upcb, echo_receive_callback, NULL);
      }
      else
      {
        printf("can not bind pcb");
      }
   }
   else
   {
     printf("can not create pcb");
   } 
}
/*!
 ******************************************************************************
 ** \brief  Called when an UDP datagrm has been received on the port UDP_PORT..
 **
 ** \param arg user supplied argument (udp_pcb.recv_arg)
 ** \param pcb the udp_pcb which received data
 ** \param p the packet buffer that was received
 ** \param addr the remote IP address from which the packet was received
 ** \param port the remote port from which the packet was received
 ******************************************************************************
 */
void echo_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, u16_t port)
{

  /* Connect to the remote client */
  udp_connect(upcb, addr,port );
    
  /* Tell the client that we have accepted it */
  udp_send(upcb, p);

  /* free the UDP connection, so we can accept new clients */
  udp_disconnect(upcb);
	
  /* Free the p buffer */
  pbuf_free(p);
   
}


/*****************************************************************************/
/* END OF FILE */

