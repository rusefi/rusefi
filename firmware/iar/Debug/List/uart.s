///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:10:05 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       src\uart.c                                           /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       src\uart.c -lCN F:\stuff\rusefi_sourceforge\firmware /
//                       \iar\Debug\List\ -lA F:\stuff\rusefi_sourceforge\fir /
//                       mware\iar\Debug\List\ -o F:\stuff\rusefi_sourceforge /
//                       \firmware\iar\Debug\Obj\ --no_cse --no_unroll        /
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
//                       uart.s                                               /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME uart

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1


        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        END
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\src\uart.c
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
//   29  * @file    uart.c
//   30  * @brief   UART Driver code.
//   31  *
//   32  * @addtogroup UART
//   33  * @{
//   34  */
//   35 
//   36 #include "ch.h"
//   37 #include "hal.h"
//   38 
//   39 #if HAL_USE_UART || defined(__DOXYGEN__)
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
//   62  * @brief   UART Driver initialization.
//   63  * @note    This function is implicitly invoked by @p halInit(), there is
//   64  *          no need to explicitly initialize the driver.
//   65  *
//   66  * @init
//   67  */
//   68 void uartInit(void) {
//   69 
//   70   uart_lld_init();
//   71 }
//   72 
//   73 /**
//   74  * @brief   Initializes the standard part of a @p UARTDriver structure.
//   75  *
//   76  * @param[out] uartp    pointer to the @p UARTDriver object
//   77  *
//   78  * @init
//   79  */
//   80 void uartObjectInit(UARTDriver *uartp) {
//   81 
//   82   uartp->state   = UART_STOP;
//   83   uartp->txstate = UART_TX_IDLE;
//   84   uartp->rxstate = UART_RX_IDLE;
//   85   uartp->config  = NULL;
//   86   /* Optional, user-defined initializer.*/
//   87 #if defined(UART_DRIVER_EXT_INIT_HOOK)
//   88   UART_DRIVER_EXT_INIT_HOOK(uartp);
//   89 #endif
//   90 }
//   91 
//   92 /**
//   93  * @brief   Configures and activates the UART peripheral.
//   94  *
//   95  * @param[in] uartp     pointer to the @p UARTDriver object
//   96  * @param[in] config    pointer to the @p UARTConfig object
//   97  *
//   98  * @api
//   99  */
//  100 void uartStart(UARTDriver *uartp, const UARTConfig *config) {
//  101 
//  102   chDbgCheck((uartp != NULL) && (config != NULL), "uartStart");
//  103 
//  104   chSysLock();
//  105   chDbgAssert((uartp->state == UART_STOP) || (uartp->state == UART_READY),
//  106               "uartStart(), #1", "invalid state");
//  107 
//  108   uartp->config = config;
//  109   uart_lld_start(uartp);
//  110   uartp->state = UART_READY;
//  111   chSysUnlock();
//  112 }
//  113 
//  114 /**
//  115  * @brief   Deactivates the UART peripheral.
//  116  *
//  117  * @param[in] uartp     pointer to the @p UARTDriver object
//  118  *
//  119  * @api
//  120  */
//  121 void uartStop(UARTDriver *uartp) {
//  122 
//  123   chDbgCheck(uartp != NULL, "uartStop");
//  124 
//  125   chSysLock();
//  126   chDbgAssert((uartp->state == UART_STOP) || (uartp->state == UART_READY),
//  127               "uartStop(), #1", "invalid state");
//  128 
//  129   uart_lld_stop(uartp);
//  130   uartp->state = UART_STOP;
//  131   uartp->txstate = UART_TX_IDLE;
//  132   uartp->rxstate = UART_RX_IDLE;
//  133   chSysUnlock();
//  134 }
//  135 
//  136 /**
//  137  * @brief   Starts a transmission on the UART peripheral.
//  138  * @note    The buffers are organized as uint8_t arrays for data sizes below
//  139  *          or equal to 8 bits else it is organized as uint16_t arrays.
//  140  *
//  141  * @param[in] uartp     pointer to the @p UARTDriver object
//  142  * @param[in] n         number of data frames to send
//  143  * @param[in] txbuf     the pointer to the transmit buffer
//  144  *
//  145  * @api
//  146  */
//  147 void uartStartSend(UARTDriver *uartp, size_t n, const void *txbuf) {
//  148 
//  149   chDbgCheck((uartp != NULL) && (n > 0) && (txbuf != NULL),
//  150              "uartStartSend");
//  151              
//  152   chSysLock();
//  153   chDbgAssert(uartp->state == UART_READY,
//  154               "uartStartSend(), #1", "is active");
//  155   chDbgAssert(uartp->txstate != UART_TX_ACTIVE,
//  156               "uartStartSend(), #2", "tx active");
//  157 
//  158   uart_lld_start_send(uartp, n, txbuf);
//  159   uartp->txstate = UART_TX_ACTIVE;
//  160   chSysUnlock();
//  161 }
//  162 
//  163 /**
//  164  * @brief   Starts a transmission on the UART peripheral.
//  165  * @note    The buffers are organized as uint8_t arrays for data sizes below
//  166  *          or equal to 8 bits else it is organized as uint16_t arrays.
//  167  * @note    This function has to be invoked from a lock zone.
//  168  *
//  169  * @param[in] uartp     pointer to the @p UARTDriver object
//  170  * @param[in] n         number of data frames to send
//  171  * @param[in] txbuf     the pointer to the transmit buffer
//  172  *
//  173  * @iclass
//  174  */
//  175 void uartStartSendI(UARTDriver *uartp, size_t n, const void *txbuf) {
//  176 
//  177   chDbgCheckClassI();
//  178   chDbgCheck((uartp != NULL) && (n > 0) && (txbuf != NULL),
//  179              "uartStartSendI");
//  180   chDbgAssert(uartp->state == UART_READY,
//  181               "uartStartSendI(), #1", "is active");
//  182   chDbgAssert(uartp->txstate != UART_TX_ACTIVE,
//  183               "uartStartSendI(), #2", "tx active");
//  184 
//  185   uart_lld_start_send(uartp, n, txbuf);
//  186   uartp->txstate = UART_TX_ACTIVE;
//  187 }
//  188 
//  189 /**
//  190  * @brief   Stops any ongoing transmission.
//  191  * @note    Stopping a transmission also suppresses the transmission callbacks.
//  192  *
//  193  * @param[in] uartp     pointer to the @p UARTDriver object
//  194  *
//  195  * @return              The number of data frames not transmitted by the
//  196  *                      stopped transmit operation.
//  197  * @retval 0            There was no transmit operation in progress.
//  198  *
//  199  * @api
//  200  */
//  201 size_t uartStopSend(UARTDriver *uartp) {
//  202   size_t n;
//  203 
//  204   chDbgCheck(uartp != NULL, "uartStopSend");
//  205 
//  206   chSysLock();
//  207   chDbgAssert(uartp->state == UART_READY, "uartStopSend(), #1", "not active");
//  208 
//  209   if (uartp->txstate == UART_TX_ACTIVE) {
//  210     n = uart_lld_stop_send(uartp);
//  211     uartp->txstate = UART_TX_IDLE;
//  212   }
//  213   else
//  214     n = 0;
//  215   chSysUnlock();
//  216   return n;
//  217 }
//  218 
//  219 /**
//  220  * @brief   Stops any ongoing transmission.
//  221  * @note    Stopping a transmission also suppresses the transmission callbacks.
//  222  * @note    This function has to be invoked from a lock zone.
//  223  *
//  224  * @param[in] uartp     pointer to the @p UARTDriver object
//  225  *
//  226  * @return              The number of data frames not transmitted by the
//  227  *                      stopped transmit operation.
//  228  * @retval 0            There was no transmit operation in progress.
//  229  *
//  230  * @iclass
//  231  */
//  232 size_t uartStopSendI(UARTDriver *uartp) {
//  233 
//  234   chDbgCheckClassI();
//  235   chDbgCheck(uartp != NULL, "uartStopSendI");
//  236   chDbgAssert(uartp->state == UART_READY, "uartStopSendI(), #1", "not active");
//  237 
//  238   if (uartp->txstate == UART_TX_ACTIVE) {
//  239     size_t n = uart_lld_stop_send(uartp);
//  240     uartp->txstate = UART_TX_IDLE;
//  241     return n;
//  242   }
//  243   return 0;
//  244 }
//  245 
//  246 /**
//  247  * @brief   Starts a receive operation on the UART peripheral.
//  248  * @note    The buffers are organized as uint8_t arrays for data sizes below
//  249  *          or equal to 8 bits else it is organized as uint16_t arrays.
//  250  *
//  251  * @param[in] uartp     pointer to the @p UARTDriver object
//  252  * @param[in] n         number of data frames to send
//  253  * @param[in] rxbuf     the pointer to the receive buffer
//  254  *
//  255  * @api
//  256  */
//  257 void uartStartReceive(UARTDriver *uartp, size_t n, void *rxbuf) {
//  258 
//  259   chDbgCheck((uartp != NULL) && (n > 0) && (rxbuf != NULL),
//  260              "uartStartReceive");
//  261 
//  262   chSysLock();
//  263   chDbgAssert(uartp->state == UART_READY,
//  264               "uartStartReceive(), #1", "is active");
//  265   chDbgAssert(uartp->rxstate != UART_RX_ACTIVE,
//  266               "uartStartReceive(), #2", "rx active");
//  267 
//  268   uart_lld_start_receive(uartp, n, rxbuf);
//  269   uartp->rxstate = UART_RX_ACTIVE;
//  270   chSysUnlock();
//  271 }
//  272 
//  273 /**
//  274  * @brief   Starts a receive operation on the UART peripheral.
//  275  * @note    The buffers are organized as uint8_t arrays for data sizes below
//  276  *          or equal to 8 bits else it is organized as uint16_t arrays.
//  277  * @note    This function has to be invoked from a lock zone.
//  278  *
//  279  * @param[in] uartp     pointer to the @p UARTDriver object
//  280  * @param[in] n         number of data frames to send
//  281  * @param[out] rxbuf    the pointer to the receive buffer
//  282  *
//  283  * @iclass
//  284  */
//  285 void uartStartReceiveI(UARTDriver *uartp, size_t n, void *rxbuf) {
//  286 
//  287   chDbgCheckClassI();
//  288   chDbgCheck((uartp != NULL) && (n > 0) && (rxbuf != NULL),
//  289              "uartStartReceiveI");
//  290   chDbgAssert(uartp->state == UART_READY,
//  291               "uartStartReceiveI(), #1", "is active");
//  292   chDbgAssert(uartp->rxstate != UART_RX_ACTIVE,
//  293               "uartStartReceiveI(), #2", "rx active");
//  294 
//  295   uart_lld_start_receive(uartp, n, rxbuf);
//  296   uartp->rxstate = UART_RX_ACTIVE;
//  297 }
//  298 
//  299 /**
//  300  * @brief   Stops any ongoing receive operation.
//  301  * @note    Stopping a receive operation also suppresses the receive callbacks.
//  302  *
//  303  * @param[in] uartp     pointer to the @p UARTDriver object
//  304  *
//  305  * @return              The number of data frames not received by the
//  306  *                      stopped receive operation.
//  307  * @retval 0            There was no receive operation in progress.
//  308  *
//  309  * @api
//  310  */
//  311 size_t uartStopReceive(UARTDriver *uartp) {
//  312   size_t n;
//  313 
//  314   chDbgCheck(uartp != NULL, "uartStopReceive");
//  315 
//  316   chSysLock();
//  317   chDbgAssert(uartp->state == UART_READY,
//  318               "uartStopReceive(), #1", "not active");
//  319 
//  320   if (uartp->rxstate == UART_RX_ACTIVE) {
//  321     n = uart_lld_stop_receive(uartp);
//  322     uartp->rxstate = UART_RX_IDLE;
//  323   }
//  324   else
//  325     n = 0;
//  326   chSysUnlock();
//  327   return n;
//  328 }
//  329 
//  330 /**
//  331  * @brief   Stops any ongoing receive operation.
//  332  * @note    Stopping a receive operation also suppresses the receive callbacks.
//  333  * @note    This function has to be invoked from a lock zone.
//  334  *
//  335  * @param[in] uartp      pointer to the @p UARTDriver object
//  336  *
//  337  * @return              The number of data frames not received by the
//  338  *                      stopped receive operation.
//  339  * @retval 0            There was no receive operation in progress.
//  340  *
//  341  * @iclass
//  342  */
//  343 size_t uartStopReceiveI(UARTDriver *uartp) {
//  344 
//  345   chDbgCheckClassI();
//  346   chDbgCheck(uartp != NULL, "uartStopReceiveI");
//  347   chDbgAssert(uartp->state == UART_READY,
//  348               "uartStopReceiveI(), #1", "not active");
//  349 
//  350   if (uartp->rxstate == UART_RX_ACTIVE) {
//  351     size_t n = uart_lld_stop_receive(uartp);
//  352     uartp->rxstate = UART_RX_IDLE;
//  353     return n;
//  354   }
//  355   return 0;
//  356 }
//  357 
//  358 #endif /* HAL_USE_UART */
//  359 
//  360 /** @} */
// 
//
// 
//
//
//Errors: none
//Warnings: none
