///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:26 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       platforms\STM32F4xx\hal_lld.c                        /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       platforms\STM32F4xx\hal_lld.c -lCN                   /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\Debug\List\ /
//                        -lA F:\stuff\rusefi_sourceforge\firmware\iar\Debug\ /
//                       List\ -o F:\stuff\rusefi_sourceforge\firmware\iar\De /
//                       bug\Obj\ --no_cse --no_unroll --no_inline            /
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
//                       hal_lld.s                                            /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME hal_lld

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN dmaInit

        PUBLIC hal_lld_init
        PUBLIC stm32_clock_init
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\platforms\STM32F4xx\hal_lld.c
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
//   18  * @file    STM32F4xx/hal_lld.c
//   19  * @brief   STM32F4xx/STM32F2xx HAL subsystem low level driver source.
//   20  *
//   21  * @addtogroup HAL
//   22  * @{
//   23  */
//   24 
//   25 /* TODO: LSEBYP like in F3.*/
//   26 
//   27 #include "ch.h"
//   28 #include "hal.h"
//   29 
//   30 /*===========================================================================*/
//   31 /* Driver local definitions.                                                 */
//   32 /*===========================================================================*/
//   33 
//   34 /*===========================================================================*/
//   35 /* Driver exported variables.                                                */
//   36 /*===========================================================================*/
//   37 
//   38 /*===========================================================================*/
//   39 /* Driver local variables and types.                                         */
//   40 /*===========================================================================*/
//   41 
//   42 /*===========================================================================*/
//   43 /* Driver local functions.                                                   */
//   44 /*===========================================================================*/
//   45 
//   46 /**
//   47  * @brief   Initializes the backup domain.
//   48  * @note    WARNING! Changing clock source impossible without resetting
//   49  *          of the whole BKP domain.
//   50  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function hal_lld_backup_domain_init
          CFI NoCalls
        THUMB
