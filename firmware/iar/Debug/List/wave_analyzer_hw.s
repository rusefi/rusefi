///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     24/May/2015  11:47:23 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\hw_layer\wave_a /
//                       nalyzer_hw.cpp                                       /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\hw_layer\wave_a /
//                       nalyzer_hw.cpp -lCN F:\stuff\rusefi_sourceforge\firm /
//                       ware\iar\Debug\List\ -lA F:\stuff\rusefi_sourceforge /
//                       \firmware\iar\Debug\List\ -o                         /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\Debug\Obj\  /
//                       --no_unroll --debug --endian=little --cpu=Cortex-M4  /
//                       -e --fpu=VFPv4_sp --dlib_config "C:\Program Files    /
//                       (x86)\IAR Systems\Embedded Workbench                 /
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
//                       -Ohs --eec++                                         /
//    MISRA C version =  2004                                                 /
//      Enabled       =  1.2-1.4 2.3 3.4 4.2 6.5 7 8.2 8.3 9.3 12.3 13.6      /
//                       13.7 15.5 17.3 19.17 20.7-20.9 20.11                 /
//      Checked       =  1.4 2.3 4.2 6.5 7 8.2 8.3 9.3 12.3 13.7 15.5         /
//                       20.7-20.9 20.11                                      /
//      Not checked   =  1.1-1.3 1.5-2.2 2.4-4.1 5-6.4 8.1 8.4-9.2 10-12.2    /
//                       12.4-13.6 14-15.4 16-20.6 20.10 20.12 21             /
//    List file       =  F:\stuff\rusefi_sourceforge\firmware\iar\Debug\List\ /
//                       wave_analyzer_hw.s                                   /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME wave_analyzer_hw

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1
        #define SHT_INIT_ARRAY 0xe

        EXTERN ICUD1
        EXTERN ICUD2
        EXTERN ICUD3
        EXTERN ICUD9
        EXTERN _Z11efiIcuStartP9ICUDriverPK9ICUConfig
        EXTERN _Z12mySetPadModePKcP12GPIO_TypeDefjj
        EXTERN _Z8getHwPin11brain_pin_e
        EXTERN _Z9getHwPort11brain_pin_e
        EXTERN _ZN16IntListenerArray22invokeJustArgCallbacksEv
        EXTERN _ZN16IntListenerArrayC1Ev
        EXTERN __aeabi_memclr4
        EXTERN __aeabi_vec_ctor_nocookie_nodtor
        EXTERN firmwareError
        EXTERN icuDisable
        EXTERN icuEnable
        EXTERN icuStop

        PUBLIC _Z16startInputDriverP12WaveReaderHwb
        PUBLIC _Z16turnOnCapturePin11brain_pin_e
        PUBLIC _Z21getInputCaptureDriver11brain_pin_e
        PUBLIC _Z22getInputCaptureChannel11brain_pin_e
        PUBLIC _Z22initWaveAnalyzerDriver11brain_pin_e
        PUBLIC _ZN12WaveReaderHwC1Ev
        PUBLIC _ZN9ArrayListI12WaveReaderHwLi8EE3addEv
        PUBLIC _ZN9ArrayListI12WaveReaderHwLi8EE5resetEv
        PUBLIC _ZN9ArrayListI12WaveReaderHwLi8EEC1Ev
        PUBLIC _ZN9ArrayListI12WaveReaderHwLi8EEC2Ev
        PUBLIC _ZZN9ArrayListI12WaveReaderHwLi8EE3addEvEs
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\hw_layer\wave_analyzer_hw.cpp
//    1 /*
//    2  * @file	wave_analyzer_hw.cpp
//    3  * @brief	Helper methods related to Input Capture Unit (ICU)
//    4  *
//    5  * @date Jun 23, 2013
//    6  * @author Andrey Belomutskiy, (c) 2012-2015
//    7  */
//    8 
//    9 #include "wave_analyzer_hw.h"
//   10 #include "mpu_util.h"
//   11 #include "fl_stack.h"
//   12 
//   13 #if EFI_WAVE_ANALYZER || defined(__DOXYGEN__)
//   14 
//   15 #include "eficonsole.h"
//   16 #include "pin_repository.h"
//   17 
//   18 static void icuWidthCallback(ICUDriver *driver);
//   19 static void icuPeriordCallBack(ICUDriver *driver);
//   20 
//   21 /*
//   22  * 30ms seems like width maximum, at 16bit precision that means
//   23  * CORE_CLOCK / 33.33333 = TICKS * 65536
//   24  * 168000000 / 33.333333 / 65536 = 76.90
//   25  */

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//   26 static ICUConfig wave_icucfg = { ICU_INPUT_ACTIVE_LOW, CORE_CLOCK / 100, icuWidthCallback, icuPeriordCallBack, 0,
wave_icucfg:
        DATA
        DC8 1, 0, 0, 0
        DC32 1680000, _Z16icuWidthCallbackP9ICUDriver
        DC32 _Z18icuPeriordCallBackP9ICUDriver, 0H
        DC8 0, 0, 0, 0
        DC32 0
