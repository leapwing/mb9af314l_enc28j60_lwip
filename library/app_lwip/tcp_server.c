/*!
 ******************************************************************************
 **
 ** \file tcp_server.c
 **
 ** \brief tcp server for  throughput testing and pressure testing via speedtest tool on the PC
 **
 ** \author FSLA AE Team
 **
 ** \version V0.1
 **
 ** \date 2012-8-03
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
/* include files                                                             */
/*---------------------------------------------------------------------------*/


#include "lwip/debug.h"
#include "lwip/stats.h"
#include "lwip/tcp.h"

/*---------------------------------------------------------------------------*/
/* local defines                                                             */
/*---------------------------------------------------------------------------*/
#define TCP_SERVER_PORT 5000

/*---------------------------------------------------------------------------*/
/* local data                                                                */
/*---------------------------------------------------------------------------*/
#if LWIP_TCP

static struct tcp_pcb *tcp_server_pcb;

typedef enum tcpsever_states
{
    ES_NONE = 0,
    ES_ACCEPTED,
    ES_RECEIVED,
    ES_CLOSING
}SERVER_STATES;

struct tcpsever_state
{
    SERVER_STATES state;
    u8_t retries;
    struct tcp_pcb *pcb;
    /* pbuf (chain) to recycle */
    struct pbuf *p;
};

/*---------------------------------------------------------------------------*/
/* local functions                                                           */
/*---------------------------------------------------------------------------*/
static err_t tcpsever_accept(void *arg, struct tcp_pcb *newpcb, err_t err);
static err_t tcpsever_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
static void tcpsever_error(void *arg, err_t err);
#if 0
static err_t tcpsever_poll(void *arg, struct tcp_pcb *tpcb);
#endif
static err_t tcpsever_sent(void *arg, struct tcp_pcb *tpcb, u16_t len);
static void tcpsever_send(struct tcp_pcb *tpcb, struct tcpsever_state *es);
static void tcpsever_close(struct tcp_pcb *tpcb, struct tcpsever_state *es);

void tcp_server_init(void)
{
    err_t err;

    tcp_server_pcb = tcp_new();
    if (tcp_server_pcb != NULL)
    {
        err = tcp_bind(tcp_server_pcb, IP_ADDR_ANY, TCP_SERVER_PORT);
        if (err == ERR_OK)
        {
            tcp_server_pcb = tcp_listen(tcp_server_pcb);
            tcp_accept(tcp_server_pcb, tcpsever_accept);
        }
        else
        {
            /* abort? output diagnostic? */
        }
    }
    else
    {
        /* abort? output diagnostic? */
    }
}


err_t tcpsever_accept(void *arg, struct tcp_pcb *newpcb, err_t err)
{
    err_t ret_err;
    struct tcpsever_state *es;

    LWIP_UNUSED_ARG(arg);
    LWIP_UNUSED_ARG(err);

    /* commonly observed practive to call tcp_setprio(), why? */
    tcp_setprio(newpcb, TCP_PRIO_MIN);

    es = (struct tcpsever_state *)mem_malloc(sizeof(struct tcpsever_state));
    if (es != NULL)
    {
        es->state = ES_ACCEPTED;
        es->pcb = newpcb;
        es->retries = 0;
        es->p = NULL;
        /* pass newly allocated es to our callbacks */
        tcp_arg(newpcb, es);
        tcp_recv(newpcb, tcpsever_recv);
        tcp_err(newpcb, tcpsever_error);
#if 0
        tcp_poll(newpcb, tcpsever_poll, 0);
#endif
        ret_err = ERR_OK;
    }
    else
    {
        ret_err = ERR_MEM;
    }
    return ret_err;
}

