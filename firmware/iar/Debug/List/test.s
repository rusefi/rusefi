///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:22 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\development\tes /
//                       t\test.c                                             /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\development\tes /
//                       t\test.c -lCN F:\stuff\rusefi_sourceforge\firmware\i /
//                       ar\Debug\List\ -lA F:\stuff\rusefi_sourceforge\firmw /
//                       are\iar\Debug\List\ -o F:\stuff\rusefi_sourceforge\f /
//                       irmware\iar\Debug\Obj\ --no_cse --no_unroll          /
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
//                       test.s                                               /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME test

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
// F:\stuff\rusefi_sourceforge\firmware\development\test\test.c
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
//   17 /**
//   18  * @file    test.c
//   19  * @brief   Tests support code.
//   20  *
//   21  * @addtogroup test
//   22  * @{
//   23  */
//   24 
//   25 #include "main.h"
//   26 
//   27 #if EFI_PERF_METRICS
//   28 
//   29 #include "test.h"
//   30 #include "testbmk.h"
//   31 
//   32 /*
//   33  * Array of all the test patterns.
//   34  */
//   35 static ROMCONST struct testcase * ROMCONST *patterns[] = {
//   36   patternbmk,
//   37   NULL
//   38 };
//   39 
//   40 static bool_t local_fail, global_fail;
//   41 static unsigned failpoint;
//   42 static char tokens_buffer[MAX_TOKENS];
//   43 static char *tokp;
//   44 
//   45 /*
//   46  * Static working areas, the following areas can be used for threads or
//   47  * used as temporary buffers.
//   48  */
//   49 union test_buffers test;
//   50 
//   51 /*
//   52  * Pointers to the spawned threads.
//   53  */
//   54 Thread *threads[MAX_THREADS];
//   55 
//   56 /*
//   57  * Pointers to the working areas.
//   58  */
//   59 void * ROMCONST wa[5] = {test.wa.T0, test.wa.T1, test.wa.T2,
//   60                          test.wa.T3, test.wa.T4};
//   61 
//   62 /*
//   63  * Console output.
//   64  */
//   65 static BaseSequentialStream *chp;
//   66 
//   67 /**
//   68  * @brief   Prints a decimal unsigned number.
//   69  *
//   70  * @param[in] n         the number to be printed
//   71  */
//   72 void test_printn(uint32_t n) {
//   73   char buf[16], *p;
//   74 
//   75   if (!n)
//   76     chSequentialStreamPut(chp, '0');
//   77   else {
//   78     p = buf;
//   79     while (n)
//   80       *p++ = (n % 10) + '0', n /= 10;
//   81     while (p > buf)
//   82       chSequentialStreamPut(chp, *--p);
//   83   }
//   84 }
//   85 
//   86 /**
//   87  * @brief   Prints a line without final end-of-line.
//   88  *
//   89  * @param[in] msgp      the message
//   90  */
//   91 void test_print(const char *msgp) {
//   92 
//   93   while (*msgp)
//   94     chSequentialStreamPut(chp, *msgp++);
//   95 }
//   96 
//   97 /**
//   98  * @brief   Prints a line.
//   99  *
//  100  * @param[in] msgp      the message
//  101  */
//  102 void test_println(const char *msgp) {
//  103 
//  104   test_print(msgp);
//  105   chSequentialStreamWrite(chp, (const uint8_t *)"\r\n", 2);
//  106 }
//  107 
//  108 /*
//  109  * Tokens.
//  110  */
//  111 static void clear_tokens(void) {
//  112 
//  113   tokp = tokens_buffer;
//  114 }
//  115 
//  116 static void print_tokens(void) {
//  117   char *cp = tokens_buffer;
//  118 
//  119   while (cp < tokp)
//  120     chSequentialStreamPut(chp, *cp++);
//  121 }
//  122 
//  123 /**
//  124  * @brief   Emits a token into the tokens buffer.
//  125  *
//  126  * @param[in] token     the token as a char
//  127  */
//  128 void test_emit_token(char token) {
//  129 
//  130   chSysLock();
//  131   *tokp++ = token;
//  132   chSysUnlock();
//  133 }
//  134 
//  135 /*
//  136  * Assertions.
//  137  */
//  138 bool_t _test_fail(unsigned point) {
//  139 
//  140   local_fail = TRUE;
//  141   global_fail = TRUE;
//  142   failpoint = point;
//  143   return TRUE;
//  144 }
//  145 
//  146 bool_t _test_assert(unsigned point, bool_t condition) {
//  147 
//  148   if (!condition)
//  149     return _test_fail(point);
//  150   return FALSE;
//  151 }
//  152 
//  153 bool_t _test_assert_sequence(unsigned point, char *expected) {
//  154   char *cp = tokens_buffer;
//  155   while (cp < tokp) {
//  156     if (*cp++ != *expected++)
//  157      return _test_fail(point);
//  158   }
//  159   if (*expected)
//  160     return _test_fail(point);
//  161   clear_tokens();
//  162   return FALSE;
//  163 }
//  164 
//  165 bool_t _test_assert_time_window(unsigned point, systime_t start, systime_t end) {
//  166 
//  167   return _test_assert(point, chTimeIsWithin(start, end));
//  168 }
//  169 
//  170 /*
//  171  * Threads utils.
//  172  */
//  173 
//  174 /**
//  175  * @brief   Sets a termination request in all the test-spawned threads.
//  176  */
//  177 void test_terminate_threads(void) {
//  178   int i;
//  179 
//  180   for (i = 0; i < MAX_THREADS; i++)
//  181     if (threads[i])
//  182       chThdTerminate(threads[i]);
//  183 }
//  184 
//  185 /**
//  186  * @brief   Waits for the completion of all the test-spawned threads.
//  187  */
//  188 void test_wait_threads(void) {
//  189   int i;
//  190 
//  191   for (i = 0; i < MAX_THREADS; i++)
//  192     if (threads[i] != NULL) {
//  193       chThdWait(threads[i]);
//  194       threads[i] = NULL;
//  195     }
//  196 }
//  197 
//  198 #if CH_DBG_THREADS_PROFILING
//  199 /**
//  200  * @brief   CPU pulse.
//  201  * @note    The current implementation is not totally reliable.
//  202  *
//  203  * @param[in] duration      CPU pulse duration in milliseconds
//  204  */
//  205 void test_cpu_pulse(unsigned duration) {
//  206   systime_t start, end, now;
//  207 
//  208   start = chThdSelf()->p_time;
//  209   end = start + MS2ST(duration);
//  210   do {
//  211     now = chThdSelf()->p_time;
//  212 #if defined(SIMULATOR)
//  213     ChkIntSources();
//  214 #endif
//  215   }
//  216   while (end > start ? (now >= start) && (now < end) :
//  217                        (now >= start) || (now < end));
//  218 }
//  219 #endif
//  220 
//  221 /**
//  222  * @brief   Delays execution until next system time tick.
//  223  *
//  224  * @return              The system time.
//  225  */
//  226 systime_t test_wait_tick(void) {
//  227 
//  228   chThdSleep(1);
//  229   return chTimeNow();
//  230 }
//  231 
//  232 /*
//  233  * Timer utils.
//  234  */
//  235 
//  236 /**
//  237  * @brief   Set to @p TRUE when the test timer reaches its deadline.
//  238  */
//  239 bool_t test_timer_done;
//  240 
//  241 static VirtualTimer vt;
//  242 static void tmr(void *p) {
//  243   (void)p;
//  244 
//  245   test_timer_done = TRUE;
//  246 }
//  247 
//  248 /**
//  249  * @brief   Starts the test timer.
//  250  *
//  251  * @param[in] ms        time in milliseconds
//  252  */
//  253 void test_start_timer(unsigned ms) {
//  254 
//  255   systime_t duration = MS2ST(ms);
//  256   test_timer_done = FALSE;
//  257   chVTSet(&vt, duration, tmr, NULL);
//  258 }
//  259 
//  260 /*
//  261  * Test suite execution.
//  262  */
//  263 static void execute_test(const struct testcase *tcp) {
//  264   int i;
//  265 
//  266   /* Initialization */
//  267   clear_tokens();
//  268   local_fail = FALSE;
//  269   for (i = 0; i < MAX_THREADS; i++)
//  270     threads[i] = NULL;
//  271 
//  272   if (tcp->setup != NULL)
//  273     tcp->setup();
//  274   tcp->execute();
//  275   if (tcp->teardown != NULL)
//  276     tcp->teardown();
//  277 
//  278   test_wait_threads();
//  279 }
//  280 
//  281 static void print_line(void) {
//  282   unsigned i;
//  283 
//  284   for (i = 0; i < 76; i++)
//  285     chSequentialStreamPut(chp, '-');
//  286   chSequentialStreamWrite(chp, (const uint8_t *)"\r\n", 2);
//  287 }
//  288 
//  289 /**
//  290  * @brief   Test execution thread function.
//  291  *
//  292  * @param[in] p         pointer to a @p BaseChannel object for test output
//  293  * @return              A failure boolean value.
//  294  */
//  295 msg_t TestThread(void *p) {
//  296   int i, j;
//  297 
//  298   chp = p;
//  299   test_println("");
//  300   test_println("*** ChibiOS/RT test suite");
//  301   test_println("***");
//  302   test_print("*** Kernel:       ");
//  303   test_println(CH_KERNEL_VERSION);
//  304   test_print("*** Compiled:     ");
//  305   test_println(__DATE__ " - " __TIME__);
//  306 #ifdef CH_COMPILER_NAME
//  307   test_print("*** Compiler:     ");
//  308   test_println(CH_COMPILER_NAME);
//  309 #endif
//  310   test_print("*** Architecture: ");
//  311   test_println(CH_ARCHITECTURE_NAME);
//  312 #ifdef CH_CORE_VARIANT_NAME
//  313   test_print("*** Core Variant: ");
//  314   test_println(CH_CORE_VARIANT_NAME);
//  315 #endif
//  316 #ifdef CH_PORT_INFO
//  317   test_print("*** Port Info:    ");
//  318   test_println(CH_PORT_INFO);
//  319 #endif
//  320 #ifdef PLATFORM_NAME
//  321   test_print("*** Platform:     ");
//  322   test_println(PLATFORM_NAME);
//  323 #endif
//  324 #ifdef BOARD_NAME
//  325   test_print("*** Test Board:   ");
//  326   test_println(BOARD_NAME);
//  327 #endif
//  328   test_println("");
//  329 
//  330   global_fail = FALSE;
//  331   i = 0;
//  332   while (patterns[i]) {
//  333     j = 0;
//  334     while (patterns[i][j]) {
//  335       print_line();
//  336       test_print("--- Test Case ");
//  337       test_printn(i + 1);
//  338       test_print(".");
//  339       test_printn(j + 1);
//  340       test_print(" (");
//  341       test_print(patterns[i][j]->name);
//  342       test_println(")");
//  343 #if DELAY_BETWEEN_TESTS > 0
//  344       chThdSleepMilliseconds(DELAY_BETWEEN_TESTS);
//  345 #endif
//  346       execute_test(patterns[i][j]);
//  347       if (local_fail) {
//  348         test_print("--- Result: FAILURE (#");
//  349         test_printn(failpoint);
//  350         test_print(" [");
//  351         print_tokens();
//  352         test_println("])");
//  353       }
//  354       else
//  355         test_println("--- Result: SUCCESS");
//  356       j++;
//  357     }
//  358     i++;
//  359   }
//  360   print_line();
//  361   test_println("");
//  362   test_print("Final result: ");
//  363   if (global_fail)
//  364     test_println("FAILURE");
//  365   else
//  366     test_println("SUCCESS");
//  367 
//  368   return (msg_t)global_fail;
//  369 }
//  370 
//  371 /** @} */
//  372 #endif /* EFI_PERF_METRICS */
// 
//
// 
//
//
//Errors: none
//Warnings: none
