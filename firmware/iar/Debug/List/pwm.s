///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:32 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       src\pwm.c                                            /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       src\pwm.c -lCN F:\stuff\rusefi_sourceforge\firmware\ /
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
//                       pwm.s                                                /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME pwm

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN chDbgPanic
        EXTERN chDbgPanic3
        EXTERN dbg_check_lock
        EXTERN dbg_check_unlock
        EXTERN pwm_lld_disable_channel
        EXTERN pwm_lld_enable_channel
        EXTERN pwm_lld_init
        EXTERN pwm_lld_start
        EXTERN pwm_lld_stop

        PUBLIC pwmChangePeriod
        PUBLIC pwmDisableChannel
        PUBLIC pwmEnableChannel
        PUBLIC pwmInit
        PUBLIC pwmObjectInit
        PUBLIC pwmStart
        PUBLIC pwmStop
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\src\pwm.c
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
//   29  * @file    pwm.c
//   30  * @brief   PWM Driver code.
//   31  *
//   32  * @addtogroup PWM
//   33  * @{
//   34  */
//   35 
//   36 #include "ch.h"
//   37 #include "hal.h"
//   38 
//   39 #if HAL_USE_PWM || defined(__DOXYGEN__)
//   40 
//   41 /*===========================================================================*/
//   42 /* Driver local definitions.                                                 */
//   43 /*===========================================================================*/
//   44 
//   45 /*===========================================================================*/
//   46 /* Driver exported variables.                                                */
//   47 /*===========================================================================*/
//   48 
//   49 /*===========================================================================*/
//   50 /* Driver local variables and types.                                         */
//   51 /*===========================================================================*/
//   52 
//   53 /*===========================================================================*/
//   54 /* Driver local functions.                                                   */
//   55 /*===========================================================================*/
//   56 
//   57 /*===========================================================================*/
//   58 /* Driver exported functions.                                                */
//   59 /*===========================================================================*/
//   60 
//   61 /**
//   62  * @brief   PWM Driver initialization.
//   63  * @note    This function is implicitly invoked by @p halInit(), there is
//   64  *          no need to explicitly initialize the driver.
//   65  *
//   66  * @init
//   67  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function pwmInit
        THUMB
//   68 void pwmInit(void) {
pwmInit:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   69 
//   70   pwm_lld_init();
          CFI FunCall pwm_lld_init
        BL       pwm_lld_init
//   71 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock0
//   72 
//   73 /**
//   74  * @brief   Initializes the standard part of a @p PWMDriver structure.
//   75  *
//   76  * @param[out] pwmp     pointer to a @p PWMDriver object
//   77  *
//   78  * @init
//   79  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function pwmObjectInit
          CFI NoCalls
        THUMB
