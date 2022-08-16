/*******************************************************************************
* Copyright (C) 2013-2016, Cypress Semiconductor Corporation or a              *
* subsidiary of Cypress Semiconductor Corporation.  All rights reserved.       *
*                                                                              *
* This software, including source code, documentation and related              *
* materials ("Software"), is owned by Cypress Semiconductor Corporation or     *
* one of its subsidiaries ("Cypress") and is protected by and subject to       *
* worldwide patent protection (United States and foreign), United States       *
* copyright laws and international treaty provisions. Therefore, you may use   *
* this Software only as provided in the license agreement accompanying the     *
* software package from which you obtained this Software ("EULA").             *
*                                                                              *
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,     *
* non-transferable license to copy, modify, and compile the                    *
* Software source code solely for use in connection with Cypress's             *
* integrated circuit products.  Any reproduction, modification, translation,   *
* compilation, or representation of this Software except as specified          *
* above is prohibited without the express written permission of Cypress.       *
*                                                                              *
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO                         *
* WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING,                         *
* BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED                                 *
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A                              *
* PARTICULAR PURPOSE. Cypress reserves the right to make                       *
* changes to the Software without notice. Cypress does not assume any          *
* liability arising out of the application or use of the Software or any       *
* product or circuit described in the Software. Cypress does not               *
* authorize its products for use in any products where a malfunction or        *
* failure of the Cypress product may reasonably be expected to result in       *
* significant property damage, injury or death ("High Risk Product"). By       *
* including Cypress's product in a High Risk Product, the manufacturer         *
* of such system or application assumes all risk of such use and in doing      *
* so agrees to indemnify Cypress against all liability.                        *
*******************************************************************************/
/******************************************************************************/
/** \file usblegacy.h
 **
 ** A detailed description is available at 
 ** @link UsbLegacyGroup USB backward support for older software stacks of PDL, L3 or older templates  description @endlink
 **
 ** History:
 **   - 2015-11-05  V1.0  MSc  First Version
 *****************************************************************************/

#ifndef __USBLEGACY_H__
#define __USBLEGACY_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif
    
/**
 ******************************************************************************
 ** \defgroup UsbLegacyGroup USB backward support for older software stacks of PDL, L3 or older templates 
 **   
 ******************************************************************************/
//@{
    
/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include "base_types.h"
#include "mcu.h"

/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define')                           */
/*****************************************************************************/

/*****************************************************************************/
/* Global type definitions ('typedef')                                       */
/*****************************************************************************/

      
/******************************************************************************
 * USB_MODULE
 ******************************************************************************/
/* USB_MODULE register bit fields */
typedef struct stc_usbn_hcnt_field
{
  __IO uint16_t HOST       : 1;
  __IO uint16_t URST       : 1;
  __IO uint16_t SOFIRE     : 1;
  __IO uint16_t DIRE       : 1;
  __IO uint16_t CNNIRE     : 1;
  __IO uint16_t CMPIRE     : 1;
  __IO uint16_t URIRE      : 1;
  __IO uint16_t RWKIRE     : 1;
  __IO uint16_t RETRY      : 1;
  __IO uint16_t CANCEL     : 1;
  __IO uint16_t SOFSTEP    : 1;
} stc_usbn_hcnt_field_t;

typedef struct stc_usbn_hcnt0_field
{
  __IO  uint8_t HOST       : 1;
  __IO  uint8_t URST       : 1;
  __IO  uint8_t SOFIRE     : 1;
  __IO  uint8_t DIRE       : 1;
  __IO  uint8_t CNNIRE     : 1;
  __IO  uint8_t CMPIRE     : 1;
  __IO  uint8_t URIRE      : 1;
  __IO  uint8_t RWKIRE     : 1;
} stc_usbn_hcnt0_field_t;

typedef struct stc_usbn_hcnt1_field
{
  __IO  uint8_t RETRY      : 1;
  __IO  uint8_t CANCEL     : 1;
  __IO  uint8_t SOFSTEP    : 1;
} stc_usbn_hcnt1_field_t;

