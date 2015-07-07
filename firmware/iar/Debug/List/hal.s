///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:05 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       src\hal.c                                            /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       src\hal.c -lCN F:\stuff\rusefi_sourceforge\firmware\ /
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
//                       hal.s                                                /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME hal

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _pal_lld_init
        EXTERN adcInit
        EXTERN boardInit
        EXTERN canInit
        EXTERN extInit
        EXTERN gptInit
        EXTERN hal_lld_init
        EXTERN i2cInit
        EXTERN icuInit
        EXTERN mmcInit
        EXTERN pal_default_config
        EXTERN pwmInit
        EXTERN rtcInit
        EXTERN sdInit
        EXTERN sduInit
        EXTERN spiInit
        EXTERN usbInit

        PUBLIC halInit
        PUBLIC halIsCounterWithin
        PUBLIC halPolledDelay
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\src\hal.c
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
//   29  * @file    hal.c
//   30  * @brief   HAL subsystem code.
//   31  *
//   32  * @addtogroup HAL
//   33  * @{
//   34  */
//   35 
//   36 #include "ch.h"
//   37 #include "hal.h"
//   38 
//   39 /*===========================================================================*/
//   40 /* Driver local definitions.                                                 */
//   41 /*===========================================================================*/
//   42 
//   43 /*===========================================================================*/
//   44 /* Driver exported variables.                                                */
//   45 /*===========================================================================*/
//   46 
//   47 /*===========================================================================*/
//   48 /* Driver local variables and types.                                         */
//   49 /*===========================================================================*/
//   50 
//   51 /*===========================================================================*/
//   52 /* Driver local functions.                                                   */
//   53 /*===========================================================================*/
//   54 
//   55 /*===========================================================================*/
//   56 /* Driver exported functions.                                                */
//   57 /*===========================================================================*/
//   58 
//   59 /**
//   60  * @brief   HAL initialization.
//   61  * @details This function invokes the low level initialization code then
//   62  *          initializes all the drivers enabled in the HAL. Finally the
//   63  *          board-specific initialization is performed by invoking
//   64  *          @p boardInit() (usually defined in @p board.c).
//   65  *
//   66  * @init
//   67  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function halInit
        THUMB
//   68 void halInit(void) {
halInit:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   69 
//   70   hal_lld_init();
          CFI FunCall hal_lld_init
        BL       hal_lld_init
//   71 
//   72 #if HAL_USE_TM || defined(__DOXYGEN__)
//   73   tmInit();
//   74 #endif
//   75 #if HAL_USE_PAL || defined(__DOXYGEN__)
//   76   palInit(&pal_default_config);
        LDR.N    R0,??DataTable2
          CFI FunCall _pal_lld_init
        BL       _pal_lld_init
//   77 #endif
//   78 #if HAL_USE_ADC || defined(__DOXYGEN__)
//   79   adcInit();
          CFI FunCall adcInit
        BL       adcInit
//   80 #endif
//   81 #if HAL_USE_CAN || defined(__DOXYGEN__)
//   82   canInit();
          CFI FunCall canInit
        BL       canInit
//   83 #endif
//   84 #if HAL_USE_EXT || defined(__DOXYGEN__)
//   85   extInit();
          CFI FunCall extInit
        BL       extInit
//   86 #endif
//   87 #if HAL_USE_GPT || defined(__DOXYGEN__)
//   88   gptInit();
          CFI FunCall gptInit
        BL       gptInit
//   89 #endif
//   90 #if HAL_USE_I2C || defined(__DOXYGEN__)
//   91   i2cInit();
          CFI FunCall i2cInit
        BL       i2cInit
//   92 #endif
//   93 #if HAL_USE_ICU || defined(__DOXYGEN__)
//   94   icuInit();
          CFI FunCall icuInit
        BL       icuInit
//   95 #endif
//   96 #if HAL_USE_MAC || defined(__DOXYGEN__)
//   97   macInit();
//   98 #endif
//   99 #if HAL_USE_PWM || defined(__DOXYGEN__)
//  100   pwmInit();
          CFI FunCall pwmInit
        BL       pwmInit
//  101 #endif
//  102 #if HAL_USE_SERIAL || defined(__DOXYGEN__)
//  103   sdInit();
          CFI FunCall sdInit
        BL       sdInit
//  104 #endif
//  105 #if HAL_USE_SDC || defined(__DOXYGEN__)
//  106   sdcInit();
//  107 #endif
//  108 #if HAL_USE_SPI || defined(__DOXYGEN__)
//  109   spiInit();
          CFI FunCall spiInit
        BL       spiInit
//  110 #endif
//  111 #if HAL_USE_UART || defined(__DOXYGEN__)
//  112   uartInit();
//  113 #endif
//  114 #if HAL_USE_USB || defined(__DOXYGEN__)
//  115   usbInit();
          CFI FunCall usbInit
        BL       usbInit
//  116 #endif
//  117 #if HAL_USE_MMC_SPI || defined(__DOXYGEN__)
//  118   mmcInit();
          CFI FunCall mmcInit
        BL       mmcInit
//  119 #endif
//  120 #if HAL_USE_SERIAL_USB || defined(__DOXYGEN__)
//  121   sduInit();
          CFI FunCall sduInit
        BL       sduInit
//  122 #endif
//  123 #if HAL_USE_RTC || defined(__DOXYGEN__)
//  124   rtcInit();
          CFI FunCall rtcInit
        BL       rtcInit
