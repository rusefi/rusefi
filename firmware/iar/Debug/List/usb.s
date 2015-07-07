///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:10:05 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       src\usb.c                                            /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       src\usb.c -lCN F:\stuff\rusefi_sourceforge\firmware\ /
//                       iar\Debug\List\ -lA F:\stuff\rusefi_sourceforge\firm /
//                       ware\iar\Debug\List\ -o F:\stuff\rusefi_sourceforge\ /
//                       firmware\iar\Debug\Obj\ --no_cse --no_unroll         /
//                       --no_inline --no_code_motion --no_tbaa               /
//                       --no_clustering --no_scheduling --debug              /
//                       --endian=little --cpu=Cortex-M4 -e --fpu=VFPv4_sp    /
//                       --dlib_config "C:\Program Files (x86)\IAR            /
//                       Systems\Embedded Workbench                           /
//                       6.5\arm\INC\c\DLib_Config_Normal.h"                  /
//                       --misrac2004=1.2-1.4,2.3,3.4,4.2,6.5-7,8.2-8.3,9.3,  /
//                       12.3,13.6-13,15.5,17.3,19.17,20.7-20.9,20.11 -I      /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\ -I      /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\kernel\include\ -I F:\stuff\rusefi_sourceforge\fi /
//                       rmware\iar\..\chibios\os\ports\common\ARMCMx\ -I     /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\ports\common\ARMCMx\CMSIS\include\ -I             /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\ports\IAR\ARMCMx\ -I F:\stuff\rusefi_sourceforge\ /
//                       firmware\iar\..\chibios\os\ports\IAR\ARMCMx\STM32F4x /
//                       x\ -I F:\stuff\rusefi_sourceforge\firmware\iar\..\ch /
//                       ibios\os\hal\include\ -I F:\stuff\rusefi_sourceforge /
//                       \firmware\iar\..\chibios\os\hal\platforms\STM32\ -I  /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32\DMAv1\ -I                     /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32\GPIOv2\ -I                    /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32\I2Cv1\ -I                     /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32\SPIv1\ -I                     /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32\TIMv1\ -I                     /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32\RTCv2\ -I                     /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32\USARTv1\ -I                   /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32\OTGv1\ -I                     /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32F4xx\ -I                       /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\various\ -I F:\stuff\rusefi_sourceforge\firmware\ /
//                       iar\..\chibios\os\various\devices_lib\accel\ -I      /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       boards\ST_STM32F4_DISCOVERY\ -I                      /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\controll /
//                       ers\ -I F:\stuff\rusefi_sourceforge\firmware\iar\..\ /
//                       controllers\algo\ -I F:\stuff\rusefi_sourceforge\fir /
//                       mware\iar\..\controllers\core\ -I                    /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\controll /
//                       ers\system\ -I F:\stuff\rusefi_sourceforge\firmware\ /
//                       iar\..\controllers\sensors\ -I                       /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\controll /
//                       ers\math\ -I F:\stuff\rusefi_sourceforge\firmware\ia /
//                       r\..\controllers\trigger\ -I                         /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\console\ /
//                        -I F:\stuff\rusefi_sourceforge\firmware\iar\..\conf /
//                       ig\ -I F:\stuff\rusefi_sourceforge\firmware\iar\..\c /
//                       onfig\engines\ -I F:\stuff\rusefi_sourceforge\firmwa /
//                       re\iar\..\config\stm32f4ems\ -I                      /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\console\ /
//                       binary\ -I F:\stuff\rusefi_sourceforge\firmware\iar\ /
//                       ..\console_util\ -I F:\stuff\rusefi_sourceforge\firm /
//                       ware\iar\..\development\ -I                          /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\developm /
//                       ent\test\ -I F:\stuff\rusefi_sourceforge\firmware\ia /
//                       r\..\development\hw_layer\ -I                        /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\hw_layer /
//                       \algo\ -I F:\stuff\rusefi_sourceforge\firmware\iar\. /
//                       .\hw_layer\lcd\ -I F:\stuff\rusefi_sourceforge\firmw /
//                       are\iar\..\hw_layer\stm32f4\ -I                      /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\hw_layer /
//                       \serial_over_usb\ -I F:\stuff\rusefi_sourceforge\fir /
//                       mware\iar\..\hw_layer\ -I                            /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\ext\ -I  /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\ext_algo /
//                       \ -I F:\stuff\rusefi_sourceforge\firmware\iar\..\dev /
//                       elopment\ -I F:\stuff\rusefi_sourceforge\firmware\ia /
//                       r\..\development\hw_layer\ -I                        /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\util\    /
//                       -On                                                  /
//    MISRA C version =  2004                                                 /
//      Enabled       =  1.2-1.4 2.3 3.4 4.2 6.5 7 8.2 8.3 9.3 12.3 13.6      /
//                       13.7 15.5 17.3 19.17 20.7-20.9 20.11                 /
//      Checked       =  1.4 2.3 4.2 6.5 7 8.2 8.3 9.3 12.3 13.7 15.5         /
//                       20.7-20.9 20.11                                      /
//      Not checked   =  1.1-1.3 1.5-2.2 2.4-4.1 5-6.4 8.1 8.4-9.2 10-12.2    /
//                       12.4-13.6 14-15.4 16-20.6 20.10 20.12 21             /
//    List file       =  F:\stuff\rusefi_sourceforge\firmware\iar\Debug\List\ /
//                       usb.s                                                /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME usb

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN __aeabi_memset
        EXTERN chDbgCheckClassI
        EXTERN chDbgPanic
        EXTERN chDbgPanic3
        EXTERN dbg_check_lock
        EXTERN dbg_check_lock_from_isr
        EXTERN dbg_check_unlock
        EXTERN dbg_check_unlock_from_isr
        EXTERN usb_lld_clear_in
        EXTERN usb_lld_clear_out
        EXTERN usb_lld_disable_endpoints
        EXTERN usb_lld_get_status_in
        EXTERN usb_lld_get_status_out
        EXTERN usb_lld_init
        EXTERN usb_lld_init_endpoint
        EXTERN usb_lld_prepare_receive
        EXTERN usb_lld_prepare_transmit
        EXTERN usb_lld_read_setup
        EXTERN usb_lld_reset
        EXTERN usb_lld_set_address
        EXTERN usb_lld_stall_in
        EXTERN usb_lld_stall_out
        EXTERN usb_lld_start
        EXTERN usb_lld_start_in
        EXTERN usb_lld_start_out
        EXTERN usb_lld_stop

        PUBLIC _usb_ep0in
        PUBLIC _usb_ep0out
        PUBLIC _usb_ep0setup
        PUBLIC _usb_reset
        PUBLIC memset
        PUBLIC usbDisableEndpointsI
        PUBLIC usbInit
        PUBLIC usbInitEndpointI
        PUBLIC usbObjectInit
        PUBLIC usbPrepareQueuedReceive
        PUBLIC usbPrepareQueuedTransmit
        PUBLIC usbPrepareReceive
        PUBLIC usbPrepareTransmit
        PUBLIC usbStallReceiveI
        PUBLIC usbStallTransmitI
        PUBLIC usbStart
        PUBLIC usbStartReceiveI
        PUBLIC usbStartTransmitI
        PUBLIC usbStop
        
          CFI Names cfiNames0
          CFI StackFrame CFA R13 DATA
          CFI Resource R0:32, R1:32, R2:32, R3:32, R4:32, R5:32, R6:32, R7:32
          CFI Resource R8:32, R9:32, R10:32, R11:32, R12:32, R13:32, R14:32
          CFI Resource D0:64, D1:64, D2:64, D3:64, D4:64, D5:64, D6:64, D7:64
          CFI Resource D8:64, D9:64, D10:64, D11:64, D12:64, D13:64, D14:64
          CFI Resource D15:64
          CFI EndNames cfiNames0
        
          CFI Common cfiCommon0 Using cfiNames0
          CFI CodeAlign 2
          CFI DataAlign 4
          CFI ReturnAddress R14 CODE
          CFI CFA R13+0
          CFI R0 Undefined
          CFI R1 Undefined
          CFI R2 Undefined
          CFI R3 Undefined
          CFI R4 SameValue
          CFI R5 SameValue
          CFI R6 SameValue
          CFI R7 SameValue
          CFI R8 SameValue
          CFI R9 SameValue
          CFI R10 SameValue
          CFI R11 SameValue
          CFI R12 Undefined
          CFI R14 SameValue
          CFI D0 Undefined
          CFI D1 Undefined
          CFI D2 Undefined
          CFI D3 Undefined
          CFI D4 Undefined
          CFI D5 Undefined
          CFI D6 Undefined
          CFI D7 Undefined
          CFI D8 SameValue
          CFI D9 SameValue
          CFI D10 SameValue
          CFI D11 SameValue
          CFI D12 SameValue
          CFI D13 SameValue
          CFI D14 SameValue
          CFI D15 SameValue
          CFI EndCommon cfiCommon0
        
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\src\usb.c
//    1 /*
//    2     ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
//    3                  2011,2012,2013 Giovanni Di Sirio.
//    4 
//    5     This file is part of ChibiOS/RT.
//    6 
//    7     ChibiOS/RT is free software; you can redistribute it and/or modify
//    8     it under the terms of the GNU General Public License as published by
//    9     the Free Software Foundation; either version 3 of the License, or
//   10     (at your option) any later version.
//   11 
//   12     ChibiOS/RT is distributed in the hope that it will be useful,
//   13     but WITHOUT ANY WARRANTY; without even the implied warranty of
//   14     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   15     GNU General Public License for more details.
//   16 
//   17     You should have received a copy of the GNU General Public License
//   18     along with this program.  If not, see <http://www.gnu.org/licenses/>.
//   19 
//   20                                       ---
//   21 
//   22     A special exception to the GPL can be applied should you wish to distribute
//   23     a combined work that includes ChibiOS/RT, without being obliged to provide
//   24     the source code for any proprietary components. See the file exception.txt
//   25     for full details of how and when the exception can be applied.
//   26 */
//   27 
//   28 /**
//   29  * @file    usb.c
//   30  * @brief   USB Driver code.
//   31  *
//   32  * @addtogroup USB
//   33  * @{
//   34  */
//   35 
//   36 #include <string.h>

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP memset
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function memset
        THUMB