typedef struct stc_usbn_hirq_field
{
  __IO  uint8_t SOFIRQ     : 1;
  __IO  uint8_t DIRQ       : 1;
  __IO  uint8_t CNNIRQ     : 1;
  __IO  uint8_t CMPIRQ     : 1;
  __IO  uint8_t URIRQ      : 1;
  __IO  uint8_t RWKIRQ     : 1;
        uint8_t RESERVED1  : 1;
  __IO  uint8_t TCAN       : 1;
} stc_usbn_hirq_field_t;

typedef struct stc_usbn_herr_field
{
  __IO  uint8_t HS0        : 1;
  __IO  uint8_t HS1        : 1;
  __IO  uint8_t STUFF      : 1;
  __IO  uint8_t TGERR      : 1;
  __IO  uint8_t CRC        : 1;
  __IO  uint8_t TOUT       : 1;
  __IO  uint8_t RERR       : 1;
  __IO  uint8_t LSTOF      : 1;
} stc_usbn_herr_field_t;

typedef struct stc_usbn_hstate_field
{
  __IO  uint8_t CSTAT      : 1;
  __IO  uint8_t TMODE      : 1;
  __IO  uint8_t SUSP       : 1;
  __IO  uint8_t SOFBUSY    : 1;
  __IO  uint8_t CLKSEL     : 1;
  __IO  uint8_t ALIVE      : 1;
} stc_usbn_hstate_field_t;

typedef struct stc_usbn_hfcomp_field
{
  __IO  uint8_t FRAMECOMP0 : 1;
  __IO  uint8_t FRAMECOMP1 : 1;
  __IO  uint8_t FRAMECOMP2 : 1;
  __IO  uint8_t FRAMECOMP3 : 1;
  __IO  uint8_t FRAMECOMP4 : 1;
  __IO  uint8_t FRAMECOMP5 : 1;
  __IO  uint8_t FRAMECOMP6 : 1;
  __IO  uint8_t FRAMECOMP7 : 1;
} stc_usbn_hfcomp_field_t;

typedef struct stc_usbn_hrtimer_field
{
  __IO uint16_t RTIMER0    : 1;
  __IO uint16_t RTIMER1    : 1;
  __IO uint16_t RTIMER2    : 1;
  __IO uint16_t RTIMER3    : 1;
  __IO uint16_t RTIMER4    : 1;
  __IO uint16_t RTIMER5    : 1;
  __IO uint16_t RTIMER6    : 1;
  __IO uint16_t RTIMER7    : 1;
  __IO uint16_t RTIMER8    : 1;
  __IO uint16_t RTIMER9    : 1;
  __IO uint16_t RTIMER10   : 1;
  __IO uint16_t RTIMER11   : 1;
  __IO uint16_t RTIMER12   : 1;
  __IO uint16_t RTIMER13   : 1;
  __IO uint16_t RTIMER14   : 1;
  __IO uint16_t RTIMER15   : 1;
} stc_usbn_hrtimer_field_t;

typedef struct stc_usbn_hrtimer0_field
{
  __IO  uint8_t RTIMER00   : 1;
  __IO  uint8_t RTIMER01   : 1;
  __IO  uint8_t RTIMER02   : 1;
  __IO  uint8_t RTIMER03   : 1;
  __IO  uint8_t RTIMER04   : 1;
  __IO  uint8_t RTIMER05   : 1;
  __IO  uint8_t RTIMER06   : 1;
  __IO  uint8_t RTIMER07   : 1;
} stc_usbn_hrtimer0_field_t;

typedef struct stc_usbn_hrtimer1_field
{
  __IO  uint8_t RTIMER10   : 1;
  __IO  uint8_t RTIMER11   : 1;
  __IO  uint8_t RTIMER12   : 1;
  __IO  uint8_t RTIMER13   : 1;
  __IO  uint8_t RTIMER14   : 1;
  __IO  uint8_t RTIMER15   : 1;
  __IO  uint8_t RTIMER16   : 1;
  __IO  uint8_t RTIMER17   : 1;
} stc_usbn_hrtimer1_field_t;

typedef struct stc_usbn_hrtimer2_field
{
  __IO  uint8_t RTIMER20   : 1;
  __IO  uint8_t RTIMER21   : 1;
  __IO  uint8_t RTIMER22   : 1;
} stc_usbn_hrtimer2_field_t;

