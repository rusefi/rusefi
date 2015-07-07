///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:08:41 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       src\i2c.c                                            /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       src\i2c.c -lCN F:\stuff\rusefi_sourceforge\firmware\ /
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
//                       i2c.s                                                /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME i2c

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN chDbgPanic
        EXTERN chDbgPanic3
        EXTERN chMtxInit
        EXTERN chMtxLock
        EXTERN chMtxUnlock
        EXTERN dbg_check_lock
        EXTERN dbg_check_unlock
        EXTERN i2c_lld_init
        EXTERN i2c_lld_master_receive_timeout
        EXTERN i2c_lld_master_transmit_timeout
        EXTERN i2c_lld_start
        EXTERN i2c_lld_stop

        PUBLIC i2cAcquireBus
        PUBLIC i2cGetErrors
        PUBLIC i2cInit
        PUBLIC i2cMasterReceiveTimeout
        PUBLIC i2cMasterTransmitTimeout
        PUBLIC i2cObjectInit
        PUBLIC i2cReleaseBus
        PUBLIC i2cStart
        PUBLIC i2cStop
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\src\i2c.c
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
//   33  * @file    i2c.c
//   34  * @brief   I2C Driver code.
//   35  *
//   36  * @addtogroup I2C
//   37  * @{
//   38  */
//   39 #include "ch.h"
//   40 #include "hal.h"
//   41 
//   42 #if HAL_USE_I2C || defined(__DOXYGEN__)
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
//   56 /*===========================================================================*/
//   57 /* Driver local functions.                                                   */
//   58 /*===========================================================================*/
//   59 
//   60 /*===========================================================================*/
//   61 /* Driver exported functions.                                                */
//   62 /*===========================================================================*/
//   63 
//   64 /**
//   65  * @brief   I2C Driver initialization.
//   66  * @note    This function is implicitly invoked by @p halInit(), there is
//   67  *          no need to explicitly initialize the driver.
//   68  *
//   69  * @init
//   70  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function i2cInit
        THUMB
//   71 void i2cInit(void) {
i2cInit:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   72   i2c_lld_init();
          CFI FunCall i2c_lld_init
        BL       i2c_lld_init
//   73 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock0
//   74 
//   75 /**
//   76  * @brief   Initializes the standard part of a @p I2CDriver structure.
//   77  *
//   78  * @param[out] i2cp     pointer to the @p I2CDriver object
//   79  *
//   80  * @init
//   81  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function i2cObjectInit
        THUMB