// __intrinsic __nounwind __interwork __softfp void *memset(void *, int, size_t)
memset:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
        MOVS     R2,R5
        MOVS     R1,R6
        MOVS     R0,R4
          CFI FunCall __aeabi_memset
        BL       __aeabi_memset
        MOVS     R0,R4
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"usbStart\\"()">`:
        DATA
        DC8 "\"usbStart\"()"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "F:\\\\stuff\\\\rusefi_sourc...">`:
        DATA
        DC8 46H, 3AH, 5CH, 73H, 74H, 75H, 66H, 66H
        DC8 5CH, 72H, 75H, 73H, 65H, 66H, 69H, 5FH
        DC8 73H, 6FH, 75H, 72H, 63H, 65H, 66H, 6FH
        DC8 72H, 67H, 65H, 5CH, 66H, 69H, 72H, 6DH
        DC8 77H, 61H, 72H, 65H, 5CH, 63H, 68H, 69H
        DC8 62H, 69H, 6FH, 73H, 5CH, 6FH, 73H, 5CH
        DC8 68H, 61H, 6CH, 5CH, 73H, 72H, 63H, 5CH
        DC8 75H, 73H, 62H, 2EH, 63H, 0
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "usbStart(), #1">`:
        DATA
        DC8 "usbStart(), #1"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"usbStop\\"()">`:
        DATA
        DC8 "\"usbStop\"()"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "usbStop(), #1">`:
        DATA
        DC8 "usbStop(), #1"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"usbInitEndpointI\\"()">`:
        DATA
        DC8 "\"usbInitEndpointI\"()"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "usbEnableEndpointI(), #1">`:
        DATA
        DC8 "usbEnableEndpointI(), #1"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "usbEnableEndpointI(), #2">`:
        DATA
        DC8 "usbEnableEndpointI(), #2"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"usbDisableEndpointsI\\"()">`:
        DATA
        DC8 "\"usbDisableEndpointsI\"()"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "usbDisableEndpointsI(...">`:
        DATA
        DC8 "usbDisableEndpointsI(), #1"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"usbStartReceiveI\\"()">`:
        DATA
        DC8 "\"usbStartReceiveI\"()"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"usbStartTransmitI\\"()">`:
        DATA
        DC8 "\"usbStartTransmitI\"()"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"usbStallReceiveI\\"()">`:
        DATA
        DC8 "\"usbStallReceiveI\"()"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"usbStallTransmitI\\"()">`:
        DATA
        DC8 "\"usbStallTransmitI\"()"
        DC8 0, 0
//   37 
//   38 #include "ch.h"
//   39 #include "hal.h"
//   40 #include "usb.h"
//   41 
//   42 #if HAL_USE_USB || defined(__DOXYGEN__)
//   43 
//   44 /*===========================================================================*/
//   45 /* Driver local definitions.                                                 */
//   46 /*===========================================================================*/
//   47 
//   48 /*===========================================================================*/
//   49 /* Driver exported variables.                                                */
//   50 /*===========================================================================*/
//   51 
//   52 /*===========================================================================*/
//   53 /* Driver local variables and types.                                         */
//   54 /*===========================================================================*/
//   55 

        SECTION `.rodata`:CONST:REORDER:NOROOT(1)
//   56 static const uint8_t zero_status[] = {0x00, 0x00};
zero_status:
        DATA
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(1)
//   57 static const uint8_t active_status[] ={0x00, 0x00};
active_status:
        DATA
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(1)
//   58 static const uint8_t halted_status[] = {0x01, 0x00};
halted_status:
        DATA
        DC8 1, 0
//   59 
//   60 /*===========================================================================*/
//   61 /* Driver local functions.                                                   */
//   62 /*===========================================================================*/
//   63 
//   64 /**
//   65  * @brief  SET ADDRESS transaction callback.
//   66  *
//   67  * @param[in] usbp      pointer to the @p USBDriver object
//   68  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function set_address
        THUMB