//   51 static void hal_lld_backup_domain_init(void) {
//   52 
//   53   /* Backup domain access enabled and left open.*/
//   54   PWR->CR |= PWR_CR_DBP;
hal_lld_backup_domain_init:
        LDR.N    R0,??DataTable2  ;; 0x40007000
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x100
        LDR.N    R1,??DataTable2  ;; 0x40007000
        STR      R0,[R1, #+0]
//   55 
//   56   /* Reset BKP domain if different clock source selected.*/
//   57   if ((RCC->BDCR & STM32_RTCSEL_MASK) != STM32_RTCSEL) {
        LDR.N    R0,??DataTable2_1  ;; 0x40023870
        LDR      R0,[R0, #+0]
        ANDS     R0,R0,#0x300
        CMP      R0,#+256
        BEQ.N    ??hal_lld_backup_domain_init_0
//   58     /* Backup domain reset.*/
//   59     RCC->BDCR = RCC_BDCR_BDRST;
        LDR.N    R0,??DataTable2_1  ;; 0x40023870
        MOVS     R1,#+65536
        STR      R1,[R0, #+0]
//   60     RCC->BDCR = 0;
        LDR.N    R0,??DataTable2_1  ;; 0x40023870
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//   61   }
//   62 
//   63 #if STM32_LSE_ENABLED
//   64 #if defined(STM32_LSE_BYPASS)
//   65   /* LSE Bypass.*/
//   66   RCC->BDCR |= RCC_BDCR_LSEON | RCC_BDCR_LSEBYP;
//   67 #else
//   68   /* No LSE Bypass.*/
//   69   RCC->BDCR |= RCC_BDCR_LSEON;
??hal_lld_backup_domain_init_0:
        LDR.N    R0,??DataTable2_1  ;; 0x40023870
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x1
        LDR.N    R1,??DataTable2_1  ;; 0x40023870
        STR      R0,[R1, #+0]
//   70 #endif
//   71   int waitCounter = 0;
        MOVS     R0,#+0
//   72   while ((RCC->BDCR & RCC_BDCR_LSERDY) == 0 && ++waitCounter <LSE_TIMEOUT)
??hal_lld_backup_domain_init_1:
        LDR.N    R1,??DataTable2_1  ;; 0x40023870
        LDR      R1,[R1, #+0]
        LSLS     R1,R1,#+30
        BMI.N    ??hal_lld_backup_domain_init_2
        ADDS     R0,R0,#+1
        LDR.N    R1,??DataTable2_2  ;; 0xf4240
        CMP      R0,R1
        BLT.N    ??hal_lld_backup_domain_init_1
//   73     ;                                       /* Waits until LSE is stable.   */
//   74 #endif /* STM32_LSE_ENABLED */
//   75 
//   76 #if HAL_USE_RTC
//   77   /* If the backup domain hasn't been initialized yet then proceed with
//   78      initialization.*/
//   79   if ((RCC->BDCR & RCC_BDCR_RTCEN) == 0) {
??hal_lld_backup_domain_init_2:
        LDR.N    R1,??DataTable2_1  ;; 0x40023870
        LDR      R1,[R1, #+0]
        LSLS     R1,R1,#+16
        BMI.N    ??hal_lld_backup_domain_init_3
//   80     /* Selects clock source.*/
//   81     RCC->BDCR |= STM32_RTCSEL;
        LDR.N    R1,??DataTable2_1  ;; 0x40023870
        LDR      R1,[R1, #+0]
        ORRS     R1,R1,#0x100
        LDR.N    R2,??DataTable2_1  ;; 0x40023870
        STR      R1,[R2, #+0]
//   82 
//   83     /* RTC clock enabled.*/
//   84     RCC->BDCR |= RCC_BDCR_RTCEN;
        LDR.N    R1,??DataTable2_1  ;; 0x40023870
        LDR      R1,[R1, #+0]
        ORRS     R1,R1,#0x8000
        LDR.N    R2,??DataTable2_1  ;; 0x40023870
        STR      R1,[R2, #+0]
//   85   }
//   86 #endif /* HAL_USE_RTC */
//   87 
//   88 #if STM32_BKPRAM_ENABLE
//   89   rccEnableBKPSRAM(false);
//   90 
//   91   PWR->CSR |= PWR_CSR_BRE;
//   92   while ((PWR->CSR & PWR_CSR_BRR) == 0)
//   93     ;                                /* Waits until the regulator is stable */
//   94 #else
//   95   PWR->CSR &= ~PWR_CSR_BRE;
??hal_lld_backup_domain_init_3:
        LDR.N    R1,??DataTable2_3  ;; 0x40007004
        LDR      R1,[R1, #+0]
        BICS     R1,R1,#0x200
        LDR.N    R2,??DataTable2_3  ;; 0x40007004
        STR      R1,[R2, #+0]
//   96 #endif /* STM32_BKPRAM_ENABLE */
//   97 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock0
//   98 
//   99 /*===========================================================================*/
//  100 /* Driver interrupt handlers.                                                */
//  101 /*===========================================================================*/
//  102 
//  103 /*===========================================================================*/
//  104 /* Driver exported functions.                                                */
//  105 /*===========================================================================*/
//  106 
//  107 /**
//  108  * @brief   Low level HAL driver initialization.
//  109  *
//  110  * @notapi
//  111  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function hal_lld_init
        THUMB
//  112 void hal_lld_init(void) {
hal_lld_init:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
//  113 
//  114   /* Reset of all peripherals. AHB3 is not reseted because it could have
//  115      been initialized in the board initialization file (board.c).*/
//  116   rccResetAHB1(~0);
        LDR.N    R0,??DataTable2_4  ;; 0x40023810
        LDR      R4,[R0, #+0]
        LDR.N    R0,??DataTable2_4  ;; 0x40023810
        MOVS     R1,#-1
        STR      R1,[R0, #+0]
        LDR.N    R0,??DataTable2_4  ;; 0x40023810
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//  117   rccResetAHB2(~0);
        LDR.N    R0,??DataTable2_5  ;; 0x40023814
        LDR      R5,[R0, #+0]
        LDR.N    R0,??DataTable2_5  ;; 0x40023814
        MOVS     R1,#-1
        STR      R1,[R0, #+0]
        LDR.N    R0,??DataTable2_5  ;; 0x40023814
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//  118   rccResetAPB1(~RCC_APB1RSTR_PWRRST);
        LDR.N    R0,??DataTable2_6  ;; 0x40023820
        LDR      R0,[R0, #+0]
        ORNS     R0,R0,#+268435456
        LDR.N    R1,??DataTable2_6  ;; 0x40023820
        STR      R0,[R1, #+0]
        LDR.N    R0,??DataTable2_6  ;; 0x40023820
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//  119   rccResetAPB2(~0);
        LDR.N    R0,??DataTable2_7  ;; 0x40023824
        LDR      R6,[R0, #+0]
        LDR.N    R0,??DataTable2_7  ;; 0x40023824
        MOVS     R1,#-1
        STR      R1,[R0, #+0]
        LDR.N    R0,??DataTable2_7  ;; 0x40023824
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//  120 
//  121   /* SysTick initialization using the system clock.*/
//  122   SysTick->LOAD = STM32_HCLK / CH_FREQUENCY - 1;
        LDR.N    R0,??DataTable2_8  ;; 0xe000e014
        LDR.N    R1,??DataTable2_9  ;; 0x2903f
        STR      R1,[R0, #+0]
//  123   SysTick->VAL = 0;
        LDR.N    R0,??DataTable2_10  ;; 0xe000e018
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//  124   SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
//  125                   SysTick_CTRL_ENABLE_Msk |
//  126                   SysTick_CTRL_TICKINT_Msk;
        LDR.N    R0,??DataTable2_11  ;; 0xe000e010
        MOVS     R1,#+7
        STR      R1,[R0, #+0]
//  127 
//  128   /* DWT cycle counter enable.*/
//  129   SCS_DEMCR |= SCS_DEMCR_TRCENA;
        LDR.N    R0,??DataTable2_12  ;; 0xe000edfc
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x1000000
        LDR.N    R1,??DataTable2_12  ;; 0xe000edfc
        STR      R0,[R1, #+0]
//  130   DWT_CTRL  |= DWT_CTRL_CYCCNTENA;
        LDR.N    R0,??DataTable2_13  ;; 0xe0001000
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x1
        LDR.N    R1,??DataTable2_13  ;; 0xe0001000
        STR      R0,[R1, #+0]
//  131 
//  132   /* PWR clock enabled.*/
//  133   rccEnablePWRInterface(FALSE);
        LDR.N    R0,??DataTable2_14  ;; 0x40023840
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x10000000
        LDR.N    R1,??DataTable2_14  ;; 0x40023840
        STR      R0,[R1, #+0]
//  134 
//  135   /* Initializes the backup domain.*/
//  136   hal_lld_backup_domain_init();
          CFI FunCall hal_lld_backup_domain_init
        BL       hal_lld_backup_domain_init
//  137 
//  138 #if defined(STM32_DMA_REQUIRED)
//  139   dmaInit();
          CFI FunCall dmaInit
        BL       dmaInit
//  140 #endif
//  141 
//  142   /* Programmable voltage detector enable.*/
//  143 #if STM32_PVD_ENABLE
//  144   PWR->CR |= PWR_CR_PVDE | (STM32_PLS & STM32_PLS_MASK);
//  145 #endif /* STM32_PVD_ENABLE */
//  146 }
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock1
//  147 
//  148 /**
//  149  * @brief   STM32F2xx clocks and PLL initialization.
//  150  * @note    All the involved constants come from the file @p board.h.
//  151  * @note    This function should be invoked just after the system reset.
//  152  *
//  153  * @special
//  154  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function stm32_clock_init
          CFI NoCalls
        THUMB
//  155 void stm32_clock_init(void) {
//  156 
//  157 #if !STM32_NO_INIT
//  158   /* PWR clock enable.*/
//  159   RCC->APB1ENR = RCC_APB1ENR_PWREN;
stm32_clock_init:
        LDR.N    R0,??DataTable2_14  ;; 0x40023840
        MOVS     R1,#+268435456
        STR      R1,[R0, #+0]
//  160 
//  161   /* PWR initialization.*/
//  162 #if defined(STM32F4XX) || defined(__DOXYGEN__)
//  163   PWR->CR = STM32_VOS;
        LDR.N    R0,??DataTable2  ;; 0x40007000
        MOV      R1,#+49152
        STR      R1,[R0, #+0]
//  164 #else
//  165   PWR->CR = 0;
//  166 #endif
//  167 
//  168   /* HSI setup, it enforces the reset situation in order to handle possible
//  169      problems with JTAG probes and re-initializations.*/
//  170   RCC->CR |= RCC_CR_HSION;                  /* Make sure HSI is ON.         */
        LDR.N    R0,??DataTable2_15  ;; 0x40023800
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x1
        LDR.N    R1,??DataTable2_15  ;; 0x40023800
        STR      R0,[R1, #+0]
//  171   while (!(RCC->CR & RCC_CR_HSIRDY))
??stm32_clock_init_0:
        LDR.N    R0,??DataTable2_15  ;; 0x40023800
        LDR      R0,[R0, #+0]
        LSLS     R0,R0,#+30
        BPL.N    ??stm32_clock_init_0
//  172     ;                                       /* Wait until HSI is stable.    */
//  173 
//  174   /* HSI is selected as new source without touching the other fields in
//  175      CFGR. Clearing the register has to be postponed after HSI is the
//  176      new source.*/
//  177   RCC->CFGR &= ~RCC_CFGR_SW;                /* Reset SW */
        LDR.N    R0,??DataTable2_16  ;; 0x40023808
        LDR      R0,[R0, #+0]
        LSRS     R0,R0,#+2
        LSLS     R0,R0,#+2
        LDR.N    R1,??DataTable2_16  ;; 0x40023808
        STR      R0,[R1, #+0]
//  178   RCC->CFGR |= RCC_CFGR_SWS_HSI;            /* Select HSI as internal*/
        LDR.N    R0,??DataTable2_16  ;; 0x40023808
        LDR.N    R1,??DataTable2_16  ;; 0x40023808
        LDR      R1,[R1, #+0]
        STR      R1,[R0, #+0]
//  179   while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI)
??stm32_clock_init_1:
        LDR.N    R0,??DataTable2_16  ;; 0x40023808
        LDR      R0,[R0, #+0]
        TST      R0,#0xC
        BNE.N    ??stm32_clock_init_1
//  180     ;                                       /* Wait until HSI is selected.  */
//  181 
//  182   /* Registers finally cleared to reset values.*/
//  183   RCC->CR &= RCC_CR_HSITRIM | RCC_CR_HSION; /* CR Reset value.              */
        LDR.N    R0,??DataTable2_15  ;; 0x40023800
        LDR      R0,[R0, #+0]
        ANDS     R0,R0,#0xF9
        LDR.N    R1,??DataTable2_15  ;; 0x40023800
        STR      R0,[R1, #+0]
//  184   RCC->CFGR = 0;                            /* CFGR reset value.            */
        LDR.N    R0,??DataTable2_16  ;; 0x40023808
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//  185 
//  186 #if STM32_HSE_ENABLED
//  187   /* HSE activation.*/
//  188 #if defined(STM32_HSE_BYPASS)
//  189   /* HSE Bypass.*/
//  190   RCC->CR |= RCC_CR_HSEON | RCC_CR_HSEBYP;
//  191 #else
//  192   /* No HSE Bypass.*/
//  193   RCC->CR |= RCC_CR_HSEON;
        LDR.N    R0,??DataTable2_15  ;; 0x40023800
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x10000
        LDR.N    R1,??DataTable2_15  ;; 0x40023800
        STR      R0,[R1, #+0]
//  194 #endif
//  195   while ((RCC->CR & RCC_CR_HSERDY) == 0)
??stm32_clock_init_2:
        LDR.N    R0,??DataTable2_15  ;; 0x40023800
        LDR      R0,[R0, #+0]
        LSLS     R0,R0,#+14
        BPL.N    ??stm32_clock_init_2
//  196     ;                           /* Waits until HSE is stable.               */
//  197 #endif
//  198 
//  199 #if STM32_LSI_ENABLED
//  200   /* LSI activation.*/
//  201   RCC->CSR |= RCC_CSR_LSION;
        LDR.N    R0,??DataTable2_17  ;; 0x40023874
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x1
        LDR.N    R1,??DataTable2_17  ;; 0x40023874
        STR      R0,[R1, #+0]
//  202   while ((RCC->CSR & RCC_CSR_LSIRDY) == 0)
??stm32_clock_init_3:
        LDR.N    R0,??DataTable2_17  ;; 0x40023874
        LDR      R0,[R0, #+0]
        LSLS     R0,R0,#+30
        BPL.N    ??stm32_clock_init_3
//  203     ;                           /* Waits until LSI is stable.               */
//  204 #endif
//  205 
//  206 #if STM32_ACTIVATE_PLL
//  207   /* PLL activation.*/
//  208   RCC->PLLCFGR = STM32_PLLQ | STM32_PLLSRC | STM32_PLLP | STM32_PLLN |
//  209                  STM32_PLLM;
        LDR.N    R0,??DataTable2_18  ;; 0x40023804
        LDR.N    R1,??DataTable2_19  ;; 0x7405408
        STR      R1,[R0, #+0]
//  210   RCC->CR |= RCC_CR_PLLON;
        LDR.N    R0,??DataTable2_15  ;; 0x40023800
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x1000000
        LDR.N    R1,??DataTable2_15  ;; 0x40023800
        STR      R0,[R1, #+0]
//  211 
//  212   /* Synchronization with voltage regulator stabilization.*/
//  213 #if defined(STM32F4XX)
//  214   while ((PWR->CSR & PWR_CSR_VOSRDY) == 0)
??stm32_clock_init_4:
        LDR.N    R0,??DataTable2_3  ;; 0x40007004
        LDR      R0,[R0, #+0]
        LSLS     R0,R0,#+17
        BPL.N    ??stm32_clock_init_4
//  215     ;                           /* Waits until power regulator is stable.   */
//  216 
//  217 #if STM32_OVERDRIVE_REQUIRED
//  218   /* Overdrive activation performed after activating the PLL in order to save
//  219      time as recommended in RM in "Entering Over-drive mode" paragraph.*/
//  220   PWR->CR |= PWR_CR_ODEN;
//  221   while (!(PWR->CSR & PWR_CSR_ODRDY))
//  222       ;
//  223   PWR->CR |= PWR_CR_ODSWEN;
//  224   while (!(PWR->CSR & PWR_CSR_ODSWRDY))
//  225       ;
//  226 #endif /* STM32_OVERDRIVE_REQUIRED */
//  227 #endif /* defined(STM32F4XX) */
//  228 
//  229   /* Waiting for PLL lock.*/
//  230   while (!(RCC->CR & RCC_CR_PLLRDY))
??stm32_clock_init_5:
        LDR.N    R0,??DataTable2_15  ;; 0x40023800
        LDR      R0,[R0, #+0]
        LSLS     R0,R0,#+6
        BPL.N    ??stm32_clock_init_5
//  231     ;
//  232 #endif /* STM32_OVERDRIVE_REQUIRED */
//  233 
//  234 #if STM32_ACTIVATE_PLLI2S
//  235   /* PLLI2S activation.*/
//  236   RCC->PLLI2SCFGR = STM32_PLLI2SR | STM32_PLLI2SN;
//  237   RCC->CR |= RCC_CR_PLLI2SON;
//  238 
//  239   /* Waiting for PLL lock.*/
//  240   while (!(RCC->CR & RCC_CR_PLLI2SRDY))
//  241     ;
//  242 #endif
//  243 
//  244   /* Other clock-related settings (dividers, MCO etc).*/
//  245   RCC->CFGR = STM32_MCO2PRE | STM32_MCO2SEL | STM32_MCO1PRE | STM32_MCO1SEL |
//  246               STM32_RTCPRE | STM32_PPRE2 | STM32_PPRE1 | STM32_HPRE;
        LDR.N    R0,??DataTable2_16  ;; 0x40023808
        LDR.N    R1,??DataTable2_20  ;; 0x38089400
        STR      R1,[R0, #+0]
//  247 
//  248   /* Flash setup.*/
//  249 #if defined(STM32_USE_REVISION_A_FIX)
//  250   /* Some old revisions of F4x MCUs randomly crashes with compiler
//  251      optimizations enabled AND flash caches enabled. */
//  252   if ((DBGMCU->IDCODE == 0x20006411) && (SCB->CPUID == 0x410FC241))
//  253     FLASH->ACR = FLASH_ACR_PRFTEN | STM32_FLASHBITS;
//  254   else
//  255     FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN |
//  256                  FLASH_ACR_DCEN | STM32_FLASHBITS;
//  257 #else
//  258   FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN |
//  259                FLASH_ACR_DCEN | STM32_FLASHBITS;
        LDR.N    R0,??DataTable2_21  ;; 0x40023c00
        MOVW     R1,#+1797
        STR      R1,[R0, #+0]
//  260 #endif
//  261 
//  262   /* Switching to the configured clock source if it is different from MSI.*/
//  263 #if (STM32_SW != STM32_SW_HSI)
//  264   RCC->CFGR |= STM32_SW;        /* Switches on the selected clock source.   */
        LDR.N    R0,??DataTable2_16  ;; 0x40023808
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x2
        LDR.N    R1,??DataTable2_16  ;; 0x40023808
        STR      R0,[R1, #+0]
//  265   while ((RCC->CFGR & RCC_CFGR_SWS) != (STM32_SW << 2))
??stm32_clock_init_6:
        LDR.N    R0,??DataTable2_16  ;; 0x40023808
        LDR      R0,[R0, #+0]
        ANDS     R0,R0,#0xC
        CMP      R0,#+8
        BNE.N    ??stm32_clock_init_6
//  266     ;
//  267 #endif
//  268 #endif /* STM32_NO_INIT */
//  269 
//  270   /* SYSCFG clock enabled here because it is a multi-functional unit shared
//  271      among multiple drivers.*/
//  272   rccEnableAPB2(RCC_APB2ENR_SYSCFGEN, TRUE);
        LDR.N    R0,??DataTable2_22  ;; 0x40023844
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x4000
        LDR.N    R1,??DataTable2_22  ;; 0x40023844
        STR      R0,[R1, #+0]
        LDR.N    R0,??DataTable2_23  ;; 0x40023864
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x4000
        LDR.N    R1,??DataTable2_23  ;; 0x40023864
        STR      R0,[R1, #+0]
//  273 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock2

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2:
        DC32     0x40007000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_1:
        DC32     0x40023870

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_2:
        DC32     0xf4240

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_3:
        DC32     0x40007004

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_4:
        DC32     0x40023810

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_5:
        DC32     0x40023814

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_6:
        DC32     0x40023820

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_7:
        DC32     0x40023824

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_8:
        DC32     0xe000e014

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_9:
        DC32     0x2903f

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_10:
        DC32     0xe000e018

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_11:
        DC32     0xe000e010

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_12:
        DC32     0xe000edfc

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_13:
        DC32     0xe0001000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_14:
        DC32     0x40023840

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_15:
        DC32     0x40023800

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_16:
        DC32     0x40023808

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_17:
        DC32     0x40023874

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_18:
        DC32     0x40023804

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_19:
        DC32     0x7405408

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_20:
        DC32     0x38089400

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_21:
        DC32     0x40023c00

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_22:
        DC32     0x40023844

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_23:
        DC32     0x40023864

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        END
//  274 
//  275 /** @} */
// 
// 572 bytes in section .text
// 
// 572 bytes of CODE memory
//
//Errors: none
//Warnings: none