typedef struct stc_usbn_hadr_field
{
  __IO  uint8_t ADDRESS0   : 1;
  __IO  uint8_t ADDRESS1   : 1;
  __IO  uint8_t ADDRESS2   : 1;
  __IO  uint8_t ADDRESS3   : 1;
  __IO  uint8_t ADDRESS4   : 1;
  __IO  uint8_t ADDRESS5   : 1;
  __IO  uint8_t ADDRESS6   : 1;
} stc_usbn_hadr_field_t;

typedef struct stc_usbn_heof_field
{
  __IO uint16_t EOF0       : 1;
  __IO uint16_t EOF1       : 1;
  __IO uint16_t EOF2       : 1;
  __IO uint16_t EOF3       : 1;
  __IO uint16_t EOF4       : 1;
  __IO uint16_t EOF5       : 1;
  __IO uint16_t EOF6       : 1;
  __IO uint16_t EOF7       : 1;
  __IO uint16_t EOF8       : 1;
  __IO uint16_t EOF9       : 1;
  __IO uint16_t EOF10      : 1;
  __IO uint16_t EOF11      : 1;
  __IO uint16_t EOF12      : 1;
  __IO uint16_t EOF13      : 1;
  __IO uint16_t EOF14      : 1;
  __IO uint16_t EOF15      : 1;
} stc_usbn_heof_field_t;

typedef struct stc_usbn_heof0_field
{
  __IO  uint8_t EOF00      : 1;
  __IO  uint8_t EOF01      : 1;
  __IO  uint8_t EOF02      : 1;
  __IO  uint8_t EOF03      : 1;
  __IO  uint8_t EOF04      : 1;
  __IO  uint8_t EOF05      : 1;
  __IO  uint8_t EOF06      : 1;
  __IO  uint8_t EOF07      : 1;
} stc_usbn_heof0_field_t;

typedef struct stc_usbn_heof1_field
{
  __IO  uint8_t EOF10      : 1;
  __IO  uint8_t EOF11      : 1;
  __IO  uint8_t EOF12      : 1;
  __IO  uint8_t EOF13      : 1;
  __IO  uint8_t EOF14      : 1;
  __IO  uint8_t EOF15      : 1;
} stc_usbn_heof1_field_t;

typedef struct stc_usbn_hframe_field
{
  __IO uint16_t FRAME0     : 1;
  __IO uint16_t FRAME1     : 1;
  __IO uint16_t FRAME2     : 1;
  __IO uint16_t FRAME3     : 1;
  __IO uint16_t FRAME4     : 1;
  __IO uint16_t FRAME5     : 1;
  __IO uint16_t FRAME6     : 1;
  __IO uint16_t FRAME7     : 1;
  __IO uint16_t FRAME8     : 1;
  __IO uint16_t FRAME9     : 1;
  __IO uint16_t FRAME10    : 1;
} stc_usbn_hframe_field_t;

typedef struct stc_usbn_hframe0_field
{
  __IO  uint8_t FRAME00    : 1;
  __IO  uint8_t FRAME01    : 1;
  __IO  uint8_t FRAME02    : 1;
  __IO  uint8_t FRAME03    : 1;
  __IO  uint8_t FRAME04    : 1;
  __IO  uint8_t FRAME05    : 1;
  __IO  uint8_t FRAME06    : 1;
  __IO  uint8_t FRAME07    : 1;
} stc_usbn_hframe0_field_t;

typedef struct stc_usbn_hframe1_field
{
  __IO  uint8_t FRAME10    : 1;
  __IO  uint8_t FRAME11    : 1;
  __IO  uint8_t FRAME12    : 1;
  __IO  uint8_t FRAME13    : 1;
} stc_usbn_hframe1_field_t;

typedef struct stc_usbn_htoken_field
{
  __IO  uint8_t ENDPT0     : 1;
  __IO  uint8_t ENDPT1     : 1;
  __IO  uint8_t ENDPT2     : 1;
  __IO  uint8_t ENDPT3     : 1;
  __IO  uint8_t TKNEN0     : 1;
  __IO  uint8_t TKNEN1     : 1;
  __IO  uint8_t TKNEN2     : 1;
  __IO  uint8_t TGGL       : 1;
} stc_usbn_htoken_field_t;

