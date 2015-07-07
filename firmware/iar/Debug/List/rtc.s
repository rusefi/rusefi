///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:26 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       src\rtc.c                                            /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       src\rtc.c -lCN F:\stuff\rusefi_sourceforge\firmware\ /
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
//                       rtc.s                                                /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME rtc

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN chDbgPanic3
        EXTERN dbg_check_lock
        EXTERN dbg_check_unlock
        EXTERN rtc_lld_get_alarm
        EXTERN rtc_lld_get_time
        EXTERN rtc_lld_get_time_fat
        EXTERN rtc_lld_init
        EXTERN rtc_lld_set_alarm
        EXTERN rtc_lld_set_time

        PUBLIC rtcGetAlarm
        PUBLIC rtcGetTime
        PUBLIC rtcGetTimeFat
        PUBLIC rtcInit
        PUBLIC rtcSetAlarm
        PUBLIC rtcSetTime
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\src\rtc.c
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
//   27 /*
//   28    Concepts and parts of this file have been contributed by Uladzimir Pylinsky
//   29    aka barthess.
//   30  */
//   31 
//   32 /**
//   33  * @file    rtc.c
//   34  * @brief   RTC Driver code.
//   35  *
//   36  * @addtogroup RTC
//   37  * @{
//   38  */
//   39 
//   40 #include "ch.h"
//   41 #include "hal.h"
//   42 
//   43 #if HAL_USE_RTC || defined(__DOXYGEN__)
//   44 
//   45 /*===========================================================================*/
//   46 /* Driver local definitions.                                                 */
//   47 /*===========================================================================*/
//   48 
//   49 /*===========================================================================*/
//   50 /* Driver exported variables.                                                */
//   51 /*===========================================================================*/
//   52 
//   53 /*===========================================================================*/
//   54 /* Driver local variables and types.                                         */
//   55 /*===========================================================================*/
//   56 
//   57 /*===========================================================================*/
//   58 /* Driver local functions.                                                   */
//   59 /*===========================================================================*/
//   60 
//   61 /*===========================================================================*/
//   62 /* Driver exported functions.                                                */
//   63 /*===========================================================================*/
//   64 
//   65 /**
//   66  * @brief   RTC Driver initialization.
//   67  * @note    This function is implicitly invoked by @p halInit(), there is
//   68  *          no need to explicitly initialize the driver.
//   69  *
//   70  * @init
//   71  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function rtcInit
        THUMB
//   72 void rtcInit(void) {
rtcInit:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   73 
//   74   rtc_lld_init();
          CFI FunCall rtc_lld_init
        BL       rtc_lld_init
//   75 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock0
//   76 
//   77 /**
//   78  * @brief   Set current time.
//   79  *
//   80  * @param[in] rtcp      pointer to RTC driver structure
//   81  * @param[in] timespec  pointer to a @p RTCTime structure
//   82  *
//   83  * @api
//   84  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function rtcSetTime
        THUMB
//   85 void rtcSetTime(RTCDriver *rtcp, const RTCTime *timespec) {
rtcSetTime:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//   86 
//   87   chDbgCheck((rtcp != NULL) && (timespec != NULL), "rtcSetTime");
        CMP      R4,#+0
        BEQ.N    ??rtcSetTime_0
        CMP      R5,#+0
        BNE.N    ??rtcSetTime_1
??rtcSetTime_0:
        MOVS     R2,#+87
        LDR.N    R1,??DataTable4
        LDR.N    R0,??DataTable4_1
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//   88 
//   89   chSysLock();
??rtcSetTime_1:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//   90   rtcSetTimeI(rtcp, timespec);
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall rtc_lld_set_time
        BL       rtc_lld_set_time
//   91   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//   92 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock1
//   93 
//   94 /**
//   95  * @brief   Get current time.
//   96  *
//   97  * @param[in] rtcp      pointer to RTC driver structure
//   98  * @param[out] timespec pointer to a @p RTCTime structure
//   99  *
//  100  * @api
//  101  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function rtcGetTime
        THUMB
//  102 void rtcGetTime(RTCDriver *rtcp, RTCTime *timespec) {
rtcGetTime:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  103 
//  104   chDbgCheck((rtcp != NULL) && (timespec != NULL), "rtcGetTime");
        CMP      R4,#+0
        BEQ.N    ??rtcGetTime_0
        CMP      R5,#+0
        BNE.N    ??rtcGetTime_1
??rtcGetTime_0:
        MOVS     R2,#+104
        LDR.N    R1,??DataTable4
        LDR.N    R0,??DataTable4_2
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  105 
//  106   chSysLock();
??rtcGetTime_1:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  107   rtcGetTimeI(rtcp, timespec);
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall rtc_lld_get_time
        BL       rtc_lld_get_time
//  108   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  109 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock2
//  110 
//  111 #if (RTC_ALARMS > 0) || defined(__DOXYGEN__)
//  112 /**
//  113  * @brief   Set alarm time.
//  114  *
//  115  * @param[in] rtcp      pointer to RTC driver structure
//  116  * @param[in] alarm     alarm identifier
//  117  * @param[in] alarmspec pointer to a @p RTCAlarm structure or @p NULL
//  118  *
//  119  * @api
//  120  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function rtcSetAlarm
        THUMB
//  121 void rtcSetAlarm(RTCDriver *rtcp,
//  122                  rtcalarm_t alarm,
//  123                  const RTCAlarm *alarmspec) {
rtcSetAlarm:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//  124 
//  125   chDbgCheck((rtcp != NULL) && (alarm < RTC_ALARMS), "rtcSetAlarm");
        CMP      R4,#+0
        BEQ.N    ??rtcSetAlarm_0
        CMP      R5,#+2
        BCC.N    ??rtcSetAlarm_1
