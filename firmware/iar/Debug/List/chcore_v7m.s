///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:16 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\port /
//                       s\IAR\ARMCMx\chcore_v7m.c                            /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\port /
//                       s\IAR\ARMCMx\chcore_v7m.c -lCN                       /
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
//                       chcore_v7m.s                                         /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME chcore_v7m

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _port_exit_from_isr
        EXTERN _port_switch_from_isr
        EXTERN chSysTimerHandlerI
        EXTERN dbg_check_enter_isr
        EXTERN dbg_check_leave_isr
        EXTERN dbg_check_lock_from_isr
        EXTERN dbg_check_unlock_from_isr
        EXTERN nvicSetSystemHandlerPriority
        EXTERN rlist

        PUBLIC SVCallVector
        PUBLIC SysTickVector
        PUBLIC _port_init
        PUBLIC _port_irq_epilogue
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\ports\IAR\ARMCMx\chcore_v7m.c
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
//   29  * @file    IAR/ARMCMx/chcore_v7m.c
//   30  * @brief   ARMv7-M architecture port code.
//   31  *
//   32  * @addtogroup IAR_ARMCMx_V7M_CORE
//   33  * @{
//   34  */
//   35 
//   36 #include "ch.h"
//   37 
//   38 /*===========================================================================*/
//   39 /* Port interrupt handlers.                                                  */
//   40 /*===========================================================================*/
//   41 
//   42 /**
//   43  * @brief   System Timer vector.
//   44  * @details This interrupt is used as system tick.
//   45  * @note    The timer must be initialized in the startup code.
//   46  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function SysTickVector
        THUMB
//   47 CH_IRQ_HANDLER(SysTickVector) {
SysTickVector:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   48 
//   49   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//   50 
//   51   chSysLockFromIsr();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock_from_isr
        BL       dbg_check_lock_from_isr
//   52   chSysTimerHandlerI();
          CFI FunCall chSysTimerHandlerI
        BL       chSysTimerHandlerI
//   53   chSysUnlockFromIsr();
          CFI FunCall dbg_check_unlock_from_isr
        BL       dbg_check_unlock_from_isr
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//   54 
//   55   CH_IRQ_EPILOGUE();
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//   56 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock0
//   57 
//   58 #if !CORTEX_SIMPLIFIED_PRIORITY || defined(__DOXYGEN__)
//   59 /**
//   60  * @brief   SVC vector.
//   61  * @details The SVC vector is used for exception mode re-entering after a
//   62  *          context switch.
//   63  * @note    The PendSV vector is only used in advanced kernel mode.
//   64  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function SVCallVector
          CFI NoCalls
        THUMB
