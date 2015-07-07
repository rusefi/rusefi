///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:20 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\kern /
//                       el\src\chdynamic.c                                   /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\kern /
//                       el\src\chdynamic.c -lCN F:\stuff\rusefi_sourceforge\ /
//                       firmware\iar\Debug\List\ -lA                         /
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
//                       chdynamic.s                                          /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME chdynamic

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
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\kernel\src\chdynamic.c
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
//   29  * @file    chdynamic.c
//   30  * @brief   Dynamic threads code.
//   31  *
//   32  * @addtogroup dynamic_threads
//   33  * @details Dynamic threads related APIs and services.
//   34  * @{
//   35  */
//   36 
//   37 #include "ch.h"
//   38 
//   39 #if CH_USE_DYNAMIC || defined(__DOXYGEN__)
//   40 
//   41 /**
//   42  * @brief   Adds a reference to a thread object.
//   43  * @pre     The configuration option @p CH_USE_DYNAMIC must be enabled in order
//   44  *          to use this function.
//   45  *
//   46  * @param[in] tp        pointer to the thread
//   47  * @return              The same thread pointer passed as parameter
//   48  *                      representing the new reference.
//   49  *
//   50  * @api
//   51  */
//   52 Thread *chThdAddRef(Thread *tp) {
//   53 
//   54   chSysLock();
//   55   chDbgAssert(tp->p_refs < 255, "chThdAddRef(), #1", "too many references");
//   56   tp->p_refs++;
//   57   chSysUnlock();
//   58   return tp;
//   59 }
//   60 
//   61 /**
//   62  * @brief   Releases a reference to a thread object.
//   63  * @details If the references counter reaches zero <b>and</b> the thread
//   64  *          is in the @p THD_STATE_FINAL state then the thread's memory is
//   65  *          returned to the proper allocator.
//   66  * @pre     The configuration option @p CH_USE_DYNAMIC must be enabled in order
//   67  *          to use this function.
//   68  * @note    Static threads are not affected.
//   69  *
//   70  * @param[in] tp        pointer to the thread
//   71  *
//   72  * @api
//   73  */
//   74 void chThdRelease(Thread *tp) {
//   75   trefs_t refs;
//   76 
//   77   chSysLock();
//   78   chDbgAssert(tp->p_refs > 0, "chThdRelease(), #1", "not referenced");
//   79   refs = --tp->p_refs;
//   80   chSysUnlock();
//   81 
//   82   /* If the references counter reaches zero and the thread is in its
//   83      terminated state then the memory can be returned to the proper
//   84      allocator. Of course static threads are not affected.*/
//   85   if ((refs == 0) && (tp->p_state == THD_STATE_FINAL)) {
//   86     switch (tp->p_flags & THD_MEM_MODE_MASK) {
//   87 #if CH_USE_HEAP
//   88     case THD_MEM_MODE_HEAP:
//   89 #if CH_USE_REGISTRY
//   90       REG_REMOVE(tp);
//   91 #endif
//   92       chHeapFree(tp);
//   93       break;
//   94 #endif
//   95 #if CH_USE_MEMPOOLS
//   96     case THD_MEM_MODE_MEMPOOL:
//   97 #if CH_USE_REGISTRY
//   98       REG_REMOVE(tp);
//   99 #endif
//  100       chPoolFree(tp->p_mpool, tp);
//  101       break;
//  102 #endif
//  103     }
//  104   }
//  105 }
//  106 
//  107 #if CH_USE_HEAP || defined(__DOXYGEN__)
//  108 /**
//  109  * @brief   Creates a new thread allocating the memory from the heap.
//  110  * @pre     The configuration options @p CH_USE_DYNAMIC and @p CH_USE_HEAP
//  111  *          must be enabled in order to use this function.
//  112  * @note    A thread can terminate by calling @p chThdExit() or by simply
//  113  *          returning from its main function.
//  114  * @note    The memory allocated for the thread is not released when the thread
//  115  *          terminates but when a @p chThdWait() is performed.
//  116  *
//  117  * @param[in] heapp     heap from which allocate the memory or @p NULL for the
//  118  *                      default heap
//  119  * @param[in] size      size of the working area to be allocated
//  120  * @param[in] prio      the priority level for the new thread
//  121  * @param[in] pf        the thread function
//  122  * @param[in] arg       an argument passed to the thread function. It can be
//  123  *                      @p NULL.
//  124  * @return              The pointer to the @p Thread structure allocated for
//  125  *                      the thread into the working space area.
//  126  * @retval NULL         if the memory cannot be allocated.
//  127  *
//  128  * @api
//  129  */
//  130 Thread *chThdCreateFromHeap(MemoryHeap *heapp, size_t size,
//  131                             tprio_t prio, tfunc_t pf, void *arg) {
//  132   void *wsp;
//  133   Thread *tp;
//  134 
//  135   wsp = chHeapAlloc(heapp, size);
//  136   if (wsp == NULL)
//  137     return NULL;
//  138   
//  139 #if CH_DBG_FILL_THREADS
//  140   _thread_memfill((uint8_t *)wsp,
//  141                   (uint8_t *)wsp + sizeof(Thread),
//  142                   CH_THREAD_FILL_VALUE);
//  143   _thread_memfill((uint8_t *)wsp + sizeof(Thread),
//  144                   (uint8_t *)wsp + size,
//  145                   CH_STACK_FILL_VALUE);
//  146 #endif
//  147   
//  148   chSysLock();
//  149   tp = chThdCreateI(wsp, size, prio, pf, arg);
//  150   tp->p_flags = THD_MEM_MODE_HEAP;
//  151   chSchWakeupS(tp, RDY_OK);
//  152   chSysUnlock();
//  153   return tp;
//  154 }
//  155 #endif /* CH_USE_HEAP */
//  156 
//  157 #if CH_USE_MEMPOOLS || defined(__DOXYGEN__)
//  158 /**
//  159  * @brief   Creates a new thread allocating the memory from the specified
//  160  *          memory pool.
//  161  * @pre     The configuration options @p CH_USE_DYNAMIC and @p CH_USE_MEMPOOLS
//  162  *          must be enabled in order to use this function.
//  163  * @note    A thread can terminate by calling @p chThdExit() or by simply
//  164  *          returning from its main function.
//  165  * @note    The memory allocated for the thread is not released when the thread
//  166  *          terminates but when a @p chThdWait() is performed.
//  167  *
//  168  * @param[in] mp        pointer to the memory pool object
//  169  * @param[in] prio      the priority level for the new thread
//  170  * @param[in] pf        the thread function
//  171  * @param[in] arg       an argument passed to the thread function. It can be
//  172  *                      @p NULL.
//  173  * @return              The pointer to the @p Thread structure allocated for
//  174  *                      the thread into the working space area.
//  175  * @retval  NULL        if the memory pool is empty.
//  176  *
//  177  * @api
//  178  */
//  179 Thread *chThdCreateFromMemoryPool(MemoryPool *mp, tprio_t prio,
//  180                                   tfunc_t pf, void *arg) {
//  181   void *wsp;
//  182   Thread *tp;
//  183 
//  184   chDbgCheck(mp != NULL, "chThdCreateFromMemoryPool");
//  185 
//  186   wsp = chPoolAlloc(mp);
//  187   if (wsp == NULL)
//  188     return NULL;
//  189   
//  190 #if CH_DBG_FILL_THREADS
//  191   _thread_memfill((uint8_t *)wsp,
//  192                   (uint8_t *)wsp + sizeof(Thread),
//  193                   CH_THREAD_FILL_VALUE);
//  194   _thread_memfill((uint8_t *)wsp + sizeof(Thread),
//  195                   (uint8_t *)wsp + mp->mp_object_size,
//  196                   CH_STACK_FILL_VALUE);
//  197 #endif
//  198 
//  199   chSysLock();
//  200   tp = chThdCreateI(wsp, mp->mp_object_size, prio, pf, arg);
//  201   tp->p_flags = THD_MEM_MODE_MEMPOOL;
//  202   tp->p_mpool = mp;
//  203   chSchWakeupS(tp, RDY_OK);
//  204   chSysUnlock();
//  205   return tp;
//  206 }
//  207 #endif /* CH_USE_MEMPOOLS */
//  208 
//  209 #endif /* CH_USE_DYNAMIC */
//  210 
//  211 /** @} */
// 
//
// 
//
//
//Errors: none
//Warnings: none
