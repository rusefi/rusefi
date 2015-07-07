///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:44 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\hw_layer\digita /
//                       l_input_hw.cpp                                       /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\hw_layer\digita /
//                       l_input_hw.cpp -lCN F:\stuff\rusefi_sourceforge\firm /
//                       ware\iar\Debug\List\ -lA F:\stuff\rusefi_sourceforge /
//                       \firmware\iar\Debug\List\ -o                         /
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
//                       -On --eec++                                          /
//    MISRA C version =  2004                                                 /
//      Enabled       =  1.2-1.4 2.3 3.4 4.2 6.5 7 8.2 8.3 9.3 12.3 13.6      /
//                       13.7 15.5 17.3 19.17 20.7-20.9 20.11                 /
//      Checked       =  1.4 2.3 4.2 6.5 7 8.2 8.3 9.3 12.3 13.7 15.5         /
//                       20.7-20.9 20.11                                      /
//      Not checked   =  1.1-1.3 1.5-2.2 2.4-4.1 5-6.4 8.1 8.4-9.2 10-12.2    /
//                       12.4-13.6 14-15.4 16-20.6 20.10 20.12 21             /
//    List file       =  F:\stuff\rusefi_sourceforge\firmware\iar\Debug\List\ /
//                       digital_input_hw.s                                   /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME digital_input_hw

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
        EXTERN __aeabi_memset
        EXTERN __aeabi_vec_ctor_nocookie_nodtor
        EXTERN firmwareError
        EXTERN icuDisable
        EXTERN icuEnable
        EXTERN icuStop

        PUBLIC _Z16startInputDriverP15digital_input_sb
        PUBLIC _Z16turnOnCapturePinPKc11brain_pin_e
        PUBLIC _Z21getInputCaptureDriver11brain_pin_e
        PUBLIC _Z22getInputCaptureChannel11brain_pin_e
        PUBLIC _Z22initWaveAnalyzerDriverPKc11brain_pin_e
        PUBLIC _ZN15digital_input_sC1Ev
        PUBLIC _ZN9ArrayListI15digital_input_sLi8EE3addEv
        PUBLIC _ZN9ArrayListI15digital_input_sLi8EE5resetEv
        PUBLIC _ZN9ArrayListI15digital_input_sLi8EEC1Ev
        PUBLIC _ZN9ArrayListI15digital_input_sLi8EEC2Ev
        PUBLIC _ZZN9ArrayListI15digital_input_sLi8EE3addEvEs
        PUBLIC memset
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\hw_layer\digital_input_hw.cpp
//    1 /*
//    2  * @file	digital_input_hw.cpp
//    3  * @brief	Helper methods related to Input Capture Unit (ICU)
//    4  *
//    5  * @date Jun 23, 2013
//    6  * @author Andrey Belomutskiy, (c) 2012-2015
//    7  */
//    8 
//    9 #include "digital_input_hw.h"
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

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function __sti__routine
        THUMB
// static __intrinsic __interwork __softfp void __sti__routine()
__sti__routine:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   29 static ArrayList<digital_input_s, 8> registeredIcus;
        LDR.N    R0,??DataTable6
          CFI FunCall _ZN9ArrayListI15digital_input_sLi8EEC1Ev
        BL       _ZN9ArrayListI15digital_input_sLi8EEC1Ev
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock0

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
registeredIcus:
        DS8 2052
//   30 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z19finddigital_input_sP9ICUDriver
        THUMB
