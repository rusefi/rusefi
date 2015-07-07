///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:42 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\vari /
//                       ous\chrtclib.c                                       /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\vari /
//                       ous\chrtclib.c -lCN F:\stuff\rusefi_sourceforge\firm /
//                       ware\iar\Debug\List\ -lA F:\stuff\rusefi_sourceforge /
//                       \firmware\iar\Debug\List\ -o                         /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\Debug\Obj\  /
//                       --no_cse --no_unroll --no_inline --no_code_motion    /
//                       --no_tbaa --no_clustering --no_scheduling --debug    /
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
//                       chrtclib.s                                           /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME chrtclib

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN __aeabi_memclr4
        EXTERN __localtime32
        EXTERN __mktime32
        EXTERN memcpy
        EXTERN rtcGetTime
        EXTERN rtcSetTime

        PUBLIC rtcGetTimeTm
        PUBLIC rtcGetTimeUnixSec
        PUBLIC rtcGetTimeUnixUsec
        PUBLIC rtcSetTimeTm
        PUBLIC rtcSetTimeUnixSec
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\various\chrtclib.c
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
//   22  * @file    chrtclib.c
//   23  * @brief   RTC time conversion utilities code.
//   24  *
//   25  * @addtogroup chrtclib
//   26  * @{
//   27  */
//   28 
//   29 #include <time.h>
//   30 
//   31 #include "ch.h"
//   32 #include "hal.h"
//   33 
//   34 #include "chrtclib.h"
//   35 
//   36 #if HAL_USE_RTC || defined(__DOXYGEN__)
//   37 
//   38 #if (defined(STM32F4XX) || defined(STM32F2XX) || defined(STM32L1XX) ||        \ 
//   39      defined(STM32F30X) || defined(STM32F37X) ||                              \ 
//   40      defined(STM32F1XX) || defined(STM32F10X_MD) || defined(STM32F10X_LD) ||  \ 
//   41      defined(STM32F10X_HD) || defined(STM32F10X_CL) || defined(STM32F0XX) ||  \ 
//   42      defined(LPC122X) || defined(__DOXYGEN__))
//   43 #if STM32_RTC_IS_CALENDAR
//   44 /**
//   45  * @brief   Converts from STM32 BCD to canonicalized time format.
//   46  *
//   47  * @param[out] timp     pointer to a @p tm structure as defined in time.h
//   48  * @param[in] timespec  pointer to a @p RTCTime structure
//   49  *
//   50  * @notapi
//   51  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function stm32_rtc_bcd2tm
          CFI NoCalls
        THUMB
//   52 static void stm32_rtc_bcd2tm(struct tm *timp, RTCTime *timespec) {
stm32_rtc_bcd2tm:
        PUSH     {R4-R6}
          CFI R6 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+12
