/*!
 ******************************************************************************
 **
 ** \file tcp_client.c
 **
 ** \brief tcp client demo
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
#define DEST_IP_ADDR0   192
#define DEST_IP_ADDR1   168
#define DEST_IP_ADDR2   1
#define DEST_IP_ADDR3   74

#define DEST_PORT       8

/*---------------------------------------------------------------------------*/
/* local data                                                                                                                    */
/*---------------------------------------------------------------------------*/
struct tcp_pcb *client_pcb;
uint8_t *pdata="TCP client is OK! Fujitsu MB9AF314 EVB testing!\n";

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
 struct client es;


/*---------------------------------------------------------------------------*/
/* local functions                                                                                                              */
/*---------------------------------------------------------------------------*/
static err_t tcp_client_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
static void tcp_client_connection_close(struct tcp_pcb *tpcb, struct client * es);
static err_t tcp_client_poll(void *arg, struct tcp_pcb *tpcb);
static void tcp_client_send(struct tcp_pcb *tpcb, struct client * es);
static err_t tcp_client_connected(void *arg, struct tcp_pcb *tpcb, err_t err);

/*---------------------------------------------------------------------------*/
/* global functions                                                          							  */
/*---------------------------------------------------------------------------*/


/*!
  * \brief Function called when TCP connection established
  * \param tpcb: pointer on the connection contol block
  * \param err: when connection correctly established err should be ERR_OK 
  * \retval err_t: returned error 
  */
static err_t tcp_client_connected(void *arg, struct tcp_pcb *tpcb, err_t err)
{
    struct client *es;
    LWIP_ASSERT("arg != NULL",arg != NULL);

    es = (struct client *)arg;
  
    if (err == ERR_OK)   
    {
        es->state = ES_CONNECTED;

        /* initialize LwIP tcp_recv callback function */ 
        tcp_recv(tpcb, tcp_client_recv);

        /* initialize LwIP tcp_poll callback function */
        tcp_poll(tpcb, tcp_client_poll, 2);///1 second interval
        
        /* send data */
        tcp_client_send(tpcb,es);

        return ERR_OK;

    }
    else
    {
        /* close connection */
        tcp_client_connection_close(tpcb, es);
    }
    
    return err;
}
    
/*!
  * \brief tcp_receiv callback
  * \param arg: argument to be passed to receive callback 
  * \param tpcb: tcp connection control block 
  * \param err: receive error code 
  * \retval err_t: retuned error  
  */
static err_t tcp_client_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{ 
    struct client *es;
    err_t ret_err;
    uint8_t i;
    uint8_t check[4]={0};
    uint8_t *payload=p->payload;


    LWIP_ASSERT("arg != NULL",arg != NULL);
  
    es = (struct client *)arg;
  
    /* receive an empty tcp frame from server => close connection */
    if (p == NULL)
    {
        /* remote host closed connection */
        es->state = ES_CLOSING;
        if(es->p_tx == NULL)
        {
            /* we're done sending, close connection */
            tcp_client_connection_close(tpcb, es);
        }
        else
        {    
            /* send remaining data*/
            tcp_client_send(tpcb, es);
        }
        ret_err = ERR_OK;
    }   
    /* else : a non empty frame was received from echo server but for some reason err != ERR_OK */
    else if(err != ERR_OK)
    {
        /* free received pbuf*/
        if (p != NULL)
        {
            pbuf_free(p);
        }
        ret_err = err;
    }
    else if(es->state == ES_CONNECTED)
    {
        for(i=0;i<5;i++)
        {
            check[i]=*payload;
            payload++;
        }

        /* Acknowledge data reception */
        tcp_recved(tpcb, p->tot_len);  
        
        if(p->tot_len==sizeof(check))
        if((check[0]=='s')&&(check[1]=='t')&&(check[2]=='o')&&(check[3]=='p'))
        {          
            pbuf_free(p); 
            tcp_client_connection_close(tpcb, es);
            tcp_abort(tpcb);    //reset the connection of Client to server to finish the session

            for(i=0;i<4;i++)
            {
                check[i]=0;
            }
        }
         
        ret_err = ERR_OK;
    }
    /* data received when connection already closed */
    else
    {
        /* Acknowledge data reception */
        tcp_recved(tpcb, p->tot_len);
        
        /* free pbuf and do nothing */
        pbuf_free(p);
        ret_err = ERR_OK;
    }
    return ret_err;
}