//   80 void pwmObjectInit(PWMDriver *pwmp) {
//   81 
//   82   pwmp->state    = PWM_STOP;
pwmObjectInit:
        MOVS     R1,#+1
        STRB     R1,[R0, #+0]
//   83   pwmp->config   = NULL;
        MOVS     R1,#+0
        STR      R1,[R0, #+4]
//   84 #if defined(PWM_DRIVER_EXT_INIT_HOOK)
//   85   PWM_DRIVER_EXT_INIT_HOOK(pwmp);
//   86 #endif
//   87 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock1
//   88 
//   89 /**
//   90  * @brief   Configures and activates the PWM peripheral.
//   91  * @note    Starting a driver that is already in the @p PWM_READY state
//   92  *          disables all the active channels.
//   93  *
//   94  * @param[in] pwmp      pointer to a @p PWMDriver object
//   95  * @param[in] config    pointer to a @p PWMConfig object
//   96  *
//   97  * @api
//   98  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function pwmStart
        THUMB
//   99 void pwmStart(PWMDriver *pwmp, const PWMConfig *config) {
pwmStart:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  100 
//  101   chDbgCheck((pwmp != NULL) && (config != NULL), "pwmStart");
        CMP      R4,#+0
        BEQ.N    ??pwmStart_0
        CMP      R5,#+0
        BNE.N    ??pwmStart_1
??pwmStart_0:
        MOVS     R2,#+101
        LDR.N    R1,??DataTable4
        LDR.N    R0,??DataTable4_1
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  102 
//  103   chSysLock();
??pwmStart_1:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  104   chDbgAssert((pwmp->state == PWM_STOP) || (pwmp->state == PWM_READY),
//  105               "pwmStart(), #1", "invalid state");
        LDRB     R0,[R4, #+0]
        CMP      R0,#+1
        BEQ.N    ??pwmStart_2
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BEQ.N    ??pwmStart_2
        LDR.N    R0,??DataTable4_2
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  106   pwmp->config = config;
??pwmStart_2:
        STR      R5,[R4, #+4]
//  107   pwmp->period = config->period;
        LDRH     R0,[R5, #+4]
        STRH     R0,[R4, #+8]
//  108   pwm_lld_start(pwmp);
        MOVS     R0,R4
          CFI FunCall pwm_lld_start
        BL       pwm_lld_start
//  109   pwmp->state = PWM_READY;
        MOVS     R0,#+2
        STRB     R0,[R4, #+0]
//  110   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  111 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock2
//  112 
//  113 /**
//  114  * @brief   Deactivates the PWM peripheral.
//  115  *
//  116  * @param[in] pwmp      pointer to a @p PWMDriver object
//  117  *
//  118  * @api
//  119  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function pwmStop
        THUMB
//  120 void pwmStop(PWMDriver *pwmp) {
pwmStop:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  121 
//  122   chDbgCheck(pwmp != NULL, "pwmStop");
        CMP      R4,#+0
        BNE.N    ??pwmStop_0
        MOVS     R2,#+122
        LDR.N    R1,??DataTable4
        LDR.N    R0,??DataTable4_3
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  123 
//  124   chSysLock();
??pwmStop_0:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  125   chDbgAssert((pwmp->state == PWM_STOP) || (pwmp->state == PWM_READY),
//  126               "pwmStop(), #1", "invalid state");
        LDRB     R0,[R4, #+0]
        CMP      R0,#+1
        BEQ.N    ??pwmStop_1
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BEQ.N    ??pwmStop_1
        LDR.N    R0,??DataTable4_4
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  127   pwm_lld_stop(pwmp);
??pwmStop_1:
        MOVS     R0,R4
          CFI FunCall pwm_lld_stop
        BL       pwm_lld_stop
//  128   pwmp->state = PWM_STOP;
        MOVS     R0,#+1
        STRB     R0,[R4, #+0]
//  129   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  130 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock3
//  131 
//  132 /**
//  133  * @brief   Changes the period the PWM peripheral.
//  134  * @details This function changes the period of a PWM unit that has already
//  135  *          been activated using @p pwmStart().
//  136  * @pre     The PWM unit must have been activated using @p pwmStart().
//  137  * @post    The PWM unit period is changed to the new value.
//  138  * @note    If a period is specified that is shorter than the pulse width
//  139  *          programmed in one of the channels then the behavior is not
//  140  *          guaranteed.
//  141  *
//  142  * @param[in] pwmp      pointer to a @p PWMDriver object
//  143  * @param[in] period    new cycle time in ticks
//  144  *
//  145  * @api
//  146  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function pwmChangePeriod
        THUMB
//  147 void pwmChangePeriod(PWMDriver *pwmp, pwmcnt_t period) {
pwmChangePeriod:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  148 
//  149   chDbgCheck(pwmp != NULL, "pwmChangePeriod");
        CMP      R4,#+0
        BNE.N    ??pwmChangePeriod_0
        MOVS     R2,#+149
        LDR.N    R1,??DataTable4
        LDR.N    R0,??DataTable4_5
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  150 
//  151   chSysLock();
??pwmChangePeriod_0:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  152   chDbgAssert(pwmp->state == PWM_READY,
//  153               "pwmChangePeriod(), #1", "invalid state");
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BEQ.N    ??pwmChangePeriod_1
        LDR.N    R0,??DataTable4_6
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  154   pwmChangePeriodI(pwmp, period);
??pwmChangePeriod_1:
        STRH     R5,[R4, #+8]
        LDR      R0,[R4, #+16]
        SUBS     R1,R5,#+1
        UXTH     R1,R1            ;; ZeroExt  R1,R1,#+16,#+16
        STR      R1,[R0, #+44]
//  155   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  156 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock4
//  157 
//  158 /**
//  159  * @brief   Enables a PWM channel.
//  160  * @pre     The PWM unit must have been activated using @p pwmStart().
//  161  * @post    The channel is active using the specified configuration.
//  162  * @note    Depending on the hardware implementation this function has
//  163  *          effect starting on the next cycle (recommended implementation)
//  164  *          or immediately (fallback implementation).
//  165  *
//  166  * @param[in] pwmp      pointer to a @p PWMDriver object
//  167  * @param[in] channel   PWM channel identifier (0...PWM_CHANNELS-1)
//  168  * @param[in] width     PWM pulse width as clock pulses number
//  169  *
//  170  * @api
//  171  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function pwmEnableChannel
        THUMB
//  172 void pwmEnableChannel(PWMDriver *pwmp,
//  173                       pwmchannel_t channel,
//  174                       pwmcnt_t width) {
pwmEnableChannel:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//  175 
//  176   chDbgCheck((pwmp != NULL) && (channel < PWM_CHANNELS),
//  177              "pwmEnableChannel");
        CMP      R4,#+0
        BEQ.N    ??pwmEnableChannel_0
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+4
        BLT.N    ??pwmEnableChannel_1
??pwmEnableChannel_0:
        MOVS     R2,#+177
        LDR.N    R1,??DataTable4
        LDR.N    R0,??DataTable4_7
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  178 
//  179   chSysLock();
??pwmEnableChannel_1:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  180   chDbgAssert(pwmp->state == PWM_READY,
//  181               "pwmEnableChannel(), #1", "not ready");
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BEQ.N    ??pwmEnableChannel_2
        LDR.N    R0,??DataTable4_8
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  182   pwm_lld_enable_channel(pwmp, channel, width);
??pwmEnableChannel_2:
        MOVS     R2,R6
        UXTH     R2,R2            ;; ZeroExt  R2,R2,#+16,#+16
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
          CFI FunCall pwm_lld_enable_channel
        BL       pwm_lld_enable_channel
//  183   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  184 }
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock5
//  185 
//  186 /**
//  187  * @brief   Disables a PWM channel.
//  188  * @pre     The PWM unit must have been activated using @p pwmStart().
//  189  * @post    The channel is disabled and its output line returned to the
//  190  *          idle state.
//  191  * @note    Depending on the hardware implementation this function has
//  192  *          effect starting on the next cycle (recommended implementation)
//  193  *          or immediately (fallback implementation).
//  194  *
//  195  * @param[in] pwmp      pointer to a @p PWMDriver object
//  196  * @param[in] channel   PWM channel identifier (0...PWM_CHANNELS-1)
//  197  *
//  198  * @api
//  199  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function pwmDisableChannel
        THUMB
//  200 void pwmDisableChannel(PWMDriver *pwmp, pwmchannel_t channel) {
pwmDisableChannel:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  201 
//  202   chDbgCheck((pwmp != NULL) && (channel < PWM_CHANNELS),
//  203              "pwmEnableChannel");
        CMP      R4,#+0
        BEQ.N    ??pwmDisableChannel_0
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+4
        BLT.N    ??pwmDisableChannel_1
??pwmDisableChannel_0:
        MOVS     R2,#+203
        LDR.N    R1,??DataTable4
        LDR.N    R0,??DataTable4_7
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  204 
//  205   chSysLock();
??pwmDisableChannel_1:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  206   chDbgAssert(pwmp->state == PWM_READY,
//  207               "pwmDisableChannel(), #1", "not ready");
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BEQ.N    ??pwmDisableChannel_2
        LDR.N    R0,??DataTable4_9
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  208   pwm_lld_disable_channel(pwmp, channel);
??pwmDisableChannel_2:
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
          CFI FunCall pwm_lld_disable_channel
        BL       pwm_lld_disable_channel
//  209   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  210 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock6

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4:
        DC32     `?<Constant "F:\\\\stuff\\\\rusefi_sourc...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_1:
        DC32     `?<Constant "\\"pwmStart\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_2:
        DC32     `?<Constant "pwmStart(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_3:
        DC32     `?<Constant "\\"pwmStop\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_4:
        DC32     `?<Constant "pwmStop(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_5:
        DC32     `?<Constant "\\"pwmChangePeriod\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_6:
        DC32     `?<Constant "pwmChangePeriod(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_7:
        DC32     `?<Constant "\\"pwmEnableChannel\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_8:
        DC32     `?<Constant "pwmEnableChannel(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_9:
        DC32     `?<Constant "pwmDisableChannel(), #1">`

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"pwmStart\\"()">`:
        DATA
        DC8 "\"pwmStart\"()"
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
        DC8 70H, 77H, 6DH, 2EH, 63H, 0
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "pwmStart(), #1">`:
        DATA
        DC8 "pwmStart(), #1"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"pwmStop\\"()">`:
        DATA
        DC8 "\"pwmStop\"()"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "pwmStop(), #1">`:
        DATA
        DC8 "pwmStop(), #1"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"pwmChangePeriod\\"()">`:
        DATA
        DC8 "\"pwmChangePeriod\"()"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "pwmChangePeriod(), #1">`:
        DATA
        DC8 "pwmChangePeriod(), #1"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"pwmEnableChannel\\"()">`:
        DATA
        DC8 "\"pwmEnableChannel\"()"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "pwmEnableChannel(), #1">`:
        DATA
        DC8 "pwmEnableChannel(), #1"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "pwmDisableChannel(), #1">`:
        DATA
        DC8 "pwmDisableChannel(), #1"

        END
//  211 
//  212 #endif /* HAL_USE_PWM */
//  213 
//  214 /** @} */
// 
// 240 bytes in section .rodata
// 416 bytes in section .text
// 
// 416 bytes of CODE  memory
// 240 bytes of CONST memory
//
//Errors: none
//Warnings: none
