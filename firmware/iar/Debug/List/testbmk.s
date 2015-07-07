///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:10:04 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\development\tes /
//                       t\testbmk.c                                          /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\development\tes /
//                       t\testbmk.c -lCN F:\stuff\rusefi_sourceforge\firmwar /
//                       e\iar\Debug\List\ -lA F:\stuff\rusefi_sourceforge\fi /
//                       rmware\iar\Debug\List\ -o                            /
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
//                       testbmk.s                                            /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME testbmk

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
// F:\stuff\rusefi_sourceforge\firmware\development\test\testbmk.c
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
//   16 
//   17 #include "main.h"
//   18 
//   19 #if EFI_PERF_METRICS
//   20 #include "test.h"
//   21 
//   22 /**
//   23  * @page test_benchmarks Kernel Benchmarks
//   24  *
//   25  * File: @ref testbmk.c
//   26  *
//   27  * <h2>Description</h2>
//   28  * This module implements a series of system benchmarks. The benchmarks are
//   29  * useful as a stress test and as a reference when comparing ChibiOS/RT
//   30  * with similar systems.
//   31  *
//   32  * <h2>Objective</h2>
//   33  * Objective of the test module is to provide a performance index for the
//   34  * most critical system subsystems. The performance numbers allow to
//   35  * discover performance regressions between successive ChibiOS/RT releases.
//   36  *
//   37  * <h2>Preconditions</h2>
//   38  * None.
//   39  *
//   40  * <h2>Test Cases</h2>
//   41  * - @subpage test_benchmarks_001
//   42  * - @subpage test_benchmarks_002
//   43  * - @subpage test_benchmarks_003
//   44  * - @subpage test_benchmarks_004
//   45  * - @subpage test_benchmarks_005
//   46  * - @subpage test_benchmarks_006
//   47  * - @subpage test_benchmarks_007
//   48  * - @subpage test_benchmarks_008
//   49  * - @subpage test_benchmarks_009
//   50  * - @subpage test_benchmarks_010
//   51  * - @subpage test_benchmarks_011
//   52  * - @subpage test_benchmarks_012
//   53  * - @subpage test_benchmarks_013
//   54  * .
//   55  * @file testbmk.c Kernel Benchmarks
//   56  * @brief Kernel Benchmarks source file
//   57  * @file testbmk.h
//   58  * @brief Kernel Benchmarks header file
//   59  */
//   60 
//   61 static Semaphore sem1;
//   62 #if CH_USE_MUTEXES || defined(__DOXYGEN__)
//   63 static Mutex mtx1;
//   64 #endif
//   65 
//   66 static msg_t thread1(void *p) {
//   67   Thread *tp;
//   68   msg_t msg;
//   69 
//   70   (void)p;
//   71   do {
//   72     tp = chMsgWait();
//   73     msg = chMsgGet(tp);
//   74     chMsgRelease(tp, msg);
//   75   } while (msg);
//   76   return 0;
//   77 }
//   78 
//   79 #ifdef __GNUC__
//   80 __attribute__((noinline))
//   81 #endif
//   82 static unsigned int msg_loop_test(Thread *tp) {
//   83 
//   84   uint32_t n = 0;
//   85   test_wait_tick();
//   86   test_start_timer(1000);
//   87   do {
//   88     (void)chMsgSend(tp, 1);
//   89     n++;
//   90 #if defined(SIMULATOR)
//   91     ChkIntSources();
//   92 #endif
//   93   } while (!test_timer_done);
//   94   (void)chMsgSend(tp, 0);
//   95   return n;
//   96 }
//   97 
//   98 /**
//   99  * @page test_benchmarks_001 Messages performance #1
//  100  *
//  101  * <h2>Description</h2>
//  102  * A message server thread is created with a lower priority than the client
//  103  * thread, the messages throughput per second is measured and the result
//  104  * printed in the output log.
//  105  */
//  106 
//  107 static void bmk1_execute(void) {
//  108   uint32_t n;
//  109 
//  110   threads[0] = chThdCreateStatic(wa[0], WA_SIZE, chThdGetPriority()-1, thread1, NULL);
//  111   n = msg_loop_test(threads[0]);
//  112   test_wait_threads();
//  113   test_print("--- Score : ");
//  114   test_printn(n);
//  115   test_print(" msgs/S, ");
//  116   test_printn(n << 1);
//  117   test_println(" ctxswc/S");
//  118 }
//  119 
//  120 ROMCONST struct testcase testbmk1 = {
//  121   "Benchmark, messages #1",
//  122   NULL,
//  123   NULL,
//  124   bmk1_execute
//  125 };
//  126 
//  127 /**
//  128  * @page test_benchmarks_002 Messages performance #2
//  129  *
//  130  * <h2>Description</h2>
//  131  * A message server thread is created with an higher priority than the client
//  132  * thread, the messages throughput per second is measured and the result
//  133  * printed in the output log.
//  134  */
//  135 
//  136 static void bmk2_execute(void) {
//  137   uint32_t n;
//  138 
//  139   threads[0] = chThdCreateStatic(wa[0], WA_SIZE, chThdGetPriority()+1, thread1, NULL);
//  140   n = msg_loop_test(threads[0]);
//  141   test_wait_threads();
//  142   test_print("--- Score : ");
//  143   test_printn(n);
//  144   test_print(" msgs/S, ");
//  145   test_printn(n << 1);
//  146   test_println(" ctxswc/S");
//  147 }
//  148 
//  149 ROMCONST struct testcase testbmk2 = {
//  150   "Benchmark, messages #2",
//  151   NULL,
//  152   NULL,
//  153   bmk2_execute
//  154 };
//  155 
//  156 static msg_t thread2(void *p) {
//  157 
//  158   return (msg_t)p;
//  159 }
//  160 
//  161 /**
//  162  * @page test_benchmarks_003 Messages performance #3
//  163  *
//  164  * <h2>Description</h2>
//  165  * A message server thread is created with an higher priority than the client
//  166  * thread, four lower priority threads crowd the ready list, the messages
//  167  * throughput per second is measured while the ready list and the result
//  168  * printed in the output log.
//  169  */
//  170 
//  171 static void bmk3_execute(void) {
//  172   uint32_t n;
//  173 
//  174   threads[0] = chThdCreateStatic(wa[0], WA_SIZE, chThdGetPriority()+1, thread1, NULL);
//  175   threads[1] = chThdCreateStatic(wa[1], WA_SIZE, chThdGetPriority()-2, thread2, NULL);
//  176   threads[2] = chThdCreateStatic(wa[2], WA_SIZE, chThdGetPriority()-3, thread2, NULL);
//  177   threads[3] = chThdCreateStatic(wa[3], WA_SIZE, chThdGetPriority()-4, thread2, NULL);
//  178   threads[4] = chThdCreateStatic(wa[4], WA_SIZE, chThdGetPriority()-5, thread2, NULL);
//  179   n = msg_loop_test(threads[0]);
//  180   test_wait_threads();
//  181   test_print("--- Score : ");
//  182   test_printn(n);
//  183   test_print(" msgs/S, ");
//  184   test_printn(n << 1);
//  185   test_println(" ctxswc/S");
//  186 }
//  187 
//  188 ROMCONST struct testcase testbmk3 = {
//  189   "Benchmark, messages #3",
//  190   NULL,
//  191   NULL,
//  192   bmk3_execute
//  193 };
//  194 
//  195 /**
//  196  * @page test_benchmarks_004 Context Switch performance
//  197  *
//  198  * <h2>Description</h2>
//  199  * A thread is created that just performs a @p chSchGoSleepS() into a loop,
//  200  * the thread is awakened as fast is possible by the tester thread.<br>
//  201  * The Context Switch performance is calculated by measuring the number of
//  202  * iterations after a second of continuous operations.
//  203  */
//  204 
//  205 msg_t thread4(void *p) {
//  206   msg_t msg;
//  207   Thread *self = chThdSelf();
//  208 
//  209   (void)p;
//  210   chSysLock();
//  211   do {
//  212     chSchGoSleepS(THD_STATE_SUSPENDED);
//  213     msg = self->p_u.rdymsg;
//  214   } while (msg == RDY_OK);
//  215   chSysUnlock();
//  216   return 0;
//  217 }
//  218 
//  219 static void bmk4_execute(void) {
//  220   Thread *tp;
//  221   uint32_t n;
//  222 
//  223   tp = threads[0] = chThdCreateStatic(wa[0], WA_SIZE, chThdGetPriority()+1, thread4, NULL);
//  224   n = 0;
//  225   test_wait_tick();
//  226   test_start_timer(1000);
//  227   do {
//  228     chSysLock();
//  229     chSchWakeupS(tp, RDY_OK);
//  230     chSchWakeupS(tp, RDY_OK);
//  231     chSchWakeupS(tp, RDY_OK);
//  232     chSchWakeupS(tp, RDY_OK);
//  233     chSysUnlock();
//  234     n += 4;
//  235 #if defined(SIMULATOR)
//  236     ChkIntSources();
//  237 #endif
//  238   } while (!test_timer_done);
//  239   chSysLock();
//  240   chSchWakeupS(tp, RDY_TIMEOUT);
//  241   chSysUnlock();
//  242 
//  243   test_wait_threads();
//  244   test_print("--- Score : ");
//  245   test_printn(n * 2);
//  246   test_println(" ctxswc/S");
//  247 }
//  248 
//  249 ROMCONST struct testcase testbmk4 = {
//  250   "Benchmark, context switch",
//  251   NULL,
//  252   NULL,
//  253   bmk4_execute
//  254 };
//  255 
//  256 /**
//  257  * @page test_benchmarks_005 Threads performance, full cycle
//  258  *
//  259  * <h2>Description</h2>
//  260  * Threads are continuously created and terminated into a loop. A full
//  261  * @p chThdCreateStatic() / @p chThdExit() / @p chThdWait() cycle is performed
//  262  * in each iteration.<br>
//  263  * The performance is calculated by measuring the number of iterations after
//  264  * a second of continuous operations.
//  265  */
//  266 
//  267 static void bmk5_execute(void) {
//  268 
//  269   uint32_t n = 0;
//  270   void *wap = wa[0];
//  271   tprio_t prio = chThdGetPriority() - 1;
//  272   test_wait_tick();
//  273   test_start_timer(1000);
//  274   do {
//  275     chThdWait(chThdCreateStatic(wap, WA_SIZE, prio, thread2, NULL));
//  276     n++;
//  277 #if defined(SIMULATOR)
//  278     ChkIntSources();
//  279 #endif
//  280   } while (!test_timer_done);
//  281   test_print("--- Score : ");
//  282   test_printn(n);
//  283   test_println(" threads/S");
//  284 }
//  285 
//  286 ROMCONST struct testcase testbmk5 = {
//  287   "Benchmark, threads, full cycle",
//  288   NULL,
//  289   NULL,
//  290   bmk5_execute
//  291 };
//  292 
//  293 /**
//  294  * @page test_benchmarks_006 Threads performance, create/exit only
//  295  *
//  296  * <h2>Description</h2>
//  297  * Threads are continuously created and terminated into a loop. A partial
//  298  * @p chThdCreateStatic() / @p chThdExit() cycle is performed in each
//  299  * iteration, the @p chThdWait() is not necessary because the thread is
//  300  * created at an higher priority so there is no need to wait for it to
//  301  * terminate.<br>
//  302  * The performance is calculated by measuring the number of iterations after
//  303  * a second of continuous operations.
//  304  */
//  305 
//  306 static void bmk6_execute(void) {
//  307 
//  308   uint32_t n = 0;
//  309   void *wap = wa[0];
//  310   tprio_t prio = chThdGetPriority() + 1;
//  311   test_wait_tick();
//  312   test_start_timer(1000);
//  313   do {
//  314     chThdCreateStatic(wap, WA_SIZE, prio, thread2, NULL);
//  315     n++;
//  316 #if defined(SIMULATOR)
//  317     ChkIntSources();
//  318 #endif
//  319   } while (!test_timer_done);
//  320   test_print("--- Score : ");
//  321   test_printn(n);
//  322   test_println(" threads/S");
//  323 }
//  324 
//  325 ROMCONST struct testcase testbmk6 = {
//  326   "Benchmark, threads, create only",
//  327   NULL,
//  328   NULL,
//  329   bmk6_execute
//  330 };
//  331 
//  332 /**
//  333  * @page test_benchmarks_007 Mass reschedule performance
//  334  *
//  335  * <h2>Description</h2>
//  336  * Five threads are created and atomically rescheduled by resetting the
//  337  * semaphore where they are waiting on. The operation is performed into a
//  338  * continuous loop.<br>
//  339  * The performance is calculated by measuring the number of iterations after
//  340  * a second of continuous operations.
//  341  */
//  342 
//  343 static msg_t thread3(void *p) {
//  344 
//  345   (void)p;
//  346   while (!chThdShouldTerminate())
//  347     chSemWait(&sem1);
//  348   return 0;
//  349 }
//  350 
//  351 static void bmk7_setup(void) {
//  352 
//  353   chSemInit(&sem1, 0);
//  354 }
//  355 
//  356 static void bmk7_execute(void) {
//  357   uint32_t n;
//  358 
//  359   threads[0] = chThdCreateStatic(wa[0], WA_SIZE, chThdGetPriority()+5, thread3, NULL);
//  360   threads[1] = chThdCreateStatic(wa[1], WA_SIZE, chThdGetPriority()+4, thread3, NULL);
//  361   threads[2] = chThdCreateStatic(wa[2], WA_SIZE, chThdGetPriority()+3, thread3, NULL);
//  362   threads[3] = chThdCreateStatic(wa[3], WA_SIZE, chThdGetPriority()+2, thread3, NULL);
//  363   threads[4] = chThdCreateStatic(wa[4], WA_SIZE, chThdGetPriority()+1, thread3, NULL);
//  364 
//  365   n = 0;
//  366   test_wait_tick();
//  367   test_start_timer(1000);
//  368   do {
//  369     chSemReset(&sem1, 0);
//  370     n++;
//  371 #if defined(SIMULATOR)
//  372     ChkIntSources();
//  373 #endif
//  374   } while (!test_timer_done);
//  375   test_terminate_threads();
//  376   chSemReset(&sem1, 0);
//  377   test_wait_threads();
//  378 
//  379   test_print("--- Score : ");
//  380   test_printn(n);
//  381   test_print(" reschedules/S, ");
//  382   test_printn(n * 6);
//  383   test_println(" ctxswc/S");
//  384 }
//  385 
//  386 ROMCONST struct testcase testbmk7 = {
//  387   "Benchmark, mass reschedule, 5 threads",
//  388   bmk7_setup,
//  389   NULL,
//  390   bmk7_execute
//  391 };
//  392 
//  393 /**
//  394  * @page test_benchmarks_008 I/O Round-Robin voluntary reschedule.
//  395  *
//  396  * <h2>Description</h2>
//  397  * Five threads are created at equal priority, each thread just increases a
//  398  * variable and yields.<br>
//  399  * The performance is calculated by measuring the number of iterations after
//  400  * a second of continuous operations.
//  401  */
//  402 
//  403 static msg_t thread8(void *p) {
//  404 
//  405   do {
//  406     chThdYield();
//  407     chThdYield();
//  408     chThdYield();
//  409     chThdYield();
//  410     (*(uint32_t *)p) += 4;
//  411 #if defined(SIMULATOR)
//  412     ChkIntSources();
//  413 #endif
//  414   } while(!chThdShouldTerminate());
//  415   return 0;
//  416 }
//  417 
//  418 static void bmk8_execute(void) {
//  419   uint32_t n;
//  420 
//  421   n = 0;
//  422   test_wait_tick();
//  423 
//  424   threads[0] = chThdCreateStatic(wa[0], WA_SIZE, chThdGetPriority()-1, thread8, (void *)&n);
//  425   threads[1] = chThdCreateStatic(wa[1], WA_SIZE, chThdGetPriority()-1, thread8, (void *)&n);
//  426   threads[2] = chThdCreateStatic(wa[2], WA_SIZE, chThdGetPriority()-1, thread8, (void *)&n);
//  427   threads[3] = chThdCreateStatic(wa[3], WA_SIZE, chThdGetPriority()-1, thread8, (void *)&n);
//  428   threads[4] = chThdCreateStatic(wa[4], WA_SIZE, chThdGetPriority()-1, thread8, (void *)&n);
//  429 
//  430   chThdSleepSeconds(1);
//  431   test_terminate_threads();
//  432   test_wait_threads();
//  433 
//  434   test_print("--- Score : ");
//  435   test_printn(n);
//  436   test_println(" ctxswc/S");
//  437 }
//  438 
//  439 ROMCONST struct testcase testbmk8 = {
//  440   "Benchmark, round robin context switching",
//  441   NULL,
//  442   NULL,
//  443   bmk8_execute
//  444 };
//  445 
//  446 #if CH_USE_QUEUES || defined(__DOXYGEN__)
//  447 /**
//  448  * @page test_benchmarks_009 I/O Queues throughput
//  449  *
//  450  * <h2>Description</h2>
//  451  * Four bytes are written and then read from an @p InputQueue into a continuous
//  452  * loop.<br>
//  453  * The performance is calculated by measuring the number of iterations after
//  454  * a second of continuous operations.
//  455  */
//  456 
//  457 static void bmk9_execute(void) {
//  458   uint32_t n;
//  459   static uint8_t ib[16];
//  460   static InputQueue iq;
//  461 
//  462   chIQInit(&iq, ib, sizeof(ib), NULL, NULL);
//  463   n = 0;
//  464   test_wait_tick();
//  465   test_start_timer(1000);
//  466   do {
//  467     chSysLock();
//  468     chIQPutI(&iq, 0);
//  469     chIQPutI(&iq, 1);
//  470     chIQPutI(&iq, 2);
//  471     chIQPutI(&iq, 3);
//  472     chSysUnlock();
//  473     (void)chIQGet(&iq);
//  474     (void)chIQGet(&iq);
//  475     (void)chIQGet(&iq);
//  476     (void)chIQGet(&iq);
//  477     n++;
//  478 #if defined(SIMULATOR)
//  479     ChkIntSources();
//  480 #endif
//  481   } while (!test_timer_done);
//  482   test_print("--- Score : ");
//  483   test_printn(n * 4);
//  484   test_println(" bytes/S");
//  485 }
//  486 
//  487 ROMCONST struct testcase testbmk9 = {
//  488   "Benchmark, I/O Queues throughput",
//  489   NULL,
//  490   NULL,
//  491   bmk9_execute
//  492 };
//  493 #endif /* CH_USE_QUEUES */
//  494 
//  495 /**
//  496  * @page test_benchmarks_010 Virtual Timers set/reset performance
//  497  *
//  498  * <h2>Description</h2>
//  499  * A virtual timer is set and immediately reset into a continuous loop.<br>
//  500  * The performance is calculated by measuring the number of iterations after
//  501  * a second of continuous operations.
//  502  */
//  503 
//  504 static void tmo(void *param) {(void)param;}
//  505 
//  506 static void bmk10_execute(void) {
//  507   static VirtualTimer vt1, vt2;
//  508   uint32_t n = 0;
//  509 
//  510   test_wait_tick();
//  511   test_start_timer(1000);
//  512   do {
//  513     chSysLock();
//  514     chVTSetI(&vt1, 1, tmo, NULL);
//  515     chVTSetI(&vt2, 10000, tmo, NULL);
//  516     chVTResetI(&vt1);
//  517     chVTResetI(&vt2);
//  518     chSysUnlock();
//  519     n++;
//  520 #if defined(SIMULATOR)
//  521     ChkIntSources();
//  522 #endif
//  523   } while (!test_timer_done);
//  524   test_print("--- Score : ");
//  525   test_printn(n * 2);
//  526   test_println(" timers/S");
//  527 }
//  528 
//  529 ROMCONST struct testcase testbmk10 = {
//  530   "Benchmark, virtual timers set/reset",
//  531   NULL,
//  532   NULL,
//  533   bmk10_execute
//  534 };
//  535 
//  536 #if CH_USE_SEMAPHORES || defined(__DOXYGEN__)
//  537 /**
//  538  * @page test_benchmarks_011 Semaphores wait/signal performance
//  539  *
//  540  * <h2>Description</h2>
//  541  * A counting semaphore is taken/released into a continuous loop, no Context
//  542  * Switch happens because the counter is always non negative.<br>
//  543  * The performance is calculated by measuring the number of iterations after
//  544  * a second of continuous operations.
//  545  */
//  546 
//  547 static void bmk11_setup(void) {
//  548 
//  549   chSemInit(&sem1, 1);
//  550 }
//  551 
//  552 static void bmk11_execute(void) {
//  553   uint32_t n = 0;
//  554 
//  555   test_wait_tick();
//  556   test_start_timer(1000);
//  557   do {
//  558     chSemWait(&sem1);
//  559     chSemSignal(&sem1);
//  560     chSemWait(&sem1);
//  561     chSemSignal(&sem1);
//  562     chSemWait(&sem1);
//  563     chSemSignal(&sem1);
//  564     chSemWait(&sem1);
//  565     chSemSignal(&sem1);
//  566     n++;
//  567 #if defined(SIMULATOR)
//  568     ChkIntSources();
//  569 #endif
//  570   } while (!test_timer_done);
//  571   test_print("--- Score : ");
//  572   test_printn(n * 4);
//  573   test_println(" wait+signal/S");
//  574 }
//  575 
//  576 ROMCONST struct testcase testbmk11 = {
//  577   "Benchmark, semaphores wait/signal",
//  578   bmk11_setup,
//  579   NULL,
//  580   bmk11_execute
//  581 };
//  582 #endif /* CH_USE_SEMAPHORES */
//  583 
//  584 #if CH_USE_MUTEXES || defined(__DOXYGEN__)
//  585 /**
//  586  * @page test_benchmarks_012 Mutexes lock/unlock performance
//  587  *
//  588  * <h2>Description</h2>
//  589  * A mutex is locked/unlocked into a continuous loop, no Context Switch happens
//  590  * because there are no other threads asking for the mutex.<br>
//  591  * The performance is calculated by measuring the number of iterations after
//  592  * a second of continuous operations.
//  593  */
//  594 
//  595 static void bmk12_setup(void) {
//  596 
//  597   chMtxInit(&mtx1);
//  598 }
//  599 
//  600 static void bmk12_execute(void) {
//  601   uint32_t n = 0;
//  602 
//  603   test_wait_tick();
//  604   test_start_timer(1000);
//  605   do {
//  606     chMtxLock(&mtx1);
//  607     chMtxUnlock();
//  608     chMtxLock(&mtx1);
//  609     chMtxUnlock();
//  610     chMtxLock(&mtx1);
//  611     chMtxUnlock();
//  612     chMtxLock(&mtx1);
//  613     chMtxUnlock();
//  614     n++;
//  615 #if defined(SIMULATOR)
//  616     ChkIntSources();
//  617 #endif
//  618   } while (!test_timer_done);
//  619   test_print("--- Score : ");
//  620   test_printn(n * 4);
//  621   test_println(" lock+unlock/S");
//  622 }
//  623 
//  624 ROMCONST struct testcase testbmk12 = {
//  625   "Benchmark, mutexes lock/unlock",
//  626   bmk12_setup,
//  627   NULL,
//  628   bmk12_execute
//  629 };
//  630 #endif
//  631 
//  632 /**
//  633  * @page test_benchmarks_013 RAM Footprint
//  634  *
//  635  * <h2>Description</h2>
//  636  * The memory size of the various kernel objects is printed.
//  637  */
//  638 
//  639 static void bmk13_execute(void) {
//  640 
//  641   test_print("--- System: ");
//  642   test_printn(sizeof(ReadyList) + sizeof(VTList) +
//  643               PORT_IDLE_THREAD_STACK_SIZE +
//  644               (sizeof(Thread) + sizeof(struct intctx) +
//  645                sizeof(struct extctx) +
//  646                PORT_INT_REQUIRED_STACK) * 2);
//  647   test_println(" bytes");
//  648   test_print("--- Thread: ");
//  649   test_printn(sizeof(Thread));
//  650   test_println(" bytes");
//  651   test_print("--- Timer : ");
//  652   test_printn(sizeof(VirtualTimer));
//  653   test_println(" bytes");
//  654 #if CH_USE_SEMAPHORES || defined(__DOXYGEN__)
//  655   test_print("--- Semaph: ");
//  656   test_printn(sizeof(Semaphore));
//  657   test_println(" bytes");
//  658 #endif
//  659 #if CH_USE_EVENTS || defined(__DOXYGEN__)
//  660   test_print("--- EventS: ");
//  661   test_printn(sizeof(EventSource));
//  662   test_println(" bytes");
//  663   test_print("--- EventL: ");
//  664   test_printn(sizeof(EventListener));
//  665   test_println(" bytes");
//  666 #endif
//  667 #if CH_USE_MUTEXES || defined(__DOXYGEN__)
//  668   test_print("--- Mutex : ");
//  669   test_printn(sizeof(Mutex));
//  670   test_println(" bytes");
//  671 #endif
//  672 #if CH_USE_CONDVARS || defined(__DOXYGEN__)
//  673   test_print("--- CondV.: ");
//  674   test_printn(sizeof(CondVar));
//  675   test_println(" bytes");
//  676 #endif
//  677 #if CH_USE_QUEUES || defined(__DOXYGEN__)
//  678   test_print("--- Queue : ");
//  679   test_printn(sizeof(GenericQueue));
//  680   test_println(" bytes");
//  681 #endif
//  682 #if CH_USE_MAILBOXES || defined(__DOXYGEN__)
//  683   test_print("--- MailB.: ");
//  684   test_printn(sizeof(Mailbox));
//  685   test_println(" bytes");
//  686 #endif
//  687 }
//  688 
//  689 ROMCONST struct testcase testbmk13 = {
//  690   "Benchmark, RAM footprint",
//  691   NULL,
//  692   NULL,
//  693   bmk13_execute
//  694 };
//  695 
//  696 /**
//  697  * @brief   Test sequence for benchmarks.
//  698  */
//  699 ROMCONST struct testcase * ROMCONST patternbmk[] = {
//  700 #if !TEST_NO_BENCHMARKS
//  701   &testbmk1,
//  702   &testbmk2,
//  703   &testbmk3,
//  704   &testbmk4,
//  705   &testbmk5,
//  706   &testbmk6,
//  707   &testbmk7,
//  708   &testbmk8,
//  709 #if CH_USE_QUEUES || defined(__DOXYGEN__)
//  710   &testbmk9,
//  711 #endif
//  712   &testbmk10,
//  713 #if CH_USE_SEMAPHORES || defined(__DOXYGEN__)
//  714   &testbmk11,
//  715 #endif
//  716 #if CH_USE_MUTEXES || defined(__DOXYGEN__)
//  717   &testbmk12,
//  718 #endif
//  719   &testbmk13,
//  720 #endif
//  721   NULL
//  722 };
//  723 
//  724 #endif /* EFI_PERF_METRICS */
// 
//
// 
//
//
//Errors: none
//Warnings: none
