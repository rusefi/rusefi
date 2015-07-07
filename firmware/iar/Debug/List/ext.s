///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:48 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       src\ext.c                                            /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       src\ext.c -lCN F:\stuff\rusefi_sourceforge\firmware\ /
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
//                       ext.s                                                /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME ext

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN chDbgPanic
        EXTERN chDbgPanic3
        EXTERN dbg_check_lock
        EXTERN dbg_check_unlock
        EXTERN ext_lld_channel_disable
        EXTERN ext_lld_channel_enable
        EXTERN ext_lld_init
        EXTERN ext_lld_start
        EXTERN ext_lld_stop

        PUBLIC extChannelDisable
        PUBLIC extChannelEnable
        PUBLIC extInit
        PUBLIC extObjectInit
        PUBLIC extSetChannelModeI
        PUBLIC extStart
        PUBLIC extStop
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\src\ext.c
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
//   29  * @file    ext.c
//   30  * @brief   EXT Driver code.
//   31  *
//   32  * @addtogroup EXT
//   33  * @{
//   34  */
//   35 
//   36 #include "ch.h"
//   37 #include "hal.h"
//   38 
//   39 #if HAL_USE_EXT || defined(__DOXYGEN__)
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
//   62  * @brief   EXT Driver initialization.
//   63  * @note    This function is implicitly invoked by @p halInit(), there is
//   64  *          no need to explicitly initialize the driver.
//   65  *
//   66  * @init
//   67  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function extInit
        THUMB
//   68 void extInit(void) {
extInit:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   69 
//   70   ext_lld_init();
          CFI FunCall ext_lld_init
        BL       ext_lld_init
//   71 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock0
//   72 
//   73 /**
//   74  * @brief   Initializes the standard part of a @p EXTDriver structure.
//   75  *
//   76  * @param[out] extp     pointer to the @p EXTDriver object
//   77  *
//   78  * @init
//   79  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function extObjectInit
          CFI NoCalls
        THUMB