typedef struct stc_usbn_udcc_field
{
  __IO uint16_t PWC        : 1;
  __IO uint16_t RFBK       : 1;
       uint16_t RESERVED1  : 1;
  __IO uint16_t STALCLREN  : 1;
  __IO uint16_t USTP       : 1;
  __IO uint16_t HCONX      : 1;
  __IO uint16_t RESUM      : 1;
  __IO uint16_t RST        : 1;
} stc_usbn_udcc_field_t;

typedef struct stc_usbn_ep0c_field
{
  __IO uint16_t PKS00      : 1;
  __IO uint16_t PKS01      : 1;
  __IO uint16_t PKS02      : 1;
  __IO uint16_t PKS03      : 1;
  __IO uint16_t PKS04      : 1;
  __IO uint16_t PKS05      : 1;
  __IO uint16_t PKS06      : 1;
       uint16_t RESERVED1  : 2;
  __IO uint16_t STAL       : 1;
} stc_usbn_ep0c_field_t;

typedef struct stc_usbn_ep1c_field
{
  __IO uint16_t PKS10      : 1;
  __IO uint16_t PKS11      : 1;
  __IO uint16_t PKS12      : 1;
  __IO uint16_t PKS13      : 1;
  __IO uint16_t PKS14      : 1;
  __IO uint16_t PKS15      : 1;
  __IO uint16_t PKS16      : 1;
  __IO uint16_t PKS17      : 1;
  __IO uint16_t PKS18      : 1;
  __IO uint16_t STAL       : 1;
  __IO uint16_t NULE       : 1;
  __IO uint16_t DMAE       : 1;
  __IO uint16_t DIR        : 1;
  __IO uint16_t TYPE0      : 1;
  __IO uint16_t TYPE1      : 1;
  __IO uint16_t EPEN       : 1;
} stc_usbn_ep1c_field_t;

typedef struct stc_usbn_ep2c_field
{
  __IO uint16_t PKS20      : 1;
  __IO uint16_t PKS21      : 1;
  __IO uint16_t PKS22      : 1;
  __IO uint16_t PKS23      : 1;
  __IO uint16_t PKS24      : 1;
  __IO uint16_t PKS25      : 1;
  __IO uint16_t PKS26      : 1;
       uint16_t RESERVED1  : 2;
  __IO uint16_t STAL       : 1;
  __IO uint16_t NULE       : 1;
  __IO uint16_t DMAE       : 1;
  __IO uint16_t DIR        : 1;
  __IO uint16_t TYPE0      : 1;
  __IO uint16_t TYPE1      : 1;
  __IO uint16_t EPEN       : 1;
} stc_usbn_ep2c_field_t;

typedef struct stc_usbn_ep3c_field
{
  __IO uint16_t PKS30      : 1;
  __IO uint16_t PKS31      : 1;
  __IO uint16_t PKS32      : 1;
  __IO uint16_t PKS33      : 1;
  __IO uint16_t PKS34      : 1;
  __IO uint16_t PKS35      : 1;
  __IO uint16_t PKS36      : 1;
       uint16_t RESERVED1  : 2;
  __IO uint16_t STAL       : 1;
  __IO uint16_t NULE       : 1;
  __IO uint16_t DMAE       : 1;
  __IO uint16_t DIR        : 1;
  __IO uint16_t TYPE0      : 1;
  __IO uint16_t TYPE1      : 1;
  __IO uint16_t EPEN       : 1;
} stc_usbn_ep3c_field_t;

typedef struct stc_usbn_ep4c_field
{
  __IO uint16_t PKS40      : 1;
  __IO uint16_t PKS41      : 1;
  __IO uint16_t PKS42      : 1;
  __IO uint16_t PKS43      : 1;
  __IO uint16_t PKS44      : 1;
  __IO uint16_t PKS45      : 1;
  __IO uint16_t PKS46      : 1;
       uint16_t RESERVED1  : 2;
  __IO uint16_t STAL       : 1;
  __IO uint16_t NULE       : 1;
  __IO uint16_t DMAE       : 1;
  __IO uint16_t DIR        : 1;
  __IO uint16_t TYPE0      : 1;
  __IO uint16_t TYPE1      : 1;
  __IO uint16_t EPEN       : 1;
} stc_usbn_ep4c_field_t;