//   65 void SVCallVector(void) {
//   66   struct extctx *ctxp;
//   67 
//   68 #if CORTEX_USE_FPU
//   69   /* Enforcing unstacking of the FP part of the context.*/
//   70   SCB_FPCCR &= ~FPCCR_LSPACT;
SVCallVector:
        LDR.N    R1,??DataTable2  ;; 0xe000ef34
        LDR      R1,[R1, #+0]
        LSRS     R1,R1,#+1
        LSLS     R1,R1,#+1
        LDR.N    R2,??DataTable2  ;; 0xe000ef34
        STR      R1,[R2, #+0]
//   71 #endif
//   72 
//   73   /* Current PSP value.*/
//   74   ctxp = (struct extctx *)__get_PSP();
        MRS      R1,PSP
        MOVS     R0,R1
//   75 
//   76   /* Discarding the current exception context and positioning the stack to
//   77      point to the real one.*/
//   78   ctxp++;
        ADDS     R0,R0,#+104
//   79 
//   80   /* Restoring real position of the original stack frame.*/
//   81   __set_PSP((unsigned long)ctxp);
        MSR      PSP,R0
//   82   port_unlock_from_isr();
        MOVS     R1,#+0
        MSR      BASEPRI,R1
//   83 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock1
//   84 #endif /* !CORTEX_SIMPLIFIED_PRIORITY */
//   85 
//   86 #if CORTEX_SIMPLIFIED_PRIORITY || defined(__DOXYGEN__)
//   87 /**
//   88  * @brief   PendSV vector.
//   89  * @details The PendSV vector is used for exception mode re-entering after a
//   90  *          context switch.
//   91  * @note    The PendSV vector is only used in compact kernel mode.
//   92  */
//   93 void PendSVVector(void) {
//   94   struct extctx *ctxp;
//   95 
//   96 #if CORTEX_USE_FPU
//   97   /* Enforcing unstacking of the FP part of the context.*/
//   98   SCB_FPCCR &= ~FPCCR_LSPACT;
//   99 #endif
//  100 
//  101   /* Current PSP value.*/
//  102   ctxp = (struct extctx *)__get_PSP();
//  103 
//  104   /* Discarding the current exception context and positioning the stack to
//  105      point to the real one.*/
//  106   ctxp++;
//  107 
//  108   /* Restoring real position of the original stack frame.*/
//  109   __set_PSP((unsigned long)ctxp);
//  110 }
//  111 #endif /* CORTEX_SIMPLIFIED_PRIORITY */
//  112 
//  113 /*===========================================================================*/
//  114 /* Port exported functions.                                                  */
//  115 /*===========================================================================*/
//  116 
//  117 /**
//  118  * @brief   Port-related initialization code.
//  119  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _port_init
        THUMB
//  120 void _port_init(void) {
_port_init:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  121 
//  122   /* Initialization of the vector table and priority related settings.*/
//  123   SCB_VTOR = CORTEX_VTOR_INIT;
        LDR.N    R0,??DataTable2_1  ;; 0xe000ed08
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//  124   SCB_AIRCR = AIRCR_VECTKEY | AIRCR_PRIGROUP(CORTEX_PRIGROUP_INIT);
        LDR.N    R0,??DataTable2_2  ;; 0xe000ed0c
        LDR.N    R1,??DataTable2_3  ;; 0x5fa0300
        STR      R1,[R0, #+0]
//  125 
//  126 #if CORTEX_USE_FPU
//  127   {
//  128     /* Initializing the FPU context save in lazy mode.*/
//  129     SCB_FPCCR = FPCCR_ASPEN | FPCCR_LSPEN;
        LDR.N    R0,??DataTable2  ;; 0xe000ef34
        MOVS     R1,#-1073741824
        STR      R1,[R0, #+0]
//  130 
//  131     /* CP10 and CP11 set to full access.*/
//  132     SCB_CPACR |= 0x00F00000;
        LDR.N    R0,??DataTable2_4  ;; 0xe000ed88
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0xF00000
        LDR.N    R1,??DataTable2_4  ;; 0xe000ed88
        STR      R0,[R1, #+0]
//  133 
//  134     /* Enables FPU context save/restore on exception entry/exit (FPCA bit).*/
//  135     __set_CONTROL(__get_CONTROL() | 4);
        MRS      R0,CONTROL
        ORRS     R0,R0,#0x4
        MSR      CONTROL,R0
//  136 
//  137     /* FPSCR and FPDSCR initially zero.*/
//  138     __set_FPSCR(0);
        MOVS     R0,#+0
        VMSR     FPSCR,R0
//  139     SCB_FPDSCR = 0;
        LDR.N    R0,??DataTable2_5  ;; 0xe000ef3c
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//  140   }
//  141 #endif
//  142 
//  143   /* Initialization of the system vectors used by the port.*/
//  144   nvicSetSystemHandlerPriority(HANDLER_SVCALL,
//  145     CORTEX_PRIORITY_MASK(CORTEX_PRIORITY_SVCALL));
        MOVS     R1,#+16
        MOVS     R0,#+7
          CFI FunCall nvicSetSystemHandlerPriority
        BL       nvicSetSystemHandlerPriority
//  146   nvicSetSystemHandlerPriority(HANDLER_PENDSV,
//  147     CORTEX_PRIORITY_MASK(CORTEX_PRIORITY_PENDSV));
        MOVS     R1,#+32
        MOVS     R0,#+10
          CFI FunCall nvicSetSystemHandlerPriority
        BL       nvicSetSystemHandlerPriority
//  148   nvicSetSystemHandlerPriority(HANDLER_SYSTICK,
//  149     CORTEX_PRIORITY_MASK(CORTEX_PRIORITY_SYSTICK));
        MOVS     R1,#+96
        MOVS     R0,#+11
          CFI FunCall nvicSetSystemHandlerPriority
        BL       nvicSetSystemHandlerPriority
//  150 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock2
//  151 
//  152 /**
//  153  * @brief   Exception exit redirection to _port_switch_from_isr().
//  154  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _port_irq_epilogue
          CFI NoCalls
        THUMB
//  155 void _port_irq_epilogue(void) {
//  156 
//  157   port_lock_from_isr();
_port_irq_epilogue:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
//  158   if ((SCB_ICSR & ICSR_RETTOBASE) != 0) {
        LDR.N    R0,??DataTable2_6  ;; 0xe000ed04
        LDR      R0,[R0, #+0]
        LSLS     R0,R0,#+20
        BPL.N    ??_port_irq_epilogue_0
//  159     struct extctx *ctxp;
//  160 
//  161 #if CORTEX_USE_FPU
//  162     /* Enforcing a lazy FPU state save. Note, it goes in the original
//  163        context because the FPCAR register has not been modified.*/
//  164     (void)__get_FPSCR();
        VMRS     R1,FPSCR
//  165 #endif
//  166 
//  167     /* Current PSP value.*/
//  168     ctxp = (struct extctx *)__get_PSP();
        MRS      R2,PSP
        MOVS     R0,R2
//  169 
//  170     /* Adding an artificial exception return context, there is no need to
//  171        populate it fully.*/
//  172     ctxp--;
        SUBS     R0,R0,#+104
//  173     ctxp->xpsr = (regarm_t)0x01000000;
        MOVS     R2,#+16777216
        STR      R2,[R0, #+28]
//  174 #if CORTEX_USE_FPU
//  175     ctxp->fpscr = (regarm_t)SCB_FPDSCR;
        LDR.N    R2,??DataTable2_5  ;; 0xe000ef3c
        LDR      R2,[R2, #+0]
        STR      R2,[R0, #+96]
//  176 #endif
//  177     __set_PSP((unsigned long)ctxp);
        MSR      PSP,R0
//  178 
//  179     /* The exit sequence is different depending on if a preemption is
//  180        required or not.*/
//  181     if (chSchIsPreemptionRequired()) {
        LDR.N    R2,??DataTable2_7
        LDR      R2,[R2, #+24]
        LDRB     R2,[R2, #+34]
        CMP      R2,#+0
        BEQ.N    ??_port_irq_epilogue_1
        LDR.N    R1,??DataTable2_7
        LDR      R1,[R1, #+24]
        LDR      R1,[R1, #+8]
        LDR.N    R2,??DataTable2_7
        LDR      R2,[R2, #+0]
        LDR      R2,[R2, #+8]
        CMP      R1,R2
        BCS.N    ??_port_irq_epilogue_2
        MOVS     R1,#+1
        B.N      ??_port_irq_epilogue_3
??_port_irq_epilogue_2:
        MOVS     R1,#+0
??_port_irq_epilogue_3:
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        B.N      ??_port_irq_epilogue_4
??_port_irq_epilogue_1:
        LDR.N    R1,??DataTable2_7
        LDR      R1,[R1, #+0]
        LDR      R1,[R1, #+8]
        LDR.N    R2,??DataTable2_7
        LDR      R2,[R2, #+24]
        LDR      R2,[R2, #+8]
        CMP      R1,R2
        BCC.N    ??_port_irq_epilogue_5
        MOVS     R1,#+1
        B.N      ??_port_irq_epilogue_6
??_port_irq_epilogue_5:
        MOVS     R1,#+0
??_port_irq_epilogue_6:
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
??_port_irq_epilogue_4:
        CMP      R1,#+0
        BEQ.N    ??_port_irq_epilogue_7
//  182       /* Preemption is required we need to enforce a context switch.*/
//  183       ctxp->pc = (regarm_t)_port_switch_from_isr;
        LDR.N    R1,??DataTable2_8
        STR      R1,[R0, #+24]
        B.N      ??_port_irq_epilogue_8
//  184     }
//  185     else {
//  186       /* Preemption not required, we just need to exit the exception
//  187          atomically.*/
//  188       ctxp->pc = (regarm_t)_port_exit_from_isr;
??_port_irq_epilogue_7:
        LDR.N    R1,??DataTable2_9
        STR      R1,[R0, #+24]
//  189     }
//  190 
//  191     /* Note, returning without unlocking is intentional, this is done in
//  192        order to keep the rest of the context switch atomic.*/
//  193     return;
??_port_irq_epilogue_8:
        B.N      ??_port_irq_epilogue_9
//  194   }
//  195   port_unlock_from_isr();
??_port_irq_epilogue_0:
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  196 }
??_port_irq_epilogue_9:
        BX       LR               ;; return
          CFI EndBlock cfiBlock3

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2:
        DC32     0xe000ef34

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_1:
        DC32     0xe000ed08

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_2:
        DC32     0xe000ed0c

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_3:
        DC32     0x5fa0300

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_4:
        DC32     0xe000ed88

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_5:
        DC32     0xe000ef3c

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_6:
        DC32     0xe000ed04

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_7:
        DC32     rlist

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_8:
        DC32     _port_switch_from_isr

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_9:
        DC32     _port_exit_from_isr

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        END
//  197 
//  198 /** @} */
// 
// 324 bytes in section .text
// 
// 324 bytes of CODE memory
//
//Errors: none
//Warnings: none