//   53   uint32_t tv_time = timespec->tv_time;
        LDR      R2,[R1, #+4]
//   54   uint32_t tv_date = timespec->tv_date;
        LDR      R3,[R1, #+0]
//   55 
//   56 #if CH_DBG_ENABLE_CHECKS
//   57   timp->tm_isdst = 0;
        MOVS     R4,#+0
        STR      R4,[R0, #+32]
//   58   timp->tm_wday  = 0;
        MOVS     R4,#+0
        STR      R4,[R0, #+24]
//   59   timp->tm_mday  = 0;
        MOVS     R4,#+0
        STR      R4,[R0, #+12]
//   60   timp->tm_yday  = 0;
        MOVS     R4,#+0
        STR      R4,[R0, #+28]
//   61   timp->tm_mon   = 0;
        MOVS     R4,#+0
        STR      R4,[R0, #+16]
//   62   timp->tm_year  = 0;
        MOVS     R4,#+0
        STR      R4,[R0, #+20]
//   63   timp->tm_sec   = 0;
        MOVS     R4,#+0
        STR      R4,[R0, #+0]
//   64   timp->tm_min   = 0;
        MOVS     R4,#+0
        STR      R4,[R0, #+4]
//   65   timp->tm_hour  = 0;
        MOVS     R4,#+0
        STR      R4,[R0, #+8]
//   66 #endif
//   67 
//   68   timp->tm_isdst = -1;
        MOVS     R4,#-1
        STR      R4,[R0, #+32]
//   69 
//   70   timp->tm_wday = (tv_date & RTC_DR_WDU) >> RTC_DR_WDU_OFFSET;
        UBFX     R4,R3,#+13,#+3
        STR      R4,[R0, #+24]
//   71   if (timp->tm_wday == 7)
        LDR      R4,[R0, #+24]
        CMP      R4,#+7
        BNE.N    ??stm32_rtc_bcd2tm_0
//   72     timp->tm_wday = 0;
        MOVS     R4,#+0
        STR      R4,[R0, #+24]
//   73 
//   74   timp->tm_mday =  (tv_date & RTC_DR_DU) >> RTC_DR_DU_OFFSET;
??stm32_rtc_bcd2tm_0:
        ANDS     R4,R3,#0xF
        STR      R4,[R0, #+12]
//   75   timp->tm_mday += ((tv_date & RTC_DR_DT) >> RTC_DR_DT_OFFSET) * 10;
        LDR      R4,[R0, #+12]
        UBFX     R5,R3,#+4,#+2
        MOVS     R6,#+10
        MLA      R4,R6,R5,R4
        STR      R4,[R0, #+12]
//   76 
//   77   timp->tm_mon  =  (tv_date & RTC_DR_MU) >> RTC_DR_MU_OFFSET;
        UBFX     R4,R3,#+8,#+4
        STR      R4,[R0, #+16]
//   78   timp->tm_mon  += ((tv_date & RTC_DR_MT) >> RTC_DR_MT_OFFSET) * 10;
        LDR      R4,[R0, #+16]
        UBFX     R5,R3,#+12,#+1
        MOVS     R6,#+10
        MLA      R4,R6,R5,R4
        STR      R4,[R0, #+16]
//   79   timp->tm_mon  -= 1;
        LDR      R4,[R0, #+16]
        SUBS     R4,R4,#+1
        STR      R4,[R0, #+16]
//   80 
//   81   timp->tm_year =  (tv_date & RTC_DR_YU) >> RTC_DR_YU_OFFSET;
        UBFX     R4,R3,#+16,#+4
        STR      R4,[R0, #+20]
//   82   timp->tm_year += ((tv_date & RTC_DR_YT) >> RTC_DR_YT_OFFSET) * 10;
        LDR      R4,[R0, #+20]
        UBFX     R5,R3,#+20,#+4
        MOVS     R6,#+10
        MLA      R4,R6,R5,R4
        STR      R4,[R0, #+20]
//   83   timp->tm_year += 2000 - 1900;
        LDR      R4,[R0, #+20]
        ADDS     R4,R4,#+100
        STR      R4,[R0, #+20]
//   84 
//   85   timp->tm_sec  =  (tv_time & RTC_TR_SU) >> RTC_TR_SU_OFFSET;
        ANDS     R4,R2,#0xF
        STR      R4,[R0, #+0]
//   86   timp->tm_sec  += ((tv_time & RTC_TR_ST) >> RTC_TR_ST_OFFSET) * 10;
        LDR      R4,[R0, #+0]
        UBFX     R5,R2,#+4,#+3
        MOVS     R6,#+10
        MLA      R4,R6,R5,R4
        STR      R4,[R0, #+0]
//   87 
//   88   timp->tm_min  =  (tv_time & RTC_TR_MNU) >> RTC_TR_MNU_OFFSET;
        UBFX     R4,R2,#+8,#+4
        STR      R4,[R0, #+4]
//   89   timp->tm_min  += ((tv_time & RTC_TR_MNT) >> RTC_TR_MNT_OFFSET) * 10;
        LDR      R4,[R0, #+4]
        UBFX     R5,R2,#+12,#+3
        MOVS     R6,#+10
        MLA      R4,R6,R5,R4
        STR      R4,[R0, #+4]
//   90 
//   91   timp->tm_hour =  (tv_time & RTC_TR_HU) >> RTC_TR_HU_OFFSET;
        UBFX     R4,R2,#+16,#+4
        STR      R4,[R0, #+8]
//   92   timp->tm_hour += ((tv_time & RTC_TR_HT) >> RTC_TR_HT_OFFSET) * 10;
        LDR      R4,[R0, #+8]
        UBFX     R5,R2,#+20,#+2
        MOVS     R6,#+10
        MLA      R4,R6,R5,R4
        STR      R4,[R0, #+8]
//   93   timp->tm_hour += 12 * ((tv_time & RTC_TR_PM) >> RTC_TR_PM_OFFSET);
        LDR      R4,[R0, #+8]
        UBFX     R5,R2,#+22,#+1
        MOVS     R6,#+12
        MLA      R4,R6,R5,R4
        STR      R4,[R0, #+8]
//   94 }
        POP      {R4-R6}
          CFI R4 SameValue
          CFI R5 SameValue
          CFI R6 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock0
//   95 
//   96 /**
//   97  * @brief   Converts from canonicalized to STM32 BCD time format.
//   98  *
//   99  * @param[in] timp      pointer to a @p tm structure as defined in time.h
//  100  * @param[out] timespec pointer to a @p RTCTime structure
//  101  *
//  102  * @notapi
//  103  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function stm32_rtc_tm2bcd
          CFI NoCalls
        THUMB
//  104 static void stm32_rtc_tm2bcd(struct tm *timp, RTCTime *timespec) {
stm32_rtc_tm2bcd:
        PUSH     {R4,R5}
          CFI R5 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//  105   uint32_t v = 0;
        MOVS     R2,#+0
//  106 
//  107   timespec->tv_date = 0;
        MOVS     R3,#+0
        STR      R3,[R1, #+0]
//  108   timespec->tv_time = 0;
        MOVS     R3,#+0
        STR      R3,[R1, #+4]
//  109 
//  110   v = timp->tm_year - 100;
        LDR      R3,[R0, #+20]
        SUBS     R3,R3,#+100
        MOVS     R2,R3
//  111   timespec->tv_date |= ((v / 10) << RTC_DR_YT_OFFSET) & RTC_DR_YT;
        LDR      R3,[R1, #+0]
        MOVS     R4,#+10
        UDIV     R4,R2,R4
        LSLS     R4,R4,#+20
        ANDS     R4,R4,#0xF00000
        ORRS     R3,R4,R3
        STR      R3,[R1, #+0]
//  112   timespec->tv_date |= (v % 10) << RTC_DR_YU_OFFSET;
        MOVS     R3,#+10
        LDR      R4,[R1, #+0]
        UDIV     R5,R2,R3
        MLS      R3,R3,R5,R2
        ORRS     R3,R4,R3, LSL #+16
        STR      R3,[R1, #+0]
//  113 
//  114   if (timp->tm_wday == 0)
        LDR      R3,[R0, #+24]
        CMP      R3,#+0
        BNE.N    ??stm32_rtc_tm2bcd_0
//  115     v = 7;
        MOVS     R3,#+7
        MOVS     R2,R3
        B.N      ??stm32_rtc_tm2bcd_1
//  116   else
//  117     v = timp->tm_wday;
??stm32_rtc_tm2bcd_0:
        LDR      R3,[R0, #+24]
        MOVS     R2,R3
//  118   timespec->tv_date |= (v << RTC_DR_WDU_OFFSET) & RTC_DR_WDU;
??stm32_rtc_tm2bcd_1:
        LDR      R3,[R1, #+0]
        LSLS     R4,R2,#+13
        ANDS     R4,R4,#0xE000
        ORRS     R3,R4,R3
        STR      R3,[R1, #+0]
//  119 
//  120   v = timp->tm_mon + 1;
        LDR      R3,[R0, #+16]
        ADDS     R3,R3,#+1
        MOVS     R2,R3
//  121   timespec->tv_date |= ((v / 10) << RTC_DR_MT_OFFSET) & RTC_DR_MT;
        LDR      R3,[R1, #+0]
        MOVS     R4,#+10
        UDIV     R4,R2,R4
        LSLS     R4,R4,#+12
        ANDS     R4,R4,#0x1000
        ORRS     R3,R4,R3
        STR      R3,[R1, #+0]
//  122   timespec->tv_date |= (v % 10) << RTC_DR_MU_OFFSET;
        MOVS     R3,#+10
        LDR      R4,[R1, #+0]
        UDIV     R5,R2,R3
        MLS      R3,R3,R5,R2
        ORRS     R3,R4,R3, LSL #+8
        STR      R3,[R1, #+0]
//  123 
//  124   v = timp->tm_mday;
        LDR      R3,[R0, #+12]
        MOVS     R2,R3
//  125   timespec->tv_date |= ((v / 10) << RTC_DR_DT_OFFSET) & RTC_DR_DT;
        LDR      R3,[R1, #+0]
        MOVS     R4,#+10
        UDIV     R4,R2,R4
        LSLS     R4,R4,#+4
        ANDS     R4,R4,#0x30
        ORRS     R3,R4,R3
        STR      R3,[R1, #+0]
//  126   timespec->tv_date |= (v % 10) << RTC_DR_DU_OFFSET;
        MOVS     R3,#+10
        LDR      R4,[R1, #+0]
        UDIV     R5,R2,R3
        MLS      R3,R3,R5,R2
        ORRS     R3,R3,R4
        STR      R3,[R1, #+0]
//  127 
//  128   v = timp->tm_hour;
        LDR      R3,[R0, #+8]
        MOVS     R2,R3
//  129   timespec->tv_time |= ((v / 10) << RTC_TR_HT_OFFSET) & RTC_TR_HT;
        LDR      R3,[R1, #+4]
        MOVS     R4,#+10
        UDIV     R4,R2,R4
        LSLS     R4,R4,#+20
        ANDS     R4,R4,#0x300000
        ORRS     R3,R4,R3
        STR      R3,[R1, #+4]
//  130   timespec->tv_time |= (v % 10) << RTC_TR_HU_OFFSET;
        MOVS     R3,#+10
        LDR      R4,[R1, #+4]
        UDIV     R5,R2,R3
        MLS      R3,R3,R5,R2
        ORRS     R3,R4,R3, LSL #+16
        STR      R3,[R1, #+4]
//  131 
//  132   v = timp->tm_min;
        LDR      R3,[R0, #+4]
        MOVS     R2,R3
//  133   timespec->tv_time |= ((v / 10) << RTC_TR_MNT_OFFSET) & RTC_TR_MNT;
        LDR      R3,[R1, #+4]
        MOVS     R4,#+10
        UDIV     R4,R2,R4
        LSLS     R4,R4,#+12
        ANDS     R4,R4,#0x7000
        ORRS     R3,R4,R3
        STR      R3,[R1, #+4]
//  134   timespec->tv_time |= (v % 10) << RTC_TR_MNU_OFFSET;
        MOVS     R3,#+10
        LDR      R4,[R1, #+4]
        UDIV     R5,R2,R3
        MLS      R3,R3,R5,R2
        ORRS     R3,R4,R3, LSL #+8
        STR      R3,[R1, #+4]
//  135 
//  136   v = timp->tm_sec;
        LDR      R3,[R0, #+0]
        MOVS     R2,R3
//  137   timespec->tv_time |= ((v / 10) << RTC_TR_ST_OFFSET) & RTC_TR_ST;
        LDR      R3,[R1, #+4]
        MOVS     R4,#+10
        UDIV     R4,R2,R4
        LSLS     R4,R4,#+4
        ANDS     R4,R4,#0x70
        ORRS     R3,R4,R3
        STR      R3,[R1, #+4]
//  138   timespec->tv_time |= (v % 10) << RTC_TR_SU_OFFSET;
        MOVS     R3,#+10
        LDR      R4,[R1, #+4]
        UDIV     R5,R2,R3
        MLS      R3,R3,R5,R2
        ORRS     R3,R3,R4
        STR      R3,[R1, #+4]
//  139 }
        POP      {R4,R5}
          CFI R4 SameValue
          CFI R5 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock1
//  140 
//  141 /**
//  142  * @brief   Gets raw time from RTC and converts it to canonicalized format.
//  143  *
//  144  * @param[in] rtcp      pointer to RTC driver structure
//  145  * @param[out] timp     pointer to a @p tm structure as defined in time.h
//  146  *
//  147  * @api
//  148  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function rtcGetTimeTm
        THUMB
//  149 void rtcGetTimeTm(RTCDriver *rtcp, struct tm *timp) {
rtcGetTimeTm:
        PUSH     {R4,R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+12
        SUB      SP,SP,#+20
          CFI CFA R13+32
        MOVS     R4,R0
        MOVS     R5,R1
//  150 #if STM32_RTC_HAS_SUBSECONDS
//  151   RTCTime timespec = {0,0,FALSE,0};
        ADD      R0,SP,#+0
        MOVS     R1,#+16
          CFI FunCall __aeabi_memclr4
        BL       __aeabi_memclr4
//  152 #else
//  153   RTCTime timespec = {0,0,FALSE};
//  154 #endif
//  155 
//  156   rtcGetTime(rtcp, &timespec);
        ADD      R1,SP,#+0
        MOVS     R0,R4
          CFI FunCall rtcGetTime
        BL       rtcGetTime
//  157   stm32_rtc_bcd2tm(timp, &timespec);
        ADD      R1,SP,#+0
        MOVS     R0,R5
          CFI FunCall stm32_rtc_bcd2tm
        BL       stm32_rtc_bcd2tm
//  158 }
        ADD      SP,SP,#+20
          CFI CFA R13+12
        POP      {R4,R5,PC}       ;; return
          CFI EndBlock cfiBlock2
//  159 
//  160 /**
//  161  * @brief   Sets RTC time.
//  162  *
//  163  * @param[in] rtcp      pointer to RTC driver structure
//  164  * @param[out] timp     pointer to a @p tm structure as defined in time.h
//  165  *
//  166  * @api
//  167  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function rtcSetTimeTm
        THUMB
//  168 void rtcSetTimeTm(RTCDriver *rtcp, struct tm *timp) {
rtcSetTimeTm:
        PUSH     {R4,R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+12
        SUB      SP,SP,#+20
          CFI CFA R13+32
        MOVS     R4,R0
        MOVS     R5,R1
//  169 #if STM32_RTC_HAS_SUBSECONDS
//  170   RTCTime timespec = {0,0,FALSE,0};
        ADD      R0,SP,#+0
        MOVS     R1,#+16
          CFI FunCall __aeabi_memclr4
        BL       __aeabi_memclr4
//  171 #else
//  172   RTCTime timespec = {0,0,FALSE};
//  173 #endif
//  174 
//  175   stm32_rtc_tm2bcd(timp, &timespec);
        ADD      R1,SP,#+0
        MOVS     R0,R5
          CFI FunCall stm32_rtc_tm2bcd
        BL       stm32_rtc_tm2bcd
//  176   rtcSetTime(rtcp, &timespec);
        ADD      R1,SP,#+0
        MOVS     R0,R4
          CFI FunCall rtcSetTime
        BL       rtcSetTime
//  177 }
        ADD      SP,SP,#+20
          CFI CFA R13+12
        POP      {R4,R5,PC}       ;; return
          CFI EndBlock cfiBlock3
//  178 
//  179 /**
//  180  * @brief   Gets raw time from RTC and converts it to unix format.
//  181  *
//  182  * @param[in] rtcp      pointer to RTC driver structure
//  183  * @return              Unix time value in seconds.
//  184  *
//  185  * @api
//  186  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function rtcGetTimeUnixSec
        THUMB
//  187 time_t rtcGetTimeUnixSec(RTCDriver *rtcp) {
rtcGetTimeUnixSec:
        PUSH     {R4,R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+12
        SUB      SP,SP,#+60
          CFI CFA R13+72
        MOVS     R4,R0
//  188 #if STM32_RTC_HAS_SUBSECONDS
//  189   RTCTime timespec = {0,0,FALSE,0};
        ADD      R0,SP,#+0
        MOVS     R1,#+16
          CFI FunCall __aeabi_memclr4
        BL       __aeabi_memclr4
//  190 #else
//  191   RTCTime timespec = {0,0,FALSE};
//  192 #endif
//  193   struct tm timp;
//  194 
//  195   rtcGetTime(rtcp, &timespec);
        ADD      R1,SP,#+0
        MOVS     R0,R4
          CFI FunCall rtcGetTime
        BL       rtcGetTime
//  196   stm32_rtc_bcd2tm(&timp, &timespec);
        ADD      R1,SP,#+0
        ADD      R0,SP,#+16
          CFI FunCall stm32_rtc_bcd2tm
        BL       stm32_rtc_bcd2tm
//  197 
//  198   return mktime(&timp);
        ADD      R5,SP,#+16
        MOVS     R0,R5
          CFI FunCall __mktime32
        BL       __mktime32
        ADD      SP,SP,#+60
          CFI CFA R13+12
        POP      {R4,R5,PC}       ;; return
          CFI EndBlock cfiBlock4
//  199 }
//  200 
//  201 /**
//  202  * @brief   Sets RTC time.
//  203  *
//  204  * @param[in] rtcp      pointer to RTC driver structure
//  205  * @param[in] tv_sec    time specification
//  206  * @return              Unix time value in seconds.
//  207  *
//  208  * @api
//  209  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function rtcSetTimeUnixSec
        THUMB
//  210 void rtcSetTimeUnixSec(RTCDriver *rtcp, time_t tv_sec) {
rtcSetTimeUnixSec:
        PUSH     {R1,R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+20
        SUB      SP,SP,#+60
          CFI CFA R13+80
        MOVS     R4,R0
//  211 #if STM32_RTC_HAS_SUBSECONDS
//  212   RTCTime timespec = {0,0,FALSE,0};
        ADD      R0,SP,#+0
        MOVS     R1,#+16
          CFI FunCall __aeabi_memclr4
        BL       __aeabi_memclr4
//  213 #else
//  214   RTCTime timespec = {0,0,FALSE};
//  215 #endif
//  216   struct tm timp;
//  217 
//  218 #if defined __GNUC__
//  219   localtime_r(&tv_sec, &timp);
//  220 #else
//  221   struct tm *t = localtime(&tv_sec);
        ADD      R6,SP,#+60
        MOVS     R0,R6
          CFI FunCall __localtime32
        BL       __localtime32
        MOVS     R5,R0
//  222   memcpy(&timp, t, sizeof(struct tm));
        MOVS     R2,#+44
        MOVS     R1,R5
        ADD      R0,SP,#+16
          CFI FunCall memcpy
        BL       memcpy
//  223 #endif
//  224   stm32_rtc_tm2bcd(&timp, &timespec);
        ADD      R1,SP,#+0
        ADD      R0,SP,#+16
          CFI FunCall stm32_rtc_tm2bcd
        BL       stm32_rtc_tm2bcd
//  225   rtcSetTime(rtcp, &timespec);
        ADD      R1,SP,#+0
        MOVS     R0,R4
          CFI FunCall rtcSetTime
        BL       rtcSetTime
//  226 }
        ADD      SP,SP,#+64
          CFI CFA R13+16
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock5
//  227 
//  228 /**
//  229  * @brief   Gets raw time from RTC and converts it to unix format.
//  230  *
//  231  * @param[in] rtcp      pointer to RTC driver structure
//  232  * @return              Unix time value in microseconds.
//  233  *
//  234  * @api
//  235  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function rtcGetTimeUnixUsec
        THUMB
//  236 uint64_t rtcGetTimeUnixUsec(RTCDriver *rtcp) {
rtcGetTimeUnixUsec:
        PUSH     {R4-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+20
        SUB      SP,SP,#+60
          CFI CFA R13+80
        MOVS     R6,R0
//  237 #if STM32_RTC_HAS_SUBSECONDS
//  238   uint64_t result = 0;
        MOVS     R4,#+0
        MOVS     R5,#+0
//  239   RTCTime timespec = {0,0,FALSE,0};
        ADD      R0,SP,#+0
        MOVS     R1,#+16
          CFI FunCall __aeabi_memclr4
        BL       __aeabi_memclr4
//  240   struct tm timp;
//  241 
//  242   rtcGetTime(rtcp, &timespec);
        ADD      R1,SP,#+0
        MOVS     R0,R6
          CFI FunCall rtcGetTime
        BL       rtcGetTime
//  243   stm32_rtc_bcd2tm(&timp, &timespec);
        ADD      R1,SP,#+0
        ADD      R0,SP,#+16
          CFI FunCall stm32_rtc_bcd2tm
        BL       stm32_rtc_bcd2tm
//  244 
//  245   result = (uint64_t)mktime(&timp) * 1000000;
        ADD      R7,SP,#+16
        MOVS     R0,R7
          CFI FunCall __mktime32
        BL       __mktime32
        LDR.N    R1,??DataTable0  ;; 0xf4240
        UMULL    R0,R1,R1,R0
        MOVS     R4,R0
        MOVS     R5,R1
//  246   return result + timespec.tv_msec * 1000;
        LDR      R0,[SP, #+12]
        MOV      R1,#+1000
        MULS     R0,R1,R0
        MOVS     R1,#+0
        ADDS     R0,R4,R0
        ADCS     R1,R5,R1
        ADD      SP,SP,#+60
          CFI CFA R13+20
        POP      {R4-R7,PC}       ;; return
          CFI EndBlock cfiBlock6
//  247 #else
//  248   return (uint64_t)rtcGetTimeUnixSec(rtcp) * 1000000;
//  249 #endif
//  250 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0:
        DC32     0xf4240

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC32 0, 0
        DC8 0, 0, 0, 0
        DC32 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC32 0, 0
        DC8 0, 0, 0, 0
        DC32 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC32 0, 0
        DC8 0, 0, 0, 0
        DC32 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC32 0, 0
        DC8 0, 0, 0, 0
        DC32 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC32 0, 0
        DC8 0, 0, 0, 0
        DC32 0

        END
//  251 
//  252 #else /* STM32_RTC_IS_CALENDAR */
//  253 /**
//  254  * @brief   Gets raw time from RTC and converts it to canonicalized format.
//  255  *
//  256  * @param[in] rtcp      pointer to RTC driver structure
//  257  * @param[out] timp     pointer to a @p tm structure as defined in time.h
//  258  *
//  259  * @api
//  260  */
//  261 void rtcGetTimeTm(RTCDriver *rtcp, struct tm *timp) {
//  262   RTCTime timespec = {0};
//  263 
//  264   rtcGetTime(rtcp, &timespec);
//  265 #if defined __GNUC__
//  266   localtime_r((time_t *)&(timespec.tv_sec), timp);
//  267 #else
//  268   struct tm *t = localtime((time_t *)&(timespec.tv_sec));
//  269   memcpy(&timp, t, sizeof(struct tm));
//  270 #endif
//  271 
//  272 }
//  273 
//  274 /**
//  275  * @brief   Sets RTC time.
//  276  *
//  277  * @param[in] rtcp      pointer to RTC driver structure
//  278  * @param[out] timp     pointer to a @p tm structure as defined in time.h
//  279  *
//  280  * @api
//  281  */
//  282 void rtcSetTimeTm(RTCDriver *rtcp, struct tm *timp) {
//  283   RTCTime timespec = {0};
//  284 
//  285   timespec.tv_sec = mktime(timp);
//  286 #if !defined(LPC122X)
//  287   timespec.tv_msec = 0;
//  288 #endif
//  289   rtcSetTime(rtcp, &timespec);
//  290 }
//  291 
//  292 /**
//  293  * @brief   Gets raw time from RTC and converts it to unix format.
//  294  *
//  295  * @param[in] rtcp      pointer to RTC driver structure
//  296  * @return              Unix time value in seconds.
//  297  *
//  298  * @api
//  299  */
//  300 time_t rtcGetTimeUnixSec(RTCDriver *rtcp) {
//  301   RTCTime timespec = {0};
//  302 
//  303   rtcGetTime(rtcp, &timespec);
//  304   return timespec.tv_sec;
//  305 }
//  306 
//  307 /**
//  308  * @brief   Sets RTC time.
//  309  *
//  310  * @param[in] rtcp      pointer to RTC driver structure
//  311  * @param[in] tv_sec    time specification
//  312  * @return              Unix time value in seconds.
//  313  *
//  314  * @api
//  315  */
//  316 void rtcSetTimeUnixSec(RTCDriver *rtcp, time_t tv_sec) {
//  317   RTCTime timespec = {0};
//  318 
//  319   timespec.tv_sec = tv_sec;
//  320 #if !defined(LPC122X)
//  321   timespec.tv_msec = 0;
//  322 #endif
//  323   rtcSetTime(rtcp, &timespec);
//  324 }
//  325 
//  326 /**
//  327  * @brief   Gets raw time from RTC and converts it to unix format.
//  328  *
//  329  * @param[in] rtcp      pointer to RTC driver structure
//  330  * @return              Unix time value in microseconds.
//  331  *
//  332  * @api
//  333  */
//  334 uint64_t rtcGetTimeUnixUsec(RTCDriver *rtcp) {
//  335 #if STM32_RTC_HAS_SUBSECONDS
//  336   uint64_t result = 0;
//  337   RTCTime timespec = {0,0};
//  338 
//  339   rtcGetTime(rtcp, &timespec);
//  340   result = (uint64_t)timespec.tv_sec * 1000000;
//  341   return result + timespec.tv_msec * 1000;
//  342 #else
//  343   return (uint64_t)rtcGetTimeUnixSec(rtcp) * 1000000;
//  344 #endif
//  345 }
//  346 
//  347 /**
//  348  * @brief   Get current time in format suitable for usage in FatFS.
//  349  *
//  350  * @param[in] rtcp      pointer to RTC driver structure
//  351  * @return              FAT time value.
//  352  *
//  353  * @api
//  354  */
//  355 uint32_t rtcGetTimeFatFromCounter(RTCDriver *rtcp) {
//  356   uint32_t fattime;
//  357   struct tm timp;
//  358 
//  359   rtcGetTimeTm(rtcp, &timp);
//  360 
//  361   fattime  = (timp.tm_sec)       >> 1;
//  362   fattime |= (timp.tm_min)       << 5;
//  363   fattime |= (timp.tm_hour)      << 11;
//  364   fattime |= (timp.tm_mday)      << 16;
//  365   fattime |= (timp.tm_mon + 1)   << 21;
//  366   fattime |= (timp.tm_year - 80) << 25;
//  367 
//  368   return fattime;
//  369 }
//  370 #endif /* STM32_RTC_IS_CALENDAR */
//  371 #endif /* (defined(STM32F4XX) || defined(STM32F2XX) || defined(STM32L1XX) || defined(STM32F1XX)) */
//  372 
//  373 #endif /* HAL_USE_RTC */
//  374 
//  375 /** @} */
// 
//  80 bytes in section .rodata
// 740 bytes in section .text
// 
// 740 bytes of CODE  memory
//  80 bytes of CONST memory
//
//Errors: none
//Warnings: 1
