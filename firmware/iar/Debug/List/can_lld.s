///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:41 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       platforms\STM32\can_lld.c                            /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       platforms\STM32\can_lld.c -lCN                       /
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
//                       can_lld.s                                            /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME can_lld

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _port_irq_epilogue
        EXTERN canObjectInit
        EXTERN chDbgPanic
        EXTERN chDbgPanic3
        EXTERN chEvtBroadcastFlagsI
        EXTERN chSchGoSleepTimeoutS
        EXTERN chSemSignalI
        EXTERN dbg_check_enter_isr
        EXTERN dbg_check_leave_isr
        EXTERN dbg_check_lock_from_isr
        EXTERN dbg_check_unlock_from_isr
        EXTERN nvicDisableVector
        EXTERN nvicEnableVector

        PUBLIC CAND1
        PUBLIC CAND2
        PUBLIC Vector13C
        PUBLIC Vector140
        PUBLIC Vector144
        PUBLIC Vector148
        PUBLIC Vector8C
        PUBLIC Vector90
        PUBLIC Vector94
        PUBLIC Vector98
        PUBLIC canSTM32SetFilters
        PUBLIC can_lld_init
        PUBLIC can_lld_is_rx_nonempty
        PUBLIC can_lld_is_tx_empty
        PUBLIC can_lld_receive
        PUBLIC can_lld_sleep
        PUBLIC can_lld_start
        PUBLIC can_lld_stop
        PUBLIC can_lld_transmit
        PUBLIC can_lld_wakeup
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\platforms\STM32\can_lld.c
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
//   18  * @file    STM32/can_lld.c
//   19  * @brief   STM32 CAN subsystem low level driver source.
//   20  *
//   21  * @addtogroup CAN
//   22  * @{
//   23  */
//   24 
//   25 #include "ch.h"
//   26 #include "hal.h"
//   27 
//   28 #if HAL_USE_CAN || defined(__DOXYGEN__)
//   29 
//   30 /*===========================================================================*/
//   31 /* Driver local definitions.                                                 */
//   32 /*===========================================================================*/
//   33 
//   34 /*===========================================================================*/
//   35 /* Driver exported variables.                                                */
//   36 /*===========================================================================*/
//   37 
//   38 /** @brief CAN1 driver identifier.*/
//   39 #if STM32_CAN_USE_CAN1 || defined(__DOXYGEN__)

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   40 CANDriver CAND1;
CAND1:
        DS8 56
//   41 #endif
//   42 
//   43 /** @brief CAN2 driver identifier.*/
//   44 #if STM32_CAN_USE_CAN2 || defined(__DOXYGEN__)

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   45 CANDriver CAND2;
CAND2:
        DS8 56
//   46 #endif
//   47 
//   48 /*===========================================================================*/
//   49 /* Driver local variables and types.                                         */
//   50 /*===========================================================================*/
//   51 
//   52 /*===========================================================================*/
//   53 /* Driver local functions.                                                   */
//   54 /*===========================================================================*/
//   55 
//   56 /**
//   57  * @brief   Programs the filters.
//   58  *
//   59  * @param[in] can2sb    number of the first filter assigned to CAN2
//   60  * @param[in] num       number of entries in the filters array, if zero then
//   61  *                      a default filter is programmed
//   62  * @param[in] cfp       pointer to the filters array, can be @p NULL if
//   63  *                      (num == 0)
//   64  *
//   65  * @notapi
//   66  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function can_lld_set_filters
          CFI NoCalls
        THUMB
//   67 static void can_lld_set_filters(uint32_t can2sb,
//   68                                 uint32_t num,
//   69                                 const CANFilter *cfp) {
can_lld_set_filters:
        PUSH     {R4-R7}
          CFI R7 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
