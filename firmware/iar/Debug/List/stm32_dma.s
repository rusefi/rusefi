///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:08:54 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       platforms\STM32F4xx\stm32_dma.c                      /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       platforms\STM32F4xx\stm32_dma.c -lCN                 /
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
//                       stm32_dma.s                                          /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME stm32_dma

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _port_irq_epilogue
        EXTERN chDbgPanic
        EXTERN chDbgPanic3
        EXTERN dbg_check_enter_isr
        EXTERN dbg_check_leave_isr
        EXTERN nvicDisableVector
        EXTERN nvicEnableVector

        PUBLIC Vector120
        PUBLIC Vector124
        PUBLIC Vector128
        PUBLIC Vector12C
        PUBLIC Vector130
        PUBLIC Vector150
        PUBLIC Vector154
        PUBLIC Vector158
        PUBLIC Vector6C
        PUBLIC Vector70
        PUBLIC Vector74
        PUBLIC Vector78
        PUBLIC Vector7C
        PUBLIC Vector80
        PUBLIC Vector84
        PUBLIC VectorFC
        PUBLIC _stm32_dma_streams
        PUBLIC dmaInit
        PUBLIC dmaStreamAllocate
        PUBLIC dmaStreamRelease
        
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
        

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"dmaStreamAllocate\\"()">`:
        DATA
        DC8 "\"dmaStreamAllocate\"()"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "F:\\\\stuff\\\\rusefi_sourc...">`:
        DATA
        DC8 46H, 3AH, 5CH, 73H, 74H, 75H, 66H, 66H
        DC8 5CH, 72H, 75H, 73H, 65H, 66H, 69H, 5FH
        DC8 73H, 6FH, 75H, 72H, 63H, 65H, 66H, 6FH
        DC8 72H, 67H, 65H, 5CH, 66H, 69H, 72H, 6DH
        DC8 77H, 61H, 72H, 65H, 5CH, 63H, 68H, 69H
        DC8 62H, 69H, 6FH, 73H, 5CH, 6FH, 73H, 5CH
        DC8 68H, 61H, 6CH, 5CH, 70H, 6CH, 61H, 74H
        DC8 66H, 6FH, 72H, 6DH, 73H, 5CH, 53H, 54H
        DC8 4DH, 33H, 32H, 46H, 34H, 78H, 78H, 5CH
        DC8 73H, 74H, 6DH, 33H, 32H, 5FH, 64H, 6DH
        DC8 61H, 2EH, 63H, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"dmaStreamRelease\\"()">`:
        DATA
        DC8 "\"dmaStreamRelease\"()"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "dmaStreamRelease(), #1">`:
        DATA
        DC8 "dmaStreamRelease(), #1"
        DC8 0
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\platforms\STM32F4xx\stm32_dma.c
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
//   18  * @file    STM32F4xx/stm32_dma.c
//   19  * @brief   Enhanced DMA helper driver code.
//   20  *
//   21  * @addtogroup STM32F4xx_DMA
//   22  * @details DMA sharing helper driver. In the STM32 the DMA streams are a
//   23  *          shared resource, this driver allows to allocate and free DMA
//   24  *          streams at runtime in order to allow all the other device
//   25  *          drivers to coordinate the access to the resource.
//   26  * @note    The DMA ISR handlers are all declared into this module because
//   27  *          sharing, the various device drivers can associate a callback to
//   28  *          ISRs when allocating streams.
//   29  * @{
//   30  */
//   31 
//   32 #include "ch.h"
//   33 #include "hal.h"
//   34 
//   35 /* The following macro is only defined if some driver requiring DMA services
//   36    has been enabled.*/
//   37 #if defined(STM32_DMA_REQUIRED) || defined(__DOXYGEN__)
//   38 
//   39 /*===========================================================================*/
//   40 /* Driver local definitions.                                                 */
//   41 /*===========================================================================*/
//   42 
//   43 /**
//   44  * @brief   Mask of the DMA1 streams in @p dma_streams_mask.
//   45  */
//   46 #define STM32_DMA1_STREAMS_MASK     0x000000FF
//   47 
//   48 /**
//   49  * @brief   Mask of the DMA2 streams in @p dma_streams_mask.
//   50  */
//   51 #define STM32_DMA2_STREAMS_MASK     0x0000FF00
//   52 
//   53 /**
//   54  * @brief   Post-reset value of the stream CR register.
//   55  */
//   56 #define STM32_DMA_CR_RESET_VALUE    0x00000000
//   57 
//   58 /**
//   59  * @brief   Post-reset value of the stream FCR register.
//   60  */
//   61 #define STM32_DMA_FCR_RESET_VALUE   0x00000021
//   62 
//   63 /*===========================================================================*/
//   64 /* Driver exported variables.                                                */
//   65 /*===========================================================================*/
//   66 
//   67 /**
//   68  * @brief   DMA streams descriptors.
//   69  * @details This table keeps the association between an unique stream
//   70  *          identifier and the involved physical registers.
//   71  * @note    Don't use this array directly, use the appropriate wrapper macros
//   72  *          instead: @p STM32_DMA1_STREAM0, @p STM32_DMA1_STREAM1 etc.
//   73  */

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
//   74 const stm32_dma_stream_t _stm32_dma_streams[STM32_DMA_STREAMS] = {
_stm32_dma_streams:
        DATA
        DC32 40026010H, 40026008H
        DC8 0, 0, 11, 0
        DC32 40026028H, 40026008H
        DC8 6, 1, 12, 0
        DC32 40026040H, 40026008H
        DC8 16, 2, 13, 0
        DC32 40026058H, 40026008H
        DC8 22, 3, 14, 0
        DC32 40026070H, 4002600CH
        DC8 0, 4, 15, 0
        DC32 40026088H, 4002600CH
        DC8 6, 5, 16, 0
        DC32 400260A0H, 4002600CH
        DC8 16, 6, 17, 0
        DC32 400260B8H, 4002600CH
        DC8 22, 7, 47, 0
        DC32 40026410H, 40026408H
        DC8 0, 8, 56, 0
        DC32 40026428H, 40026408H
        DC8 6, 9, 57, 0
        DC32 40026440H, 40026408H
        DC8 16, 10, 58, 0
        DC32 40026458H, 40026408H
        DC8 22, 11, 59, 0
        DC32 40026470H, 4002640CH
        DC8 0, 12, 60, 0
        DC32 40026488H, 4002640CH
        DC8 6, 13, 68, 0
        DC32 400264A0H, 4002640CH
        DC8 16, 14, 69, 0
        DC32 400264B8H, 4002640CH
        DC8 22, 15, 70, 0
//   75   {DMA1_Stream0, &DMA1->LIFCR, 0, 0, DMA1_Stream0_IRQn},
//   76   {DMA1_Stream1, &DMA1->LIFCR, 6, 1, DMA1_Stream1_IRQn},
//   77   {DMA1_Stream2, &DMA1->LIFCR, 16, 2, DMA1_Stream2_IRQn},
//   78   {DMA1_Stream3, &DMA1->LIFCR, 22, 3, DMA1_Stream3_IRQn},
//   79   {DMA1_Stream4, &DMA1->HIFCR, 0, 4, DMA1_Stream4_IRQn},
//   80   {DMA1_Stream5, &DMA1->HIFCR, 6, 5, DMA1_Stream5_IRQn},
//   81   {DMA1_Stream6, &DMA1->HIFCR, 16, 6, DMA1_Stream6_IRQn},
//   82   {DMA1_Stream7, &DMA1->HIFCR, 22, 7, DMA1_Stream7_IRQn},
//   83   {DMA2_Stream0, &DMA2->LIFCR, 0, 8, DMA2_Stream0_IRQn},
//   84   {DMA2_Stream1, &DMA2->LIFCR, 6, 9, DMA2_Stream1_IRQn},
//   85   {DMA2_Stream2, &DMA2->LIFCR, 16, 10, DMA2_Stream2_IRQn},
//   86   {DMA2_Stream3, &DMA2->LIFCR, 22, 11, DMA2_Stream3_IRQn},
//   87   {DMA2_Stream4, &DMA2->HIFCR, 0, 12, DMA2_Stream4_IRQn},
//   88   {DMA2_Stream5, &DMA2->HIFCR, 6, 13, DMA2_Stream5_IRQn},
//   89   {DMA2_Stream6, &DMA2->HIFCR, 16, 14, DMA2_Stream6_IRQn},
//   90   {DMA2_Stream7, &DMA2->HIFCR, 22, 15, DMA2_Stream7_IRQn},
//   91 };
//   92 
//   93 /*===========================================================================*/
//   94 /* Driver local variables and types.                                         */
//   95 /*===========================================================================*/
//   96 
//   97 /**
//   98  * @brief   DMA ISR redirector type.
//   99  */
//  100 typedef struct {
//  101   stm32_dmaisr_t        dma_func;       /**< @brief DMA callback function.  */
//  102   void                  *dma_param;     /**< @brief DMA callback parameter. */
//  103 } dma_isr_redir_t;
//  104 
//  105 /**
//  106  * @brief   Mask of the allocated streams.
//  107  */

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  108 static uint32_t dma_streams_mask;
dma_streams_mask:
        DS8 4
//  109 
//  110 /**
//  111  * @brief   DMA IRQ redirectors.
//  112  */

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  113 static dma_isr_redir_t dma_isr_redir[STM32_DMA_STREAMS];
dma_isr_redir:
        DS8 128
//  114 
//  115 /*===========================================================================*/
//  116 /* Driver local functions.                                                   */
//  117 /*===========================================================================*/
//  118 
//  119 /*===========================================================================*/
//  120 /* Driver interrupt handlers.                                                */
//  121 /*===========================================================================*/
//  122 
//  123 /**
//  124  * @brief   DMA1 stream 0 shared interrupt handler.
//  125  *
//  126  * @isr
//  127  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function Vector6C
        THUMB
//  128 CH_IRQ_HANDLER(DMA1_Stream0_IRQHandler) {
Vector6C:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//  129   uint32_t flags;
//  130 
//  131   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//  132 
//  133   flags = (DMA1->LISR >> 0) & STM32_DMA_ISR_MASK;
        LDR.W    R0,??DataTable18  ;; 0x40026000
        LDR      R0,[R0, #+0]
        ANDS     R0,R0,#0x3D
        MOVS     R4,R0
//  134   DMA1->LIFCR = flags << 0;
        LDR.W    R0,??DataTable18_1  ;; 0x40026008
        STR      R4,[R0, #+0]
//  135   if (dma_isr_redir[0].dma_func)
        LDR.W    R0,??DataTable18_2
        LDR      R0,[R0, #+0]
        CMP      R0,#+0
        BEQ.N    ??Vector6C_0
//  136     dma_isr_redir[0].dma_func(dma_isr_redir[0].dma_param, flags);
        MOVS     R1,R4
        LDR.W    R0,??DataTable18_2
        LDR      R0,[R0, #+4]
        LDR.W    R2,??DataTable18_2
        LDR      R2,[R2, #+0]
          CFI FunCall
        BLX      R2
//  137 
//  138   CH_IRQ_EPILOGUE();
??Vector6C_0:
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//  139 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock0
//  140 
//  141 /**
//  142  * @brief   DMA1 stream 1 shared interrupt handler.
//  143  *
//  144  * @isr
//  145  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function Vector70
        THUMB
//  146 CH_IRQ_HANDLER(DMA1_Stream1_IRQHandler) {
Vector70:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//  147   uint32_t flags;
//  148 
//  149   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//  150 
//  151   flags = (DMA1->LISR >> 6) & STM32_DMA_ISR_MASK;
        LDR.W    R0,??DataTable18  ;; 0x40026000
        LDR      R0,[R0, #+0]
        LSRS     R0,R0,#+6
        ANDS     R0,R0,#0x3D
        MOVS     R4,R0
//  152   DMA1->LIFCR = flags << 6;
        LSLS     R0,R4,#+6
        LDR.W    R1,??DataTable18_1  ;; 0x40026008
        STR      R0,[R1, #+0]
//  153   if (dma_isr_redir[1].dma_func)
        LDR.W    R0,??DataTable18_2
        LDR      R0,[R0, #+8]
        CMP      R0,#+0
        BEQ.N    ??Vector70_0
//  154     dma_isr_redir[1].dma_func(dma_isr_redir[1].dma_param, flags);
        MOVS     R1,R4
        LDR.W    R0,??DataTable18_2
        LDR      R0,[R0, #+12]
        LDR.W    R2,??DataTable18_2
        LDR      R2,[R2, #+8]
          CFI FunCall
        BLX      R2
//  155 
//  156   CH_IRQ_EPILOGUE();
??Vector70_0:
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//  157 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock1
//  158 
//  159 /**
//  160  * @brief   DMA1 stream 2 shared interrupt handler.
//  161  *
//  162  * @isr
//  163  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function Vector74
        THUMB
//  164 CH_IRQ_HANDLER(DMA1_Stream2_IRQHandler) {
Vector74:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//  165   uint32_t flags;
//  166 
//  167   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//  168 
//  169   flags = (DMA1->LISR >> 16) & STM32_DMA_ISR_MASK;
        LDR.W    R0,??DataTable18  ;; 0x40026000
        LDR      R0,[R0, #+0]
        LSRS     R0,R0,#+16
        ANDS     R0,R0,#0x3D
        MOVS     R4,R0
//  170   DMA1->LIFCR = flags << 16;
        LSLS     R0,R4,#+16
        LDR.W    R1,??DataTable18_1  ;; 0x40026008
        STR      R0,[R1, #+0]
//  171   if (dma_isr_redir[2].dma_func)
        LDR.W    R0,??DataTable18_2
        LDR      R0,[R0, #+16]
        CMP      R0,#+0
        BEQ.N    ??Vector74_0
//  172     dma_isr_redir[2].dma_func(dma_isr_redir[2].dma_param, flags);
        MOVS     R1,R4
        LDR.W    R0,??DataTable18_2
        LDR      R0,[R0, #+20]
        LDR.W    R2,??DataTable18_2
        LDR      R2,[R2, #+16]
          CFI FunCall
        BLX      R2
//  173 
//  174   CH_IRQ_EPILOGUE();
??Vector74_0:
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//  175 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock2
//  176 
//  177 /**
//  178  * @brief   DMA1 stream 3 shared interrupt handler.
//  179  *
//  180  * @isr
//  181  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function Vector78
        THUMB
//  182 CH_IRQ_HANDLER(DMA1_Stream3_IRQHandler) {
Vector78:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//  183   uint32_t flags;
//  184 
//  185   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//  186 
//  187   flags = (DMA1->LISR >> 22) & STM32_DMA_ISR_MASK;
        LDR.W    R0,??DataTable18  ;; 0x40026000
        LDR      R0,[R0, #+0]
        LSRS     R0,R0,#+22
        ANDS     R0,R0,#0x3D
        MOVS     R4,R0
//  188   DMA1->LIFCR = flags << 22;
        LSLS     R0,R4,#+22
        LDR.W    R1,??DataTable18_1  ;; 0x40026008
        STR      R0,[R1, #+0]
//  189   if (dma_isr_redir[3].dma_func)
        LDR.W    R0,??DataTable18_2
        LDR      R0,[R0, #+24]
        CMP      R0,#+0
        BEQ.N    ??Vector78_0
//  190     dma_isr_redir[3].dma_func(dma_isr_redir[3].dma_param, flags);
        MOVS     R1,R4
        LDR.W    R0,??DataTable18_2
        LDR      R0,[R0, #+28]
        LDR.W    R2,??DataTable18_2
        LDR      R2,[R2, #+24]
          CFI FunCall
        BLX      R2
//  191 
//  192   CH_IRQ_EPILOGUE();
??Vector78_0:
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//  193 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock3
//  194 
//  195 /**
//  196  * @brief   DMA1 stream 4 shared interrupt handler.
//  197  *
//  198  * @isr
//  199  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function Vector7C
        THUMB
//  200 CH_IRQ_HANDLER(DMA1_Stream4_IRQHandler) {
Vector7C:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//  201   uint32_t flags;
//  202 
//  203   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//  204 
//  205   flags = (DMA1->HISR >> 0) & STM32_DMA_ISR_MASK;
        LDR.W    R0,??DataTable18_3  ;; 0x40026004
        LDR      R0,[R0, #+0]
        ANDS     R0,R0,#0x3D
        MOVS     R4,R0
//  206   DMA1->HIFCR = flags << 0;
        LDR.W    R0,??DataTable18_4  ;; 0x4002600c
        STR      R4,[R0, #+0]
//  207   if (dma_isr_redir[4].dma_func)
        LDR.W    R0,??DataTable18_2
        LDR      R0,[R0, #+32]
        CMP      R0,#+0
        BEQ.N    ??Vector7C_0
//  208     dma_isr_redir[4].dma_func(dma_isr_redir[4].dma_param, flags);
        MOVS     R1,R4
        LDR.W    R0,??DataTable18_2
        LDR      R0,[R0, #+36]
        LDR.W    R2,??DataTable18_2
        LDR      R2,[R2, #+32]
          CFI FunCall
        BLX      R2
//  209 
//  210   CH_IRQ_EPILOGUE();
??Vector7C_0:
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//  211 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock4
//  212 
//  213 /**
//  214  * @brief   DMA1 stream 5 shared interrupt handler.
//  215  *
//  216  * @isr
//  217  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function Vector80
        THUMB
//  218 CH_IRQ_HANDLER(DMA1_Stream5_IRQHandler) {
Vector80:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//  219   uint32_t flags;
//  220 
//  221   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//  222 
//  223   flags = (DMA1->HISR >> 6) & STM32_DMA_ISR_MASK;
        LDR.W    R0,??DataTable18_3  ;; 0x40026004
        LDR      R0,[R0, #+0]
        LSRS     R0,R0,#+6
        ANDS     R0,R0,#0x3D
        MOVS     R4,R0
//  224   DMA1->HIFCR = flags << 6;
        LSLS     R0,R4,#+6
        LDR.W    R1,??DataTable18_4  ;; 0x4002600c
        STR      R0,[R1, #+0]
//  225   if (dma_isr_redir[5].dma_func)
        LDR.W    R0,??DataTable18_2
        LDR      R0,[R0, #+40]
        CMP      R0,#+0
        BEQ.N    ??Vector80_0
//  226     dma_isr_redir[5].dma_func(dma_isr_redir[5].dma_param, flags);
        MOVS     R1,R4
        LDR.W    R0,??DataTable18_2
        LDR      R0,[R0, #+44]
        LDR.W    R2,??DataTable18_2
        LDR      R2,[R2, #+40]
          CFI FunCall
        BLX      R2
//  227 
//  228   CH_IRQ_EPILOGUE();
??Vector80_0:
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//  229 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock5
//  230 
//  231 /**
//  232  * @brief   DMA1 stream 6 shared interrupt handler.
//  233  *
//  234  * @isr
//  235  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function Vector84
        THUMB
//  236 CH_IRQ_HANDLER(DMA1_Stream6_IRQHandler) {
Vector84:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//  237   uint32_t flags;
//  238 
//  239   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//  240 
//  241   flags = (DMA1->HISR >> 16) & STM32_DMA_ISR_MASK;
        LDR.W    R0,??DataTable18_3  ;; 0x40026004
        LDR      R0,[R0, #+0]
        LSRS     R0,R0,#+16
        ANDS     R0,R0,#0x3D
        MOVS     R4,R0
//  242   DMA1->HIFCR = flags << 16;
        LSLS     R0,R4,#+16
        LDR.W    R1,??DataTable18_4  ;; 0x4002600c
        STR      R0,[R1, #+0]
//  243   if (dma_isr_redir[6].dma_func)
        LDR.W    R0,??DataTable18_2
        LDR      R0,[R0, #+48]
        CMP      R0,#+0
        BEQ.N    ??Vector84_0
//  244     dma_isr_redir[6].dma_func(dma_isr_redir[6].dma_param, flags);
        MOVS     R1,R4
        LDR.W    R0,??DataTable18_2
        LDR      R0,[R0, #+52]
        LDR.W    R2,??DataTable18_2
        LDR      R2,[R2, #+48]
          CFI FunCall
        BLX      R2
//  245 
//  246   CH_IRQ_EPILOGUE();
??Vector84_0:
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//  247 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock6
//  248 
//  249 /**
//  250  * @brief   DMA1 stream 7 shared interrupt handler.
//  251  *
//  252  * @isr
//  253  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function VectorFC
        THUMB
//  254 CH_IRQ_HANDLER(DMA1_Stream7_IRQHandler) {
VectorFC:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//  255   uint32_t flags;
//  256 
//  257   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//  258 
//  259   flags = (DMA1->HISR >> 22) & STM32_DMA_ISR_MASK;
        LDR.W    R0,??DataTable18_3  ;; 0x40026004
        LDR      R0,[R0, #+0]
        LSRS     R0,R0,#+22
        ANDS     R0,R0,#0x3D
        MOVS     R4,R0
//  260   DMA1->HIFCR = flags << 22;
        LSLS     R0,R4,#+22
        LDR.W    R1,??DataTable18_4  ;; 0x4002600c
        STR      R0,[R1, #+0]
//  261   if (dma_isr_redir[7].dma_func)
        LDR.W    R0,??DataTable18_2
        LDR      R0,[R0, #+56]
        CMP      R0,#+0
        BEQ.N    ??VectorFC_0
//  262     dma_isr_redir[7].dma_func(dma_isr_redir[7].dma_param, flags);
        MOVS     R1,R4
        LDR.W    R0,??DataTable18_2
        LDR      R0,[R0, #+60]
        LDR.W    R2,??DataTable18_2
        LDR      R2,[R2, #+56]
          CFI FunCall
        BLX      R2
//  263 
//  264   CH_IRQ_EPILOGUE();
??VectorFC_0:
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//  265 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock7
//  266 
//  267 /**
//  268  * @brief   DMA2 stream 0 shared interrupt handler.
//  269  *
//  270  * @isr
//  271  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function Vector120
        THUMB
//  272 CH_IRQ_HANDLER(DMA2_Stream0_IRQHandler) {
Vector120:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//  273   uint32_t flags;
//  274 
//  275   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//  276 
//  277   flags = (DMA2->LISR >> 0) & STM32_DMA_ISR_MASK;
        LDR.W    R0,??DataTable18_5  ;; 0x40026400
        LDR      R0,[R0, #+0]
        ANDS     R0,R0,#0x3D
        MOVS     R4,R0
//  278   DMA2->LIFCR = flags << 0;
        LDR.N    R0,??DataTable18_6  ;; 0x40026408
        STR      R4,[R0, #+0]
//  279   if (dma_isr_redir[8].dma_func)
        LDR.N    R0,??DataTable18_2
        LDR      R0,[R0, #+64]
        CMP      R0,#+0
        BEQ.N    ??Vector120_0
//  280     dma_isr_redir[8].dma_func(dma_isr_redir[8].dma_param, flags);
        MOVS     R1,R4
        LDR.N    R0,??DataTable18_2
        LDR      R0,[R0, #+68]
        LDR.N    R2,??DataTable18_2
        LDR      R2,[R2, #+64]
          CFI FunCall
        BLX      R2
//  281 
//  282   CH_IRQ_EPILOGUE();
??Vector120_0:
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//  283 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock8
//  284 
//  285 /**
//  286  * @brief   DMA2 stream 1 shared interrupt handler.
//  287  *
//  288  * @isr
//  289  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function Vector124
        THUMB
//  290 CH_IRQ_HANDLER(DMA2_Stream1_IRQHandler) {
Vector124:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//  291   uint32_t flags;
//  292 
//  293   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//  294 
//  295   flags = (DMA2->LISR >> 6) & STM32_DMA_ISR_MASK;
        LDR.N    R0,??DataTable18_5  ;; 0x40026400
        LDR      R0,[R0, #+0]
        LSRS     R0,R0,#+6
        ANDS     R0,R0,#0x3D
        MOVS     R4,R0
//  296   DMA2->LIFCR = flags << 6;
        LSLS     R0,R4,#+6
        LDR.N    R1,??DataTable18_6  ;; 0x40026408
        STR      R0,[R1, #+0]
//  297   if (dma_isr_redir[9].dma_func)
        LDR.N    R0,??DataTable18_2
        LDR      R0,[R0, #+72]
        CMP      R0,#+0
        BEQ.N    ??Vector124_0
//  298     dma_isr_redir[9].dma_func(dma_isr_redir[9].dma_param, flags);
        MOVS     R1,R4
        LDR.N    R0,??DataTable18_2
        LDR      R0,[R0, #+76]
        LDR.N    R2,??DataTable18_2
        LDR      R2,[R2, #+72]
          CFI FunCall
        BLX      R2
//  299 
//  300   CH_IRQ_EPILOGUE();
??Vector124_0:
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//  301 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock9
//  302 
//  303 /**
//  304  * @brief   DMA2 stream 2 shared interrupt handler.
//  305  *
//  306  * @isr
//  307  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function Vector128
        THUMB
//  308 CH_IRQ_HANDLER(DMA2_Stream2_IRQHandler) {
Vector128:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//  309   uint32_t flags;
//  310 
//  311   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//  312 
//  313   flags = (DMA2->LISR >> 16) & STM32_DMA_ISR_MASK;
        LDR.N    R0,??DataTable18_5  ;; 0x40026400
        LDR      R0,[R0, #+0]
        LSRS     R0,R0,#+16
        ANDS     R0,R0,#0x3D
        MOVS     R4,R0
//  314   DMA2->LIFCR = flags << 16;
        LSLS     R0,R4,#+16
        LDR.N    R1,??DataTable18_6  ;; 0x40026408
        STR      R0,[R1, #+0]
//  315   if (dma_isr_redir[10].dma_func)
        LDR.N    R0,??DataTable18_2
        LDR      R0,[R0, #+80]
        CMP      R0,#+0
        BEQ.N    ??Vector128_0
//  316     dma_isr_redir[10].dma_func(dma_isr_redir[10].dma_param, flags);
        MOVS     R1,R4
        LDR.N    R0,??DataTable18_2
        LDR      R0,[R0, #+84]
        LDR.N    R2,??DataTable18_2
        LDR      R2,[R2, #+80]
          CFI FunCall
        BLX      R2
//  317 
//  318   CH_IRQ_EPILOGUE();
??Vector128_0:
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//  319 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock10
//  320 
//  321 /**
//  322  * @brief   DMA2 stream 3 shared interrupt handler.
//  323  *
//  324  * @isr
//  325  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function Vector12C
        THUMB
//  326 CH_IRQ_HANDLER(DMA2_Stream3_IRQHandler) {
Vector12C:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//  327   uint32_t flags;
//  328 
//  329   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//  330 
//  331   flags = (DMA2->LISR >> 22) & STM32_DMA_ISR_MASK;
        LDR.N    R0,??DataTable18_5  ;; 0x40026400
        LDR      R0,[R0, #+0]
        LSRS     R0,R0,#+22
        ANDS     R0,R0,#0x3D
        MOVS     R4,R0
//  332   DMA2->LIFCR = flags << 22;
        LSLS     R0,R4,#+22
        LDR.N    R1,??DataTable18_6  ;; 0x40026408
        STR      R0,[R1, #+0]
//  333   if (dma_isr_redir[11].dma_func)
        LDR.N    R0,??DataTable18_2
        LDR      R0,[R0, #+88]
        CMP      R0,#+0
        BEQ.N    ??Vector12C_0
//  334     dma_isr_redir[11].dma_func(dma_isr_redir[11].dma_param, flags);
        MOVS     R1,R4
        LDR.N    R0,??DataTable18_2
        LDR      R0,[R0, #+92]
        LDR.N    R2,??DataTable18_2
        LDR      R2,[R2, #+88]
          CFI FunCall
        BLX      R2
//  335 
//  336   CH_IRQ_EPILOGUE();
??Vector12C_0:
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//  337 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock11
//  338 
//  339 /**
//  340  * @brief   DMA2 stream 4 shared interrupt handler.
//  341  *
//  342  * @isr
//  343  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function Vector130
        THUMB
//  344 CH_IRQ_HANDLER(DMA2_Stream4_IRQHandler) {
Vector130:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//  345   uint32_t flags;
//  346 
//  347   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//  348 
//  349   flags = (DMA2->HISR >> 0) & STM32_DMA_ISR_MASK;
        LDR.N    R0,??DataTable18_7  ;; 0x40026404
        LDR      R0,[R0, #+0]
        ANDS     R0,R0,#0x3D
        MOVS     R4,R0
//  350   DMA2->HIFCR = flags << 0;
        LDR.N    R0,??DataTable18_8  ;; 0x4002640c
        STR      R4,[R0, #+0]
//  351   if (dma_isr_redir[12].dma_func)
        LDR.N    R0,??DataTable18_2
        LDR      R0,[R0, #+96]
        CMP      R0,#+0
        BEQ.N    ??Vector130_0
//  352     dma_isr_redir[12].dma_func(dma_isr_redir[12].dma_param, flags);
        MOVS     R1,R4
        LDR.N    R0,??DataTable18_2
        LDR      R0,[R0, #+100]
        LDR.N    R2,??DataTable18_2
        LDR      R2,[R2, #+96]
          CFI FunCall
        BLX      R2
//  353 
//  354   CH_IRQ_EPILOGUE();
??Vector130_0:
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//  355 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock12
//  356 
//  357 /**
//  358  * @brief   DMA2 stream 5 shared interrupt handler.
//  359  *
//  360  * @isr
//  361  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock13 Using cfiCommon0
          CFI Function Vector150
        THUMB
//  362 CH_IRQ_HANDLER(DMA2_Stream5_IRQHandler) {
Vector150:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//  363   uint32_t flags;
//  364 
//  365   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//  366 
//  367   flags = (DMA2->HISR >> 6) & STM32_DMA_ISR_MASK;
        LDR.N    R0,??DataTable18_7  ;; 0x40026404
        LDR      R0,[R0, #+0]
        LSRS     R0,R0,#+6
        ANDS     R0,R0,#0x3D
        MOVS     R4,R0
//  368   DMA2->HIFCR = flags << 6;
        LSLS     R0,R4,#+6
        LDR.N    R1,??DataTable18_8  ;; 0x4002640c
        STR      R0,[R1, #+0]
//  369   if (dma_isr_redir[13].dma_func)
        LDR.N    R0,??DataTable18_2
        LDR      R0,[R0, #+104]
        CMP      R0,#+0
        BEQ.N    ??Vector150_0
//  370     dma_isr_redir[13].dma_func(dma_isr_redir[13].dma_param, flags);
        MOVS     R1,R4
        LDR.N    R0,??DataTable18_2
        LDR      R0,[R0, #+108]
        LDR.N    R2,??DataTable18_2
        LDR      R2,[R2, #+104]
          CFI FunCall
        BLX      R2
//  371 
//  372   CH_IRQ_EPILOGUE();
??Vector150_0:
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//  373 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock13
//  374 
//  375 /**
//  376  * @brief   DMA2 stream 6 shared interrupt handler.
//  377  *
//  378  * @isr
//  379  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock14 Using cfiCommon0
          CFI Function Vector154
        THUMB
//  380 CH_IRQ_HANDLER(DMA2_Stream6_IRQHandler) {
Vector154:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//  381   uint32_t flags;
//  382 
//  383   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//  384 
//  385   flags = (DMA2->HISR >> 16) & STM32_DMA_ISR_MASK;
        LDR.N    R0,??DataTable18_7  ;; 0x40026404
        LDR      R0,[R0, #+0]
        LSRS     R0,R0,#+16
        ANDS     R0,R0,#0x3D
        MOVS     R4,R0
//  386   DMA2->HIFCR = flags << 16;
        LSLS     R0,R4,#+16
        LDR.N    R1,??DataTable18_8  ;; 0x4002640c
        STR      R0,[R1, #+0]
//  387   if (dma_isr_redir[14].dma_func)
        LDR.N    R0,??DataTable18_2
        LDR      R0,[R0, #+112]
        CMP      R0,#+0
        BEQ.N    ??Vector154_0
//  388     dma_isr_redir[14].dma_func(dma_isr_redir[14].dma_param, flags);
        MOVS     R1,R4
        LDR.N    R0,??DataTable18_2
        LDR      R0,[R0, #+116]
        LDR.N    R2,??DataTable18_2
        LDR      R2,[R2, #+112]
          CFI FunCall
        BLX      R2
//  389 
//  390   CH_IRQ_EPILOGUE();
??Vector154_0:
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//  391 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock14
//  392 
//  393 /**
//  394  * @brief   DMA2 stream 7 shared interrupt handler.
//  395  *
//  396  * @isr
//  397  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock15 Using cfiCommon0
          CFI Function Vector158
        THUMB
//  398 CH_IRQ_HANDLER(DMA2_Stream7_IRQHandler) {
Vector158:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//  399   uint32_t flags;
//  400 
//  401   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//  402 
//  403   flags = (DMA2->HISR >> 22) & STM32_DMA_ISR_MASK;
        LDR.N    R0,??DataTable18_7  ;; 0x40026404
        LDR      R0,[R0, #+0]
        LSRS     R0,R0,#+22
        ANDS     R0,R0,#0x3D
        MOVS     R4,R0
//  404   DMA2->HIFCR = flags << 22;
        LSLS     R0,R4,#+22
        LDR.N    R1,??DataTable18_8  ;; 0x4002640c
        STR      R0,[R1, #+0]
//  405   if (dma_isr_redir[15].dma_func)
        LDR.N    R0,??DataTable18_2
        LDR      R0,[R0, #+120]
        CMP      R0,#+0
        BEQ.N    ??Vector158_0
//  406     dma_isr_redir[15].dma_func(dma_isr_redir[15].dma_param, flags);
        MOVS     R1,R4
        LDR.N    R0,??DataTable18_2
        LDR      R0,[R0, #+124]
        LDR.N    R2,??DataTable18_2
        LDR      R2,[R2, #+120]
          CFI FunCall
        BLX      R2
//  407 
//  408   CH_IRQ_EPILOGUE();
??Vector158_0:
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//  409 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock15
//  410 
//  411 /*===========================================================================*/
//  412 /* Driver exported functions.                                                */
//  413 /*===========================================================================*/
//  414 
//  415 /**
//  416  * @brief   STM32 DMA helper initialization.
//  417  *
//  418  * @init
//  419  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock16 Using cfiCommon0
          CFI Function dmaInit
          CFI NoCalls
        THUMB
//  420 void dmaInit(void) {
//  421   int i;
//  422 
//  423   dma_streams_mask = 0;
dmaInit:
        LDR.N    R1,??DataTable18_9
        MOVS     R2,#+0
        STR      R2,[R1, #+0]
//  424   for (i = 0; i < STM32_DMA_STREAMS; i++) {
        MOVS     R1,#+0
        MOVS     R0,R1
??dmaInit_0:
        CMP      R0,#+16
        BGE.N    ??dmaInit_1
//  425     _stm32_dma_streams[i].stream->CR = 0;
        MOVS     R1,#+12
        LDR.N    R2,??DataTable18_10
        MLA      R1,R1,R0,R2
        LDR      R1,[R1, #+0]
        MOVS     R2,#+0
        STR      R2,[R1, #+0]
//  426     dma_isr_redir[i].dma_func = NULL;
        LDR.N    R1,??DataTable18_2
        MOVS     R2,#+0
        STR      R2,[R1, R0, LSL #+3]
//  427   }
        ADDS     R0,R0,#+1
        B.N      ??dmaInit_0
//  428   DMA1->LIFCR = 0xFFFFFFFF;
??dmaInit_1:
        LDR.N    R1,??DataTable18_1  ;; 0x40026008
        MOVS     R2,#-1
        STR      R2,[R1, #+0]
//  429   DMA1->HIFCR = 0xFFFFFFFF;
        LDR.N    R1,??DataTable18_4  ;; 0x4002600c
        MOVS     R2,#-1
        STR      R2,[R1, #+0]
//  430   DMA2->LIFCR = 0xFFFFFFFF;
        LDR.N    R1,??DataTable18_6  ;; 0x40026408
        MOVS     R2,#-1
        STR      R2,[R1, #+0]
//  431   DMA2->HIFCR = 0xFFFFFFFF;
        LDR.N    R1,??DataTable18_8  ;; 0x4002640c
        MOVS     R2,#-1
        STR      R2,[R1, #+0]
//  432 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock16
//  433 
//  434 /**
//  435  * @brief   Allocates a DMA stream.
//  436  * @details The stream is allocated and, if required, the DMA clock enabled.
//  437  *          The function also enables the IRQ vector associated to the stream
//  438  *          and initializes its priority.
//  439  * @pre     The stream must not be already in use or an error is returned.
//  440  * @post    The stream is allocated and the default ISR handler redirected
//  441  *          to the specified function.
//  442  * @post    The stream ISR vector is enabled and its priority configured.
//  443  * @post    The stream must be freed using @p dmaStreamRelease() before it can
//  444  *          be reused with another peripheral.
//  445  * @post    The stream is in its post-reset state.
//  446  * @note    This function can be invoked in both ISR or thread context.
//  447  *
//  448  * @param[in] dmastp    pointer to a stm32_dma_stream_t structure
//  449  * @param[in] priority  IRQ priority mask for the DMA stream
//  450  * @param[in] func      handling function pointer, can be @p NULL
//  451  * @param[in] param     a parameter to be passed to the handling function
//  452  * @return              The operation status.
//  453  * @retval FALSE        no error, stream taken.
//  454  * @retval TRUE         error, stream already taken.
//  455  *
//  456  * @special
//  457  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock17 Using cfiCommon0
          CFI Function dmaStreamAllocate
        THUMB
//  458 bool_t dmaStreamAllocate(const stm32_dma_stream_t *dmastp,
//  459                          uint32_t priority,
//  460                          stm32_dmaisr_t func,
//  461                          void *param) {
dmaStreamAllocate:
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
        MOVS     R7,R3
//  462 
//  463   chDbgCheck(dmastp != NULL, "dmaStreamAllocate");
        CMP      R4,#+0
        BNE.N    ??dmaStreamAllocate_0
        MOVW     R2,#+463
        LDR.N    R1,??DataTable18_11
        LDR.N    R0,??DataTable18_12
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  464 
//  465   /* Checks if the stream is already taken.*/
//  466   if ((dma_streams_mask & (1 << dmastp->selfindex)) != 0)
??dmaStreamAllocate_0:
        LDR.N    R0,??DataTable18_9
        LDR      R0,[R0, #+0]
        LDRB     R1,[R4, #+9]
        LSRS     R0,R0,R1
        LSLS     R0,R0,#+31
        BPL.N    ??dmaStreamAllocate_1
//  467     return TRUE;
        MOVS     R0,#+1
        B.N      ??dmaStreamAllocate_2
//  468 
//  469   /* Marks the stream as allocated.*/
//  470   dma_isr_redir[dmastp->selfindex].dma_func  = func;
??dmaStreamAllocate_1:
        LDRB     R0,[R4, #+9]
        LDR.N    R1,??DataTable18_2
        STR      R6,[R1, R0, LSL #+3]
//  471   dma_isr_redir[dmastp->selfindex].dma_param = param;
        LDRB     R0,[R4, #+9]
        LDR.N    R1,??DataTable18_2
        ADDS     R0,R1,R0, LSL #+3
        STR      R7,[R0, #+4]
//  472   dma_streams_mask |= (1 << dmastp->selfindex);
        LDR.N    R0,??DataTable18_9
        LDR      R0,[R0, #+0]
        MOVS     R1,#+1
        LDRSB    R2,[R4, #+9]
        LSLS     R1,R1,R2
        ORRS     R0,R1,R0
        LDR.N    R1,??DataTable18_9
        STR      R0,[R1, #+0]
//  473 
//  474   /* Enabling DMA clocks required by the current streams set.*/
//  475   if ((dma_streams_mask & STM32_DMA1_STREAMS_MASK) != 0)
        LDR.N    R0,??DataTable18_9
        LDRB     R0,[R0, #+0]
        LSLS     R0,R0,#+24
        BEQ.N    ??dmaStreamAllocate_3
//  476     rccEnableDMA1(FALSE);
        LDR.N    R0,??DataTable18_13  ;; 0x40023830
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x200000
        LDR.N    R1,??DataTable18_13  ;; 0x40023830
        STR      R0,[R1, #+0]
//  477   if ((dma_streams_mask & STM32_DMA2_STREAMS_MASK) != 0)
??dmaStreamAllocate_3:
        LDR.N    R0,??DataTable18_9
        LDR      R0,[R0, #+0]
        TST      R0,#0xFF00
        BEQ.N    ??dmaStreamAllocate_4
//  478     rccEnableDMA2(FALSE);
        LDR.N    R0,??DataTable18_13  ;; 0x40023830
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x400000
        LDR.N    R1,??DataTable18_13  ;; 0x40023830
        STR      R0,[R1, #+0]
//  479 
//  480   /* Putting the stream in a safe state.*/
//  481   dmaStreamDisable(dmastp);
??dmaStreamAllocate_4:
        LDR      R0,[R4, #+0]
        LDR      R0,[R0, #+0]
        LSRS     R0,R0,#+5
        LSLS     R0,R0,#+5
        LDR      R1,[R4, #+0]
        STR      R0,[R1, #+0]
??dmaStreamAllocate_5:
        LDR      R0,[R4, #+0]
        LDR      R0,[R0, #+0]
        LSLS     R0,R0,#+31
        BMI.N    ??dmaStreamAllocate_5
        MOVS     R0,#+61
        LDRSB    R1,[R4, #+8]
        LSLS     R0,R0,R1
        LDR      R1,[R4, #+4]
        STR      R0,[R1, #+0]
//  482   dmastp->stream->CR = STM32_DMA_CR_RESET_VALUE;
        LDR      R0,[R4, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//  483   dmastp->stream->FCR = STM32_DMA_FCR_RESET_VALUE;
        LDR      R0,[R4, #+0]
        MOVS     R1,#+33
        STR      R1,[R0, #+20]
//  484 
//  485   /* Enables the associated IRQ vector if a callback is defined.*/
//  486   if (func != NULL)
        MOVS     R0,R6
        CMP      R0,#+0
        BEQ.N    ??dmaStreamAllocate_6
//  487     nvicEnableVector(dmastp->vector, CORTEX_PRIORITY_MASK(priority));
        LSLS     R1,R5,#+4
        LDRB     R0,[R4, #+10]
          CFI FunCall nvicEnableVector
        BL       nvicEnableVector
//  488 
//  489   return FALSE;
??dmaStreamAllocate_6:
        MOVS     R0,#+0
??dmaStreamAllocate_2:
        POP      {R1,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock17
//  490 }
//  491 
//  492 /**
//  493  * @brief   Releases a DMA stream.
//  494  * @details The stream is freed and, if required, the DMA clock disabled.
//  495  *          Trying to release a unallocated stream is an illegal operation
//  496  *          and is trapped if assertions are enabled.
//  497  * @pre     The stream must have been allocated using @p dmaStreamAllocate().
//  498  * @post    The stream is again available.
//  499  * @note    This function can be invoked in both ISR or thread context.
//  500  *
//  501  * @param[in] dmastp    pointer to a stm32_dma_stream_t structure
//  502  *
//  503  * @special
//  504  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock18 Using cfiCommon0
          CFI Function dmaStreamRelease
        THUMB
//  505 void dmaStreamRelease(const stm32_dma_stream_t *dmastp) {
dmaStreamRelease:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  506 
//  507   chDbgCheck(dmastp != NULL, "dmaStreamRelease");
        CMP      R4,#+0
        BNE.N    ??dmaStreamRelease_0
        MOVW     R2,#+507
        LDR.N    R1,??DataTable18_11
        LDR.N    R0,??DataTable18_14
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  508 
//  509   /* Check if the streams is not taken.*/
//  510   chDbgAssert((dma_streams_mask & (1 << dmastp->selfindex)) != 0,
//  511               "dmaStreamRelease(), #1", "not allocated");
??dmaStreamRelease_0:
        LDR.N    R0,??DataTable18_9
        LDR      R0,[R0, #+0]
        LDRB     R1,[R4, #+9]
        LSRS     R0,R0,R1
        LSLS     R0,R0,#+31
        BMI.N    ??dmaStreamRelease_1
        LDR.N    R0,??DataTable18_15
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  512 
//  513   /* Disables the associated IRQ vector.*/
//  514   nvicDisableVector(dmastp->vector);
??dmaStreamRelease_1:
        LDRB     R0,[R4, #+10]
          CFI FunCall nvicDisableVector
        BL       nvicDisableVector
//  515 
//  516   /* Marks the stream as not allocated.*/
//  517   dma_streams_mask &= ~(1 << dmastp->selfindex);
        LDR.N    R0,??DataTable18_9
        LDR      R0,[R0, #+0]
        MOVS     R1,#+1
        LDRSB    R2,[R4, #+9]
        LSLS     R1,R1,R2
        BICS     R0,R0,R1
        LDR.N    R1,??DataTable18_9
        STR      R0,[R1, #+0]
//  518 
//  519   /* Shutting down clocks that are no more required, if any.*/
//  520   if ((dma_streams_mask & STM32_DMA1_STREAMS_MASK) == 0)
        LDR.N    R0,??DataTable18_9
        LDRB     R0,[R0, #+0]
        LSLS     R0,R0,#+24
        BNE.N    ??dmaStreamRelease_2
//  521     rccDisableDMA1(FALSE);
        LDR.N    R0,??DataTable18_13  ;; 0x40023830
        LDR      R0,[R0, #+0]
        BICS     R0,R0,#0x200000
        LDR.N    R1,??DataTable18_13  ;; 0x40023830
        STR      R0,[R1, #+0]
//  522   if ((dma_streams_mask & STM32_DMA2_STREAMS_MASK) == 0)
??dmaStreamRelease_2:
        LDR.N    R0,??DataTable18_9
        LDR      R0,[R0, #+0]
        TST      R0,#0xFF00
        BNE.N    ??dmaStreamRelease_3
//  523     rccDisableDMA2(FALSE);
        LDR.N    R0,??DataTable18_13  ;; 0x40023830
        LDR      R0,[R0, #+0]
        BICS     R0,R0,#0x400000
        LDR.N    R1,??DataTable18_13  ;; 0x40023830
        STR      R0,[R1, #+0]
//  524 }
??dmaStreamRelease_3:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock18

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable18:
        DC32     0x40026000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable18_1:
        DC32     0x40026008

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable18_2:
        DC32     dma_isr_redir

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable18_3:
        DC32     0x40026004

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable18_4:
        DC32     0x4002600c

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable18_5:
        DC32     0x40026400

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable18_6:
        DC32     0x40026408

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable18_7:
        DC32     0x40026404

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable18_8:
        DC32     0x4002640c

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable18_9:
        DC32     dma_streams_mask

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable18_10:
        DC32     _stm32_dma_streams

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable18_11:
        DC32     `?<Constant "F:\\\\stuff\\\\rusefi_sourc...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable18_12:
        DC32     `?<Constant "\\"dmaStreamAllocate\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable18_13:
        DC32     0x40023830

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable18_14:
        DC32     `?<Constant "\\"dmaStreamRelease\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable18_15:
        DC32     `?<Constant "dmaStreamRelease(), #1">`

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        END
//  525 
//  526 #endif /* STM32_DMA_REQUIRED */
//  527 
//  528 /** @} */
// 
//   132 bytes in section .bss
//   348 bytes in section .rodata
// 1 356 bytes in section .text
// 
// 1 356 bytes of CODE  memory
//   348 bytes of CONST memory
//   132 bytes of DATA  memory
//
//Errors: none
//Warnings: none