err_t tcpsever_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
    struct tcpsever_state *es;
    err_t ret_err;

    LWIP_ASSERT("arg != NULL",arg != NULL);
    es = (struct tcpsever_state *)arg;
    if (p == NULL)
    {
        /* remote host closed connection */
        es->state = ES_CLOSING;
        if(es->p == NULL)
        {
            /* we're done sending, close it */
            tcpsever_close(tpcb, es);
        }
        else
        {
            /* we're not done yet */
            tcp_sent(tpcb, tcpsever_sent);
            tcpsever_send(tpcb, es);
        }
        ret_err = ERR_OK;
    }
    else if(err != ERR_OK)
    {
        /* cleanup, for unkown reason */
        if (p != NULL)
        {
            es->p = NULL;
            pbuf_free(p);
        }
        ret_err = err;
    }
    else if(es->state == ES_ACCEPTED)
    {
        es->p = p;
        tcp_sent(tpcb, tcpsever_sent);
        tcpsever_send(tpcb, es);
        ret_err = ERR_OK;
    }
    else if(es->state == ES_CLOSING)
    {
        /* odd case, remote side closing twice, trash data */
        tcp_recved(tpcb, p->tot_len);
        es->p = NULL;
        pbuf_free(p);
        ret_err = ERR_OK;
    }
    else
    {
        /* unkown es->state, trash data  */
        tcp_recved(tpcb, p->tot_len);
        es->p = NULL;
        pbuf_free(p);
        ret_err = ERR_OK;
    }
    return ret_err;
}

void tcpsever_error(void *arg, err_t err)
{
    struct tcpsever_state *es;

    LWIP_UNUSED_ARG(err);

    es = (struct tcpsever_state *)arg;
    if (es != NULL)
    {
        mem_free(es);
    }
}
#if 0
err_t tcpsever_poll(void *arg, struct tcp_pcb *tpcb)
{
    err_t ret_err;
    struct tcpsever_state *es;

    es = (struct tcpsever_state *)arg;
    if (es != NULL)
    {
        if (es->p != NULL)
        {
            /* there is a remaining pbuf (chain)  */
            tcp_sent(tpcb, tcpsever_sent);
            tcpsever_send(tpcb, es);
        }
        else
        {
            /* no remaining pbuf (chain)  */
            if(es->state == ES_CLOSING)
            {
                tcpsever_close(tpcb, es);
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
#endif
err_t tcpsever_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
    struct tcpsever_state *es;

    LWIP_UNUSED_ARG(len);

    es = (struct tcpsever_state *)arg;
    es->retries = 0;

    if(es->p != NULL)
    {
        /* still got pbufs to send */
        tcp_sent(tpcb, tcpsever_sent);
        tcpsever_send(tpcb, es);
    }
    else
    {
        /* no more pbufs to send */
        if(es->state == ES_CLOSING)
        {
            tcpsever_close(tpcb, es);
        }
    }
    return ERR_OK;
}

void tcpsever_send(struct tcp_pcb *tpcb, struct tcpsever_state *es)
{
    struct pbuf *ptr;
    err_t wr_err = ERR_OK;

    while ((wr_err == ERR_OK) &&
         (es->p != NULL) &&
         (es->p->len <= tcp_sndbuf(tpcb)))
    {
        ptr = es->p;

        /* enqueue data for transmission */
        wr_err = tcp_write(tpcb, ptr->payload, ptr->len, TCP_WRITE_FLAG_COPY);
        if (wr_err == ERR_OK)
        {
            u16_t plen;
            u8_t freed;

            plen = ptr->len;
            /* continue with next pbuf in chain (if any) */
            es->p = ptr->next;
            if(es->p != NULL)
            {
                /* new reference! */
                pbuf_ref(es->p);
            }
            /* chop first pbuf from chain */
            do
            {
                /* try hard to free pbuf */
                freed = pbuf_free(ptr);
            }
            while(freed == 0);
            /* we can read more data now */
            tcp_recved(tpcb, plen);

        }
        else if(wr_err == ERR_MEM)
        {
            /* we are low on memory, try later / harder, defer to poll */
            es->p = ptr;
        }
        else
        {
            /* other problem ?? */
        }
    }
}

void tcpsever_close(struct tcp_pcb *tpcb, struct tcpsever_state *es)
{
    tcp_arg(tpcb, NULL);
    tcp_sent(tpcb, NULL);
    tcp_recv(tpcb, NULL);
    tcp_err(tpcb, NULL);
    tcp_poll(tpcb, NULL, 0);

    if (es != NULL)
    {
        mem_free(es);
    }
    tcp_close(tpcb);
}

#endif /* LWIP_TCP */
/*****************************************************************************/
/* END OF FILE */