//   27 		ICU_CHANNEL_1, 0 };
//   28 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   29 static ArrayList<WaveReaderHw, 8> registeredIcus;
registeredIcus:
        DS8 2052

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function __sti__routine
        THUMB
// static __intrinsic __interwork __softfp void __sti__routine()
__sti__routine:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        LDR.N    R4,??DataTable11
        LDR.N    R1,??DataTable11_1
        MOVS     R3,#+8
        MOV      R2,#+256
        ADDS     R0,R4,#+4
          CFI FunCall __aeabi_vec_ctor_nocookie_nodtor
        BL       __aeabi_vec_ctor_nocookie_nodtor
        MOV      R1,#+2048
        ADDS     R0,R4,#+4
          CFI FunCall __aeabi_memclr4
        BL       __aeabi_memclr4
        MOVS     R0,#+0
        STR      R0,[R4, #+0]
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock0
//   30 
//   31 static WaveReaderHw * findWaveReaderHw(ICUDriver *driver) {
//   32 	for (int i = 0; i < registeredIcus.size; i++) {
//   33 		if (registeredIcus.elements[i].driver == driver) {
//   34 			return &registeredIcus.elements[i];
//   35 		}
//   36 	}
//   37 	firmwareError("reader not found");
//   38 	return (WaveReaderHw *) NULL;
//   39 }
//   40 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z16icuWidthCallbackP9ICUDriver
        THUMB
//   41 static void icuWidthCallback(ICUDriver *driver) {
_Z16icuWidthCallbackP9ICUDriver:
        PUSH     {R4,R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+12
//   42 	/*
//   43 	 * see comment in icuPeriordCallBack
//   44 	 int rowWidth = icuGetWidth(driver);
//   45 	 */
//   46 	WaveReaderHw * hw = findWaveReaderHw(driver);
        LDR.N    R2,??DataTable11
        LDR      R4,[R2, #+0]
        SUB      SP,SP,#+4
          CFI CFA R13+16
        MOVS     R1,#+0
        MOV      R3,R2
        B.N      ??icuWidthCallback_0
??icuWidthCallback_1:
        ADDS     R1,R1,#+1
        ADD      R3,R3,#+256
??icuWidthCallback_0:
        CMP      R1,R4
        BGE.N    ??icuWidthCallback_2
        LDR      R5,[R3, #+4]
        CMP      R5,R0
        BNE.N    ??icuWidthCallback_1
        ADD      SP,SP,#+4
          CFI CFA R13+12
        POP      {R4,R5,LR}
          CFI R4 SameValue
          CFI R5 SameValue
          CFI R14 SameValue
          CFI CFA R13+0
        ADD      R0,R2,R1, LSL #+8
        ADDS     R0,R0,#+4
        ADDS     R0,R0,#+8
        B.W      _ZN16IntListenerArray22invokeJustArgCallbacksEv
          CFI R4 Frame(CFA, -12)
          CFI R5 Frame(CFA, -8)
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+16
??icuWidthCallback_2:
        ADR.W    R0,`?<Constant "reader not found">`
          CFI FunCall firmwareError
        BL       firmwareError
//   47 	hw->widthListeners.invokeJustArgCallbacks();
        ADD      SP,SP,#+4
          CFI CFA R13+12
        POP      {R4,R5,LR}
          CFI R4 SameValue
          CFI R5 SameValue
          CFI R14 SameValue
          CFI CFA R13+0
        MOVS     R0,#+0
        ADDS     R0,R0,#+8
          CFI FunCall _ZN16IntListenerArray22invokeJustArgCallbacksEv
        B.W      _ZN16IntListenerArray22invokeJustArgCallbacksEv
          CFI EndBlock cfiBlock1
//   48 }
//   49 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z18icuPeriordCallBackP9ICUDriver
        THUMB
//   50 static void icuPeriordCallBack(ICUDriver *driver) {
_Z18icuPeriordCallBackP9ICUDriver:
        PUSH     {R4,R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+12
//   51 	/*
//   52 	 * 	we do not use timer period at all - we just need the event. For all time characteristics,
//   53 	 * 	we use system time
//   54 	 * 	int period = icuGetPeriod(driver);
//   55 	 */
//   56 
//   57 	WaveReaderHw * hw = findWaveReaderHw(driver);
        LDR.N    R2,??DataTable11
        LDR      R4,[R2, #+0]
        SUB      SP,SP,#+4
          CFI CFA R13+16
        MOVS     R1,#+0
        MOV      R3,R2
        B.N      ??icuPeriordCallBack_0
??icuPeriordCallBack_1:
        ADDS     R1,R1,#+1
        ADD      R3,R3,#+256
??icuPeriordCallBack_0:
        CMP      R1,R4
        BGE.N    ??icuPeriordCallBack_2
        LDR      R5,[R3, #+4]
        CMP      R5,R0
        BNE.N    ??icuPeriordCallBack_1
        ADD      SP,SP,#+4
          CFI CFA R13+12
        POP      {R4,R5,LR}
          CFI R4 SameValue
          CFI R5 SameValue
          CFI R14 SameValue
          CFI CFA R13+0
        ADD      R0,R2,R1, LSL #+8
        ADDS     R0,R0,#+4
        ADDS     R0,R0,#+132
        B.W      _ZN16IntListenerArray22invokeJustArgCallbacksEv
          CFI R4 Frame(CFA, -12)
          CFI R5 Frame(CFA, -8)
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+16
??icuPeriordCallBack_2:
        ADR.W    R0,`?<Constant "reader not found">`
          CFI FunCall firmwareError
        BL       firmwareError
//   58 	hw->periodListeners.invokeJustArgCallbacks();
        ADD      SP,SP,#+4
          CFI CFA R13+12
        POP      {R4,R5,LR}
          CFI R4 SameValue
          CFI R5 SameValue
          CFI R14 SameValue
          CFI CFA R13+0
        MOVS     R0,#+0
        ADDS     R0,R0,#+132
          CFI FunCall _ZN16IntListenerArray22invokeJustArgCallbacksEv
        B.W      _ZN16IntListenerArray22invokeJustArgCallbacksEv
          CFI EndBlock cfiBlock2
//   59 }
//   60 
//   61 static uint32_t getAlternateFunctions(ICUDriver *driver) {
//   62 	if (driver == NULL) {
//   63 		firmwareError("getAlternateFunctions(NULL)");
//   64 		return 0xffffffff;
//   65 	}
//   66 #if STM32_ICU_USE_TIM1
//   67 	if (driver == &ICUD1) {
//   68 		return GPIO_AF_TIM1;
//   69 	}
//   70 #endif
//   71 #if STM32_ICU_USE_TIM2
//   72 	if (driver == &ICUD2) {
//   73 		return GPIO_AF_TIM2;
//   74 	}
//   75 #endif
//   76 #if STM32_ICU_USE_TIM3
//   77 	if (driver == &ICUD3) {
//   78 		return GPIO_AF_TIM3;
//   79 	}
//   80 #endif
//   81 #if STM32_ICU_USE_TIM4
//   82 	if (driver == &ICUD4) {
//   83 		return GPIO_AF_TIM4;
//   84 	}
//   85 #endif
//   86 #if STM32_ICU_USE_TIM9
//   87 	if (driver == &ICUD9) {
//   88 		return GPIO_AF_TIM9;
//   89 	}
//   90 #endif
//   91 	firmwareError("No such driver");
//   92 	return 0xffffffff;
//   93 }
//   94 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z22getInputCaptureChannel11brain_pin_e
        THUMB
//   95 icuchannel_t getInputCaptureChannel(brain_pin_e hwPin) {
_Z22getInputCaptureChannel11brain_pin_e:
        PUSH     {LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+4
//   96 	switch (hwPin) {
        MOV      R1,R0
        SUB      SP,SP,#+4
          CFI CFA R13+8
        CMP      R1,#+5
        IT       NE 
        CMPNE    R1,#+8
        BEQ.N    ??getInputCaptureChannel_0
        CMP      R1,#+38
        IT       NE 
        CMPNE    R1,#+69
        BEQ.N    ??getInputCaptureChannel_0
        CMP      R1,#+70
        BEQ.N    ??getInputCaptureChannel_1
        CMP      R1,#+73
        BEQ.N    ??getInputCaptureChannel_0
        CMP      R1,#+75
        BNE.N    ??getInputCaptureChannel_2
//   97 	case GPIOA_5:
//   98 	case GPIOA_8:
//   99 	case GPIOC_6:
//  100 	case GPIOE_5:
//  101 	case GPIOE_9:
//  102 		return ICU_CHANNEL_1;
//  103 
//  104 	case GPIOE_6:
//  105 	case GPIOE_11:
//  106 		return ICU_CHANNEL_2;
??getInputCaptureChannel_1:
        MOVS     R0,#+1
        ADD      SP,SP,#+4
          CFI CFA R13+4
        POP      {PC}
          CFI CFA R13+8
//  107 	default:
//  108 		firmwareError("Unexpected hw pin in getInputCaptureChannel %d", hwPin);
??getInputCaptureChannel_2:
        ADR.W    R0,`?<Constant "Unexpected hw pin in ...">`
          CFI FunCall firmwareError
        BL       firmwareError
//  109 		return ICU_CHANNEL_1;
??getInputCaptureChannel_0:
        MOVS     R0,#+0
        ADD      SP,SP,#+4
          CFI CFA R13+4
        POP      {PC}             ;; return
          CFI EndBlock cfiBlock3
//  110 	}
//  111 }
//  112 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _Z21getInputCaptureDriver11brain_pin_e
          CFI NoCalls
        THUMB
//  113 ICUDriver * getInputCaptureDriver(brain_pin_e hwPin) {
//  114 #if STM32_ICU_USE_TIM1
//  115 	if (hwPin == GPIOA_8) {
_Z21getInputCaptureDriver11brain_pin_e:
        CMP      R0,#+8
        ITT      NE 
        CMPNE    R0,#+73
//  116 		return &ICUD1;
//  117 	}
//  118 	if (hwPin == GPIOE_9) {
        CMPNE    R0,#+75
//  119 		return &ICUD1;
//  120 	}
//  121 	if (hwPin == GPIOE_11) {
        BNE.N    ??getInputCaptureDriver_0
//  122 		return &ICUD1;
        LDR.N    R0,??DataTable11_2
        BX       LR
//  123 	}
//  124 #endif
//  125 #if STM32_ICU_USE_TIM2
//  126 	if (hwPin == GPIOA_5) {
??getInputCaptureDriver_0:
        CMP      R0,#+5
        BNE.N    ??getInputCaptureDriver_1
//  127 		return &ICUD2;
        LDR.N    R0,??DataTable11_3
        BX       LR
//  128 	}
//  129 #endif
//  130 #if STM32_ICU_USE_TIM3
//  131 	if (hwPin == GPIOC_6) {
??getInputCaptureDriver_1:
        CMP      R0,#+38
        BNE.N    ??getInputCaptureDriver_2
//  132 		return &ICUD3;
        LDR.N    R0,??DataTable11_4
        BX       LR
//  133 	}
//  134 #endif
//  135 #if STM32_ICU_USE_TIM9
//  136 	if (hwPin == GPIOE_5) {
??getInputCaptureDriver_2:
        CMP      R0,#+69
        BNE.N    ??getInputCaptureDriver_3
//  137 		return &ICUD9;
        LDR.N    R0,??DataTable11_5
        BX       LR
//  138 	}
//  139 #endif
//  140 	return (ICUDriver *) NULL;
??getInputCaptureDriver_3:
        MOVS     R0,#+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock4
//  141 }
//  142 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _Z16turnOnCapturePin11brain_pin_e
        THUMB
//  143 void turnOnCapturePin(brain_pin_e brainPin) {
_Z16turnOnCapturePin11brain_pin_e:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOV      R6,R0
//  144 	ioportid_t port = getHwPort(brainPin);
          CFI FunCall _Z9getHwPort11brain_pin_e
        BL       _Z9getHwPort11brain_pin_e
        MOV      R4,R0
//  145 	ioportmask_t pin = getHwPin(brainPin);
        MOV      R0,R6
          CFI FunCall _Z8getHwPin11brain_pin_e
        BL       _Z8getHwPin11brain_pin_e
        MOV      R5,R0
//  146 
//  147 	ICUDriver *driver = getInputCaptureDriver(brainPin);
        CMP      R6,#+8
        ITT      NE 
        CMPNE    R6,#+73
        CMPNE    R6,#+75
        BEQ.N    ??turnOnCapturePin_0
        CMP      R6,#+5
        IT       EQ 
        LDREQ.N  R0,??DataTable11_3
        BEQ.N    ??turnOnCapturePin_1
        CMP      R6,#+38
        IT       EQ 
        LDREQ.N  R0,??DataTable11_4
        BEQ.N    ??turnOnCapturePin_1
        CMP      R6,#+69
        BNE.N    ??turnOnCapturePin_2
        LDR.N    R0,??DataTable11_5
        B.N      ??turnOnCapturePin_1
??turnOnCapturePin_0:
        LDR.N    R0,??DataTable11_2
//  148 	if (driver != NULL) {
??turnOnCapturePin_1:
        CBZ.N    R0,??turnOnCapturePin_2
//  149 		iomode_t mode = (iomode_t) PAL_MODE_ALTERNATE(getAlternateFunctions(driver));
        LDR.N    R1,??DataTable11_2
        CMP      R0,R1
        ITT      NE 
        LDRNE.N  R1,??DataTable11_3
        CMPNE    R0,R1
        BEQ.N    ??turnOnCapturePin_3
        LDR.N    R1,??DataTable11_4
        CMP      R0,R1
        IT       EQ 
        MOVEQ    R0,#+2
        BEQ.N    ??turnOnCapturePin_4
        LDR.N    R1,??DataTable11_5
        CMP      R0,R1
        IT       EQ 
        MOVEQ    R0,#+3
        BEQ.N    ??turnOnCapturePin_4
        ADR.W    R0,`?<Constant "No such driver">`
          CFI FunCall firmwareError
        BL       firmwareError
        MOV      R0,#-1
        B.N      ??turnOnCapturePin_4
??turnOnCapturePin_3:
        MOVS     R0,#+1
//  150 		mySetPadMode("wave input", port, pin, mode);
??turnOnCapturePin_4:
        MOV      R2,R5
        MOV      R1,R4
        POP      {R4-R6,LR}
          CFI R4 SameValue
          CFI R5 SameValue
          CFI R6 SameValue
          CFI R14 SameValue
          CFI CFA R13+0
        LSLS     R0,R0,#+7
        ORR      R3,R0,#0x2
        ADR.W    R0,`?<Constant "wave input">`
          CFI FunCall _Z12mySetPadModePKcP12GPIO_TypeDefjj
        B.W      _Z12mySetPadModePKcP12GPIO_TypeDefjj
          CFI R4 Frame(CFA, -16)
          CFI R5 Frame(CFA, -12)
          CFI R6 Frame(CFA, -8)
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+16
//  151 	}
//  152 }
??turnOnCapturePin_2:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock5
//  153 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _Z22initWaveAnalyzerDriver11brain_pin_e
        THUMB
//  154 WaveReaderHw * initWaveAnalyzerDriver(brain_pin_e brainPin) {
_Z22initWaveAnalyzerDriver11brain_pin_e:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOV      R4,R0
//  155 	ICUDriver *driver = getInputCaptureDriver(brainPin);
        CMP      R4,#+8
        ITT      NE 
        CMPNE    R4,#+73
        CMPNE    R4,#+75
        BEQ.N    ??initWaveAnalyzerDriver_0
        CMP      R4,#+5
        IT       EQ 
        LDREQ.N  R5,??DataTable11_3
        BEQ.N    ??initWaveAnalyzerDriver_1
        CMP      R4,#+38
        IT       EQ 
        LDREQ.N  R5,??DataTable11_4
        BEQ.N    ??initWaveAnalyzerDriver_1
        CMP      R4,#+69
        ITE      NE 
        MOVNE    R5,#+0
        LDREQ.N  R5,??DataTable11_5
        B.N      ??initWaveAnalyzerDriver_1
??initWaveAnalyzerDriver_0:
        LDR.N    R5,??DataTable11_2
//  156 
//  157 	WaveReaderHw *hw = registeredIcus.add();
??initWaveAnalyzerDriver_1:
        LDR.N    R0,??DataTable11
        LDR      R1,[R0, #+0]
        CMP      R1,#+8
        BLT.N    ??initWaveAnalyzerDriver_2
        LDR.N    R0,??DataTable11_6
          CFI FunCall firmwareError
        BL       firmwareError
        MOVS     R6,#+0
        B.N      ??initWaveAnalyzerDriver_3
??initWaveAnalyzerDriver_2:
        ADD      R2,R0,R1, LSL #+8
        ADDS     R1,R1,#+1
        ADDS     R6,R2,#+4
        STR      R1,[R0, #+0]
//  158 
//  159 	hw->driver = driver;
??initWaveAnalyzerDriver_3:
        STR      R5,[R6, #+0]
//  160 	turnOnCapturePin(brainPin);
        MOV      R0,R4
          CFI FunCall _Z16turnOnCapturePin11brain_pin_e
        BL       _Z16turnOnCapturePin11brain_pin_e
//  161 	return hw;
        MOV      R0,R6
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock6
//  162 }

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function _Z16startInputDriverP12WaveReaderHwb
        THUMB
_Z16startInputDriverP12WaveReaderHwb:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOV      R4,R0
        SUBS     R0,R1,#+1
        SBCS     R0,R0,R0
        LDR.N    R5,??DataTable11_7
        STRB     R1,[R4, #+4]
        LSRS     R0,R0,#+31
        STRB     R0,[R5, #+0]
        LDR      R6,[R4, #+0]
        CBZ.N    R6,??startInputDriver_0
        LDRB     R0,[R4, #+5]
        CBZ.N    R0,??startInputDriver_1
        MOV      R0,R6
          CFI FunCall icuDisable
        BL       icuDisable
        MOV      R0,R6
          CFI FunCall icuStop
        BL       icuStop
??startInputDriver_1:
        MOV      R1,R5
        MOV      R0,R6
          CFI FunCall _Z11efiIcuStartP9ICUDriverPK9ICUConfig
        BL       _Z11efiIcuStartP9ICUDriverPK9ICUConfig
        MOV      R0,R6
          CFI FunCall icuEnable
        BL       icuEnable
??startInputDriver_0:
        MOVS     R0,#+1
        STRB     R0,[R4, #+5]
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock7

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11:
        DC32     registeredIcus

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_1:
        DC32     _ZN12WaveReaderHwC1Ev

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_2:
        DC32     ICUD1

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_3:
        DC32     ICUD2

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_4:
        DC32     ICUD3

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_5:
        DC32     ICUD9

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_6:
        DC32     _ZZN9ArrayListI12WaveReaderHwLi8EE3addEvEs

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_7:
        DC32     wave_icucfg

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
`?<Constant "reader not found">`:
        DC8 "reader not found"
        DC8 0, 0, 0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
`?<Constant "No such driver">`:
        DC8 "No such driver"
        DC8 0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
`?<Constant "Unexpected hw pin in ...">`:
        DC8 "Unexpected hw pin in getInputCaptureChannel %d"
        DC8 0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
`?<Constant "wave input">`:
        DC8 "wave input"
        DC8 0

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN12WaveReaderHwC1Ev
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function _ZN12WaveReaderHwC1Ev
        THUMB
// __code __interwork __softfp WaveReaderHw::<_ZN12WaveReaderHwC1Ev>()
_ZN12WaveReaderHwC1Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOV      R4,R0
        ADD      R0,R4,#+8
          CFI FunCall _ZN16IntListenerArrayC1Ev
        BL       _ZN16IntListenerArrayC1Ev
        ADD      R0,R4,#+132
          CFI FunCall _ZN16IntListenerArrayC1Ev
        BL       _ZN16IntListenerArrayC1Ev
        MOV      R0,R4
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock8

        SECTION `.text`:CODE:REORDER:NOROOT(2)
        SECTION_GROUP _ZN9ArrayListI12WaveReaderHwLi8EEC1Ev
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function _ZN9ArrayListI12WaveReaderHwLi8EEC1Ev
        THUMB
// __code __interwork __softfp ArrayList<WaveReaderHw, 8>::ArrayList()
_ZN9ArrayListI12WaveReaderHwLi8EEC1Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOV      R4,R0
        MOVS     R3,#+8
        MOV      R2,#+256
        LDR.N    R1,??ArrayList_0
        ADDS     R0,R4,#+4
          CFI FunCall __aeabi_vec_ctor_nocookie_nodtor
        BL       __aeabi_vec_ctor_nocookie_nodtor
        MOV      R1,#+2048
        ADDS     R0,R4,#+4
          CFI FunCall __aeabi_memclr4
        BL       __aeabi_memclr4
        MOVS     R0,#+0
        STR      R0,[R4, #+0]
        MOV      R0,R4
        POP      {R4,PC}          ;; return
        DATA
??ArrayList_0:
        DC32     _ZN12WaveReaderHwC1Ev
          CFI EndBlock cfiBlock9

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN9ArrayListI12WaveReaderHwLi8EE5resetEv
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function _ZN9ArrayListI12WaveReaderHwLi8EE5resetEv
          CFI NoCalls
        THUMB
// __interwork __softfp void ArrayList<WaveReaderHw, 8>::reset()
_ZN9ArrayListI12WaveReaderHwLi8EE5resetEv:
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
        BX       LR               ;; return
          CFI EndBlock cfiBlock10

        SECTION `.text`:CODE:REORDER:NOROOT(2)
        SECTION_GROUP _ZN9ArrayListI12WaveReaderHwLi8EE3addEv
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function _ZN9ArrayListI12WaveReaderHwLi8EE3addEv
        THUMB
// __interwork __softfp WaveReaderHw *ArrayList<WaveReaderHw, 8>::add()
_ZN9ArrayListI12WaveReaderHwLi8EE3addEv:
        PUSH     {LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+4
        SUB      SP,SP,#+4
          CFI CFA R13+8
        LDR      R1,[R0, #+0]
        CMP      R1,#+8
        BLT.N    ??add_0
        LDR.N    R0,??add_1
          CFI FunCall firmwareError
        BL       firmwareError
        MOVS     R0,#+0
        ADD      SP,SP,#+4
          CFI CFA R13+4
        POP      {PC}
          CFI CFA R13+8
??add_0:
        ADDS     R2,R1,#+1
        STR      R2,[R0, #+0]
        ADD      R0,R0,R1, LSL #+8
        ADDS     R0,R0,#+4
        ADD      SP,SP,#+4
          CFI CFA R13+4
        POP      {PC}             ;; return
        DATA
??add_1:
        DC32     _ZZN9ArrayListI12WaveReaderHwLi8EE3addEvEs
          CFI EndBlock cfiBlock11

        SECTION `.init_array`:CODE:ROOT(2)
        SECTION_TYPE SHT_INIT_ARRAY, 0
        DATA
        DC32    RELOC_ARM_TARGET1 __sti__routine

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        SECTION_GROUP _ZZN9ArrayListI12WaveReaderHwLi8EE3addEvEs
// __absolute char const <_ZZN9ArrayListI12WaveReaderHwLi8EE3addEvEs>[24]
_ZZN9ArrayListI12WaveReaderHwLi8EE3addEvEs:
        DATA
        DC8 "add() too many elements"

        SECTION `.rodata`:CONST:NOROOT(2)
        DATA
`?<Constant "getAlternateFunctions...">`:
        DC8 "getAlternateFunctions(NULL)"

        SECTION `.text`:CODE:REORDER:NOROOT(2)
        SECTION_GROUP _ZN9ArrayListI12WaveReaderHwLi8EEC2Ev
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function _ZN9ArrayListI12WaveReaderHwLi8EEC2Ev
        THUMB
// __code __interwork __softfp ArrayList<WaveReaderHw, 8>::subobject ArrayList()
_ZN9ArrayListI12WaveReaderHwLi8EEC2Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOV      R4,R0
        MOVS     R3,#+8
        MOV      R2,#+256
        LDR.N    R1,`??subobject ArrayList_0`
        ADDS     R0,R4,#+4
          CFI FunCall __aeabi_vec_ctor_nocookie_nodtor
        BL       __aeabi_vec_ctor_nocookie_nodtor
        MOV      R1,#+2048
        ADDS     R0,R4,#+4
          CFI FunCall __aeabi_memclr4
        BL       __aeabi_memclr4
        MOVS     R0,#+0
        STR      R0,[R4, #+0]
        MOV      R0,R4
        POP      {R4,PC}          ;; return
        DATA
`??subobject ArrayList_0`:
        DC32     _ZN12WaveReaderHwC1Ev
          CFI EndBlock cfiBlock12

        END
//  163 
//  164 void startInputDriver(WaveReaderHw *hw, bool isActiveHigh) {
//  165 	hw->isActiveHigh = isActiveHigh;
//  166 	if (hw->isActiveHigh) {
//  167 		wave_icucfg.mode = ICU_INPUT_ACTIVE_HIGH;
//  168 	} else {
//  169 		wave_icucfg.mode = ICU_INPUT_ACTIVE_LOW;
//  170 	}
//  171 	ICUDriver *driver = hw->driver;
//  172 
//  173 	if (driver != NULL) {
//  174 		if (hw->started) {
//  175 			icuDisable(driver);
//  176 			icuStop(driver);
//  177 		}
//  178 		efiIcuStart(driver, &wave_icucfg);
//  179 		icuEnable(driver);
//  180 	}
//  181 	hw->started = true;
//  182 }
//  183 
//  184 #endif
// 
// 2 052 bytes in section .bss
//    28 bytes in section .data
//     4 bytes in section .init_array
//    52 bytes in section .rodata
//   814 bytes in section .text
// 
//   668 bytes of CODE  memory (+ 150 bytes shared)
//    28 bytes of CONST memory (+  24 bytes shared)
// 2 080 bytes of DATA  memory
//
//Errors: none
//Warnings: none