typedef struct stc_usbn_ep5c_field
{
  __IO uint16_t PKS50      : 1;
  __IO uint16_t PKS51      : 1;
  __IO uint16_t PKS52      : 1;
  __IO uint16_t PKS53      : 1;
  __IO uint16_t PKS54      : 1;
  __IO uint16_t PKS55      : 1;
  __IO uint16_t PKS56      : 1;
       uint16_t RESERVED1  : 2;
  __IO uint16_t STAL       : 1;
  __IO uint16_t NULE       : 1;
  __IO uint16_t DMAE       : 1;
  __IO uint16_t DIR        : 1;
  __IO uint16_t TYPE0      : 1;
  __IO uint16_t TYPE1      : 1;
  __IO uint16_t EPEN       : 1;
} stc_usbn_ep5c_field_t;

typedef struct stc_usbn_tmsp_field
{
  __IO uint16_t TMSP0      : 1;
  __IO uint16_t TMSP1      : 1;
  __IO uint16_t TMSP2      : 1;
  __IO uint16_t TMSP3      : 1;
  __IO uint16_t TMSP4      : 1;
  __IO uint16_t TMSP5      : 1;
  __IO uint16_t TMSP6      : 1;
  __IO uint16_t TMSP7      : 1;
  __IO uint16_t TMSP8      : 1;
  __IO uint16_t TMSP9      : 1;
  __IO uint16_t TMSP10     : 1;
} stc_usbn_tmsp_field_t;

typedef struct stc_usbn_udcs_field
{
  __IO  uint8_t CONF       : 1;
  __IO  uint8_t SETP       : 1;
  __IO  uint8_t WKUP       : 1;
  __IO  uint8_t BRST       : 1;
  __IO  uint8_t SOF        : 1;
  __IO  uint8_t SUSP       : 1;
} stc_usbn_udcs_field_t;

typedef struct stc_usbn_udcie_field
{
  __IO  uint8_t CONFIE     : 1;
  __IO  uint8_t CONFN      : 1;
  __IO  uint8_t WKUPIE     : 1;
  __IO  uint8_t BRSTIE     : 1;
  __IO  uint8_t SOFIE      : 1;
  __IO  uint8_t SUSPIE     : 1;
} stc_usbn_udcie_field_t;

typedef struct stc_usbn_ep0is_field
{
       uint16_t RESERVED1  : 10;
  __IO uint16_t DRQI       : 1;
       uint16_t RESERVED2  : 3;
  __IO uint16_t DRQIIE     : 1;
  __IO uint16_t BFINI      : 1;
} stc_usbn_ep0is_field_t;

typedef struct stc_usbn_ep0os_field
{
  __IO uint16_t SIZE0      : 1;
  __IO uint16_t SIZE1      : 1;
  __IO uint16_t SIZE2      : 1;
  __IO uint16_t SIZE3      : 1;
  __IO uint16_t SIZE4      : 1;
  __IO uint16_t SIZE5      : 1;
  __IO uint16_t SIZE6      : 1;
       uint16_t RESERVED1  : 2;
  __IO uint16_t SPK        : 1;
  __IO uint16_t DRQO       : 1;
       uint16_t RESERVED2  : 2;
  __IO uint16_t SPKIE      : 1;
  __IO uint16_t DRQOIE     : 1;
  __IO uint16_t BFINI      : 1;
} stc_usbn_ep0os_field_t;

typedef struct stc_usbn_ep1s_field
{
  __IO uint16_t SIZE10     : 1;
  __IO uint16_t SIZE11     : 1;
  __IO uint16_t SIZE12     : 1;
  __IO uint16_t SIZE13     : 1;
  __IO uint16_t SIZE14     : 1;
  __IO uint16_t SIZE15     : 1;
  __IO uint16_t SIZE16     : 1;
  __IO uint16_t SIZE17     : 1;
  __IO uint16_t SIZE18     : 1;
  __IO uint16_t SPK        : 1;
  __IO uint16_t DRQ        : 1;
  __IO uint16_t BUSY       : 1;
       uint16_t RESERVED1  : 1;
  __IO uint16_t SPKIE      : 1;
  __IO uint16_t DRQIE      : 1;
  __IO uint16_t BFINI      : 1;
} stc_usbn_ep1s_field_t;