//  125 #endif
//  126   /* Board specific initialization.*/
//  127   boardInit();
          CFI FunCall boardInit
        BL       boardInit
//  128 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock0
//  129 
//  130 #if HAL_IMPLEMENTS_COUNTERS || defined(__DOXYGEN__)
//  131 /**
//  132  * @brief   Realtime window test.
//  133  * @details This function verifies if the current realtime counter value
//  134  *          lies within the specified range or not. The test takes care
//  135  *          of the realtime counter wrapping to zero on overflow.
//  136  * @note    When start==end then the function returns always true because the
//  137  *          whole time range is specified.
//  138  * @note    This is an optional service that could not be implemented in
//  139  *          all HAL implementations.
//  140  * @note    This function can be called from any context.
//  141  *
//  142  * @par Example 1
//  143  * Example of a guarded loop using the realtime counter. The loop implements
//  144  * a timeout after one second.
//  145  * @code
//  146  *   halrtcnt_t start = halGetCounterValue();
//  147  *   halrtcnt_t timeout  = start + S2RTT(1);
//  148  *   while (my_condition) {
//  149  *     if (!halIsCounterWithin(start, timeout)
//  150  *       return TIMEOUT;
//  151  *     // Do something.
//  152  *   }
//  153  *   // Continue.
//  154  * @endcode
//  155  *
//  156  * @par Example 2
//  157  * Example of a loop that lasts exactly 50 microseconds.
//  158  * @code
//  159  *   halrtcnt_t start = halGetCounterValue();
//  160  *   halrtcnt_t timeout  = start + US2RTT(50);
//  161  *   while (halIsCounterWithin(start, timeout)) {
//  162  *     // Do something.
//  163  *   }
//  164  *   // Continue.
//  165  * @endcode
//  166  *
//  167  * @param[in] start     the start of the time window (inclusive)
//  168  * @param[in] end       the end of the time window (non inclusive)
//  169  * @retval TRUE         current time within the specified time window.
//  170  * @retval FALSE        current time not within the specified time window.
//  171  *
//  172  * @special
//  173  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function halIsCounterWithin
          CFI NoCalls
        THUMB
//  174 bool_t halIsCounterWithin(halrtcnt_t start, halrtcnt_t end) {
//  175   halrtcnt_t now = halGetCounterValue();
halIsCounterWithin:
        LDR.N    R2,??DataTable2_1  ;; 0xe0001004
        LDR      R2,[R2, #+0]
//  176 
//  177   return end > start ? (now >= start) && (now < end) :
//  178                        (now >= start) || (now < end);
        CMP      R0,R1
        BCS.N    ??halIsCounterWithin_0
        CMP      R2,R0
        BCC.N    ??halIsCounterWithin_1
        CMP      R2,R1
        BCS.N    ??halIsCounterWithin_1
        MOVS     R0,#+1
        B.N      ??halIsCounterWithin_2
??halIsCounterWithin_1:
        MOVS     R0,#+0
??halIsCounterWithin_2:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        B.N      ??halIsCounterWithin_3
??halIsCounterWithin_0:
        CMP      R2,R0
        BCS.N    ??halIsCounterWithin_4
        CMP      R2,R1
        BCS.N    ??halIsCounterWithin_5
??halIsCounterWithin_4:
        MOVS     R0,#+1
        B.N      ??halIsCounterWithin_6
??halIsCounterWithin_5:
        MOVS     R0,#+0
??halIsCounterWithin_6:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
??halIsCounterWithin_3:
        CMP      R0,#+0
        BEQ.N    ??halIsCounterWithin_7
        MOVS     R0,#+1
        B.N      ??halIsCounterWithin_8
??halIsCounterWithin_7:
        MOVS     R0,#+0
??halIsCounterWithin_8:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        BX       LR               ;; return
          CFI EndBlock cfiBlock1
//  179 }
//  180 
//  181 /**
//  182  * @brief   Polled delay.
//  183  * @note    The real delays is always few cycles in excess of the specified
//  184  *          value.
//  185  * @note    This is an optional service that could not be implemented in
//  186  *          all HAL implementations.
//  187  * @note    This function can be called from any context.
//  188  *
//  189  * @param[in] ticks     number of ticks
//  190  *
//  191  * @special
//  192  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function halPolledDelay
        THUMB
//  193 void halPolledDelay(halrtcnt_t ticks) {
halPolledDelay:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
//  194   halrtcnt_t start = halGetCounterValue();
        LDR.N    R0,??DataTable2_1  ;; 0xe0001004
        LDR      R5,[R0, #+0]
//  195   halrtcnt_t timeout  = start + (ticks);
        ADDS     R6,R4,R5
//  196   while (halIsCounterWithin(start, timeout))
??halPolledDelay_0:
        MOVS     R1,R6
        MOVS     R0,R5
          CFI FunCall halIsCounterWithin
        BL       halIsCounterWithin
        CMP      R0,#+0
        BNE.N    ??halPolledDelay_0
//  197     ;
//  198 }
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock2

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2:
        DC32     pal_default_config

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_1:
        DC32     0xe0001004

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        END
//  199 #endif /* HAL_IMPLEMENTS_COUNTERS */
//  200 
//  201 /** @} */
// 
// 158 bytes in section .text
// 
// 158 bytes of CODE memory
//
//Errors: none
//Warnings: none
