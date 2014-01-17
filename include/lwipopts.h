/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Simon Goldschmidt
 *
 */
#ifndef __LWIPOPTS_H__
#define __LWIPOPTS_H__

/* Prevent having to link sys_arch.c (we don't test the API layers in unit tests) */
#define NO_SYS                          1
#define LWIP_NETCONN                    0
#define LWIP_SOCKET                     0
#define LWIP_COMPAT_SOCKETS             0
#define LWIP_POSIX_SOCKETS_IO_NAMES     0

/* Minimal changes to opt.h required for tcp unit tests: */
#define MEMP_MEM_MALLOC                 0
#define MEM_ALIGNMENT                   4
#define MEM_SIZE                        (8*1024)
#define TCP_MSS							1500	
#define TCP_SND_QUEUELEN                (4 * TCP_SND_BUF / TCP_MSS)//((4 * (TCP_SND_BUF) + (TCP_MSS - 1))/(TCP_MSS))
//#define MEMP_NUM_TCP_PCB                2
//#define MEMP_NUM_TCP_PCB_LISTEN         2
#define MEMP_NUM_TCP_SEG                TCP_SND_QUEUELEN
#define TCP_SND_BUF                     (4 * TCP_MSS)
#define TCP_WND                         (4 * TCP_MSS)
//#define LWIP_ALLOW_MEM_FREE_FROM_OTHER_CONTEXT 1
#define LWIP_HAVE_SLIPIF                1
#define MEMP_NUM_PBUF                   16
#define PBUF_POOL_SIZE                  8
//#define MEM_USE_POOLS                   0
//#define MEMP_USE_CUSTOM_POOLS           0

/* Minimal changes to opt.h required for dhcp unit tests: */
#define LWIP_DHCP                       0
//#define LWIP_NETIF_HOSTNAME             0

/* Minimal changes to opt.h required for statistics unit tests: */
#define LWIP_STATS                      0
#define LWIP_STATS_DISPLAY              0

/* Minimal changes to opt.h required for dns unit tests: */
#define LWIP_DNS                        0
#define DNS_LOCAL_HOSTLIST				0
#define DNS_LOCAL_HOSTLIST_IS_DYNAMIC   0

/*
The Fujitsu FM3 devices with Ethernet support computing and verifying the IP, UDP, TCP and ICMP checksums by hardware:
 - To use this feature let the following define uncommented.
 - To disable it and process by CPU, please comment out the definition of CHECKSUM_BY_HARDWARE.
*/
//#define CHECKSUM_BY_HARDWARE

#ifdef CHECKSUM_BY_HARDWARE
  /* CHECKSUM_GEN_IP==0: Generate checksums by hardware for outgoing IP packets.*/
  #define CHECKSUM_GEN_IP                 0
  /* CHECKSUM_GEN_UDP==0: Generate checksums by hardware for outgoing UDP packets.*/
  #define CHECKSUM_GEN_UDP                0
  /* CHECKSUM_GEN_TCP==0: Generate checksums by hardware for outgoing TCP packets.*/
  #define CHECKSUM_GEN_TCP                0
  /* CHECKSUM_CHECK_IP==0: Check checksums by hardware for incoming IP packets.*/
  #define CHECKSUM_CHECK_IP               0
  /* CHECKSUM_CHECK_UDP==0: Check checksums by hardware for incoming UDP packets.*/
  #define CHECKSUM_CHECK_UDP              0
  /* CHECKSUM_CHECK_TCP==0: Check checksums by hardware for incoming TCP packets.*/
  #define CHECKSUM_CHECK_TCP              0
#else
  /* CHECKSUM_GEN_IP==1: Generate checksums in software for outgoing IP packets.*/
  #define CHECKSUM_GEN_IP                 1
  /* CHECKSUM_GEN_UDP==1: Generate checksums in software for outgoing UDP packets.*/
  #define CHECKSUM_GEN_UDP                1
  /* CHECKSUM_GEN_TCP==1: Generate checksums in software for outgoing TCP packets.*/
  #define CHECKSUM_GEN_TCP                1
  /* CHECKSUM_CHECK_IP==1: Check checksums in software for incoming IP packets.*/
  #define CHECKSUM_CHECK_IP               1
  /* CHECKSUM_CHECK_UDP==1: Check checksums in software for incoming UDP packets.*/
  #define CHECKSUM_CHECK_UDP              1
  /* CHECKSUM_CHECK_TCP==1: Check checksums in software for incoming TCP packets.*/
  #define CHECKSUM_CHECK_TCP              1
#endif

//#define LWIP_DEBUG 					  LWIP_DBG_ON
//#define NETIF_DEBUG                     LWIP_DBG_ON
//#define DNS_DEBUG                       LWIP_DBG_ON
//#define UDP_DEBUG                       LWIP_DBG_ON
//#define DHCP_DEBUG                      LWIP_DBG_ON
//#define TCP_QLEN_DEBUG 				  LWIP_DBG_ON
//#define TCP_OUTPUT_DEBUG				  LWIP_DBG_ON

/**
 * HTTPD_DEBUG: Enable debugging for HTTPD.
 */
//#define HTTPD_DEBUG                     LWIP_DBG_ON


#define LWIP_NOASSERT 1
/*
   -----------------------------------------
   ---------- HTTP server options ----------
   -----------------------------------------
*/

#define TCP_WAIT_MSL 60000UL
//#define TCP_MSL 5000UL
#define LWIP_HTTPD_SSI 1
#define LWIP_HTTPD_CGI 1
#define LWIP_HTTPD_SUPPORT_POST   1
#define HTTPD_SERVER_AGENT "LwIP/1.4.1 HTTP server running on a Fujitsu FM3"

/*
   -----------------------------------------
   ---------- SMTP client options ----------
   -----------------------------------------
*/
#define SMTP_DEBUG LWIP_DBG_OFF
#define SMTP_DEBUG_TRACE 1

#endif /* __LWIPOPTS_H__ */