typedef struct stc_usbn_ep2s_field
{
  __IO uint16_t SIZE20     : 1;
  __IO uint16_t SIZE21     : 1;
  __IO uint16_t SIZE22     : 1;
  __IO uint16_t SIZE23     : 1;
  __IO uint16_t SIZE24     : 1;
  __IO uint16_t SIZE25     : 1;
  __IO uint16_t SIZE26     : 1;
       uint16_t RESERVED1  : 2;
  __IO uint16_t SPK        : 1;
  __IO uint16_t DRQ        : 1;
  __IO uint16_t BUSY       : 1;
       uint16_t RESERVED2  : 1;
  __IO uint16_t SPKIE      : 1;
  __IO uint16_t DRQIE      : 1;
  __IO uint16_t BFINI      : 1;
} stc_usbn_ep2s_field_t;

typedef struct stc_usbn_ep4s_field
{
  __IO uint16_t SIZE40     : 1;
  __IO uint16_t SIZE41     : 1;
  __IO uint16_t SIZE42     : 1;
  __IO uint16_t SIZE43     : 1;
  __IO uint16_t SIZE44     : 1;
  __IO uint16_t SIZE45     : 1;
  __IO uint16_t SIZE46     : 1;
       uint16_t RESERVED1  : 2;
  __IO uint16_t SPK        : 1;
  __IO uint16_t DRQ        : 1;
  __IO uint16_t BUSY       : 1;
       uint16_t RESERVED2  : 1;
  __IO uint16_t SPKIE      : 1;
  __IO uint16_t DRQIE      : 1;
  __IO uint16_t BFINI      : 1;
} stc_usbn_ep4s_field_t;

typedef struct stc_usbn_ep5s_field
{
  __IO uint16_t SIZE50     : 1;
  __IO uint16_t SIZE51     : 1;
  __IO uint16_t SIZE52     : 1;
  __IO uint16_t SIZE53     : 1;
  __IO uint16_t SIZE54     : 1;
  __IO uint16_t SIZE55     : 1;
  __IO uint16_t SIZE56     : 1;
       uint16_t RESERVED1  : 2;
  __IO uint16_t SPK        : 1;
  __IO uint16_t DRQ        : 1;
  __IO uint16_t BUSY       : 1;
       uint16_t RESERVED2  : 1;
  __IO uint16_t SPKIE      : 1;
  __IO uint16_t DRQIE      : 1;
  __IO uint16_t BFINI      : 1;
} stc_usbn_ep5s_field_t;


typedef struct stc_usbn_epns_field
{
  __IO uint16_t SIZE       : 8;
  __IO uint16_t SPK        : 1;
  __IO uint16_t DRQ        : 1;
  __IO uint16_t BUSY       : 1;
       uint16_t RESERVED1  : 1;
  __IO uint16_t SPKIE      : 1;
  __IO uint16_t DRQIE      : 1;
  __IO uint16_t BFINI      : 1;
} stc_usbn_epns_field_t;

typedef struct stc_usbn_epns
{
    union {
        __IO uint16_t EPNS;
        stc_usbn_epns_field_t EPNS_f;
    };
    uint8_t RESERVED[2];
} stc_usbn_epns_t;

typedef struct stc_usbn_epnc_field
{
  __IO uint16_t PKS        : 9;
  __IO uint16_t STAL       : 1;
  __IO uint16_t NULE       : 1;
  __IO uint16_t DMAE       : 1;
  __IO uint16_t DIR        : 1;
  __IO uint16_t TYPE0      : 1;
  __IO uint16_t TYPE1      : 1;
  __IO uint16_t EPEN       : 1;
} stc_usbn_epnc_field_t;

typedef struct stc_usbn_epnc
{
    union {
        __IO uint16_t EPNC;
        stc_usbn_epnc_field_t EPNC_f;
    };
    uint8_t RESERVED[2];

} stc_usbn_epnc_t;

typedef struct stc_usbn_epdt
{
  union {
    __IO uint16_t EPNDT;
    struct {
      __IO  uint8_t EPNDTL;
      __IO  uint8_t EPNDTH;
    };
  };
  uint8_t RESERVED[2];
} stc_usbn_epdt_t;

/******************************************************************************
 * USB_MODULE
 ******************************************************************************/
