///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:10:06 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       platforms\STM32\OTGv1\usb_lld.c                      /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       platforms\STM32\OTGv1\usb_lld.c -lCN                 /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\Debug\List\ /
//                        -lA F:\stuff\rusefi_sourceforge\firmware\iar\Debug\ /
//                       List\ -o F:\stuff\rusefi_sourceforge\firmware\iar\De /
//                       bug\Obj\ --no_cse --no_unroll --no_inline            /
//                       --no_code_motion --no_tbaa --no_clustering           /
//                       --no_scheduling --debug --endian=little              /
//                       --cpu=Cortex-M4 -e --fpu=VFPv4_sp --dlib_config      /
//                       "C:\Program Files (x86)\IAR Systems\Embedded         /
//                       Workbench 6.5\arm\INC\c\DLib_Config_Normal.h"        /
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
//                       usb_lld.s                                            /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME usb_lld

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN __aeabi_memcpy
        EXTERN _port_irq_epilogue
        EXTERN _thread_memfill
        EXTERN _usb_ep0in
        EXTERN _usb_ep0out
        EXTERN _usb_ep0setup
        EXTERN _usb_reset
        EXTERN chDbgPanic
        EXTERN chSchGoSleepS
        EXTERN chSchReadyI
        EXTERN chSchRescheduleS
        EXTERN chThdCreateI
        EXTERN dbg_check_enter_isr
        EXTERN dbg_check_leave_isr
        EXTERN dbg_check_lock
        EXTERN dbg_check_lock_from_isr
        EXTERN dbg_check_unlock
        EXTERN dbg_check_unlock_from_isr
        EXTERN halPolledDelay
        EXTERN nvicEnableVector
        EXTERN rlist
        EXTERN usbObjectInit

        PUBLIC USBD1
        PUBLIC Vector14C
        PUBLIC memcpy
        PUBLIC usb_lld_clear_in
        PUBLIC usb_lld_clear_out
        PUBLIC usb_lld_disable_endpoints
        PUBLIC usb_lld_get_status_in
        PUBLIC usb_lld_get_status_out
        PUBLIC usb_lld_init
        PUBLIC usb_lld_init_endpoint
        PUBLIC usb_lld_prepare_receive
        PUBLIC usb_lld_prepare_transmit
        PUBLIC usb_lld_read_setup
        PUBLIC usb_lld_reset
        PUBLIC usb_lld_set_address
        PUBLIC usb_lld_stall_in
        PUBLIC usb_lld_stall_out
        PUBLIC usb_lld_start
        PUBLIC usb_lld_start_in
        PUBLIC usb_lld_start_out
        PUBLIC usb_lld_stop
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\platforms\STM32\OTGv1\usb_lld.c
//    1 /*
//    2     ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio
//    3 
//    4     Licensed under the Apache License, Version 2.0 (the "License");
//    5     you may not use this file except in compliance with the License.
//    6     You may obtain a copy of the License at
//    7 
//    8         http://www.apache.org/licenses/LICENSE-2.0
//    9 
//   10     Unless required by applicable law or agreed to in writing, software
//   11     distributed under the License is distributed on an "AS IS" BASIS,
//   12     WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   13     See the License for the specific language governing permissions and
//   14     limitations under the License.
//   15 */
//   16 
//   17 /**
//   18  * @file    STM32/OTGv1/usb_lld.c
//   19  * @brief   STM32 USB subsystem low level driver source.
//   20  *
//   21  * @addtogroup USB
//   22  * @{
//   23  */
//   24 
//   25 #include <string.h>

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP memcpy
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function memcpy
        THUMB
// __intrinsic __nounwind __interwork __softfp void *memcpy(void *, void const *, size_t)
memcpy:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
        MOVS     R2,R6
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall __aeabi_memcpy
        BL       __aeabi_memcpy
        MOVS     R0,R4
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock0
//   26 
//   27 #include "ch.h"

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function fifo_remove
          CFI NoCalls
        THUMB
