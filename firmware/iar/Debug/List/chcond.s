///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:41 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\kern /
//                       el\src\chcond.c                                      /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\kern /
//                       el\src\chcond.c -lCN F:\stuff\rusefi_sourceforge\fir /
//                       mware\iar\Debug\List\ -lA                            /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\Debug\List\ /
//                        -o F:\stuff\rusefi_sourceforge\firmware\iar\Debug\O /
//                       bj\ --no_cse --no_unroll --no_inline                 /
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
//                       chcond.s                                             /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME chcond

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
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\kernel\src\chcond.c
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
//   28    Concepts and parts of this file have been contributed by Leon Woestenberg.
//   29  */
//   30 
//   31 /**
//   32  * @file    chcond.c
//   33  * @brief   Condition Variables code.
//   34  *
//   35  * @addtogroup condvars Condition Variables
//   36  * @details This module implements the Condition Variables mechanism. Condition
//   37  *          variables are an extensions to the Mutex subsystem and cannot
//   38  *          work alone.
//   39  *          <h2>Operation mode</h2>
//   40  *          The condition variable is a synchronization object meant to be
//   41  *          used inside a zone protected by a @p Mutex. Mutexes and CondVars
//   42  *          together can implement a Monitor construct.
//   43  * @pre     In order to use the condition variable APIs the @p CH_USE_CONDVARS
//   44  *          option must be enabled in @p chconf.h.
//   45  * @{
//   46  */
//   47 
//   48 #include "ch.h"
//   49 
//   50 #if (CH_USE_CONDVARS && CH_USE_MUTEXES) || defined(__DOXYGEN__)
//   51 
//   52 /**
//   53  * @brief   Initializes s @p CondVar structure.
//   54  *
//   55  * @param[out] cp       pointer to a @p CondVar structure
//   56  *
//   57  * @init
//   58  */
//   59 void chCondInit(CondVar *cp) {
//   60 
//   61   chDbgCheck(cp != NULL, "chCondInit");
//   62 
//   63   queue_init(&cp->c_queue);
//   64 }
//   65 
//   66 /**
//   67  * @brief   Signals one thread that is waiting on the condition variable.
//   68  *
//   69  * @param[in] cp        pointer to the @p CondVar structure
//   70  *
//   71  * @api
//   72  */
//   73 void chCondSignal(CondVar *cp) {
//   74 
//   75   chDbgCheck(cp != NULL, "chCondSignal");
//   76 
//   77   chSysLock();
//   78   if (notempty(&cp->c_queue))
//   79     chSchWakeupS(fifo_remove(&cp->c_queue), RDY_OK);
//   80   chSysUnlock();
//   81 }
//   82 
//   83 /**
//   84  * @brief   Signals one thread that is waiting on the condition variable.
//   85  * @post    This function does not reschedule so a call to a rescheduling
//   86  *          function must be performed before unlocking the kernel. Note that
//   87  *          interrupt handlers always reschedule on exit so an explicit
//   88  *          reschedule must not be performed in ISRs.
//   89  *
//   90  * @param[in] cp        pointer to the @p CondVar structure
//   91  *
//   92  * @iclass
//   93  */
//   94 void chCondSignalI(CondVar *cp) {
//   95 
//   96   chDbgCheckClassI();
//   97   chDbgCheck(cp != NULL, "chCondSignalI");
//   98 
//   99   if (notempty(&cp->c_queue))
//  100     chSchReadyI(fifo_remove(&cp->c_queue))->p_u.rdymsg = RDY_OK;
//  101 }
//  102 
//  103 /**
//  104  * @brief   Signals all threads that are waiting on the condition variable.
//  105  *
//  106  * @param[in] cp        pointer to the @p CondVar structure
//  107  *
//  108  * @api
//  109  */
//  110 void chCondBroadcast(CondVar *cp) {
//  111 
//  112   chSysLock();
//  113   chCondBroadcastI(cp);
//  114   chSchRescheduleS();
//  115   chSysUnlock();
//  116 }
//  117 
//  118 /**
//  119  * @brief   Signals all threads that are waiting on the condition variable.
//  120  * @post    This function does not reschedule so a call to a rescheduling
//  121  *          function must be performed before unlocking the kernel. Note that
//  122  *          interrupt handlers always reschedule on exit so an explicit
//  123  *          reschedule must not be performed in ISRs.
//  124  *
//  125  * @param[in] cp        pointer to the @p CondVar structure
//  126  *
//  127  * @iclass
//  128  */
//  129 void chCondBroadcastI(CondVar *cp) {
//  130 
//  131   chDbgCheckClassI();
//  132   chDbgCheck(cp != NULL, "chCondBroadcastI");
//  133 
//  134   /* Empties the condition variable queue and inserts all the Threads into the
//  135      ready list in FIFO order. The wakeup message is set to @p RDY_RESET in
//  136      order to make a chCondBroadcast() detectable from a chCondSignal().*/
//  137   while (cp->c_queue.p_next != (void *)&cp->c_queue)
//  138     chSchReadyI(fifo_remove(&cp->c_queue))->p_u.rdymsg = RDY_RESET;
//  139 }
//  140 
//  141 /**
//  142  * @brief   Waits on the condition variable releasing the mutex lock.
//  143  * @details Releases the currently owned mutex, waits on the condition
//  144  *          variable, and finally acquires the mutex again. All the sequence
//  145  *          is performed atomically.
//  146  * @pre     The invoking thread <b>must</b> have at least one owned mutex.
//  147  *
//  148  * @param[in] cp        pointer to the @p CondVar structure
//  149  * @return              A message specifying how the invoking thread has been
//  150  *                      released from the condition variable.
//  151  * @retval RDY_OK       if the condvar has been signaled using
//  152  *                      @p chCondSignal().
//  153  * @retval RDY_RESET    if the condvar has been signaled using
//  154  *                      @p chCondBroadcast().
//  155  *
//  156  * @api
//  157  */
//  158 msg_t chCondWait(CondVar *cp) {
//  159   msg_t msg;
//  160 
//  161   chSysLock();
//  162   msg = chCondWaitS(cp);
//  163   chSysUnlock();
//  164   return msg;
//  165 }
//  166 
//  167 /**
//  168  * @brief   Waits on the condition variable releasing the mutex lock.
//  169  * @details Releases the currently owned mutex, waits on the condition
//  170  *          variable, and finally acquires the mutex again. All the sequence
//  171  *          is performed atomically.
//  172  * @pre     The invoking thread <b>must</b> have at least one owned mutex.
//  173  *
//  174  * @param[in] cp        pointer to the @p CondVar structure
//  175  * @return              A message specifying how the invoking thread has been
//  176  *                      released from the condition variable.
//  177  * @retval RDY_OK       if the condvar has been signaled using
//  178  *                      @p chCondSignal().
//  179  * @retval RDY_RESET    if the condvar has been signaled using
//  180  *                      @p chCondBroadcast().
//  181  *
//  182  * @sclass
//  183  */
//  184 msg_t chCondWaitS(CondVar *cp) {
//  185   Thread *ctp = currp;
//  186   Mutex *mp;
//  187   msg_t msg;
//  188 
//  189   chDbgCheckClassS();
//  190   chDbgCheck(cp != NULL, "chCondWaitS");
//  191   chDbgAssert(ctp->p_mtxlist != NULL,
//  192               "chCondWaitS(), #1",
//  193               "not owning a mutex");
//  194 
//  195   mp = chMtxUnlockS();
//  196   ctp->p_u.wtobjp = cp;
//  197   prio_insert(ctp, &cp->c_queue);
//  198   chSchGoSleepS(THD_STATE_WTCOND);
//  199   msg = ctp->p_u.rdymsg;
//  200   chMtxLockS(mp);
//  201   return msg;
//  202 }
//  203 
//  204 #if CH_USE_CONDVARS_TIMEOUT || defined(__DOXYGEN__)
//  205 /**
//  206  * @brief   Waits on the condition variable releasing the mutex lock.
//  207  * @details Releases the currently owned mutex, waits on the condition
//  208  *          variable, and finally acquires the mutex again. All the sequence
//  209  *          is performed atomically.
//  210  * @pre     The invoking thread <b>must</b> have at least one owned mutex.
//  211  * @pre     The configuration option @p CH_USE_CONDVARS_TIMEOUT must be enabled
//  212  *          in order to use this function.
//  213  * @post    Exiting the function because a timeout does not re-acquire the
//  214  *          mutex, the mutex ownership is lost.
//  215  *
//  216  * @param[in] cp        pointer to the @p CondVar structure
//  217  * @param[in] time      the number of ticks before the operation timeouts, the
//  218  *                      special values are handled as follow:
//  219  *                      - @a TIME_INFINITE no timeout.
//  220  *                      - @a TIME_IMMEDIATE this value is not allowed.
//  221  *                      .
//  222  * @return              A message specifying how the invoking thread has been
//  223  *                      released from the condition variable.
//  224  * @retval RDY_OK       if the condvar has been signaled using
//  225  *                      @p chCondSignal().
//  226  * @retval RDY_RESET    if the condvar has been signaled using
//  227  *                      @p chCondBroadcast().
//  228  * @retval RDY_TIMEOUT  if the condvar has not been signaled within the
//  229  *                      specified timeout.
//  230  *
//  231  * @api
//  232  */
//  233 msg_t chCondWaitTimeout(CondVar *cp, systime_t time) {
//  234   msg_t msg;
//  235 
//  236   chSysLock();
//  237   msg = chCondWaitTimeoutS(cp, time);
//  238   chSysUnlock();
//  239   return msg;
//  240 }
//  241 
//  242 /**
//  243  * @brief   Waits on the condition variable releasing the mutex lock.
//  244  * @details Releases the currently owned mutex, waits on the condition
//  245  *          variable, and finally acquires the mutex again. All the sequence
//  246  *          is performed atomically.
//  247  * @pre     The invoking thread <b>must</b> have at least one owned mutex.
//  248  * @pre     The configuration option @p CH_USE_CONDVARS_TIMEOUT must be enabled
//  249  *          in order to use this function.
//  250  * @post    Exiting the function because a timeout does not re-acquire the
//  251  *          mutex, the mutex ownership is lost.
//  252  *
//  253  * @param[in] cp        pointer to the @p CondVar structure
//  254  * @param[in] time      the number of ticks before the operation timeouts, the
//  255  *                      special values are handled as follow:
//  256  *                      - @a TIME_INFINITE no timeout.
//  257  *                      - @a TIME_IMMEDIATE this value is not allowed.
//  258  *                      .
//  259  * @return              A message specifying how the invoking thread has been
//  260  *                      released from the condition variable.
//  261  * @retval RDY_OK       if the condvar has been signaled using
//  262  *                      @p chCondSignal().
//  263  * @retval RDY_RESET    if the condvar has been signaled using
//  264  *                      @p chCondBroadcast().
//  265  * @retval RDY_TIMEOUT  if the condvar has not been signaled within the
//  266  *                      specified timeout.
//  267  *
//  268  * @sclass
//  269  */
//  270 msg_t chCondWaitTimeoutS(CondVar *cp, systime_t time) {
//  271   Mutex *mp;
//  272   msg_t msg;
//  273 
//  274   chDbgCheckClassS();
//  275   chDbgCheck((cp != NULL) && (time != TIME_IMMEDIATE), "chCondWaitTimeoutS");
//  276   chDbgAssert(currp->p_mtxlist != NULL,
//  277               "chCondWaitTimeoutS(), #1",
//  278               "not owning a mutex");
//  279 
//  280   mp = chMtxUnlockS();
//  281   currp->p_u.wtobjp = cp;
//  282   prio_insert(currp, &cp->c_queue);
//  283   msg = chSchGoSleepTimeoutS(THD_STATE_WTCOND, time);
//  284   if (msg != RDY_TIMEOUT)
//  285     chMtxLockS(mp);
//  286   return msg;
//  287 }
//  288 #endif /* CH_USE_CONDVARS_TIMEOUT */
//  289 
//  290 #endif /* CH_USE_CONDVARS && CH_USE_MUTEXES */
//  291 
//  292 /** @} */
// 
//
// 
//
//
//Errors: none
//Warnings: none