??rtcSetAlarm_0:
        MOVS     R2,#+125
        LDR.N    R1,??DataTable4
        LDR.N    R0,??DataTable4_3
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  126 
//  127   chSysLock();
??rtcSetAlarm_1:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  128   rtcSetAlarmI(rtcp, alarm, alarmspec);
        MOVS     R2,R6
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall rtc_lld_set_alarm
        BL       rtc_lld_set_alarm
//  129   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  130 }
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock3
//  131 
//  132 /**
//  133  * @brief   Get current alarm.
//  134  * @note    If an alarm has not been set then the returned alarm specification
//  135  *          is not meaningful.
//  136  *
//  137  * @param[in] rtcp      pointer to RTC driver structure
//  138  * @param[in] alarm     alarm identifier
//  139  * @param[out] alarmspec pointer to a @p RTCAlarm structure
//  140  *
//  141  * @api
//  142  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function rtcGetAlarm
        THUMB
//  143 void rtcGetAlarm(RTCDriver *rtcp,
//  144                  rtcalarm_t alarm,
//  145                  RTCAlarm *alarmspec) {
rtcGetAlarm:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//  146 
//  147   chDbgCheck((rtcp != NULL) && (alarm < RTC_ALARMS) && (alarmspec != NULL),
//  148              "rtcGetAlarm");
        CMP      R4,#+0
        BEQ.N    ??rtcGetAlarm_0
        CMP      R5,#+2
        BCS.N    ??rtcGetAlarm_0
        CMP      R6,#+0
        BNE.N    ??rtcGetAlarm_1
??rtcGetAlarm_0:
        MOVS     R2,#+148
        LDR.N    R1,??DataTable4
        LDR.N    R0,??DataTable4_4
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  149 
//  150   chSysLock();
??rtcGetAlarm_1:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  151   rtcGetAlarmI(rtcp, alarm, alarmspec);
        MOVS     R2,R6
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall rtc_lld_get_alarm
        BL       rtc_lld_get_alarm
//  152   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  153 }
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock4
//  154 #endif /* RTC_ALARMS > 0 */
//  155 
//  156 #if RTC_SUPPORTS_CALLBACKS || defined(__DOXYGEN__)
//  157 /**
//  158  * @brief   Enables or disables RTC callbacks.
//  159  * @details This function enables or disables the callback, use a @p NULL
//  160  *          pointer in order to disable it.
//  161  *
//  162  * @param[in] rtcp      pointer to RTC driver structure
//  163  * @param[in] callback  callback function pointer or @p NULL
//  164  *
//  165  * @api
//  166  */
//  167 void rtcSetCallback(RTCDriver *rtcp, rtccb_t callback) {
//  168 
//  169   chDbgCheck((rtcp != NULL), "rtcSetCallback");
//  170 
//  171   chSysLock();
//  172   rtcSetCallbackI(rtcp, callback);
//  173   chSysUnlock();
//  174 }
//  175 #endif /* RTC_SUPPORTS_CALLBACKS */
//  176 
//  177 /**
//  178  * @brief   Get current time in format suitable for usage in FatFS.
//  179  *
//  180  * @param[in] rtcp      pointer to RTC driver structure
//  181  * @return              FAT time value.
//  182  *
//  183  * @api
//  184  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function rtcGetTimeFat
        THUMB
//  185 uint32_t rtcGetTimeFat(RTCDriver *rtcp) {
rtcGetTimeFat:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  186 
//  187   chDbgCheck((rtcp != NULL), "rtcSetTime");
        CMP      R4,#+0
        BNE.N    ??rtcGetTimeFat_0
        MOVS     R2,#+187
        LDR.N    R1,??DataTable4
        LDR.N    R0,??DataTable4_1
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  188   return rtc_lld_get_time_fat(rtcp);
??rtcGetTimeFat_0:
        MOVS     R0,R4
          CFI FunCall rtc_lld_get_time_fat
        BL       rtc_lld_get_time_fat
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock5
//  189 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4:
        DC32     `?<Constant "F:\\\\stuff\\\\rusefi_sourc...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_1:
        DC32     `?<Constant "\\"rtcSetTime\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_2:
        DC32     `?<Constant "\\"rtcGetTime\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_3:
        DC32     `?<Constant "\\"rtcSetAlarm\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_4:
        DC32     `?<Constant "\\"rtcGetAlarm\\"()">`

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"rtcSetTime\\"()">`:
        DATA
        DC8 "\"rtcSetTime\"()"
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
        DC8 68H, 61H, 6CH, 5CH, 73H, 72H, 63H, 5CH
        DC8 72H, 74H, 63H, 2EH, 63H, 0
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"rtcGetTime\\"()">`:
        DATA
        DC8 "\"rtcGetTime\"()"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"rtcSetAlarm\\"()">`:
        DATA
        DC8 "\"rtcSetAlarm\"()"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"rtcGetAlarm\\"()">`:
        DATA
        DC8 "\"rtcGetAlarm\"()"

        END
//  190 
//  191 #endif /* HAL_USE_RTC */
//  192 
//  193 /** @} */
// 
// 128 bytes in section .rodata
// 282 bytes in section .text
// 
// 282 bytes of CODE  memory
// 128 bytes of CONST memory
//
//Errors: none
//Warnings: none
