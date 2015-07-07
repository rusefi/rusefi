///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:10:00 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       platforms\STM32\RTCv2\rtc_lld.c                      /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       platforms\STM32\RTCv2\rtc_lld.c -lCN                 /
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
//                       rtc_lld.s                                            /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME rtc_lld

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN chDbgPanic3
        EXTERN dbg_check_lock
        EXTERN dbg_check_unlock

        PUBLIC RTCD1
        PUBLIC rtcGetPeriodicWakeup_v2
        PUBLIC rtcSetPeriodicWakeup_v2
        PUBLIC rtc_lld_get_alarm
        PUBLIC rtc_lld_get_time
        PUBLIC rtc_lld_get_time_fat
        PUBLIC rtc_lld_init
        PUBLIC rtc_lld_set_alarm
        PUBLIC rtc_lld_set_time
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\platforms\STM32\RTCv2\rtc_lld.c
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
//   16 /*
//   17    Concepts and parts of this file have been contributed by Uladzimir Pylinsky
//   18    aka barthess.
//   19  */
//   20 
//   21 /**
//   22  * @file    STM32/RTCv2/rtc_lld.c
//   23  * @brief   RTC low level driver.
//   24  *
//   25  * @addtogroup RTC
//   26  * @{
//   27  */
//   28 
//   29 #include "ch.h"
//   30 #include "hal.h"
//   31 
//   32 #if HAL_USE_RTC || defined(__DOXYGEN__)
//   33 
//   34 /*===========================================================================*/
//   35 /* Driver local definitions.                                                 */
//   36 /*===========================================================================*/
//   37 
//   38 /*===========================================================================*/
//   39 /* Driver exported variables.                                                */
//   40 /*===========================================================================*/
//   41 
//   42 /**
//   43  * @brief RTC driver identifier.
//   44  */

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   45 RTCDriver RTCD1;
RTCD1:
        DS8 4
//   46 
//   47 /*===========================================================================*/
//   48 /* Driver local variables and types.                                         */
//   49 /*===========================================================================*/
//   50 
//   51 /*===========================================================================*/
//   52 /* Driver local functions.                                                   */
//   53 /*===========================================================================*/
//   54 
//   55 /**
//   56  * @brief   Wait for synchronization of RTC registers with APB1 bus.
//   57  * @details This function must be invoked before trying to read RTC registers.
//   58  *
//   59  * @notapi
//   60  */
//   61 #define rtc_lld_apb1_sync() {                                                \ 
//   62     int counter = 0;                                                         \ 
//   63 	while ((RTCD1.id_rtc->ISR & RTC_ISR_RSF) == 0 && ++counter <LSE_TIMEOUT);\ 
//   64 	}
//   65 
//   66 /**
//   67  * @brief   Beginning of configuration procedure.
//   68  *
//   69  * @notapi
//   70  */
//   71 #define rtc_lld_enter_init() {                                              \ 
//   72   RTCD1.id_rtc->ISR |= RTC_ISR_INIT;                                        \ 
//   73   int counter = 0;                                                          \ 
//   74   while ((RTCD1.id_rtc->ISR & RTC_ISR_INITF) == 0 && ++counter <LSE_TIMEOUT)\ 
//   75     ;                                                                       \ 
//   76 }
//   77 
//   78 /**
//   79  * @brief   Finalizing of configuration procedure.
//   80  *
//   81  * @notapi
//   82  */
//   83 #define rtc_lld_exit_init() {RTCD1.id_rtc->ISR &= ~RTC_ISR_INIT;}
//   84 
//   85 /*===========================================================================*/
//   86 /* Driver interrupt handlers.                                                */
//   87 /*===========================================================================*/
//   88 
//   89 /*===========================================================================*/
//   90 /* Driver exported functions.                                                */
//   91 /*===========================================================================*/
//   92 
//   93 /**
//   94  * @brief   Enable access to registers.
//   95  *
//   96  * @api
//   97  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function rtc_lld_init
          CFI NoCalls
        THUMB