//   80 void extObjectInit(EXTDriver *extp) {
//   81 
//   82   extp->state  = EXT_STOP;
extObjectInit:
        MOVS     R1,#+1
        STRB     R1,[R0, #+0]
//   83   extp->config = NULL;
        MOVS     R1,#+0
        STR      R1,[R0, #+4]
//   84 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock1
//   85 
//   86 /**
//   87  * @brief   Configures and activates the EXT peripheral.
//   88  * @post    After activation all EXT channels are in the disabled state,
//   89  *          use @p extChannelEnable() in order to activate them.
//   90  *
//   91  * @param[in] extp      pointer to the @p EXTDriver object
//   92  * @param[in] config    pointer to the @p EXTConfig object
//   93  *
//   94  * @api
//   95  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function extStart
        THUMB
//   96 void extStart(EXTDriver *extp, const EXTConfig *config) {
extStart:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//   97 
//   98   chDbgCheck((extp != NULL) && (config != NULL), "extStart");
        CMP      R4,#+0
        BEQ.N    ??extStart_0
        CMP      R5,#+0
        BNE.N    ??extStart_1
??extStart_0:
        MOVS     R2,#+98
        LDR.N    R1,??DataTable4
        LDR.N    R0,??DataTable4_1
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//   99 
//  100   chSysLock();
??extStart_1:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  101   chDbgAssert((extp->state == EXT_STOP) || (extp->state == EXT_ACTIVE),
//  102               "extStart(), #1", "invalid state");
        LDRB     R0,[R4, #+0]
        CMP      R0,#+1
        BEQ.N    ??extStart_2
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BEQ.N    ??extStart_2
        LDR.N    R0,??DataTable4_2
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  103   extp->config = config;
??extStart_2:
        STR      R5,[R4, #+4]
//  104   ext_lld_start(extp);
        MOVS     R0,R4
          CFI FunCall ext_lld_start
        BL       ext_lld_start
//  105   extp->state = EXT_ACTIVE;
        MOVS     R0,#+2
        STRB     R0,[R4, #+0]
//  106   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  107 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock2
//  108 
//  109 /**
//  110  * @brief   Deactivates the EXT peripheral.
//  111  *
//  112  * @param[in] extp      pointer to the @p EXTDriver object
//  113  *
//  114  * @api
//  115  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function extStop
        THUMB
//  116 void extStop(EXTDriver *extp) {
extStop:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  117 
//  118   chDbgCheck(extp != NULL, "extStop");
        CMP      R4,#+0
        BNE.N    ??extStop_0
        MOVS     R2,#+118
        LDR.N    R1,??DataTable4
        LDR.N    R0,??DataTable4_3
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  119 
//  120   chSysLock();
??extStop_0:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  121   chDbgAssert((extp->state == EXT_STOP) || (extp->state == EXT_ACTIVE),
//  122               "extStop(), #1", "invalid state");
        LDRB     R0,[R4, #+0]
        CMP      R0,#+1
        BEQ.N    ??extStop_1
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BEQ.N    ??extStop_1
        LDR.N    R0,??DataTable4_4
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  123   ext_lld_stop(extp);
??extStop_1:
        MOVS     R0,R4
          CFI FunCall ext_lld_stop
        BL       ext_lld_stop
//  124   extp->state = EXT_STOP;
        MOVS     R0,#+1
        STRB     R0,[R4, #+0]
//  125   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  126 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock3
//  127 
//  128 /**
//  129  * @brief   Enables an EXT channel.
//  130  * @pre     The channel must not be in @p EXT_CH_MODE_DISABLED mode.
//  131  *
//  132  * @param[in] extp      pointer to the @p EXTDriver object
//  133  * @param[in] channel   channel to be enabled
//  134  *
//  135  * @api
//  136  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function extChannelEnable
        THUMB
//  137 void extChannelEnable(EXTDriver *extp, expchannel_t channel) {
extChannelEnable:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  138 
//  139   chDbgCheck((extp != NULL) && (channel < EXT_MAX_CHANNELS),
//  140              "extChannelEnable");
        CMP      R4,#+0
        BEQ.N    ??extChannelEnable_0
        CMP      R5,#+23
        BCC.N    ??extChannelEnable_1
??extChannelEnable_0:
        MOVS     R2,#+140
        LDR.N    R1,??DataTable4
        LDR.N    R0,??DataTable4_5
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  141 
//  142   chSysLock();
??extChannelEnable_1:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  143   chDbgAssert((extp->state == EXT_ACTIVE) &&
//  144               ((extp->config->channels[channel].mode &
//  145                 EXT_CH_MODE_EDGES_MASK) != EXT_CH_MODE_DISABLED),
//  146               "extChannelEnable(), #1", "invalid state");
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BNE.N    ??extChannelEnable_2
        LDR      R0,[R4, #+4]
        LDRB     R0,[R0, R5, LSL #+3]
        TST      R0,#0x3
        BNE.N    ??extChannelEnable_3
??extChannelEnable_2:
        LDR.N    R0,??DataTable4_6
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  147   extChannelEnableI(extp, channel);
??extChannelEnable_3:
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall ext_lld_channel_enable
        BL       ext_lld_channel_enable
//  148   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  149 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock4
//  150 
//  151 /**
//  152  * @brief   Disables an EXT channel.
//  153  * @pre     The channel must not be in @p EXT_CH_MODE_DISABLED mode.
//  154  *
//  155  * @param[in] extp      pointer to the @p EXTDriver object
//  156  * @param[in] channel   channel to be disabled
//  157  *
//  158  * @api
//  159  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function extChannelDisable
        THUMB
//  160 void extChannelDisable(EXTDriver *extp, expchannel_t channel) {
extChannelDisable:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  161 
//  162   chDbgCheck((extp != NULL) && (channel < EXT_MAX_CHANNELS),
//  163              "extChannelDisable");
        CMP      R4,#+0
        BEQ.N    ??extChannelDisable_0
        CMP      R5,#+23
        BCC.N    ??extChannelDisable_1
??extChannelDisable_0:
        MOVS     R2,#+163
        LDR.N    R1,??DataTable4
        LDR.N    R0,??DataTable4_7
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  164 
//  165   chSysLock();
??extChannelDisable_1:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  166   chDbgAssert((extp->state == EXT_ACTIVE) &&
//  167               ((extp->config->channels[channel].mode &
//  168                 EXT_CH_MODE_EDGES_MASK) != EXT_CH_MODE_DISABLED),
//  169               "extChannelDisable(), #1", "invalid state");
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BNE.N    ??extChannelDisable_2
        LDR      R0,[R4, #+4]
        LDRB     R0,[R0, R5, LSL #+3]
        TST      R0,#0x3
        BNE.N    ??extChannelDisable_3
??extChannelDisable_2:
        LDR.N    R0,??DataTable4_8
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  170   extChannelDisableI(extp, channel);
??extChannelDisable_3:
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall ext_lld_channel_disable
        BL       ext_lld_channel_disable
//  171   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  172 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock5
//  173 
//  174 /**
//  175  * @brief   Changes the operation mode of a channel.
//  176  * @note    This function attempts to write over the current configuration
//  177  *          structure that must have been not declared constant. This
//  178  *          violates the @p const qualifier in @p extStart() but it is
//  179  *          intentional.
//  180  * @note    This function cannot be used if the configuration structure is
//  181  *          declared @p const.
//  182  * @note    The effect of this function on constant configuration structures
//  183  *          is not defined.
//  184  *
//  185  * @param[in] extp      pointer to the @p EXTDriver object
//  186  * @param[in] channel   channel to be changed
//  187  * @param[in] extcp     new configuration for the channel
//  188  *
//  189  * @iclass
//  190  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function extSetChannelModeI
        THUMB
//  191 void extSetChannelModeI(EXTDriver *extp,
//  192                         expchannel_t channel,
//  193                         const EXTChannelConfig *extcp) {
extSetChannelModeI:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//  194   EXTChannelConfig *oldcp;
//  195 
//  196   chDbgCheck((extp != NULL) && (channel < EXT_MAX_CHANNELS) &&
//  197              (extcp != NULL), "extSetChannelModeI");
        CMP      R4,#+0
        BEQ.N    ??extSetChannelModeI_0
        CMP      R5,#+23
        BCS.N    ??extSetChannelModeI_0
        CMP      R6,#+0
        BNE.N    ??extSetChannelModeI_1
??extSetChannelModeI_0:
        MOVS     R2,#+197
        LDR.N    R1,??DataTable4
        LDR.N    R0,??DataTable4_9
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  198 
//  199   chDbgAssert(extp->state == EXT_ACTIVE,
//  200               "extSetChannelModeI(), #1", "invalid state");
??extSetChannelModeI_1:
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BEQ.N    ??extSetChannelModeI_2
        LDR.N    R0,??DataTable4_10
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  201 
//  202   /* Note that here the access is enforced as non-const, known access
//  203      violation.*/
//  204   oldcp = (EXTChannelConfig *)&extp->config->channels[channel];
??extSetChannelModeI_2:
        LDR      R0,[R4, #+4]
        ADDS     R0,R0,R5, LSL #+3
        MOVS     R7,R0
//  205 
//  206   /* Overwiting the old channels configuration then the channel is reconfigured
//  207      by the low level driver.*/
//  208   *oldcp = *extcp;
        LDRD     R0,R1,[R6, #+0]
        STRD     R0,R1,[R7, #+0]
//  209   ext_lld_channel_enable(extp, channel);
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall ext_lld_channel_enable
        BL       ext_lld_channel_enable
//  210 }
        POP      {R0,R4-R7,PC}    ;; return
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
        DC32     `?<Constant "\\"extStart\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_2:
        DC32     `?<Constant "extStart(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_3:
        DC32     `?<Constant "\\"extStop\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_4:
        DC32     `?<Constant "extStop(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_5:
        DC32     `?<Constant "\\"extChannelEnable\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_6:
        DC32     `?<Constant "extChannelEnable(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_7:
        DC32     `?<Constant "\\"extChannelDisable\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_8:
        DC32     `?<Constant "extChannelDisable(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_9:
        DC32     `?<Constant "\\"extSetChannelModeI\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_10:
        DC32     `?<Constant "extSetChannelModeI(), #1">`

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"extStart\\"()">`:
        DATA
        DC8 "\"extStart\"()"
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
        DC8 65H, 78H, 74H, 2EH, 63H, 0
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "extStart(), #1">`:
        DATA
        DC8 "extStart(), #1"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"extStop\\"()">`:
        DATA
        DC8 "\"extStop\"()"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "extStop(), #1">`:
        DATA
        DC8 "extStop(), #1"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"extChannelEnable\\"()">`:
        DATA
        DC8 "\"extChannelEnable\"()"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "extChannelEnable(), #1">`:
        DATA
        DC8 "extChannelEnable(), #1"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"extChannelDisable\\"()">`:
        DATA
        DC8 "\"extChannelDisable\"()"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "extChannelDisable(), #1">`:
        DATA
        DC8 "extChannelDisable(), #1"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"extSetChannelModeI\\"()">`:
        DATA
        DC8 "\"extSetChannelModeI\"()"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "extSetChannelModeI(), #1">`:
        DATA
        DC8 "extSetChannelModeI(), #1"
        DC8 0, 0, 0

        END
//  211 
//  212 #endif /* HAL_USE_EXT */
//  213 
//  214 /** @} */
// 
// 272 bytes in section .rodata
// 430 bytes in section .text
// 
// 430 bytes of CODE  memory
// 272 bytes of CONST memory
//
//Errors: none
//Warnings: none