//   31 static digital_input_s * finddigital_input_s(ICUDriver *driver) {
_Z19finddigital_input_sP9ICUDriver:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   32 	for (int i = 0; i < registeredIcus.size; i++) {
        MOVS     R0,#+0
??finddigital_input_s_0:
        LDR.N    R1,??DataTable6
        LDR      R1,[R1, #+0]
        CMP      R0,R1
        BGE.N    ??finddigital_input_s_1
//   33 		if (registeredIcus.elements[i].driver == driver) {
        MOV      R1,#+256
        LDR.N    R2,??DataTable6
        MLA      R1,R1,R0,R2
        LDR      R1,[R1, #+4]
        CMP      R1,R4
        BNE.N    ??finddigital_input_s_2
//   34 			return &registeredIcus.elements[i];
        MOV      R1,#+256
        LDR.N    R2,??DataTable6
        MLA      R0,R1,R0,R2
        ADDS     R0,R0,#+4
        B.N      ??finddigital_input_s_3
//   35 		}
//   36 	}
??finddigital_input_s_2:
        ADDS     R0,R0,#+1
        B.N      ??finddigital_input_s_0
//   37 	firmwareError("reader not found");
??finddigital_input_s_1:
        LDR.N    R0,??DataTable6_1
          CFI FunCall firmwareError
        BL       firmwareError
//   38 	return (digital_input_s *) NULL;
        MOVS     R0,#+0
??finddigital_input_s_3:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock1
//   39 }
//   40 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z16icuWidthCallbackP9ICUDriver
        THUMB
//   41 static void icuWidthCallback(ICUDriver *driver) {
_Z16icuWidthCallbackP9ICUDriver:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//   42 	/*
//   43 	 * see comment in icuPeriordCallBack
//   44 	 int rowWidth = icuGetWidth(driver);
//   45 	 */
//   46 	digital_input_s * hw = finddigital_input_s(driver);
        MOVS     R0,R4
          CFI FunCall _Z19finddigital_input_sP9ICUDriver
        BL       _Z19finddigital_input_sP9ICUDriver
        MOVS     R5,R0
//   47 	hw->widthListeners.invokeJustArgCallbacks();
        ADDS     R0,R5,#+8
          CFI FunCall _ZN16IntListenerArray22invokeJustArgCallbacksEv
        BL       _ZN16IntListenerArray22invokeJustArgCallbacksEv
//   48 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock2
//   49 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z18icuPeriordCallBackP9ICUDriver
        THUMB
//   50 static void icuPeriordCallBack(ICUDriver *driver) {
_Z18icuPeriordCallBackP9ICUDriver:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//   51 	/*
//   52 	 * 	we do not use timer period at all - we just need the event. For all time characteristics,
//   53 	 * 	we use system time
//   54 	 * 	int period = icuGetPeriod(driver);
//   55 	 */
//   56 
//   57 	digital_input_s * hw = finddigital_input_s(driver);
        MOVS     R0,R4
          CFI FunCall _Z19finddigital_input_sP9ICUDriver
        BL       _Z19finddigital_input_sP9ICUDriver
        MOVS     R5,R0
//   58 	hw->periodListeners.invokeJustArgCallbacks();
        ADDS     R0,R5,#+132
          CFI FunCall _ZN16IntListenerArray22invokeJustArgCallbacksEv
        BL       _ZN16IntListenerArray22invokeJustArgCallbacksEv
//   59 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock3
//   60 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _Z21getAlternateFunctionsP9ICUDriver
        THUMB
//   61 static uint32_t getAlternateFunctions(ICUDriver *driver) {
_Z21getAlternateFunctionsP9ICUDriver:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   62 	if (driver == NULL) {
        CMP      R4,#+0
        BNE.N    ??getAlternateFunctions_0
//   63 		firmwareError("getAlternateFunctions(NULL)");
        LDR.N    R0,??DataTable6_2
          CFI FunCall firmwareError
        BL       firmwareError
//   64 		return 0xffffffff;
        MOVS     R0,#-1
        B.N      ??getAlternateFunctions_1
//   65 	}
//   66 #if STM32_ICU_USE_TIM1
//   67 	if (driver == &ICUD1) {
??getAlternateFunctions_0:
        LDR.N    R0,??DataTable6_3
        CMP      R4,R0
        BNE.N    ??getAlternateFunctions_2
//   68 		return GPIO_AF_TIM1;
        MOVS     R0,#+1
        B.N      ??getAlternateFunctions_1
//   69 	}
//   70 #endif
//   71 #if STM32_ICU_USE_TIM2
//   72 	if (driver == &ICUD2) {
??getAlternateFunctions_2:
        LDR.N    R0,??DataTable6_4
        CMP      R4,R0
        BNE.N    ??getAlternateFunctions_3
//   73 		return GPIO_AF_TIM2;
        MOVS     R0,#+1
        B.N      ??getAlternateFunctions_1
//   74 	}
//   75 #endif
//   76 #if STM32_ICU_USE_TIM3
//   77 	if (driver == &ICUD3) {
??getAlternateFunctions_3:
        LDR.N    R0,??DataTable6_5
        CMP      R4,R0
        BNE.N    ??getAlternateFunctions_4
//   78 		return GPIO_AF_TIM3;
        MOVS     R0,#+2
        B.N      ??getAlternateFunctions_1
//   79 	}
//   80 #endif
//   81 #if STM32_ICU_USE_TIM4
//   82 	if (driver == &ICUD4) {
//   83 		return GPIO_AF_TIM4;
//   84 	}
//   85 #endif
//   86 #if STM32_ICU_USE_TIM9
//   87 	if (driver == &ICUD9) {
??getAlternateFunctions_4:
        LDR.N    R0,??DataTable6_6
        CMP      R4,R0
        BNE.N    ??getAlternateFunctions_5
//   88 		return GPIO_AF_TIM9;
        MOVS     R0,#+3
        B.N      ??getAlternateFunctions_1
//   89 	}
//   90 #endif
//   91 	firmwareError("No such driver");
??getAlternateFunctions_5:
        LDR.N    R0,??DataTable6_7
          CFI FunCall firmwareError
        BL       firmwareError
//   92 	return 0xffffffff;
        MOVS     R0,#-1
??getAlternateFunctions_1:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock4
//   93 }
//   94 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _Z22getInputCaptureChannel11brain_pin_e
        THUMB
//   95 icuchannel_t getInputCaptureChannel(brain_pin_e hwPin) {
_Z22getInputCaptureChannel11brain_pin_e:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   96 	switch (hwPin) {
        MOVS     R0,R4
        CMP      R0,#+5
        BEQ.N    ??getInputCaptureChannel_0
        CMP      R0,#+8
        BEQ.N    ??getInputCaptureChannel_0
        CMP      R0,#+38
        BEQ.N    ??getInputCaptureChannel_0
        CMP      R0,#+69
        BEQ.N    ??getInputCaptureChannel_0
        CMP      R0,#+70
        BEQ.N    ??getInputCaptureChannel_1
        CMP      R0,#+73
        BEQ.N    ??getInputCaptureChannel_0
        CMP      R0,#+75
        BEQ.N    ??getInputCaptureChannel_1
        B.N      ??getInputCaptureChannel_2
//   97 	case GPIOA_5:
//   98 	case GPIOA_8:
//   99 	case GPIOC_6:
//  100 	case GPIOE_5:
//  101 	case GPIOE_9:
//  102 		return ICU_CHANNEL_1;
??getInputCaptureChannel_0:
        MOVS     R0,#+0
        B.N      ??getInputCaptureChannel_3
//  103 
//  104 	case GPIOE_6:
//  105 	case GPIOE_11:
//  106 		return ICU_CHANNEL_2;
??getInputCaptureChannel_1:
        MOVS     R0,#+1
        B.N      ??getInputCaptureChannel_3
//  107 	default:
//  108 		firmwareError("Unexpected hw pin in getInputCaptureChannel %d", hwPin);
??getInputCaptureChannel_2:
        MOVS     R1,R4
        LDR.N    R0,??DataTable6_8
          CFI FunCall firmwareError
        BL       firmwareError
//  109 		return ICU_CHANNEL_1;
        MOVS     R0,#+0
??getInputCaptureChannel_3:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock5
//  110 	}
//  111 }
//  112 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _Z21getInputCaptureDriver11brain_pin_e
          CFI NoCalls
        THUMB
//  113 ICUDriver * getInputCaptureDriver(brain_pin_e hwPin) {
_Z21getInputCaptureDriver11brain_pin_e:
        MOVS     R1,R0
//  114 #if STM32_ICU_USE_TIM1
//  115 	if (hwPin == GPIOA_8) {
        CMP      R1,#+8
        BNE.N    ??getInputCaptureDriver_0
//  116 		return &ICUD1;
        LDR.N    R0,??DataTable6_3
        B.N      ??getInputCaptureDriver_1
//  117 	}
//  118 	if (hwPin == GPIOE_9) {
??getInputCaptureDriver_0:
        CMP      R1,#+73
        BNE.N    ??getInputCaptureDriver_2
//  119 		return &ICUD1;
        LDR.N    R0,??DataTable6_3
        B.N      ??getInputCaptureDriver_1
//  120 	}
//  121 	if (hwPin == GPIOE_11) {
??getInputCaptureDriver_2:
        CMP      R1,#+75
        BNE.N    ??getInputCaptureDriver_3
//  122 		return &ICUD1;
        LDR.N    R0,??DataTable6_3
        B.N      ??getInputCaptureDriver_1
//  123 	}
//  124 #endif
//  125 #if STM32_ICU_USE_TIM2
//  126 	if (hwPin == GPIOA_5) {
??getInputCaptureDriver_3:
        CMP      R1,#+5
        BNE.N    ??getInputCaptureDriver_4
//  127 		return &ICUD2;
        LDR.N    R0,??DataTable6_4
        B.N      ??getInputCaptureDriver_1
//  128 	}
//  129 #endif
//  130 #if STM32_ICU_USE_TIM3
//  131 	if (hwPin == GPIOC_6) {
??getInputCaptureDriver_4:
        CMP      R1,#+38
        BNE.N    ??getInputCaptureDriver_5
//  132 		return &ICUD3;
        LDR.N    R0,??DataTable6_5
        B.N      ??getInputCaptureDriver_1
//  133 	}
//  134 #endif
//  135 #if STM32_ICU_USE_TIM9
//  136 	if (hwPin == GPIOE_5) {
??getInputCaptureDriver_5:
        CMP      R1,#+69
        BNE.N    ??getInputCaptureDriver_6
//  137 		return &ICUD9;
        LDR.N    R0,??DataTable6_6
        B.N      ??getInputCaptureDriver_1
//  138 	}
//  139 #endif
//  140 	return (ICUDriver *) NULL;
??getInputCaptureDriver_6:
        MOVS     R0,#+0
??getInputCaptureDriver_1:
        BX       LR               ;; return
          CFI EndBlock cfiBlock6
//  141 }
//  142 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function _Z16turnOnCapturePinPKc11brain_pin_e
        THUMB
//  143 void turnOnCapturePin(const char *msg, brain_pin_e brainPin) {
_Z16turnOnCapturePinPKc11brain_pin_e:
        PUSH     {R3-R9,LR}
          CFI R14 Frame(CFA, -4)
          CFI R9 Frame(CFA, -8)
          CFI R8 Frame(CFA, -12)
          CFI R7 Frame(CFA, -16)
          CFI R6 Frame(CFA, -20)
          CFI R5 Frame(CFA, -24)
          CFI R4 Frame(CFA, -28)
          CFI CFA R13+32
        MOVS     R4,R0
        MOVS     R5,R1
//  144 	ioportid_t port = getHwPort(brainPin);
        MOVS     R0,R5
          CFI FunCall _Z9getHwPort11brain_pin_e
        BL       _Z9getHwPort11brain_pin_e
        MOVS     R6,R0
//  145 	ioportmask_t pin = getHwPin(brainPin);
        MOVS     R0,R5
          CFI FunCall _Z8getHwPin11brain_pin_e
        BL       _Z8getHwPin11brain_pin_e
        MOVS     R7,R0
//  146 
//  147 	ICUDriver *driver = getInputCaptureDriver(brainPin);
        MOVS     R0,R5
          CFI FunCall _Z21getInputCaptureDriver11brain_pin_e
        BL       _Z21getInputCaptureDriver11brain_pin_e
        MOV      R8,R0
//  148 	if (driver != NULL) {
        CMP      R8,#+0
        BEQ.N    ??turnOnCapturePin_0
//  149 		iomode_t mode = (iomode_t) PAL_MODE_ALTERNATE(getAlternateFunctions(driver));
        MOV      R0,R8
          CFI FunCall _Z21getAlternateFunctionsP9ICUDriver
        BL       _Z21getAlternateFunctionsP9ICUDriver
        LSLS     R0,R0,#+7
        ORRS     R9,R0,#0x2
//  150 		mySetPadMode(msg, port, pin, mode);
        MOV      R3,R9
        MOVS     R2,R7
        MOVS     R1,R6
        MOVS     R0,R4
          CFI FunCall _Z12mySetPadModePKcP12GPIO_TypeDefjj
        BL       _Z12mySetPadModePKcP12GPIO_TypeDefjj
//  151 	}
//  152 }
??turnOnCapturePin_0:
        POP      {R0,R4-R9,PC}    ;; return
          CFI EndBlock cfiBlock7
//  153 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function _Z22initWaveAnalyzerDriverPKc11brain_pin_e
        THUMB
//  154 digital_input_s * initWaveAnalyzerDriver(const char *msg, brain_pin_e brainPin) {
_Z22initWaveAnalyzerDriverPKc11brain_pin_e:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
//  155 	ICUDriver *driver = getInputCaptureDriver(brainPin);
        MOVS     R0,R5
          CFI FunCall _Z21getInputCaptureDriver11brain_pin_e
        BL       _Z21getInputCaptureDriver11brain_pin_e
        MOVS     R6,R0
//  156 
//  157 	digital_input_s *hw = registeredIcus.add();
        LDR.N    R0,??DataTable6
          CFI FunCall _ZN9ArrayListI15digital_input_sLi8EE3addEv
        BL       _ZN9ArrayListI15digital_input_sLi8EE3addEv
        MOVS     R7,R0
//  158 
//  159 	hw->driver = driver;
        STR      R6,[R7, #+0]
//  160 	turnOnCapturePin(msg, brainPin);
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z16turnOnCapturePinPKc11brain_pin_e
        BL       _Z16turnOnCapturePinPKc11brain_pin_e
//  161 	return hw;
        MOVS     R0,R7
        POP      {R1,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock8
//  162 }

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function _Z16startInputDriverP15digital_input_sb
        THUMB
_Z16startInputDriverP15digital_input_sb:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        STRB     R5,[R4, #+4]
        LDRB     R0,[R4, #+4]
        CMP      R0,#+0
        BEQ.N    ??startInputDriver_0
        LDR.N    R0,??DataTable6_9
        MOVS     R1,#+0
        STRB     R1,[R0, #+0]
        B.N      ??startInputDriver_1
??startInputDriver_0:
        LDR.N    R0,??DataTable6_9
        MOVS     R1,#+1
        STRB     R1,[R0, #+0]
??startInputDriver_1:
        LDR      R6,[R4, #+0]
        CMP      R6,#+0
        BEQ.N    ??startInputDriver_2
        LDRB     R0,[R4, #+5]
        CMP      R0,#+0
        BEQ.N    ??startInputDriver_3
        MOVS     R0,R6
          CFI FunCall icuDisable
        BL       icuDisable
        MOVS     R0,R6
          CFI FunCall icuStop
        BL       icuStop
??startInputDriver_3:
        LDR.N    R1,??DataTable6_9
        MOVS     R0,R6
          CFI FunCall _Z11efiIcuStartP9ICUDriverPK9ICUConfig
        BL       _Z11efiIcuStartP9ICUDriverPK9ICUConfig
        MOVS     R0,R6
          CFI FunCall icuEnable
        BL       icuEnable
??startInputDriver_2:
        MOVS     R0,#+1
        STRB     R0,[R4, #+5]
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock9

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6:
        DC32     registeredIcus

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_1:
        DC32     `?<Constant "reader not found">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_2:
        DC32     `?<Constant "getAlternateFunctions...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_3:
        DC32     ICUD1

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_4:
        DC32     ICUD2

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_5:
        DC32     ICUD3

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_6:
        DC32     ICUD9

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_7:
        DC32     `?<Constant "No such driver">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_8:
        DC32     `?<Constant "Unexpected hw pin in ...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_9:
        DC32     wave_icucfg

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP memset
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function memset
        THUMB
// __intrinsic __nounwind __interwork __softfp void *memset(void *, int, size_t)
memset:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
        MOVS     R2,R5
        MOVS     R1,R6
        MOVS     R0,R4
          CFI FunCall __aeabi_memset
        BL       __aeabi_memset
        MOVS     R0,R4
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock10

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN15digital_input_sC1Ev
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function _ZN15digital_input_sC1Ev
        THUMB
// __code __interwork __softfp digital_input_s::<_ZN15digital_input_sC1Ev>()
_ZN15digital_input_sC1Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        ADDS     R0,R4,#+8
          CFI FunCall _ZN16IntListenerArrayC1Ev
        BL       _ZN16IntListenerArrayC1Ev
        ADDS     R0,R4,#+132
          CFI FunCall _ZN16IntListenerArrayC1Ev
        BL       _ZN16IntListenerArrayC1Ev
        MOVS     R0,R4
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock11

        SECTION `.text`:CODE:REORDER:NOROOT(2)
        SECTION_GROUP _ZN9ArrayListI15digital_input_sLi8EEC1Ev
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function _ZN9ArrayListI15digital_input_sLi8EEC1Ev
        THUMB
// __code __interwork __softfp ArrayList<digital_input_s, 8>::ArrayList()
_ZN9ArrayListI15digital_input_sLi8EEC1Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R3,#+8
        MOV      R2,#+256
        LDR.N    R1,??ArrayList_0
        ADDS     R0,R4,#+4
          CFI FunCall __aeabi_vec_ctor_nocookie_nodtor
        BL       __aeabi_vec_ctor_nocookie_nodtor
        MOV      R2,#+2048
        MOVS     R1,#+0
        ADDS     R0,R4,#+4
          CFI FunCall memset
        BL       memset
        MOVS     R0,R4
          CFI FunCall _ZN9ArrayListI15digital_input_sLi8EE5resetEv
        BL       _ZN9ArrayListI15digital_input_sLi8EE5resetEv
        MOVS     R0,R4
        POP      {R4,PC}          ;; return
        DATA
??ArrayList_0:
        DC32     _ZN15digital_input_sC1Ev
          CFI EndBlock cfiBlock12

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN9ArrayListI15digital_input_sLi8EE5resetEv
          CFI Block cfiBlock13 Using cfiCommon0
          CFI Function _ZN9ArrayListI15digital_input_sLi8EE5resetEv
          CFI NoCalls
        THUMB
// __interwork __softfp void ArrayList<digital_input_s, 8>::reset()
_ZN9ArrayListI15digital_input_sLi8EE5resetEv:
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
        BX       LR               ;; return
          CFI EndBlock cfiBlock13

        SECTION `.text`:CODE:REORDER:NOROOT(2)
        SECTION_GROUP _ZN9ArrayListI15digital_input_sLi8EE3addEv
          CFI Block cfiBlock14 Using cfiCommon0
          CFI Function _ZN9ArrayListI15digital_input_sLi8EE3addEv
        THUMB
// __interwork __softfp digital_input_s *ArrayList<digital_input_s, 8>::add()
_ZN9ArrayListI15digital_input_sLi8EE3addEv:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        LDR      R0,[R4, #+0]
        CMP      R0,#+8
        BLT.N    ??add_0
        LDR.N    R0,??add_1
          CFI FunCall firmwareError
        BL       firmwareError
        MOVS     R0,#+0
        B.N      ??add_2
??add_0:
        LDR      R0,[R4, #+0]
        ADDS     R1,R0,#+1
        STR      R1,[R4, #+0]
        MOV      R1,#+256
        MLA      R0,R1,R0,R4
        ADDS     R0,R0,#+4
??add_2:
        POP      {R4,PC}          ;; return
        Nop      
        DATA
??add_1:
        DC32     _ZZN9ArrayListI15digital_input_sLi8EE3addEvEs
          CFI EndBlock cfiBlock14

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
        SECTION_GROUP _ZZN9ArrayListI15digital_input_sLi8EE3addEvEs
// __absolute char const <_ZZN9ArrayListI15digital_input_sLi8EE3addEvEs>[24]
_ZZN9ArrayListI15digital_input_sLi8EE3addEvEs:
        DATA
        DC8 "add() too many elements"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "reader not found">`:
        DATA
        DC8 "reader not found"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "getAlternateFunctions...">`:
        DATA
        DC8 "getAlternateFunctions(NULL)"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "No such driver">`:
        DATA
        DC8 "No such driver"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Unexpected hw pin in ...">`:
        DATA
        DC8 "Unexpected hw pin in getInputCaptureChannel %d"
        DC8 0

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN9ArrayListI15digital_input_sLi8EEC2Ev
          CFI Block cfiBlock15 Using cfiCommon0
          CFI Function _ZN9ArrayListI15digital_input_sLi8EEC2Ev
        THUMB
// __code __interwork __softfp ArrayList<digital_input_s, 8>::subobject ArrayList()
_ZN9ArrayListI15digital_input_sLi8EEC2Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN9ArrayListI15digital_input_sLi8EEC1Ev
        BL       _ZN9ArrayListI15digital_input_sLi8EEC1Ev
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock15

        END
//  163 
//  164 void startInputDriver(digital_input_s *hw, bool isActiveHigh) {
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
//   136 bytes in section .rodata
//   660 bytes in section .text
// 
//   512 bytes of CODE  memory (+ 152 bytes shared)
//   112 bytes of CONST memory (+  24 bytes shared)
// 2 080 bytes of DATA  memory
//
//Errors: none
//Warnings: none
