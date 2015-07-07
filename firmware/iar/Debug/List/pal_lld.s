///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:57 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       platforms\STM32\GPIOv2\pal_lld.c                     /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       platforms\STM32\GPIOv2\pal_lld.c -lCN                /
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
//                       pal_lld.s                                            /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME pal_lld

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        PUBLIC _pal_lld_init
        PUBLIC _pal_lld_setgroupmode
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\platforms\STM32\GPIOv2\pal_lld.c
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
//   18  * @file    STM32/GPIOv2/pal_lld.c
//   19  * @brief   STM32L1xx/STM32F2xx/STM32F4xx GPIO low level driver code.
//   20  *
//   21  * @addtogroup PAL
//   22  * @{
//   23  */
//   24 
//   25 #include "ch.h"
//   26 #include "hal.h"
//   27 
//   28 #if HAL_USE_PAL || defined(__DOXYGEN__)
//   29 
//   30 /*===========================================================================*/
//   31 /* Driver local definitions.                                                 */
//   32 /*===========================================================================*/
//   33 
//   34 #if defined(STM32L1XX)
//   35 #define AHB_EN_MASK     (RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN |          \ 
//   36                          RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIODEN |          \ 
//   37                          RCC_AHBENR_GPIOEEN | RCC_AHBENR_GPIOHEN)
//   38 #define AHB_LPEN_MASK   AHB_EN_MASK
//   39 
//   40 #elif defined(STM32F030) || defined(STM32F0XX_MD)
//   41 #define AHB_EN_MASK     (RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN |          \ 
//   42                          RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIODEN |          \ 
//   43                          RCC_AHBENR_GPIOFEN)
//   44 
//   45 #elif defined(STM32F0XX_LD)
//   46 #define AHB_EN_MASK     (RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN |          \ 
//   47                          RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIOFEN)
//   48 
//   49 #elif defined(STM32F2XX)
//   50 #define AHB1_EN_MASK    (RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN |        \ 
//   51                          RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIODEN |        \ 
//   52                          RCC_AHB1ENR_GPIOEEN | RCC_AHB1ENR_GPIOFEN |        \ 
//   53                          RCC_AHB1ENR_GPIOGEN | RCC_AHB1ENR_GPIOHEN |        \ 
//   54                          RCC_AHB1ENR_GPIOIEN)
//   55 #define AHB1_LPEN_MASK  AHB1_EN_MASK
//   56 
//   57 #elif defined(STM32F30X) || defined(STM32F37X)
//   58 #define AHB_EN_MASK     (RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN |          \ 
//   59                          RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIODEN |          \ 
//   60                          RCC_AHBENR_GPIOEEN | RCC_AHBENR_GPIOFEN)
//   61 
//   62 #elif defined(STM32F4XX)
//   63 #if STM32_HAS_GPIOF && STM32_HAS_GPIOG && STM32_HAS_GPIOI
//   64 #define AHB1_EN_MASK    (RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN |        \ 
//   65                          RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIODEN |        \ 
//   66                          RCC_AHB1ENR_GPIOEEN | RCC_AHB1ENR_GPIOFEN |        \ 
//   67                          RCC_AHB1ENR_GPIOGEN | RCC_AHB1ENR_GPIOHEN |        \ 
//   68                          RCC_AHB1ENR_GPIOIEN)
//   69 #else
//   70 #define AHB1_EN_MASK    (RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN |        \ 
//   71                          RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIODEN |        \ 
//   72                          RCC_AHB1ENR_GPIOEEN)
//   73 #endif /* STM32_HAS_GPIOF && STM32_HAS_GPIOG && STM32_HAS_GPIOI */
//   74 #define AHB1_LPEN_MASK  AHB1_EN_MASK
//   75 
//   76 #else
//   77 #error "missing or unsupported platform for GPIOv2 PAL driver"
//   78 #endif
//   79 
//   80 /*===========================================================================*/
//   81 /* Driver exported variables.                                                */
//   82 /*===========================================================================*/
//   83 
//   84 /*===========================================================================*/
//   85 /* Driver local variables and types.                                         */
//   86 /*===========================================================================*/
//   87 
//   88 /*===========================================================================*/
//   89 /* Driver local functions.                                                   */
//   90 /*===========================================================================*/
//   91 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function initgpio
          CFI NoCalls
        THUMB