//   82 void i2cObjectInit(I2CDriver *i2cp) {
i2cObjectInit:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   83 
//   84   i2cp->state  = I2C_STOP;
        MOVS     R0,#+1
        STRB     R0,[R4, #+0]
//   85   i2cp->config = NULL;
        MOVS     R0,#+0
        STR      R0,[R4, #+4]
//   86 
//   87 #if I2C_USE_MUTUAL_EXCLUSION
//   88 #if CH_USE_MUTEXES
//   89   chMtxInit(&i2cp->mutex);
        ADDS     R0,R4,#+12
          CFI FunCall chMtxInit
        BL       chMtxInit
//   90 #else
//   91   chSemInit(&i2cp->semaphore, 1);
//   92 #endif /* CH_USE_MUTEXES */
//   93 #endif /* I2C_USE_MUTUAL_EXCLUSION */
//   94 
//   95 #if defined(I2C_DRIVER_EXT_INIT_HOOK)
//   96   I2C_DRIVER_EXT_INIT_HOOK(i2cp);
//   97 #endif
//   98 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock1
//   99 
//  100 /**
//  101  * @brief   Configures and activates the I2C peripheral.
//  102  *
//  103  * @param[in] i2cp      pointer to the @p I2CDriver object
//  104  * @param[in] config    pointer to the @p I2CConfig object
//  105  *
//  106  * @api
//  107  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function i2cStart
        THUMB
//  108 void i2cStart(I2CDriver *i2cp, const I2CConfig *config) {
i2cStart:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  109 
//  110   chDbgCheck((i2cp != NULL) && (config != NULL), "i2cStart");
        CMP      R4,#+0
        BEQ.N    ??i2cStart_0
        CMP      R5,#+0
        BNE.N    ??i2cStart_1
??i2cStart_0:
        MOVS     R2,#+110
        LDR.N    R1,??DataTable6
        LDR.N    R0,??DataTable6_1
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  111   chDbgAssert((i2cp->state == I2C_STOP) || (i2cp->state == I2C_READY) ||
//  112               (i2cp->state == I2C_LOCKED),
//  113               "i2cStart(), #1",
//  114               "invalid state");
??i2cStart_1:
        LDRB     R0,[R4, #+0]
        CMP      R0,#+1
        BEQ.N    ??i2cStart_2
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BEQ.N    ??i2cStart_2
        LDRB     R0,[R4, #+0]
        CMP      R0,#+5
        BEQ.N    ??i2cStart_2
        LDR.N    R0,??DataTable6_2
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  115 
//  116   chSysLock();
??i2cStart_2:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  117   i2cp->config = config;
        STR      R5,[R4, #+4]
//  118   i2c_lld_start(i2cp);
        MOVS     R0,R4
          CFI FunCall i2c_lld_start
        BL       i2c_lld_start
//  119   i2cp->state = I2C_READY;
        MOVS     R0,#+2
        STRB     R0,[R4, #+0]
//  120   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  121 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock2
//  122 
//  123 /**
//  124  * @brief   Deactivates the I2C peripheral.
//  125  *
//  126  * @param[in] i2cp      pointer to the @p I2CDriver object
//  127  *
//  128  * @api
//  129  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function i2cStop
        THUMB
//  130 void i2cStop(I2CDriver *i2cp) {
i2cStop:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  131 
//  132   chDbgCheck(i2cp != NULL, "i2cStop");
        CMP      R4,#+0
        BNE.N    ??i2cStop_0
        MOVS     R2,#+132
        LDR.N    R1,??DataTable6
        LDR.N    R0,??DataTable6_3
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  133   chDbgAssert((i2cp->state == I2C_STOP) || (i2cp->state == I2C_READY) ||
//  134               (i2cp->state == I2C_LOCKED),
//  135               "i2cStop(), #1",
//  136               "invalid state");
??i2cStop_0:
        LDRB     R0,[R4, #+0]
        CMP      R0,#+1
        BEQ.N    ??i2cStop_1
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BEQ.N    ??i2cStop_1
        LDRB     R0,[R4, #+0]
        CMP      R0,#+5
        BEQ.N    ??i2cStop_1
        LDR.N    R0,??DataTable6_4
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  137 
//  138   chSysLock();
??i2cStop_1:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  139   i2c_lld_stop(i2cp);
        MOVS     R0,R4
          CFI FunCall i2c_lld_stop
        BL       i2c_lld_stop
//  140   i2cp->state = I2C_STOP;
        MOVS     R0,#+1
        STRB     R0,[R4, #+0]
//  141   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  142 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock3
//  143 
//  144 /**
//  145  * @brief   Returns the errors mask associated to the previous operation.
//  146  *
//  147  * @param[in] i2cp      pointer to the @p I2CDriver object
//  148  * @return              The errors mask.
//  149  *
//  150  * @api
//  151  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function i2cGetErrors
        THUMB
//  152 i2cflags_t i2cGetErrors(I2CDriver *i2cp) {
i2cGetErrors:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  153 
//  154   chDbgCheck(i2cp != NULL, "i2cGetErrors");
        CMP      R4,#+0
        BNE.N    ??i2cGetErrors_0
        MOVS     R2,#+154
        LDR.N    R1,??DataTable6
        LDR.N    R0,??DataTable6_5
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  155 
//  156   return i2c_lld_get_errors(i2cp);
??i2cGetErrors_0:
        LDR      R0,[R4, #+8]
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock4
//  157 }
//  158 
//  159 /**
//  160  * @brief   Sends data via the I2C bus.
//  161  * @details Function designed to realize "read-through-write" transfer
//  162  *          paradigm. If you want transmit data without any further read,
//  163  *          than set @b rxbytes field to 0.
//  164  *
//  165  * @param[in] i2cp      pointer to the @p I2CDriver object
//  166  * @param[in] addr      slave device address (7 bits) without R/W bit
//  167  * @param[in] txbuf     pointer to transmit buffer
//  168  * @param[in] txbytes   number of bytes to be transmitted
//  169  * @param[out] rxbuf    pointer to receive buffer
//  170  * @param[in] rxbytes   number of bytes to be received, set it to 0 if
//  171  *                      you want transmit only
//  172  * @param[in] timeout   the number of ticks before the operation timeouts,
//  173  *                      the following special values are allowed:
//  174  *                      - @a TIME_INFINITE no timeout.
//  175  *                      .
//  176  *
//  177  * @return              The operation status.
//  178  * @retval RDY_OK       if the function succeeded.
//  179  * @retval RDY_RESET    if one or more I2C errors occurred, the errors can
//  180  *                      be retrieved using @p i2cGetErrors().
//  181  * @retval RDY_TIMEOUT  if a timeout occurred before operation end.
//  182  *
//  183  * @api
//  184  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function i2cMasterTransmitTimeout
        THUMB
//  185 msg_t i2cMasterTransmitTimeout(I2CDriver *i2cp,
//  186                                i2caddr_t addr,
//  187                                const uint8_t *txbuf,
//  188                                size_t txbytes,
//  189                                uint8_t *rxbuf,
//  190                                size_t rxbytes,
//  191                                systime_t timeout) {
i2cMasterTransmitTimeout:
        PUSH     {R1-R11,LR}
          CFI R14 Frame(CFA, -4)
          CFI R11 Frame(CFA, -8)
          CFI R10 Frame(CFA, -12)
          CFI R9 Frame(CFA, -16)
          CFI R8 Frame(CFA, -20)
          CFI R7 Frame(CFA, -24)
          CFI R6 Frame(CFA, -28)
          CFI R5 Frame(CFA, -32)
          CFI R4 Frame(CFA, -36)
          CFI CFA R13+48
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
        MOVS     R7,R3
        LDR      R8,[SP, #+48]
        LDR      R9,[SP, #+52]
        LDR      R10,[SP, #+56]
//  192   msg_t rdymsg;
//  193 
//  194   chDbgCheck((i2cp != NULL) && (addr != 0) &&
//  195              (txbytes > 0) && (txbuf != NULL) &&
//  196              ((rxbytes == 0) || ((rxbytes > 0) && (rxbuf != NULL))) &&
//  197              (timeout != TIME_IMMEDIATE),
//  198              "i2cMasterTransmitTimeout");
        CMP      R4,#+0
        BEQ.N    ??i2cMasterTransmitTimeout_0
        UXTH     R5,R5            ;; ZeroExt  R5,R5,#+16,#+16
        CMP      R5,#+0
        BEQ.N    ??i2cMasterTransmitTimeout_0
        CMP      R7,#+0
        BEQ.N    ??i2cMasterTransmitTimeout_0
        CMP      R6,#+0
        BEQ.N    ??i2cMasterTransmitTimeout_0
        CMP      R9,#+0
        BEQ.N    ??i2cMasterTransmitTimeout_1
        CMP      R9,#+0
        BEQ.N    ??i2cMasterTransmitTimeout_0
        CMP      R8,#+0
        BEQ.N    ??i2cMasterTransmitTimeout_0
??i2cMasterTransmitTimeout_1:
        CMP      R10,#+0
        BNE.N    ??i2cMasterTransmitTimeout_2
??i2cMasterTransmitTimeout_0:
        MOVS     R2,#+198
        LDR.N    R1,??DataTable6
        LDR.N    R0,??DataTable6_6
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  199 
//  200   chDbgAssert(i2cp->state == I2C_READY,
//  201               "i2cMasterTransmitTimeout(), #1", "not ready");
??i2cMasterTransmitTimeout_2:
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BEQ.N    ??i2cMasterTransmitTimeout_3
        LDR.N    R0,??DataTable6_7
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  202 
//  203   chSysLock();
??i2cMasterTransmitTimeout_3:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  204   i2cp->errors = I2CD_NO_ERROR;
        MOVS     R0,#+0
        STR      R0,[R4, #+8]
//  205   i2cp->state = I2C_ACTIVE_TX;
        MOVS     R0,#+3
        STRB     R0,[R4, #+0]
//  206   rdymsg = i2c_lld_master_transmit_timeout(i2cp, addr, txbuf, txbytes,
//  207                                            rxbuf, rxbytes, timeout);
        STR      R10,[SP, #+8]
        STR      R9,[SP, #+4]
        STR      R8,[SP, #+0]
        MOVS     R3,R7
        MOVS     R2,R6
        MOVS     R1,R5
        UXTH     R1,R1            ;; ZeroExt  R1,R1,#+16,#+16
        MOVS     R0,R4
          CFI FunCall i2c_lld_master_transmit_timeout
        BL       i2c_lld_master_transmit_timeout
        MOV      R11,R0
//  208   if (rdymsg == RDY_TIMEOUT)
        CMN      R11,#+1
        BNE.N    ??i2cMasterTransmitTimeout_4
//  209     i2cp->state = I2C_LOCKED;
        MOVS     R0,#+5
        STRB     R0,[R4, #+0]
        B.N      ??i2cMasterTransmitTimeout_5
//  210   else
//  211     i2cp->state = I2C_READY;
??i2cMasterTransmitTimeout_4:
        MOVS     R0,#+2
        STRB     R0,[R4, #+0]
//  212   chSysUnlock();
??i2cMasterTransmitTimeout_5:
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  213   return rdymsg;
        MOV      R0,R11
        POP      {R1-R11,PC}      ;; return
          CFI EndBlock cfiBlock5
//  214 }
//  215 
//  216 /**
//  217  * @brief   Receives data from the I2C bus.
//  218  *
//  219  * @param[in] i2cp      pointer to the @p I2CDriver object
//  220  * @param[in] addr      slave device address (7 bits) without R/W bit
//  221  * @param[out] rxbuf    pointer to receive buffer
//  222  * @param[in] rxbytes   number of bytes to be received
//  223  * @param[in] timeout   the number of ticks before the operation timeouts,
//  224  *                      the following special values are allowed:
//  225  *                      - @a TIME_INFINITE no timeout.
//  226  *                      .
//  227  *
//  228  * @return              The operation status.
//  229  * @retval RDY_OK       if the function succeeded.
//  230  * @retval RDY_RESET    if one or more I2C errors occurred, the errors can
//  231  *                      be retrieved using @p i2cGetErrors().
//  232  * @retval RDY_TIMEOUT  if a timeout occurred before operation end.
//  233  *
//  234  * @api
//  235  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function i2cMasterReceiveTimeout
        THUMB
//  236 msg_t i2cMasterReceiveTimeout(I2CDriver *i2cp,
//  237                               i2caddr_t addr,
//  238                               uint8_t *rxbuf,
//  239                               size_t rxbytes,
//  240                               systime_t timeout){
i2cMasterReceiveTimeout:
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
        MOVS     R5,R1
        MOVS     R6,R2
        MOVS     R7,R3
        LDR      R8,[SP, #+32]
//  241 
//  242   msg_t rdymsg;
//  243 
//  244   chDbgCheck((i2cp != NULL) && (addr != 0) &&
//  245              (rxbytes > 0) && (rxbuf != NULL) &&
//  246              (timeout != TIME_IMMEDIATE),
//  247              "i2cMasterReceiveTimeout");
        CMP      R4,#+0
        BEQ.N    ??i2cMasterReceiveTimeout_0
        UXTH     R5,R5            ;; ZeroExt  R5,R5,#+16,#+16
        CMP      R5,#+0
        BEQ.N    ??i2cMasterReceiveTimeout_0
        CMP      R7,#+0
        BEQ.N    ??i2cMasterReceiveTimeout_0
        CMP      R6,#+0
        BEQ.N    ??i2cMasterReceiveTimeout_0
        CMP      R8,#+0
        BNE.N    ??i2cMasterReceiveTimeout_1
??i2cMasterReceiveTimeout_0:
        MOVS     R2,#+247
        LDR.N    R1,??DataTable6
        LDR.N    R0,??DataTable6_8
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  248 
//  249   chDbgAssert(i2cp->state == I2C_READY,
//  250               "i2cMasterReceive(), #1", "not ready");
??i2cMasterReceiveTimeout_1:
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BEQ.N    ??i2cMasterReceiveTimeout_2
        LDR.N    R0,??DataTable6_9
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  251 
//  252   chSysLock();
??i2cMasterReceiveTimeout_2:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  253   i2cp->errors = I2CD_NO_ERROR;
        MOVS     R0,#+0
        STR      R0,[R4, #+8]
//  254   i2cp->state = I2C_ACTIVE_RX;
        MOVS     R0,#+4
        STRB     R0,[R4, #+0]
//  255   rdymsg = i2c_lld_master_receive_timeout(i2cp, addr, rxbuf, rxbytes, timeout);
        STR      R8,[SP, #+0]
        MOVS     R3,R7
        MOVS     R2,R6
        MOVS     R1,R5
        UXTH     R1,R1            ;; ZeroExt  R1,R1,#+16,#+16
        MOVS     R0,R4
          CFI FunCall i2c_lld_master_receive_timeout
        BL       i2c_lld_master_receive_timeout
        MOV      R9,R0
//  256   if (rdymsg == RDY_TIMEOUT)
        CMN      R9,#+1
        BNE.N    ??i2cMasterReceiveTimeout_3
//  257     i2cp->state = I2C_LOCKED;
        MOVS     R0,#+5
        STRB     R0,[R4, #+0]
        B.N      ??i2cMasterReceiveTimeout_4
//  258   else
//  259     i2cp->state = I2C_READY;
??i2cMasterReceiveTimeout_3:
        MOVS     R0,#+2
        STRB     R0,[R4, #+0]
//  260   chSysUnlock();
??i2cMasterReceiveTimeout_4:
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  261   return rdymsg;
        MOV      R0,R9
        POP      {R1,R4-R9,PC}    ;; return
          CFI EndBlock cfiBlock6
//  262 }
//  263 
//  264 #if I2C_USE_MUTUAL_EXCLUSION || defined(__DOXYGEN__)
//  265 /**
//  266  * @brief   Gains exclusive access to the I2C bus.
//  267  * @details This function tries to gain ownership to the I2C bus, if the bus
//  268  *          is already being used then the invoking thread is queued.
//  269  * @pre     In order to use this function the option @p I2C_USE_MUTUAL_EXCLUSION
//  270  *          must be enabled.
//  271  *
//  272  * @param[in] i2cp      pointer to the @p I2CDriver object
//  273  *
//  274  * @api
//  275  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function i2cAcquireBus
        THUMB
//  276 void i2cAcquireBus(I2CDriver *i2cp) {
i2cAcquireBus:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  277 
//  278   chDbgCheck(i2cp != NULL, "i2cAcquireBus");
        CMP      R4,#+0
        BNE.N    ??i2cAcquireBus_0
        MOV      R2,#+278
        LDR.N    R1,??DataTable6
        LDR.N    R0,??DataTable6_10
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  279 
//  280 #if CH_USE_MUTEXES
//  281   chMtxLock(&i2cp->mutex);
??i2cAcquireBus_0:
        ADDS     R0,R4,#+12
          CFI FunCall chMtxLock
        BL       chMtxLock
//  282 #elif CH_USE_SEMAPHORES
//  283   chSemWait(&i2cp->semaphore);
//  284 #endif
//  285 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock7
//  286 
//  287 /**
//  288  * @brief   Releases exclusive access to the I2C bus.
//  289  * @pre     In order to use this function the option @p I2C_USE_MUTUAL_EXCLUSION
//  290  *          must be enabled.
//  291  *
//  292  * @param[in] i2cp      pointer to the @p I2CDriver object
//  293  *
//  294  * @api
//  295  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function i2cReleaseBus
        THUMB
//  296 void i2cReleaseBus(I2CDriver *i2cp) {
i2cReleaseBus:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  297 
//  298   chDbgCheck(i2cp != NULL, "i2cReleaseBus");
        CMP      R4,#+0
        BNE.N    ??i2cReleaseBus_0
        MOV      R2,#+298
        LDR.N    R1,??DataTable6
        LDR.N    R0,??DataTable6_11
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  299 
//  300 #if CH_USE_MUTEXES
//  301   chMtxUnlock();
??i2cReleaseBus_0:
          CFI FunCall chMtxUnlock
        BL       chMtxUnlock
//  302 #elif CH_USE_SEMAPHORES
//  303   chSemSignal(&i2cp->semaphore);
//  304 #endif
//  305 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock8

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6:
        DC32     `?<Constant "F:\\\\stuff\\\\rusefi_sourc...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_1:
        DC32     `?<Constant "\\"i2cStart\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_2:
        DC32     `?<Constant "i2cStart(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_3:
        DC32     `?<Constant "\\"i2cStop\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_4:
        DC32     `?<Constant "i2cStop(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_5:
        DC32     `?<Constant "\\"i2cGetErrors\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_6:
        DC32     `?<Constant "\\"i2cMasterTransmitTim...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_7:
        DC32     `?<Constant "i2cMasterTransmitTime...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_8:
        DC32     `?<Constant "\\"i2cMasterReceiveTime...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_9:
        DC32     `?<Constant "i2cMasterReceive(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_10:
        DC32     `?<Constant "\\"i2cAcquireBus\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_11:
        DC32     `?<Constant "\\"i2cReleaseBus\\"()">`

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"i2cStart\\"()">`:
        DATA
        DC8 "\"i2cStart\"()"
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
        DC8 69H, 32H, 63H, 2EH, 63H, 0
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "i2cStart(), #1">`:
        DATA
        DC8 "i2cStart(), #1"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"i2cStop\\"()">`:
        DATA
        DC8 "\"i2cStop\"()"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "i2cStop(), #1">`:
        DATA
        DC8 "i2cStop(), #1"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"i2cGetErrors\\"()">`:
        DATA
        DC8 "\"i2cGetErrors\"()"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"i2cMasterTransmitTim...">`:
        DATA
        DC8 "\"i2cMasterTransmitTimeout\"()"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "i2cMasterTransmitTime...">`:
        DATA
        DC8 "i2cMasterTransmitTimeout(), #1"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"i2cMasterReceiveTime...">`:
        DATA
        DC8 "\"i2cMasterReceiveTimeout\"()"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "i2cMasterReceive(), #1">`:
        DATA
        DC8 "i2cMasterReceive(), #1"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"i2cAcquireBus\\"()">`:
        DATA
        DC8 "\"i2cAcquireBus\"()"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"i2cReleaseBus\\"()">`:
        DATA
        DC8 "\"i2cReleaseBus\"()"
        DC8 0, 0

        END
//  306 #endif /* I2C_USE_MUTUAL_EXCLUSION */
//  307 
//  308 #endif /* HAL_USE_I2C */
//  309 
//  310 /** @} */
// 
// 300 bytes in section .rodata
// 610 bytes in section .text
// 
// 610 bytes of CODE  memory
// 300 bytes of CONST memory
//
//Errors: none
//Warnings: none