//   70 
//   71   /* Temporarily enabling CAN1 clock.*/
//   72   rccEnableCAN1(FALSE);
        LDR.W    R3,??DataTable13  ;; 0x40023840
        LDR      R3,[R3, #+0]
        ORRS     R3,R3,#0x2000000
        LDR.W    R4,??DataTable13  ;; 0x40023840
        STR      R3,[R4, #+0]
//   73 
//   74   /* Filters initialization.*/
//   75   CAN1->FMR = (CAN1->FMR & 0xFFFF0000) | (can2sb << 8) | CAN_FMR_FINIT;
        LDR.W    R3,??DataTable13_1  ;; 0x40006600
        LDR      R3,[R3, #+0]
        LSRS     R3,R3,#+16
        LSLS     R3,R3,#+16
        ORRS     R3,R3,R0, LSL #+8
        ORRS     R3,R3,#0x1
        LDR.W    R4,??DataTable13_1  ;; 0x40006600
        STR      R3,[R4, #+0]
//   76   if (num > 0) {
        CMP      R1,#+0
        BEQ.N    ??can_lld_set_filters_0
//   77     uint32_t i, fmask;
//   78 
//   79     /* All filters cleared.*/
//   80     CAN1->FA1R = 0;
        LDR.W    R5,??DataTable13_2  ;; 0x4000661c
        MOVS     R6,#+0
        STR      R6,[R5, #+0]
//   81     CAN1->FM1R = 0;
        LDR.W    R5,??DataTable13_3  ;; 0x40006604
        MOVS     R6,#+0
        STR      R6,[R5, #+0]
//   82     CAN1->FS1R = 0;
        LDR.W    R5,??DataTable13_4  ;; 0x4000660c
        MOVS     R6,#+0
        STR      R6,[R5, #+0]
//   83     CAN1->FFA1R = 0;
        LDR.W    R5,??DataTable13_5  ;; 0x40006614
        MOVS     R6,#+0
        STR      R6,[R5, #+0]
//   84     for (i = 0; i < STM32_CAN_MAX_FILTERS; i++) {
        MOVS     R5,#+0
        MOVS     R3,R5
??can_lld_set_filters_1:
        CMP      R3,#+28
        BCS.N    ??can_lld_set_filters_2
//   85       CAN1->sFilterRegister[i].FR1 = 0;
        LDR.W    R5,??DataTable13_6  ;; 0x40006640
        MOVS     R6,#+0
        STR      R6,[R5, R3, LSL #+3]
//   86       CAN1->sFilterRegister[i].FR2 = 0;
        LDR.W    R5,??DataTable13_6  ;; 0x40006640
        ADDS     R5,R5,R3, LSL #+3
        MOVS     R6,#+0
        STR      R6,[R5, #+4]
//   87     }
        ADDS     R3,R3,#+1
        B.N      ??can_lld_set_filters_1
//   88 
//   89     /* Scanning the filters array.*/
//   90     for (i = 0; i < num; i++) {
??can_lld_set_filters_2:
        MOVS     R5,#+0
        MOVS     R3,R5
??can_lld_set_filters_3:
        CMP      R3,R1
        BCS.N    ??can_lld_set_filters_4
//   91       fmask = 1 << cfp->filter;
        MOVS     R5,#+1
        LDR      R6,[R2, #+0]
        LSLS     R5,R5,R6
        MOVS     R4,R5
//   92       if (cfp->mode)
        LDRB     R5,[R2, #+4]
        LSLS     R5,R5,#+31
        BPL.N    ??can_lld_set_filters_5
//   93         CAN1->FM1R |= fmask;
        LDR.W    R5,??DataTable13_3  ;; 0x40006604
        LDR      R5,[R5, #+0]
        ORRS     R5,R4,R5
        LDR.W    R6,??DataTable13_3  ;; 0x40006604
        STR      R5,[R6, #+0]
//   94       if (cfp->scale)
??can_lld_set_filters_5:
        LDR      R5,[R2, #+4]
        UBFX     R5,R5,#+1,#+1
        CMP      R5,#+0
        BEQ.N    ??can_lld_set_filters_6
//   95         CAN1->FS1R |= fmask;
        LDR.W    R5,??DataTable13_4  ;; 0x4000660c
        LDR      R5,[R5, #+0]
        ORRS     R5,R4,R5
        LDR.W    R6,??DataTable13_4  ;; 0x4000660c
        STR      R5,[R6, #+0]
//   96       if (cfp->assignment)
??can_lld_set_filters_6:
        LDR      R5,[R2, #+4]
        UBFX     R5,R5,#+2,#+1
        CMP      R5,#+0
        BEQ.N    ??can_lld_set_filters_7
//   97         CAN1->FFA1R |= fmask;
        LDR.W    R5,??DataTable13_5  ;; 0x40006614
        LDR      R5,[R5, #+0]
        ORRS     R5,R4,R5
        LDR.W    R6,??DataTable13_5  ;; 0x40006614
        STR      R5,[R6, #+0]
//   98       CAN1->sFilterRegister[cfp->filter].FR1 = cfp->register1;
??can_lld_set_filters_7:
        LDR      R5,[R2, #+0]
        LDR.W    R6,??DataTable13_6  ;; 0x40006640
        LDR      R7,[R2, #+8]
        STR      R7,[R6, R5, LSL #+3]
//   99       CAN1->sFilterRegister[cfp->filter].FR2 = cfp->register2;
        LDR      R5,[R2, #+0]
        LDR.W    R6,??DataTable13_6  ;; 0x40006640
        ADDS     R5,R6,R5, LSL #+3
        LDR      R6,[R2, #+12]
        STR      R6,[R5, #+4]
//  100       CAN1->FA1R |= fmask;
        LDR.W    R5,??DataTable13_2  ;; 0x4000661c
        LDR      R5,[R5, #+0]
        ORRS     R5,R4,R5
        LDR.W    R6,??DataTable13_2  ;; 0x4000661c
        STR      R5,[R6, #+0]
//  101       cfp++;
        ADDS     R2,R2,#+16
//  102     }
        ADDS     R3,R3,#+1
        B.N      ??can_lld_set_filters_3
//  103   }
//  104   else {
//  105     /* Setting up a single default filter that enables everything for both
//  106        CANs.*/
//  107     CAN1->sFilterRegister[0].FR1 = 0;
??can_lld_set_filters_0:
        LDR.W    R3,??DataTable13_6  ;; 0x40006640
        MOVS     R4,#+0
        STR      R4,[R3, #+0]
//  108     CAN1->sFilterRegister[0].FR2 = 0;
        LDR.W    R3,??DataTable13_7  ;; 0x40006644
        MOVS     R4,#+0
        STR      R4,[R3, #+0]
//  109 #if STM32_HAS_CAN2
//  110     CAN1->sFilterRegister[can2sb].FR1 = 0;
        LDR.W    R3,??DataTable13_6  ;; 0x40006640
        MOVS     R4,#+0
        STR      R4,[R3, R0, LSL #+3]
//  111     CAN1->sFilterRegister[can2sb].FR2 = 0;
        LDR.W    R3,??DataTable13_6  ;; 0x40006640
        ADDS     R3,R3,R0, LSL #+3
        MOVS     R4,#+0
        STR      R4,[R3, #+4]
//  112 #endif
//  113     CAN1->FM1R = 0;
        LDR.W    R3,??DataTable13_3  ;; 0x40006604
        MOVS     R4,#+0
        STR      R4,[R3, #+0]
//  114     CAN1->FFA1R = 0;
        LDR.W    R3,??DataTable13_5  ;; 0x40006614
        MOVS     R4,#+0
        STR      R4,[R3, #+0]
//  115 #if STM32_HAS_CAN2
//  116     CAN1->FS1R = 1 | (1 << can2sb);
        MOVS     R3,#+1
        LSLS     R3,R3,R0
        ORRS     R3,R3,#0x1
        LDR.W    R4,??DataTable13_4  ;; 0x4000660c
        STR      R3,[R4, #+0]
//  117     CAN1->FA1R = 1 | (1 << can2sb);
        MOVS     R3,#+1
        LSLS     R3,R3,R0
        ORRS     R3,R3,#0x1
        LDR.W    R4,??DataTable13_2  ;; 0x4000661c
        STR      R3,[R4, #+0]
//  118 #else
//  119     CAN1->FS1R = 1;
//  120     CAN1->FA1R = 1;
//  121 #endif
//  122   }
//  123   CAN1->FMR &= ~CAN_FMR_FINIT;
??can_lld_set_filters_4:
        LDR.W    R3,??DataTable13_1  ;; 0x40006600
        LDR      R3,[R3, #+0]
        LSRS     R3,R3,#+1
        LSLS     R3,R3,#+1
        LDR.W    R4,??DataTable13_1  ;; 0x40006600
        STR      R3,[R4, #+0]
//  124 
//  125   /* Clock disabled, it will be enabled again in can_lld_start().*/
//  126   rccDisableCAN1(FALSE);
        LDR.W    R3,??DataTable13  ;; 0x40023840
        LDR      R3,[R3, #+0]
        BICS     R3,R3,#0x2000000
        LDR.W    R4,??DataTable13  ;; 0x40023840
        STR      R3,[R4, #+0]
//  127 }
        POP      {R4-R7}
          CFI R4 SameValue
          CFI R5 SameValue
          CFI R6 SameValue
          CFI R7 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock0
//  128 
//  129 /**
//  130  * @brief   Common TX ISR handler.
//  131  *
//  132  * @param[in] canp      pointer to the @p CANDriver object
//  133  *
//  134  * @notapi
//  135  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function can_lld_tx_handler
        THUMB
//  136 static void can_lld_tx_handler(CANDriver *canp) {
can_lld_tx_handler:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  137 
//  138   /* No more events until a message is transmitted.*/
//  139   canp->can->TSR = CAN_TSR_RQCP0 | CAN_TSR_RQCP1 | CAN_TSR_RQCP2;
        LDR      R0,[R4, #+52]
        LDR.W    R1,??DataTable13_8  ;; 0x10101
        STR      R1,[R0, #+8]
//  140   chSysLockFromIsr();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock_from_isr
        BL       dbg_check_lock_from_isr
//  141   while (chSemGetCounterI(&canp->txsem) < 0)
??can_lld_tx_handler_0:
        LDR      R0,[R4, #+16]
        CMP      R0,#+0
        BPL.N    ??can_lld_tx_handler_1
//  142     chSemSignalI(&canp->txsem);
        ADDS     R0,R4,#+8
          CFI FunCall chSemSignalI
        BL       chSemSignalI
        B.N      ??can_lld_tx_handler_0
//  143   chEvtBroadcastFlagsI(&canp->txempty_event, CAN_MAILBOX_TO_MASK(1));
??can_lld_tx_handler_1:
        MOVS     R1,#+1
        ADDS     R0,R4,#+36
          CFI FunCall chEvtBroadcastFlagsI
        BL       chEvtBroadcastFlagsI
//  144   chSysUnlockFromIsr();
          CFI FunCall dbg_check_unlock_from_isr
        BL       dbg_check_unlock_from_isr
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  145 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock1
//  146 
//  147 /**
//  148  * @brief   Common RX0 ISR handler.
//  149  *
//  150  * @param[in] canp      pointer to the @p CANDriver object
//  151  *
//  152  * @notapi
//  153  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function can_lld_rx0_handler
        THUMB
//  154 static void can_lld_rx0_handler(CANDriver *canp) {
can_lld_rx0_handler:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  155   uint32_t rf0r;
//  156 
//  157   rf0r = canp->can->RF0R;
        LDR      R0,[R4, #+52]
        LDR      R0,[R0, #+12]
        MOVS     R5,R0
//  158   if ((rf0r & CAN_RF0R_FMP0) > 0) {
        TST      R5,#0x3
        BEQ.N    ??can_lld_rx0_handler_0
//  159     /* No more receive events until the queue 0 has been emptied.*/
//  160     canp->can->IER &= ~CAN_IER_FMPIE0;
        LDR      R0,[R4, #+52]
        LDR      R0,[R0, #+20]
        BICS     R0,R0,#0x2
        LDR      R1,[R4, #+52]
        STR      R0,[R1, #+20]
//  161     chSysLockFromIsr();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock_from_isr
        BL       dbg_check_lock_from_isr
//  162     while (chSemGetCounterI(&canp->rxsem) < 0)
??can_lld_rx0_handler_1:
        LDR      R0,[R4, #+28]
        CMP      R0,#+0
        BPL.N    ??can_lld_rx0_handler_2
//  163       chSemSignalI(&canp->rxsem);
        ADDS     R0,R4,#+20
          CFI FunCall chSemSignalI
        BL       chSemSignalI
        B.N      ??can_lld_rx0_handler_1
//  164     chEvtBroadcastFlagsI(&canp->rxfull_event, CAN_MAILBOX_TO_MASK(1));
??can_lld_rx0_handler_2:
        MOVS     R1,#+1
        ADDS     R0,R4,#+32
          CFI FunCall chEvtBroadcastFlagsI
        BL       chEvtBroadcastFlagsI
//  165     chSysUnlockFromIsr();
          CFI FunCall dbg_check_unlock_from_isr
        BL       dbg_check_unlock_from_isr
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  166   }
//  167   if ((rf0r & CAN_RF0R_FOVR0) > 0) {
??can_lld_rx0_handler_0:
        LSLS     R0,R5,#+27
        BPL.N    ??can_lld_rx0_handler_3
//  168     /* Overflow events handling.*/
//  169     canp->can->RF0R = CAN_RF0R_FOVR0;
        LDR      R0,[R4, #+52]
        MOVS     R1,#+16
        STR      R1,[R0, #+12]
//  170     chSysLockFromIsr();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock_from_isr
        BL       dbg_check_lock_from_isr
//  171     chEvtBroadcastFlagsI(&canp->error_event, CAN_OVERFLOW_ERROR);
        MOVS     R1,#+16
        ADDS     R0,R4,#+40
          CFI FunCall chEvtBroadcastFlagsI
        BL       chEvtBroadcastFlagsI
//  172     chSysUnlockFromIsr();
          CFI FunCall dbg_check_unlock_from_isr
        BL       dbg_check_unlock_from_isr
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  173   }
//  174 }
??can_lld_rx0_handler_3:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock2
//  175 
//  176 /**
//  177  * @brief   Common RX1 ISR handler.
//  178  *
//  179  * @param[in] canp      pointer to the @p CANDriver object
//  180  *
//  181  * @notapi
//  182  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function can_lld_rx1_handler
        THUMB
//  183 static void can_lld_rx1_handler(CANDriver *canp) {
can_lld_rx1_handler:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  184   uint32_t rf1r;
//  185 
//  186   rf1r = canp->can->RF1R;
        LDR      R0,[R4, #+52]
        LDR      R0,[R0, #+16]
        MOVS     R5,R0
//  187   if ((rf1r & CAN_RF1R_FMP1) > 0) {
        TST      R5,#0x3
        BEQ.N    ??can_lld_rx1_handler_0
//  188     /* No more receive events until the queue 0 has been emptied.*/
//  189     canp->can->IER &= ~CAN_IER_FMPIE1;
        LDR      R0,[R4, #+52]
        LDR      R0,[R0, #+20]
        BICS     R0,R0,#0x10
        LDR      R1,[R4, #+52]
        STR      R0,[R1, #+20]
//  190     chSysLockFromIsr();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock_from_isr
        BL       dbg_check_lock_from_isr
//  191     while (chSemGetCounterI(&canp->rxsem) < 0)
??can_lld_rx1_handler_1:
        LDR      R0,[R4, #+28]
        CMP      R0,#+0
        BPL.N    ??can_lld_rx1_handler_2
//  192       chSemSignalI(&canp->rxsem);
        ADDS     R0,R4,#+20
          CFI FunCall chSemSignalI
        BL       chSemSignalI
        B.N      ??can_lld_rx1_handler_1
//  193     chEvtBroadcastFlagsI(&canp->rxfull_event, CAN_MAILBOX_TO_MASK(2));
??can_lld_rx1_handler_2:
        MOVS     R1,#+2
        ADDS     R0,R4,#+32
          CFI FunCall chEvtBroadcastFlagsI
        BL       chEvtBroadcastFlagsI
//  194     chSysUnlockFromIsr();
          CFI FunCall dbg_check_unlock_from_isr
        BL       dbg_check_unlock_from_isr
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  195   }
//  196   if ((rf1r & CAN_RF1R_FOVR1) > 0) {
??can_lld_rx1_handler_0:
        LSLS     R0,R5,#+27
        BPL.N    ??can_lld_rx1_handler_3
//  197     /* Overflow events handling.*/
//  198     canp->can->RF1R = CAN_RF1R_FOVR1;
        LDR      R0,[R4, #+52]
        MOVS     R1,#+16
        STR      R1,[R0, #+16]
//  199     chSysLockFromIsr();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock_from_isr
        BL       dbg_check_lock_from_isr
//  200     chEvtBroadcastFlagsI(&canp->error_event, CAN_OVERFLOW_ERROR);
        MOVS     R1,#+16
        ADDS     R0,R4,#+40
          CFI FunCall chEvtBroadcastFlagsI
        BL       chEvtBroadcastFlagsI
//  201     chSysUnlockFromIsr();
          CFI FunCall dbg_check_unlock_from_isr
        BL       dbg_check_unlock_from_isr
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  202   }
//  203 }
??can_lld_rx1_handler_3:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock3
//  204 
//  205 /**
//  206  * @brief   Common SCE ISR handler.
//  207  *
//  208  * @param[in] canp      pointer to the @p CANDriver object
//  209  *
//  210  * @notapi
//  211  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function can_lld_sce_handler
        THUMB
//  212 static void can_lld_sce_handler(CANDriver *canp) {
can_lld_sce_handler:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
//  213   uint32_t msr;
//  214 
//  215   msr = canp->can->MSR;
        LDR      R0,[R4, #+52]
        LDR      R0,[R0, #+4]
        MOVS     R5,R0
//  216   canp->can->MSR = CAN_MSR_ERRI | CAN_MSR_WKUI | CAN_MSR_SLAKI;
        LDR      R0,[R4, #+52]
        MOVS     R1,#+28
        STR      R1,[R0, #+4]
//  217   /* Wakeup event.*/
//  218 #if CAN_USE_SLEEP_MODE
//  219   if (msr & CAN_MSR_WKUI) {
        LSLS     R0,R5,#+28
        BPL.N    ??can_lld_sce_handler_0
//  220     canp->state = CAN_READY;
        MOVS     R0,#+3
        STRB     R0,[R4, #+0]
//  221     canp->can->MCR &= ~CAN_MCR_SLEEP;
        LDR      R0,[R4, #+52]
        LDR      R0,[R0, #+0]
        BICS     R0,R0,#0x2
        LDR      R1,[R4, #+52]
        STR      R0,[R1, #+0]
//  222     chSysLockFromIsr();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock_from_isr
        BL       dbg_check_lock_from_isr
//  223     chEvtBroadcastI(&canp->wakeup_event);
        MOVS     R1,#+0
        ADDS     R0,R4,#+48
          CFI FunCall chEvtBroadcastFlagsI
        BL       chEvtBroadcastFlagsI
//  224     chSysUnlockFromIsr();
          CFI FunCall dbg_check_unlock_from_isr
        BL       dbg_check_unlock_from_isr
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  225   }
//  226 #endif /* CAN_USE_SLEEP_MODE */
//  227   /* Error event.*/
//  228   if (msr & CAN_MSR_ERRI) {
??can_lld_sce_handler_0:
        LSLS     R0,R5,#+29
        BPL.N    ??can_lld_sce_handler_1
//  229     flagsmask_t flags;
//  230     uint32_t esr = canp->can->ESR;
        LDR      R0,[R4, #+52]
        LDR      R7,[R0, #+24]
//  231 
//  232     canp->can->ESR &= ~CAN_ESR_LEC;
        LDR      R0,[R4, #+52]
        LDR      R0,[R0, #+24]
        BICS     R0,R0,#0x70
        LDR      R1,[R4, #+52]
        STR      R0,[R1, #+24]
//  233     flags = (flagsmask_t)(esr & 7);
        ANDS     R0,R7,#0x7
        MOVS     R6,R0
//  234     if ((esr & CAN_ESR_LEC) > 0)
        TST      R7,#0x70
        BEQ.N    ??can_lld_sce_handler_2
//  235       flags |= CAN_FRAMING_ERROR;
        ORRS     R6,R6,#0x8
//  236 
//  237     chSysLockFromIsr();
??can_lld_sce_handler_2:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock_from_isr
        BL       dbg_check_lock_from_isr
//  238     /* The content of the ESR register is copied unchanged in the upper
//  239        half word of the listener flags mask.*/
//  240     chEvtBroadcastFlagsI(&canp->error_event, flags | (flagsmask_t)(esr << 16));
        ORRS     R1,R6,R7, LSL #+16
        ADDS     R0,R4,#+40
          CFI FunCall chEvtBroadcastFlagsI
        BL       chEvtBroadcastFlagsI
//  241     chSysUnlockFromIsr();
          CFI FunCall dbg_check_unlock_from_isr
        BL       dbg_check_unlock_from_isr
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  242   }
//  243 }
??can_lld_sce_handler_1:
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock4
//  244 
//  245 /*===========================================================================*/
//  246 /* Driver interrupt handlers.                                                */
//  247 /*===========================================================================*/
//  248 
//  249 #if STM32_CAN_USE_CAN1 || defined(__DOXYGEN__)
//  250 /**
//  251  * @brief   CAN1 TX interrupt handler.
//  252  *
//  253  * @isr
//  254  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function Vector8C
        THUMB
//  255 CH_IRQ_HANDLER(STM32_CAN1_TX_HANDLER) {
Vector8C:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  256 
//  257   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//  258 
//  259   can_lld_tx_handler(&CAND1);
        LDR.W    R0,??DataTable13_9
          CFI FunCall can_lld_tx_handler
        BL       can_lld_tx_handler
//  260 
//  261   CH_IRQ_EPILOGUE();
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//  262 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock5
//  263 
//  264 /*
//  265  * @brief   CAN1 RX0 interrupt handler.
//  266  *
//  267  * @isr
//  268  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function Vector90
        THUMB
//  269 CH_IRQ_HANDLER(STM32_CAN1_RX0_HANDLER) {
Vector90:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  270 
//  271   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//  272 
//  273   can_lld_rx0_handler(&CAND1);
        LDR.W    R0,??DataTable13_9
          CFI FunCall can_lld_rx0_handler
        BL       can_lld_rx0_handler
//  274 
//  275   CH_IRQ_EPILOGUE();
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//  276 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock6
//  277 
//  278 /**
//  279  * @brief   CAN1 RX1 interrupt handler.
//  280  *
//  281  * @isr
//  282  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function Vector94
        THUMB
//  283 CH_IRQ_HANDLER(STM32_CAN1_RX1_HANDLER) {
Vector94:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  284 
//  285   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//  286 
//  287   can_lld_rx1_handler(&CAND1);
        LDR.W    R0,??DataTable13_9
          CFI FunCall can_lld_rx1_handler
        BL       can_lld_rx1_handler
//  288 
//  289   CH_IRQ_EPILOGUE();
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//  290 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock7
//  291 
//  292 /**
//  293  * @brief   CAN1 SCE interrupt handler.
//  294  *
//  295  * @isr
//  296  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function Vector98
        THUMB
//  297 CH_IRQ_HANDLER(STM32_CAN1_SCE_HANDLER) {
Vector98:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  298 
//  299   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//  300 
//  301   can_lld_sce_handler(&CAND1);
        LDR.W    R0,??DataTable13_9
          CFI FunCall can_lld_sce_handler
        BL       can_lld_sce_handler
//  302 
//  303   CH_IRQ_EPILOGUE();
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//  304 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock8
//  305 #endif /* STM32_CAN_USE_CAN1 */
//  306 
//  307 #if STM32_CAN_USE_CAN2 || defined(__DOXYGEN__)
//  308 /**
//  309  * @brief   CAN2 TX interrupt handler.
//  310  *
//  311  * @isr
//  312  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function Vector13C
        THUMB
//  313 CH_IRQ_HANDLER(STM32_CAN2_TX_HANDLER) {
Vector13C:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  314 
//  315   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//  316 
//  317   can_lld_tx_handler(&CAND2);
        LDR.W    R0,??DataTable13_10
          CFI FunCall can_lld_tx_handler
        BL       can_lld_tx_handler
//  318 
//  319   CH_IRQ_EPILOGUE();
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//  320 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock9
//  321 
//  322 /*
//  323  * @brief   CAN2 RX0 interrupt handler.
//  324  *
//  325  * @isr
//  326  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function Vector140
        THUMB
//  327 CH_IRQ_HANDLER(STM32_CAN2_RX0_HANDLER) {
Vector140:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  328 
//  329   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//  330 
//  331   can_lld_rx0_handler(&CAND2);
        LDR.W    R0,??DataTable13_10
          CFI FunCall can_lld_rx0_handler
        BL       can_lld_rx0_handler
//  332 
//  333   CH_IRQ_EPILOGUE();
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//  334 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock10
//  335 
//  336 /**
//  337  * @brief   CAN2 RX1 interrupt handler.
//  338  *
//  339  * @isr
//  340  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function Vector144
        THUMB
//  341 CH_IRQ_HANDLER(STM32_CAN2_RX1_HANDLER) {
Vector144:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  342 
//  343   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//  344 
//  345   can_lld_rx1_handler(&CAND2);
        LDR.W    R0,??DataTable13_10
          CFI FunCall can_lld_rx1_handler
        BL       can_lld_rx1_handler
//  346 
//  347   CH_IRQ_EPILOGUE();
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//  348 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock11
//  349 
//  350 /**
//  351  * @brief   CAN2 SCE interrupt handler.
//  352  *
//  353  * @isr
//  354  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function Vector148
        THUMB
//  355 CH_IRQ_HANDLER(STM32_CAN2_SCE_HANDLER) {
Vector148:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  356 
//  357   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//  358 
//  359   can_lld_sce_handler(&CAND2);
        LDR.W    R0,??DataTable13_10
          CFI FunCall can_lld_sce_handler
        BL       can_lld_sce_handler
//  360 
//  361   CH_IRQ_EPILOGUE();
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//  362 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock12
//  363 #endif /* STM32_CAN_USE_CAN2 */
//  364 
//  365 /*===========================================================================*/
//  366 /* Driver exported functions.                                                */
//  367 /*===========================================================================*/
//  368 
//  369 /**
//  370  * @brief   Low level CAN driver initialization.
//  371  *
//  372  * @notapi
//  373  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock13 Using cfiCommon0
          CFI Function can_lld_init
        THUMB
//  374 void can_lld_init(void) {
can_lld_init:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  375 
//  376 #if STM32_CAN_USE_CAN1
//  377   /* Driver initialization.*/
//  378   canObjectInit(&CAND1);
        LDR.W    R0,??DataTable13_9
          CFI FunCall canObjectInit
        BL       canObjectInit
//  379   CAND1.can = CAN1;
        LDR.W    R0,??DataTable13_9
        LDR.W    R1,??DataTable13_11  ;; 0x40006400
        STR      R1,[R0, #+52]
//  380 #endif
//  381 #if STM32_CAN_USE_CAN2
//  382   /* Driver initialization.*/
//  383   canObjectInit(&CAND2);
        LDR.W    R0,??DataTable13_10
          CFI FunCall canObjectInit
        BL       canObjectInit
//  384   CAND2.can = CAN2;
        LDR.W    R0,??DataTable13_10
        LDR.W    R1,??DataTable13_12  ;; 0x40006800
        STR      R1,[R0, #+52]
//  385 #endif
//  386 
//  387   /* Filters initialization.*/
//  388 #if STM32_HAS_CAN2
//  389   can_lld_set_filters(STM32_CAN_MAX_FILTERS / 2, 0, NULL);
        MOVS     R2,#+0
        MOVS     R1,#+0
        MOVS     R0,#+14
          CFI FunCall can_lld_set_filters
        BL       can_lld_set_filters
//  390 #else
//  391   can_lld_set_filters(STM32_CAN_MAX_FILTERS, 0, NULL);
//  392 #endif
//  393 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock13
//  394 
//  395 /**
//  396  * @brief   Configures and activates the CAN peripheral.
//  397  *
//  398  * @param[in] canp      pointer to the @p CANDriver object
//  399  *
//  400  * @notapi
//  401  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock14 Using cfiCommon0
          CFI Function can_lld_start
        THUMB
//  402 void can_lld_start(CANDriver *canp) {
can_lld_start:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  403 
//  404   /* Clock activation.*/
//  405 #if STM32_CAN_USE_CAN1
//  406   if (&CAND1 == canp) {
        LDR.W    R0,??DataTable13_9
        CMP      R0,R4
        BNE.N    ??can_lld_start_0
//  407     nvicEnableVector(STM32_CAN1_TX_NUMBER,
//  408                      CORTEX_PRIORITY_MASK(STM32_CAN_CAN1_IRQ_PRIORITY));
        MOVS     R1,#+176
        MOVS     R0,#+19
          CFI FunCall nvicEnableVector
        BL       nvicEnableVector
//  409     nvicEnableVector(STM32_CAN1_RX0_NUMBER,
//  410                      CORTEX_PRIORITY_MASK(STM32_CAN_CAN1_IRQ_PRIORITY));
        MOVS     R1,#+176
        MOVS     R0,#+20
          CFI FunCall nvicEnableVector
        BL       nvicEnableVector
//  411     nvicEnableVector(STM32_CAN1_RX1_NUMBER,
//  412                      CORTEX_PRIORITY_MASK(STM32_CAN_CAN1_IRQ_PRIORITY));
        MOVS     R1,#+176
        MOVS     R0,#+21
          CFI FunCall nvicEnableVector
        BL       nvicEnableVector
//  413     nvicEnableVector(STM32_CAN1_SCE_NUMBER,
//  414                      CORTEX_PRIORITY_MASK(STM32_CAN_CAN1_IRQ_PRIORITY));
        MOVS     R1,#+176
        MOVS     R0,#+22
          CFI FunCall nvicEnableVector
        BL       nvicEnableVector
//  415     rccEnableCAN1(FALSE);
        LDR.W    R0,??DataTable13  ;; 0x40023840
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x2000000
        LDR.W    R1,??DataTable13  ;; 0x40023840
        STR      R0,[R1, #+0]
//  416   }
//  417 #endif
//  418 #if STM32_CAN_USE_CAN2
//  419   if (&CAND2 == canp) {
??can_lld_start_0:
        LDR.W    R0,??DataTable13_10
        CMP      R0,R4
        BNE.N    ??can_lld_start_1
//  420 
//  421     chDbgAssert(CAND1.state != CAN_STOP,
//  422                 "can_lld_start(), #1", "CAN1 must be started");
        LDR.W    R0,??DataTable13_9
        LDRB     R0,[R0, #+0]
        CMP      R0,#+1
        BNE.N    ??can_lld_start_2
        LDR.W    R0,??DataTable13_13
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  423 
//  424     nvicEnableVector(STM32_CAN2_TX_NUMBER,
//  425                      CORTEX_PRIORITY_MASK(STM32_CAN_CAN2_IRQ_PRIORITY));
??can_lld_start_2:
        MOVS     R1,#+176
        MOVS     R0,#+63
          CFI FunCall nvicEnableVector
        BL       nvicEnableVector
//  426     nvicEnableVector(STM32_CAN2_RX0_NUMBER,
//  427                      CORTEX_PRIORITY_MASK(STM32_CAN_CAN2_IRQ_PRIORITY));
        MOVS     R1,#+176
        MOVS     R0,#+64
          CFI FunCall nvicEnableVector
        BL       nvicEnableVector
//  428     nvicEnableVector(STM32_CAN2_RX1_NUMBER,
//  429                      CORTEX_PRIORITY_MASK(STM32_CAN_CAN2_IRQ_PRIORITY));
        MOVS     R1,#+176
        MOVS     R0,#+65
          CFI FunCall nvicEnableVector
        BL       nvicEnableVector
//  430     nvicEnableVector(STM32_CAN2_SCE_NUMBER,
//  431                      CORTEX_PRIORITY_MASK(STM32_CAN_CAN2_IRQ_PRIORITY));
        MOVS     R1,#+176
        MOVS     R0,#+66
          CFI FunCall nvicEnableVector
        BL       nvicEnableVector
//  432     rccEnableCAN2(FALSE);
        LDR.W    R0,??DataTable13  ;; 0x40023840
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x4000000
        LDR.W    R1,??DataTable13  ;; 0x40023840
        STR      R0,[R1, #+0]
//  433   }
//  434 #endif
//  435 
//  436   /* Entering initialization mode. */
//  437   canp->state = CAN_STARTING;
??can_lld_start_1:
        MOVS     R0,#+2
        STRB     R0,[R4, #+0]
//  438   canp->can->MCR = CAN_MCR_INRQ;
        LDR      R0,[R4, #+52]
        MOVS     R1,#+1
        STR      R1,[R0, #+0]
//  439   while ((canp->can->MSR & CAN_MSR_INAK) == 0)
??can_lld_start_3:
        LDR      R0,[R4, #+52]
        LDR      R0,[R0, #+4]
        LSLS     R0,R0,#+31
        BMI.N    ??can_lld_start_4
//  440     chThdSleepS(1);
        MOVS     R1,#+1
        MOVS     R0,#+6
          CFI FunCall chSchGoSleepTimeoutS
        BL       chSchGoSleepTimeoutS
        B.N      ??can_lld_start_3
//  441   /* BTR initialization.*/
//  442   canp->can->BTR = canp->config->btr;
??can_lld_start_4:
        LDR      R0,[R4, #+52]
        LDR      R1,[R4, #+4]
        LDR      R1,[R1, #+4]
        STR      R1,[R0, #+28]
//  443   /* MCR initialization.*/
//  444   canp->can->MCR = canp->config->mcr;
        LDR      R0,[R4, #+52]
        LDR      R1,[R4, #+4]
        LDR      R1,[R1, #+0]
        STR      R1,[R0, #+0]
//  445 
//  446   /* Interrupt sources initialization.*/
//  447   canp->can->IER = CAN_IER_TMEIE  | CAN_IER_FMPIE0 | CAN_IER_FMPIE1 |
//  448                    CAN_IER_WKUIE  | CAN_IER_ERRIE  | CAN_IER_LECIE  |
//  449                    CAN_IER_BOFIE  | CAN_IER_EPVIE  | CAN_IER_EWGIE  |
//  450                    CAN_IER_FOVIE0 | CAN_IER_FOVIE1;
        LDR      R0,[R4, #+52]
        LDR.W    R1,??DataTable13_14  ;; 0x18f5b
        STR      R1,[R0, #+20]
//  451 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock14
//  452 
//  453 /**
//  454  * @brief   Deactivates the CAN peripheral.
//  455  *
//  456  * @param[in] canp      pointer to the @p CANDriver object
//  457  *
//  458  * @notapi
//  459  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock15 Using cfiCommon0
          CFI Function can_lld_stop
        THUMB
//  460 void can_lld_stop(CANDriver *canp) {
can_lld_stop:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  461 
//  462   /* If in ready state then disables the CAN peripheral.*/
//  463   if (canp->state == CAN_READY) {
        LDRB     R0,[R4, #+0]
        CMP      R0,#+3
        BNE.N    ??can_lld_stop_0
//  464 #if STM32_CAN_USE_CAN1
//  465     if (&CAND1 == canp) {
        LDR.W    R0,??DataTable13_9
        CMP      R0,R4
        BNE.N    ??can_lld_stop_1
//  466 
//  467 #if STM32_CAN_USE_CAN2
//  468       chDbgAssert(CAND2.state == CAN_STOP,
//  469                   "can_lld_stop(), #1", "CAN2 must be stopped");
        LDR.W    R0,??DataTable13_10
        LDRB     R0,[R0, #+0]
        CMP      R0,#+1
        BEQ.N    ??can_lld_stop_2
        LDR.W    R0,??DataTable13_15
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  470 #endif
//  471 
//  472       CAN1->MCR = 0x00010002;                   /* Register reset value.    */
??can_lld_stop_2:
        LDR.N    R0,??DataTable13_11  ;; 0x40006400
        LDR.W    R1,??DataTable13_16  ;; 0x10002
        STR      R1,[R0, #+0]
//  473       CAN1->IER = 0x00000000;                   /* All sources disabled.    */
        LDR.N    R0,??DataTable13_17  ;; 0x40006414
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//  474       nvicDisableVector(STM32_CAN1_TX_NUMBER);
        MOVS     R0,#+19
          CFI FunCall nvicDisableVector
        BL       nvicDisableVector
//  475       nvicDisableVector(STM32_CAN1_RX0_NUMBER);
        MOVS     R0,#+20
          CFI FunCall nvicDisableVector
        BL       nvicDisableVector
//  476       nvicDisableVector(STM32_CAN1_RX1_NUMBER);
        MOVS     R0,#+21
          CFI FunCall nvicDisableVector
        BL       nvicDisableVector
//  477       nvicDisableVector(STM32_CAN1_SCE_NUMBER);
        MOVS     R0,#+22
          CFI FunCall nvicDisableVector
        BL       nvicDisableVector
//  478       rccDisableCAN1(FALSE);
        LDR.N    R0,??DataTable13  ;; 0x40023840
        LDR      R0,[R0, #+0]
        BICS     R0,R0,#0x2000000
        LDR.N    R1,??DataTable13  ;; 0x40023840
        STR      R0,[R1, #+0]
//  479     }
//  480 #endif
//  481 #if STM32_CAN_USE_CAN2
//  482     if (&CAND2 == canp) {
??can_lld_stop_1:
        LDR.N    R0,??DataTable13_10
        CMP      R0,R4
        BNE.N    ??can_lld_stop_0
//  483       CAN2->MCR = 0x00010002;                   /* Register reset value.    */
        LDR.N    R0,??DataTable13_12  ;; 0x40006800
        LDR.N    R1,??DataTable13_16  ;; 0x10002
        STR      R1,[R0, #+0]
//  484       CAN2->IER = 0x00000000;                   /* All sources disabled.    */
        LDR.N    R0,??DataTable13_18  ;; 0x40006814
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//  485       nvicDisableVector(STM32_CAN2_TX_NUMBER);
        MOVS     R0,#+63
          CFI FunCall nvicDisableVector
        BL       nvicDisableVector
//  486       nvicDisableVector(STM32_CAN2_RX0_NUMBER);
        MOVS     R0,#+64
          CFI FunCall nvicDisableVector
        BL       nvicDisableVector
//  487       nvicDisableVector(STM32_CAN2_RX1_NUMBER);
        MOVS     R0,#+65
          CFI FunCall nvicDisableVector
        BL       nvicDisableVector
//  488       nvicDisableVector(STM32_CAN2_SCE_NUMBER);
        MOVS     R0,#+66
          CFI FunCall nvicDisableVector
        BL       nvicDisableVector
//  489       rccDisableCAN2(FALSE);
        LDR.N    R0,??DataTable13  ;; 0x40023840
        LDR      R0,[R0, #+0]
        BICS     R0,R0,#0x4000000
        LDR.N    R1,??DataTable13  ;; 0x40023840
        STR      R0,[R1, #+0]
//  490     }
//  491 #endif
//  492   }
//  493 }
??can_lld_stop_0:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock15
//  494 
//  495 /**
//  496  * @brief   Determines whether a frame can be transmitted.
//  497  *
//  498  * @param[in] canp      pointer to the @p CANDriver object
//  499  * @param[in] mailbox   mailbox number, @p CAN_ANY_MAILBOX for any mailbox
//  500  *
//  501  * @return              The queue space availability.
//  502  * @retval FALSE        no space in the transmit queue.
//  503  * @retval TRUE         transmit slot available.
//  504  *
//  505  * @notapi
//  506  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock16 Using cfiCommon0
          CFI Function can_lld_is_tx_empty
          CFI NoCalls
        THUMB
//  507 bool_t can_lld_is_tx_empty(CANDriver *canp, canmbx_t mailbox) {
can_lld_is_tx_empty:
        MOVS     R2,R0
//  508 
//  509   switch (mailbox) {
        CMP      R1,#+0
        BEQ.N    ??can_lld_is_tx_empty_0
        CMP      R1,#+2
        BEQ.N    ??can_lld_is_tx_empty_1
        BCC.N    ??can_lld_is_tx_empty_2
        CMP      R1,#+3
        BEQ.N    ??can_lld_is_tx_empty_3
        B.N      ??can_lld_is_tx_empty_4
//  510   case CAN_ANY_MAILBOX:
//  511     return (canp->can->TSR & CAN_TSR_TME) != 0;
??can_lld_is_tx_empty_0:
        LDR      R0,[R2, #+52]
        LDR      R0,[R0, #+8]
        TST      R0,#0x1C000000
        BEQ.N    ??can_lld_is_tx_empty_5
        MOVS     R0,#+1
        B.N      ??can_lld_is_tx_empty_6
??can_lld_is_tx_empty_5:
        MOVS     R0,#+0
??can_lld_is_tx_empty_6:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        B.N      ??can_lld_is_tx_empty_7
//  512   case 1:
//  513     return (canp->can->TSR & CAN_TSR_TME0) != 0;
??can_lld_is_tx_empty_2:
        LDR      R0,[R2, #+52]
        LDR      R0,[R0, #+8]
        ANDS     R0,R0,#0x4000000
        LSRS     R0,R0,#+26
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        B.N      ??can_lld_is_tx_empty_7
//  514   case 2:
//  515     return (canp->can->TSR & CAN_TSR_TME1) != 0;
??can_lld_is_tx_empty_1:
        LDR      R0,[R2, #+52]
        LDR      R0,[R0, #+8]
        ANDS     R0,R0,#0x8000000
        LSRS     R0,R0,#+27
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        B.N      ??can_lld_is_tx_empty_7
//  516   case 3:
//  517     return (canp->can->TSR & CAN_TSR_TME2) != 0;
??can_lld_is_tx_empty_3:
        LDR      R0,[R2, #+52]
        LDR      R0,[R0, #+8]
        ANDS     R0,R0,#0x10000000
        LSRS     R0,R0,#+28
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        B.N      ??can_lld_is_tx_empty_7
//  518   default:
//  519     return FALSE;
??can_lld_is_tx_empty_4:
        MOVS     R0,#+0
??can_lld_is_tx_empty_7:
        BX       LR               ;; return
          CFI EndBlock cfiBlock16
//  520   }
//  521 }
//  522 
//  523 /**
//  524  * @brief   Inserts a frame into the transmit queue.
//  525  *
//  526  * @param[in] canp      pointer to the @p CANDriver object
//  527  * @param[in] ctfp      pointer to the CAN frame to be transmitted
//  528  * @param[in] mailbox   mailbox number,  @p CAN_ANY_MAILBOX for any mailbox
//  529  *
//  530  * @notapi
//  531  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock17 Using cfiCommon0
          CFI Function can_lld_transmit
          CFI NoCalls
        THUMB
//  532 void can_lld_transmit(CANDriver *canp,
//  533                       canmbx_t mailbox,
//  534                       const CANTxFrame *ctfp) {
can_lld_transmit:
        PUSH     {R4-R6}
          CFI R6 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+12
//  535   uint32_t tir;
//  536   CAN_TxMailBox_TypeDef *tmbp;
//  537 
//  538   /* Pointer to a free transmission mailbox.*/
//  539   switch (mailbox) {
        CMP      R1,#+0
        BEQ.N    ??can_lld_transmit_0
        CMP      R1,#+2
        BEQ.N    ??can_lld_transmit_1
        BCC.N    ??can_lld_transmit_2
        CMP      R1,#+3
        BEQ.N    ??can_lld_transmit_3
        B.N      ??can_lld_transmit_4
//  540   case CAN_ANY_MAILBOX:
//  541     tmbp = &canp->can->sTxMailBox[(canp->can->TSR & CAN_TSR_CODE) >> 24];
??can_lld_transmit_0:
        LDR      R5,[R0, #+52]
        LDR      R5,[R5, #+8]
        UBFX     R5,R5,#+24,#+2
        LDR      R6,[R0, #+52]
        ADDS     R5,R6,R5, LSL #+4
        ADDS     R5,R5,#+384
        MOVS     R4,R5
//  542     break;
        B.N      ??can_lld_transmit_5
//  543   case 1:
//  544     tmbp = &canp->can->sTxMailBox[0];
??can_lld_transmit_2:
        LDR      R5,[R0, #+52]
        ADDS     R5,R5,#+384
        MOVS     R4,R5
//  545     break;
        B.N      ??can_lld_transmit_5
//  546   case 2:
//  547     tmbp = &canp->can->sTxMailBox[1];
??can_lld_transmit_1:
        LDR      R5,[R0, #+52]
        ADDS     R5,R5,#+400
        MOVS     R4,R5
//  548     break;
        B.N      ??can_lld_transmit_5
//  549   case 3:
//  550     tmbp = &canp->can->sTxMailBox[2];
??can_lld_transmit_3:
        LDR      R5,[R0, #+52]
        ADDS     R5,R5,#+416
        MOVS     R4,R5
//  551     break;
        B.N      ??can_lld_transmit_5
//  552   default:
//  553     return;
??can_lld_transmit_4:
        B.N      ??can_lld_transmit_6
//  554   }
//  555 
//  556   /* Preparing the message.*/
//  557   if (ctfp->IDE)
??can_lld_transmit_5:
        LDRB     R5,[R2, #+0]
        UBFX     R5,R5,#+5,#+1
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+0
        BEQ.N    ??can_lld_transmit_7
//  558     tir = ((uint32_t)ctfp->EID << 3) | ((uint32_t)ctfp->RTR << 1) |
//  559           CAN_TI0R_IDE;
        LDR      R5,[R2, #+4]
        LDRB     R6,[R2, #+0]
        UBFX     R6,R6,#+4,#+1
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        LSLS     R6,R6,#+1
        ORRS     R5,R6,R5, LSL #+3
        ORRS     R5,R5,#0x4
        MOVS     R3,R5
        B.N      ??can_lld_transmit_8
//  560   else
//  561     tir = ((uint32_t)ctfp->SID << 21) | ((uint32_t)ctfp->RTR << 1);
??can_lld_transmit_7:
        LDR      R5,[R2, #+4]
        LDRB     R6,[R2, #+0]
        UBFX     R6,R6,#+4,#+1
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        LSLS     R6,R6,#+1
        ORRS     R5,R6,R5, LSL #+21
        MOVS     R3,R5
//  562   tmbp->TDTR = ctfp->DLC;
??can_lld_transmit_8:
        LDRB     R5,[R2, #+0]
        ANDS     R5,R5,#0xF
        STR      R5,[R4, #+4]
//  563   tmbp->TDLR = ctfp->data32[0];
        LDR      R5,[R2, #+8]
        STR      R5,[R4, #+8]
//  564   tmbp->TDHR = ctfp->data32[1];
        LDR      R5,[R2, #+12]
        STR      R5,[R4, #+12]
//  565   tmbp->TIR  = tir | CAN_TI0R_TXRQ;
        ORRS     R5,R3,#0x1
        STR      R5,[R4, #+0]
//  566 }
??can_lld_transmit_6:
        POP      {R4-R6}
          CFI R4 SameValue
          CFI R5 SameValue
          CFI R6 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock17
//  567 
//  568 /**
//  569  * @brief   Determines whether a frame has been received.
//  570  *
//  571  * @param[in] canp      pointer to the @p CANDriver object
//  572  * @param[in] mailbox   mailbox number, @p CAN_ANY_MAILBOX for any mailbox
//  573  *
//  574  * @return              The queue space availability.
//  575  * @retval FALSE        no space in the transmit queue.
//  576  * @retval TRUE         transmit slot available.
//  577  *
//  578  * @notapi
//  579  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock18 Using cfiCommon0
          CFI Function can_lld_is_rx_nonempty
          CFI NoCalls
        THUMB
//  580 bool_t can_lld_is_rx_nonempty(CANDriver *canp, canmbx_t mailbox) {
can_lld_is_rx_nonempty:
        MOVS     R2,R0
//  581 
//  582   switch (mailbox) {
        CMP      R1,#+0
        BEQ.N    ??can_lld_is_rx_nonempty_0
        CMP      R1,#+2
        BEQ.N    ??can_lld_is_rx_nonempty_1
        BCC.N    ??can_lld_is_rx_nonempty_2
        B.N      ??can_lld_is_rx_nonempty_3
//  583   case CAN_ANY_MAILBOX:
//  584     return ((canp->can->RF0R & CAN_RF0R_FMP0) != 0 ||
//  585             (canp->can->RF1R & CAN_RF1R_FMP1) != 0);
??can_lld_is_rx_nonempty_0:
        LDR      R0,[R2, #+52]
        LDR      R0,[R0, #+12]
        TST      R0,#0x3
        BNE.N    ??can_lld_is_rx_nonempty_4
        LDR      R0,[R2, #+52]
        LDR      R0,[R0, #+16]
        TST      R0,#0x3
        BEQ.N    ??can_lld_is_rx_nonempty_5
??can_lld_is_rx_nonempty_4:
        MOVS     R0,#+1
        B.N      ??can_lld_is_rx_nonempty_6
??can_lld_is_rx_nonempty_5:
        MOVS     R0,#+0
??can_lld_is_rx_nonempty_6:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        B.N      ??can_lld_is_rx_nonempty_7
//  586   case 1:
//  587     return (canp->can->RF0R & CAN_RF0R_FMP0) != 0;
??can_lld_is_rx_nonempty_2:
        LDR      R0,[R2, #+52]
        LDR      R0,[R0, #+12]
        TST      R0,#0x3
        BEQ.N    ??can_lld_is_rx_nonempty_8
        MOVS     R0,#+1
        B.N      ??can_lld_is_rx_nonempty_9
??can_lld_is_rx_nonempty_8:
        MOVS     R0,#+0
??can_lld_is_rx_nonempty_9:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        B.N      ??can_lld_is_rx_nonempty_7
//  588   case 2:
//  589     return (canp->can->RF1R & CAN_RF1R_FMP1) != 0;
??can_lld_is_rx_nonempty_1:
        LDR      R0,[R2, #+52]
        LDR      R0,[R0, #+16]
        TST      R0,#0x3
        BEQ.N    ??can_lld_is_rx_nonempty_10
        MOVS     R0,#+1
        B.N      ??can_lld_is_rx_nonempty_11
??can_lld_is_rx_nonempty_10:
        MOVS     R0,#+0
??can_lld_is_rx_nonempty_11:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        B.N      ??can_lld_is_rx_nonempty_7
//  590   default:
//  591     return FALSE;
??can_lld_is_rx_nonempty_3:
        MOVS     R0,#+0
??can_lld_is_rx_nonempty_7:
        BX       LR               ;; return
          CFI EndBlock cfiBlock18
//  592   }
//  593 }
//  594 
//  595 /**
//  596  * @brief   Receives a frame from the input queue.
//  597  *
//  598  * @param[in] canp      pointer to the @p CANDriver object
//  599  * @param[in] mailbox   mailbox number, @p CAN_ANY_MAILBOX for any mailbox
//  600  * @param[out] crfp     pointer to the buffer where the CAN frame is copied
//  601  *
//  602  * @notapi
//  603  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock19 Using cfiCommon0
          CFI Function can_lld_receive
          CFI NoCalls
        THUMB
//  604 void can_lld_receive(CANDriver *canp,
//  605                      canmbx_t mailbox,
//  606                      CANRxFrame *crfp) {
can_lld_receive:
        PUSH     {R4-R6}
          CFI R6 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+12
//  607   uint32_t rir, rdtr;
//  608 
//  609   if (mailbox == CAN_ANY_MAILBOX) {
        CMP      R1,#+0
        BNE.N    ??can_lld_receive_0
//  610     if ((canp->can->RF0R & CAN_RF0R_FMP0) != 0)
        LDR      R5,[R0, #+52]
        LDR      R5,[R5, #+12]
        TST      R5,#0x3
        BEQ.N    ??can_lld_receive_1
//  611       mailbox = 1;
        MOVS     R5,#+1
        MOVS     R1,R5
        B.N      ??can_lld_receive_0
//  612     else if ((canp->can->RF1R & CAN_RF1R_FMP1) != 0)
??can_lld_receive_1:
        LDR      R5,[R0, #+52]
        LDR      R5,[R5, #+16]
        TST      R5,#0x3
        BEQ.N    ??can_lld_receive_2
//  613       mailbox = 2;
        MOVS     R5,#+2
        MOVS     R1,R5
        B.N      ??can_lld_receive_0
//  614     else {
//  615       /* Should not happen, do nothing.*/
//  616       return;
??can_lld_receive_2:
        B.N      ??can_lld_receive_3
//  617     }
//  618   }
//  619   switch (mailbox) {
??can_lld_receive_0:
        MOVS     R5,R1
        CMP      R5,#+1
        BEQ.N    ??can_lld_receive_4
        CMP      R5,#+2
        BEQ.N    ??can_lld_receive_5
        B.N      ??can_lld_receive_6
//  620   case 1:
//  621     /* Fetches the message.*/
//  622     rir  = canp->can->sFIFOMailBox[0].RIR;
??can_lld_receive_4:
        LDR      R5,[R0, #+52]
        LDR      R5,[R5, #+432]
        MOVS     R3,R5
//  623     rdtr = canp->can->sFIFOMailBox[0].RDTR;
        LDR      R5,[R0, #+52]
        LDR      R5,[R5, #+436]
        MOVS     R4,R5
//  624     crfp->data32[0] = canp->can->sFIFOMailBox[0].RDLR;
        LDR      R5,[R0, #+52]
        LDR      R5,[R5, #+440]
        STR      R5,[R2, #+12]
//  625     crfp->data32[1] = canp->can->sFIFOMailBox[0].RDHR;
        LDR      R5,[R0, #+52]
        LDR      R5,[R5, #+444]
        STR      R5,[R2, #+16]
//  626 
//  627     /* Releases the mailbox.*/
//  628     canp->can->RF0R = CAN_RF0R_RFOM0;
        LDR      R5,[R0, #+52]
        MOVS     R6,#+32
        STR      R6,[R5, #+12]
//  629 
//  630     /* If the queue is empty re-enables the interrupt in order to generate
//  631        events again.*/
//  632     if ((canp->can->RF0R & CAN_RF0R_FMP0) == 0)
        LDR      R5,[R0, #+52]
        LDR      R5,[R5, #+12]
        TST      R5,#0x3
        BNE.N    ??can_lld_receive_7
//  633       canp->can->IER |= CAN_IER_FMPIE0;
        LDR      R5,[R0, #+52]
        LDR      R5,[R5, #+20]
        ORRS     R5,R5,#0x2
        LDR      R6,[R0, #+52]
        STR      R5,[R6, #+20]
//  634     break;
??can_lld_receive_7:
        B.N      ??can_lld_receive_8
//  635   case 2:
//  636     /* Fetches the message.*/
//  637     rir  = canp->can->sFIFOMailBox[1].RIR;
??can_lld_receive_5:
        LDR      R5,[R0, #+52]
        LDR      R5,[R5, #+448]
        MOVS     R3,R5
//  638     rdtr = canp->can->sFIFOMailBox[1].RDTR;
        LDR      R5,[R0, #+52]
        LDR      R5,[R5, #+452]
        MOVS     R4,R5
//  639     crfp->data32[0] = canp->can->sFIFOMailBox[1].RDLR;
        LDR      R5,[R0, #+52]
        LDR      R5,[R5, #+456]
        STR      R5,[R2, #+12]
//  640     crfp->data32[1] = canp->can->sFIFOMailBox[1].RDHR;
        LDR      R5,[R0, #+52]
        LDR      R5,[R5, #+460]
        STR      R5,[R2, #+16]
//  641 
//  642     /* Releases the mailbox.*/
//  643     canp->can->RF1R = CAN_RF1R_RFOM1;
        LDR      R5,[R0, #+52]
        MOVS     R6,#+32
        STR      R6,[R5, #+16]
//  644 
//  645     /* If the queue is empty re-enables the interrupt in order to generate
//  646        events again.*/
//  647     if ((canp->can->RF1R & CAN_RF1R_FMP1) == 0)
        LDR      R5,[R0, #+52]
        LDR      R5,[R5, #+16]
        TST      R5,#0x3
        BNE.N    ??can_lld_receive_9
//  648       canp->can->IER |= CAN_IER_FMPIE1;
        LDR      R5,[R0, #+52]
        LDR      R5,[R5, #+20]
        ORRS     R5,R5,#0x10
        LDR      R6,[R0, #+52]
        STR      R5,[R6, #+20]
//  649     break;
??can_lld_receive_9:
        B.N      ??can_lld_receive_8
//  650   default:
//  651     /* Should not happen, do nothing.*/
//  652     return;
??can_lld_receive_6:
        B.N      ??can_lld_receive_3
//  653   }
//  654 
//  655   /* Decodes the various fields in the RX frame.*/
//  656   crfp->RTR = (rir & CAN_RI0R_RTR) >> 1;
??can_lld_receive_8:
        LSRS     R5,R3,#+1
        LDRB     R6,[R2, #+4]
        BFI      R6,R5,#+4,#+1
        STRB     R6,[R2, #+4]
//  657   crfp->IDE = (rir & CAN_RI0R_IDE) >> 2;
        LSRS     R5,R3,#+2
        LDRB     R6,[R2, #+4]
        BFI      R6,R5,#+5,#+1
        STRB     R6,[R2, #+4]
//  658   if (crfp->IDE)
        LDRB     R5,[R2, #+4]
        UBFX     R5,R5,#+5,#+1
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+0
        BEQ.N    ??can_lld_receive_10
//  659     crfp->EID = rir >> 3;
        LSRS     R5,R3,#+3
        LDR      R6,[R2, #+8]
        BFI      R6,R5,#+0,#+29
        STR      R6,[R2, #+8]
        B.N      ??can_lld_receive_11
//  660   else
//  661     crfp->SID = rir >> 21;
??can_lld_receive_10:
        LSRS     R5,R3,#+21
        LDR      R6,[R2, #+8]
        BFI      R6,R5,#+0,#+11
        STR      R6,[R2, #+8]
//  662   crfp->DLC = rdtr & CAN_RDT0R_DLC;
??can_lld_receive_11:
        MOVS     R5,R4
        LDRB     R6,[R2, #+4]
        BFI      R6,R5,#+0,#+4
        STRB     R6,[R2, #+4]
//  663   crfp->FMI = (uint8_t)(rdtr >> 8);
        LSRS     R5,R4,#+8
        STRB     R5,[R2, #+0]
//  664   crfp->TIME = (uint16_t)(rdtr >> 16);
        LSRS     R5,R4,#+16
        STRH     R5,[R2, #+2]
//  665 }
??can_lld_receive_3:
        POP      {R4-R6}
          CFI R4 SameValue
          CFI R5 SameValue
          CFI R6 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock19
//  666 
//  667 #if CAN_USE_SLEEP_MODE || defined(__DOXYGEN__)
//  668 /**
//  669  * @brief   Enters the sleep mode.
//  670  *
//  671  * @param[in] canp      pointer to the @p CANDriver object
//  672  *
//  673  * @notapi
//  674  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock20 Using cfiCommon0
          CFI Function can_lld_sleep
          CFI NoCalls
        THUMB
//  675 void can_lld_sleep(CANDriver *canp) {
//  676 
//  677   canp->can->MCR |= CAN_MCR_SLEEP;
can_lld_sleep:
        LDR      R1,[R0, #+52]
        LDR      R1,[R1, #+0]
        ORRS     R1,R1,#0x2
        LDR      R2,[R0, #+52]
        STR      R1,[R2, #+0]
//  678 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock20
//  679 
//  680 /**
//  681  * @brief   Enforces leaving the sleep mode.
//  682  *
//  683  * @param[in] canp      pointer to the @p CANDriver object
//  684  *
//  685  * @notapi
//  686  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock21 Using cfiCommon0
          CFI Function can_lld_wakeup
          CFI NoCalls
        THUMB
//  687 void can_lld_wakeup(CANDriver *canp) {
//  688 
//  689   canp->can->MCR &= ~CAN_MCR_SLEEP;
can_lld_wakeup:
        LDR      R1,[R0, #+52]
        LDR      R1,[R1, #+0]
        BICS     R1,R1,#0x2
        LDR      R2,[R0, #+52]
        STR      R1,[R2, #+0]
//  690 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock21
//  691 #endif /* CAN_USE_SLEEP_MODE */
//  692 
//  693 /**
//  694  * @brief   Programs the filters.
//  695  * @note    This is an STM32-specific API.
//  696  *
//  697  * @param[in] can2sb    number of the first filter assigned to CAN2
//  698  * @param[in] num       number of entries in the filters array, if zero then
//  699  *                      a default filter is programmed
//  700  * @param[in] cfp       pointer to the filters array, can be @p NULL if
//  701  *                      (num == 0)
//  702  *
//  703  * @api
//  704  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock22 Using cfiCommon0
          CFI Function canSTM32SetFilters
        THUMB
//  705 void canSTM32SetFilters(uint32_t can2sb, uint32_t num, const CANFilter *cfp) {
canSTM32SetFilters:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//  706 
//  707   chDbgCheck((can2sb >= 1) && (can2sb < STM32_CAN_MAX_FILTERS) &&
//  708              (num <= STM32_CAN_MAX_FILTERS),
//  709              "canSTM32SetFilters");
        CMP      R4,#+0
        BEQ.N    ??canSTM32SetFilters_0
        CMP      R4,#+28
        BCS.N    ??canSTM32SetFilters_0
        CMP      R5,#+29
        BCC.N    ??canSTM32SetFilters_1
??canSTM32SetFilters_0:
        MOVW     R2,#+709
        LDR.N    R1,??DataTable13_19
        LDR.N    R0,??DataTable13_20
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  710 
//  711 #if STM32_CAN_USE_CAN1
//  712   chDbgAssert(CAND1.state == CAN_STOP,
//  713               "canSTM32SetFilters(), #1", "invalid state");
??canSTM32SetFilters_1:
        LDR.N    R0,??DataTable13_9
        LDRB     R0,[R0, #+0]
        CMP      R0,#+1
        BEQ.N    ??canSTM32SetFilters_2
        LDR.N    R0,??DataTable13_21
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  714 #endif
//  715 #if STM32_CAN_USE_CAN2
//  716   chDbgAssert(CAND2.state == CAN_STOP,
//  717               "canSTM32SetFilters(), #2", "invalid state");
??canSTM32SetFilters_2:
        LDR.N    R0,??DataTable13_10
        LDRB     R0,[R0, #+0]
        CMP      R0,#+1
        BEQ.N    ??canSTM32SetFilters_3
        LDR.N    R0,??DataTable13_22
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  718 #endif
//  719 
//  720   can_lld_set_filters(can2sb, num, cfp);
??canSTM32SetFilters_3:
        MOVS     R2,R6
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall can_lld_set_filters
        BL       can_lld_set_filters
//  721 }
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock22

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13:
        DC32     0x40023840

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_1:
        DC32     0x40006600

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_2:
        DC32     0x4000661c

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_3:
        DC32     0x40006604

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_4:
        DC32     0x4000660c

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_5:
        DC32     0x40006614

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_6:
        DC32     0x40006640

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_7:
        DC32     0x40006644

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_8:
        DC32     0x10101

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_9:
        DC32     CAND1

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_10:
        DC32     CAND2

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_11:
        DC32     0x40006400

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_12:
        DC32     0x40006800

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_13:
        DC32     `?<Constant "can_lld_start(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_14:
        DC32     0x18f5b

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_15:
        DC32     `?<Constant "can_lld_stop(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_16:
        DC32     0x10002

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_17:
        DC32     0x40006414

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_18:
        DC32     0x40006814

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_19:
        DC32     `?<Constant "F:\\\\stuff\\\\rusefi_sourc...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_20:
        DC32     `?<Constant "\\"canSTM32SetFilters\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_21:
        DC32     `?<Constant "canSTM32SetFilters(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_22:
        DC32     `?<Constant "canSTM32SetFilters(), #2">`

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "can_lld_start(), #1">`:
        DATA
        DC8 "can_lld_start(), #1"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "can_lld_stop(), #1">`:
        DATA
        DC8 "can_lld_stop(), #1"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"canSTM32SetFilters\\"()">`:
        DATA
        DC8 "\"canSTM32SetFilters\"()"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "F:\\\\stuff\\\\rusefi_sourc...">`:
        DATA
        DC8 46H, 3AH, 5CH, 73H, 74H, 75H, 66H, 66H
        DC8 5CH, 72H, 75H, 73H, 65H, 66H, 69H, 5FH
        DC8 73H, 6FH, 75H, 72H, 63H, 65H, 66H, 6FH
        DC8 72H, 67H, 65H, 5CH, 66H, 69H, 72H, 6DH
        DC8 77H, 61H, 72H, 65H, 5CH, 63H, 68H, 69H
        DC8 62H, 69H, 6FH, 73H, 5CH, 6FH, 73H, 5CH
        DC8 68H, 61H, 6CH, 5CH, 70H, 6CH, 61H, 74H
        DC8 66H, 6FH, 72H, 6DH, 73H, 5CH, 53H, 54H
        DC8 4DH, 33H, 32H, 5CH, 63H, 61H, 6EH, 5FH
        DC8 6CH, 6CH, 64H, 2EH, 63H, 0
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "canSTM32SetFilters(), #1">`:
        DATA
        DC8 "canSTM32SetFilters(), #1"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "canSTM32SetFilters(), #2">`:
        DATA
        DC8 "canSTM32SetFilters(), #2"
        DC8 0, 0, 0

        END
//  722 
//  723 #endif /* HAL_USE_CAN */
//  724 
//  725 /** @} */
// 
//   112 bytes in section .bss
//   200 bytes in section .rodata
// 2 130 bytes in section .text
// 
// 2 130 bytes of CODE  memory
//   200 bytes of CONST memory
//   112 bytes of DATA  memory
//
//Errors: none
//Warnings: none
