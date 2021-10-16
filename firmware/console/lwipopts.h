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
#ifndef LWIP_HDR_LWIPOPTS_H__
#define LWIP_HDR_LWIPOPTS_H__

/* Fixed settings mandated by the ChibiOS integration.*/
#include "static_lwipopts.h"

#define DEFAULT_TCP_RECVMBOX_SIZE 40
#define DEFAULT_ACCEPTMBOX_SIZE 4

/* Optional, application-specific settings.*/
#if !defined(TCPIP_MBOX_SIZE)
#define TCPIP_MBOX_SIZE                 MEMP_NUM_PBUF
#endif
#if !defined(TCPIP_THREAD_STACKSIZE)
#define TCPIP_THREAD_STACKSIZE          1024
#endif

/* Use ChibiOS specific priorities. */
#if !defined(TCPIP_THREAD_PRIO)
#define TCPIP_THREAD_PRIO               (NORMALPRIO + 5)
#endif
#if !defined(LWIP_THREAD_PRIORITY)
#define LWIP_THREAD_PRIORITY            (NORMALPRIO + 4)
#endif

#define LWIP_LINK_POLL_INTERVAL   TIME_S2I(1)
#define LWIP_IPADDR(p)            IP4_ADDR(p, 192, 168, 10, 1)
#define LWIP_GATEWAY(p)           IP4_ADDR(p, 192, 168, 10, 1)

#define LWIP_COMPAT_SOCKETS 0

#define LWIP_RAW 0
#define LWIP_TCP 1
#define LWIP_UDP 0
#define LWIP_STATS 0

// In theory we're only talking to a single PC, so we don't need a big ARP table
#define ARP_TABLE_SIZE 4

// ascii `rus`
#define LWIP_ETHADDR_0 0x72
#define LWIP_ETHADDR_1 0x75
#define LWIP_ETHADDR_2 0x73
#define LWIP_ETHADDR_3 0x12
#define LWIP_ETHADDR_4 0x34
#define LWIP_ETHADDR_5 0x56

#endif /* LWIP_HDR_LWIPOPTS_H__ */
