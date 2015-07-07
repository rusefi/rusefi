///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:02 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\hw_layer\trigge /
//                       r_input.cpp                                          /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\hw_layer\trigge /
//                       r_input.cpp -lCN F:\stuff\rusefi_sourceforge\firmwar /
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
//                       -On --eec++                                          /
//    MISRA C version =  2004                                                 /
//      Enabled       =  1.2-1.4 2.3 3.4 4.2 6.5 7 8.2 8.3 9.3 12.3 13.6      /
//                       13.7 15.5 17.3 19.17 20.7-20.9 20.11                 /
//      Checked       =  1.4 2.3 4.2 6.5 7 8.2 8.3 9.3 12.3 13.7 15.5         /
//                       20.7-20.9 20.11                                      /
//      Not checked   =  1.1-1.3 1.5-2.2 2.4-4.1 5-6.4 8.1 8.4-9.2 10-12.2    /
//                       12.4-13.6 14-15.4 16-20.6 20.10 20.12 21             /
//    List file       =  F:\stuff\rusefi_sourceforge\firmware\iar\Debug\List\ /
//                       trigger_input.s                                      /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME trigger_input

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _Z10hwPortname11brain_pin_e
        EXTERN _Z11efiIcuStartP9ICUDriverPK9ICUConfig
        EXTERN _Z16turnOnCapturePinPKc11brain_pin_e
        EXTERN _Z19hwHandleShaftSignal15trigger_event_e
        EXTERN _Z21getInputCaptureDriver11brain_pin_e
        EXTERN _Z9unmarkPin11brain_pin_e
        EXTERN _engine
        EXTERN activeConfiguration
        EXTERN boardConfiguration
        EXTERN engine
        EXTERN firmwareError
        EXTERN icuDisable
        EXTERN icuEnable
        EXTERN icuStop
        EXTERN persistentState
        EXTERN scheduleMsg

        PUBLIC _Z20stopTriggerInputPinsv
        PUBLIC _Z22turnOnTriggerInputPinsP7Logging
        PUBLIC _Z24applyNewTriggerInputPinsv
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\hw_layer\trigger_input.cpp
//    1 /**
//    2  * @file	trigger_input.cpp
//    3  * @brief	Position sensor hardware layer
//    4  *
//    5  * @date Dec 30, 2012
//    6  * @author Andrey Belomutskiy, (c) 2012-2015
//    7  */
//    8 
//    9 #include "main.h"
//   10 
//   11 #if EFI_SHAFT_POSITION_INPUT || defined(__DOXYGEN__)
//   12 
//   13 #include "trigger_input.h"
//   14 #include "digital_input_hw.h"
//   15 #include "pin_repository.h"
//   16 #include "trigger_structure.h"
//   17 #include "trigger_central.h"
//   18 #include "engine_configuration.h"
//   19 
//   20 #define TRIGGER_SUPPORTED_CHANNELS 2
//   21 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   22 static ICUDriver *primaryCrankDriver;
primaryCrankDriver:
        DS8 4
//   23 
//   24 EXTERN_ENGINE
//   25 ;

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   26 static Logging *logger;
logger:
        DS8 4
//   27 
//   28 /**
//   29  * that's hardware timer input capture IRQ entry point
//   30  * 'width' events happens before the 'period' event
//   31  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _Z24shaft_icu_width_callbackP9ICUDriver
        THUMB
//   32 static void shaft_icu_width_callback(ICUDriver *icup) {
_Z24shaft_icu_width_callbackP9ICUDriver:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
//   33 // todo: support for 3rd trigger input channel
//   34 // todo: start using real event time from HW event, not just software timer?
//   35 	int isPrimary = icup == primaryCrankDriver;
        LDR.N    R0,??DataTable7
        LDR      R0,[R0, #+0]
        CMP      R4,R0
        BNE.N    ??shaft_icu_width_callback_0
        MOVS     R5,#+1
        B.N      ??shaft_icu_width_callback_1
??shaft_icu_width_callback_0:
        MOVS     R5,#+0
??shaft_icu_width_callback_1:
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
//   36 	if (!isPrimary && !engine->triggerShape.needSecondTriggerInput) {
        CMP      R5,#+0
        BNE.N    ??shaft_icu_width_callback_2
        LDR.N    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+645]
        CMP      R0,#+0
        BEQ.N    ??shaft_icu_width_callback_3