//   98 void rtc_lld_init(void){
//   99   RTCD1.id_rtc = RTC;
rtc_lld_init:
        LDR.N    R1,??DataTable3
        LDR.N    R2,??DataTable3_1  ;; 0x40002800
        STR      R2,[R1, #+0]
//  100 
//  101   /* Asynchronous part of preloader. Set it to maximum value. */
//  102   uint32_t prediv_a = 0x7F;
        MOVS     R1,#+127
//  103 
//  104   /* Disable write protection. */
//  105   RTCD1.id_rtc->WPR = 0xCA;
        LDR.N    R2,??DataTable3
        LDR      R2,[R2, #+0]
        MOVS     R3,#+202
        STR      R3,[R2, #+36]
//  106   RTCD1.id_rtc->WPR = 0x53;
        LDR.N    R2,??DataTable3
        LDR      R2,[R2, #+0]
        MOVS     R3,#+83
        STR      R3,[R2, #+36]
//  107 
//  108   /* If calendar not init yet. */
//  109   if (!(RTC->ISR & RTC_ISR_INITS)){
        LDR.N    R2,??DataTable3_2  ;; 0x4000280c
        LDR      R2,[R2, #+0]
        LSLS     R2,R2,#+27
        BMI.N    ??rtc_lld_init_0
//  110     rtc_lld_enter_init();
        LDR.N    R2,??DataTable3
        LDR      R2,[R2, #+0]
        LDR      R2,[R2, #+12]
        ORRS     R2,R2,#0x80
        LDR.N    R3,??DataTable3
        LDR      R3,[R3, #+0]
        STR      R2,[R3, #+12]
        MOVS     R2,#+0
        MOVS     R0,R2
??rtc_lld_init_1:
        LDR.N    R2,??DataTable3
        LDR      R2,[R2, #+0]
        LDR      R2,[R2, #+12]
        LSLS     R2,R2,#+25
        BMI.N    ??rtc_lld_init_2
        ADDS     R0,R0,#+1
        LDR.N    R2,??DataTable3_3  ;; 0xf4240
        CMP      R0,R2
        BLT.N    ??rtc_lld_init_1
//  111 
//  112     /* Prescaler register must be written in two SEPARATE writes. */
//  113     prediv_a = (prediv_a << 16) |
//  114                 (((STM32_RTCCLK / (prediv_a + 1)) - 1) & 0x7FFF);
??rtc_lld_init_2:
        MOV      R0,#+32768
        ADDS     R2,R1,#+1
        UDIV     R0,R0,R2
        SUBS     R0,R0,#+1
        LSLS     R0,R0,#+17       ;; ZeroExtS R0,R0,#+17,#+17
        LSRS     R0,R0,#+17
        ORRS     R1,R0,R1, LSL #+16
//  115     RTCD1.id_rtc->PRER = prediv_a;
        LDR.N    R0,??DataTable3
        LDR      R0,[R0, #+0]
        STR      R1,[R0, #+16]
//  116     RTCD1.id_rtc->PRER = prediv_a;
        LDR.N    R0,??DataTable3
        LDR      R0,[R0, #+0]
        STR      R1,[R0, #+16]
//  117     rtc_lld_exit_init();
        LDR.N    R0,??DataTable3
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+12]
        BICS     R0,R0,#0x80
        LDR.N    R2,??DataTable3
        LDR      R2,[R2, #+0]
        STR      R0,[R2, #+12]
//  118   }
//  119 }
??rtc_lld_init_0:
        BX       LR               ;; return
          CFI EndBlock cfiBlock0
//  120 
//  121 /**
//  122  * @brief   Set current time.
//  123  * @note    Fractional part will be silently ignored. There is no possibility
//  124  *          to set it on STM32 platform.
//  125  *
//  126  * @param[in] rtcp      pointer to RTC driver structure
//  127  * @param[in] timespec  pointer to a @p RTCTime structure
//  128  *
//  129  * @api
//  130  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function rtc_lld_set_time
          CFI NoCalls
        THUMB
//  131 void rtc_lld_set_time(RTCDriver *rtcp, const RTCTime *timespec) {
rtc_lld_set_time:
        PUSH     {R4}
          CFI R4 Frame(CFA, -4)
          CFI CFA R13+4
//  132   (void)rtcp;
//  133 
//  134   rtc_lld_enter_init();
        LDR.N    R3,??DataTable3
        LDR      R3,[R3, #+0]
        LDR      R3,[R3, #+12]
        ORRS     R3,R3,#0x80
        LDR.N    R4,??DataTable3
        LDR      R4,[R4, #+0]
        STR      R3,[R4, #+12]
        MOVS     R3,#+0
        MOVS     R2,R3
??rtc_lld_set_time_0:
        LDR.N    R3,??DataTable3
        LDR      R3,[R3, #+0]
        LDR      R3,[R3, #+12]
        LSLS     R3,R3,#+25
        BMI.N    ??rtc_lld_set_time_1
        ADDS     R2,R2,#+1
        LDR.N    R3,??DataTable3_3  ;; 0xf4240
        CMP      R2,R3
        BLT.N    ??rtc_lld_set_time_0
//  135   if (timespec->h12)
??rtc_lld_set_time_1:
        LDRB     R2,[R1, #+8]
        CMP      R2,#+0
        BEQ.N    ??rtc_lld_set_time_2
//  136     RTCD1.id_rtc->CR |= RTC_CR_FMT;
        LDR.N    R2,??DataTable3
        LDR      R2,[R2, #+0]
        LDR      R2,[R2, #+8]
        ORRS     R2,R2,#0x40
        LDR.N    R3,??DataTable3
        LDR      R3,[R3, #+0]
        STR      R2,[R3, #+8]
        B.N      ??rtc_lld_set_time_3
//  137   else
//  138     RTCD1.id_rtc->CR &= ~RTC_CR_FMT;
??rtc_lld_set_time_2:
        LDR.N    R2,??DataTable3
        LDR      R2,[R2, #+0]
        LDR      R2,[R2, #+8]
        BICS     R2,R2,#0x40
        LDR.N    R3,??DataTable3
        LDR      R3,[R3, #+0]
        STR      R2,[R3, #+8]
//  139   RTCD1.id_rtc->TR = timespec->tv_time;
??rtc_lld_set_time_3:
        LDR      R2,[R1, #+4]
        LDR.N    R3,??DataTable3
        LDR      R3,[R3, #+0]
        STR      R2,[R3, #+0]
//  140   RTCD1.id_rtc->DR = timespec->tv_date;
        LDR.N    R2,??DataTable3
        LDR      R2,[R2, #+0]
        LDR      R3,[R1, #+0]
        STR      R3,[R2, #+4]
//  141   rtc_lld_exit_init();
        LDR.N    R2,??DataTable3
        LDR      R2,[R2, #+0]
        LDR      R2,[R2, #+12]
        BICS     R2,R2,#0x80
        LDR.N    R3,??DataTable3
        LDR      R3,[R3, #+0]
        STR      R2,[R3, #+12]
//  142 }
        POP      {R4}
          CFI R4 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock1
//  143 
//  144 /**
//  145  * @brief   Get current time.
//  146  *
//  147  * @param[in] rtcp      pointer to RTC driver structure
//  148  * @param[out] timespec pointer to a @p RTCTime structure
//  149  *
//  150  * @api
//  151  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function rtc_lld_get_time
          CFI NoCalls
        THUMB
//  152 void rtc_lld_get_time(RTCDriver *rtcp, RTCTime *timespec) {
rtc_lld_get_time:
        PUSH     {R4}
          CFI R4 Frame(CFA, -4)
          CFI CFA R13+4
//  153   (void)rtcp;
//  154 
//  155   rtc_lld_apb1_sync();
        MOVS     R3,#+0
        MOVS     R2,R3
??rtc_lld_get_time_0:
        LDR.N    R3,??DataTable3
        LDR      R3,[R3, #+0]
        LDR      R3,[R3, #+12]
        LSLS     R3,R3,#+26
        BMI.N    ??rtc_lld_get_time_1
        ADDS     R2,R2,#+1
        LDR.N    R3,??DataTable3_3  ;; 0xf4240
        CMP      R2,R3
        BLT.N    ??rtc_lld_get_time_0
//  156 
//  157 #if STM32_RTC_HAS_SUBSECONDS
//  158   {
//  159     uint32_t prer = RTCD1.id_rtc->PRER & 0x7FFF;
??rtc_lld_get_time_1:
        LDR.N    R2,??DataTable3
        LDR      R2,[R2, #+0]
        LDR      R2,[R2, #+16]
        LSLS     R2,R2,#+17       ;; ZeroExtS R2,R2,#+17,#+17
        LSRS     R2,R2,#+17
//  160     uint32_t ssr  = RTCD1.id_rtc->SSR;
        LDR.N    R3,??DataTable3
        LDR      R3,[R3, #+0]
        LDR      R3,[R3, #+40]
//  161     timespec->tv_msec = (1000 * (prer - ssr)) / (prer + 1);
        SUBS     R3,R2,R3
        MOV      R4,#+1000
        MULS     R3,R4,R3
        ADDS     R2,R2,#+1
        UDIV     R2,R3,R2
        STR      R2,[R1, #+12]
//  162   }
//  163 #endif /* STM32_RTC_HAS_SUBSECONDS */
//  164   timespec->tv_time = RTCD1.id_rtc->TR;
        LDR.N    R2,??DataTable3
        LDR      R2,[R2, #+0]
        LDR      R2,[R2, #+0]
        STR      R2,[R1, #+4]
//  165   timespec->tv_date = RTCD1.id_rtc->DR;
        LDR.N    R2,??DataTable3
        LDR      R2,[R2, #+0]
        LDR      R2,[R2, #+4]
        STR      R2,[R1, #+0]
//  166 }
        POP      {R4}
          CFI R4 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock2
//  167 
//  168 /**
//  169  * @brief     Set alarm time.
//  170  *
//  171  * @note      Default value after BKP domain reset for both comparators is 0.
//  172  * @note      Function does not performs any checks of alarm time validity.
//  173  *
//  174  * @param[in] rtcp      pointer to RTC driver structure
//  175  * @param[in] alarm     alarm identifier starting from zero
//  176  * @param[in] alarmspec pointer to a @p RTCAlarm structure
//  177  *
//  178  * @api
//  179  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function rtc_lld_set_alarm
          CFI NoCalls
        THUMB
//  180 void rtc_lld_set_alarm(RTCDriver *rtcp,
//  181                        rtcalarm_t alarm,
//  182                        const RTCAlarm *alarmspec) {
rtc_lld_set_alarm:
        PUSH     {R4}
          CFI R4 Frame(CFA, -4)
          CFI CFA R13+4
//  183 
//  184   if (alarm == 0) {
        CMP      R1,#+0
        BNE.N    ??rtc_lld_set_alarm_0
//  185     if (alarmspec != NULL){
        CMP      R2,#+0
        BEQ.N    ??rtc_lld_set_alarm_1
//  186       rtcp->id_rtc->CR &= ~RTC_CR_ALRAE;
        LDR      R3,[R0, #+0]
        LDR      R3,[R3, #+8]
        BICS     R3,R3,#0x100
        LDR      R4,[R0, #+0]
        STR      R3,[R4, #+8]
//  187       while(!(rtcp->id_rtc->ISR & RTC_ISR_ALRAWF))
??rtc_lld_set_alarm_2:
        LDR      R3,[R0, #+0]
        LDR      R3,[R3, #+12]
        LSLS     R3,R3,#+31
        BPL.N    ??rtc_lld_set_alarm_2
//  188         ;
//  189       rtcp->id_rtc->ALRMAR = alarmspec->tv_datetime;
        LDR      R3,[R0, #+0]
        LDR      R4,[R2, #+0]
        STR      R4,[R3, #+28]
//  190       rtcp->id_rtc->CR |= RTC_CR_ALRAE;
        LDR      R3,[R0, #+0]
        LDR      R3,[R3, #+8]
        ORRS     R3,R3,#0x100
        LDR      R4,[R0, #+0]
        STR      R3,[R4, #+8]
//  191       rtcp->id_rtc->CR |= RTC_CR_ALRAIE;
        LDR      R3,[R0, #+0]
        LDR      R3,[R3, #+8]
        ORRS     R3,R3,#0x1000
        LDR      R4,[R0, #+0]
        STR      R3,[R4, #+8]
        B.N      ??rtc_lld_set_alarm_3
//  192     }
//  193     else {
//  194       rtcp->id_rtc->CR &= ~RTC_CR_ALRAIE;
??rtc_lld_set_alarm_1:
        LDR      R3,[R0, #+0]
        LDR      R3,[R3, #+8]
        BICS     R3,R3,#0x1000
        LDR      R4,[R0, #+0]
        STR      R3,[R4, #+8]
//  195       rtcp->id_rtc->CR &= ~RTC_CR_ALRAE;
        LDR      R3,[R0, #+0]
        LDR      R3,[R3, #+8]
        BICS     R3,R3,#0x100
        LDR      R4,[R0, #+0]
        STR      R3,[R4, #+8]
        B.N      ??rtc_lld_set_alarm_3
//  196     }
//  197   }
//  198 #if RTC_ALARMS == 2
//  199   else{
//  200     if (alarmspec != NULL){
??rtc_lld_set_alarm_0:
        CMP      R2,#+0
        BEQ.N    ??rtc_lld_set_alarm_4
//  201       rtcp->id_rtc->CR &= ~RTC_CR_ALRBE;
        LDR      R3,[R0, #+0]
        LDR      R3,[R3, #+8]
        BICS     R3,R3,#0x200
        LDR      R4,[R0, #+0]
        STR      R3,[R4, #+8]
//  202       while(!(rtcp->id_rtc->ISR & RTC_ISR_ALRBWF))
??rtc_lld_set_alarm_5:
        LDR      R3,[R0, #+0]
        LDR      R3,[R3, #+12]
        LSLS     R3,R3,#+30
        BPL.N    ??rtc_lld_set_alarm_5
//  203         ;
//  204       rtcp->id_rtc->ALRMBR = alarmspec->tv_datetime;
        LDR      R3,[R0, #+0]
        LDR      R4,[R2, #+0]
        STR      R4,[R3, #+32]
//  205       rtcp->id_rtc->CR |= RTC_CR_ALRBE;
        LDR      R3,[R0, #+0]
        LDR      R3,[R3, #+8]
        ORRS     R3,R3,#0x200
        LDR      R4,[R0, #+0]
        STR      R3,[R4, #+8]
//  206       rtcp->id_rtc->CR |= RTC_CR_ALRBIE;
        LDR      R3,[R0, #+0]
        LDR      R3,[R3, #+8]
        ORRS     R3,R3,#0x2000
        LDR      R4,[R0, #+0]
        STR      R3,[R4, #+8]
        B.N      ??rtc_lld_set_alarm_3
//  207     }
//  208     else {
//  209       rtcp->id_rtc->CR &= ~RTC_CR_ALRBIE;
??rtc_lld_set_alarm_4:
        LDR      R3,[R0, #+0]
        LDR      R3,[R3, #+8]
        BICS     R3,R3,#0x2000
        LDR      R4,[R0, #+0]
        STR      R3,[R4, #+8]
//  210       rtcp->id_rtc->CR &= ~RTC_CR_ALRBE;
        LDR      R3,[R0, #+0]
        LDR      R3,[R3, #+8]
        BICS     R3,R3,#0x200
        LDR      R4,[R0, #+0]
        STR      R3,[R4, #+8]
//  211     }
//  212   }
//  213 #endif /* RTC_ALARMS == 2 */
//  214 }
??rtc_lld_set_alarm_3:
        POP      {R4}
          CFI R4 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock3
//  215 
//  216 /**
//  217  * @brief   Get alarm time.
//  218  *
//  219  * @param[in] rtcp       pointer to RTC driver structure
//  220  * @param[in] alarm      alarm identifier starting from zero
//  221  * @param[out] alarmspec pointer to a @p RTCAlarm structure
//  222  *
//  223  * @api
//  224  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function rtc_lld_get_alarm
          CFI NoCalls
        THUMB
//  225 void rtc_lld_get_alarm(RTCDriver *rtcp,
//  226                        rtcalarm_t alarm,
//  227                        RTCAlarm *alarmspec) {
//  228 
//  229   if (alarm == 0)
rtc_lld_get_alarm:
        CMP      R1,#+0
        BNE.N    ??rtc_lld_get_alarm_0
//  230     alarmspec->tv_datetime = rtcp->id_rtc->ALRMAR;
        LDR      R3,[R0, #+0]
        LDR      R3,[R3, #+28]
        STR      R3,[R2, #+0]
        B.N      ??rtc_lld_get_alarm_1
//  231 #if RTC_ALARMS == 2
//  232   else
//  233     alarmspec->tv_datetime = rtcp->id_rtc->ALRMBR;
??rtc_lld_get_alarm_0:
        LDR      R3,[R0, #+0]
        LDR      R3,[R3, #+32]
        STR      R3,[R2, #+0]
//  234 #endif
//  235 }
??rtc_lld_get_alarm_1:
        BX       LR               ;; return
          CFI EndBlock cfiBlock4
//  236 
//  237 /**
//  238  * @brief     Sets time of periodic wakeup.
//  239  *
//  240  * @note      Default value after BKP domain reset is 0x0000FFFF
//  241  *
//  242  * @param[in] rtcp       pointer to RTC driver structure
//  243  * @param[in] wakeupspec pointer to a @p RTCWakeup structure
//  244  *
//  245  * @api
//  246  */
//  247 #if RTC_HAS_PERIODIC_WAKEUPS

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function rtcSetPeriodicWakeup_v2
        THUMB
//  248 void rtcSetPeriodicWakeup_v2(RTCDriver *rtcp, const RTCWakeup *wakeupspec) {
rtcSetPeriodicWakeup_v2:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  249 
//  250   if (wakeupspec != NULL){
        CMP      R5,#+0
        BEQ.N    ??rtcSetPeriodicWakeup_v2_0
//  251     chDbgCheck((wakeupspec->wakeup != 0x30000),
//  252                "rtc_lld_set_periodic_wakeup, forbidden combination");
        LDR      R0,[R5, #+0]
        CMP      R0,#+196608
        BNE.N    ??rtcSetPeriodicWakeup_v2_1
        MOVS     R2,#+252
        LDR.N    R1,??DataTable3_4
        LDR.N    R0,??DataTable3_5
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  253 
//  254     rtcp->id_rtc->CR &= ~RTC_CR_WUTE;
??rtcSetPeriodicWakeup_v2_1:
        LDR      R0,[R4, #+0]
        LDR      R0,[R0, #+8]
        BICS     R0,R0,#0x400
        LDR      R1,[R4, #+0]
        STR      R0,[R1, #+8]
//  255     while(!(rtcp->id_rtc->ISR & RTC_ISR_WUTWF))
??rtcSetPeriodicWakeup_v2_2:
        LDR      R0,[R4, #+0]
        LDR      R0,[R0, #+12]
        LSLS     R0,R0,#+29
        BPL.N    ??rtcSetPeriodicWakeup_v2_2
//  256       ;
//  257     rtcp->id_rtc->WUTR = wakeupspec->wakeup & 0xFFFF;
        LDR      R0,[R4, #+0]
        LDR      R1,[R5, #+0]
        UXTH     R1,R1            ;; ZeroExt  R1,R1,#+16,#+16
        STR      R1,[R0, #+20]
//  258     rtcp->id_rtc->CR   = (wakeupspec->wakeup >> 16) & 0x7;
        LDR      R0,[R5, #+0]
        UBFX     R0,R0,#+16,#+3
        LDR      R1,[R4, #+0]
        STR      R0,[R1, #+8]
//  259     rtcp->id_rtc->CR |= RTC_CR_WUTIE;
        LDR      R0,[R4, #+0]
        LDR      R0,[R0, #+8]
        ORRS     R0,R0,#0x4000
        LDR      R1,[R4, #+0]
        STR      R0,[R1, #+8]
//  260     rtcp->id_rtc->CR |= RTC_CR_WUTE;
        LDR      R0,[R4, #+0]
        LDR      R0,[R0, #+8]
        ORRS     R0,R0,#0x400
        LDR      R1,[R4, #+0]
        STR      R0,[R1, #+8]
        B.N      ??rtcSetPeriodicWakeup_v2_3
//  261   }
//  262   else {
//  263     rtcp->id_rtc->CR &= ~RTC_CR_WUTIE;
??rtcSetPeriodicWakeup_v2_0:
        LDR      R0,[R4, #+0]
        LDR      R0,[R0, #+8]
        BICS     R0,R0,#0x4000
        LDR      R1,[R4, #+0]
        STR      R0,[R1, #+8]
//  264     rtcp->id_rtc->CR &= ~RTC_CR_WUTE;
        LDR      R0,[R4, #+0]
        LDR      R0,[R0, #+8]
        BICS     R0,R0,#0x400
        LDR      R1,[R4, #+0]
        STR      R0,[R1, #+8]
//  265   }
//  266 }
??rtcSetPeriodicWakeup_v2_3:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock5

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3:
        DC32     RTCD1

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_1:
        DC32     0x40002800

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_2:
        DC32     0x4000280c

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_3:
        DC32     0xf4240

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_4:
        DC32     `?<Constant "F:\\\\stuff\\\\rusefi_sourc...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_5:
        DC32     `?<Constant "\\"rtc_lld_set_periodic...">`
//  267 #endif /* RTC_HAS_PERIODIC_WAKEUPS */
//  268 
//  269 /**
//  270  * @brief     Gets time of periodic wakeup.
//  271  *
//  272  * @note      Default value after BKP domain reset is 0x0000FFFF
//  273  *
//  274  * @param[in] rtcp        pointer to RTC driver structure
//  275  * @param[out] wakeupspec pointer to a @p RTCWakeup structure
//  276  *
//  277  * @api
//  278  */
//  279 #if RTC_HAS_PERIODIC_WAKEUPS

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function rtcGetPeriodicWakeup_v2
          CFI NoCalls
        THUMB
//  280 void rtcGetPeriodicWakeup_v2(RTCDriver *rtcp, RTCWakeup *wakeupspec) {
//  281 
//  282   wakeupspec->wakeup  = 0;
rtcGetPeriodicWakeup_v2:
        MOVS     R2,#+0
        STR      R2,[R1, #+0]
//  283   wakeupspec->wakeup |= rtcp->id_rtc->WUTR;
        LDR      R2,[R1, #+0]
        LDR      R3,[R0, #+0]
        LDR      R3,[R3, #+20]
        ORRS     R2,R3,R2
        STR      R2,[R1, #+0]
//  284   wakeupspec->wakeup |= (((uint32_t)rtcp->id_rtc->CR) & 0x7) << 16;
        LDR      R2,[R1, #+0]
        LDR      R3,[R0, #+0]
        LDR      R3,[R3, #+8]
        ANDS     R3,R3,#0x7
        ORRS     R2,R2,R3, LSL #+16
        STR      R2,[R1, #+0]
//  285 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock6
//  286 #endif /* RTC_HAS_PERIODIC_WAKEUPS */
//  287 
//  288 /**
//  289  * @brief   Get current time in format suitable for usage in FatFS.
//  290  *
//  291  * @param[in] rtcp      pointer to RTC driver structure
//  292  * @return              FAT time value.
//  293  *
//  294  * @api
//  295  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function rtc_lld_get_time_fat
        THUMB
//  296 uint32_t rtc_lld_get_time_fat(RTCDriver *rtcp) {
rtc_lld_get_time_fat:
        PUSH     {R0-R8,LR}
          CFI R14 Frame(CFA, -4)
          CFI R8 Frame(CFA, -8)
          CFI R7 Frame(CFA, -12)
          CFI R6 Frame(CFA, -16)
          CFI R5 Frame(CFA, -20)
          CFI R4 Frame(CFA, -24)
          CFI CFA R13+40
        MOVS     R4,R0
//  297   uint32_t fattime;
//  298   RTCTime timespec;
//  299   uint32_t tv_time;
//  300   uint32_t tv_date;
//  301   uint32_t v;
//  302 
//  303   chSysLock();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  304   rtcGetTimeI(rtcp, &timespec);
        ADD      R1,SP,#+0
        MOVS     R0,R4
          CFI FunCall rtc_lld_get_time
        BL       rtc_lld_get_time
//  305   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  306 
//  307   tv_time = timespec.tv_time;
        LDR      R0,[SP, #+4]
        MOVS     R6,R0
//  308   tv_date = timespec.tv_date;
        LDR      R0,[SP, #+0]
        MOVS     R7,R0
//  309 
//  310   v =  (tv_time & RTC_TR_SU) >> RTC_TR_SU_OFFSET;
        ANDS     R0,R6,#0xF
        MOV      R8,R0
//  311   v += ((tv_time & RTC_TR_ST) >> RTC_TR_ST_OFFSET) * 10;
        UBFX     R0,R6,#+4,#+3
        MOVS     R1,#+10
        MLA      R8,R1,R0,R8
//  312   fattime  = v >> 1;
        LSRS     R0,R8,#+1
        MOVS     R5,R0
//  313 
//  314   v =  (tv_time & RTC_TR_MNU) >> RTC_TR_MNU_OFFSET;
        UBFX     R0,R6,#+8,#+4
        MOV      R8,R0
//  315   v += ((tv_time & RTC_TR_MNT) >> RTC_TR_MNT_OFFSET) * 10;
        UBFX     R0,R6,#+12,#+3
        MOVS     R1,#+10
        MLA      R8,R1,R0,R8
//  316   fattime |= v << 5;
        ORRS     R5,R5,R8, LSL #+5
//  317 
//  318   v =  (tv_time & RTC_TR_HU) >> RTC_TR_HU_OFFSET;
        UBFX     R0,R6,#+16,#+4
        MOV      R8,R0
//  319   v += ((tv_time & RTC_TR_HT) >> RTC_TR_HT_OFFSET) * 10;
        UBFX     R0,R6,#+20,#+2
        MOVS     R1,#+10
        MLA      R8,R1,R0,R8
//  320   v += 12 * ((tv_time & RTC_TR_PM) >> RTC_TR_PM_OFFSET);
        UBFX     R0,R6,#+22,#+1
        MOVS     R1,#+12
        MLA      R8,R1,R0,R8
//  321   fattime |= v << 11;
        ORRS     R5,R5,R8, LSL #+11
//  322 
//  323   v =  (tv_date & RTC_DR_DU) >> RTC_DR_DU_OFFSET;
        ANDS     R0,R7,#0xF
        MOV      R8,R0
//  324   v += ((tv_date & RTC_DR_DT) >> RTC_DR_DT_OFFSET) * 10;
        UBFX     R0,R7,#+4,#+2
        MOVS     R1,#+10
        MLA      R8,R1,R0,R8
//  325   fattime |= v << 16;
        ORRS     R5,R5,R8, LSL #+16
//  326 
//  327   v =  (tv_date & RTC_DR_MU) >> RTC_DR_MU_OFFSET;
        UBFX     R0,R7,#+8,#+4
        MOV      R8,R0
//  328   v += ((tv_date & RTC_DR_MT) >> RTC_DR_MT_OFFSET) * 10;
        UBFX     R0,R7,#+12,#+1
        MOVS     R1,#+10
        MLA      R8,R1,R0,R8
//  329   fattime |= v << 21;
        ORRS     R5,R5,R8, LSL #+21
//  330 
//  331   v =  (tv_date & RTC_DR_YU) >> RTC_DR_YU_OFFSET;
        UBFX     R0,R7,#+16,#+4
        MOV      R8,R0
//  332   v += ((tv_date & RTC_DR_YT) >> RTC_DR_YT_OFFSET) * 10;
        UBFX     R0,R7,#+20,#+4
        MOVS     R1,#+10
        MLA      R8,R1,R0,R8
//  333   v += 2000 - 1900 - 80;
        ADDS     R8,R8,#+20
//  334   fattime |= v << 25;
        ORRS     R5,R5,R8, LSL #+25
//  335 
//  336   return fattime;
        MOVS     R0,R5
        ADD      SP,SP,#+16
          CFI CFA R13+24
        POP      {R4-R8,PC}       ;; return
          CFI EndBlock cfiBlock7
//  337 }

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"rtc_lld_set_periodic...">`:
        DATA
        DC8 22H, 72H, 74H, 63H, 5FH, 6CH, 6CH, 64H
        DC8 5FH, 73H, 65H, 74H, 5FH, 70H, 65H, 72H
        DC8 69H, 6FH, 64H, 69H, 63H, 5FH, 77H, 61H
        DC8 6BH, 65H, 75H, 70H, 2CH, 20H, 66H, 6FH
        DC8 72H, 62H, 69H, 64H, 64H, 65H, 6EH, 20H
        DC8 63H, 6FH, 6DH, 62H, 69H, 6EH, 61H, 74H
        DC8 69H, 6FH, 6EH, 22H, 28H, 29H, 0
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
        DC8 4DH, 33H, 32H, 5CH, 52H, 54H, 43H, 76H
        DC8 32H, 5CH, 72H, 74H, 63H, 5FH, 6CH, 6CH
        DC8 64H, 2EH, 63H, 0

        END
//  338 
//  339 #endif /* HAL_USE_RTC */
//  340 
//  341 /** @} */
// 
//   4 bytes in section .bss
// 140 bytes in section .rodata
// 864 bytes in section .text
// 
// 864 bytes of CODE  memory
// 140 bytes of CONST memory
//   4 bytes of DATA  memory
//
//Errors: none
//Warnings: none