/* USB channel 0 registers */
typedef struct stc_usbn
{
  union {
    union {
      __IO uint16_t HCNT;
      stc_usbn_hcnt_field_t HCNT_f;
    };
    struct {
      union {
        __IO  uint8_t HCNT0;
        stc_usbn_hcnt0_field_t HCNT0_f;
      };
      union {
        __IO  uint8_t HCNT1;
        stc_usbn_hcnt1_field_t HCNT1_f;
      };
    };
  };
        uint8_t RESERVED0[2];
  union {
    __IO  uint8_t HIRQ;
    stc_usbn_hirq_field_t HIRQ_f;
  };
  union {
    __IO  uint8_t HERR;
    stc_usbn_herr_field_t HERR_f;
  };
        uint8_t RESERVED1[2];
  union {
    __IO  uint8_t HSTATE;
    stc_usbn_hstate_field_t HSTATE_f;
  };
  union {
    __IO  uint8_t HFCOMP;
    stc_usbn_hfcomp_field_t HFCOMP_f;
  };
        uint8_t RESERVED2[2];
  union {
    union {
      __IO uint16_t HRTIMER;
      stc_usbn_hrtimer_field_t HRTIMER_f;
    };
    struct {
      union {
        __IO  uint8_t HRTIMER0;
        stc_usbn_hrtimer0_field_t HRTIMER0_f;
      };
      union {
        __IO  uint8_t HRTIMER1;
        stc_usbn_hrtimer1_field_t HRTIMER1_f;
      };
    };
  };
        uint8_t RESERVED3[2];
  union {
    __IO  uint8_t HRTIMER2;
    stc_usbn_hrtimer2_field_t HRTIMER2_f;
  };
  union {
    __IO  uint8_t HADR;
    stc_usbn_hadr_field_t HADR_f;
  };
        uint8_t RESERVED4[2];
  union {
    union {
      __IO uint16_t HEOF;
      stc_usbn_heof_field_t HEOF_f;
    };
    struct {
      union {
        __IO  uint8_t HEOF0;
        stc_usbn_heof0_field_t HEOF0_f;
      };
      union {
        __IO  uint8_t HEOF1;
        stc_usbn_heof1_field_t HEOF1_f;
      };
    };
  };
        uint8_t RESERVED5[2];
  union {
    union {
      __IO uint16_t HFRAME;
      stc_usbn_hframe_field_t HFRAME_f;
    };
    struct {
      union {
        __IO  uint8_t HFRAME0;
        stc_usbn_hframe0_field_t HFRAME0_f;
      };
      union {
        __IO  uint8_t HFRAME1;
        stc_usbn_hframe1_field_t HFRAME1_f;
      };
    };
  };
        uint8_t RESERVED6[2];
  union {
    __IO  uint8_t HTOKEN;
    stc_usbn_htoken_field_t HTOKEN_f;
  };
        uint8_t RESERVED7[3];
  union {
    __IO uint16_t UDCC;
    stc_usbn_udcc_field_t UDCC_f;
  };
        uint8_t RESERVED8[2];
  union {
    __IO uint16_t EP0C;
    stc_usbn_ep0c_field_t EP0C_f;
  };
        uint8_t RESERVED9[2];
  union {
    __IO uint16_t EP1C;
    stc_usbn_ep1c_field_t EP1C_f;
  };
        uint8_t RESERVED10[2];
  union {
    __IO uint16_t EP2C;
    stc_usbn_ep2c_field_t EP2C_f;
  };
        uint8_t RESERVED11[2];
  union {
    __IO uint16_t EP3C;
    stc_usbn_ep3c_field_t EP3C_f;
  };
        uint8_t RESERVED12[2];
  union {
    __IO uint16_t EP4C;
    stc_usbn_ep4c_field_t EP4C_f;
  };
        uint8_t RESERVED13[2];
  union {
    __IO uint16_t EP5C;
    stc_usbn_ep5c_field_t EP5C_f;
  };
        uint8_t RESERVED14[2];
  union {
    __IO uint16_t TMSP;
    stc_usbn_tmsp_field_t TMSP_f;
  };
        uint8_t RESERVED15[2];
  union {
    __IO  uint8_t UDCS;
    stc_usbn_udcs_field_t UDCS_f;
  };
  union {
    __IO  uint8_t UDCIE;
    stc_usbn_udcie_field_t UDCIE_f;
  };
        uint8_t RESERVED16[2];
  union {
    __IO uint16_t EP0IS;
    stc_usbn_ep0is_field_t EP0IS_f;
  };
        uint8_t RESERVED17[2];
  union {
    __IO uint16_t EP0OS;
    stc_usbn_ep0os_field_t EP0OS_f;
  };
        uint8_t RESERVED18[2];
  union {
    __IO uint16_t EP1S;
    stc_usbn_ep1s_field_t EP1S_f;
  };
        uint8_t RESERVED19[2];
  union {
    __IO uint16_t EP2S;
    stc_usbn_ep2s_field_t EP2S_f;
  };
        uint8_t RESERVED20[2];
  __IO uint16_t EP3S;
        uint8_t RESERVED21[2];
  union {
    __IO uint16_t EP4S;
    stc_usbn_ep4s_field_t EP4S_f;
  };
        uint8_t RESERVED22[2];
  union {
    __IO uint16_t EP5S;
    stc_usbn_ep5s_field_t EP5S_f;
  };
        uint8_t RESERVED23[2];
  union {
    __IO uint16_t EP0DT;
    struct {
      __IO  uint8_t EP0DTL;
      __IO  uint8_t EP0DTH;
    };
  };
        uint8_t RESERVED24[2];
  union {
    __IO uint16_t EP1DT;
    struct {
      __IO  uint8_t EP1DTL;
      __IO  uint8_t EP1DTH;
    };
  };
        uint8_t RESERVED25[2];
  union {
    __IO uint16_t EP2DT;
    struct {
      __IO  uint8_t EP2DTL;
      __IO  uint8_t EP2DTH;
    };
  };
        uint8_t RESERVED26[2];
  union {
    __IO uint16_t EP3DT;
    struct {
      __IO  uint8_t EP3DTL;
      __IO  uint8_t EP3DTH;
    };
  };
        uint8_t RESERVED27[2];
  union {
    __IO uint16_t EP4DT;
    struct {
      __IO  uint8_t EP4DTL;
      __IO  uint8_t EP4DTH;
    };
  };
        uint8_t RESERVED28[2];
  union {
    __IO uint16_t EP5DT;
    struct {
      __IO  uint8_t EP5DTL;
      __IO  uint8_t EP5DTH;
    };
  };
} stc_usbn_t;