//   37 		return;
//   38 	}
//   39 	//	icucnt_t last_width = icuGetWidth(icup); so far we are fine with system time
//   40 	// todo: add support for 3rd channel
//   41 	trigger_event_e signal = isPrimary ? SHAFT_PRIMARY_UP : SHAFT_SECONDARY_UP;
??shaft_icu_width_callback_2:
        CMP      R5,#+0
        BEQ.N    ??shaft_icu_width_callback_4
        MOVS     R6,#+1
        B.N      ??shaft_icu_width_callback_5
??shaft_icu_width_callback_4:
        MOVS     R6,#+3
//   42 
//   43 	if (isLessImportant(signal) && CONFIG(useOnlyFrontForTrigger))
??shaft_icu_width_callback_5:
        LDR.N    R0,??DataTable7_2
        LDRB     R0,[R0, #+3560]
        CMP      R0,#+0
        BEQ.N    ??shaft_icu_width_callback_6
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        CMP      R6,#+1
        BNE.N    ??shaft_icu_width_callback_7
??shaft_icu_width_callback_6:
        LDR.N    R0,??DataTable7_2
        LDRB     R0,[R0, #+3560]
        CMP      R0,#+0
        BNE.N    ??shaft_icu_width_callback_8
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        CMP      R6,#+0
        BEQ.N    ??shaft_icu_width_callback_8
??shaft_icu_width_callback_7:
        LDR.N    R0,??DataTable7_3
        LDRB     R0,[R0, #+1517]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+6
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BNE.N    ??shaft_icu_width_callback_3
//   44 		return;
//   45 	hwHandleShaftSignal(signal);
??shaft_icu_width_callback_8:
        MOVS     R0,R6
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
          CFI FunCall _Z19hwHandleShaftSignal15trigger_event_e
        BL       _Z19hwHandleShaftSignal15trigger_event_e
//   46 }
??shaft_icu_width_callback_3:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock0
//   47 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z25shaft_icu_period_callbackP9ICUDriver
        THUMB
//   48 static void shaft_icu_period_callback(ICUDriver *icup) {
_Z25shaft_icu_period_callbackP9ICUDriver:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
//   49 	int isPrimary = icup == primaryCrankDriver;
        LDR.N    R0,??DataTable7
        LDR      R0,[R0, #+0]
        CMP      R4,R0
        BNE.N    ??shaft_icu_period_callback_0
        MOVS     R5,#+1
        B.N      ??shaft_icu_period_callback_1
??shaft_icu_period_callback_0:
        MOVS     R5,#+0
??shaft_icu_period_callback_1:
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
//   50 	if (!isPrimary && !engine->triggerShape.needSecondTriggerInput) {
        CMP      R5,#+0
        BNE.N    ??shaft_icu_period_callback_2
        LDR.N    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+645]
        CMP      R0,#+0
        BEQ.N    ??shaft_icu_period_callback_3
//   51 		return;
//   52 	}
//   53 
//   54 	// todo: add support for 3rd channel
//   55 	//	icucnt_t last_period = icuGetPeriod(icup); so far we are fine with system time
//   56 	trigger_event_e signal = isPrimary ? SHAFT_PRIMARY_DOWN : SHAFT_SECONDARY_DOWN;
??shaft_icu_period_callback_2:
        CMP      R5,#+0
        BEQ.N    ??shaft_icu_period_callback_4
        MOVS     R6,#+0
        B.N      ??shaft_icu_period_callback_5
??shaft_icu_period_callback_4:
        MOVS     R6,#+2
//   57 	if (isLessImportant(signal) && CONFIG(useOnlyFrontForTrigger))
??shaft_icu_period_callback_5:
        LDR.N    R0,??DataTable7_2
        LDRB     R0,[R0, #+3560]
        CMP      R0,#+0
        BEQ.N    ??shaft_icu_period_callback_6
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        CMP      R6,#+1
        BNE.N    ??shaft_icu_period_callback_7
??shaft_icu_period_callback_6:
        LDR.N    R0,??DataTable7_2
        LDRB     R0,[R0, #+3560]
        CMP      R0,#+0
        BNE.N    ??shaft_icu_period_callback_8
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        CMP      R6,#+0
        BEQ.N    ??shaft_icu_period_callback_8
??shaft_icu_period_callback_7:
        LDR.N    R0,??DataTable7_3
        LDRB     R0,[R0, #+1517]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+6
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BNE.N    ??shaft_icu_period_callback_3
//   58 		return;
//   59 	hwHandleShaftSignal(signal);
??shaft_icu_period_callback_8:
        MOVS     R0,R6
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
          CFI FunCall _Z19hwHandleShaftSignal15trigger_event_e
        BL       _Z19hwHandleShaftSignal15trigger_event_e
//   60 }
??shaft_icu_period_callback_3:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock1
//   61 
//   62 /**
//   63  * the main purpose of this configuration structure is to specify the input interrupt callbacks
//   64  */

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//   65 static ICUConfig shaft_icucfg = { ICU_INPUT_ACTIVE_LOW, 100000, /* 100kHz ICU clock frequency.   */
shaft_icucfg:
        DATA
        DC8 1, 0, 0, 0
        DC32 100000, _Z24shaft_icu_width_callbackP9ICUDriver
        DC32 _Z25shaft_icu_period_callbackP9ICUDriver
        DC8 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
//   66 shaft_icu_width_callback, shaft_icu_period_callback };
//   67 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z21turnOnTriggerInputPin11brain_pin_e
        THUMB
//   68 static ICUDriver *turnOnTriggerInputPin(brain_pin_e hwPin) {
_Z21turnOnTriggerInputPin11brain_pin_e:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//   69 	// configure pin
//   70 	turnOnCapturePin("trigger", hwPin);
        MOVS     R1,R4
        LDR.N    R0,??DataTable7_4
          CFI FunCall _Z16turnOnCapturePinPKc11brain_pin_e
        BL       _Z16turnOnCapturePinPKc11brain_pin_e
//   71 	shaft_icucfg.channel = ICU_CHANNEL_1;
        LDR.N    R0,??DataTable7_5
        MOVS     R1,#+0
        STRB     R1,[R0, #+20]
//   72 
//   73 	ICUDriver *driver = getInputCaptureDriver(hwPin);
        MOVS     R0,R4
          CFI FunCall _Z21getInputCaptureDriver11brain_pin_e
        BL       _Z21getInputCaptureDriver11brain_pin_e
        MOVS     R5,R0
//   74 	scheduleMsg(logger, "turnOnTriggerInputPin %s", hwPortname(hwPin));
        MOVS     R0,R4
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R2,R0
        LDR.N    R1,??DataTable7_6
        LDR.N    R0,??DataTable7_7
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   75 	// todo: reuse 'setWaveReaderMode' method here?
//   76 	if (driver != NULL) {
        CMP      R5,#+0
        BEQ.N    ??turnOnTriggerInputPin_0
//   77 		// todo: once http://forum.chibios.org/phpbb/viewtopic.php?f=16&t=1757 is fixed
//   78 //		bool_t needWidthCallback = !CONFIG(useOnlyFrontForTrigger) || TRIGGER_SHAPE(useRiseEdge);
//   79 //		shaft_icucfg.width_cb = needWidthCallback ? shaft_icu_width_callback : NULL;
//   80 
//   81 //		bool_t needPeriodCallback = !CONFIG(useOnlyFrontForTrigger) || !TRIGGER_SHAPE(useRiseEdge);
//   82 //		shaft_icucfg.period_cb = needPeriodCallback ? shaft_icu_period_callback : NULL;
//   83 
//   84 		efiIcuStart(driver, &shaft_icucfg);
        LDR.N    R1,??DataTable7_5
        MOVS     R0,R5
          CFI FunCall _Z11efiIcuStartP9ICUDriverPK9ICUConfig
        BL       _Z11efiIcuStartP9ICUDriverPK9ICUConfig
//   85 		if (driver->state == ICU_READY) {
        LDRB     R0,[R5, #+0]
        CMP      R0,#+2
        BNE.N    ??turnOnTriggerInputPin_1
//   86 			icuEnable(driver);
        MOVS     R0,R5
          CFI FunCall icuEnable
        BL       icuEnable
        B.N      ??turnOnTriggerInputPin_0
//   87 		} else {
//   88 			// we would be here for example if same pin is used for multiple input capture purposes
//   89 			firmwareError("ICU unexpected state [%s]", hwPortname(hwPin));
??turnOnTriggerInputPin_1:
        MOVS     R0,R4
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R1,R0
        LDR.N    R0,??DataTable7_8
          CFI FunCall firmwareError
        BL       firmwareError
//   90 		}
//   91 	}
//   92 	return driver;
??turnOnTriggerInputPin_0:
        MOVS     R0,R5
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock2
//   93 }
//   94 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z22turnOffTriggerInputPin11brain_pin_e
        THUMB
//   95 static void turnOffTriggerInputPin(brain_pin_e hwPin) {
_Z22turnOffTriggerInputPin11brain_pin_e:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//   96 	ICUDriver *driver = getInputCaptureDriver(hwPin);
        MOVS     R0,R4
          CFI FunCall _Z21getInputCaptureDriver11brain_pin_e
        BL       _Z21getInputCaptureDriver11brain_pin_e
        MOVS     R5,R0
//   97 	if (driver != NULL) {
        CMP      R5,#+0
        BEQ.N    ??turnOffTriggerInputPin_0
//   98 		icuDisable(driver);
        MOVS     R0,R5
          CFI FunCall icuDisable
        BL       icuDisable
//   99 		icuStop(driver);
        MOVS     R0,R5
          CFI FunCall icuStop
        BL       icuStop
//  100 		scheduleMsg(logger, "turnOffTriggerInputPin %s", hwPortname(hwPin));
        MOVS     R0,R4
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R2,R0
        LDR.N    R1,??DataTable7_9
        LDR.N    R0,??DataTable7_7
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  101 		unmarkPin(hwPin);
        MOVS     R0,R4
          CFI FunCall _Z9unmarkPin11brain_pin_e
        BL       _Z9unmarkPin11brain_pin_e
//  102 	}
//  103 }
??turnOffTriggerInputPin_0:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock3
//  104 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _Z22rememberPrimaryChannelv
        THUMB
//  105 static void rememberPrimaryChannel(void) {
_Z22rememberPrimaryChannelv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  106 	primaryCrankDriver = getInputCaptureDriver(boardConfiguration->triggerInputPins[0]);
        LDR.N    R0,??DataTable7_10
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+296]
          CFI FunCall _Z21getInputCaptureDriver11brain_pin_e
        BL       _Z21getInputCaptureDriver11brain_pin_e
        LDR.N    R1,??DataTable7
        STR      R0,[R1, #+0]
//  107 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock4
//  108 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _Z22turnOnTriggerInputPinsP7Logging
        THUMB
//  109 void turnOnTriggerInputPins(Logging *sharedLogger) {
_Z22turnOnTriggerInputPinsP7Logging:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  110 	logger = sharedLogger;
        LDR.N    R0,??DataTable7_7
        STR      R4,[R0, #+0]
//  111 	for (int i = 0; i < TRIGGER_SUPPORTED_CHANNELS; i++) {
        MOVS     R5,#+0
??turnOnTriggerInputPins_0:
        CMP      R5,#+2
        BGE.N    ??turnOnTriggerInputPins_1
//  112 		turnOnTriggerInputPin(boardConfiguration->triggerInputPins[i]);
        LDR.N    R0,??DataTable7_10
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,R5, LSL #+2
        LDR      R0,[R0, #+296]
          CFI FunCall _Z21turnOnTriggerInputPin11brain_pin_e
        BL       _Z21turnOnTriggerInputPin11brain_pin_e
//  113 	}
        ADDS     R5,R5,#+1
        B.N      ??turnOnTriggerInputPins_0
//  114 
//  115 	rememberPrimaryChannel();
??turnOnTriggerInputPins_1:
          CFI FunCall _Z22rememberPrimaryChannelv
        BL       _Z22rememberPrimaryChannelv
//  116 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock5
//  117 
//  118 extern engine_configuration_s activeConfiguration;
//  119 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _Z20stopTriggerInputPinsv
        THUMB
//  120 void stopTriggerInputPins(void) {
_Z20stopTriggerInputPinsv:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//  121 for (int i = 0; i < TRIGGER_SUPPORTED_CHANNELS; i++) {
        MOVS     R4,#+0
??stopTriggerInputPins_0:
        CMP      R4,#+2
        BGE.N    ??stopTriggerInputPins_1
//  122 	if (boardConfiguration->triggerInputPins[i] != activeConfiguration.bc.triggerInputPins[i]) {
        LDR.N    R0,??DataTable7_10
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,R4, LSL #+2
        LDR      R0,[R0, #+296]
        LDR.N    R1,??DataTable7_11
        ADDS     R1,R1,R4, LSL #+2
        LDR      R1,[R1, #+920]
        CMP      R0,R1
        BEQ.N    ??stopTriggerInputPins_2
//  123 		turnOffTriggerInputPin(activeConfiguration.bc.triggerInputPins[i]);
        LDR.N    R0,??DataTable7_11
        ADDS     R0,R0,R4, LSL #+2
        LDR      R0,[R0, #+920]
          CFI FunCall _Z22turnOffTriggerInputPin11brain_pin_e
        BL       _Z22turnOffTriggerInputPin11brain_pin_e
//  124 	}
//  125 }
??stopTriggerInputPins_2:
        ADDS     R4,R4,#+1
        B.N      ??stopTriggerInputPins_0
//  126 }
??stopTriggerInputPins_1:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock6
//  127 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function _Z24applyNewTriggerInputPinsv
        THUMB
//  128 void applyNewTriggerInputPins(void) {
_Z24applyNewTriggerInputPinsv:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//  129 // first we will turn off all the changed pins
//  130 stopTriggerInputPins();
          CFI FunCall _Z20stopTriggerInputPinsv
        BL       _Z20stopTriggerInputPinsv
//  131 
//  132 // then we will enable all the changed pins
//  133 for (int i = 0; i < TRIGGER_SUPPORTED_CHANNELS; i++) {
        MOVS     R4,#+0
??applyNewTriggerInputPins_0:
        CMP      R4,#+2
        BGE.N    ??applyNewTriggerInputPins_1
//  134 	if (boardConfiguration->triggerInputPins[i] != activeConfiguration.bc.triggerInputPins[i]) {
        LDR.N    R0,??DataTable7_10
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,R4, LSL #+2
        LDR      R0,[R0, #+296]
        LDR.N    R1,??DataTable7_11
        ADDS     R1,R1,R4, LSL #+2
        LDR      R1,[R1, #+920]
        CMP      R0,R1
        BEQ.N    ??applyNewTriggerInputPins_2
//  135 		turnOnTriggerInputPin(boardConfiguration->triggerInputPins[i]);
        LDR.N    R0,??DataTable7_10
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,R4, LSL #+2
        LDR      R0,[R0, #+296]
          CFI FunCall _Z21turnOnTriggerInputPin11brain_pin_e
        BL       _Z21turnOnTriggerInputPin11brain_pin_e
//  136 	}
//  137 }
??applyNewTriggerInputPins_2:
        ADDS     R4,R4,#+1
        B.N      ??applyNewTriggerInputPins_0
//  138 
//  139 rememberPrimaryChannel();
??applyNewTriggerInputPins_1:
          CFI FunCall _Z22rememberPrimaryChannelv
        BL       _Z22rememberPrimaryChannelv
//  140 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock7

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7:
        DC32     primaryCrankDriver

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_1:
        DC32     engine

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_2:
        DC32     _engine

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_3:
        DC32     persistentState

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_4:
        DC32     `?<Constant "trigger">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_5:
        DC32     shaft_icucfg

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_6:
        DC32     `?<Constant "turnOnTriggerInputPin %s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_7:
        DC32     logger

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_8:
        DC32     `?<Constant "ICU unexpected state ...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_9:
        DC32     `?<Constant "turnOffTriggerInputPi...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_10:
        DC32     boardConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_11:
        DC32     activeConfiguration

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "trigger">`:
        DATA
        DC8 "trigger"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "turnOnTriggerInputPin %s">`:
        DATA
        DC8 "turnOnTriggerInputPin %s"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ICU unexpected state ...">`:
        DATA
        DC8 "ICU unexpected state [%s]"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "turnOffTriggerInputPi...">`:
        DATA
        DC8 "turnOffTriggerInputPin %s"
        DC8 0, 0

        END
//  141 
//  142 #endif /* EFI_SHAFT_POSITION_INPUT */
// 
//   8 bytes in section .bss
//  28 bytes in section .data
//  92 bytes in section .rodata
// 580 bytes in section .text
// 
// 580 bytes of CODE  memory
//  92 bytes of CONST memory
//  36 bytes of DATA  memory
//
//Errors: none
//Warnings: 1
