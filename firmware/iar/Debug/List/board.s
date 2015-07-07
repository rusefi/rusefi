///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:39 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\boards\ /
//                       ST_STM32F4_DISCOVERY\board.c                         /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\boards\ /
//                       ST_STM32F4_DISCOVERY\board.c -lCN                    /
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
//                       board.s                                              /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME board

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN stm32_clock_init

        PUBLIC __early_init
        PUBLIC boardInit
        PUBLIC mmc_lld_is_card_inserted
        PUBLIC mmc_lld_is_write_protected
        PUBLIC pal_default_config
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\chibios\boards\ST_STM32F4_DISCOVERY\board.c
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
//   17 #include "ch.h"
//   18 #include "hal.h"
//   19 
//   20 #if HAL_USE_PAL || defined(__DOXYGEN__)
//   21 /**
//   22  * @brief   PAL setup.
//   23  * @details Digital I/O ports static configuration as defined in @p board.h.
//   24  *          This variable is used by the HAL when initializing the PAL driver.
//   25  */

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
//   26 const PALConfig pal_default_config =
pal_default_config:
        DATA
        DC32 715172352, 0, 4294945791, 1073807444, 65535, 1431699456, 698880
        DC32 532608, 576, 4294967295, 1431389461, 65535, 67108864, 64, 35684353
        DC32 0, 4294967295, 1413813588, 65535, 1610612736, 394752, 1426063616
        DC32 0, 4294967295, 5591125, 4095, 0, 0, 64, 0, 4294967295, 0, 65535, 0
        DC32 0, 0, 0, 4294967295, 0, 65535, 0, 0, 0, 0, 4294967295, 0, 65535, 0
        DC32 0, 0, 0, 4294967295, 0, 65535, 0, 0, 0, 0, 4294967295, 0, 65535, 0
        DC32 0
//   27 {
//   28   {VAL_GPIOA_MODER, VAL_GPIOA_OTYPER, VAL_GPIOA_OSPEEDR, VAL_GPIOA_PUPDR,
//   29    VAL_GPIOA_ODR,   VAL_GPIOA_AFRL,   VAL_GPIOA_AFRH},
//   30   {VAL_GPIOB_MODER, VAL_GPIOB_OTYPER, VAL_GPIOB_OSPEEDR, VAL_GPIOB_PUPDR,
//   31    VAL_GPIOB_ODR,   VAL_GPIOB_AFRL,   VAL_GPIOB_AFRH},
//   32   {VAL_GPIOC_MODER, VAL_GPIOC_OTYPER, VAL_GPIOC_OSPEEDR, VAL_GPIOC_PUPDR,
//   33    VAL_GPIOC_ODR,   VAL_GPIOC_AFRL,   VAL_GPIOC_AFRH},
//   34   {VAL_GPIOD_MODER, VAL_GPIOD_OTYPER, VAL_GPIOD_OSPEEDR, VAL_GPIOD_PUPDR,
//   35    VAL_GPIOD_ODR,   VAL_GPIOD_AFRL,   VAL_GPIOD_AFRH},
//   36   {VAL_GPIOE_MODER, VAL_GPIOE_OTYPER, VAL_GPIOE_OSPEEDR, VAL_GPIOE_PUPDR,
//   37    VAL_GPIOE_ODR,   VAL_GPIOE_AFRL,   VAL_GPIOE_AFRH},
//   38   {VAL_GPIOF_MODER, VAL_GPIOF_OTYPER, VAL_GPIOF_OSPEEDR, VAL_GPIOF_PUPDR,
//   39    VAL_GPIOF_ODR,   VAL_GPIOF_AFRL,   VAL_GPIOF_AFRH},
//   40   {VAL_GPIOG_MODER, VAL_GPIOG_OTYPER, VAL_GPIOG_OSPEEDR, VAL_GPIOG_PUPDR,
//   41    VAL_GPIOG_ODR,   VAL_GPIOG_AFRL,   VAL_GPIOG_AFRH},
//   42   {VAL_GPIOH_MODER, VAL_GPIOH_OTYPER, VAL_GPIOH_OSPEEDR, VAL_GPIOH_PUPDR,
//   43    VAL_GPIOH_ODR,   VAL_GPIOH_AFRL,   VAL_GPIOH_AFRH},
//   44   {VAL_GPIOI_MODER, VAL_GPIOI_OTYPER, VAL_GPIOI_OSPEEDR, VAL_GPIOI_PUPDR,
//   45    VAL_GPIOI_ODR,   VAL_GPIOI_AFRL,   VAL_GPIOI_AFRH}
//   46 };
//   47 #endif
//   48 
//   49 /**
//   50  * @brief   Early initialization code.
//   51  * @details This initialization must be performed just after stack setup
//   52  *          and before any other initialization.
//   53  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function __early_init
        THUMB
//   54 void __early_init(void) {
__early_init:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   55 
//   56   stm32_clock_init();
          CFI FunCall stm32_clock_init
        BL       stm32_clock_init
//   57 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock0
//   58 
//   59 #if HAL_USE_SDC || defined(__DOXYGEN__)
//   60 /**
//   61  * @brief   SDC card detection.
//   62  */
//   63 bool_t sdc_lld_is_card_inserted(SDCDriver *sdcp) {
//   64 
//   65   (void)sdcp;
//   66   /* TODO: Fill the implementation.*/
//   67   return TRUE;
//   68 }
//   69 
//   70 /**
//   71  * @brief   SDC card write protection detection.
//   72  */
//   73 bool_t sdc_lld_is_write_protected(SDCDriver *sdcp) {
//   74 
//   75   (void)sdcp;
//   76   /* TODO: Fill the implementation.*/
//   77   return FALSE;
//   78 }
//   79 #endif /* HAL_USE_SDC */
//   80 
//   81 #if HAL_USE_MMC_SPI || defined(__DOXYGEN__)
//   82 /**
//   83  * @brief   MMC_SPI card detection.
//   84  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function mmc_lld_is_card_inserted
          CFI NoCalls
        THUMB
//   85 bool_t mmc_lld_is_card_inserted(MMCDriver *mmcp) {
mmc_lld_is_card_inserted:
        MOVS     R1,R0
//   86 
//   87   (void)mmcp;
//   88   /* TODO: Fill the implementation.*/
//   89   return TRUE;
        MOVS     R0,#+1
        BX       LR               ;; return
          CFI EndBlock cfiBlock1
//   90 }
//   91 
//   92 /**
//   93  * @brief   MMC_SPI card write protection detection.
//   94  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function mmc_lld_is_write_protected
          CFI NoCalls
        THUMB
//   95 bool_t mmc_lld_is_write_protected(MMCDriver *mmcp) {
mmc_lld_is_write_protected:
        MOVS     R1,R0
//   96 
//   97   (void)mmcp;
//   98   /* TODO: Fill the implementation.*/
//   99   return FALSE;
        MOVS     R0,#+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock2
//  100 }
//  101 #endif
//  102 
//  103 /**
//  104  * @brief   Board-specific initialization code.
//  105  * @todo    Add your board-specific code, if any.
//  106  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function boardInit
          CFI NoCalls
        THUMB
//  107 void boardInit(void) {
//  108 }
boardInit:
        BX       LR               ;; return
          CFI EndBlock cfiBlock3

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        END
// 
// 252 bytes in section .rodata
//  22 bytes in section .text
// 
//  22 bytes of CODE  memory
// 252 bytes of CONST memory
//
//Errors: none
//Warnings: none