#if (defined(_MB9A110K_H_) || defined(_MB9A110L_H_) || defined(_MB9A110M_H_) || defined(_MB9A110N_H_) || \
     defined(_MB9A130K_H_) || defined(_MB9A130L_H_) || defined(_MB9A130M_H_) || defined(_MB9A130N_H_) || \
                              defined(_MB9A310L_H_) || defined(_MB9A310M_H_) || defined(_MB9A310N_H_) || \
                                                       defined(_MB9B500N_H_) || defined(_MB9B500R_H_))                           
    #define USBLIB_USB0_IRQn USBF_IRQn
    #define USBLIB_USB0_DEVHOST_IRQn USBF_USBH_IRQn
#elif defined(FM4_USB0_BASE) || defined(FM4_USB1_BASE)
    #define USBLIB_USB0_IRQn USB0_IRQn
    #define USBLIB_USB0_DEVHOST_IRQn USB0_HOST_IRQn
    #if FM4_DEVICE_TYPE == 3
        #define USBLIB_USB1_IRQn USB1_HDMICEC0_IRQn
        #define USBLIB_USB1_DEVHOST_IRQn USB1_HOST_HDMICEC1_IRQn
    #else
        #define USBLIB_USB1_IRQn USB1_IRQn
        #define USBLIB_USB1_DEVHOST_IRQn USB1_HOST_IRQn
    #endif
#else
    #define USBLIB_USB0_IRQn USB0F_IRQn 
    #define USBLIB_USB0_DEVHOST_IRQn USB0F_USB0H_IRQn 
    #define USBLIB_USB1_IRQn USB1F_IRQn 
    #define USBLIB_USB1_DEVHOST_IRQn USB1F_USB1H_IRQn
#endif

/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source)           */
/*****************************************************************************/

/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source)             */
/*****************************************************************************/


#ifdef __cplusplus
}
#endif

//@} // UsbLegacyGroup

#endif /* __USBLEGACY_H__*/

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/