/*!
  * \brief function used to send data
  * \param  tpcb: tcp control block
  * \param  es: pointer on structure of type client containing info on data 
  *             to be sent
  * \retval None 
  */
static void tcp_client_send(struct tcp_pcb *tpcb, struct client * es)
{
    struct pbuf *ptr;
    err_t wr_err = ERR_OK;
    
    /* get pointer on pbuf from es structure */
    ptr = es->p_tx;

    /* enqueue data for transmission */
    wr_err = tcp_write(tpcb, ptr->payload, ptr->len, TCP_WRITE_FLAG_COPY);

    if (wr_err == ERR_OK)
    {      
        /* continue with next pbuf in chain (if any) */
        es->p_tx = ptr->next;

        if(es->p_tx != NULL)
        {
            /* increment reference count for es->p */
            pbuf_ref(es->p_tx);
        }

        /* free pbuf: will free pbufs up to es->p (because es->p has a reference count > 0) */
        //pbuf_free(ptr);

    }
    else if(wr_err == ERR_MEM)
    {
        /* we are low on memory, try later, defer to poll */
        es->p_tx = ptr;
    }
    else
    {

    }
  }

/*!
  * \brief  This function implements the tcp_poll callback function
  * \param  arg: pointer on argument passed to callback
  * \param  tpcb: tcp connection control block
  * \retval err_t: error code
  */
static err_t tcp_client_poll(void *arg, struct tcp_pcb *tpcb)
{
    err_t ret_err;
    struct client *es;

    es = (struct client*)arg;
    if (es != NULL)
    {
        if (es->p_tx != NULL)
        {
            /* there is a remaining pbuf (chain) , try to send data */
            tcp_client_send(tpcb, es);
        }
        else
        {
            /* no remaining pbuf (chain)  */
            if(es->state == ES_CLOSING)
            {
                /* close tcp connection */
                tcp_client_connection_close(tpcb, es);
            }
        }
        ret_err = ERR_OK;
    }
    else
    {
        /* nothing to be done */
        tcp_abort(tpcb);
        ret_err = ERR_ABRT;
    }
    return ret_err;
}

/*!
  * \brief This function is used to close the tcp connection with server
  * \param tpcb: tcp connection control block
  * \param es: pointer on echoclient structure
  * \retval None
  */
static void tcp_client_connection_close(struct tcp_pcb *tpcb, struct client * es )
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
void tcp_client_init(void)
{
    struct ip_addr DestIPaddr;

    /* create new tcp pcb */
    client_pcb = tcp_new();

    if (client_pcb != NULL)
    {
        IP4_ADDR( &DestIPaddr, DEST_IP_ADDR0, DEST_IP_ADDR1, DEST_IP_ADDR2, DEST_IP_ADDR3 );

        es.state = ES_NONE;
        es.pcb = client_pcb;
        es.p_tx = pbuf_alloc(PBUF_LINK, strlen((char const *)pdata), PBUF_ROM);//,PBUF_RAM
        
        if(es.p_tx!=NULL)
        {
            es.p_tx->len= strlen((char const *)pdata);    
            es.p_tx->tot_len=strlen((char const *)pdata);
            es.p_tx->payload=pdata;//data;
            es.p_tx->next=es.p_tx;;
        }
        /* pass newly allocated es structure as argument to tpcb */
        tcp_arg(client_pcb,(void *) &es);
        /* connect to destination address/port */
        tcp_connect(client_pcb,&DestIPaddr,DEST_PORT,tcp_client_connected);
        
    }
}

#endif /* LWIP_TCP */
/*****************************************************************************/
/* END OF FILE */

