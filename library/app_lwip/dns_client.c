/*!
 ******************************************************************************
 **
 ** \file dns_client.c
 **
 ** \brief dns client init and callback
 **
 ** \author FSLA AE Team
 **
 ** \version V1.0
 **
 ** \date 2012-7-19
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

#include "lwip/memp.h"

#include "lwIP/tcp.h"
#include "lwIP/udp.h"
#include "lwIP/tcpip.h"
#include "netif/etharp.h"
#include "lwIP/dhcp.h"
#include "ethernetif.h"
#include "lwip/dns.h"
#include "lwip/inet.h"

#include <stdio.h>
#include <string.h>
#include "board.h"
#include "debug.h"

#if LWIP_DNS
/*---------------------------------------------------------------------------*/
/* local defines                                                                                                                 */
/*---------------------------------------------------------------------------*/
#define UDPSERVERPORT    7    //  UDP local connection port
#define MAX_IP_ADDR_LEN    16 //  xxx.xxx.xxx.xxx + \n
/*---------------------------------------------------------------------------*/
/* local data                                                                                                                    */
/*---------------------------------------------------------------------------*/
uint8_t dns_name[DNS_MAX_NAME_LENGTH]={0};
uint8_t message[MAX_IP_ADDR_LEN]={0};       //hold message of retieved IP address

uint8_t *parsing="dns is parsing,try later \n";
uint8_t *errmsg="dns table is full or not support \n";

/*---------------------------------------------------------------------------*/
/* global functions                                                          							  */
/*---------------------------------------------------------------------------*/

extern void dns_init(void);

extern err_t dns_gethostbyname(const char *hostname, struct ip_addr *addr, dns_found_callback found, 
                  void *callback_arg);

  /*!
 ******************************************************************************
 ** \brief   called when an UDP datagrm has been received on the port UDP_PORT.
 **
 ** \param arg user supplied argument (udp_pcb.recv_arg)
 ** \param pcb the udp_pcb which received data
 ** \param p the packet buffer that was received
 ** \param addr the remote IP address from which the packet was received
 ** \param port the remote port from which the packet was received
 **
 ******************************************************************************
 */
static void udp_server_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, u16_t port)
{
    struct ip_addr dnsaddr;
    uint8_t i;
    static char *hostname;
    err_t err=0xFF;
    uint8_t name_len=0;
    uint8_t *payload=NULL;
    uint8_t check[3]={0};

    /* Connect to the remote client */
    udp_connect(upcb, addr, port);

    /* Get dns string from client */
    if(p!=NULL)
    {
        payload=p->payload;
        for(i=0;i<3;i++)
        {
            check[i]=*payload;
            payload++;
        }

        if((check[0]=='w')&&(check[1]=='w')&&(check[2]=='w'))
        {
            for(i=0;i<3;i++)
            {
                check[i]=0;
            }

            strcpy((char *)dns_name,p->payload);
            err=dns_gethostbyname((const char *)dns_name,&dnsaddr,NULL,NULL);
            if(err==ERR_OK)
            {
                hostname=inet_ntoa(*((struct in_addr*)(&dnsaddr)));
                name_len=strlen(hostname);
                strcpy((char *)message,hostname);
                message[name_len]='\n';

                p->payload=message;
                p->len=strlen((char const *)message);
                p->tot_len=strlen((char const *)message);
            }
            else if(err==ERR_INPROGRESS)
            {
                p->payload=parsing;
                p->len=strlen((char const *)parsing);
                p->tot_len=strlen((char const *)parsing);
            }
            else
            {
                p->payload=errmsg;
                p->len=strlen((char const *)errmsg);
                p->tot_len=strlen((char const *)errmsg);
            }
            udp_send(upcb, p);          //send message to client

        }
        else
        {
            udp_send(upcb, p);  //echo back for other package
        }
    }


    /* free the UDP connection, so we can accept new clients */
    udp_disconnect(upcb);

     for(i=0;i<MAX_IP_ADDR_LEN;i++)
    {
        message[i]=0;   //clear buffer
    }
    /* Free the p buffer */
    pbuf_free(p);

}

  /*!
 ******************************************************************************
 ** \brief  Initialize the server application.
 **
 ** \param none
 **
 ******************************************************************************
 */
static void udp_server_init(void)
{
   struct udp_pcb *upcb;
   err_t err;

   /* Create a new UDP control block  */
   upcb = udp_new();

   if (upcb)
   {
     /* Bind the upcb to the UDP_PORT port */
     /* Using IP_ADDR_ANY allow the upcb to be used by any local interface */
      err = udp_bind(upcb, IP_ADDR_ANY, UDPSERVERPORT);

      if(err == ERR_OK)
      {
            /* Set a receive callback for the upcb */
            udp_recv(upcb, udp_server_receive_callback, NULL);

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
 ** \brief  initialize the dns client
 **
 ** \param none
 **
 ******************************************************************************
 */
 void dns_client_init(void)
{
    udp_server_init();
    dns_init();
}

#endif
/*****************************************************************************/
/* END OF FILE */