//   92 static void initgpio(GPIO_TypeDef *gpiop, const stm32_gpio_setup_t *config) {
//   93 
//   94   gpiop->OTYPER  = config->otyper;
initgpio:
        LDR      R2,[R1, #+4]
        STR      R2,[R0, #+4]
//   95   gpiop->OSPEEDR = config->ospeedr;
        LDR      R2,[R1, #+8]
        STR      R2,[R0, #+8]
//   96   gpiop->PUPDR   = config->pupdr;
        LDR      R2,[R1, #+12]
        STR      R2,[R0, #+12]
//   97   gpiop->ODR     = config->odr;
        LDR      R2,[R1, #+16]
        STR      R2,[R0, #+20]
//   98   gpiop->AFRL    = config->afrl;
        LDR      R2,[R1, #+20]
        STR      R2,[R0, #+32]
//   99   gpiop->AFRH    = config->afrh;
        LDR      R2,[R1, #+24]
        STR      R2,[R0, #+36]
//  100   gpiop->MODER   = config->moder;
        LDR      R2,[R1, #+0]
        STR      R2,[R0, #+0]
//  101 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock0
//  102 
//  103 /*===========================================================================*/
//  104 /* Driver interrupt handlers.                                                */
//  105 /*===========================================================================*/
//  106 
//  107 /*===========================================================================*/
//  108 /* Driver exported functions.                                                */
//  109 /*===========================================================================*/
//  110 
//  111 /**
//  112  * @brief   STM32 I/O ports configuration.
//  113  * @details Ports A-D(E, F, G, H) clocks enabled.
//  114  *
//  115  * @param[in] config    the STM32 ports configuration
//  116  *
//  117  * @notapi
//  118  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _pal_lld_init
        THUMB
//  119 void _pal_lld_init(const PALConfig *config) {
_pal_lld_init:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  120 
//  121   /*
//  122    * Enables the GPIO related clocks.
//  123    */
//  124 #if defined(STM32L1XX)
//  125   rccEnableAHB(AHB_EN_MASK, TRUE);
//  126   RCC->AHBLPENR |= AHB_LPEN_MASK;
//  127 #elif defined(STM32F0XX)
//  128   rccEnableAHB(AHB_EN_MASK, TRUE);
//  129 #elif defined(STM32F30X) || defined(STM32F37X)
//  130   rccEnableAHB(AHB_EN_MASK, TRUE);
//  131 #elif defined(STM32F2XX) || defined(STM32F4XX)
//  132   RCC->AHB1ENR   |= AHB1_EN_MASK;
        LDR.N    R0,??DataTable0  ;; 0x40023830
        LDR      R0,[R0, #+0]
        MOVW     R1,#+511
        ORRS     R0,R1,R0
        LDR.N    R1,??DataTable0  ;; 0x40023830
        STR      R0,[R1, #+0]
//  133   RCC->AHB1LPENR |= AHB1_LPEN_MASK;
        LDR.N    R0,??DataTable0_1  ;; 0x40023850
        LDR      R0,[R0, #+0]
        MOVW     R1,#+511
        ORRS     R0,R1,R0
        LDR.N    R1,??DataTable0_1  ;; 0x40023850
        STR      R0,[R1, #+0]
//  134 #endif
//  135 
//  136   /*
//  137    * Initial GPIO setup.
//  138    */
//  139 #if STM32_HAS_GPIOA
//  140   initgpio(GPIOA, &config->PAData);
        MOVS     R1,R4
        LDR.N    R0,??DataTable0_2  ;; 0x40020000
          CFI FunCall initgpio
        BL       initgpio
//  141 #endif
//  142 #if STM32_HAS_GPIOB
//  143   initgpio(GPIOB, &config->PBData);
        ADDS     R1,R4,#+28
        LDR.N    R0,??DataTable0_3  ;; 0x40020400
          CFI FunCall initgpio
        BL       initgpio
//  144 #endif
//  145 #if STM32_HAS_GPIOC
//  146   initgpio(GPIOC, &config->PCData);
        ADDS     R1,R4,#+56
        LDR.N    R0,??DataTable0_4  ;; 0x40020800
          CFI FunCall initgpio
        BL       initgpio
//  147 #endif
//  148 #if STM32_HAS_GPIOD
//  149   initgpio(GPIOD, &config->PDData);
        ADDS     R1,R4,#+84
        LDR.N    R0,??DataTable0_5  ;; 0x40020c00
          CFI FunCall initgpio
        BL       initgpio
//  150 #endif
//  151 #if STM32_HAS_GPIOE
//  152   initgpio(GPIOE, &config->PEData);
        ADDS     R1,R4,#+112
        LDR.N    R0,??DataTable0_6  ;; 0x40021000
          CFI FunCall initgpio
        BL       initgpio
//  153 #endif
//  154 #if STM32_HAS_GPIOF
//  155   initgpio(GPIOF, &config->PFData);
        ADDS     R1,R4,#+140
        LDR.N    R0,??DataTable0_7  ;; 0x40021400
          CFI FunCall initgpio
        BL       initgpio
//  156 #endif
//  157 #if STM32_HAS_GPIOG
//  158   initgpio(GPIOG, &config->PGData);
        ADDS     R1,R4,#+168
        LDR.N    R0,??DataTable0_8  ;; 0x40021800
          CFI FunCall initgpio
        BL       initgpio
//  159 #endif
//  160 #if STM32_HAS_GPIOH
//  161   initgpio(GPIOH, &config->PHData);
        ADDS     R1,R4,#+196
        LDR.N    R0,??DataTable0_9  ;; 0x40021c00
          CFI FunCall initgpio
        BL       initgpio
//  162 #endif
//  163 #if STM32_HAS_GPIOI
//  164   initgpio(GPIOI, &config->PIData);
        ADDS     R1,R4,#+224
        LDR.N    R0,??DataTable0_10  ;; 0x40022000
          CFI FunCall initgpio
        BL       initgpio
//  165 #endif
//  166 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock1

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0:
        DC32     0x40023830

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0_1:
        DC32     0x40023850

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0_2:
        DC32     0x40020000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0_3:
        DC32     0x40020400

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0_4:
        DC32     0x40020800

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0_5:
        DC32     0x40020c00

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0_6:
        DC32     0x40021000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0_7:
        DC32     0x40021400

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0_8:
        DC32     0x40021800

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0_9:
        DC32     0x40021c00

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0_10:
        DC32     0x40022000
//  167 
//  168 /**
//  169  * @brief   Pads mode setup.
//  170  * @details This function programs a pads group belonging to the same port
//  171  *          with the specified mode.
//  172  * @note    @p PAL_MODE_UNCONNECTED is implemented as push pull at minimum
//  173  *          speed.
//  174  *
//  175  * @param[in] port      the port identifier
//  176  * @param[in] mask      the group mask
//  177  * @param[in] mode      the mode
//  178  *
//  179  * @notapi
//  180  */
//  181 #if 1

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _pal_lld_setgroupmode
          CFI NoCalls
        THUMB
//  182 void _pal_lld_setgroupmode(ioportid_t port,
//  183                            ioportmask_t mask,
//  184                            iomode_t mode) {
_pal_lld_setgroupmode:
        PUSH     {R3-R11,LR}
          CFI R14 Frame(CFA, -4)
          CFI R11 Frame(CFA, -8)
          CFI R10 Frame(CFA, -12)
          CFI R9 Frame(CFA, -16)
          CFI R8 Frame(CFA, -20)
          CFI R7 Frame(CFA, -24)
          CFI R6 Frame(CFA, -28)
          CFI R5 Frame(CFA, -32)
          CFI R4 Frame(CFA, -36)
          CFI CFA R13+40
//  185 
//  186   uint32_t moder   = (mode & PAL_STM32_MODE_MASK) >> 0;
        ANDS     R3,R2,#0x3
//  187   uint32_t otyper  = (mode & PAL_STM32_OTYPE_MASK) >> 2;
        UBFX     R4,R2,#+2,#+1
//  188   uint32_t ospeedr = (mode & PAL_STM32_OSPEED_MASK) >> 3;
        UBFX     R5,R2,#+3,#+2
//  189   uint32_t pupdr   = (mode & PAL_STM32_PUDR_MASK) >> 5;
        UBFX     R6,R2,#+5,#+2
//  190   uint32_t altr    = (mode & PAL_STM32_ALTERNATE_MASK) >> 7;
        UBFX     R7,R2,#+7,#+4
        STR      R7,[SP, #+0]
//  191   uint32_t bit     = 0;
        MOVS     R7,#+0
//  192   while (TRUE) {
//  193     if ((mask & 1) != 0) {
??_pal_lld_setgroupmode_0:
        LSLS     R12,R1,#+31
        BPL.N    ??_pal_lld_setgroupmode_1
//  194       uint32_t altrmask, m1, m2, m4;
//  195 
//  196       altrmask = altr << ((bit & 7) * 4);
        LDR      R10,[SP, #+0]
        ANDS     R11,R7,#0x7
        UXTB     R11,R11          ;; ZeroExt  R11,R11,#+24,#+24
        LSLS     R11,R11,#+2
        LSLS     R10,R10,R11
        MOV      R12,R10
//  197       m4 = 15 << ((bit & 7) * 4);
        MOVS     R10,#+15
        ANDS     R11,R7,#0x7
        UXTB     R11,R11          ;; ZeroExt  R11,R11,#+24,#+24
        LSLS     R11,R11,#+2
        LSLS     R10,R10,R11
        MOV      R9,R10
//  198       if (bit < 8)
        CMP      R7,#+8
        BCS.N    ??_pal_lld_setgroupmode_2
//  199         port->AFRL = (port->AFRL & ~m4) | altrmask;
        LDR      R10,[R0, #+32]
        BICS     R10,R10,R9
        ORRS     R10,R12,R10
        STR      R10,[R0, #+32]
        B.N      ??_pal_lld_setgroupmode_3
//  200       else
//  201         port->AFRH = (port->AFRH & ~m4) | altrmask;
??_pal_lld_setgroupmode_2:
        LDR      R10,[R0, #+36]
        BICS     R10,R10,R9
        ORRS     R10,R12,R10
        STR      R10,[R0, #+36]
//  202       m1 = 1 << bit;
??_pal_lld_setgroupmode_3:
        MOVS     R10,#+1
        LSLS     R10,R10,R7
        MOV      LR,R10
//  203       port->OTYPER  = (port->OTYPER & ~m1) | otyper;
        LDR      R10,[R0, #+4]
        BICS     R10,R10,LR
        ORRS     R10,R4,R10
        STR      R10,[R0, #+4]
//  204       m2 = 3 << (bit * 2);
        MOVS     R10,#+3
        MOV      R11,R7
        UXTB     R11,R11          ;; ZeroExt  R11,R11,#+24,#+24
        LSLS     R11,R11,#+1
        LSLS     R10,R10,R11
        MOV      R8,R10
//  205       port->OSPEEDR = (port->OSPEEDR & ~m2) | ospeedr;
        LDR      R10,[R0, #+8]
        BICS     R10,R10,R8
        ORRS     R10,R5,R10
        STR      R10,[R0, #+8]
//  206       port->PUPDR   = (port->PUPDR & ~m2) | pupdr;
        LDR      R10,[R0, #+12]
        BICS     R10,R10,R8
        ORRS     R10,R6,R10
        STR      R10,[R0, #+12]
//  207       port->MODER   = (port->MODER & ~m2) | moder;
        LDR      R10,[R0, #+0]
        BICS     R8,R10,R8
        ORRS     R8,R3,R8
        STR      R8,[R0, #+0]
//  208     }
//  209     mask >>= 1;
??_pal_lld_setgroupmode_1:
        LSRS     R1,R1,#+1
//  210     if (!mask)
        CMP      R1,#+0
        BEQ.N    ??_pal_lld_setgroupmode_4
//  211       return;
//  212     otyper <<= 1;
??_pal_lld_setgroupmode_5:
        LSLS     R4,R4,#+1
//  213     ospeedr <<= 2;
        LSLS     R5,R5,#+2
//  214     pupdr <<= 2;
        LSLS     R6,R6,#+2
//  215     moder <<= 2;
        LSLS     R3,R3,#+2
//  216     bit++;
        ADDS     R7,R7,#+1
        B.N      ??_pal_lld_setgroupmode_0
//  217   }
??_pal_lld_setgroupmode_4:
        POP      {R0,R4-R11,PC}   ;; return
          CFI EndBlock cfiBlock2
//  218 }

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        END
//  219 #else
//  220 void _pal_lld_setgroupmode(ioportid_t port,
//  221                            ioportmask_t mask,
//  222                            iomode_t mode) {
//  223   uint32_t afrm, moderm, pupdrm, otyperm, ospeedrm;
//  224   uint32_t m1 = (uint32_t)mask;
//  225   uint32_t m2 = 0;
//  226   uint32_t m4l = 0;
//  227   uint32_t m4h = 0;
//  228   uint32_t bit = 0;
//  229   do {
//  230     if ((mask & 1) != 0) {
//  231       m2 |= 3 << bit;
//  232       if (bit < 16)
//  233         m4l |= 15 << ((bit & 14) * 2);
//  234       else
//  235         m4h |= 15 << ((bit & 14) * 2);
//  236     }
//  237     bit += 2;
//  238     mask >>= 1;
//  239   } while (mask);
//  240 
//  241   afrm = ((mode & PAL_STM32_ALTERNATE_MASK) >> 7) * 0x1111;
//  242   port->AFRL = (port->AFRL & ~m4l) | (afrm & m4l);
//  243   port->AFRH = (port->AFRH & ~m4h) | (afrm & m4h);
//  244 
//  245   ospeedrm = ((mode & PAL_STM32_OSPEED_MASK) >> 3) * 0x5555;
//  246   port->OSPEEDR = (port->OSPEEDR & ~m2) | (ospeedrm & m2);
//  247 
//  248   otyperm = ((mode & PAL_STM32_OTYPE_MASK) >> 2) * 0xffff;
//  249   port->OTYPER = (port->OTYPER & ~m1) | (otyperm & m1);
//  250 
//  251   pupdrm = ((mode & PAL_STM32_PUDR_MASK) >> 5) * 0x5555;
//  252   port->PUPDR = (port->PUPDR & ~m2) | (pupdrm & m2);
//  253 
//  254   moderm = ((mode & PAL_STM32_MODE_MASK) >> 0) * 0x5555;
//  255   port->MODER = (port->MODER & ~m2) | (moderm & m2);
//  256 }
//  257 #endif
//  258 
//  259 #endif /* HAL_USE_PAL */
//  260 
//  261 /** @} */
// 
// 428 bytes in section .text
// 
// 428 bytes of CODE memory
//
//Errors: none
//Warnings: none