//   69 static void set_address(USBDriver *usbp) {
set_address:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   70 
//   71   usbp->address = usbp->setup[2];
        LDRB     R0,[R4, #+70]
        STRB     R0,[R4, #+78]
//   72   usb_lld_set_address(usbp);
        MOVS     R0,R4
          CFI FunCall usb_lld_set_address
        BL       usb_lld_set_address
//   73   _usb_isr_invoke_event_cb(usbp, USB_EVENT_ADDRESS);
        LDR      R0,[R4, #+4]
        LDR      R0,[R0, #+0]
        CMP      R0,#+0
        BEQ.N    ??set_address_0
        MOVS     R1,#+1
        MOVS     R0,R4
        LDR      R2,[R4, #+4]
        LDR      R2,[R2, #+0]
          CFI FunCall
        BLX      R2
//   74   usbp->state = USB_SELECTED;
??set_address_0:
        MOVS     R0,#+3
        STRB     R0,[R4, #+0]
//   75 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock1
//   76 
//   77 /**
//   78  * @brief   Standard requests handler.
//   79  * @details This is the standard requests default handler, most standard
//   80  *          requests are handled here, the user can override the standard
//   81  *          handling using the @p requests_hook_cb hook in the
//   82  *          @p USBConfig structure.
//   83  *
//   84  * @param[in] usbp      pointer to the @p USBDriver object
//   85  * @return              The request handling exit code.
//   86  * @retval FALSE        Request not recognized by the handler or error.
//   87  * @retval TRUE         Request handled.
//   88  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function default_handler
        THUMB
//   89 static bool_t default_handler(USBDriver *usbp) {
default_handler:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
//   90   const USBDescriptor *dp;
//   91 
//   92   /* Decoding the request.*/
//   93   switch (((usbp->setup[0] & (USB_RTYPE_RECIPIENT_MASK |
//   94                               USB_RTYPE_TYPE_MASK)) |
//   95            (usbp->setup[1] << 8))) {
        LDRB     R0,[R4, #+68]
        ANDS     R0,R0,#0x7F
        LDRB     R1,[R4, #+69]
        ORRS     R0,R0,R1, LSL #+8
        UXTH     R0,R0            ;; ZeroExt  R0,R0,#+16,#+16
        CMP      R0,#+0
        BEQ.N    ??default_handler_0
        CMP      R0,#+1
        BEQ.W    ??default_handler_1
        CMP      R0,#+2
        BEQ.W    ??default_handler_2
        CMP      R0,#+256
        BEQ.N    ??default_handler_3
        CMP      R0,#+258
        BEQ.W    ??default_handler_4
        CMP      R0,#+768
        BEQ.N    ??default_handler_5
        MOVW     R1,#+770
        CMP      R0,R1
        BEQ.W    ??default_handler_6
        CMP      R0,#+1280
        BEQ.N    ??default_handler_7
        CMP      R0,#+1536
        BEQ.N    ??default_handler_8
        CMP      R0,#+2048
        BEQ.N    ??default_handler_9
        CMP      R0,#+2304
        BEQ.N    ??default_handler_10
        MOVW     R1,#+3074
        CMP      R0,R1
        BEQ.W    ??default_handler_1
        B.N      ??default_handler_11
//   96   case USB_RTYPE_RECIPIENT_DEVICE | (USB_REQ_GET_STATUS << 8):
//   97     /* Just returns the current status word.*/
//   98     usbSetupTransfer(usbp, (uint8_t *)&usbp->status, 2, NULL);
??default_handler_0:
        ADDS     R0,R4,#+76
        STR      R0,[R4, #+56]
        MOVS     R0,#+2
        STR      R0,[R4, #+60]
        MOVS     R0,#+0
        STR      R0,[R4, #+64]
//   99     return TRUE;
        MOVS     R0,#+1
        B.N      ??default_handler_12
//  100   case USB_RTYPE_RECIPIENT_DEVICE | (USB_REQ_CLEAR_FEATURE << 8):
//  101     /* Only the DEVICE_REMOTE_WAKEUP is handled here, any other feature
//  102        number is handled as an error.*/
//  103     if (usbp->setup[2] == USB_FEATURE_DEVICE_REMOTE_WAKEUP) {
??default_handler_3:
        LDRB     R0,[R4, #+70]
        CMP      R0,#+1
        BNE.N    ??default_handler_13
//  104       usbp->status &= ~2;
        LDRH     R0,[R4, #+76]
        MOVW     R1,#+65533
        ANDS     R0,R1,R0
        STRH     R0,[R4, #+76]
//  105       usbSetupTransfer(usbp, NULL, 0, NULL);
        MOVS     R0,#+0
        STR      R0,[R4, #+56]
        MOVS     R0,#+0
        STR      R0,[R4, #+60]
        MOVS     R0,#+0
        STR      R0,[R4, #+64]
//  106       return TRUE;
        MOVS     R0,#+1
        B.N      ??default_handler_12
//  107     }
//  108     return FALSE;
??default_handler_13:
        MOVS     R0,#+0
        B.N      ??default_handler_12
//  109   case USB_RTYPE_RECIPIENT_DEVICE | (USB_REQ_SET_FEATURE << 8):
//  110     /* Only the DEVICE_REMOTE_WAKEUP is handled here, any other feature
//  111        number is handled as an error.*/
//  112     if (usbp->setup[2] == USB_FEATURE_DEVICE_REMOTE_WAKEUP) {
??default_handler_5:
        LDRB     R0,[R4, #+70]
        CMP      R0,#+1
        BNE.N    ??default_handler_14
//  113       usbp->status |= 2;
        LDRH     R0,[R4, #+76]
        ORRS     R0,R0,#0x2
        STRH     R0,[R4, #+76]
//  114       usbSetupTransfer(usbp, NULL, 0, NULL);
        MOVS     R0,#+0
        STR      R0,[R4, #+56]
        MOVS     R0,#+0
        STR      R0,[R4, #+60]
        MOVS     R0,#+0
        STR      R0,[R4, #+64]
//  115       return TRUE;
        MOVS     R0,#+1
        B.N      ??default_handler_12
//  116     }
//  117     return FALSE;
??default_handler_14:
        MOVS     R0,#+0
        B.N      ??default_handler_12
//  118   case USB_RTYPE_RECIPIENT_DEVICE | (USB_REQ_SET_ADDRESS << 8):
//  119     /* The SET_ADDRESS handling can be performed here or postponed after
//  120        the status packed depending on the USB_SET_ADDRESS_MODE low
//  121        driver setting.*/
//  122 #if USB_SET_ADDRESS_MODE == USB_EARLY_SET_ADDRESS
//  123     if ((usbp->setup[0] == USB_RTYPE_RECIPIENT_DEVICE) &&
//  124         (usbp->setup[1] == USB_REQ_SET_ADDRESS))
??default_handler_7:
        LDRB     R0,[R4, #+68]
        CMP      R0,#+0
        BNE.N    ??default_handler_15
        LDRB     R0,[R4, #+69]
        CMP      R0,#+5
        BNE.N    ??default_handler_15
//  125       set_address(usbp);
        MOVS     R0,R4
          CFI FunCall set_address
        BL       set_address
//  126     usbSetupTransfer(usbp, NULL, 0, NULL);
??default_handler_15:
        MOVS     R0,#+0
        STR      R0,[R4, #+56]
        MOVS     R0,#+0
        STR      R0,[R4, #+60]
        MOVS     R0,#+0
        STR      R0,[R4, #+64]
//  127 #else
//  128     usbSetupTransfer(usbp, NULL, 0, set_address);
//  129 #endif
//  130     return TRUE;
        MOVS     R0,#+1
        B.N      ??default_handler_12
//  131   case USB_RTYPE_RECIPIENT_DEVICE | (USB_REQ_GET_DESCRIPTOR << 8):
//  132     /* Handling descriptor requests from the host.*/
//  133     dp = usbp->config->get_descriptor_cb(
//  134            usbp, usbp->setup[3], usbp->setup[2],
//  135            usbFetchWord(&usbp->setup[4]));
??default_handler_8:
        LDRB     R0,[R4, #+72]
        LDRB     R1,[R4, #+73]
        ORRS     R3,R0,R1, LSL #+8
        UXTH     R3,R3            ;; ZeroExt  R3,R3,#+16,#+16
        LDRB     R2,[R4, #+70]
        LDRB     R1,[R4, #+71]
        MOVS     R0,R4
        LDR      R6,[R4, #+4]
        LDR      R6,[R6, #+4]
          CFI FunCall
        BLX      R6
        MOVS     R5,R0
//  136     if (dp == NULL)
        CMP      R5,#+0
        BNE.N    ??default_handler_16
//  137       return FALSE;
        MOVS     R0,#+0
        B.N      ??default_handler_12
//  138     usbSetupTransfer(usbp, (uint8_t *)dp->ud_string, dp->ud_size, NULL);
??default_handler_16:
        LDR      R0,[R5, #+4]
        STR      R0,[R4, #+56]
        LDR      R0,[R5, #+0]
        STR      R0,[R4, #+60]
        MOVS     R0,#+0
        STR      R0,[R4, #+64]
//  139     return TRUE;
        MOVS     R0,#+1
        B.N      ??default_handler_12
//  140   case USB_RTYPE_RECIPIENT_DEVICE | (USB_REQ_GET_CONFIGURATION << 8):
//  141     /* Returning the last selected configuration.*/
//  142     usbSetupTransfer(usbp, &usbp->configuration, 1, NULL);
??default_handler_9:
        ADDW     R0,R4,#+79
        STR      R0,[R4, #+56]
        MOVS     R0,#+1
        STR      R0,[R4, #+60]
        MOVS     R0,#+0
        STR      R0,[R4, #+64]
//  143     return TRUE;
        MOVS     R0,#+1
        B.N      ??default_handler_12
//  144   case USB_RTYPE_RECIPIENT_DEVICE | (USB_REQ_SET_CONFIGURATION << 8):
//  145     /* Handling configuration selection from the host.*/
//  146     usbp->configuration = usbp->setup[2];
??default_handler_10:
        LDRB     R0,[R4, #+70]
        STRB     R0,[R4, #+79]
//  147     if (usbp->configuration == 0)
        LDRB     R0,[R4, #+79]
        CMP      R0,#+0
        BNE.N    ??default_handler_17
//  148       usbp->state = USB_SELECTED;
        MOVS     R0,#+3
        STRB     R0,[R4, #+0]
        B.N      ??default_handler_18
//  149     else
//  150       usbp->state = USB_ACTIVE;
??default_handler_17:
        MOVS     R0,#+4
        STRB     R0,[R4, #+0]
//  151     _usb_isr_invoke_event_cb(usbp, USB_EVENT_CONFIGURED);
??default_handler_18:
        LDR      R0,[R4, #+4]
        LDR      R0,[R0, #+0]
        CMP      R0,#+0
        BEQ.N    ??default_handler_19
        MOVS     R1,#+2
        MOVS     R0,R4
        LDR      R2,[R4, #+4]
        LDR      R2,[R2, #+0]
          CFI FunCall
        BLX      R2
//  152     usbSetupTransfer(usbp, NULL, 0, NULL);
??default_handler_19:
        MOVS     R0,#+0
        STR      R0,[R4, #+56]
        MOVS     R0,#+0
        STR      R0,[R4, #+60]
        MOVS     R0,#+0
        STR      R0,[R4, #+64]
//  153     return TRUE;
        MOVS     R0,#+1
        B.N      ??default_handler_12
//  154   case USB_RTYPE_RECIPIENT_INTERFACE | (USB_REQ_GET_STATUS << 8):
//  155   case USB_RTYPE_RECIPIENT_ENDPOINT | (USB_REQ_SYNCH_FRAME << 8):
//  156     /* Just sending two zero bytes, the application can change the behavior
//  157        using a hook..*/
//  158     usbSetupTransfer(usbp, (uint8_t *)zero_status, 2, NULL);
??default_handler_1:
        LDR.W    R0,??DataTable8
        STR      R0,[R4, #+56]
        MOVS     R0,#+2
        STR      R0,[R4, #+60]
        MOVS     R0,#+0
        STR      R0,[R4, #+64]
//  159     return TRUE;
        MOVS     R0,#+1
        B.N      ??default_handler_12
//  160   case USB_RTYPE_RECIPIENT_ENDPOINT | (USB_REQ_GET_STATUS << 8):
//  161     /* Sending the EP status.*/
//  162     if (usbp->setup[4] & 0x80) {
??default_handler_2:
        LDRB     R0,[R4, #+72]
        LSLS     R0,R0,#+24
        BPL.N    ??default_handler_20
//  163       switch (usb_lld_get_status_in(usbp, usbp->setup[4] & 0x0F)) {
        LDRB     R0,[R4, #+72]
        ANDS     R1,R0,#0xF
        MOVS     R0,R4
          CFI FunCall usb_lld_get_status_in
        BL       usb_lld_get_status_in
        CMP      R0,#+1
        BEQ.N    ??default_handler_21
        CMP      R0,#+2
        BEQ.N    ??default_handler_22
        B.N      ??default_handler_23
//  164       case EP_STATUS_STALLED:
//  165         usbSetupTransfer(usbp, (uint8_t *)halted_status, 2, NULL);
??default_handler_21:
        LDR.W    R0,??DataTable8_1
        STR      R0,[R4, #+56]
        MOVS     R0,#+2
        STR      R0,[R4, #+60]
        MOVS     R0,#+0
        STR      R0,[R4, #+64]
//  166         return TRUE;
        MOVS     R0,#+1
        B.N      ??default_handler_12
//  167       case EP_STATUS_ACTIVE:
//  168         usbSetupTransfer(usbp, (uint8_t *)active_status, 2, NULL);
??default_handler_22:
        LDR.W    R0,??DataTable8_2
        STR      R0,[R4, #+56]
        MOVS     R0,#+2
        STR      R0,[R4, #+60]
        MOVS     R0,#+0
        STR      R0,[R4, #+64]
//  169         return TRUE;
        MOVS     R0,#+1
        B.N      ??default_handler_12
//  170       default:
//  171         return FALSE;
??default_handler_23:
        MOVS     R0,#+0
        B.N      ??default_handler_12
//  172       }
//  173     }
//  174     else {
//  175       switch (usb_lld_get_status_out(usbp, usbp->setup[4] & 0x0F)) {
??default_handler_20:
        LDRB     R0,[R4, #+72]
        ANDS     R1,R0,#0xF
        MOVS     R0,R4
          CFI FunCall usb_lld_get_status_out
        BL       usb_lld_get_status_out
        CMP      R0,#+1
        BEQ.N    ??default_handler_24
        CMP      R0,#+2
        BEQ.N    ??default_handler_25
        B.N      ??default_handler_26
//  176       case EP_STATUS_STALLED:
//  177         usbSetupTransfer(usbp, (uint8_t *)halted_status, 2, NULL);
??default_handler_24:
        LDR.W    R0,??DataTable8_1
        STR      R0,[R4, #+56]
        MOVS     R0,#+2
        STR      R0,[R4, #+60]
        MOVS     R0,#+0
        STR      R0,[R4, #+64]
//  178         return TRUE;
        MOVS     R0,#+1
        B.N      ??default_handler_12
//  179       case EP_STATUS_ACTIVE:
//  180         usbSetupTransfer(usbp, (uint8_t *)active_status, 2, NULL);
??default_handler_25:
        LDR.W    R0,??DataTable8_2
        STR      R0,[R4, #+56]
        MOVS     R0,#+2
        STR      R0,[R4, #+60]
        MOVS     R0,#+0
        STR      R0,[R4, #+64]
//  181         return TRUE;
        MOVS     R0,#+1
        B.N      ??default_handler_12
//  182       default:
//  183         return FALSE;
??default_handler_26:
        MOVS     R0,#+0
        B.N      ??default_handler_12
//  184       }
//  185     }
//  186   case USB_RTYPE_RECIPIENT_ENDPOINT | (USB_REQ_CLEAR_FEATURE << 8):
//  187     /* Only ENDPOINT_HALT is handled as feature.*/
//  188     if (usbp->setup[2] != USB_FEATURE_ENDPOINT_HALT)
??default_handler_4:
        LDRB     R0,[R4, #+70]
        CMP      R0,#+0
        BEQ.N    ??default_handler_27
//  189       return FALSE;
        MOVS     R0,#+0
        B.N      ??default_handler_12
//  190     /* Clearing the EP status, not valid for EP0, it is ignored in that case.*/
//  191     if ((usbp->setup[4] & 0x0F) > 0) {
??default_handler_27:
        LDRB     R0,[R4, #+72]
        ANDS     R0,R0,#0xF
        CMP      R0,#+1
        BLT.N    ??default_handler_28
//  192       if (usbp->setup[4] & 0x80)
        LDRB     R0,[R4, #+72]
        LSLS     R0,R0,#+24
        BPL.N    ??default_handler_29
//  193         usb_lld_clear_in(usbp, usbp->setup[4] & 0x0F);
        LDRB     R0,[R4, #+72]
        ANDS     R1,R0,#0xF
        MOVS     R0,R4
          CFI FunCall usb_lld_clear_in
        BL       usb_lld_clear_in
        B.N      ??default_handler_28
//  194       else
//  195         usb_lld_clear_out(usbp, usbp->setup[4] & 0x0F);
??default_handler_29:
        LDRB     R0,[R4, #+72]
        ANDS     R1,R0,#0xF
        MOVS     R0,R4
          CFI FunCall usb_lld_clear_out
        BL       usb_lld_clear_out
//  196     }
//  197     usbSetupTransfer(usbp, NULL, 0, NULL);
??default_handler_28:
        MOVS     R0,#+0
        STR      R0,[R4, #+56]
        MOVS     R0,#+0
        STR      R0,[R4, #+60]
        MOVS     R0,#+0
        STR      R0,[R4, #+64]
//  198     return TRUE;
        MOVS     R0,#+1
        B.N      ??default_handler_12
//  199   case USB_RTYPE_RECIPIENT_ENDPOINT | (USB_REQ_SET_FEATURE << 8):
//  200     /* Only ENDPOINT_HALT is handled as feature.*/
//  201     if (usbp->setup[2] != USB_FEATURE_ENDPOINT_HALT)
??default_handler_6:
        LDRB     R0,[R4, #+70]
        CMP      R0,#+0
        BEQ.N    ??default_handler_30
//  202       return FALSE;
        MOVS     R0,#+0
        B.N      ??default_handler_12
//  203     /* Stalling the EP, not valid for EP0, it is ignored in that case.*/
//  204     if ((usbp->setup[4] & 0x0F) > 0) {
??default_handler_30:
        LDRB     R0,[R4, #+72]
        ANDS     R0,R0,#0xF
        CMP      R0,#+1
        BLT.N    ??default_handler_31
//  205       if (usbp->setup[4] & 0x80)
        LDRB     R0,[R4, #+72]
        LSLS     R0,R0,#+24
        BPL.N    ??default_handler_32
//  206         usb_lld_stall_in(usbp, usbp->setup[4] & 0x0F);
        LDRB     R0,[R4, #+72]
        ANDS     R1,R0,#0xF
        MOVS     R0,R4
          CFI FunCall usb_lld_stall_in
        BL       usb_lld_stall_in
        B.N      ??default_handler_31
//  207       else
//  208         usb_lld_stall_out(usbp, usbp->setup[4] & 0x0F);
??default_handler_32:
        LDRB     R0,[R4, #+72]
        ANDS     R1,R0,#0xF
        MOVS     R0,R4
          CFI FunCall usb_lld_stall_out
        BL       usb_lld_stall_out
//  209     }
//  210     usbSetupTransfer(usbp, NULL, 0, NULL);
??default_handler_31:
        MOVS     R0,#+0
        STR      R0,[R4, #+56]
        MOVS     R0,#+0
        STR      R0,[R4, #+60]
        MOVS     R0,#+0
        STR      R0,[R4, #+64]
//  211     return TRUE;
        MOVS     R0,#+1
        B.N      ??default_handler_12
//  212   case USB_RTYPE_RECIPIENT_DEVICE | (USB_REQ_SET_DESCRIPTOR << 8):
//  213   case USB_RTYPE_RECIPIENT_INTERFACE | (USB_REQ_CLEAR_FEATURE << 8):
//  214   case USB_RTYPE_RECIPIENT_INTERFACE | (USB_REQ_SET_FEATURE << 8):
//  215   case USB_RTYPE_RECIPIENT_INTERFACE | (USB_REQ_GET_INTERFACE << 8):
//  216   case USB_RTYPE_RECIPIENT_INTERFACE | (USB_REQ_SET_INTERFACE << 8):
//  217     /* All the above requests are not handled here, if you need them then
//  218        use the hook mechanism and provide handling.*/
//  219   default:
//  220     return FALSE;
??default_handler_11:
        MOVS     R0,#+0
??default_handler_12:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock2
//  221   }
//  222 }
//  223 
//  224 /*===========================================================================*/
//  225 /* Driver exported functions.                                                */
//  226 /*===========================================================================*/
//  227 
//  228 /**
//  229  * @brief   USB Driver initialization.
//  230  * @note    This function is implicitly invoked by @p halInit(), there is
//  231  *          no need to explicitly initialize the driver.
//  232  *
//  233  * @init
//  234  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function usbInit
        THUMB
//  235 void usbInit(void) {
usbInit:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  236 
//  237   usb_lld_init();
          CFI FunCall usb_lld_init
        BL       usb_lld_init
//  238 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock3
//  239 
//  240 /**
//  241  * @brief   Initializes the standard part of a @p USBDriver structure.
//  242  *
//  243  * @param[out] usbp     pointer to the @p USBDriver object
//  244  *
//  245  * @init
//  246  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function usbObjectInit
          CFI NoCalls
        THUMB
//  247 void usbObjectInit(USBDriver *usbp) {
//  248   unsigned i;
//  249 
//  250   usbp->state        = USB_STOP;
usbObjectInit:
        MOVS     R2,#+1
        STRB     R2,[R0, #+0]
//  251   usbp->config       = NULL;
        MOVS     R2,#+0
        STR      R2,[R0, #+4]
//  252   for (i = 0; i < USB_MAX_ENDPOINTS; i++) {
        MOVS     R2,#+0
        MOVS     R1,R2
??usbObjectInit_0:
        CMP      R1,#+3
        BCS.N    ??usbObjectInit_1
//  253     usbp->in_params[i]  = NULL;
        ADDS     R2,R0,R1, LSL #+2
        MOVS     R3,#+0
        STR      R3,[R2, #+28]
//  254     usbp->out_params[i] = NULL;
        ADDS     R2,R0,R1, LSL #+2
        MOVS     R3,#+0
        STR      R3,[R2, #+40]
//  255   }
        ADDS     R1,R1,#+1
        B.N      ??usbObjectInit_0
//  256   usbp->transmitting = 0;
??usbObjectInit_1:
        MOVS     R2,#+0
        STRH     R2,[R0, #+8]
//  257   usbp->receiving    = 0;
        MOVS     R2,#+0
        STRH     R2,[R0, #+10]
//  258 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock4
//  259 
//  260 /**
//  261  * @brief   Configures and activates the USB peripheral.
//  262  *
//  263  * @param[in] usbp      pointer to the @p USBDriver object
//  264  * @param[in] config    pointer to the @p USBConfig object
//  265  *
//  266  * @api
//  267  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function usbStart
        THUMB
//  268 void usbStart(USBDriver *usbp, const USBConfig *config) {
usbStart:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  269   unsigned i;
//  270 
//  271   chDbgCheck((usbp != NULL) && (config != NULL), "usbStart");
        CMP      R4,#+0
        BEQ.N    ??usbStart_0
        CMP      R5,#+0
        BNE.N    ??usbStart_1
??usbStart_0:
        MOVW     R2,#+271
        LDR.N    R1,??DataTable8_3
        LDR.N    R0,??DataTable8_4
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  272 
//  273   chSysLock();
??usbStart_1:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  274   chDbgAssert((usbp->state == USB_STOP) || (usbp->state == USB_READY),
//  275               "usbStart(), #1", "invalid state");
        LDRB     R0,[R4, #+0]
        CMP      R0,#+1
        BEQ.N    ??usbStart_2
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BEQ.N    ??usbStart_2
        LDR.N    R0,??DataTable8_5
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  276   usbp->config = config;
??usbStart_2:
        STR      R5,[R4, #+4]
//  277   for (i = 0; i <= USB_MAX_ENDPOINTS; i++)
        MOVS     R0,#+0
        MOVS     R6,R0
??usbStart_3:
        CMP      R6,#+4
        BCS.N    ??usbStart_4
//  278     usbp->epc[i] = NULL;
        ADDS     R0,R4,R6, LSL #+2
        MOVS     R1,#+0
        STR      R1,[R0, #+12]
        ADDS     R6,R6,#+1
        B.N      ??usbStart_3
//  279   usb_lld_start(usbp);
??usbStart_4:
        MOVS     R0,R4
          CFI FunCall usb_lld_start
        BL       usb_lld_start
//  280   usbp->state = USB_READY;
        MOVS     R0,#+2
        STRB     R0,[R4, #+0]
//  281   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  282 }
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock5
//  283 
//  284 /**
//  285  * @brief   Deactivates the USB peripheral.
//  286  *
//  287  * @param[in] usbp      pointer to the @p USBDriver object
//  288  *
//  289  * @api
//  290  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function usbStop
        THUMB
//  291 void usbStop(USBDriver *usbp) {
usbStop:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  292 
//  293   chDbgCheck(usbp != NULL, "usbStop");
        CMP      R4,#+0
        BNE.N    ??usbStop_0
        MOVW     R2,#+293
        LDR.N    R1,??DataTable8_3
        LDR.N    R0,??DataTable8_6
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  294 
//  295   chSysLock();
??usbStop_0:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  296   chDbgAssert((usbp->state == USB_STOP) || (usbp->state == USB_READY) ||
//  297               (usbp->state == USB_SELECTED) || (usbp->state == USB_ACTIVE),
//  298               "usbStop(), #1", "invalid state");
        LDRB     R0,[R4, #+0]
        CMP      R0,#+1
        BEQ.N    ??usbStop_1
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BEQ.N    ??usbStop_1
        LDRB     R0,[R4, #+0]
        CMP      R0,#+3
        BEQ.N    ??usbStop_1
        LDRB     R0,[R4, #+0]
        CMP      R0,#+4
        BEQ.N    ??usbStop_1
        LDR.N    R0,??DataTable8_7
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  299   usb_lld_stop(usbp);
??usbStop_1:
        MOVS     R0,R4
          CFI FunCall usb_lld_stop
        BL       usb_lld_stop
//  300   usbp->state = USB_STOP;
        MOVS     R0,#+1
        STRB     R0,[R4, #+0]
//  301   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  302 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock6
//  303 
//  304 /**
//  305  * @brief   Enables an endpoint.
//  306  * @details This function enables an endpoint, both IN and/or OUT directions
//  307  *          depending on the configuration structure.
//  308  * @note    This function must be invoked in response of a SET_CONFIGURATION
//  309  *          or SET_INTERFACE message.
//  310  *
//  311  * @param[in] usbp      pointer to the @p USBDriver object
//  312  * @param[in] ep        endpoint number
//  313  * @param[in] epcp      the endpoint configuration
//  314  *
//  315  * @iclass
//  316  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function usbInitEndpointI
        THUMB
//  317 void usbInitEndpointI(USBDriver *usbp, usbep_t ep,
//  318                       const USBEndpointConfig *epcp) {
usbInitEndpointI:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//  319 
//  320   chDbgCheckClassI();
          CFI FunCall chDbgCheckClassI
        BL       chDbgCheckClassI
//  321   chDbgCheck((usbp != NULL) && (epcp != NULL), "usbInitEndpointI");
        CMP      R4,#+0
        BEQ.N    ??usbInitEndpointI_0
        CMP      R6,#+0
        BNE.N    ??usbInitEndpointI_1
??usbInitEndpointI_0:
        MOVW     R2,#+321
        LDR.N    R1,??DataTable8_3
        LDR.N    R0,??DataTable8_8
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  322   chDbgAssert(usbp->state == USB_ACTIVE,
//  323               "usbEnableEndpointI(), #1", "invalid state");
??usbInitEndpointI_1:
        LDRB     R0,[R4, #+0]
        CMP      R0,#+4
        BEQ.N    ??usbInitEndpointI_2
        LDR.N    R0,??DataTable8_9
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  324   chDbgAssert(usbp->epc[ep] == NULL,
//  325               "usbEnableEndpointI(), #2", "already initialized");
??usbInitEndpointI_2:
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R0,R4,R5, LSL #+2
        LDR      R0,[R0, #+12]
        CMP      R0,#+0
        BEQ.N    ??usbInitEndpointI_3
        LDR.N    R0,??DataTable8_10
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  326 
//  327   /* Logically enabling the endpoint in the USBDriver structure.*/
//  328   if (epcp->in_state != NULL)
??usbInitEndpointI_3:
        LDR      R0,[R6, #+20]
        CMP      R0,#+0
        BEQ.N    ??usbInitEndpointI_4
//  329     memset(epcp->in_state, 0, sizeof(USBInEndpointState));
        MOVS     R2,#+20
        MOVS     R1,#+0
        LDR      R0,[R6, #+20]
          CFI FunCall memset
        BL       memset
//  330   if (epcp->out_state != NULL)
??usbInitEndpointI_4:
        LDR      R0,[R6, #+24]
        CMP      R0,#+0
        BEQ.N    ??usbInitEndpointI_5
//  331     memset(epcp->out_state, 0, sizeof(USBOutEndpointState));
        MOVS     R2,#+20
        MOVS     R1,#+0
        LDR      R0,[R6, #+24]
          CFI FunCall memset
        BL       memset
//  332 
//  333   usbp->epc[ep] = epcp;
??usbInitEndpointI_5:
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R0,R4,R5, LSL #+2
        STR      R6,[R0, #+12]
//  334 
//  335   /* Low level endpoint activation.*/
//  336   usb_lld_init_endpoint(usbp, ep);
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
          CFI FunCall usb_lld_init_endpoint
        BL       usb_lld_init_endpoint
//  337 }
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock7
//  338 
//  339 /**
//  340  * @brief   Disables all the active endpoints.
//  341  * @details This function disables all the active endpoints except the
//  342  *          endpoint zero.
//  343  * @note    This function must be invoked in response of a SET_CONFIGURATION
//  344  *          message with configuration number zero.
//  345  *
//  346  * @param[in] usbp      pointer to the @p USBDriver object
//  347  *
//  348  * @iclass
//  349  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function usbDisableEndpointsI
        THUMB
//  350 void usbDisableEndpointsI(USBDriver *usbp) {
usbDisableEndpointsI:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  351   unsigned i;
//  352 
//  353   chDbgCheckClassI();
          CFI FunCall chDbgCheckClassI
        BL       chDbgCheckClassI
//  354   chDbgCheck(usbp != NULL, "usbDisableEndpointsI");
        CMP      R4,#+0
        BNE.N    ??usbDisableEndpointsI_0
        MOV      R2,#+354
        LDR.N    R1,??DataTable8_3
        LDR.N    R0,??DataTable8_11
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  355   chDbgAssert(usbp->state == USB_SELECTED,
//  356               "usbDisableEndpointsI(), #1", "invalid state");
??usbDisableEndpointsI_0:
        LDRB     R0,[R4, #+0]
        CMP      R0,#+3
        BEQ.N    ??usbDisableEndpointsI_1
        LDR.N    R0,??DataTable8_12
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  357 
//  358   usbp->transmitting &= ~1;
??usbDisableEndpointsI_1:
        LDRH     R0,[R4, #+8]
        MOVW     R1,#+65534
        ANDS     R0,R1,R0
        STRH     R0,[R4, #+8]
//  359   usbp->receiving    &= ~1;
        LDRH     R0,[R4, #+10]
        MOVW     R1,#+65534
        ANDS     R0,R1,R0
        STRH     R0,[R4, #+10]
//  360   for (i = 1; i <= USB_MAX_ENDPOINTS; i++)
        MOVS     R0,#+1
        MOVS     R5,R0
??usbDisableEndpointsI_2:
        CMP      R5,#+4
        BCS.N    ??usbDisableEndpointsI_3
//  361     usbp->epc[i] = NULL;
        ADDS     R0,R4,R5, LSL #+2
        MOVS     R1,#+0
        STR      R1,[R0, #+12]
        ADDS     R5,R5,#+1
        B.N      ??usbDisableEndpointsI_2
//  362 
//  363   /* Low level endpoints deactivation.*/
//  364   usb_lld_disable_endpoints(usbp);
??usbDisableEndpointsI_3:
        MOVS     R0,R4
          CFI FunCall usb_lld_disable_endpoints
        BL       usb_lld_disable_endpoints
//  365 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock8
//  366 
//  367 /**
//  368  * @brief   Prepares for a receive transaction on an OUT endpoint.
//  369  * @post    The endpoint is ready for @p usbStartReceiveI().
//  370  * @note    This function can be called both in ISR and thread context.
//  371  *
//  372  * @param[in] usbp      pointer to the @p USBDriver object
//  373  * @param[in] ep        endpoint number
//  374  * @param[out] buf      buffer where to copy the received data
//  375  * @param[in] n         transaction size
//  376  *
//  377  * @special
//  378  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function usbPrepareReceive
        THUMB
//  379 void usbPrepareReceive(USBDriver *usbp, usbep_t ep, uint8_t *buf, size_t n) {
usbPrepareReceive:
        PUSH     {R4-R8,LR}
          CFI R14 Frame(CFA, -4)
          CFI R8 Frame(CFA, -8)
          CFI R7 Frame(CFA, -12)
          CFI R6 Frame(CFA, -16)
          CFI R5 Frame(CFA, -20)
          CFI R4 Frame(CFA, -24)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
        MOVS     R7,R3
//  380   USBOutEndpointState *osp = usbp->epc[ep]->out_state;
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R0,R4,R5, LSL #+2
        LDR      R0,[R0, #+12]
        LDR      R8,[R0, #+24]
//  381 
//  382   osp->rxqueued           = FALSE;
        MOVS     R0,#+0
        STRB     R0,[R8, #+0]
//  383   osp->mode.linear.rxbuf  = buf;
        STR      R6,[R8, #+12]
//  384   osp->rxsize             = n;
        STR      R7,[R8, #+4]
//  385   osp->rxcnt              = 0;
        MOVS     R0,#+0
        STR      R0,[R8, #+8]
//  386 
//  387   usb_lld_prepare_receive(usbp, ep);
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
          CFI FunCall usb_lld_prepare_receive
        BL       usb_lld_prepare_receive
//  388 }
        POP      {R4-R8,PC}       ;; return
          CFI EndBlock cfiBlock9
//  389 
//  390 /**
//  391  * @brief   Prepares for a transmit transaction on an IN endpoint.
//  392  * @post    The endpoint is ready for @p usbStartTransmitI().
//  393  * @note    This function can be called both in ISR and thread context.
//  394  * @note    The queue must contain at least the amount of data specified
//  395  *          as transaction size.
//  396  *
//  397  * @param[in] usbp      pointer to the @p USBDriver object
//  398  * @param[in] ep        endpoint number
//  399  * @param[in] buf       buffer where to fetch the data to be transmitted
//  400  * @param[in] n         transaction size
//  401  *
//  402  * @special
//  403  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function usbPrepareTransmit
        THUMB
//  404 void usbPrepareTransmit(USBDriver *usbp, usbep_t ep,
//  405                         const uint8_t *buf, size_t n) {
usbPrepareTransmit:
        PUSH     {R4-R8,LR}
          CFI R14 Frame(CFA, -4)
          CFI R8 Frame(CFA, -8)
          CFI R7 Frame(CFA, -12)
          CFI R6 Frame(CFA, -16)
          CFI R5 Frame(CFA, -20)
          CFI R4 Frame(CFA, -24)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
        MOVS     R7,R3
//  406   USBInEndpointState *isp = usbp->epc[ep]->in_state;
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R0,R4,R5, LSL #+2
        LDR      R0,[R0, #+12]
        LDR      R8,[R0, #+20]
//  407 
//  408   isp->txqueued           = FALSE;
        MOVS     R0,#+0
        STRB     R0,[R8, #+0]
//  409   isp->mode.linear.txbuf  = buf;
        STR      R6,[R8, #+12]
//  410   isp->txsize             = n;
        STR      R7,[R8, #+4]
//  411   isp->txcnt              = 0;
        MOVS     R0,#+0
        STR      R0,[R8, #+8]
//  412 
//  413   usb_lld_prepare_transmit(usbp, ep);
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
          CFI FunCall usb_lld_prepare_transmit
        BL       usb_lld_prepare_transmit
//  414 }
        POP      {R4-R8,PC}       ;; return
          CFI EndBlock cfiBlock10
//  415 
//  416 /**
//  417  * @brief   Prepares for a receive transaction on an OUT endpoint.
//  418  * @post    The endpoint is ready for @p usbStartReceiveI().
//  419  * @note    This function can be called both in ISR and thread context.
//  420  * @note    The queue must have enough free space to accommodate the
//  421  *          specified transaction size rounded to the next packet size
//  422  *          boundary. For example if the transaction size is 1 and the
//  423  *          packet size is 64 then the queue must have space for at least
//  424  *          64 bytes.
//  425  *
//  426  * @param[in] usbp      pointer to the @p USBDriver object
//  427  * @param[in] ep        endpoint number
//  428  * @param[in] iqp       input queue to be filled with incoming data
//  429  * @param[in] n         transaction size
//  430  *
//  431  * @special
//  432  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function usbPrepareQueuedReceive
        THUMB
//  433 void usbPrepareQueuedReceive(USBDriver *usbp, usbep_t ep,
//  434                              InputQueue *iqp, size_t n) {
usbPrepareQueuedReceive:
        PUSH     {R4-R8,LR}
          CFI R14 Frame(CFA, -4)
          CFI R8 Frame(CFA, -8)
          CFI R7 Frame(CFA, -12)
          CFI R6 Frame(CFA, -16)
          CFI R5 Frame(CFA, -20)
          CFI R4 Frame(CFA, -24)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
        MOVS     R7,R3
//  435   USBOutEndpointState *osp = usbp->epc[ep]->out_state;
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R0,R4,R5, LSL #+2
        LDR      R0,[R0, #+12]
        LDR      R8,[R0, #+24]
//  436 
//  437   osp->rxqueued           = TRUE;
        MOVS     R0,#+1
        STRB     R0,[R8, #+0]
//  438   osp->mode.queue.rxqueue = iqp;
        STR      R6,[R8, #+12]
//  439   osp->rxsize             = n;
        STR      R7,[R8, #+4]
//  440   osp->rxcnt              = 0;
        MOVS     R0,#+0
        STR      R0,[R8, #+8]
//  441 
//  442   usb_lld_prepare_receive(usbp, ep);
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
          CFI FunCall usb_lld_prepare_receive
        BL       usb_lld_prepare_receive
//  443 }
        POP      {R4-R8,PC}       ;; return
          CFI EndBlock cfiBlock11
//  444 
//  445 /**
//  446  * @brief   Prepares for a transmit transaction on an IN endpoint.
//  447  * @post    The endpoint is ready for @p usbStartTransmitI().
//  448  * @note    This function can be called both in ISR and thread context.
//  449  * @note    The transmit transaction size is equal to the data contained
//  450  *          in the queue.
//  451  *
//  452  * @param[in] usbp      pointer to the @p USBDriver object
//  453  * @param[in] ep        endpoint number
//  454  * @param[in] oqp       output queue to be fetched for outgoing data
//  455  * @param[in] n         transaction size
//  456  *
//  457  * @special
//  458  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function usbPrepareQueuedTransmit
        THUMB
//  459 void usbPrepareQueuedTransmit(USBDriver *usbp, usbep_t ep,
//  460                               OutputQueue *oqp, size_t n) {
usbPrepareQueuedTransmit:
        PUSH     {R4-R8,LR}
          CFI R14 Frame(CFA, -4)
          CFI R8 Frame(CFA, -8)
          CFI R7 Frame(CFA, -12)
          CFI R6 Frame(CFA, -16)
          CFI R5 Frame(CFA, -20)
          CFI R4 Frame(CFA, -24)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
        MOVS     R7,R3
//  461   USBInEndpointState *isp = usbp->epc[ep]->in_state;
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R0,R4,R5, LSL #+2
        LDR      R0,[R0, #+12]
        LDR      R8,[R0, #+20]
//  462 
//  463   isp->txqueued           = TRUE;
        MOVS     R0,#+1
        STRB     R0,[R8, #+0]
//  464   isp->mode.queue.txqueue = oqp;
        STR      R6,[R8, #+12]
//  465   isp->txsize             = n;
        STR      R7,[R8, #+4]
//  466   isp->txcnt              = 0;
        MOVS     R0,#+0
        STR      R0,[R8, #+8]
//  467 
//  468   usb_lld_prepare_transmit(usbp, ep);
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
          CFI FunCall usb_lld_prepare_transmit
        BL       usb_lld_prepare_transmit
//  469 }
        POP      {R4-R8,PC}       ;; return
          CFI EndBlock cfiBlock12
//  470 
//  471 /**
//  472  * @brief   Starts a receive transaction on an OUT endpoint.
//  473  * @post    The endpoint callback is invoked when the transfer has been
//  474  *          completed.
//  475  *
//  476  * @param[in] usbp      pointer to the @p USBDriver object
//  477  * @param[in] ep        endpoint number
//  478  *
//  479  * @return              The operation status.
//  480  * @retval FALSE        Operation started successfully.
//  481  * @retval TRUE         Endpoint busy, operation not started.
//  482  *
//  483  * @iclass
//  484  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock13 Using cfiCommon0
          CFI Function usbStartReceiveI
        THUMB
//  485 bool_t usbStartReceiveI(USBDriver *usbp, usbep_t ep) {
usbStartReceiveI:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  486 
//  487   chDbgCheckClassI();
          CFI FunCall chDbgCheckClassI
        BL       chDbgCheckClassI
//  488   chDbgCheck(usbp != NULL, "usbStartReceiveI");
        CMP      R4,#+0
        BNE.N    ??usbStartReceiveI_0
        MOV      R2,#+488
        LDR.N    R1,??DataTable8_3
        LDR.N    R0,??DataTable8_13
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  489 
//  490   if (usbGetReceiveStatusI(usbp, ep))
??usbStartReceiveI_0:
        LDRH     R0,[R4, #+10]
        MOVS     R1,#+1
        LSLS     R1,R1,R5
        TST      R0,R1
        BEQ.N    ??usbStartReceiveI_1
//  491     return TRUE;
        MOVS     R0,#+1
        B.N      ??usbStartReceiveI_2
//  492 
//  493   usbp->receiving |= (1 << ep);
??usbStartReceiveI_1:
        LDRH     R0,[R4, #+10]
        MOVS     R1,#+1
        LSLS     R1,R1,R5
        ORRS     R0,R1,R0
        STRH     R0,[R4, #+10]
//  494   usb_lld_start_out(usbp, ep);
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
          CFI FunCall usb_lld_start_out
        BL       usb_lld_start_out
//  495   return FALSE;
        MOVS     R0,#+0
??usbStartReceiveI_2:
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock13
//  496 }
//  497 
//  498 /**
//  499  * @brief   Starts a transmit transaction on an IN endpoint.
//  500  * @post    The endpoint callback is invoked when the transfer has been
//  501  *          completed.
//  502  *
//  503  * @param[in] usbp      pointer to the @p USBDriver object
//  504  * @param[in] ep        endpoint number
//  505  *
//  506  * @return              The operation status.
//  507  * @retval FALSE        Operation started successfully.
//  508  * @retval TRUE         Endpoint busy, operation not started.
//  509  *
//  510  * @iclass
//  511  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock14 Using cfiCommon0
          CFI Function usbStartTransmitI
        THUMB
//  512 bool_t usbStartTransmitI(USBDriver *usbp, usbep_t ep) {
usbStartTransmitI:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  513 
//  514   chDbgCheckClassI();
          CFI FunCall chDbgCheckClassI
        BL       chDbgCheckClassI
//  515   chDbgCheck(usbp != NULL, "usbStartTransmitI");
        CMP      R4,#+0
        BNE.N    ??usbStartTransmitI_0
        MOVW     R2,#+515
        LDR.N    R1,??DataTable8_3
        LDR.N    R0,??DataTable8_14
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  516 
//  517   if (usbGetTransmitStatusI(usbp, ep))
??usbStartTransmitI_0:
        LDRH     R0,[R4, #+8]
        MOVS     R1,#+1
        LSLS     R1,R1,R5
        TST      R0,R1
        BEQ.N    ??usbStartTransmitI_1
//  518     return TRUE;
        MOVS     R0,#+1
        B.N      ??usbStartTransmitI_2
//  519 
//  520   usbp->transmitting |= (1 << ep);
??usbStartTransmitI_1:
        LDRH     R0,[R4, #+8]
        MOVS     R1,#+1
        LSLS     R1,R1,R5
        ORRS     R0,R1,R0
        STRH     R0,[R4, #+8]
//  521   usb_lld_start_in(usbp, ep);
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
          CFI FunCall usb_lld_start_in
        BL       usb_lld_start_in
//  522   return FALSE;
        MOVS     R0,#+0
??usbStartTransmitI_2:
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock14
//  523 }
//  524 
//  525 /**
//  526  * @brief   Stalls an OUT endpoint.
//  527  *
//  528  * @param[in] usbp      pointer to the @p USBDriver object
//  529  * @param[in] ep        endpoint number
//  530  *
//  531  * @return              The operation status.
//  532  * @retval FALSE        Endpoint stalled.
//  533  * @retval TRUE         Endpoint busy, not stalled.
//  534  *
//  535  * @iclass
//  536  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock15 Using cfiCommon0
          CFI Function usbStallReceiveI
        THUMB
//  537 bool_t usbStallReceiveI(USBDriver *usbp, usbep_t ep) {
usbStallReceiveI:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  538 
//  539   chDbgCheckClassI();
          CFI FunCall chDbgCheckClassI
        BL       chDbgCheckClassI
//  540   chDbgCheck(usbp != NULL, "usbStallReceiveI");
        CMP      R4,#+0
        BNE.N    ??usbStallReceiveI_0
        MOV      R2,#+540
        LDR.N    R1,??DataTable8_3
        LDR.N    R0,??DataTable8_15
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  541 
//  542   if (usbGetReceiveStatusI(usbp, ep))
??usbStallReceiveI_0:
        LDRH     R0,[R4, #+10]
        MOVS     R1,#+1
        LSLS     R1,R1,R5
        TST      R0,R1
        BEQ.N    ??usbStallReceiveI_1
//  543     return TRUE;
        MOVS     R0,#+1
        B.N      ??usbStallReceiveI_2
//  544 
//  545   usb_lld_stall_out(usbp, ep);
??usbStallReceiveI_1:
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
          CFI FunCall usb_lld_stall_out
        BL       usb_lld_stall_out
//  546   return FALSE;
        MOVS     R0,#+0
??usbStallReceiveI_2:
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock15
//  547 }
//  548 
//  549 /**
//  550  * @brief   Stalls an IN endpoint.
//  551  *
//  552  * @param[in] usbp      pointer to the @p USBDriver object
//  553  * @param[in] ep        endpoint number
//  554  *
//  555  * @return              The operation status.
//  556  * @retval FALSE        Endpoint stalled.
//  557  * @retval TRUE         Endpoint busy, not stalled.
//  558  *
//  559  * @iclass
//  560  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock16 Using cfiCommon0
          CFI Function usbStallTransmitI
        THUMB
//  561 bool_t usbStallTransmitI(USBDriver *usbp, usbep_t ep) {
usbStallTransmitI:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  562 
//  563   chDbgCheckClassI();
          CFI FunCall chDbgCheckClassI
        BL       chDbgCheckClassI
//  564   chDbgCheck(usbp != NULL, "usbStallTransmitI");
        CMP      R4,#+0
        BNE.N    ??usbStallTransmitI_0
        MOV      R2,#+564
        LDR.N    R1,??DataTable8_3
        LDR.N    R0,??DataTable8_16
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  565 
//  566   if (usbGetTransmitStatusI(usbp, ep))
??usbStallTransmitI_0:
        LDRH     R0,[R4, #+8]
        MOVS     R1,#+1
        LSLS     R1,R1,R5
        TST      R0,R1
        BEQ.N    ??usbStallTransmitI_1
//  567     return TRUE;
        MOVS     R0,#+1
        B.N      ??usbStallTransmitI_2
//  568 
//  569   usb_lld_stall_in(usbp, ep);
??usbStallTransmitI_1:
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
          CFI FunCall usb_lld_stall_in
        BL       usb_lld_stall_in
//  570   return FALSE;
        MOVS     R0,#+0
??usbStallTransmitI_2:
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock16
//  571 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8:
        DC32     zero_status

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_1:
        DC32     halted_status

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_2:
        DC32     active_status

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_3:
        DC32     `?<Constant "F:\\\\stuff\\\\rusefi_sourc...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_4:
        DC32     `?<Constant "\\"usbStart\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_5:
        DC32     `?<Constant "usbStart(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_6:
        DC32     `?<Constant "\\"usbStop\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_7:
        DC32     `?<Constant "usbStop(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_8:
        DC32     `?<Constant "\\"usbInitEndpointI\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_9:
        DC32     `?<Constant "usbEnableEndpointI(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_10:
        DC32     `?<Constant "usbEnableEndpointI(), #2">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_11:
        DC32     `?<Constant "\\"usbDisableEndpointsI\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_12:
        DC32     `?<Constant "usbDisableEndpointsI(...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_13:
        DC32     `?<Constant "\\"usbStartReceiveI\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_14:
        DC32     `?<Constant "\\"usbStartTransmitI\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_15:
        DC32     `?<Constant "\\"usbStallReceiveI\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_16:
        DC32     `?<Constant "\\"usbStallTransmitI\\"()">`
//  572 
//  573 /**
//  574  * @brief   USB reset routine.
//  575  * @details This function must be invoked when an USB bus reset condition is
//  576  *          detected.
//  577  *
//  578  * @param[in] usbp      pointer to the @p USBDriver object
//  579  *
//  580  * @notapi
//  581  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock17 Using cfiCommon0
          CFI Function _usb_reset
        THUMB
//  582 void _usb_reset(USBDriver *usbp) {
_usb_reset:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  583   unsigned i;
//  584 
//  585   usbp->state         = USB_READY;
        MOVS     R0,#+2
        STRB     R0,[R4, #+0]
//  586   usbp->status        = 0;
        MOVS     R0,#+0
        STRH     R0,[R4, #+76]
//  587   usbp->address       = 0;
        MOVS     R0,#+0
        STRB     R0,[R4, #+78]
//  588   usbp->configuration = 0;
        MOVS     R0,#+0
        STRB     R0,[R4, #+79]
//  589   usbp->transmitting  = 0;
        MOVS     R0,#+0
        STRH     R0,[R4, #+8]
//  590   usbp->receiving     = 0;
        MOVS     R0,#+0
        STRH     R0,[R4, #+10]
//  591 
//  592   /* Invalidates all endpoints into the USBDriver structure.*/
//  593   for (i = 0; i <= USB_MAX_ENDPOINTS; i++)
        MOVS     R0,#+0
        MOVS     R5,R0
??_usb_reset_0:
        CMP      R5,#+4
        BCS.N    ??_usb_reset_1
//  594     usbp->epc[i] = NULL;
        ADDS     R0,R4,R5, LSL #+2
        MOVS     R1,#+0
        STR      R1,[R0, #+12]
        ADDS     R5,R5,#+1
        B.N      ??_usb_reset_0
//  595 
//  596   /* EP0 state machine initialization.*/
//  597   usbp->ep0state = USB_EP0_WAITING_SETUP;
??_usb_reset_1:
        MOVS     R0,#+0
        STRB     R0,[R4, #+52]
//  598 
//  599   /* Low level reset.*/
//  600   usb_lld_reset(usbp);
        MOVS     R0,R4
          CFI FunCall usb_lld_reset
        BL       usb_lld_reset
//  601 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock17
//  602 
//  603 /**
//  604  * @brief   Default EP0 SETUP callback.
//  605  * @details This function is used by the low level driver as default handler
//  606  *          for EP0 SETUP events.
//  607  *
//  608  * @param[in] usbp      pointer to the @p USBDriver object
//  609  * @param[in] ep        endpoint number, always zero
//  610  *
//  611  * @notapi
//  612  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock18 Using cfiCommon0
          CFI Function _usb_ep0setup
        THUMB
//  613 void _usb_ep0setup(USBDriver *usbp, usbep_t ep) {
_usb_ep0setup:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  614   size_t max;
//  615 
//  616   usbp->ep0state = USB_EP0_WAITING_SETUP;
        MOVS     R0,#+0
        STRB     R0,[R4, #+52]
//  617   usbReadSetup(usbp, ep, usbp->setup);
        ADDS     R2,R4,#+68
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
          CFI FunCall usb_lld_read_setup
        BL       usb_lld_read_setup
//  618 
//  619   /* First verify if the application has an handler installed for this
//  620      request.*/
//  621   if (!(usbp->config->requests_hook_cb) ||
//  622       !(usbp->config->requests_hook_cb(usbp))) {
        LDR      R0,[R4, #+4]
        LDR      R0,[R0, #+8]
        CMP      R0,#+0
        BEQ.N    ??_usb_ep0setup_0
        MOVS     R0,R4
        LDR      R1,[R4, #+4]
        LDR      R1,[R1, #+8]
          CFI FunCall
        BLX      R1
        CMP      R0,#+0
        BNE.N    ??_usb_ep0setup_1
//  623     /* Invoking the default handler, if this fails then stalls the
//  624        endpoint zero as error.*/
//  625     if (((usbp->setup[0] & USB_RTYPE_TYPE_MASK) != USB_RTYPE_TYPE_STD) ||
//  626         !default_handler(usbp)) {
??_usb_ep0setup_0:
        LDRB     R0,[R4, #+68]
        MOVS     R1,#+96
        TST      R0,R1
        BNE.N    ??_usb_ep0setup_2
        MOVS     R0,R4
          CFI FunCall default_handler
        BL       default_handler
        CMP      R0,#+0
        BNE.N    ??_usb_ep0setup_1
//  627       /* Error response, the state machine goes into an error state, the low
//  628          level layer will have to reset it to USB_EP0_WAITING_SETUP after
//  629          receiving a SETUP packet.*/
//  630       usb_lld_stall_in(usbp, 0);
??_usb_ep0setup_2:
        MOVS     R1,#+0
        MOVS     R0,R4
          CFI FunCall usb_lld_stall_in
        BL       usb_lld_stall_in
//  631       usb_lld_stall_out(usbp, 0);
        MOVS     R1,#+0
        MOVS     R0,R4
          CFI FunCall usb_lld_stall_out
        BL       usb_lld_stall_out
//  632       _usb_isr_invoke_event_cb(usbp, USB_EVENT_STALLED);
        LDR      R0,[R4, #+4]
        LDR      R0,[R0, #+0]
        CMP      R0,#+0
        BEQ.N    ??_usb_ep0setup_3
        MOVS     R1,#+5
        MOVS     R0,R4
        LDR      R2,[R4, #+4]
        LDR      R2,[R2, #+0]
          CFI FunCall
        BLX      R2
//  633       usbp->ep0state = USB_EP0_ERROR;
??_usb_ep0setup_3:
        MOVS     R0,#+6
        STRB     R0,[R4, #+52]
//  634       return;
        B.N      ??_usb_ep0setup_4
//  635     }
//  636   }
//  637 #if (USB_SET_ADDRESS_ACK_HANDLING == USB_SET_ADDRESS_ACK_HW)
//  638   if (usbp->setup[1] == USB_REQ_SET_ADDRESS) {
//  639     /* Zero-length packet sent by hardware */
//  640     return;
//  641   }
//  642 #endif
//  643   /* Transfer preparation. The request handler must have populated
//  644      correctly the fields ep0next, ep0n and ep0endcb using the macro
//  645      usbSetupTransfer().*/
//  646   max = usbFetchWord(&usbp->setup[6]);
??_usb_ep0setup_1:
        LDRB     R0,[R4, #+74]
        LDRB     R1,[R4, #+75]
        ORRS     R0,R0,R1, LSL #+8
        MOVS     R6,R0
//  647   /* The transfer size cannot exceed the specified amount.*/
//  648   if (usbp->ep0n > max)
        LDR      R0,[R4, #+60]
        CMP      R6,R0
        BCS.N    ??_usb_ep0setup_5
//  649     usbp->ep0n = max;
        STR      R6,[R4, #+60]
//  650   if ((usbp->setup[0] & USB_RTYPE_DIR_MASK) == USB_RTYPE_DIR_DEV2HOST) {
??_usb_ep0setup_5:
        LDRB     R0,[R4, #+68]
        LSLS     R0,R0,#+24
        BPL.N    ??_usb_ep0setup_6
//  651     /* IN phase.*/
//  652     if (usbp->ep0n > 0) {
        LDR      R0,[R4, #+60]
        CMP      R0,#+0
        BEQ.N    ??_usb_ep0setup_7
//  653       /* Starts the transmit phase.*/
//  654       usbp->ep0state = USB_EP0_TX;
        MOVS     R0,#+1
        STRB     R0,[R4, #+52]
//  655       usbPrepareTransmit(usbp, 0, usbp->ep0next, usbp->ep0n);
        LDR      R3,[R4, #+60]
        LDR      R2,[R4, #+56]
        MOVS     R1,#+0
        MOVS     R0,R4
          CFI FunCall usbPrepareTransmit
        BL       usbPrepareTransmit
//  656       chSysLockFromIsr();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock_from_isr
        BL       dbg_check_lock_from_isr
//  657       usbStartTransmitI(usbp, 0);
        MOVS     R1,#+0
        MOVS     R0,R4
          CFI FunCall usbStartTransmitI
        BL       usbStartTransmitI
//  658       chSysUnlockFromIsr();
          CFI FunCall dbg_check_unlock_from_isr
        BL       dbg_check_unlock_from_isr
        MOVS     R0,#+0
        MSR      BASEPRI,R0
        B.N      ??_usb_ep0setup_8
//  659     }
//  660     else {
//  661       /* No transmission phase, directly receiving the zero sized status
//  662          packet.*/
//  663       usbp->ep0state = USB_EP0_WAITING_STS;
??_usb_ep0setup_7:
        MOVS     R0,#+3
        STRB     R0,[R4, #+52]
//  664 #if (USB_EP0_STATUS_STAGE == USB_EP0_STATUS_STAGE_SW)
//  665       usbPrepareReceive(usbp, 0, NULL, 0);
        MOVS     R3,#+0
        MOVS     R2,#+0
        MOVS     R1,#+0
        MOVS     R0,R4
          CFI FunCall usbPrepareReceive
        BL       usbPrepareReceive
//  666       chSysLockFromIsr();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock_from_isr
        BL       dbg_check_lock_from_isr
//  667       usbStartReceiveI(usbp, 0);
        MOVS     R1,#+0
        MOVS     R0,R4
          CFI FunCall usbStartReceiveI
        BL       usbStartReceiveI
//  668       chSysUnlockFromIsr();
          CFI FunCall dbg_check_unlock_from_isr
        BL       dbg_check_unlock_from_isr
        MOVS     R0,#+0
        MSR      BASEPRI,R0
        B.N      ??_usb_ep0setup_8
//  669 #else
//  670       usb_lld_end_setup(usbp, ep);
//  671 #endif
//  672     }
//  673   }
//  674   else {
//  675     /* OUT phase.*/
//  676     if (usbp->ep0n > 0) {
??_usb_ep0setup_6:
        LDR      R0,[R4, #+60]
        CMP      R0,#+0
        BEQ.N    ??_usb_ep0setup_9
//  677       /* Starts the receive phase.*/
//  678       usbp->ep0state = USB_EP0_RX;
        MOVS     R0,#+4
        STRB     R0,[R4, #+52]
//  679       usbPrepareReceive(usbp, 0, usbp->ep0next, usbp->ep0n);
        LDR      R3,[R4, #+60]
        LDR      R2,[R4, #+56]
        MOVS     R1,#+0
        MOVS     R0,R4
          CFI FunCall usbPrepareReceive
        BL       usbPrepareReceive
//  680       chSysLockFromIsr();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock_from_isr
        BL       dbg_check_lock_from_isr
//  681       usbStartReceiveI(usbp, 0);
        MOVS     R1,#+0
        MOVS     R0,R4
          CFI FunCall usbStartReceiveI
        BL       usbStartReceiveI
//  682       chSysUnlockFromIsr();
          CFI FunCall dbg_check_unlock_from_isr
        BL       dbg_check_unlock_from_isr
        MOVS     R0,#+0
        MSR      BASEPRI,R0
        B.N      ??_usb_ep0setup_8
//  683     }
//  684     else {
//  685       /* No receive phase, directly sending the zero sized status
//  686          packet.*/
//  687       usbp->ep0state = USB_EP0_SENDING_STS;
??_usb_ep0setup_9:
        MOVS     R0,#+5
        STRB     R0,[R4, #+52]
//  688 #if (USB_EP0_STATUS_STAGE == USB_EP0_STATUS_STAGE_SW)
//  689       usbPrepareTransmit(usbp, 0, NULL, 0);
        MOVS     R3,#+0
        MOVS     R2,#+0
        MOVS     R1,#+0
        MOVS     R0,R4
          CFI FunCall usbPrepareTransmit
        BL       usbPrepareTransmit
//  690       chSysLockFromIsr();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock_from_isr
        BL       dbg_check_lock_from_isr
//  691       usbStartTransmitI(usbp, 0);
        MOVS     R1,#+0
        MOVS     R0,R4
          CFI FunCall usbStartTransmitI
        BL       usbStartTransmitI
//  692       chSysUnlockFromIsr();
          CFI FunCall dbg_check_unlock_from_isr
        BL       dbg_check_unlock_from_isr
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  693 #else
//  694       usb_lld_end_setup(usbp, ep);
//  695 #endif
//  696     }
//  697   }
//  698 }
??_usb_ep0setup_8:
??_usb_ep0setup_4:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock18
//  699 
//  700 /**
//  701  * @brief   Default EP0 IN callback.
//  702  * @details This function is used by the low level driver as default handler
//  703  *          for EP0 IN events.
//  704  *
//  705  * @param[in] usbp      pointer to the @p USBDriver object
//  706  * @param[in] ep        endpoint number, always zero
//  707  *
//  708  * @notapi
//  709  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock19 Using cfiCommon0
          CFI Function _usb_ep0in
        THUMB
//  710 void _usb_ep0in(USBDriver *usbp, usbep_t ep) {
_usb_ep0in:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  711   size_t max;
//  712 
//  713   (void)ep;
//  714   switch (usbp->ep0state) {
        LDRB     R0,[R4, #+52]
        CMP      R0,#+1
        BEQ.N    ??_usb_ep0in_0
        CMP      R0,#+2
        BEQ.N    ??_usb_ep0in_1
        CMP      R0,#+5
        BEQ.N    ??_usb_ep0in_2
        B.N      ??_usb_ep0in_3
//  715   case USB_EP0_TX:
//  716     max = usbFetchWord(&usbp->setup[6]);
??_usb_ep0in_0:
        LDRB     R0,[R4, #+74]
        LDRB     R1,[R4, #+75]
        ORRS     R0,R0,R1, LSL #+8
        MOVS     R6,R0
//  717     /* If the transmitted size is less than the requested size and it is a
//  718        multiple of the maximum packet size then a zero size packet must be
//  719        transmitted.*/
//  720     if ((usbp->ep0n < max) && ((usbp->ep0n % usbp->epc[0]->in_maxsize) == 0)) {
        LDR      R0,[R4, #+60]
        CMP      R0,R6
        BCS.N    ??_usb_ep0in_1
        LDR      R0,[R4, #+60]
        LDR      R1,[R4, #+12]
        LDRH     R1,[R1, #+16]
        UDIV     R2,R0,R1
        MLS      R0,R1,R2,R0
        CMP      R0,#+0
        BNE.N    ??_usb_ep0in_1
//  721       usbPrepareTransmit(usbp, 0, NULL, 0);
        MOVS     R3,#+0
        MOVS     R2,#+0
        MOVS     R1,#+0
        MOVS     R0,R4
          CFI FunCall usbPrepareTransmit
        BL       usbPrepareTransmit
//  722       chSysLockFromIsr();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock_from_isr
        BL       dbg_check_lock_from_isr
//  723       usbStartTransmitI(usbp, 0);
        MOVS     R1,#+0
        MOVS     R0,R4
          CFI FunCall usbStartTransmitI
        BL       usbStartTransmitI
//  724       chSysUnlockFromIsr();
          CFI FunCall dbg_check_unlock_from_isr
        BL       dbg_check_unlock_from_isr
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  725       usbp->ep0state = USB_EP0_WAITING_TX0;
        MOVS     R0,#+2
        STRB     R0,[R4, #+52]
//  726       return;
        B.N      ??_usb_ep0in_4
//  727     }
//  728     /* Falls into, it is intentional.*/
//  729   case USB_EP0_WAITING_TX0:
//  730     /* Transmit phase over, receiving the zero sized status packet.*/
//  731     usbp->ep0state = USB_EP0_WAITING_STS;
??_usb_ep0in_1:
        MOVS     R0,#+3
        STRB     R0,[R4, #+52]
//  732 #if (USB_EP0_STATUS_STAGE == USB_EP0_STATUS_STAGE_SW)
//  733     usbPrepareReceive(usbp, 0, NULL, 0);
        MOVS     R3,#+0
        MOVS     R2,#+0
        MOVS     R1,#+0
        MOVS     R0,R4
          CFI FunCall usbPrepareReceive
        BL       usbPrepareReceive
//  734     chSysLockFromIsr();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock_from_isr
        BL       dbg_check_lock_from_isr
//  735     usbStartReceiveI(usbp, 0);
        MOVS     R1,#+0
        MOVS     R0,R4
          CFI FunCall usbStartReceiveI
        BL       usbStartReceiveI
//  736     chSysUnlockFromIsr();
          CFI FunCall dbg_check_unlock_from_isr
        BL       dbg_check_unlock_from_isr
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  737 #else
//  738     usb_lld_end_setup(usbp, ep);
//  739 #endif
//  740     return;
        B.N      ??_usb_ep0in_4
//  741   case USB_EP0_SENDING_STS:
//  742     /* Status packet sent, invoking the callback if defined.*/
//  743     if (usbp->ep0endcb != NULL)
??_usb_ep0in_2:
        LDR      R0,[R4, #+64]
        CMP      R0,#+0
        BEQ.N    ??_usb_ep0in_5
//  744       usbp->ep0endcb(usbp);
        MOVS     R0,R4
        LDR      R1,[R4, #+64]
          CFI FunCall
        BLX      R1
//  745     usbp->ep0state = USB_EP0_WAITING_SETUP;
??_usb_ep0in_5:
        MOVS     R0,#+0
        STRB     R0,[R4, #+52]
//  746     return;
        B.N      ??_usb_ep0in_4
//  747   default:
//  748     ;
//  749   }
//  750   /* Error response, the state machine goes into an error state, the low
//  751      level layer will have to reset it to USB_EP0_WAITING_SETUP after
//  752      receiving a SETUP packet.*/
//  753   usb_lld_stall_in(usbp, 0);
??_usb_ep0in_3:
        MOVS     R1,#+0
        MOVS     R0,R4
          CFI FunCall usb_lld_stall_in
        BL       usb_lld_stall_in
//  754   usb_lld_stall_out(usbp, 0);
        MOVS     R1,#+0
        MOVS     R0,R4
          CFI FunCall usb_lld_stall_out
        BL       usb_lld_stall_out
//  755   _usb_isr_invoke_event_cb(usbp, USB_EVENT_STALLED);
        LDR      R0,[R4, #+4]
        LDR      R0,[R0, #+0]
        CMP      R0,#+0
        BEQ.N    ??_usb_ep0in_6
        MOVS     R1,#+5
        MOVS     R0,R4
        LDR      R2,[R4, #+4]
        LDR      R2,[R2, #+0]
          CFI FunCall
        BLX      R2
//  756   usbp->ep0state = USB_EP0_ERROR;
??_usb_ep0in_6:
        MOVS     R0,#+6
        STRB     R0,[R4, #+52]
//  757 }
??_usb_ep0in_4:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock19
//  758 
//  759 /**
//  760  * @brief   Default EP0 OUT callback.
//  761  * @details This function is used by the low level driver as default handler
//  762  *          for EP0 OUT events.
//  763  *
//  764  * @param[in] usbp      pointer to the @p USBDriver object
//  765  * @param[in] ep        endpoint number, always zero
//  766  *
//  767  * @notapi
//  768  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock20 Using cfiCommon0
          CFI Function _usb_ep0out
        THUMB
//  769 void _usb_ep0out(USBDriver *usbp, usbep_t ep) {
_usb_ep0out:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  770 
//  771   (void)ep;
//  772   switch (usbp->ep0state) {
        LDRB     R0,[R4, #+52]
        CMP      R0,#+3
        BEQ.N    ??_usb_ep0out_0
        CMP      R0,#+4
        BNE.N    ??_usb_ep0out_1
//  773   case USB_EP0_RX:
//  774     /* Receive phase over, sending the zero sized status packet.*/
//  775     usbp->ep0state = USB_EP0_SENDING_STS;
??_usb_ep0out_2:
        MOVS     R0,#+5
        STRB     R0,[R4, #+52]
//  776 #if (USB_EP0_STATUS_STAGE == USB_EP0_STATUS_STAGE_SW)
//  777     usbPrepareTransmit(usbp, 0, NULL, 0);
        MOVS     R3,#+0
        MOVS     R2,#+0
        MOVS     R1,#+0
        MOVS     R0,R4
          CFI FunCall usbPrepareTransmit
        BL       usbPrepareTransmit
//  778     chSysLockFromIsr();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock_from_isr
        BL       dbg_check_lock_from_isr
//  779     usbStartTransmitI(usbp, 0);
        MOVS     R1,#+0
        MOVS     R0,R4
          CFI FunCall usbStartTransmitI
        BL       usbStartTransmitI
//  780     chSysUnlockFromIsr();
          CFI FunCall dbg_check_unlock_from_isr
        BL       dbg_check_unlock_from_isr
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  781 #else
//  782     usb_lld_end_setup(usbp, ep);
//  783 #endif
//  784     return;
        B.N      ??_usb_ep0out_3
//  785   case USB_EP0_WAITING_STS:
//  786     /* Status packet received, it must be zero sized, invoking the callback
//  787        if defined.*/
//  788 #if (USB_EP0_STATUS_STAGE == USB_EP0_STATUS_STAGE_SW)
//  789     if (usbGetReceiveTransactionSizeI(usbp, 0) != 0)
??_usb_ep0out_0:
        LDR      R0,[R4, #+12]
        LDR      R0,[R0, #+24]
        LDR      R0,[R0, #+8]
        CMP      R0,#+0
        BNE.N    ??_usb_ep0out_1
//  790       break;
//  791 #endif
//  792     if (usbp->ep0endcb != NULL)
??_usb_ep0out_4:
        LDR      R0,[R4, #+64]
        CMP      R0,#+0
        BEQ.N    ??_usb_ep0out_5
//  793       usbp->ep0endcb(usbp);
        MOVS     R0,R4
        LDR      R1,[R4, #+64]
          CFI FunCall
        BLX      R1
//  794     usbp->ep0state = USB_EP0_WAITING_SETUP;
??_usb_ep0out_5:
        MOVS     R0,#+0
        STRB     R0,[R4, #+52]
//  795     return;
        B.N      ??_usb_ep0out_3
//  796   default:
//  797     ;
//  798   }
//  799   /* Error response, the state machine goes into an error state, the low
//  800      level layer will have to reset it to USB_EP0_WAITING_SETUP after
//  801      receiving a SETUP packet.*/
//  802   usb_lld_stall_in(usbp, 0);
??_usb_ep0out_1:
        MOVS     R1,#+0
        MOVS     R0,R4
          CFI FunCall usb_lld_stall_in
        BL       usb_lld_stall_in
//  803   usb_lld_stall_out(usbp, 0);
        MOVS     R1,#+0
        MOVS     R0,R4
          CFI FunCall usb_lld_stall_out
        BL       usb_lld_stall_out
//  804   _usb_isr_invoke_event_cb(usbp, USB_EVENT_STALLED);
        LDR      R0,[R4, #+4]
        LDR      R0,[R0, #+0]
        CMP      R0,#+0
        BEQ.N    ??_usb_ep0out_6
        MOVS     R1,#+5
        MOVS     R0,R4
        LDR      R2,[R4, #+4]
        LDR      R2,[R2, #+0]
          CFI FunCall
        BLX      R2
//  805   usbp->ep0state = USB_EP0_ERROR;
??_usb_ep0out_6:
        MOVS     R0,#+6
        STRB     R0,[R4, #+52]
//  806 }
??_usb_ep0out_3:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock20

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        END
//  807 
//  808 #endif /* HAL_USE_USB */
//  809 
//  810 /** @} */
// 
//   362 bytes in section .rodata
// 2 490 bytes in section .text
// 
// 2 468 bytes of CODE  memory (+ 22 bytes shared)
//   362 bytes of CONST memory
//
//Errors: none
//Warnings: none