// static __interwork __softfp struct Thread *fifo_remove(ThreadsQueue *)
fifo_remove:
        MOVS     R1,R0
        LDR      R0,[R1, #+0]
        LDR      R2,[R0, #+0]
        STR      R2,[R1, #+0]
        STR      R1,[R2, #+4]
        BX       LR               ;; return
          CFI EndBlock cfiBlock1
//   28 #include "hal.h"
//   29 
//   30 #if HAL_USE_USB || defined(__DOXYGEN__)
//   31 
//   32 /*===========================================================================*/
//   33 /* Driver local definitions.                                                 */
//   34 /*===========================================================================*/
//   35 
//   36 #define TRDT_VALUE              5
//   37 
//   38 #define EP0_MAX_INSIZE          64
//   39 #define EP0_MAX_OUTSIZE         64
//   40 
//   41 /*===========================================================================*/
//   42 /* Driver exported variables.                                                */
//   43 /*===========================================================================*/
//   44 
//   45 /** @brief OTG_FS driver identifier.*/
//   46 #if STM32_USB_USE_OTG1 || defined(__DOXYGEN__)

        SECTION `.bss`:DATA:REORDER:NOROOT(3)
//   47 USBDriver USBD1;
USBD1:
        DS8 1440
//   48 #endif
//   49 
//   50 /** @brief OTG_HS driver identifier.*/
//   51 #if STM32_USB_USE_OTG2 || defined(__DOXYGEN__)
//   52 USBDriver USBD2;
//   53 #endif
//   54 
//   55 /*===========================================================================*/
//   56 /* Driver local variables and types.                                         */
//   57 /*===========================================================================*/
//   58 
//   59 /**
//   60  * @brief   EP0 state.
//   61  * @note    It is an union because IN and OUT endpoints are never used at the
//   62  *          same time for EP0.
//   63  */
//   64 static union {
//   65   /**
//   66    * @brief   IN EP0 state.
//   67    */
//   68   USBInEndpointState in;
//   69   /**
//   70    * @brief   OUT EP0 state.
//   71    */
//   72   USBOutEndpointState out;

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   73 } ep0_state;
ep0_state:
        DS8 20
//   74 
//   75 /**
//   76  * @brief   Buffer for the EP0 setup packets.
//   77  */

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   78 static uint8_t ep0setup_buffer[8];
ep0setup_buffer:
        DS8 8

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "otg_fifo_alloc(), #1">`:
        DATA
        DC8 "otg_fifo_alloc(), #1"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "usb_lld_pump">`:
        DATA
        DC8 "usb_lld_pump"
        DC8 0, 0, 0
//   79 
//   80 /**
//   81  * @brief   EP0 initialization structure.
//   82  */

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
//   83 static const USBEndpointConfig ep0config = {
ep0config:
        DATA
        DC32 0, _usb_ep0setup, _usb_ep0in, _usb_ep0out
        DC16 64, 64
        DC32 ep0_state, ep0_state
        DC16 1
        DC8 0, 0
        DC32 ep0setup_buffer
//   84   USB_EP_MODE_TYPE_CTRL,
//   85   _usb_ep0setup,
//   86   _usb_ep0in,
//   87   _usb_ep0out,
//   88   0x40,
//   89   0x40,
//   90   &ep0_state.in,
//   91   &ep0_state.out,
//   92   1,
//   93   ep0setup_buffer
//   94 };
//   95 
//   96 #if STM32_USB_USE_OTG1

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
//   97 static const stm32_otg_params_t fsparams = {
fsparams:
        DATA
        DC32 128, 320, 3
//   98   STM32_USB_OTG1_RX_FIFO_SIZE / 4,
//   99   STM32_OTG1_FIFO_MEM_SIZE,
//  100   STM32_OTG1_ENDOPOINTS_NUMBER
//  101 };
//  102 #endif
//  103 
//  104 #if STM32_USB_USE_OTG2
//  105 static const stm32_otg_params_t hsparams = {
//  106   STM32_USB_OTG2_RX_FIFO_SIZE / 4,
//  107   STM32_OTG2_FIFO_MEM_SIZE,
//  108   STM32_OTG2_ENDOPOINTS_NUMBER
//  109 };
//  110 #endif
//  111 
//  112 /*===========================================================================*/
//  113 /* Driver local functions.                                                   */
//  114 /*===========================================================================*/
//  115 
//  116 /**
//  117  * @brief   Wakes up the pump thread.
//  118  *
//  119  * @param[in] usbp      pointer to the @p USBDriver object
//  120  *
//  121  * @notapi
//  122  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function usb_lld_wakeup_pump
        THUMB
//  123 static void usb_lld_wakeup_pump(USBDriver *usbp) {
usb_lld_wakeup_pump:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  124 
//  125   if (usbp->thd_wait != NULL) {
        LDR      R0,[R4, #+100]
        CMP      R0,#+0
        BEQ.N    ??usb_lld_wakeup_pump_0
//  126     chThdResumeI(usbp->thd_wait);
        LDR      R0,[R4, #+100]
          CFI FunCall chSchReadyI
        BL       chSchReadyI
//  127     usbp->thd_wait = NULL;
        MOVS     R0,#+0
        STR      R0,[R4, #+100]
//  128   }
//  129 }
??usb_lld_wakeup_pump_0:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock2
//  130 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function otg_core_reset
        THUMB
//  131 static void otg_core_reset(USBDriver *usbp) {
otg_core_reset:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  132   stm32_otg_t *otgp = usbp->otg;
        LDR      R5,[R4, #+80]
//  133 
//  134   halPolledDelay(32);
        MOVS     R0,#+32
          CFI FunCall halPolledDelay
        BL       halPolledDelay
//  135 
//  136   /* Core reset and delay of at least 3 PHY cycles.*/
//  137   otgp->GRSTCTL = GRSTCTL_CSRST;
        MOVS     R0,#+1
        STR      R0,[R5, #+16]
//  138   while ((otgp->GRSTCTL & GRSTCTL_CSRST) != 0)
??otg_core_reset_0:
        LDR      R0,[R5, #+16]
        LSLS     R0,R0,#+31
        BMI.N    ??otg_core_reset_0
//  139     ;
//  140 
//  141   halPolledDelay(12);
        MOVS     R0,#+12
          CFI FunCall halPolledDelay
        BL       halPolledDelay
//  142 
//  143   /* Wait AHB idle condition.*/
//  144   while ((otgp->GRSTCTL & GRSTCTL_AHBIDL) == 0)
??otg_core_reset_1:
        LDR      R0,[R5, #+16]
        CMP      R0,#+0
        BPL.N    ??otg_core_reset_1
//  145     ;
//  146 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock3
//  147 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function otg_disable_ep
          CFI NoCalls
        THUMB
//  148 static void otg_disable_ep(USBDriver *usbp) {
otg_disable_ep:
        PUSH     {R4}
          CFI R4 Frame(CFA, -4)
          CFI CFA R13+4
//  149   stm32_otg_t *otgp = usbp->otg;
        LDR      R1,[R0, #+80]
//  150   unsigned i;
//  151 
//  152   for (i = 0; i <= usbp->otgparams->num_endpoints; i++) {
        MOVS     R3,#+0
        MOVS     R2,R3
??otg_disable_ep_0:
        LDR      R3,[R0, #+84]
        LDR      R3,[R3, #+8]
        CMP      R3,R2
        BCC.N    ??otg_disable_ep_1
//  153     /* Disable only if enabled because this sentence in the manual:
//  154        "The application must set this bit only if Endpoint Enable is
//  155         already set for this endpoint".*/
//  156     if ((otgp->ie[i].DIEPCTL & DIEPCTL_EPENA) != 0) {
        ADDS     R3,R1,R2, LSL #+5
        LDR      R3,[R3, #+2304]
        CMP      R3,#+0
        BPL.N    ??otg_disable_ep_2
//  157       otgp->ie[i].DIEPCTL = DIEPCTL_EPDIS;
        ADDS     R3,R1,R2, LSL #+5
        MOVS     R4,#+1073741824
        STR      R4,[R3, #+2304]
//  158       /* Wait for endpoint disable.*/
//  159       while (!(otgp->ie[i].DIEPINT & DIEPINT_EPDISD))
??otg_disable_ep_3:
        ADDS     R3,R1,R2, LSL #+5
        LDR      R3,[R3, #+2312]
        LSLS     R3,R3,#+30
        BMI.N    ??otg_disable_ep_4
        B.N      ??otg_disable_ep_3
//  160         ;
//  161     }
//  162     else
//  163       otgp->ie[i].DIEPCTL = 0;
??otg_disable_ep_2:
        ADDS     R3,R1,R2, LSL #+5
        MOVS     R4,#+0
        STR      R4,[R3, #+2304]
//  164     otgp->ie[i].DIEPTSIZ = 0;
??otg_disable_ep_4:
        ADDS     R3,R1,R2, LSL #+5
        MOVS     R4,#+0
        STR      R4,[R3, #+2320]
//  165     otgp->ie[i].DIEPINT = 0xFFFFFFFF;
        ADDS     R3,R1,R2, LSL #+5
        MOVS     R4,#-1
        STR      R4,[R3, #+2312]
//  166     /* Disable only if enabled because this sentence in the manual:
//  167        "The application must set this bit only if Endpoint Enable is
//  168         already set for this endpoint".
//  169        Note that the attempt to disable the OUT EP0 is ignored by the
//  170        hardware but the code is simpler this way.*/
//  171     if ((otgp->oe[i].DOEPCTL & DOEPCTL_EPENA) != 0) {
        ADDS     R3,R1,R2, LSL #+5
        LDR      R3,[R3, #+2816]
        CMP      R3,#+0
        BPL.N    ??otg_disable_ep_5
//  172       otgp->oe[i].DOEPCTL = DOEPCTL_EPDIS;
        ADDS     R3,R1,R2, LSL #+5
        MOVS     R4,#+1073741824
        STR      R4,[R3, #+2816]
//  173       /* Wait for endpoint disable.*/
//  174       while (!(otgp->oe[i].DOEPINT & DOEPINT_OTEPDIS))
??otg_disable_ep_6:
        ADDS     R3,R1,R2, LSL #+5
        LDR      R3,[R3, #+2824]
        LSLS     R3,R3,#+27
        BMI.N    ??otg_disable_ep_7
        B.N      ??otg_disable_ep_6
//  175         ;
//  176     }
//  177     else
//  178       otgp->oe[i].DOEPCTL = 0;
??otg_disable_ep_5:
        ADDS     R3,R1,R2, LSL #+5
        MOVS     R4,#+0
        STR      R4,[R3, #+2816]
//  179     otgp->oe[i].DOEPTSIZ = 0;
??otg_disable_ep_7:
        ADDS     R3,R1,R2, LSL #+5
        MOVS     R4,#+0
        STR      R4,[R3, #+2832]
//  180     otgp->oe[i].DOEPINT = 0xFFFFFFFF;
        ADDS     R3,R1,R2, LSL #+5
        MOVS     R4,#-1
        STR      R4,[R3, #+2824]
//  181   }
        ADDS     R2,R2,#+1
        B.N      ??otg_disable_ep_0
//  182   otgp->DAINTMSK = DAINTMSK_OEPM(0) | DAINTMSK_IEPM(0);
??otg_disable_ep_1:
        MOVS     R3,#+65537
        STR      R3,[R1, #+2076]
//  183 }
        POP      {R4}
          CFI R4 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock4
//  184 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function otg_rxfifo_flush
        THUMB
//  185 static void otg_rxfifo_flush(USBDriver *usbp) {
otg_rxfifo_flush:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  186   stm32_otg_t *otgp = usbp->otg;
        LDR      R5,[R4, #+80]
//  187 
//  188   otgp->GRSTCTL = GRSTCTL_RXFFLSH;
        MOVS     R0,#+16
        STR      R0,[R5, #+16]
//  189   while ((otgp->GRSTCTL & GRSTCTL_RXFFLSH) != 0)
??otg_rxfifo_flush_0:
        LDR      R0,[R5, #+16]
        LSLS     R0,R0,#+27
        BMI.N    ??otg_rxfifo_flush_0
//  190     ;
//  191   /* Wait for 3 PHY Clocks.*/
//  192   halPolledDelay(12);
        MOVS     R0,#+12
          CFI FunCall halPolledDelay
        BL       halPolledDelay
//  193 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock5
//  194 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function otg_txfifo_flush
        THUMB
//  195 static void otg_txfifo_flush(USBDriver *usbp, uint32_t fifo) {
otg_txfifo_flush:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  196   stm32_otg_t *otgp = usbp->otg;
        LDR      R6,[R4, #+80]
//  197 
//  198   otgp->GRSTCTL = GRSTCTL_TXFNUM(fifo) | GRSTCTL_TXFFLSH;
        LSLS     R0,R5,#+6
        ORRS     R0,R0,#0x20
        STR      R0,[R6, #+16]
//  199   while ((otgp->GRSTCTL & GRSTCTL_TXFFLSH) != 0)
??otg_txfifo_flush_0:
        LDR      R0,[R6, #+16]
        LSLS     R0,R0,#+26
        BMI.N    ??otg_txfifo_flush_0
//  200     ;
//  201   /* Wait for 3 PHY Clocks.*/
//  202   halPolledDelay(12);
        MOVS     R0,#+12
          CFI FunCall halPolledDelay
        BL       halPolledDelay
//  203 }
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock6
//  204 
//  205 /**
//  206  * @brief   Resets the FIFO RAM memory allocator.
//  207  *
//  208  * @param[in] usbp      pointer to the @p USBDriver object
//  209  *
//  210  * @notapi
//  211  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function otg_ram_reset
          CFI NoCalls
        THUMB
//  212 static void otg_ram_reset(USBDriver *usbp) {
//  213 
//  214   usbp->pmnext = usbp->otgparams->rx_fifo_size;
otg_ram_reset:
        LDR      R1,[R0, #+84]
        LDR      R1,[R1, #+0]
        STR      R1,[R0, #+88]
//  215 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock7
//  216 
//  217 /**
//  218  * @brief   Allocates a block from the FIFO RAM memory.
//  219  *
//  220  * @param[in] usbp      pointer to the @p USBDriver object
//  221  * @param[in] size      size of the packet buffer to allocate in words
//  222  *
//  223  * @notapi
//  224  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function otg_ram_alloc
        THUMB
//  225 static uint32_t otg_ram_alloc(USBDriver *usbp, size_t size) {
otg_ram_alloc:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  226   uint32_t next;
//  227 
//  228   next = usbp->pmnext;
        LDR      R0,[R4, #+88]
        MOVS     R6,R0
//  229   usbp->pmnext += size;
        LDR      R0,[R4, #+88]
        ADDS     R0,R5,R0
        STR      R0,[R4, #+88]
//  230   chDbgAssert(usbp->pmnext <= usbp->otgparams->otg_ram_size,
//  231               "otg_fifo_alloc(), #1", "OTG FIFO memory overflow");
        LDR      R0,[R4, #+84]
        LDR      R0,[R0, #+4]
        LDR      R1,[R4, #+88]
        CMP      R0,R1
        BCS.N    ??otg_ram_alloc_0
        LDR.W    R0,??DataTable6
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  232   return next;
??otg_ram_alloc_0:
        MOVS     R0,R6
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock8
//  233 }
//  234 
//  235 /**
//  236  * @brief   Pushes a series of words into a FIFO.
//  237  *
//  238  * @param[in] fifop     pointer to the FIFO register
//  239  * @param[in] buf       pointer to the words buffer, not necessarily word
//  240  *                      aligned
//  241  * @param[in] n         number of words to push
//  242  *
//  243  * @return              A pointer after the last word pushed.
//  244  *
//  245  * @notapi
//  246  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function otg_do_push
          CFI NoCalls
        THUMB
//  247 static uint8_t *otg_do_push(volatile uint32_t *fifop, uint8_t *buf, size_t n) {
otg_do_push:
        MOVS     R3,R0
        MOVS     R0,R1
//  248 
//  249   while (n > 0) {
??otg_do_push_0:
        CMP      R2,#+0
        BEQ.N    ??otg_do_push_1
//  250     /* Note, this line relies on the Cortex-M3/M4 ability to perform
//  251        unaligned word accesses and on the LSB-first memory organization.*/
//  252     *fifop = *((PACKED_VAR uint32_t *)buf);
        LDR      R1,[R0, #+0]
        STR      R1,[R3, #+0]
//  253     buf += 4;
        ADDS     R0,R0,#+4
//  254     n--;
        SUBS     R2,R2,#+1
        B.N      ??otg_do_push_0
//  255   }
//  256   return buf;
??otg_do_push_1:
        BX       LR               ;; return
          CFI EndBlock cfiBlock9
//  257 }
//  258 
//  259 /**
//  260  * @brief   Writes to a TX FIFO.
//  261  *
//  262  * @param[in] fifop     pointer to the FIFO register
//  263  * @param[in] buf       buffer where to copy the endpoint data
//  264  * @param[in] n         maximum number of bytes to copy
//  265  *
//  266  * @notapi
//  267  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function otg_fifo_write_from_buffer
        THUMB
//  268 static void otg_fifo_write_from_buffer(volatile uint32_t *fifop,
//  269                                        const uint8_t *buf,
//  270                                        size_t n) {
otg_fifo_write_from_buffer:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//  271 
//  272   otg_do_push(fifop, (uint8_t *)buf, (n + 3) / 4);
        ADDS     R0,R6,#+3
        LSRS     R2,R0,#+2
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall otg_do_push
        BL       otg_do_push
//  273 }
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock10
//  274 
//  275 /**
//  276  * @brief   Writes to a TX FIFO fetching data from a queue.
//  277  *
//  278  * @param[in] fifop     pointer to the FIFO register
//  279  * @param[in] oqp       pointer to an @p OutputQueue object
//  280  * @param[in] n         maximum number of bytes to copy
//  281  *
//  282  * @notapi
//  283  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function otg_fifo_write_from_queue
        THUMB
//  284 static void otg_fifo_write_from_queue(volatile uint32_t *fifop,
//  285                                       OutputQueue *oqp,
//  286                                       size_t n) {
otg_fifo_write_from_queue:
        PUSH     {R2,R4-R11,LR}
          CFI R14 Frame(CFA, -4)
          CFI R11 Frame(CFA, -8)
          CFI R10 Frame(CFA, -12)
          CFI R9 Frame(CFA, -16)
          CFI R8 Frame(CFA, -20)
          CFI R7 Frame(CFA, -24)
          CFI R6 Frame(CFA, -28)
          CFI R5 Frame(CFA, -32)
          CFI R4 Frame(CFA, -36)
          CFI CFA R13+40
        MOVS     R4,R0
        MOVS     R5,R1
//  287   size_t ntogo;
//  288 
//  289   ntogo = n;
        LDR      R0,[SP, #+0]
        MOV      R9,R0
//  290   while (ntogo > 0) {
??otg_fifo_write_from_queue_0:
        CMP      R9,#+0
        BEQ.N    ??otg_fifo_write_from_queue_1
//  291     uint32_t w, i;
//  292     size_t nw = ntogo / 4;
        LSRS     R8,R9,#+2
//  293 
//  294     if (nw > 0) {
        CMP      R8,#+0
        BEQ.N    ??otg_fifo_write_from_queue_2
//  295       size_t streak;
//  296       uint32_t nw2end = (oqp->q_top - oqp->q_rdptr) / 4;
        LDR      R0,[R5, #+16]
        LDR      R1,[R5, #+24]
        SUBS     R0,R0,R1
        MOVS     R1,#+4
        SDIV     R11,R0,R1
//  297 
//  298       ntogo -= (streak = nw <= nw2end ? nw : nw2end) * 4;
        CMP      R11,R8
        BCC.N    ??otg_fifo_write_from_queue_3
        MOV      R10,R8
        B.N      ??otg_fifo_write_from_queue_4
??otg_fifo_write_from_queue_3:
        MOV      R10,R11
??otg_fifo_write_from_queue_4:
        SUBS     R9,R9,R10, LSL #+2
//  299       oqp->q_rdptr = otg_do_push(fifop, oqp->q_rdptr, streak);
        MOV      R2,R10
        LDR      R1,[R5, #+24]
        MOVS     R0,R4
          CFI FunCall otg_do_push
        BL       otg_do_push
        STR      R0,[R5, #+24]
//  300       if (oqp->q_rdptr >= oqp->q_top) {
        LDR      R0,[R5, #+24]
        LDR      R1,[R5, #+16]
        CMP      R0,R1
        BCC.N    ??otg_fifo_write_from_queue_2
//  301         oqp->q_rdptr = oqp->q_buffer;
        LDR      R0,[R5, #+12]
        STR      R0,[R5, #+24]
//  302         continue;
        B.N      ??otg_fifo_write_from_queue_0
//  303       }
//  304     }
//  305 
//  306     /* If this condition is not satisfied then there is a word lying across
//  307        queue circular buffer boundary or there are some remaining bytes.*/
//  308     if (ntogo <= 0)
??otg_fifo_write_from_queue_2:
        CMP      R9,#+0
        BEQ.N    ??otg_fifo_write_from_queue_1
//  309       break;
//  310 
//  311     /* One byte at time.*/
//  312     w = 0;
??otg_fifo_write_from_queue_5:
        MOVS     R0,#+0
        MOVS     R6,R0
//  313     i = 0;
        MOVS     R0,#+0
        MOVS     R7,R0
//  314     while ((ntogo > 0) && (i < 4)) {
??otg_fifo_write_from_queue_6:
        CMP      R9,#+0
        BEQ.N    ??otg_fifo_write_from_queue_7
        CMP      R7,#+4
        BCS.N    ??otg_fifo_write_from_queue_7
//  315       w |= (uint32_t)*oqp->q_rdptr++ << (i * 8);
        LDR      R0,[R5, #+24]
        ADDS     R1,R0,#+1
        STR      R1,[R5, #+24]
        LDRB     R0,[R0, #+0]
        MOVS     R1,R7
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        LSLS     R1,R1,#+3
        LSLS     R0,R0,R1
        ORRS     R6,R0,R6
//  316       if (oqp->q_rdptr >= oqp->q_top)
        LDR      R0,[R5, #+24]
        LDR      R1,[R5, #+16]
        CMP      R0,R1
        BCC.N    ??otg_fifo_write_from_queue_8
//  317         oqp->q_rdptr = oqp->q_buffer;
        LDR      R0,[R5, #+12]
        STR      R0,[R5, #+24]
//  318       ntogo--;
??otg_fifo_write_from_queue_8:
        SUBS     R9,R9,#+1
//  319       i++;
        ADDS     R7,R7,#+1
        B.N      ??otg_fifo_write_from_queue_6
//  320     }
//  321     *fifop = w;
??otg_fifo_write_from_queue_7:
        STR      R6,[R4, #+0]
        B.N      ??otg_fifo_write_from_queue_0
//  322   }
//  323 
//  324   /* Updating queue.*/
//  325   chSysLock();
??otg_fifo_write_from_queue_1:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  326   oqp->q_counter += n;
        LDR      R0,[R5, #+8]
        LDR      R1,[SP, #+0]
        ADDS     R0,R1,R0
        STR      R0,[R5, #+8]
//  327   while (notempty(&oqp->q_waiting))
??otg_fifo_write_from_queue_9:
        LDR      R0,[R5, #+0]
        CMP      R0,R5
        BEQ.N    ??otg_fifo_write_from_queue_10
//  328     chSchReadyI(fifo_remove(&oqp->q_waiting))->p_u.rdymsg = Q_OK;
        MOVS     R0,R5
          CFI FunCall fifo_remove
        BL       fifo_remove
          CFI FunCall chSchReadyI
        BL       chSchReadyI
        MOVS     R1,#+0
        STR      R1,[R0, #+40]
        B.N      ??otg_fifo_write_from_queue_9
//  329   chSchRescheduleS();
??otg_fifo_write_from_queue_10:
          CFI FunCall chSchRescheduleS
        BL       chSchRescheduleS
//  330   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  331 }
        POP      {R0,R4-R11,PC}   ;; return
          CFI EndBlock cfiBlock11
//  332 
//  333 /**
//  334  * @brief   Pops a series of words from a FIFO.
//  335  *
//  336  * @param[in] fifop     pointer to the FIFO register
//  337  * @param[in] buf       pointer to the words buffer, not necessarily word
//  338  *                      aligned
//  339  * @param[in] n         number of words to push
//  340  *
//  341  * @return              A pointer after the last word pushed.
//  342  *
//  343  * @notapi
//  344  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function otg_do_pop
          CFI NoCalls
        THUMB
//  345 static uint8_t *otg_do_pop(volatile uint32_t *fifop, uint8_t *buf, size_t n) {
otg_do_pop:
        MOVS     R3,R0
        MOVS     R0,R1
//  346 
//  347   while (n > 0) {
??otg_do_pop_0:
        CMP      R2,#+0
        BEQ.N    ??otg_do_pop_1
//  348     uint32_t w = *fifop;
        LDR      R1,[R3, #+0]
//  349     /* Note, this line relies on the Cortex-M3/M4 ability to perform
//  350        unaligned word accesses and on the LSB-first memory organization.*/
//  351     *((PACKED_VAR uint32_t *)buf) = w;
        STR      R1,[R0, #+0]
//  352     buf += 4;
        ADDS     R0,R0,#+4
//  353     n--;
        SUBS     R2,R2,#+1
        B.N      ??otg_do_pop_0
//  354   }
//  355   return buf;
??otg_do_pop_1:
        BX       LR               ;; return
          CFI EndBlock cfiBlock12
//  356 }
//  357 
//  358 /**
//  359  * @brief   Reads a packet from the RXFIFO.
//  360  *
//  361  * @param[in] fifop     pointer to the FIFO register
//  362  * @param[out] buf      buffer where to copy the endpoint data
//  363  * @param[in] n         number of bytes to pull from the FIFO
//  364  * @param[in] max       number of bytes to copy into the buffer
//  365  *
//  366  * @notapi
//  367  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock13 Using cfiCommon0
          CFI Function otg_fifo_read_to_buffer
          CFI NoCalls
        THUMB
//  368 static void otg_fifo_read_to_buffer(volatile uint32_t *fifop,
//  369                                     uint8_t *buf,
//  370                                     size_t n,
//  371                                     size_t max) {
otg_fifo_read_to_buffer:
        PUSH     {R4}
          CFI R4 Frame(CFA, -4)
          CFI CFA R13+4
//  372 
//  373   n = (n + 3) / 4;
        ADDS     R2,R2,#+3
        LSRS     R2,R2,#+2
//  374   max = (max + 3) / 4;
        ADDS     R3,R3,#+3
        LSRS     R3,R3,#+2
//  375   while (n) {
??otg_fifo_read_to_buffer_0:
        CMP      R2,#+0
        BEQ.N    ??otg_fifo_read_to_buffer_1
//  376     uint32_t w = *fifop;
        LDR      R4,[R0, #+0]
//  377     if (max) {
        CMP      R3,#+0
        BEQ.N    ??otg_fifo_read_to_buffer_2
//  378       /* Note, this line relies on the Cortex-M3/M4 ability to perform
//  379          unaligned word accesses and on the LSB-first memory organization.*/
//  380       *((PACKED_VAR uint32_t *)buf) = w;
        STR      R4,[R1, #+0]
//  381       buf += 4;
        ADDS     R1,R1,#+4
//  382       max--;
        SUBS     R3,R3,#+1
//  383     }
//  384     n--;
??otg_fifo_read_to_buffer_2:
        SUBS     R2,R2,#+1
        B.N      ??otg_fifo_read_to_buffer_0
//  385   }
//  386 }
??otg_fifo_read_to_buffer_1:
        POP      {R4}
          CFI R4 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock13
//  387 
//  388 /**
//  389  * @brief   Reads a packet from the RXFIFO.
//  390  *
//  391  * @param[in] fifop     pointer to the FIFO register
//  392  * @param[in] iqp       pointer to an @p InputQueue object
//  393  * @param[in] n         number of bytes to pull from the FIFO
//  394  *
//  395  * @notapi
//  396  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock14 Using cfiCommon0
          CFI Function otg_fifo_read_to_queue
        THUMB
//  397 static void otg_fifo_read_to_queue(volatile uint32_t *fifop,
//  398                                    InputQueue *iqp,
//  399                                    size_t n) {
otg_fifo_read_to_queue:
        PUSH     {R2,R4-R11,LR}
          CFI R14 Frame(CFA, -4)
          CFI R11 Frame(CFA, -8)
          CFI R10 Frame(CFA, -12)
          CFI R9 Frame(CFA, -16)
          CFI R8 Frame(CFA, -20)
          CFI R7 Frame(CFA, -24)
          CFI R6 Frame(CFA, -28)
          CFI R5 Frame(CFA, -32)
          CFI R4 Frame(CFA, -36)
          CFI CFA R13+40
        MOVS     R4,R0
        MOVS     R5,R1
//  400   size_t ntogo;
//  401 
//  402   ntogo = n;
        LDR      R0,[SP, #+0]
        MOV      R9,R0
//  403   while (ntogo > 0) {
??otg_fifo_read_to_queue_0:
        CMP      R9,#+0
        BEQ.N    ??otg_fifo_read_to_queue_1
//  404     uint32_t w, i;
//  405     size_t nw = ntogo / 4;
        LSRS     R8,R9,#+2
//  406 
//  407     if (nw > 0) {
        CMP      R8,#+0
        BEQ.N    ??otg_fifo_read_to_queue_2
//  408       size_t streak;
//  409       uint32_t nw2end = (iqp->q_wrptr - iqp->q_wrptr) / 4;
        LDR      R0,[R5, #+20]
        LDR      R1,[R5, #+20]
        SUBS     R0,R0,R1
        MOVS     R1,#+4
        SDIV     R11,R0,R1
//  410 
//  411       ntogo -= (streak = nw <= nw2end ? nw : nw2end) * 4;
        CMP      R11,R8
        BCC.N    ??otg_fifo_read_to_queue_3
        MOV      R10,R8
        B.N      ??otg_fifo_read_to_queue_4
??otg_fifo_read_to_queue_3:
        MOV      R10,R11
??otg_fifo_read_to_queue_4:
        SUBS     R9,R9,R10, LSL #+2
//  412       iqp->q_wrptr = otg_do_pop(fifop, iqp->q_wrptr, streak);
        MOV      R2,R10
        LDR      R1,[R5, #+20]
        MOVS     R0,R4
          CFI FunCall otg_do_pop
        BL       otg_do_pop
        STR      R0,[R5, #+20]
//  413       if (iqp->q_wrptr >= iqp->q_top) {
        LDR      R0,[R5, #+20]
        LDR      R1,[R5, #+16]
        CMP      R0,R1
        BCC.N    ??otg_fifo_read_to_queue_2
//  414         iqp->q_wrptr = iqp->q_buffer;
        LDR      R0,[R5, #+12]
        STR      R0,[R5, #+20]
//  415         continue;
        B.N      ??otg_fifo_read_to_queue_0
//  416       }
//  417     }
//  418 
//  419     /* If this condition is not satisfied then there is a word lying across
//  420        queue circular buffer boundary or there are some remaining bytes.*/
//  421     if (ntogo <= 0)
??otg_fifo_read_to_queue_2:
        CMP      R9,#+0
        BEQ.N    ??otg_fifo_read_to_queue_1
//  422       break;
//  423 
//  424     /* One byte at time.*/
//  425     w = *fifop;
??otg_fifo_read_to_queue_5:
        LDR      R0,[R4, #+0]
        MOVS     R6,R0
//  426     i = 0;
        MOVS     R0,#+0
        MOVS     R7,R0
//  427     while ((ntogo > 0) && (i < 4)) {
??otg_fifo_read_to_queue_6:
        CMP      R9,#+0
        BEQ.N    ??otg_fifo_read_to_queue_0
        CMP      R7,#+4
        BCS.N    ??otg_fifo_read_to_queue_0
//  428       *iqp->q_wrptr++ = (uint8_t)(w >> (i * 8));
        LDR      R0,[R5, #+20]
        ADDS     R1,R0,#+1
        STR      R1,[R5, #+20]
        MOVS     R1,R7
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        LSLS     R1,R1,#+3
        LSRS     R1,R6,R1
        STRB     R1,[R0, #+0]
//  429       if (iqp->q_wrptr >= iqp->q_top)
        LDR      R0,[R5, #+20]
        LDR      R1,[R5, #+16]
        CMP      R0,R1
        BCC.N    ??otg_fifo_read_to_queue_7
//  430         iqp->q_wrptr = iqp->q_buffer;
        LDR      R0,[R5, #+12]
        STR      R0,[R5, #+20]
//  431       ntogo--;
??otg_fifo_read_to_queue_7:
        SUBS     R9,R9,#+1
//  432       i++;
        ADDS     R7,R7,#+1
        B.N      ??otg_fifo_read_to_queue_6
//  433     }
//  434   }
//  435 
//  436   /* Updating queue.*/
//  437   chSysLock();
??otg_fifo_read_to_queue_1:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  438   iqp->q_counter += n;
        LDR      R0,[R5, #+8]
        LDR      R1,[SP, #+0]
        ADDS     R0,R1,R0
        STR      R0,[R5, #+8]
//  439   while (notempty(&iqp->q_waiting))
??otg_fifo_read_to_queue_8:
        LDR      R0,[R5, #+0]
        CMP      R0,R5
        BEQ.N    ??otg_fifo_read_to_queue_9
//  440     chSchReadyI(fifo_remove(&iqp->q_waiting))->p_u.rdymsg = Q_OK;
        MOVS     R0,R5
          CFI FunCall fifo_remove
        BL       fifo_remove
          CFI FunCall chSchReadyI
        BL       chSchReadyI
        MOVS     R1,#+0
        STR      R1,[R0, #+40]
        B.N      ??otg_fifo_read_to_queue_8
//  441   chSchRescheduleS();
??otg_fifo_read_to_queue_9:
          CFI FunCall chSchRescheduleS
        BL       chSchRescheduleS
//  442   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  443 }
        POP      {R0,R4-R11,PC}   ;; return
          CFI EndBlock cfiBlock14
//  444 
//  445 /**
//  446  * @brief   Incoming packets handler.
//  447  *
//  448  * @param[in] usbp      pointer to the @p USBDriver object
//  449  *
//  450  * @notapi
//  451  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock15 Using cfiCommon0
          CFI Function otg_rxfifo_handler
        THUMB
//  452 static void otg_rxfifo_handler(USBDriver *usbp) {
otg_rxfifo_handler:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
//  453   uint32_t sts, cnt, ep;
//  454 
//  455   sts = usbp->otg->GRXSTSP;
        LDR      R0,[R4, #+80]
        LDR      R0,[R0, #+32]
        MOVS     R5,R0
//  456   switch (sts & GRXSTSP_PKTSTS_MASK) {
        ANDS     R0,R5,#0x1E0000
        CMP      R0,#+262144
        BEQ.N    ??otg_rxfifo_handler_0
        CMP      R0,#+524288
        BEQ.N    ??otg_rxfifo_handler_1
        CMP      R0,#+786432
        BEQ.N    ??otg_rxfifo_handler_2
        B.N      ??otg_rxfifo_handler_3
//  457   case GRXSTSP_SETUP_COMP:
//  458     break;
??otg_rxfifo_handler_1:
        B.N      ??otg_rxfifo_handler_3
//  459   case GRXSTSP_SETUP_DATA:
//  460     cnt = (sts & GRXSTSP_BCNT_MASK) >> GRXSTSP_BCNT_OFF;
??otg_rxfifo_handler_2:
        UBFX     R0,R5,#+4,#+11
        MOVS     R6,R0
//  461     ep  = (sts & GRXSTSP_EPNUM_MASK) >> GRXSTSP_EPNUM_OFF;
        ANDS     R0,R5,#0xF
        MOVS     R7,R0
//  462     otg_fifo_read_to_buffer(usbp->otg->FIFO[0], usbp->epc[ep]->setup_buf,
//  463                             cnt, 8);
        MOVS     R3,#+8
        MOVS     R2,R6
        ADDS     R0,R4,R7, LSL #+2
        LDR      R0,[R0, #+12]
        LDR      R1,[R0, #+32]
        LDR      R0,[R4, #+80]
        ADDS     R0,R0,#+4096
          CFI FunCall otg_fifo_read_to_buffer
        BL       otg_fifo_read_to_buffer
//  464     break;
        B.N      ??otg_rxfifo_handler_3
//  465   case GRXSTSP_OUT_DATA:
//  466     cnt = (sts & GRXSTSP_BCNT_MASK) >> GRXSTSP_BCNT_OFF;
??otg_rxfifo_handler_0:
        UBFX     R0,R5,#+4,#+11
        MOVS     R6,R0
//  467     ep  = (sts & GRXSTSP_EPNUM_MASK) >> GRXSTSP_EPNUM_OFF;
        ANDS     R0,R5,#0xF
        MOVS     R7,R0
//  468     if (usbp->epc[ep]->out_state->rxqueued) {
        ADDS     R0,R4,R7, LSL #+2
        LDR      R0,[R0, #+12]
        LDR      R0,[R0, #+24]
        LDRB     R0,[R0, #+0]
        CMP      R0,#+0
        BEQ.N    ??otg_rxfifo_handler_4
//  469       /* Queue associated.*/
//  470       otg_fifo_read_to_queue(usbp->otg->FIFO[0],
//  471                              usbp->epc[ep]->out_state->mode.queue.rxqueue,
//  472                              cnt);
        MOVS     R2,R6
        ADDS     R0,R4,R7, LSL #+2
        LDR      R0,[R0, #+12]
        LDR      R0,[R0, #+24]
        LDR      R1,[R0, #+12]
        LDR      R0,[R4, #+80]
        ADDS     R0,R0,#+4096
          CFI FunCall otg_fifo_read_to_queue
        BL       otg_fifo_read_to_queue
        B.N      ??otg_rxfifo_handler_5
//  473     }
//  474     else {
//  475       otg_fifo_read_to_buffer(usbp->otg->FIFO[0],
//  476                               usbp->epc[ep]->out_state->mode.linear.rxbuf,
//  477                               cnt,
//  478                               usbp->epc[ep]->out_state->rxsize -
//  479                               usbp->epc[ep]->out_state->rxcnt);
??otg_rxfifo_handler_4:
        ADDS     R0,R4,R7, LSL #+2
        LDR      R0,[R0, #+12]
        LDR      R0,[R0, #+24]
        LDR      R0,[R0, #+4]
        ADDS     R1,R4,R7, LSL #+2
        LDR      R1,[R1, #+12]
        LDR      R1,[R1, #+24]
        LDR      R1,[R1, #+8]
        SUBS     R3,R0,R1
        MOVS     R2,R6
        ADDS     R0,R4,R7, LSL #+2
        LDR      R0,[R0, #+12]
        LDR      R0,[R0, #+24]
        LDR      R1,[R0, #+12]
        LDR      R0,[R4, #+80]
        ADDS     R0,R0,#+4096
          CFI FunCall otg_fifo_read_to_buffer
        BL       otg_fifo_read_to_buffer
//  480       usbp->epc[ep]->out_state->mode.linear.rxbuf += cnt;
        ADDS     R0,R4,R7, LSL #+2
        LDR      R0,[R0, #+12]
        LDR      R0,[R0, #+24]
        ADDS     R1,R4,R7, LSL #+2
        LDR      R1,[R1, #+12]
        LDR      R1,[R1, #+24]
        LDR      R1,[R1, #+12]
        ADDS     R1,R6,R1
        STR      R1,[R0, #+12]
//  481     }
//  482     usbp->epc[ep]->out_state->rxcnt += cnt;
??otg_rxfifo_handler_5:
        ADDS     R0,R4,R7, LSL #+2
        LDR      R0,[R0, #+12]
        LDR      R0,[R0, #+24]
        ADDS     R1,R4,R7, LSL #+2
        LDR      R1,[R1, #+12]
        LDR      R1,[R1, #+24]
        LDR      R1,[R1, #+8]
        ADDS     R1,R6,R1
        STR      R1,[R0, #+8]
//  483     break;
//  484   case GRXSTSP_OUT_GLOBAL_NAK:
//  485   case GRXSTSP_OUT_COMP:
//  486   default:
//  487     ;
//  488   }
//  489 }
??otg_rxfifo_handler_3:
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock15
//  490 
//  491 /**
//  492  * @brief   Outgoing packets handler.
//  493  *
//  494  * @param[in] usbp      pointer to the @p USBDriver object
//  495  * @param[in] ep        endpoint number
//  496  *
//  497  * @notapi
//  498  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock16 Using cfiCommon0
          CFI Function otg_txfifo_handler
        THUMB
//  499 static bool_t otg_txfifo_handler(USBDriver *usbp, usbep_t ep) {
otg_txfifo_handler:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  500 
//  501   /* The TXFIFO is filled until there is space and data to be transmitted.*/
//  502   while (TRUE) {
//  503     uint32_t n;
//  504 
//  505     /* Transaction end condition.*/
//  506     if (usbp->epc[ep]->in_state->txcnt >= usbp->epc[ep]->in_state->txsize)
??otg_txfifo_handler_0:
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R0,R4,R5, LSL #+2
        LDR      R0,[R0, #+12]
        LDR      R0,[R0, #+20]
        LDR      R0,[R0, #+8]
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R1,R4,R5, LSL #+2
        LDR      R1,[R1, #+12]
        LDR      R1,[R1, #+20]
        LDR      R1,[R1, #+4]
        CMP      R0,R1
        BCC.N    ??otg_txfifo_handler_1
//  507       return TRUE;
        MOVS     R0,#+1
        B.N      ??otg_txfifo_handler_2
//  508 
//  509     /* Number of bytes remaining in current transaction.*/
//  510     n = usbp->epc[ep]->in_state->txsize - usbp->epc[ep]->in_state->txcnt;
??otg_txfifo_handler_1:
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R0,R4,R5, LSL #+2
        LDR      R0,[R0, #+12]
        LDR      R0,[R0, #+20]
        LDR      R0,[R0, #+4]
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R1,R4,R5, LSL #+2
        LDR      R1,[R1, #+12]
        LDR      R1,[R1, #+20]
        LDR      R1,[R1, #+8]
        SUBS     R0,R0,R1
        MOVS     R6,R0
//  511     if (n > usbp->epc[ep]->in_maxsize)
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R0,R4,R5, LSL #+2
        LDR      R0,[R0, #+12]
        LDRH     R0,[R0, #+16]
        CMP      R0,R6
        BCS.N    ??otg_txfifo_handler_3
//  512       n = usbp->epc[ep]->in_maxsize;
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R0,R4,R5, LSL #+2
        LDR      R0,[R0, #+12]
        LDRH     R0,[R0, #+16]
        MOVS     R6,R0
//  513 
//  514     /* Checks if in the TXFIFO there is enough space to accommodate the
//  515        next packet.*/
//  516     if (((usbp->otg->ie[ep].DTXFSTS & DTXFSTS_INEPTFSAV_MASK) * 4) < n)
??otg_txfifo_handler_3:
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        LDR      R0,[R4, #+80]
        ADDS     R0,R0,R5, LSL #+5
        LDR      R0,[R0, #+2328]
        UXTH     R0,R0            ;; ZeroExt  R0,R0,#+16,#+16
        LSLS     R0,R0,#+2
        CMP      R0,R6
        BCS.N    ??otg_txfifo_handler_4
//  517       return FALSE;
        MOVS     R0,#+0
        B.N      ??otg_txfifo_handler_2
//  518 
//  519 #if STM32_USB_OTGFIFO_FILL_BASEPRI
//  520     __set_BASEPRI(CORTEX_PRIORITY_MASK(STM32_USB_OTGFIFO_FILL_BASEPRI));
//  521 #endif
//  522     /* Handles the two cases: linear buffer or queue.*/
//  523     if (usbp->epc[ep]->in_state->txqueued) {
??otg_txfifo_handler_4:
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R0,R4,R5, LSL #+2
        LDR      R0,[R0, #+12]
        LDR      R0,[R0, #+20]
        LDRB     R0,[R0, #+0]
        CMP      R0,#+0
        BEQ.N    ??otg_txfifo_handler_5
//  524       /* Queue associated.*/
//  525       otg_fifo_write_from_queue(usbp->otg->FIFO[ep],
//  526                                 usbp->epc[ep]->in_state->mode.queue.txqueue,
//  527                                 n);
        MOVS     R2,R6
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R0,R4,R5, LSL #+2
        LDR      R0,[R0, #+12]
        LDR      R0,[R0, #+20]
        LDR      R1,[R0, #+12]
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        MOV      R0,#+4096
        LDR      R3,[R4, #+80]
        MLA      R0,R0,R5,R3
        ADDS     R0,R0,#+4096
          CFI FunCall otg_fifo_write_from_queue
        BL       otg_fifo_write_from_queue
        B.N      ??otg_txfifo_handler_6
//  528     }
//  529     else {
//  530       /* Linear buffer associated.*/
//  531       otg_fifo_write_from_buffer(usbp->otg->FIFO[ep],
//  532                                  usbp->epc[ep]->in_state->mode.linear.txbuf,
//  533                                  n);
??otg_txfifo_handler_5:
        MOVS     R2,R6
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R0,R4,R5, LSL #+2
        LDR      R0,[R0, #+12]
        LDR      R0,[R0, #+20]
        LDR      R1,[R0, #+12]
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        MOV      R0,#+4096
        LDR      R3,[R4, #+80]
        MLA      R0,R0,R5,R3
        ADDS     R0,R0,#+4096
          CFI FunCall otg_fifo_write_from_buffer
        BL       otg_fifo_write_from_buffer
//  534       usbp->epc[ep]->in_state->mode.linear.txbuf += n;
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R0,R4,R5, LSL #+2
        LDR      R0,[R0, #+12]
        LDR      R0,[R0, #+20]
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R1,R4,R5, LSL #+2
        LDR      R1,[R1, #+12]
        LDR      R1,[R1, #+20]
        LDR      R1,[R1, #+12]
        ADDS     R1,R6,R1
        STR      R1,[R0, #+12]
//  535     }
//  536 #if STM32_USB_OTGFIFO_FILL_BASEPRI
//  537   __set_BASEPRI(0);
//  538 #endif
//  539     usbp->epc[ep]->in_state->txcnt += n;
??otg_txfifo_handler_6:
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R0,R4,R5, LSL #+2
        LDR      R0,[R0, #+12]
        LDR      R0,[R0, #+20]
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R1,R4,R5, LSL #+2
        LDR      R1,[R1, #+12]
        LDR      R1,[R1, #+20]
        LDR      R1,[R1, #+8]
        ADDS     R1,R6,R1
        STR      R1,[R0, #+8]
        B.N      ??otg_txfifo_handler_0
//  540   }
??otg_txfifo_handler_2:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock16
//  541 }
//  542 
//  543 /**
//  544  * @brief   Generic endpoint IN handler.
//  545  *
//  546  * @param[in] usbp      pointer to the @p USBDriver object
//  547  * @param[in] ep        endpoint number
//  548  *
//  549  * @notapi
//  550  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock17 Using cfiCommon0
          CFI Function otg_epin_handler
        THUMB
//  551 static void otg_epin_handler(USBDriver *usbp, usbep_t ep) {
otg_epin_handler:
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
//  552   stm32_otg_t *otgp = usbp->otg;
        LDR      R6,[R4, #+80]
//  553   uint32_t epint = otgp->ie[ep].DIEPINT;
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R0,R6,R5, LSL #+5
        LDR      R7,[R0, #+2312]
//  554 
//  555   otgp->ie[ep].DIEPINT = epint;
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R0,R6,R5, LSL #+5
        STR      R7,[R0, #+2312]
//  556 
//  557   if (epint & DIEPINT_TOC) {
//  558     /* Timeouts not handled yet, not sure how to handle.*/
//  559   }
//  560   if ((epint & DIEPINT_XFRC) && (otgp->DIEPMSK & DIEPMSK_XFRCM)) {
        LSLS     R0,R7,#+31
        BPL.N    ??otg_epin_handler_0
        LDR      R0,[R6, #+2064]
        LSLS     R0,R0,#+31
        BPL.N    ??otg_epin_handler_0
//  561     /* Transmit transfer complete.*/
//  562     USBInEndpointState *isp = usbp->epc[ep]->in_state;
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R0,R4,R5, LSL #+2
        LDR      R0,[R0, #+12]
        LDR      R8,[R0, #+20]
//  563 
//  564     if (isp->txsize < isp->totsize) {
        LDR      R0,[R8, #+4]
        LDR      R1,[R8, #+16]
        CMP      R0,R1
        BCS.N    ??otg_epin_handler_1
//  565       /* In case the transaction covered only part of the total transfer
//  566          then another transaction is immediately started in order to
//  567          cover the remaining.*/
//  568       isp->txsize = isp->totsize - isp->txsize;
        LDR      R0,[R8, #+16]
        LDR      R1,[R8, #+4]
        SUBS     R0,R0,R1
        STR      R0,[R8, #+4]
//  569       isp->txcnt  = 0;
        MOVS     R0,#+0
        STR      R0,[R8, #+8]
//  570       usb_lld_prepare_transmit(usbp, ep);
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
          CFI FunCall usb_lld_prepare_transmit
        BL       usb_lld_prepare_transmit
//  571       chSysLockFromIsr();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock_from_isr
        BL       dbg_check_lock_from_isr
//  572       usb_lld_start_in(usbp, ep);
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
          CFI FunCall usb_lld_start_in
        BL       usb_lld_start_in
//  573       chSysUnlockFromIsr();
          CFI FunCall dbg_check_unlock_from_isr
        BL       dbg_check_unlock_from_isr
        MOVS     R0,#+0
        MSR      BASEPRI,R0
        B.N      ??otg_epin_handler_0
//  574     }
//  575     else {
//  576       /* End on IN transfer.*/
//  577       _usb_isr_invoke_in_cb(usbp, ep);
??otg_epin_handler_1:
        LDRH     R0,[R4, #+8]
        MOVS     R1,#+1
        LSLS     R1,R1,R5
        BICS     R0,R0,R1
        STRH     R0,[R4, #+8]
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R2,R4,R5, LSL #+2
        LDR      R2,[R2, #+12]
        LDR      R2,[R2, #+8]
          CFI FunCall
        BLX      R2
//  578     }
//  579   }
//  580   if ((epint & DIEPINT_TXFE) &&
//  581       (otgp->DIEPEMPMSK & DIEPEMPMSK_INEPTXFEM(ep))) {
??otg_epin_handler_0:
        LSLS     R0,R7,#+24
        BPL.N    ??otg_epin_handler_2
        LDR      R0,[R6, #+2100]
        LSRS     R0,R0,R5
        LSLS     R0,R0,#+31
        BPL.N    ??otg_epin_handler_2
//  582     /* The thread is made ready, it will be scheduled on ISR exit.*/
//  583     chSysLockFromIsr();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock_from_isr
        BL       dbg_check_lock_from_isr
//  584     usbp->txpending |= (1 << ep);
        LDR      R0,[R4, #+92]
        MOVS     R1,#+1
        LSLS     R1,R1,R5
        ORRS     R0,R1,R0
        STR      R0,[R4, #+92]
//  585     otgp->DIEPEMPMSK &= ~(1 << ep);
        LDR      R0,[R6, #+2100]
        MOVS     R1,#+1
        LSLS     R1,R1,R5
        BICS     R0,R0,R1
        STR      R0,[R6, #+2100]
//  586     usb_lld_wakeup_pump(usbp);
        MOVS     R0,R4
          CFI FunCall usb_lld_wakeup_pump
        BL       usb_lld_wakeup_pump
//  587     chSysUnlockFromIsr();
          CFI FunCall dbg_check_unlock_from_isr
        BL       dbg_check_unlock_from_isr
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  588   }
//  589 }
??otg_epin_handler_2:
        POP      {R4-R8,PC}       ;; return
          CFI EndBlock cfiBlock17
//  590 
//  591 /**
//  592  * @brief   Generic endpoint OUT handler.
//  593  *
//  594  * @param[in] usbp      pointer to the @p USBDriver object
//  595  * @param[in] ep        endpoint number
//  596  *
//  597  * @notapi
//  598  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock18 Using cfiCommon0
          CFI Function otg_epout_handler
        THUMB
//  599 static void otg_epout_handler(USBDriver *usbp, usbep_t ep) {
otg_epout_handler:
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
//  600   stm32_otg_t *otgp = usbp->otg;
        LDR      R6,[R4, #+80]
//  601   uint32_t epint = otgp->oe[ep].DOEPINT;
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R0,R6,R5, LSL #+5
        LDR      R7,[R0, #+2824]
//  602 
//  603   /* Resets all EP IRQ sources.*/
//  604   otgp->oe[ep].DOEPINT = epint;
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R0,R6,R5, LSL #+5
        STR      R7,[R0, #+2824]
//  605 
//  606   if ((epint & DOEPINT_STUP) && (otgp->DOEPMSK & DOEPMSK_STUPM)) {
        LSLS     R0,R7,#+28
        BPL.N    ??otg_epout_handler_0
        LDR      R0,[R6, #+2068]
        LSLS     R0,R0,#+28
        BPL.N    ??otg_epout_handler_0
//  607     /* Setup packets handling, setup packets are handled using a
//  608        specific callback.*/
//  609     _usb_isr_invoke_setup_cb(usbp, ep);
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R2,R4,R5, LSL #+2
        LDR      R2,[R2, #+12]
        LDR      R2,[R2, #+4]
          CFI FunCall
        BLX      R2
//  610 
//  611   }
//  612   if ((epint & DOEPINT_XFRC) && (otgp->DOEPMSK & DOEPMSK_XFRCM)) {
??otg_epout_handler_0:
        LSLS     R0,R7,#+31
        BPL.N    ??otg_epout_handler_1
        LDR      R0,[R6, #+2068]
        LSLS     R0,R0,#+31
        BPL.N    ??otg_epout_handler_1
//  613     /* Receive transfer complete.*/
//  614     USBOutEndpointState *osp = usbp->epc[ep]->out_state;
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R0,R4,R5, LSL #+2
        LDR      R0,[R0, #+12]
        LDR      R8,[R0, #+24]
//  615 
//  616     if (osp->rxsize < osp->totsize) {
        LDR      R0,[R8, #+4]
        LDR      R1,[R8, #+16]
        CMP      R0,R1
        BCS.N    ??otg_epout_handler_2
//  617       /* In case the transaction covered only part of the total transfer
//  618          then another transaction is immediately started in order to
//  619          cover the remaining.*/
//  620       osp->rxsize = osp->totsize - osp->rxsize;
        LDR      R0,[R8, #+16]
        LDR      R1,[R8, #+4]
        SUBS     R0,R0,R1
        STR      R0,[R8, #+4]
//  621       osp->rxcnt  = 0;
        MOVS     R0,#+0
        STR      R0,[R8, #+8]
//  622       usb_lld_prepare_receive(usbp, ep);
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
          CFI FunCall usb_lld_prepare_receive
        BL       usb_lld_prepare_receive
//  623       chSysLockFromIsr();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock_from_isr
        BL       dbg_check_lock_from_isr
//  624       usb_lld_start_out(usbp, ep);
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
          CFI FunCall usb_lld_start_out
        BL       usb_lld_start_out
//  625       chSysUnlockFromIsr();
          CFI FunCall dbg_check_unlock_from_isr
        BL       dbg_check_unlock_from_isr
        MOVS     R0,#+0
        MSR      BASEPRI,R0
        B.N      ??otg_epout_handler_1
//  626     }
//  627     else {
//  628       /* End on OUT transfer.*/
//  629       _usb_isr_invoke_out_cb(usbp, ep);
??otg_epout_handler_2:
        LDRH     R0,[R4, #+10]
        MOVS     R1,#+1
        LSLS     R1,R1,R5
        BICS     R0,R0,R1
        STRH     R0,[R4, #+10]
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R2,R4,R5, LSL #+2
        LDR      R2,[R2, #+12]
        LDR      R2,[R2, #+12]
          CFI FunCall
        BLX      R2
//  630     }
//  631   }
//  632 }
??otg_epout_handler_1:
        POP      {R4-R8,PC}       ;; return
          CFI EndBlock cfiBlock18
//  633 
//  634 /**
//  635  * @brief   OTG shared ISR.
//  636  *
//  637  * @param[in] usbp      pointer to the @p USBDriver object
//  638  *
//  639  * @notapi
//  640  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock19 Using cfiCommon0
          CFI Function usb_lld_serve_interrupt
        THUMB
//  641 static void usb_lld_serve_interrupt(USBDriver *usbp) {
usb_lld_serve_interrupt:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
//  642   stm32_otg_t *otgp = usbp->otg;
        LDR      R5,[R4, #+80]
//  643   uint32_t sts, src;
//  644 
//  645   sts  = otgp->GINTSTS;
        LDR      R0,[R5, #+20]
        MOVS     R7,R0
//  646   sts &= otgp->GINTMSK;
        LDR      R0,[R5, #+24]
        ANDS     R7,R0,R7
//  647   otgp->GINTSTS = sts;
        STR      R7,[R5, #+20]
//  648 
//  649   /* Reset interrupt handling.*/
//  650   if (sts & GINTSTS_USBRST) {
        LSLS     R0,R7,#+19
        BPL.N    ??usb_lld_serve_interrupt_0
//  651     _usb_reset(usbp);
        MOVS     R0,R4
          CFI FunCall _usb_reset
        BL       _usb_reset
//  652     _usb_isr_invoke_event_cb(usbp, USB_EVENT_RESET);
        LDR      R0,[R4, #+4]
        LDR      R0,[R0, #+0]
        CMP      R0,#+0
        BEQ.N    ??usb_lld_serve_interrupt_0
        MOVS     R1,#+0
        MOVS     R0,R4
        LDR      R2,[R4, #+4]
        LDR      R2,[R2, #+0]
          CFI FunCall
        BLX      R2
//  653   }
//  654 
//  655   /* Enumeration done.*/
//  656   if (sts & GINTSTS_ENUMDNE) {
??usb_lld_serve_interrupt_0:
        LSLS     R0,R7,#+18
        BPL.N    ??usb_lld_serve_interrupt_1
//  657     (void)otgp->DSTS;
        LDR      R0,[R5, #+2056]
//  658   }
//  659 
//  660   /* SOF interrupt handling.*/
//  661   if (sts & GINTSTS_SOF) {
??usb_lld_serve_interrupt_1:
        LSLS     R0,R7,#+28
        BPL.N    ??usb_lld_serve_interrupt_2
//  662     _usb_isr_invoke_sof_cb(usbp);
        LDR      R0,[R4, #+4]
        LDR      R0,[R0, #+12]
        CMP      R0,#+0
        BEQ.N    ??usb_lld_serve_interrupt_2
        MOVS     R0,R4
        LDR      R1,[R4, #+4]
        LDR      R1,[R1, #+12]
          CFI FunCall
        BLX      R1
//  663   }
//  664 
//  665   /* RX FIFO not empty handling.*/
//  666   if (sts & GINTSTS_RXFLVL) {
??usb_lld_serve_interrupt_2:
        LSLS     R0,R7,#+27
        BPL.N    ??usb_lld_serve_interrupt_3
//  667     /* The interrupt is masked while the thread has control or it would
//  668        be triggered again.*/
//  669     chSysLockFromIsr();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock_from_isr
        BL       dbg_check_lock_from_isr
//  670     otgp->GINTMSK &= ~GINTMSK_RXFLVLM;
        LDR      R0,[R5, #+24]
        BICS     R0,R0,#0x10
        STR      R0,[R5, #+24]
//  671     usb_lld_wakeup_pump(usbp);
        MOVS     R0,R4
          CFI FunCall usb_lld_wakeup_pump
        BL       usb_lld_wakeup_pump
//  672     chSysUnlockFromIsr();
          CFI FunCall dbg_check_unlock_from_isr
        BL       dbg_check_unlock_from_isr
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  673   }
//  674 
//  675   /* IN/OUT endpoints event handling.*/
//  676   src = otgp->DAINT;
??usb_lld_serve_interrupt_3:
        LDR      R0,[R5, #+2072]
        MOVS     R6,R0
//  677   if (sts & GINTSTS_IEPINT) {
        LSLS     R0,R7,#+13
        BPL.N    ??usb_lld_serve_interrupt_4
//  678     if (src & (1 << 0))
        LSLS     R0,R6,#+31
        BPL.N    ??usb_lld_serve_interrupt_5
//  679       otg_epin_handler(usbp, 0);
        MOVS     R1,#+0
        MOVS     R0,R4
          CFI FunCall otg_epin_handler
        BL       otg_epin_handler
//  680     if (src & (1 << 1))
??usb_lld_serve_interrupt_5:
        LSLS     R0,R6,#+30
        BPL.N    ??usb_lld_serve_interrupt_6
//  681       otg_epin_handler(usbp, 1);
        MOVS     R1,#+1
        MOVS     R0,R4
          CFI FunCall otg_epin_handler
        BL       otg_epin_handler
//  682     if (src & (1 << 2))
??usb_lld_serve_interrupt_6:
        LSLS     R0,R6,#+29
        BPL.N    ??usb_lld_serve_interrupt_7
//  683       otg_epin_handler(usbp, 2);
        MOVS     R1,#+2
        MOVS     R0,R4
          CFI FunCall otg_epin_handler
        BL       otg_epin_handler
//  684     if (src & (1 << 3))
??usb_lld_serve_interrupt_7:
        LSLS     R0,R6,#+28
        BPL.N    ??usb_lld_serve_interrupt_4
//  685       otg_epin_handler(usbp, 3);
        MOVS     R1,#+3
        MOVS     R0,R4
          CFI FunCall otg_epin_handler
        BL       otg_epin_handler
//  686 #if STM32_USB_USE_OTG2
//  687     if (src & (1 << 4))
//  688       otg_epin_handler(usbp, 4);
//  689     if (src & (1 << 5))
//  690       otg_epin_handler(usbp, 5);
//  691 #endif
//  692   }
//  693   if (sts & GINTSTS_OEPINT) {
??usb_lld_serve_interrupt_4:
        LSLS     R0,R7,#+12
        BPL.N    ??usb_lld_serve_interrupt_8
//  694     if (src & (1 << 16))
        LSLS     R0,R6,#+15
        BPL.N    ??usb_lld_serve_interrupt_9
//  695       otg_epout_handler(usbp, 0);
        MOVS     R1,#+0
        MOVS     R0,R4
          CFI FunCall otg_epout_handler
        BL       otg_epout_handler
//  696     if (src & (1 << 17))
??usb_lld_serve_interrupt_9:
        LSLS     R0,R6,#+14
        BPL.N    ??usb_lld_serve_interrupt_10
//  697       otg_epout_handler(usbp, 1);
        MOVS     R1,#+1
        MOVS     R0,R4
          CFI FunCall otg_epout_handler
        BL       otg_epout_handler
//  698     if (src & (1 << 18))
??usb_lld_serve_interrupt_10:
        LSLS     R0,R6,#+13
        BPL.N    ??usb_lld_serve_interrupt_11
//  699       otg_epout_handler(usbp, 2);
        MOVS     R1,#+2
        MOVS     R0,R4
          CFI FunCall otg_epout_handler
        BL       otg_epout_handler
//  700     if (src & (1 << 19))
??usb_lld_serve_interrupt_11:
        LSLS     R0,R6,#+12
        BPL.N    ??usb_lld_serve_interrupt_8
//  701       otg_epout_handler(usbp, 3);
        MOVS     R1,#+3
        MOVS     R0,R4
          CFI FunCall otg_epout_handler
        BL       otg_epout_handler
//  702 #if STM32_USB_USE_OTG2
//  703     if (src & (1 << 20))
//  704       otg_epout_handler(usbp, 4);
//  705     if (src & (1 << 21))
//  706       otg_epout_handler(usbp, 5);
//  707 #endif
//  708   }
//  709 }
??usb_lld_serve_interrupt_8:
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock19
//  710 
//  711 /*===========================================================================*/
//  712 /* Driver interrupt handlers and threads.                                    */
//  713 /*===========================================================================*/
//  714 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock20 Using cfiCommon0
          CFI Function usb_lld_pump
        THUMB
//  715 static msg_t usb_lld_pump(void *p) {
usb_lld_pump:
        PUSH     {R3-R9,LR}
          CFI R14 Frame(CFA, -4)
          CFI R9 Frame(CFA, -8)
          CFI R8 Frame(CFA, -12)
          CFI R7 Frame(CFA, -16)
          CFI R6 Frame(CFA, -20)
          CFI R5 Frame(CFA, -24)
          CFI R4 Frame(CFA, -28)
          CFI CFA R13+32
        MOVS     R4,R0
//  716   USBDriver *usbp = (USBDriver *)p;
        MOVS     R5,R4
//  717   stm32_otg_t *otgp = usbp->otg;
        LDR      R6,[R5, #+80]
//  718 
//  719   chRegSetThreadName("usb_lld_pump");
        LDR.W    R0,??DataTable6_1
        LDR      R0,[R0, #+24]
        LDR.W    R1,??DataTable6_2
        STR      R1,[R0, #+24]
//  720   chSysLock();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  721   while (TRUE) {
//  722     usbep_t ep;
//  723     uint32_t epmask;
//  724 
//  725     /* Nothing to do, going to sleep.*/
//  726     if ((usbp->state == USB_STOP) ||
//  727         ((usbp->txpending == 0) && !(otgp->GINTSTS & GINTSTS_RXFLVL))) {
??usb_lld_pump_0:
        LDRB     R0,[R5, #+0]
        CMP      R0,#+1
        BEQ.N    ??usb_lld_pump_1
        LDR      R0,[R5, #+92]
        CMP      R0,#+0
        BNE.N    ??usb_lld_pump_2
        LDR      R0,[R6, #+20]
        LSLS     R0,R0,#+27
        BMI.N    ??usb_lld_pump_2
//  728       otgp->GINTMSK |= GINTMSK_RXFLVLM;
??usb_lld_pump_1:
        LDR      R0,[R6, #+24]
        ORRS     R0,R0,#0x10
        STR      R0,[R6, #+24]
//  729       usbp->thd_wait = chThdSelf();
        LDR.W    R0,??DataTable6_1
        LDR      R0,[R0, #+24]
        STR      R0,[R5, #+100]
//  730       chSchGoSleepS(THD_STATE_SUSPENDED);
        MOVS     R0,#+2
          CFI FunCall chSchGoSleepS
        BL       chSchGoSleepS
//  731     }
//  732     chSysUnlock();
??usb_lld_pump_2:
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  733 
//  734     /* Checks if there are TXFIFOs to be filled.*/
//  735     for (ep = 0; ep <= usbp->otgparams->num_endpoints; ep++) {
        MOVS     R0,#+0
        MOVS     R7,R0
??usb_lld_pump_3:
        LDR      R0,[R5, #+84]
        LDR      R0,[R0, #+8]
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        CMP      R0,R7
        BCC.N    ??usb_lld_pump_4
//  736 
//  737       /* Empties the RX FIFO.*/
//  738       while (otgp->GINTSTS & GINTSTS_RXFLVL) {
??usb_lld_pump_5:
        LDR      R0,[R6, #+20]
        LSLS     R0,R0,#+27
        BPL.N    ??usb_lld_pump_6
//  739         otg_rxfifo_handler(usbp);
        MOVS     R0,R5
          CFI FunCall otg_rxfifo_handler
        BL       otg_rxfifo_handler
        B.N      ??usb_lld_pump_5
//  740       }
//  741 
//  742       epmask = (1 << ep);
??usb_lld_pump_6:
        MOVS     R0,#+1
        LSLS     R0,R0,R7
        MOV      R8,R0
//  743       if (usbp->txpending & epmask) {
        LDR      R0,[R5, #+92]
        TST      R0,R8
        BEQ.N    ??usb_lld_pump_7
//  744         bool_t done;
//  745 
//  746         chSysLock();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  747         /* USB interrupts are globally *suspended* because the peripheral
//  748            does not allow any interference during the TX FIFO filling
//  749            operation.
//  750            Synopsys document: DesignWare Cores USB 2.0 Hi-Speed On-The-Go (OTG)
//  751              "The application has to finish writing one complete packet before
//  752               switching to a different channel/endpoint FIFO. Violating this
//  753               rule results in an error.".*/
//  754         otgp->GAHBCFG &= ~GAHBCFG_GINTMSK;
        LDR      R0,[R6, #+8]
        LSRS     R0,R0,#+1
        LSLS     R0,R0,#+1
        STR      R0,[R6, #+8]
//  755         usbp->txpending &= ~epmask;
        LDR      R0,[R5, #+92]
        BICS     R0,R0,R8
        STR      R0,[R5, #+92]
//  756         chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  757 
//  758         done = otg_txfifo_handler(usbp, ep);
        MOVS     R1,R7
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R5
          CFI FunCall otg_txfifo_handler
        BL       otg_txfifo_handler
        MOV      R9,R0
//  759 
//  760         chSysLock();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  761         otgp->GAHBCFG |= GAHBCFG_GINTMSK;
        LDR      R0,[R6, #+8]
        ORRS     R0,R0,#0x1
        STR      R0,[R6, #+8]
//  762         if (!done)
        UXTB     R9,R9            ;; ZeroExt  R9,R9,#+24,#+24
        CMP      R9,#+0
        BNE.N    ??usb_lld_pump_8
//  763           otgp->DIEPEMPMSK |= epmask;
        LDR      R0,[R6, #+2100]
        ORRS     R0,R8,R0
        STR      R0,[R6, #+2100]
//  764         chSysUnlock();
??usb_lld_pump_8:
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  765       }
//  766     }
??usb_lld_pump_7:
        ADDS     R7,R7,#+1
        B.N      ??usb_lld_pump_3
//  767     chSysLock();
??usb_lld_pump_4:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
        B.N      ??usb_lld_pump_0
          CFI EndBlock cfiBlock20
//  768   }
//  769   // we are never here but GCC cannot figure this out
//  770   return 0;
//  771 }
//  772 
//  773 #if STM32_USB_USE_OTG1 || defined(__DOXYGEN__)
//  774 #if !defined(STM32_OTG1_HANDLER)
//  775 #error "STM32_OTG1_HANDLER not defined"
//  776 #endif
//  777 /**
//  778  * @brief   OTG1 interrupt handler.
//  779  *
//  780  * @isr
//  781  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock21 Using cfiCommon0
          CFI Function Vector14C
        THUMB
//  782 CH_IRQ_HANDLER(STM32_OTG1_HANDLER) {
Vector14C:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  783 
//  784   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//  785 
//  786   usb_lld_serve_interrupt(&USBD1);
        LDR.W    R0,??DataTable6_3
          CFI FunCall usb_lld_serve_interrupt
        BL       usb_lld_serve_interrupt
//  787 
//  788   CH_IRQ_EPILOGUE();
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//  789 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock21
//  790 #endif
//  791 
//  792 #if STM32_USB_USE_OTG2 || defined(__DOXYGEN__)
//  793 #if !defined(STM32_OTG2_HANDLER)
//  794 #error "STM32_OTG2_HANDLER not defined"
//  795 #endif
//  796 /**
//  797  * @brief   OTG2 interrupt handler.
//  798  *
//  799  * @isr
//  800  */
//  801 CH_IRQ_HANDLER(STM32_OTG2_HANDLER) {
//  802 
//  803   CH_IRQ_PROLOGUE();
//  804 
//  805   usb_lld_serve_interrupt(&USBD2);
//  806 
//  807   CH_IRQ_EPILOGUE();
//  808 }
//  809 #endif
//  810 
//  811 /*===========================================================================*/
//  812 /* Driver exported functions.                                                */
//  813 /*===========================================================================*/
//  814 
//  815 /**
//  816  * @brief   Low level USB driver initialization.
//  817  *
//  818  * @notapi
//  819  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock22 Using cfiCommon0
          CFI Function usb_lld_init
        THUMB
//  820 void usb_lld_init(void) {
usb_lld_init:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//  821 
//  822   /* Driver initialization.*/
//  823 #if STM32_USB_USE_OTG1
//  824   usbObjectInit(&USBD1);
        LDR.W    R0,??DataTable6_3
          CFI FunCall usbObjectInit
        BL       usbObjectInit
//  825   USBD1.thd_ptr   = NULL;
        LDR.W    R0,??DataTable6_3
        MOVS     R1,#+0
        STR      R1,[R0, #+96]
//  826   USBD1.thd_wait  = NULL;
        LDR.W    R0,??DataTable6_3
        MOVS     R1,#+0
        STR      R1,[R0, #+100]
//  827   USBD1.otg       = OTG_FS;
        LDR.W    R0,??DataTable6_3
        MOVS     R1,#+1342177280
        STR      R1,[R0, #+80]
//  828   USBD1.otgparams = &fsparams;
        LDR.N    R0,??DataTable6_3
        LDR.N    R1,??DataTable6_4
        STR      R1,[R0, #+84]
//  829 
//  830   /* Filling the thread working area here because the function
//  831      @p chThdCreateI() does not do it.*/
//  832 #if CH_DBG_FILL_THREADS
//  833   {
//  834     void *wsp = USBD1.wa_pump;
        LDR.N    R4,??DataTable6_5
//  835     _thread_memfill((uint8_t *)wsp,
//  836                     (uint8_t *)wsp + sizeof(Thread),
//  837                     CH_THREAD_FILL_VALUE);
        MOVS     R2,#+255
        ADDS     R1,R4,#+76
        MOVS     R0,R4
          CFI FunCall _thread_memfill
        BL       _thread_memfill
//  838     _thread_memfill((uint8_t *)wsp + sizeof(Thread),
//  839                     (uint8_t *)wsp + sizeof(USBD1.wa_pump),
//  840                     CH_STACK_FILL_VALUE);
        MOVS     R2,#+85
        ADDS     R1,R4,#+1336
        ADDS     R0,R4,#+76
          CFI FunCall _thread_memfill
        BL       _thread_memfill
//  841   }
//  842 #endif
//  843 #endif
//  844 
//  845 #if STM32_USB_USE_OTG2
//  846   usbObjectInit(&USBD2);
//  847   USBD2.thd_ptr   = NULL;
//  848   USBD2.thd_wait  = NULL;
//  849   USBD2.otg       = OTG_HS;
//  850   USBD2.otgparams = &hsparams;
//  851 
//  852   /* Filling the thread working area here because the function
//  853      @p chThdCreateI() does not do it.*/
//  854 #if CH_DBG_FILL_THREADS
//  855   {
//  856     void *wsp = USBD2.wa_pump;
//  857     _thread_memfill((uint8_t *)wsp,
//  858                     (uint8_t *)wsp + sizeof(Thread),
//  859                     CH_THREAD_FILL_VALUE);
//  860     _thread_memfill((uint8_t *)wsp + sizeof(Thread),
//  861                     (uint8_t *)wsp + sizeof(USBD2.wa_pump),
//  862                     CH_STACK_FILL_VALUE);
//  863   }
//  864 #endif
//  865 #endif
//  866 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock22
//  867 
//  868 /**
//  869  * @brief   Configures and activates the USB peripheral.
//  870  * @note    Starting the OTG cell can be a slow operation carried out with
//  871  *          interrupts disabled, perform it before starting time-critical
//  872  *          operations.
//  873  *
//  874  * @param[in] usbp      pointer to the @p USBDriver object
//  875  *
//  876  * @notapi
//  877  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock23 Using cfiCommon0
          CFI Function usb_lld_start
        THUMB
//  878 void usb_lld_start(USBDriver *usbp) {
usb_lld_start:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  879   stm32_otg_t *otgp = usbp->otg;
        LDR      R5,[R4, #+80]
//  880 
//  881   if (usbp->state == USB_STOP) {
        LDRB     R0,[R4, #+0]
        CMP      R0,#+1
        BNE.N    ??usb_lld_start_0
//  882     /* Clock activation.*/
//  883 #if STM32_USB_USE_OTG1
//  884     if (&USBD1 == usbp) {
        LDR.N    R0,??DataTable6_3
        CMP      R0,R4
        BNE.N    ??usb_lld_start_1
//  885       /* OTG FS clock enable and reset.*/
//  886       rccEnableOTG_FS(FALSE);
        LDR.N    R0,??DataTable6_6  ;; 0x40023834
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x80
        LDR.N    R1,??DataTable6_6  ;; 0x40023834
        STR      R0,[R1, #+0]
//  887       rccResetOTG_FS();
        LDR.N    R0,??DataTable6_7  ;; 0x40023814
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x80
        LDR.N    R1,??DataTable6_7  ;; 0x40023814
        STR      R0,[R1, #+0]
        LDR.N    R0,??DataTable6_7  ;; 0x40023814
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//  888 
//  889       /* Enables IRQ vector.*/
//  890       nvicEnableVector(STM32_OTG1_NUMBER,
//  891                        CORTEX_PRIORITY_MASK(STM32_USB_OTG1_IRQ_PRIORITY));
        MOVS     R1,#+224
        MOVS     R0,#+67
          CFI FunCall nvicEnableVector
        BL       nvicEnableVector
//  892     }
//  893 #endif
//  894 #if STM32_USB_USE_OTG2
//  895     if (&USBD2 == usbp) {
//  896       /* OTG HS clock enable and reset.*/
//  897       rccEnableOTG_HS(FALSE);
//  898       rccResetOTG_HS();
//  899 
//  900       /* Workaround for the problem described here:
//  901          http://forum.chibios.org/phpbb/viewtopic.php?f=16&t=1798 */
//  902       rccDisableOTG_HSULPI(TRUE);
//  903 
//  904       /* Enables IRQ vector.*/
//  905       nvicEnableVector(STM32_OTG2_NUMBER,
//  906                        CORTEX_PRIORITY_MASK(STM32_USB_OTG2_IRQ_PRIORITY));
//  907     }
//  908 #endif
//  909 
//  910     /* Creates the data pump threads in a suspended state. Note, it is
//  911        created only once, the first time @p usbStart() is invoked.*/
//  912     usbp->txpending = 0;
??usb_lld_start_1:
        MOVS     R0,#+0
        STR      R0,[R4, #+92]
//  913     if (usbp->thd_ptr == NULL)
        LDR      R0,[R4, #+96]
        CMP      R0,#+0
        BNE.N    ??usb_lld_start_2
//  914       usbp->thd_ptr = usbp->thd_wait = chThdCreateI(usbp->wa_pump,
//  915                                                     sizeof usbp->wa_pump,
//  916                                                     STM32_USB_OTG_THREAD_PRIO,
//  917                                                     usb_lld_pump,
//  918                                                     usbp);
        STR      R4,[SP, #+0]
        LDR.N    R3,??DataTable6_8
        MOVS     R2,#+2
        MOV      R1,#+1336
        ADDS     R0,R4,#+104
          CFI FunCall chThdCreateI
        BL       chThdCreateI
        STR      R0,[R4, #+100]
        STR      R0,[R4, #+96]
//  919 
//  920     /* - Forced device mode.
//  921        - USB turn-around time = TRDT_VALUE.
//  922        - Full Speed 1.1 PHY.*/
//  923     otgp->GUSBCFG = GUSBCFG_FDMOD | GUSBCFG_TRDT(TRDT_VALUE) | GUSBCFG_PHYSEL;
??usb_lld_start_2:
        LDR.N    R0,??DataTable6_9  ;; 0x40001440
        STR      R0,[R5, #+12]
//  924 
//  925     /* 48MHz 1.1 PHY.*/
//  926     otgp->DCFG = 0x02200000 | DCFG_DSPD_FS11;
        LDR.N    R0,??DataTable6_10  ;; 0x2200003
        STR      R0,[R5, #+2048]
//  927 
//  928     /* PHY enabled.*/
//  929     otgp->PCGCCTL = 0;
        MOVS     R0,#+0
        STR      R0,[R5, #+3584]
//  930 
//  931     /* Internal FS PHY activation.*/
//  932 #if defined(BOARD_OTG_NOVBUSSENS)
//  933     otgp->GCCFG = GCCFG_NOVBUSSENS | GCCFG_VBUSASEN | GCCFG_VBUSBSEN |
//  934                   GCCFG_PWRDWN;
//  935 #else
//  936     otgp->GCCFG = GCCFG_VBUSASEN | GCCFG_VBUSBSEN | GCCFG_PWRDWN;
        MOVS     R0,#+851968
        STR      R0,[R5, #+56]
//  937 #endif
//  938 
//  939     /* Soft core reset.*/
//  940     otg_core_reset(usbp);
        MOVS     R0,R4
          CFI FunCall otg_core_reset
        BL       otg_core_reset
//  941 
//  942     /* Interrupts on TXFIFOs half empty.*/
//  943     otgp->GAHBCFG = 0;
        MOVS     R0,#+0
        STR      R0,[R5, #+8]
//  944 
//  945     /* Endpoints re-initialization.*/
//  946     otg_disable_ep(usbp);
        MOVS     R0,R4
          CFI FunCall otg_disable_ep
        BL       otg_disable_ep
//  947 
//  948     /* Clear all pending Device Interrupts, only the USB Reset interrupt
//  949        is required initially.*/
//  950     otgp->DIEPMSK  = 0;
        MOVS     R0,#+0
        STR      R0,[R5, #+2064]
//  951     otgp->DOEPMSK  = 0;
        MOVS     R0,#+0
        STR      R0,[R5, #+2068]
//  952     otgp->DAINTMSK = 0;
        MOVS     R0,#+0
        STR      R0,[R5, #+2076]
//  953     if (usbp->config->sof_cb == NULL)
        LDR      R0,[R4, #+4]
        LDR      R0,[R0, #+12]
        CMP      R0,#+0
        BNE.N    ??usb_lld_start_3
//  954       otgp->GINTMSK  = GINTMSK_ENUMDNEM | GINTMSK_USBRSTM /*| GINTMSK_USBSUSPM |
//  955                        GINTMSK_ESUSPM  |*/;
        MOV      R0,#+12288
        STR      R0,[R5, #+24]
        B.N      ??usb_lld_start_4
//  956     else
//  957       otgp->GINTMSK  = GINTMSK_ENUMDNEM | GINTMSK_USBRSTM /*| GINTMSK_USBSUSPM |
//  958                        GINTMSK_ESUSPM */ | GINTMSK_SOFM;
??usb_lld_start_3:
        MOVW     R0,#+12296
        STR      R0,[R5, #+24]
//  959     otgp->GINTSTS  = 0xFFFFFFFF;         /* Clears all pending IRQs, if any. */
??usb_lld_start_4:
        MOVS     R0,#-1
        STR      R0,[R5, #+20]
//  960 
//  961     /* Global interrupts enable.*/
//  962     otgp->GAHBCFG |= GAHBCFG_GINTMSK;
        LDR      R0,[R5, #+8]
        ORRS     R0,R0,#0x1
        STR      R0,[R5, #+8]
//  963   }
//  964 }
??usb_lld_start_0:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock23
//  965 
//  966 /**
//  967  * @brief   Deactivates the USB peripheral.
//  968  *
//  969  * @param[in] usbp      pointer to the @p USBDriver object
//  970  *
//  971  * @notapi
//  972  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock24 Using cfiCommon0
          CFI Function usb_lld_stop
        THUMB
//  973 void usb_lld_stop(USBDriver *usbp) {
usb_lld_stop:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  974   stm32_otg_t *otgp = usbp->otg;
        LDR      R5,[R4, #+80]
//  975 
//  976   /* If in ready state then disables the USB clock.*/
//  977   if (usbp->state != USB_STOP) {
        LDRB     R0,[R4, #+0]
        CMP      R0,#+1
        BEQ.N    ??usb_lld_stop_0
//  978 
//  979     /* Disabling all endpoints in case the driver has been stopped while
//  980        active.*/
//  981     otg_disable_ep(usbp);
        MOVS     R0,R4
          CFI FunCall otg_disable_ep
        BL       otg_disable_ep
//  982 
//  983     usbp->txpending = 0;
        MOVS     R0,#+0
        STR      R0,[R4, #+92]
//  984 
//  985     otgp->DAINTMSK   = 0;
        MOVS     R0,#+0
        STR      R0,[R5, #+2076]
//  986     otgp->GAHBCFG    = 0;
        MOVS     R0,#+0
        STR      R0,[R5, #+8]
//  987     otgp->GCCFG      = 0;
        MOVS     R0,#+0
        STR      R0,[R5, #+56]
//  988 
//  989 #if STM32_USB_USE_USB1
//  990     if (&USBD1 == usbp) {
//  991       nvicDisableVector(STM32_OTG1_NUMBER);
//  992       rccDisableOTG1(FALSE);
//  993     }
//  994 #endif
//  995 
//  996 #if STM32_USB_USE_USB2
//  997     if (&USBD2 == usbp) {
//  998       nvicDisableVector(STM32_OTG2_NUMBER);
//  999       rccDisableOTG2(FALSE);
// 1000     }
// 1001 #endif
// 1002   }
// 1003 }
??usb_lld_stop_0:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock24
// 1004 
// 1005 /**
// 1006  * @brief   USB low level reset routine.
// 1007  *
// 1008  * @param[in] usbp      pointer to the @p USBDriver object
// 1009  *
// 1010  * @notapi
// 1011  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock25 Using cfiCommon0
          CFI Function usb_lld_reset
        THUMB
// 1012 void usb_lld_reset(USBDriver *usbp) {
usb_lld_reset:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
// 1013   unsigned i;
// 1014   stm32_otg_t *otgp = usbp->otg;
        LDR      R6,[R4, #+80]
// 1015 
// 1016   /* Flush the Tx FIFO.*/
// 1017   otg_txfifo_flush(usbp, 0);
        MOVS     R1,#+0
        MOVS     R0,R4
          CFI FunCall otg_txfifo_flush
        BL       otg_txfifo_flush
// 1018 
// 1019   /* All endpoints in NAK mode, interrupts cleared.*/
// 1020   for (i = 0; i <= usbp->otgparams->num_endpoints; i++) {
        MOVS     R0,#+0
        MOVS     R5,R0
??usb_lld_reset_0:
        LDR      R0,[R4, #+84]
        LDR      R0,[R0, #+8]
        CMP      R0,R5
        BCC.N    ??usb_lld_reset_1
// 1021     otgp->ie[i].DIEPCTL = DIEPCTL_SNAK;
        ADDS     R0,R6,R5, LSL #+5
        MOVS     R1,#+134217728
        STR      R1,[R0, #+2304]
// 1022     otgp->oe[i].DOEPCTL = DOEPCTL_SNAK;
        ADDS     R0,R6,R5, LSL #+5
        MOVS     R1,#+134217728
        STR      R1,[R0, #+2816]
// 1023     otgp->ie[i].DIEPINT = 0xFF;
        ADDS     R0,R6,R5, LSL #+5
        MOVS     R1,#+255
        STR      R1,[R0, #+2312]
// 1024     otgp->oe[i].DOEPINT = 0xFF;
        ADDS     R0,R6,R5, LSL #+5
        MOVS     R1,#+255
        STR      R1,[R0, #+2824]
// 1025   }
        ADDS     R5,R5,#+1
        B.N      ??usb_lld_reset_0
// 1026 
// 1027   /* Endpoint interrupts all disabled and cleared.*/
// 1028   otgp->DAINT = 0xFFFFFFFF;
??usb_lld_reset_1:
        MOVS     R0,#-1
        STR      R0,[R6, #+2072]
// 1029   otgp->DAINTMSK = DAINTMSK_OEPM(0) | DAINTMSK_IEPM(0);
        MOVS     R0,#+65537
        STR      R0,[R6, #+2076]
// 1030 
// 1031   /* Resets the FIFO memory allocator.*/
// 1032   otg_ram_reset(usbp);
        MOVS     R0,R4
          CFI FunCall otg_ram_reset
        BL       otg_ram_reset
// 1033 
// 1034   /* Receive FIFO size initialization, the address is always zero.*/
// 1035   otgp->GRXFSIZ = usbp->otgparams->rx_fifo_size;
        LDR      R0,[R4, #+84]
        LDR      R0,[R0, #+0]
        STR      R0,[R6, #+36]
// 1036   otg_rxfifo_flush(usbp);
        MOVS     R0,R4
          CFI FunCall otg_rxfifo_flush
        BL       otg_rxfifo_flush
// 1037 
// 1038   /* Resets the device address to zero.*/
// 1039   otgp->DCFG = (otgp->DCFG & ~DCFG_DAD_MASK) | DCFG_DAD(0);
        LDR      R0,[R6, #+2048]
        BICS     R0,R0,#0x7F0
        STR      R0,[R6, #+2048]
// 1040 
// 1041   /* Enables also EP-related interrupt sources.*/
// 1042   otgp->GINTMSK  |= GINTMSK_RXFLVLM | GINTMSK_OEPM  | GINTMSK_IEPM;
        LDR      R0,[R6, #+24]
        ORR      R0,R0,#0xC0000
        ORRS     R0,R0,#0x10
        STR      R0,[R6, #+24]
// 1043   otgp->DIEPMSK   = DIEPMSK_TOCM    | DIEPMSK_XFRCM;
        MOVS     R0,#+9
        STR      R0,[R6, #+2064]
// 1044   otgp->DOEPMSK   = DOEPMSK_STUPM   | DOEPMSK_XFRCM;
        MOVS     R0,#+9
        STR      R0,[R6, #+2068]
// 1045 
// 1046   /* EP0 initialization, it is a special case.*/
// 1047   usbp->epc[0] = &ep0config;
        LDR.N    R0,??DataTable6_11
        STR      R0,[R4, #+12]
// 1048   otgp->oe[0].DOEPTSIZ = 0;
        MOVS     R0,#+0
        STR      R0,[R6, #+2832]
// 1049   otgp->oe[0].DOEPCTL = DOEPCTL_SD0PID | DOEPCTL_USBAEP | DOEPCTL_EPTYP_CTRL |
// 1050                         DOEPCTL_MPSIZ(ep0config.out_maxsize);
        LDR.N    R0,??DataTable6_11
        LDRH     R0,[R0, #+18]
        ORR      R0,R0,#0x10000000
        ORRS     R0,R0,#0x8000
        STR      R0,[R6, #+2816]
// 1051   otgp->ie[0].DIEPTSIZ = 0;
        MOVS     R0,#+0
        STR      R0,[R6, #+2320]
// 1052   otgp->ie[0].DIEPCTL = DIEPCTL_SD0PID | DIEPCTL_USBAEP | DIEPCTL_EPTYP_CTRL |
// 1053                         DIEPCTL_TXFNUM(0) | DIEPCTL_MPSIZ(ep0config.in_maxsize);
        LDR.N    R0,??DataTable6_11
        LDRH     R0,[R0, #+16]
        ORR      R0,R0,#0x10000000
        ORRS     R0,R0,#0x8000
        STR      R0,[R6, #+2304]
// 1054   otgp->DIEPTXF0 = DIEPTXF_INEPTXFD(ep0config.in_maxsize / 4) |
// 1055                    DIEPTXF_INEPTXSA(otg_ram_alloc(usbp,
// 1056                                                   ep0config.in_maxsize / 4));
        LDR.N    R0,??DataTable6_11
        LDRH     R0,[R0, #+16]
        MOVS     R1,#+4
        SDIV     R1,R0,R1
        MOVS     R0,R4
          CFI FunCall otg_ram_alloc
        BL       otg_ram_alloc
        LDR.N    R1,??DataTable6_11
        LDRH     R1,[R1, #+16]
        MOVS     R2,#+4
        SDIV     R1,R1,R2
        ORRS     R0,R0,R1, LSL #+16
        STR      R0,[R6, #+40]
// 1057 }
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock25
// 1058 
// 1059 /**
// 1060  * @brief   Sets the USB address.
// 1061  *
// 1062  * @param[in] usbp      pointer to the @p USBDriver object
// 1063  *
// 1064  * @notapi
// 1065  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock26 Using cfiCommon0
          CFI Function usb_lld_set_address
          CFI NoCalls
        THUMB
// 1066 void usb_lld_set_address(USBDriver *usbp) {
// 1067   stm32_otg_t *otgp = usbp->otg;
usb_lld_set_address:
        LDR      R1,[R0, #+80]
// 1068 
// 1069   otgp->DCFG = (otgp->DCFG & ~DCFG_DAD_MASK) | DCFG_DAD(usbp->address);
        LDR      R2,[R1, #+2048]
        BICS     R2,R2,#0x7F0
        LDRB     R3,[R0, #+78]
        ORRS     R2,R2,R3, LSL #+4
        STR      R2,[R1, #+2048]
// 1070 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock26
// 1071 
// 1072 /**
// 1073  * @brief   Enables an endpoint.
// 1074  *
// 1075  * @param[in] usbp      pointer to the @p USBDriver object
// 1076  * @param[in] ep        endpoint number
// 1077  *
// 1078  * @notapi
// 1079  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock27 Using cfiCommon0
          CFI Function usb_lld_init_endpoint
        THUMB
// 1080 void usb_lld_init_endpoint(USBDriver *usbp, usbep_t ep) {
usb_lld_init_endpoint:
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
// 1081   uint32_t ctl, fsize;
// 1082   stm32_otg_t *otgp = usbp->otg;
        LDR      R8,[R4, #+80]
// 1083 
// 1084   /* IN and OUT common parameters.*/
// 1085   switch (usbp->epc[ep]->ep_mode & USB_EP_MODE_TYPE) {
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R0,R4,R5, LSL #+2
        LDR      R0,[R0, #+12]
        LDRB     R0,[R0, #+0]
        ANDS     R0,R0,#0x3
        CMP      R0,#+0
        BEQ.N    ??usb_lld_init_endpoint_0
        CMP      R0,#+2
        BEQ.N    ??usb_lld_init_endpoint_1
        BCC.N    ??usb_lld_init_endpoint_2
        CMP      R0,#+3
        BEQ.N    ??usb_lld_init_endpoint_3
        B.N      ??usb_lld_init_endpoint_4
// 1086   case USB_EP_MODE_TYPE_CTRL:
// 1087     ctl = DIEPCTL_SD0PID | DIEPCTL_USBAEP | DIEPCTL_EPTYP_CTRL;
??usb_lld_init_endpoint_0:
        LDR.N    R0,??DataTable6_12  ;; 0x10008000
        MOVS     R6,R0
// 1088     break;
        B.N      ??usb_lld_init_endpoint_5
// 1089   case USB_EP_MODE_TYPE_ISOC:
// 1090     ctl = DIEPCTL_SD0PID | DIEPCTL_USBAEP | DIEPCTL_EPTYP_ISO;
??usb_lld_init_endpoint_2:
        LDR.N    R0,??DataTable6_13  ;; 0x10048000
        MOVS     R6,R0
// 1091     break;
        B.N      ??usb_lld_init_endpoint_5
// 1092   case USB_EP_MODE_TYPE_BULK:
// 1093     ctl = DIEPCTL_SD0PID | DIEPCTL_USBAEP | DIEPCTL_EPTYP_BULK;
??usb_lld_init_endpoint_1:
        LDR.N    R0,??DataTable6_14  ;; 0x10088000
        MOVS     R6,R0
// 1094     break;
        B.N      ??usb_lld_init_endpoint_5
// 1095   case USB_EP_MODE_TYPE_INTR:
// 1096     ctl = DIEPCTL_SD0PID | DIEPCTL_USBAEP | DIEPCTL_EPTYP_INTR;
??usb_lld_init_endpoint_3:
        LDR.N    R0,??DataTable6_15  ;; 0x100c8000
        MOVS     R6,R0
// 1097     break;
        B.N      ??usb_lld_init_endpoint_5
// 1098   default:
// 1099     return;
??usb_lld_init_endpoint_4:
        B.N      ??usb_lld_init_endpoint_6
// 1100   }
// 1101 
// 1102   /* OUT endpoint activation or deactivation.*/
// 1103   otgp->oe[ep].DOEPTSIZ = 0;
??usb_lld_init_endpoint_5:
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R0,R8,R5, LSL #+5
        MOVS     R1,#+0
        STR      R1,[R0, #+2832]
// 1104   if (usbp->epc[ep]->out_cb != NULL) {
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R0,R4,R5, LSL #+2
        LDR      R0,[R0, #+12]
        LDR      R0,[R0, #+12]
        CMP      R0,#+0
        BEQ.N    ??usb_lld_init_endpoint_7
// 1105     otgp->oe[ep].DOEPCTL = ctl | DOEPCTL_MPSIZ(usbp->epc[ep]->out_maxsize);
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R0,R4,R5, LSL #+2
        LDR      R0,[R0, #+12]
        LDRH     R0,[R0, #+18]
        ORRS     R0,R0,R6
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R1,R8,R5, LSL #+5
        STR      R0,[R1, #+2816]
// 1106     otgp->DAINTMSK |= DAINTMSK_OEPM(ep);
        LDR      R0,[R8, #+2076]
        MOVS     R1,#+1
        ADDS     R2,R5,#+16
        LSLS     R1,R1,R2
        ORRS     R0,R1,R0
        STR      R0,[R8, #+2076]
        B.N      ??usb_lld_init_endpoint_8
// 1107   }
// 1108   else {
// 1109     otgp->oe[ep].DOEPCTL &= ~DOEPCTL_USBAEP;
??usb_lld_init_endpoint_7:
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R0,R8,R5, LSL #+5
        LDR      R0,[R0, #+2816]
        BICS     R0,R0,#0x8000
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R1,R8,R5, LSL #+5
        STR      R0,[R1, #+2816]
// 1110     otgp->DAINTMSK &= ~DAINTMSK_OEPM(ep);
        LDR      R0,[R8, #+2076]
        MOVS     R1,#+1
        ADDS     R2,R5,#+16
        LSLS     R1,R1,R2
        BICS     R0,R0,R1
        STR      R0,[R8, #+2076]
// 1111   }
// 1112 
// 1113   /* IN endpoint activation or deactivation.*/
// 1114   otgp->ie[ep].DIEPTSIZ = 0;
??usb_lld_init_endpoint_8:
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R0,R8,R5, LSL #+5
        MOVS     R1,#+0
        STR      R1,[R0, #+2320]
// 1115   if (usbp->epc[ep]->in_cb != NULL) {
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R0,R4,R5, LSL #+2
        LDR      R0,[R0, #+12]
        LDR      R0,[R0, #+8]
        CMP      R0,#+0
        BEQ.N    ??usb_lld_init_endpoint_9
// 1116     /* FIFO allocation for the IN endpoint.*/
// 1117     fsize = usbp->epc[ep]->in_maxsize / 4;
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R0,R4,R5, LSL #+2
        LDR      R0,[R0, #+12]
        LDRH     R0,[R0, #+16]
        MOVS     R1,#+4
        SDIV     R0,R0,R1
        MOVS     R7,R0
// 1118     if (usbp->epc[ep]->in_multiplier > 1)
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R0,R4,R5, LSL #+2
        LDR      R0,[R0, #+12]
        LDRH     R0,[R0, #+28]
        CMP      R0,#+2
        BLT.N    ??usb_lld_init_endpoint_10
// 1119       fsize *= usbp->epc[ep]->in_multiplier;
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R0,R4,R5, LSL #+2
        LDR      R0,[R0, #+12]
        LDRH     R0,[R0, #+28]
        MULS     R7,R0,R7
// 1120     otgp->DIEPTXF[ep - 1] = DIEPTXF_INEPTXFD(fsize) |
// 1121                             DIEPTXF_INEPTXSA(otg_ram_alloc(usbp, fsize));
??usb_lld_init_endpoint_10:
        MOVS     R1,R7
        MOVS     R0,R4
          CFI FunCall otg_ram_alloc
        BL       otg_ram_alloc
        ORRS     R0,R0,R7, LSL #+16
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R1,R8,R5, LSL #+2
        STR      R0,[R1, #+256]
// 1122     otg_txfifo_flush(usbp, ep);
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall otg_txfifo_flush
        BL       otg_txfifo_flush
// 1123 
// 1124     otgp->ie[ep].DIEPCTL = ctl |
// 1125                            DIEPCTL_TXFNUM(ep) |
// 1126                            DIEPCTL_MPSIZ(usbp->epc[ep]->in_maxsize);
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ORRS     R0,R6,R5, LSL #+22
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R1,R4,R5, LSL #+2
        LDR      R1,[R1, #+12]
        LDRH     R1,[R1, #+16]
        ORRS     R0,R1,R0
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R1,R8,R5, LSL #+5
        STR      R0,[R1, #+2304]
// 1127     otgp->DAINTMSK |= DAINTMSK_IEPM(ep);
        LDR      R0,[R8, #+2076]
        MOVS     R1,#+1
        LSLS     R1,R1,R5
        ORRS     R0,R1,R0
        STR      R0,[R8, #+2076]
        B.N      ??usb_lld_init_endpoint_11
// 1128   }
// 1129   else {
// 1130     otgp->DIEPTXF[ep - 1] = 0x02000400; /* Reset value.*/
??usb_lld_init_endpoint_9:
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R0,R8,R5, LSL #+2
        LDR.N    R1,??DataTable6_16  ;; 0x2000400
        STR      R1,[R0, #+256]
// 1131     otg_txfifo_flush(usbp, ep);
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall otg_txfifo_flush
        BL       otg_txfifo_flush
// 1132     otgp->ie[ep].DIEPCTL &= ~DIEPCTL_USBAEP;
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R0,R8,R5, LSL #+5
        LDR      R0,[R0, #+2304]
        BICS     R0,R0,#0x8000
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R1,R8,R5, LSL #+5
        STR      R0,[R1, #+2304]
// 1133     otgp->DAINTMSK &= ~DAINTMSK_IEPM(ep);
        LDR      R0,[R8, #+2076]
        MOVS     R1,#+1
        LSLS     R1,R1,R5
        BICS     R0,R0,R1
        STR      R0,[R8, #+2076]
// 1134   }
// 1135 }
??usb_lld_init_endpoint_11:
??usb_lld_init_endpoint_6:
        POP      {R4-R8,PC}       ;; return
          CFI EndBlock cfiBlock27

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6:
        DC32     `?<Constant "otg_fifo_alloc(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_1:
        DC32     rlist

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_2:
        DC32     `?<Constant "usb_lld_pump">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_3:
        DC32     USBD1

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_4:
        DC32     fsparams

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_5:
        DC32     USBD1+0x68

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_6:
        DC32     0x40023834

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_7:
        DC32     0x40023814

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_8:
        DC32     usb_lld_pump

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_9:
        DC32     0x40001440

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_10:
        DC32     0x2200003

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_11:
        DC32     ep0config

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_12:
        DC32     0x10008000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_13:
        DC32     0x10048000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_14:
        DC32     0x10088000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_15:
        DC32     0x100c8000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_16:
        DC32     0x2000400
// 1136 
// 1137 /**
// 1138  * @brief   Disables all the active endpoints except the endpoint zero.
// 1139  *
// 1140  * @param[in] usbp      pointer to the @p USBDriver object
// 1141  *
// 1142  * @notapi
// 1143  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock28 Using cfiCommon0
          CFI Function usb_lld_disable_endpoints
        THUMB
// 1144 void usb_lld_disable_endpoints(USBDriver *usbp) {
usb_lld_disable_endpoints:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
// 1145 
// 1146   /* Resets the FIFO memory allocator.*/
// 1147   otg_ram_reset(usbp);
        MOVS     R0,R4
          CFI FunCall otg_ram_reset
        BL       otg_ram_reset
// 1148 
// 1149   /* Disabling all endpoints.*/
// 1150   otg_disable_ep(usbp);
        MOVS     R0,R4
          CFI FunCall otg_disable_ep
        BL       otg_disable_ep
// 1151 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock28
// 1152 
// 1153 /**
// 1154  * @brief   Returns the status of an OUT endpoint.
// 1155  *
// 1156  * @param[in] usbp      pointer to the @p USBDriver object
// 1157  * @param[in] ep        endpoint number
// 1158  * @return              The endpoint status.
// 1159  * @retval EP_STATUS_DISABLED The endpoint is not active.
// 1160  * @retval EP_STATUS_STALLED  The endpoint is stalled.
// 1161  * @retval EP_STATUS_ACTIVE   The endpoint is active.
// 1162  *
// 1163  * @notapi
// 1164  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock29 Using cfiCommon0
          CFI Function usb_lld_get_status_out
          CFI NoCalls
        THUMB
// 1165 usbepstatus_t usb_lld_get_status_out(USBDriver *usbp, usbep_t ep) {
usb_lld_get_status_out:
        MOVS     R2,R0
// 1166   uint32_t ctl;
// 1167 
// 1168   (void)usbp;
// 1169 
// 1170   ctl = usbp->otg->oe[ep].DOEPCTL;
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        LDR      R0,[R2, #+80]
        ADDS     R0,R0,R1, LSL #+5
        LDR      R0,[R0, #+2816]
        MOVS     R3,R0
// 1171   if (!(ctl & DOEPCTL_USBAEP))
        LSLS     R0,R3,#+16
        BMI.N    ??usb_lld_get_status_out_0
// 1172     return EP_STATUS_DISABLED;
        MOVS     R0,#+0
        B.N      ??usb_lld_get_status_out_1
// 1173   if (ctl & DOEPCTL_STALL)
??usb_lld_get_status_out_0:
        LSLS     R0,R3,#+10
        BPL.N    ??usb_lld_get_status_out_2
// 1174     return EP_STATUS_STALLED;
        MOVS     R0,#+1
        B.N      ??usb_lld_get_status_out_1
// 1175   return EP_STATUS_ACTIVE;
??usb_lld_get_status_out_2:
        MOVS     R0,#+2
??usb_lld_get_status_out_1:
        BX       LR               ;; return
          CFI EndBlock cfiBlock29
// 1176 }
// 1177 
// 1178 /**
// 1179  * @brief   Returns the status of an IN endpoint.
// 1180  *
// 1181  * @param[in] usbp      pointer to the @p USBDriver object
// 1182  * @param[in] ep        endpoint number
// 1183  * @return              The endpoint status.
// 1184  * @retval EP_STATUS_DISABLED The endpoint is not active.
// 1185  * @retval EP_STATUS_STALLED  The endpoint is stalled.
// 1186  * @retval EP_STATUS_ACTIVE   The endpoint is active.
// 1187  *
// 1188  * @notapi
// 1189  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock30 Using cfiCommon0
          CFI Function usb_lld_get_status_in
          CFI NoCalls
        THUMB
// 1190 usbepstatus_t usb_lld_get_status_in(USBDriver *usbp, usbep_t ep) {
usb_lld_get_status_in:
        MOVS     R2,R0
// 1191   uint32_t ctl;
// 1192 
// 1193   (void)usbp;
// 1194 
// 1195   ctl = usbp->otg->ie[ep].DIEPCTL;
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        LDR      R0,[R2, #+80]
        ADDS     R0,R0,R1, LSL #+5
        LDR      R0,[R0, #+2304]
        MOVS     R3,R0
// 1196   if (!(ctl & DIEPCTL_USBAEP))
        LSLS     R0,R3,#+16
        BMI.N    ??usb_lld_get_status_in_0
// 1197     return EP_STATUS_DISABLED;
        MOVS     R0,#+0
        B.N      ??usb_lld_get_status_in_1
// 1198   if (ctl & DIEPCTL_STALL)
??usb_lld_get_status_in_0:
        LSLS     R0,R3,#+10
        BPL.N    ??usb_lld_get_status_in_2
// 1199     return EP_STATUS_STALLED;
        MOVS     R0,#+1
        B.N      ??usb_lld_get_status_in_1
// 1200   return EP_STATUS_ACTIVE;
??usb_lld_get_status_in_2:
        MOVS     R0,#+2
??usb_lld_get_status_in_1:
        BX       LR               ;; return
          CFI EndBlock cfiBlock30
// 1201 }
// 1202 
// 1203 /**
// 1204  * @brief   Reads a setup packet from the dedicated packet buffer.
// 1205  * @details This function must be invoked in the context of the @p setup_cb
// 1206  *          callback in order to read the received setup packet.
// 1207  * @pre     In order to use this function the endpoint must have been
// 1208  *          initialized as a control endpoint.
// 1209  * @post    The endpoint is ready to accept another packet.
// 1210  *
// 1211  * @param[in] usbp      pointer to the @p USBDriver object
// 1212  * @param[in] ep        endpoint number
// 1213  * @param[out] buf      buffer where to copy the packet data
// 1214  *
// 1215  * @notapi
// 1216  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock31 Using cfiCommon0
          CFI Function usb_lld_read_setup
        THUMB
// 1217 void usb_lld_read_setup(USBDriver *usbp, usbep_t ep, uint8_t *buf) {
usb_lld_read_setup:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
// 1218 
// 1219   memcpy(buf, usbp->epc[ep]->setup_buf, 8);
        MOVS     R2,#+8
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R0,R4,R5, LSL #+2
        LDR      R0,[R0, #+12]
        LDR      R1,[R0, #+32]
        MOVS     R0,R6
          CFI FunCall memcpy
        BL       memcpy
// 1220 }
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock31
// 1221 
// 1222 /**
// 1223  * @brief   Prepares for a receive operation.
// 1224  *
// 1225  * @param[in] usbp      pointer to the @p USBDriver object
// 1226  * @param[in] ep        endpoint number
// 1227  *
// 1228  * @notapi
// 1229  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock32 Using cfiCommon0
          CFI Function usb_lld_prepare_receive
          CFI NoCalls
        THUMB
// 1230 void usb_lld_prepare_receive(USBDriver *usbp, usbep_t ep) {
usb_lld_prepare_receive:
        PUSH     {R4,R5}
          CFI R5 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
// 1231   uint32_t pcnt;
// 1232   USBOutEndpointState *osp = usbp->epc[ep]->out_state;
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        ADDS     R3,R0,R1, LSL #+2
        LDR      R3,[R3, #+12]
        LDR      R3,[R3, #+24]
// 1233 
// 1234   /* Transfer initialization.*/
// 1235   osp->totsize = osp->rxsize;
        LDR      R4,[R3, #+4]
        STR      R4,[R3, #+16]
// 1236   if ((ep == 0) && (osp->rxsize  > EP0_MAX_OUTSIZE))
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        CMP      R1,#+0
        BNE.N    ??usb_lld_prepare_receive_0
        LDR      R4,[R3, #+4]
        CMP      R4,#+65
        BCC.N    ??usb_lld_prepare_receive_0
// 1237       osp->rxsize = EP0_MAX_OUTSIZE;
        MOVS     R4,#+64
        STR      R4,[R3, #+4]
// 1238 
// 1239   pcnt = (osp->rxsize + usbp->epc[ep]->out_maxsize - 1) /
// 1240          usbp->epc[ep]->out_maxsize;
??usb_lld_prepare_receive_0:
        LDR      R4,[R3, #+4]
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        ADDS     R5,R0,R1, LSL #+2
        LDR      R5,[R5, #+12]
        LDRH     R5,[R5, #+18]
        UXTAH    R4,R4,R5
        SUBS     R4,R4,#+1
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        ADDS     R5,R0,R1, LSL #+2
        LDR      R5,[R5, #+12]
        LDRH     R5,[R5, #+18]
        UDIV     R4,R4,R5
        MOVS     R2,R4
// 1241   usbp->otg->oe[ep].DOEPTSIZ = DOEPTSIZ_STUPCNT(3) | DOEPTSIZ_PKTCNT(pcnt) |
// 1242                                DOEPTSIZ_XFRSIZ(osp->rxsize);
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        LDR      R4,[R0, #+80]
        ADDS     R4,R4,R1, LSL #+5
        LDR      R5,[R3, #+4]
        ORRS     R5,R5,R2, LSL #+19
        ORRS     R5,R5,#0x60000000
        STR      R5,[R4, #+2832]
// 1243 
// 1244 }
        POP      {R4,R5}
          CFI R4 SameValue
          CFI R5 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock32
// 1245 
// 1246 /**
// 1247  * @brief   Prepares for a transmit operation.
// 1248  *
// 1249  * @param[in] usbp      pointer to the @p USBDriver object
// 1250  * @param[in] ep        endpoint number
// 1251  *
// 1252  * @notapi
// 1253  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock33 Using cfiCommon0
          CFI Function usb_lld_prepare_transmit
          CFI NoCalls
        THUMB
// 1254 void usb_lld_prepare_transmit(USBDriver *usbp, usbep_t ep) {
usb_lld_prepare_transmit:
        PUSH     {R4,R5}
          CFI R5 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
// 1255   USBInEndpointState *isp = usbp->epc[ep]->in_state;
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        ADDS     R2,R0,R1, LSL #+2
        LDR      R2,[R2, #+12]
        LDR      R2,[R2, #+20]
// 1256 
// 1257   /* Transfer initialization.*/
// 1258   isp->totsize = isp->txsize;
        LDR      R3,[R2, #+4]
        STR      R3,[R2, #+16]
// 1259   if (isp->txsize == 0) {
        LDR      R3,[R2, #+4]
        CMP      R3,#+0
        BNE.N    ??usb_lld_prepare_transmit_0
// 1260     /* Special case, sending zero size packet.*/
// 1261     usbp->otg->ie[ep].DIEPTSIZ = DIEPTSIZ_PKTCNT(1) | DIEPTSIZ_XFRSIZ(0);
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        LDR      R3,[R0, #+80]
        ADDS     R3,R3,R1, LSL #+5
        MOVS     R4,#+524288
        STR      R4,[R3, #+2320]
        B.N      ??usb_lld_prepare_transmit_1
// 1262   }
// 1263   else {
// 1264     if ((ep == 0) && (isp->txsize  > EP0_MAX_INSIZE))
??usb_lld_prepare_transmit_0:
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        CMP      R1,#+0
        BNE.N    ??usb_lld_prepare_transmit_2
        LDR      R3,[R2, #+4]
        CMP      R3,#+65
        BCC.N    ??usb_lld_prepare_transmit_2
// 1265       isp->txsize = EP0_MAX_INSIZE;
        MOVS     R3,#+64
        STR      R3,[R2, #+4]
// 1266 
// 1267     /* Normal case.*/
// 1268     uint32_t pcnt = (isp->txsize + usbp->epc[ep]->in_maxsize - 1) /
// 1269                     usbp->epc[ep]->in_maxsize;
??usb_lld_prepare_transmit_2:
        LDR      R3,[R2, #+4]
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        ADDS     R4,R0,R1, LSL #+2
        LDR      R4,[R4, #+12]
        LDRH     R4,[R4, #+16]
        UXTAH    R3,R3,R4
        SUBS     R3,R3,#+1
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        ADDS     R4,R0,R1, LSL #+2
        LDR      R4,[R4, #+12]
        LDRH     R4,[R4, #+16]
        UDIV     R3,R3,R4
// 1270     usbp->otg->ie[ep].DIEPTSIZ = DIEPTSIZ_PKTCNT(pcnt) |
// 1271                                  DIEPTSIZ_XFRSIZ(isp->txsize);
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        LDR      R4,[R0, #+80]
        ADDS     R4,R4,R1, LSL #+5
        LDR      R5,[R2, #+4]
        ORRS     R3,R5,R3, LSL #+19
        STR      R3,[R4, #+2320]
// 1272   }
// 1273 }
??usb_lld_prepare_transmit_1:
        POP      {R4,R5}
          CFI R4 SameValue
          CFI R5 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock33
// 1274 
// 1275 /**
// 1276  * @brief   Starts a receive operation on an OUT endpoint.
// 1277  *
// 1278  * @param[in] usbp      pointer to the @p USBDriver object
// 1279  * @param[in] ep        endpoint number
// 1280  *
// 1281  * @notapi
// 1282  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock34 Using cfiCommon0
          CFI Function usb_lld_start_out
          CFI NoCalls
        THUMB
// 1283 void usb_lld_start_out(USBDriver *usbp, usbep_t ep) {
// 1284 
// 1285   usbp->otg->oe[ep].DOEPCTL |= DOEPCTL_CNAK;
usb_lld_start_out:
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        LDR      R2,[R0, #+80]
        ADDS     R2,R2,R1, LSL #+5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        LDR      R3,[R0, #+80]
        ADDS     R3,R3,R1, LSL #+5
        LDR      R3,[R3, #+2816]
        ORRS     R3,R3,#0x4000000
        STR      R3,[R2, #+2816]
// 1286 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock34
// 1287 
// 1288 /**
// 1289  * @brief   Starts a transmit operation on an IN endpoint.
// 1290  *
// 1291  * @param[in] usbp      pointer to the @p USBDriver object
// 1292  * @param[in] ep        endpoint number
// 1293  *
// 1294  * @notapi
// 1295  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock35 Using cfiCommon0
          CFI Function usb_lld_start_in
          CFI NoCalls
        THUMB
// 1296 void usb_lld_start_in(USBDriver *usbp, usbep_t ep) {
// 1297 
// 1298   usbp->otg->ie[ep].DIEPCTL |= DIEPCTL_EPENA | DIEPCTL_CNAK;
usb_lld_start_in:
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        LDR      R2,[R0, #+80]
        ADDS     R2,R2,R1, LSL #+5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        LDR      R3,[R0, #+80]
        ADDS     R3,R3,R1, LSL #+5
        LDR      R3,[R3, #+2304]
        ORRS     R3,R3,#0x84000000
        STR      R3,[R2, #+2304]
// 1299   usbp->otg->DIEPEMPMSK |= DIEPEMPMSK_INEPTXFEM(ep);
        LDR      R2,[R0, #+80]
        LDR      R2,[R2, #+2100]
        MOVS     R3,#+1
        LSLS     R3,R3,R1
        ORRS     R2,R3,R2
        LDR      R3,[R0, #+80]
        STR      R2,[R3, #+2100]
// 1300 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock35
// 1301 
// 1302 /**
// 1303  * @brief   Brings an OUT endpoint in the stalled state.
// 1304  *
// 1305  * @param[in] usbp      pointer to the @p USBDriver object
// 1306  * @param[in] ep        endpoint number
// 1307  *
// 1308  * @notapi
// 1309  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock36 Using cfiCommon0
          CFI Function usb_lld_stall_out
          CFI NoCalls
        THUMB
// 1310 void usb_lld_stall_out(USBDriver *usbp, usbep_t ep) {
// 1311 
// 1312   usbp->otg->oe[ep].DOEPCTL |= DOEPCTL_STALL;
usb_lld_stall_out:
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        LDR      R2,[R0, #+80]
        ADDS     R2,R2,R1, LSL #+5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        LDR      R3,[R0, #+80]
        ADDS     R3,R3,R1, LSL #+5
        LDR      R3,[R3, #+2816]
        ORRS     R3,R3,#0x200000
        STR      R3,[R2, #+2816]
// 1313 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock36
// 1314 
// 1315 /**
// 1316  * @brief   Brings an IN endpoint in the stalled state.
// 1317  *
// 1318  * @param[in] usbp      pointer to the @p USBDriver object
// 1319  * @param[in] ep        endpoint number
// 1320  *
// 1321  * @notapi
// 1322  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock37 Using cfiCommon0
          CFI Function usb_lld_stall_in
          CFI NoCalls
        THUMB
// 1323 void usb_lld_stall_in(USBDriver *usbp, usbep_t ep) {
// 1324 
// 1325   usbp->otg->ie[ep].DIEPCTL |= DIEPCTL_STALL;
usb_lld_stall_in:
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        LDR      R2,[R0, #+80]
        ADDS     R2,R2,R1, LSL #+5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        LDR      R3,[R0, #+80]
        ADDS     R3,R3,R1, LSL #+5
        LDR      R3,[R3, #+2304]
        ORRS     R3,R3,#0x200000
        STR      R3,[R2, #+2304]
// 1326 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock37
// 1327 
// 1328 /**
// 1329  * @brief   Brings an OUT endpoint in the active state.
// 1330  *
// 1331  * @param[in] usbp      pointer to the @p USBDriver object
// 1332  * @param[in] ep        endpoint number
// 1333  *
// 1334  * @notapi
// 1335  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock38 Using cfiCommon0
          CFI Function usb_lld_clear_out
          CFI NoCalls
        THUMB
// 1336 void usb_lld_clear_out(USBDriver *usbp, usbep_t ep) {
// 1337 
// 1338   usbp->otg->oe[ep].DOEPCTL &= ~DOEPCTL_STALL;
usb_lld_clear_out:
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        LDR      R2,[R0, #+80]
        ADDS     R2,R2,R1, LSL #+5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        LDR      R3,[R0, #+80]
        ADDS     R3,R3,R1, LSL #+5
        LDR      R3,[R3, #+2816]
        BICS     R3,R3,#0x200000
        STR      R3,[R2, #+2816]
// 1339 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock38
// 1340 
// 1341 /**
// 1342  * @brief   Brings an IN endpoint in the active state.
// 1343  *
// 1344  * @param[in] usbp      pointer to the @p USBDriver object
// 1345  * @param[in] ep        endpoint number
// 1346  *
// 1347  * @notapi
// 1348  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock39 Using cfiCommon0
          CFI Function usb_lld_clear_in
          CFI NoCalls
        THUMB
// 1349 void usb_lld_clear_in(USBDriver *usbp, usbep_t ep) {
// 1350 
// 1351   usbp->otg->ie[ep].DIEPCTL &= ~DIEPCTL_STALL;
usb_lld_clear_in:
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        LDR      R2,[R0, #+80]
        ADDS     R2,R2,R1, LSL #+5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        LDR      R3,[R0, #+80]
        ADDS     R3,R3,R1, LSL #+5
        LDR      R3,[R3, #+2304]
        BICS     R3,R3,#0x200000
        STR      R3,[R2, #+2304]
// 1352 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock39

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        END
// 1353 
// 1354 #endif /* HAL_USE_USB */
// 1355 
// 1356 /** @} */
// 
// 1 468 bytes in section .bss
//    88 bytes in section .rodata
// 3 748 bytes in section .text
// 
// 3 726 bytes of CODE  memory (+ 22 bytes shared)
//    88 bytes of CONST memory
// 1 468 bytes of DATA  memory
//
//Errors: none
//Warnings: 1
