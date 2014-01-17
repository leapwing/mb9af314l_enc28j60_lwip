/*!
 ******************************************************************************
 **
 ** \file iperf_client.c
 **
 ** \brief iperf client demo for send only bandwidth testing
 **
 ** \author FSLA AE Team
 **
 ** \version V0.1
 **
 ** \date 2012-8-07
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

#include <stdio.h>
#include <string.h>
#include "board.h"
#include "debug.h"

#if LWIP_TCP
/*---------------------------------------------------------------------------*/
/* local defines                                                                                                                 */
/*---------------------------------------------------------------------------*/
//TCP server IP address
#define IPERF_SERVER_IP_ADDR0   192
#define IPERF_SERVER_IP_ADDR1   168
#define IPERF_SERVER_IP_ADDR2   1
#define IPERF_SERVER_IP_ADDR3   74

#define IPERF_SERVER_PORT       5001
#define DATA_LENGTH 1460
/*---------------------------------------------------------------------------*/
/* local data                                                                                                                    */
/*---------------------------------------------------------------------------*/
struct tcp_pcb *ipderf_client_pcb;
uint8_t iperfdata[DATA_LENGTH]={0};
uint8_t *piperfdata;
//uint8_t *piperfdata="TCP client is OK! Fujitsu MB9BF618 EVB testing!\n";

/* protocol states */
enum client_states
{
    ES_NONE = 0,
    ES_CONNECTED,
    ES_RECEIVED,
    ES_CLOSING,
};

/* structure to be passed as argument to the tcp callbacks */
struct client
{
    enum client_states state; /* connection status */
    struct tcp_pcb *pcb;          /* pointer to the current tcp_pcb */
    struct pbuf *p_tx;            /* pointer to pbuf to be transmitted */
};
 struct client ipferf_es;


/*---------------------------------------------------------------------------*/
/* local functions                                                                                                              */
/*---------------------------------------------------------------------------*/
static void iperf_client_connection_close(struct tcp_pcb *tpcb, struct client * es);
static void iperf_client_send(struct tcp_pcb *tpcb, struct client * es);
static err_t iperf_client_connected(void *arg, struct tcp_pcb *tpcb, err_t err);

/*---------------------------------------------------------------------------*/
/* global functions                                                          							  */
/*---------------------------------------------------------------------------*/
extern void DelayResolution100us(unsigned long Dly);

/*!
  * \brief Function called when TCP connection established
  * \param tpcb: pointer on the connection contol block
  * \param err: when connection correctly established err should be ERR_OK
  * \retval err_t: returned error
  */
static err_t iperf_client_connected(void *arg, struct tcp_pcb *tpcb, err_t err)
{
    struct client *es;
    LWIP_ASSERT("arg != NULL",arg != NULL);

    es = (struct client *)arg;

    if (err == ERR_OK)
    {
        es->state = ES_CONNECTED;

        /* send data */
        iperf_client_send(tpcb,es);
        return ERR_OK;

    }
    else
    {
        /* close connection */
        iperf_client_connection_close(tpcb, es);
    }

    return err;
}

/*!
  * \brief function used to send data
  * \param  tpcb: tcp control block
  * \param  es: pointer on structure of type client containing info on data
  *             to be sent
  * \retval None
  */
static void iperf_client_send(struct tcp_pcb *tpcb, struct client * es)
{
    struct pbuf *ptr;

    //while (wr_err == ERR_OK)
    {
        /* get pointer on pbuf from es structure */
        ptr = es->p_tx;
		
        /* enqueue data for transmission */
        tcp_write(tpcb, ptr->payload, ptr->len, TCP_WRITE_FLAG_COPY);		
    }
}

/*!
  * \brief This function is used to close the tcp connection with server
  * \param tpcb: tcp connection control block
  * \param es: pointer on echoclient structure
  * \retval None
  */
static void iperf_client_connection_close(struct tcp_pcb *tpcb, struct client * es )
{
    tcp_recv(tpcb, NULL);
    tcp_poll(tpcb, NULL,0);

    es->state=ES_CLOSING;
    es->pcb = NULL;
    if(es->p_tx!=NULL)
    {
        pbuf_free(es->p_tx);
        es->p_tx->next=NULL;
        es->p_tx=NULL;

    }

    /* close tcp connection */
    tcp_close(tpcb);

}
/*!
  * \brief  Init TCP client and initialize connection to the server
  * \param  None
  *
  */
void iperf_client_init(void)
{
    struct ip_addr DestIPaddr;
    uint16_t i;
    /* create new tcp pcb */
    ipderf_client_pcb = tcp_new();

    for(i=0;i<DATA_LENGTH;i++)
    {
        iperfdata[i]=0xaa;
    }
    piperfdata=iperfdata;
    if (ipderf_client_pcb != NULL)
    {
        IP4_ADDR( &DestIPaddr,IPERF_SERVER_IP_ADDR0, IPERF_SERVER_IP_ADDR1, IPERF_SERVER_IP_ADDR2, IPERF_SERVER_IP_ADDR3 );

        ipferf_es.state = ES_NONE;
        ipferf_es.pcb = ipderf_client_pcb;
        ipferf_es.p_tx = pbuf_alloc(PBUF_LINK, strlen((char const *)piperfdata), PBUF_RAM);//PBUF_ROM
		
        if(ipferf_es.p_tx!=NULL)
        {
            ipferf_es.p_tx->len= DATA_LENGTH;
            ipferf_es.p_tx->tot_len=DATA_LENGTH;
            ipferf_es.p_tx->payload=piperfdata;//data;
            ipferf_es.p_tx->next=ipferf_es.p_tx;
        }
        /* pass newly allocated es structure as argument to tpcb */
        tcp_arg(ipderf_client_pcb,(void *) &ipferf_es);
        /* connect to destination address/port */
        tcp_connect(ipderf_client_pcb,&DestIPaddr,IPERF_SERVER_PORT,iperf_client_connected);

    }
}

/*!
  * \brief  send out the fixed data
  * \param  None
  *
  */
void iperf_send(void)
{
    if(ipferf_es.state == ES_CONNECTED)
    {
        iperf_client_send(ipderf_client_pcb, &ipferf_es);
    }

}
#endif /* LWIP_TCP */
/*****************************************************************************/
/* END OF FILE */

