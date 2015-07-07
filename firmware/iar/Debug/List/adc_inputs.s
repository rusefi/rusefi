///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:16 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\hw_layer\adc_in /
//                       puts.cpp                                             /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\hw_layer\adc_in /
//                       puts.cpp -lCN F:\stuff\rusefi_sourceforge\firmware\i /
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
//                       -On --eec++                                          /
//    MISRA C version =  2004                                                 /
//      Enabled       =  1.2-1.4 2.3 3.4 4.2 6.5 7 8.2 8.3 9.3 12.3 13.6      /
//                       13.7 15.5 17.3 19.17 20.7-20.9 20.11                 /
//      Checked       =  1.4 2.3 4.2 6.5 7 8.2 8.3 9.3 12.3 13.7 15.5         /
//                       20.7-20.9 20.11                                      /
//      Not checked   =  1.1-1.3 1.5-2.2 2.4-4.1 5-6.4 8.1 8.4-9.2 10-12.2    /
//                       12.4-13.6 14-15.4 16-20.6 20.10 20.12 21             /
//    List file       =  F:\stuff\rusefi_sourceforge\firmware\iar\Debug\List\ /
//                       adc_inputs.s                                         /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME adc_inputs

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1
        #define SHT_INIT_ARRAY 0xe

        EXTERN ADCD1
        EXTERN ADCD2
        EXTERN PWMD4
        EXTERN PWMD8
        EXTERN _Z12mySetPadModePKcP12GPIO_TypeDefjj
        EXTERN _Z17adc_callback_fastP9ADCDriverPtj
        EXTERN _Z22getPinNameByAdcChannel13adc_channel_ePc
        EXTERN _Z8portnameP12GPIO_TypeDef
        EXTERN _ZN7LoggingC1EPKcPci
        EXTERN __aeabi_f2d
        EXTERN __aeabi_memset
        EXTERN adcStart
        EXTERN adcStartConversionI
        EXTERN addConsoleAction
        EXTERN addConsoleActionI
        EXTERN appendMsgPostfix
        EXTERN appendMsgPrefix
        EXTERN appendPrintf
        EXTERN boardConfiguration
        EXTERN dbg_check_lock_from_isr
        EXTERN dbg_check_unlock_from_isr
        EXTERN engineConfiguration
        EXTERN firmwareError
        EXTERN getRemainingStack
        EXTERN print
        EXTERN printMsg
        EXTERN pwmStart
        EXTERN rlist
        EXTERN scheduleLogging
        EXTERN scheduleMsg
        EXTERN warning

        PUBLIC _Z10getAdcMode13adc_channel_e
        PUBLIC _Z13getAdcChannel11brain_pin_e
        PUBLIC _Z13initAdcInputsb
        PUBLIC _Z13pokeAdcInputsv
        PUBLIC _Z16getAdcChannelPin13adc_channel_e
        PUBLIC _Z17getAdcChannelPort13adc_channel_e
        PUBLIC _Z19getInternalAdcValuePKc13adc_channel_e
        PUBLIC _ZN9AdcDevice10addChannelE13adc_channel_e
        PUBLIC _ZN9AdcDevice18getAdcValueByIndexEi
        PUBLIC _ZN9AdcDevice22getAdcValueByHwChannelEi
        PUBLIC _ZN9AdcDevice34getAdcHardwareIndexByInternalIndexEi
        PUBLIC _ZN9AdcDevice4initEv
        PUBLIC _ZN9AdcDevice4sizeEv
        PUBLIC _ZN9AdcDevice8isHwUsedE13adc_channel_e
        PUBLIC _ZN9AdcDeviceC1EP18ADCConversionGroup
        PUBLIC _ZN9AdcDeviceC2EP18ADCConversionGroup
        PUBLIC fastAdc
        PUBLIC memset
        PUBLIC slowAdc
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\hw_layer\adc_inputs.cpp
//    1 /**
//    2  * @file	adc_inputs.cpp
//    3  * @brief	Low level ADC code
//    4  *
//    5  * @date Jan 14, 2013
//    6  * @author Andrey Belomutskiy, (c) 2012-2015
//    7  */
//    8 
//    9 #include "main.h"
//   10 
//   11 #if HAL_USE_ADC || defined(__DOXYGEN__)
//   12 
//   13 #include "engine_configuration.h"
//   14 #include "adc_inputs.h"
//   15 #include "AdcConfiguration.h"
//   16 #include "mpu_util.h"
//   17 
//   18 #include "pin_repository.h"
//   19 #include "engine_math.h"
//   20 #include "board_test.h"
//   21 #include "engine_controller.h"
//   22 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   23 static adc_channel_mode_e adcHwChannelEnabled[HW_MAX_ADC_INDEX];
adcHwChannelEnabled:
        DS8 64

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   24 static const char * adcHwChannelUsage[HW_MAX_ADC_INDEX];
adcHwChannelUsage:
        DS8 64
//   25 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _ZN9AdcDeviceC2EP18ADCConversionGroup
        THUMB
// __code __interwork __softfp AdcDevice::subobject AdcDevice(ADCConversionGroup *)
_ZN9AdcDeviceC2EP18ADCConversionGroup:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN9AdcDeviceC1EP18ADCConversionGroup
        BL       _ZN9AdcDeviceC1EP18ADCConversionGroup
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock0

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
LOGGING_BUFFER:
        DS8 500

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function __sti__routine
        THUMB
// static __intrinsic __interwork __softfp void __sti__routine()
__sti__routine:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        MOV      R3,#+500
        LDR.W    R2,??DataTable17_10
        ADR.N    R1,??DataTable4  ;; "ADC"
        LDR.W    R0,??DataTable17_11
          CFI FunCall _ZN7LoggingC1EPKcPci
        BL       _ZN7LoggingC1EPKcPci
        LDR.W    R1,??DataTable17_12
        LDR.W    R0,??DataTable17_13
          CFI FunCall _ZN9AdcDeviceC1EP18ADCConversionGroup
        BL       _ZN9AdcDeviceC1EP18ADCConversionGroup
        LDR.W    R1,??DataTable17_14
        LDR.W    R0,??DataTable17_15
          CFI FunCall _ZN9AdcDeviceC1EP18ADCConversionGroup
        BL       _ZN9AdcDeviceC1EP18ADCConversionGroup
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock1

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _ZN9AdcDeviceC1EP18ADCConversionGroup
        THUMB
//   26 AdcDevice::AdcDevice(ADCConversionGroup* hwConfig) {
_ZN9AdcDeviceC1EP18ADCConversionGroup:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//   27 	this->hwConfig = hwConfig;
        STR      R5,[R4, #+156]
//   28 	channelCount = 0;
        MOVS     R0,#+0
        STR      R0,[R4, #+152]
//   29 	conversionCount = 0;
        MOVS     R0,#+0
        STR      R0,[R4, #+80]
//   30 	errorsCount = 0;
        MOVS     R0,#+0
        STR      R0,[R4, #+84]
//   31 
//   32 	hwConfig->sqr1 = 0;
        MOVS     R0,#+0
        STR      R0,[R5, #+28]
//   33 	hwConfig->sqr2 = 0;
        MOVS     R0,#+0
        STR      R0,[R5, #+32]
//   34 	hwConfig->sqr3 = 0;
        MOVS     R0,#+0
        STR      R0,[R5, #+36]
//   35 	memset(hardwareIndexByIndernalAdcIndex, 0, sizeof(hardwareIndexByIndernalAdcIndex));
        MOVS     R2,#+80
        MOVS     R1,#+0
        ADDS     R0,R4,#+160
          CFI FunCall memset
        BL       memset
//   36 	memset(internalAdcIndexByHardwareIndex, 0xFFFFFFFF, sizeof(internalAdcIndexByHardwareIndex));
        MOVS     R2,#+80
        MOVS     R1,#-1
        MOVS     R0,R4
          CFI FunCall memset
        BL       memset
//   37 }
        MOVS     R0,R4
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock2
//   38 
//   39 // todo: migrate from hardware timer to software ADC conversion triggering
//   40 // todo: I guess we would have to use ChibiOS timer and not our own timer because
//   41 // todo: adcStartConversionI requires OS lock. currently slow ADC is 10Hz (?)
//   42 #define PWM_FREQ_SLOW 5000   /* PWM clock frequency. I wonder what does this setting mean?  */
//   43 #define PWM_PERIOD_SLOW 500  /* PWM period (in PWM ticks).    */
//   44 
//   45 /**
//   46  * 8000 RPM is 133Hz
//   47  * If we want to sample MAP once per 5 degrees we need 133Hz * (360 / 5) = 9576Hz of fast ADC
//   48  */
//   49 // todo: migrate to continues ADC mode? probably not - we cannot afford the callback in
//   50 // todo: continues mode. todo: look into our options
//   51 #define PWM_FREQ_FAST 100000   /* PWM clock frequency. I wonder what does this setting mean?  */
//   52 #define PWM_PERIOD_FAST 10  /* PWM period (in PWM ticks).    */
//   53 
//   54 // is there a reason to have this configurable?
//   55 #define ADC_SLOW_DEVICE ADCD1
//   56 
//   57 // is there a reason to have this configurable?
//   58 #define ADC_FAST_DEVICE ADCD2
//   59 
//   60 static char LOGGING_BUFFER[500];

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   61 static Logging logger("ADC", LOGGING_BUFFER, sizeof(LOGGING_BUFFER));
logger:
        DS8 60

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   62 static int adcCallbackCounter_slow = 0;
adcCallbackCounter_slow:
        DS8 4
//   63 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   64 static int adcDebugReporting = FALSE;
adcDebugReporting:
        DS8 4
//   65 
//   66 extern engine_configuration_s *engineConfiguration;
//   67 extern board_configuration_s *boardConfiguration;
//   68 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z14getAvgAdcValueiPtii
          CFI NoCalls
        THUMB
//   69 static adcsample_t getAvgAdcValue(int index, adcsample_t *samples, int bufDepth, int numChannels) {
_Z14getAvgAdcValueiPtii:
        PUSH     {R4-R6}
          CFI R6 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+12
        MOVS     R4,R0
//   70 	adcsample_t result = 0;
        MOVS     R0,#+0
//   71 	int i;
//   72 	for (i = 0; i < bufDepth; i++) {
        MOVS     R6,#+0
        MOVS     R5,R6
??getAvgAdcValue_0:
        CMP      R5,R2
        BGE.N    ??getAvgAdcValue_1
//   73 		result += samples[index];
        LDRH     R6,[R1, R4, LSL #+1]
        ADDS     R0,R6,R0
//   74 		index += numChannels;
        ADDS     R4,R3,R4
//   75 	}
        ADDS     R5,R5,#+1
        B.N      ??getAvgAdcValue_0
//   76 	return result / bufDepth;
??getAvgAdcValue_1:
        UXTH     R0,R0            ;; ZeroExt  R0,R0,#+16,#+16
        SDIV     R0,R0,R2
        UXTH     R0,R0            ;; ZeroExt  R0,R0,#+16,#+16
        POP      {R4-R6}
          CFI R4 SameValue
          CFI R5 SameValue
          CFI R6 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock3
//   77 }
//   78 
//   79 static void adc_callback_slow(ADCDriver *adcp, adcsample_t *buffer, size_t n);
//   80 
//   81 #define MY_SAMPLING_SLOW ADC_SAMPLE_480
//   82 #define MY_SAMPLING_FAST ADC_SAMPLE_28
//   83 
//   84 /*
//   85  * ADC conversion group.
//   86  */

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//   87 static ADCConversionGroup adcgrpcfgSlow = { FALSE, 0, adc_callback_slow, NULL,
adcgrpcfgSlow:
        DATA
        DC8 0, 0
        DC16 0
        DC32 _Z17adc_callback_slowP9ADCDriverPtj, 0H, 3840, 1073741824, 262143
        DC32 1073741375, 0, 0, 0
//   88 /* HW dependent part.*/
//   89 ADC_TwoSamplingDelay_20Cycles,   // cr1
//   90 		ADC_CR2_SWSTART, // cr2
//   91 
//   92 		ADC_SMPR1_SMP_AN10(MY_SAMPLING_SLOW) |
//   93 		ADC_SMPR1_SMP_AN11(MY_SAMPLING_SLOW) |
//   94 		ADC_SMPR1_SMP_AN12(MY_SAMPLING_SLOW) |
//   95 		ADC_SMPR1_SMP_AN13(MY_SAMPLING_SLOW) |
//   96 		ADC_SMPR1_SMP_AN14(MY_SAMPLING_SLOW) |
//   97 		ADC_SMPR1_SMP_AN15(MY_SAMPLING_SLOW)
//   98 		, // sample times for channels 10...18
//   99 		ADC_SMPR2_SMP_AN0(MY_SAMPLING_SLOW) |
//  100 		ADC_SMPR2_SMP_AN1(MY_SAMPLING_SLOW) |
//  101 		ADC_SMPR2_SMP_AN3(MY_SAMPLING_SLOW) |
//  102 		ADC_SMPR2_SMP_AN4(MY_SAMPLING_SLOW) |
//  103 		ADC_SMPR2_SMP_AN5(MY_SAMPLING_SLOW) |
//  104 		ADC_SMPR2_SMP_AN6(MY_SAMPLING_SLOW) |
//  105 		ADC_SMPR2_SMP_AN7(MY_SAMPLING_SLOW) |
//  106 		ADC_SMPR2_SMP_AN8(MY_SAMPLING_SLOW) |
//  107 		ADC_SMPR2_SMP_AN9(MY_SAMPLING_SLOW)
//  108 
//  109 		, // In this field must be specified the sample times for channels 0...9
//  110 
//  111 		0, // Conversion group sequence 13...16 + sequence length
//  112 		0, // Conversion group sequence 7...12
//  113 		0  // Conversion group sequence 1...6
//  114 		};
//  115 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  116 AdcDevice slowAdc(&adcgrpcfgSlow);
slowAdc:
        DS8 240
//  117 

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//  118 static ADCConversionGroup adcgrpcfg_fast = { FALSE, 0 /* num_channels */, adc_callback_fast, NULL,
adcgrpcfg_fast:
        DATA
        DC8 0, 0
        DC16 0
        DC32 _Z17adc_callback_fastP9ADCDriverPtj, 0H, 0, 1073741824, 0, 2, 0, 0
        DC32 0
//  119 /* HW dependent part.*/
//  120 ADC_TwoSamplingDelay_5Cycles,   // cr1
//  121 		ADC_CR2_SWSTART, // cr2
//  122 
//  123 		0, // sample times for channels 10...18
//  124 		   // todo: IS SOMETHING MISSING HERE?
//  125 		ADC_SMPR2_SMP_AN0(MY_SAMPLING_FAST), // In this field must be specified the sample times for channels 0...9
//  126 
//  127 		0, // Conversion group sequence 13...16 + sequence length
//  128 
//  129 		0, // Conversion group sequence 7...12
//  130 		0
//  131 
//  132 // Conversion group sequence 1...6
//  133 		};
//  134 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  135 AdcDevice fastAdc(&adcgrpcfg_fast);
fastAdc:
        DS8 240
//  136 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _Z11pwmpcb_slowP9PWMDriver
        THUMB
//  137 static void pwmpcb_slow(PWMDriver *pwmp) {
_Z11pwmpcb_slowP9PWMDriver:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  138 	efiAssertVoid(getRemainingStack(chThdSelf())> 32, "lwStAdcSlow");
        LDR.W    R0,??DataTable17_16
        LDR      R0,[R0, #+24]
          CFI FunCall getRemainingStack
        BL       getRemainingStack
        CMP      R0,#+33
        BGE.N    ??pwmpcb_slow_0
        LDR.W    R0,??DataTable17_17
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??pwmpcb_slow_1
//  139 
//  140 #if EFI_INTERNAL_ADC
//  141 	(void) pwmp;
//  142 
//  143 	/* Starts an asynchronous ADC conversion operation, the conversion
//  144 	 will be executed in parallel to the current PWM cycle and will
//  145 	 terminate before the next PWM cycle.*/
//  146 	slowAdc.conversionCount++;
??pwmpcb_slow_0:
        LDR.W    R0,??DataTable17_13
        LDR      R0,[R0, #+80]
        ADDS     R0,R0,#+1
        LDR.W    R1,??DataTable17_13
        STR      R0,[R1, #+80]
//  147 	chSysLockFromIsr()
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock_from_isr
        BL       dbg_check_lock_from_isr
//  148 	;
//  149 	if (ADC_SLOW_DEVICE.state != ADC_READY &&
//  150 	ADC_SLOW_DEVICE.state != ADC_COMPLETE &&
//  151 	ADC_SLOW_DEVICE.state != ADC_ERROR) {
        LDR.W    R0,??DataTable17_18
        LDRB     R0,[R0, #+0]
        CMP      R0,#+2
        BEQ.N    ??pwmpcb_slow_2
        LDR.W    R0,??DataTable17_18
        LDRB     R0,[R0, #+0]
        CMP      R0,#+4
        BEQ.N    ??pwmpcb_slow_2
        LDR.W    R0,??DataTable17_18
        LDRB     R0,[R0, #+0]
        CMP      R0,#+5
        BEQ.N    ??pwmpcb_slow_2
//  152 		// todo: why and when does this happen? firmwareError("ADC slow not ready?");
//  153 		slowAdc.errorsCount++;
        LDR.W    R0,??DataTable17_13
        LDR      R0,[R0, #+84]
        ADDS     R0,R0,#+1
        LDR.W    R1,??DataTable17_13
        STR      R0,[R1, #+84]
//  154 		chSysUnlockFromIsr()
          CFI FunCall dbg_check_unlock_from_isr
        BL       dbg_check_unlock_from_isr
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  155 		;
//  156 		return;
        B.N      ??pwmpcb_slow_1
//  157 	}
//  158 	adcStartConversionI(&ADC_SLOW_DEVICE, &adcgrpcfgSlow, slowAdc.samples, ADC_BUF_DEPTH_SLOW);
??pwmpcb_slow_2:
        MOVS     R3,#+1
        LDR.W    R2,??DataTable17_19
        LDR.W    R1,??DataTable17_12
        LDR.W    R0,??DataTable17_18
          CFI FunCall adcStartConversionI
        BL       adcStartConversionI
//  159 	chSysUnlockFromIsr()
          CFI FunCall dbg_check_unlock_from_isr
        BL       dbg_check_unlock_from_isr
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  160 	;
//  161 #endif
//  162 }
??pwmpcb_slow_1:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock4
//  163 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _Z11pwmpcb_fastP9PWMDriver
        THUMB
//  164 static void pwmpcb_fast(PWMDriver *pwmp) {
_Z11pwmpcb_fastP9PWMDriver:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  165 	efiAssertVoid(getRemainingStack(chThdSelf())> 32, "lwStAdcFast");
        LDR.W    R0,??DataTable17_16
        LDR      R0,[R0, #+24]
          CFI FunCall getRemainingStack
        BL       getRemainingStack
        CMP      R0,#+33
        BGE.N    ??pwmpcb_fast_0
        LDR.W    R0,??DataTable17_20
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??pwmpcb_fast_1
//  166 #if EFI_INTERNAL_ADC
//  167 	(void) pwmp;
//  168 
//  169 	/*
//  170 	 * Starts an asynchronous ADC conversion operation, the conversion
//  171 	 * will be executed in parallel to the current PWM cycle and will
//  172 	 * terminate before the next PWM cycle.
//  173 	 */
//  174 	chSysLockFromIsr()
??pwmpcb_fast_0:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock_from_isr
        BL       dbg_check_lock_from_isr
//  175 	;
//  176 	if (ADC_FAST_DEVICE.state != ADC_READY &&
//  177 	ADC_FAST_DEVICE.state != ADC_COMPLETE &&
//  178 	ADC_FAST_DEVICE.state != ADC_ERROR) {
        LDR.W    R0,??DataTable17_21
        LDRB     R0,[R0, #+0]
        CMP      R0,#+2
        BEQ.N    ??pwmpcb_fast_2
        LDR.W    R0,??DataTable17_21
        LDRB     R0,[R0, #+0]
        CMP      R0,#+4
        BEQ.N    ??pwmpcb_fast_2
        LDR.W    R0,??DataTable17_21
        LDRB     R0,[R0, #+0]
        CMP      R0,#+5
        BEQ.N    ??pwmpcb_fast_2
//  179 		fastAdc.errorsCount++;
        LDR.W    R0,??DataTable17_15
        LDR      R0,[R0, #+84]
        ADDS     R0,R0,#+1
        LDR.W    R1,??DataTable17_15
        STR      R0,[R1, #+84]
//  180 		// todo: when? why? firmwareError("ADC fast not ready?");
//  181 		chSysUnlockFromIsr()
          CFI FunCall dbg_check_unlock_from_isr
        BL       dbg_check_unlock_from_isr
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  182 		;
//  183 		return;
        B.N      ??pwmpcb_fast_1
//  184 	}
//  185 	adcStartConversionI(&ADC_FAST_DEVICE, &adcgrpcfg_fast, fastAdc.samples, ADC_BUF_DEPTH_FAST);
??pwmpcb_fast_2:
        MOVS     R3,#+1
        LDR.W    R2,??DataTable17_22
        LDR.W    R1,??DataTable17_14
        LDR.W    R0,??DataTable17_21
          CFI FunCall adcStartConversionI
        BL       adcStartConversionI
//  186 	chSysUnlockFromIsr()
          CFI FunCall dbg_check_unlock_from_isr
        BL       dbg_check_unlock_from_isr
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  187 	;
//  188 	fastAdc.conversionCount++;
        LDR.W    R0,??DataTable17_15
        LDR      R0,[R0, #+80]
        ADDS     R0,R0,#+1
        LDR.W    R1,??DataTable17_15
        STR      R0,[R1, #+80]
//  189 #endif
//  190 }
??pwmpcb_fast_1:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock5
//  191 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _Z19getInternalAdcValuePKc13adc_channel_e
        THUMB
//  192 int getInternalAdcValue(const char *msg, adc_channel_e hwChannel) {
_Z19getInternalAdcValuePKc13adc_channel_e:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  193 	if (hwChannel == EFI_ADC_NONE) {
        CMP      R5,#+16
        BNE.N    ??getInternalAdcValue_0
//  194 		warning(OBD_PCM_Processor_Fault, "ADC: should not be asking for NONE %s", msg);
        MOVS     R2,R4
        LDR.W    R1,??DataTable17_23
        MOVW     R0,#+606
          CFI FunCall warning
        BL       warning
//  195 		return -1;
        MOVS     R0,#-1
        B.N      ??getInternalAdcValue_1
//  196 	}
//  197 
//  198 	if (adcHwChannelEnabled[hwChannel] == ADC_FAST) {
??getInternalAdcValue_0:
        LDR.W    R0,??DataTable17_24
        LDR      R0,[R0, R5, LSL #+2]
        CMP      R0,#+2
        BNE.N    ??getInternalAdcValue_2
//  199 		int internalIndex = fastAdc.internalAdcIndexByHardwareIndex[hwChannel];
        LDR.W    R0,??DataTable17_15
        LDR      R0,[R0, R5, LSL #+2]
//  200 		return fastAdc.samples[internalIndex];
        LDR.W    R1,??DataTable17_15
        ADDS     R0,R1,R0, LSL #+1
        LDRH     R0,[R0, #+88]
        B.N      ??getInternalAdcValue_1
//  201 	}
//  202 	if (adcHwChannelEnabled[hwChannel] != ADC_SLOW) {
??getInternalAdcValue_2:
        LDR.W    R0,??DataTable17_24
        LDR      R0,[R0, R5, LSL #+2]
        CMP      R0,#+1
        BEQ.N    ??getInternalAdcValue_3
//  203 		warning(OBD_PCM_Processor_Fault, "ADC is off [%s] index=%d", msg, hwChannel);
        MOVS     R3,R5
        MOVS     R2,R4
        LDR.W    R1,??DataTable17_25
        MOVW     R0,#+606
          CFI FunCall warning
        BL       warning
//  204 	}
//  205 
//  206 	return slowAdc.getAdcValueByHwChannel(hwChannel);
??getInternalAdcValue_3:
        MOVS     R1,R5
        LDR.W    R0,??DataTable17_13
          CFI FunCall _ZN9AdcDevice22getAdcValueByHwChannelEi
        BL       _ZN9AdcDevice22getAdcValueByHwChannelEi
??getInternalAdcValue_1:
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock6
//  207 }
//  208 

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//  209 static PWMConfig pwmcfg_slow = { PWM_FREQ_SLOW, PWM_PERIOD_SLOW, pwmpcb_slow, { {
pwmcfg_slow:
        DATA
        DC32 5000
        DC16 500
        DC8 0, 0
        DC32 _Z11pwmpcb_slowP9PWMDriver, 0, 0H, 0, 0H, 0, 0H, 0, 0H, 0, 0
//  210 PWM_OUTPUT_DISABLED, NULL }, { PWM_OUTPUT_DISABLED, NULL }, {
//  211 PWM_OUTPUT_DISABLED, NULL }, { PWM_OUTPUT_DISABLED, NULL } },
//  212 /* HW dependent part.*/
//  213 0, 0 };
//  214 

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//  215 static PWMConfig pwmcfg_fast = { PWM_FREQ_FAST, PWM_PERIOD_FAST, pwmpcb_fast, { {
pwmcfg_fast:
        DATA
        DC32 100000
        DC16 10
        DC8 0, 0
        DC32 _Z11pwmpcb_fastP9PWMDriver, 0, 0H, 0, 0H, 0, 0H, 0, 0H, 0, 0
//  216 PWM_OUTPUT_DISABLED, NULL }, { PWM_OUTPUT_DISABLED, NULL }, {
//  217 PWM_OUTPUT_DISABLED, NULL }, { PWM_OUTPUT_DISABLED, NULL } },
//  218 /* HW dependent part.*/
//  219 0, 0 };
//  220 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function _Z10initAdcPinP12GPIO_TypeDefiPKc
        THUMB
//  221 static void initAdcPin(ioportid_t port, int pin, const char *msg) {
_Z10initAdcPinP12GPIO_TypeDefiPKc:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//  222 	print("adc %s\r\n", msg);
        MOVS     R1,R6
        LDR.W    R0,??DataTable17_26
          CFI FunCall print
        BL       print
//  223 	mySetPadMode("adc input", port, pin, PAL_MODE_INPUT_ANALOG);
        MOVS     R3,#+3
        MOVS     R2,R5
        MOVS     R1,R4
        LDR.W    R0,??DataTable17_27
          CFI FunCall _Z12mySetPadModePKcP12GPIO_TypeDefjj
        BL       _Z12mySetPadModePKcP12GPIO_TypeDefjj
//  224 }
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock7

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4:
        DC8      "ADC"
//  225 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function _Z13getAdcChannel11brain_pin_e
          CFI NoCalls
        THUMB
//  226 adc_channel_e getAdcChannel(brain_pin_e pin) {
_Z13getAdcChannel11brain_pin_e:
        MOVS     R1,R0
//  227 	switch (pin) {
        MOVS     R0,R1
        CMP      R0,#+0
        BEQ.N    ??getAdcChannel_0
        CMP      R0,#+1
        BEQ.N    ??getAdcChannel_1
        CMP      R0,#+2
        BEQ.N    ??getAdcChannel_2
        CMP      R0,#+3
        BEQ.N    ??getAdcChannel_3
        CMP      R0,#+4
        BEQ.N    ??getAdcChannel_4
        CMP      R0,#+5
        BEQ.N    ??getAdcChannel_5
        CMP      R0,#+6
        BEQ.N    ??getAdcChannel_6
        CMP      R0,#+7
        BEQ.N    ??getAdcChannel_7
        CMP      R0,#+16
        BEQ.N    ??getAdcChannel_8
        CMP      R0,#+17
        BEQ.N    ??getAdcChannel_9
        CMP      R0,#+32
        BEQ.N    ??getAdcChannel_10
        CMP      R0,#+33
        BEQ.N    ??getAdcChannel_11
        CMP      R0,#+34
        BEQ.N    ??getAdcChannel_12
        CMP      R0,#+35
        BEQ.N    ??getAdcChannel_13
        CMP      R0,#+36
        BEQ.N    ??getAdcChannel_14
        CMP      R0,#+37
        BEQ.N    ??getAdcChannel_15
        B.N      ??getAdcChannel_16
//  228 	case GPIOA_0:
//  229 		return EFI_ADC_0;
??getAdcChannel_0:
        MOVS     R0,#+0
        B.N      ??getAdcChannel_17
//  230 	case GPIOA_1:
//  231 		return EFI_ADC_1;
??getAdcChannel_1:
        MOVS     R0,#+1
        B.N      ??getAdcChannel_17
//  232 	case GPIOA_2:
//  233 		return EFI_ADC_2;
??getAdcChannel_2:
        MOVS     R0,#+2
        B.N      ??getAdcChannel_17
//  234 	case GPIOA_3:
//  235 		return EFI_ADC_3;
??getAdcChannel_3:
        MOVS     R0,#+3
        B.N      ??getAdcChannel_17
//  236 	case GPIOA_4:
//  237 		return EFI_ADC_4;
??getAdcChannel_4:
        MOVS     R0,#+4
        B.N      ??getAdcChannel_17
//  238 	case GPIOA_5:
//  239 		return EFI_ADC_5;
??getAdcChannel_5:
        MOVS     R0,#+5
        B.N      ??getAdcChannel_17
//  240 	case GPIOA_6:
//  241 		return EFI_ADC_6;
??getAdcChannel_6:
        MOVS     R0,#+6
        B.N      ??getAdcChannel_17
//  242 	case GPIOA_7:
//  243 		return EFI_ADC_7;
??getAdcChannel_7:
        MOVS     R0,#+7
        B.N      ??getAdcChannel_17
//  244 	case GPIOB_0:
//  245 		return EFI_ADC_8;
??getAdcChannel_8:
        MOVS     R0,#+8
        B.N      ??getAdcChannel_17
//  246 	case GPIOB_1:
//  247 		return EFI_ADC_9;
??getAdcChannel_9:
        MOVS     R0,#+9
        B.N      ??getAdcChannel_17
//  248 	case GPIOC_0:
//  249 		return EFI_ADC_10;
??getAdcChannel_10:
        MOVS     R0,#+10
        B.N      ??getAdcChannel_17
//  250 	case GPIOC_1:
//  251 		return EFI_ADC_11;
??getAdcChannel_11:
        MOVS     R0,#+11
        B.N      ??getAdcChannel_17
//  252 	case GPIOC_2:
//  253 		return EFI_ADC_12;
??getAdcChannel_12:
        MOVS     R0,#+12
        B.N      ??getAdcChannel_17
//  254 	case GPIOC_3:
//  255 		return EFI_ADC_13;
??getAdcChannel_13:
        MOVS     R0,#+13
        B.N      ??getAdcChannel_17
//  256 	case GPIOC_4:
//  257 		return EFI_ADC_14;
??getAdcChannel_14:
        MOVS     R0,#+14
        B.N      ??getAdcChannel_17
//  258 	case GPIOC_5:
//  259 		return EFI_ADC_15;
??getAdcChannel_15:
        MOVS     R0,#+15
        B.N      ??getAdcChannel_17
//  260 	default:
//  261 		return EFI_ADC_ERROR;
??getAdcChannel_16:
        MOVW     R0,#+999
??getAdcChannel_17:
        BX       LR               ;; return
          CFI EndBlock cfiBlock8
//  262 	}
//  263 }
//  264 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function _Z17getAdcChannelPort13adc_channel_e
        THUMB
//  265 GPIO_TypeDef* getAdcChannelPort(adc_channel_e hwChannel) {
_Z17getAdcChannelPort13adc_channel_e:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  266 	// todo: replace this with an array :)
//  267 	switch (hwChannel) {
        CMP      R4,#+0
        BEQ.N    ??getAdcChannelPort_0
        CMP      R4,#+2
        BEQ.N    ??getAdcChannelPort_1
        BCC.N    ??getAdcChannelPort_2
        CMP      R4,#+4
        BEQ.N    ??getAdcChannelPort_3
        BCC.N    ??getAdcChannelPort_4
        CMP      R4,#+6
        BEQ.N    ??getAdcChannelPort_5
        BCC.N    ??getAdcChannelPort_6
        CMP      R4,#+8
        BEQ.N    ??getAdcChannelPort_7
        BCC.N    ??getAdcChannelPort_8
        CMP      R4,#+10
        BEQ.N    ??getAdcChannelPort_9
        BCC.N    ??getAdcChannelPort_10
        CMP      R4,#+12
        BEQ.N    ??getAdcChannelPort_11
        BCC.N    ??getAdcChannelPort_12
        CMP      R4,#+14
        BEQ.N    ??getAdcChannelPort_13
        BCC.N    ??getAdcChannelPort_14
        CMP      R4,#+15
        BEQ.N    ??getAdcChannelPort_15
        B.N      ??getAdcChannelPort_16
//  268 	case ADC_CHANNEL_IN0:
//  269 		return GPIOA;
??getAdcChannelPort_0:
        LDR.W    R0,??DataTable17_28  ;; 0x40020000
        B.N      ??getAdcChannelPort_17
//  270 	case ADC_CHANNEL_IN1:
//  271 		return GPIOA;
??getAdcChannelPort_2:
        LDR.W    R0,??DataTable17_28  ;; 0x40020000
        B.N      ??getAdcChannelPort_17
//  272 	case ADC_CHANNEL_IN2:
//  273 		return GPIOA;
??getAdcChannelPort_1:
        LDR.W    R0,??DataTable17_28  ;; 0x40020000
        B.N      ??getAdcChannelPort_17
//  274 	case ADC_CHANNEL_IN3:
//  275 		return GPIOA;
??getAdcChannelPort_4:
        LDR.W    R0,??DataTable17_28  ;; 0x40020000
        B.N      ??getAdcChannelPort_17
//  276 	case ADC_CHANNEL_IN4:
//  277 		return GPIOA;
??getAdcChannelPort_3:
        LDR.W    R0,??DataTable17_28  ;; 0x40020000
        B.N      ??getAdcChannelPort_17
//  278 	case ADC_CHANNEL_IN5:
//  279 		return GPIOA;
??getAdcChannelPort_6:
        LDR.W    R0,??DataTable17_28  ;; 0x40020000
        B.N      ??getAdcChannelPort_17
//  280 	case ADC_CHANNEL_IN6:
//  281 		return GPIOA;
??getAdcChannelPort_5:
        LDR.W    R0,??DataTable17_28  ;; 0x40020000
        B.N      ??getAdcChannelPort_17
//  282 	case ADC_CHANNEL_IN7:
//  283 		return GPIOA;
??getAdcChannelPort_8:
        LDR.W    R0,??DataTable17_28  ;; 0x40020000
        B.N      ??getAdcChannelPort_17
//  284 	case ADC_CHANNEL_IN8:
//  285 		return GPIOB;
??getAdcChannelPort_7:
        LDR.W    R0,??DataTable17_29  ;; 0x40020400
        B.N      ??getAdcChannelPort_17
//  286 	case ADC_CHANNEL_IN9:
//  287 		return GPIOB;
??getAdcChannelPort_10:
        LDR.W    R0,??DataTable17_29  ;; 0x40020400
        B.N      ??getAdcChannelPort_17
//  288 	case ADC_CHANNEL_IN10:
//  289 		return GPIOC;
??getAdcChannelPort_9:
        LDR.W    R0,??DataTable17_30  ;; 0x40020800
        B.N      ??getAdcChannelPort_17
//  290 	case ADC_CHANNEL_IN11:
//  291 		return GPIOC;
??getAdcChannelPort_12:
        LDR.W    R0,??DataTable17_30  ;; 0x40020800
        B.N      ??getAdcChannelPort_17
//  292 	case ADC_CHANNEL_IN12:
//  293 		return GPIOC;
??getAdcChannelPort_11:
        LDR.W    R0,??DataTable17_30  ;; 0x40020800
        B.N      ??getAdcChannelPort_17
//  294 	case ADC_CHANNEL_IN13:
//  295 		return GPIOC;
??getAdcChannelPort_14:
        LDR.W    R0,??DataTable17_30  ;; 0x40020800
        B.N      ??getAdcChannelPort_17
//  296 	case ADC_CHANNEL_IN14:
//  297 		return GPIOC;
??getAdcChannelPort_13:
        LDR.W    R0,??DataTable17_30  ;; 0x40020800
        B.N      ??getAdcChannelPort_17
//  298 	case ADC_CHANNEL_IN15:
//  299 		return GPIOC;
??getAdcChannelPort_15:
        LDR.W    R0,??DataTable17_30  ;; 0x40020800
        B.N      ??getAdcChannelPort_17
//  300 	default:
//  301 		firmwareError("Unknown hw channel %d", hwChannel);
??getAdcChannelPort_16:
        MOVS     R1,R4
        LDR.W    R0,??DataTable17_31
          CFI FunCall firmwareError
        BL       firmwareError
//  302 		return NULL;
        MOVS     R0,#+0
??getAdcChannelPort_17:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock9
//  303 	}
//  304 }
//  305 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function _Z10getAdcMode13adc_channel_e
        THUMB
//  306 const char * getAdcMode(adc_channel_e hwChannel) {
_Z10getAdcMode13adc_channel_e:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  307 	if (slowAdc.isHwUsed(hwChannel)) {
        MOVS     R1,R4
        LDR.W    R0,??DataTable17_13
          CFI FunCall _ZN9AdcDevice8isHwUsedE13adc_channel_e
        BL       _ZN9AdcDevice8isHwUsedE13adc_channel_e
        CMP      R0,#+0
        BEQ.N    ??getAdcMode_0
//  308 		return "slow";
        LDR.W    R0,??DataTable17_32
        B.N      ??getAdcMode_1
//  309 	}
//  310 	if (fastAdc.isHwUsed(hwChannel)) {
??getAdcMode_0:
        MOVS     R1,R4
        LDR.W    R0,??DataTable17_15
          CFI FunCall _ZN9AdcDevice8isHwUsedE13adc_channel_e
        BL       _ZN9AdcDevice8isHwUsedE13adc_channel_e
        CMP      R0,#+0
        BEQ.N    ??getAdcMode_2
//  311 		return "fast";
        LDR.W    R0,??DataTable17_33
        B.N      ??getAdcMode_1
//  312 	}
//  313 	return "INACTIVE";
??getAdcMode_2:
        LDR.W    R0,??DataTable17_34
??getAdcMode_1:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock10
//  314 }
//  315 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function _Z16getAdcChannelPin13adc_channel_e
        THUMB
//  316 int getAdcChannelPin(adc_channel_e hwChannel) {
_Z16getAdcChannelPin13adc_channel_e:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  317 	// todo: replace this with an array :)
//  318 	switch (hwChannel) {
        CMP      R4,#+0
        BEQ.N    ??getAdcChannelPin_0
        CMP      R4,#+2
        BEQ.N    ??getAdcChannelPin_1
        BCC.N    ??getAdcChannelPin_2
        CMP      R4,#+4
        BEQ.N    ??getAdcChannelPin_3
        BCC.N    ??getAdcChannelPin_4
        CMP      R4,#+6
        BEQ.N    ??getAdcChannelPin_5
        BCC.N    ??getAdcChannelPin_6
        CMP      R4,#+8
        BEQ.N    ??getAdcChannelPin_7
        BCC.N    ??getAdcChannelPin_8
        CMP      R4,#+10
        BEQ.N    ??getAdcChannelPin_9
        BCC.N    ??getAdcChannelPin_10
        CMP      R4,#+12
        BEQ.N    ??getAdcChannelPin_11
        BCC.N    ??getAdcChannelPin_12
        CMP      R4,#+14
        BEQ.N    ??getAdcChannelPin_13
        BCC.N    ??getAdcChannelPin_14
        CMP      R4,#+15
        BEQ.N    ??getAdcChannelPin_15
        B.N      ??getAdcChannelPin_16
//  319 	case ADC_CHANNEL_IN0:
//  320 		return 0;
??getAdcChannelPin_0:
        MOVS     R0,#+0
        B.N      ??getAdcChannelPin_17
//  321 	case ADC_CHANNEL_IN1:
//  322 		return 1;
??getAdcChannelPin_2:
        MOVS     R0,#+1
        B.N      ??getAdcChannelPin_17
//  323 	case ADC_CHANNEL_IN2:
//  324 		return 2;
??getAdcChannelPin_1:
        MOVS     R0,#+2
        B.N      ??getAdcChannelPin_17
//  325 	case ADC_CHANNEL_IN3:
//  326 		return 3;
??getAdcChannelPin_4:
        MOVS     R0,#+3
        B.N      ??getAdcChannelPin_17
//  327 	case ADC_CHANNEL_IN4:
//  328 		return 4;
??getAdcChannelPin_3:
        MOVS     R0,#+4
        B.N      ??getAdcChannelPin_17
//  329 	case ADC_CHANNEL_IN5:
//  330 		return 5;
??getAdcChannelPin_6:
        MOVS     R0,#+5
        B.N      ??getAdcChannelPin_17
//  331 	case ADC_CHANNEL_IN6:
//  332 		return 6;
??getAdcChannelPin_5:
        MOVS     R0,#+6
        B.N      ??getAdcChannelPin_17
//  333 	case ADC_CHANNEL_IN7:
//  334 		return 7;
??getAdcChannelPin_8:
        MOVS     R0,#+7
        B.N      ??getAdcChannelPin_17
//  335 	case ADC_CHANNEL_IN8:
//  336 		return 0;
??getAdcChannelPin_7:
        MOVS     R0,#+0
        B.N      ??getAdcChannelPin_17
//  337 	case ADC_CHANNEL_IN9:
//  338 		return 1;
??getAdcChannelPin_10:
        MOVS     R0,#+1
        B.N      ??getAdcChannelPin_17
//  339 	case ADC_CHANNEL_IN10:
//  340 		return 0;
??getAdcChannelPin_9:
        MOVS     R0,#+0
        B.N      ??getAdcChannelPin_17
//  341 	case ADC_CHANNEL_IN11:
//  342 		return 1;
??getAdcChannelPin_12:
        MOVS     R0,#+1
        B.N      ??getAdcChannelPin_17
//  343 	case ADC_CHANNEL_IN12:
//  344 		return 2;
??getAdcChannelPin_11:
        MOVS     R0,#+2
        B.N      ??getAdcChannelPin_17
//  345 	case ADC_CHANNEL_IN13:
//  346 		return 3;
??getAdcChannelPin_14:
        MOVS     R0,#+3
        B.N      ??getAdcChannelPin_17
//  347 	case ADC_CHANNEL_IN14:
//  348 		return 4;
??getAdcChannelPin_13:
        MOVS     R0,#+4
        B.N      ??getAdcChannelPin_17
//  349 	case ADC_CHANNEL_IN15:
//  350 		return 5;
??getAdcChannelPin_15:
        MOVS     R0,#+5
        B.N      ??getAdcChannelPin_17
//  351 	default:
//  352 		firmwareError("Unknown hw channel %d", hwChannel);
??getAdcChannelPin_16:
        MOVS     R1,R4
        LDR.W    R0,??DataTable17_31
          CFI FunCall firmwareError
        BL       firmwareError
//  353 		return -1;
        MOVS     R0,#-1
??getAdcChannelPin_17:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock11
//  354 	}
//  355 }
//  356 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function _Z16initAdcHwChannel13adc_channel_e
        THUMB
//  357 static void initAdcHwChannel(adc_channel_e hwChannel) {
_Z16initAdcHwChannel13adc_channel_e:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
//  358 	GPIO_TypeDef* port = getAdcChannelPort(hwChannel);
        MOVS     R0,R4
          CFI FunCall _Z17getAdcChannelPort13adc_channel_e
        BL       _Z17getAdcChannelPort13adc_channel_e
        MOVS     R5,R0
//  359 	int pin = getAdcChannelPin(hwChannel);
        MOVS     R0,R4
          CFI FunCall _Z16getAdcChannelPin13adc_channel_e
        BL       _Z16getAdcChannelPin13adc_channel_e
        MOVS     R6,R0
//  360 
//  361 	initAdcPin(port, pin, "hw");
        ADR.N    R2,??DataTable14  ;; 0x68, 0x77, 0x00, 0x00
        MOVS     R1,R6
        MOVS     R0,R5
          CFI FunCall _Z10initAdcPinP12GPIO_TypeDefiPKc
        BL       _Z10initAdcPinP12GPIO_TypeDefiPKc
//  362 }
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock12
//  363 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock13 Using cfiCommon0
          CFI Function _ZN9AdcDevice4sizeEv
          CFI NoCalls
        THUMB
//  364 int AdcDevice::size() {
//  365 	return channelCount;
_ZN9AdcDevice4sizeEv:
        LDR      R0,[R0, #+152]
        BX       LR               ;; return
          CFI EndBlock cfiBlock13
//  366 }
//  367 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock14 Using cfiCommon0
          CFI Function _ZN9AdcDevice22getAdcValueByHwChannelEi
          CFI NoCalls
        THUMB
//  368 int AdcDevice::getAdcValueByHwChannel(int hwChannel) {
//  369 	int internalIndex = internalAdcIndexByHardwareIndex[hwChannel];
_ZN9AdcDevice22getAdcValueByHwChannelEi:
        LDR      R2,[R0, R1, LSL #+2]
//  370 	return values.adc_data[internalIndex];
        ADDS     R0,R0,R2, LSL #+1
        LDRH     R0,[R0, #+120]
        BX       LR               ;; return
          CFI EndBlock cfiBlock14
//  371 }
//  372 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock15 Using cfiCommon0
          CFI Function _ZN9AdcDevice18getAdcValueByIndexEi
          CFI NoCalls
        THUMB
//  373 int AdcDevice::getAdcValueByIndex(int internalIndex) {
//  374 	return values.adc_data[internalIndex];
_ZN9AdcDevice18getAdcValueByIndexEi:
        ADDS     R0,R0,R1, LSL #+1
        LDRH     R0,[R0, #+120]
        BX       LR               ;; return
          CFI EndBlock cfiBlock15
//  375 }
//  376 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock16 Using cfiCommon0
          CFI Function _ZN9AdcDevice4initEv
        THUMB
//  377 void AdcDevice::init(void) {
_ZN9AdcDevice4initEv:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  378 	hwConfig->num_channels = size();
        MOVS     R0,R4
          CFI FunCall _ZN9AdcDevice4sizeEv
        BL       _ZN9AdcDevice4sizeEv
        LDR      R1,[R4, #+156]
        STRH     R0,[R1, #+2]
//  379 	hwConfig->sqr1 += ADC_SQR1_NUM_CH(size());
        LDR      R0,[R4, #+156]
        LDR      R5,[R0, #+28]
        MOVS     R0,R4
          CFI FunCall _ZN9AdcDevice4sizeEv
        BL       _ZN9AdcDevice4sizeEv
        SUBS     R0,R0,#+1
        ADDS     R0,R5,R0, LSL #+20
        LDR      R1,[R4, #+156]
        STR      R0,[R1, #+28]
//  380 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock16
//  381 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock17 Using cfiCommon0
          CFI Function _ZN9AdcDevice8isHwUsedE13adc_channel_e
          CFI NoCalls
        THUMB
//  382 bool AdcDevice::isHwUsed(adc_channel_e hwChannelIndex) {
_ZN9AdcDevice8isHwUsedE13adc_channel_e:
        MOVS     R2,R0
//  383 	for (int i = 0; i < channelCount; i++) {
        MOVS     R3,#+0
??isHwUsed_0:
        LDR      R0,[R2, #+152]
        CMP      R3,R0
        BGE.N    ??isHwUsed_1
//  384 		if (hardwareIndexByIndernalAdcIndex[i] == hwChannelIndex) {
        ADDS     R0,R2,R3, LSL #+2
        LDR      R0,[R0, #+160]
        CMP      R0,R1
        BNE.N    ??isHwUsed_2
//  385 			return true;
        MOVS     R0,#+1
        B.N      ??isHwUsed_3
//  386 		}
//  387 	}
??isHwUsed_2:
        ADDS     R3,R3,#+1
        B.N      ??isHwUsed_0
//  388 	return false;
??isHwUsed_1:
        MOVS     R0,#+0
??isHwUsed_3:
        BX       LR               ;; return
          CFI EndBlock cfiBlock17
//  389 }
//  390 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock18 Using cfiCommon0
          CFI Function _ZN9AdcDevice10addChannelE13adc_channel_e
        THUMB
//  391 void AdcDevice::addChannel(adc_channel_e hwChannel) {
_ZN9AdcDevice10addChannelE13adc_channel_e:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  392 	int logicChannel = channelCount++;
        LDR      R6,[R4, #+152]
        ADDS     R0,R6,#+1
        STR      R0,[R4, #+152]
//  393 
//  394 	internalAdcIndexByHardwareIndex[hwChannel] = logicChannel;
        STR      R6,[R4, R5, LSL #+2]
//  395 	hardwareIndexByIndernalAdcIndex[logicChannel] = hwChannel;
        ADDS     R0,R4,R6, LSL #+2
        STR      R5,[R0, #+160]
//  396 	if (logicChannel < 6) {
        CMP      R6,#+6
        BGE.N    ??addChannel_0
//  397 		hwConfig->sqr3 += (hwChannel) << (5 * logicChannel);
        LDR      R0,[R4, #+156]
        LDR      R0,[R0, #+36]
        MOVS     R1,#+5
        MUL      R1,R1,R6
        LSLS     R1,R5,R1
        ADDS     R0,R1,R0
        LDR      R1,[R4, #+156]
        STR      R0,[R1, #+36]
        B.N      ??addChannel_1
//  398 	} else if (logicChannel < 12) {
??addChannel_0:
        CMP      R6,#+12
        BGE.N    ??addChannel_2
//  399 		hwConfig->sqr2 += (hwChannel) << (5 * (logicChannel - 6));
        LDR      R0,[R4, #+156]
        LDR      R0,[R0, #+32]
        MOVS     R1,R6
        SUBS     R1,R1,#+6
        MOVS     R2,#+5
        SMULBB   R1,R1,R2
        LSLS     R1,R5,R1
        ADDS     R0,R1,R0
        LDR      R1,[R4, #+156]
        STR      R0,[R1, #+32]
        B.N      ??addChannel_1
//  400 	} else {
//  401 		hwConfig->sqr1 += (hwChannel) << (5 * (logicChannel - 12));
??addChannel_2:
        LDR      R0,[R4, #+156]
        LDR      R0,[R0, #+28]
        MOVS     R1,R6
        SUBS     R1,R1,#+12
        MOVS     R2,#+5
        SMULBB   R1,R1,R2
        LSLS     R1,R5,R1
        ADDS     R0,R1,R0
        LDR      R1,[R4, #+156]
        STR      R0,[R1, #+28]
//  402 	}
//  403 	// todo: support for more then 12 channels? not sure how needed it would be
//  404 
//  405 	initAdcHwChannel(hwChannel);
??addChannel_1:
        MOVS     R0,R5
          CFI FunCall _Z16initAdcHwChannel13adc_channel_e
        BL       _Z16initAdcHwChannel13adc_channel_e
//  406 }
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock18
//  407 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock19 Using cfiCommon0
          CFI Function _Z13printAdcValue13adc_channel_e
        THUMB
//  408 static void printAdcValue(adc_channel_e channel) {
_Z13printAdcValue13adc_channel_e:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
//  409 	int value = getAdcValue("print", channel);
        MOVS     R1,R4
        LDR.W    R0,??DataTable17_35
          CFI FunCall _Z19getInternalAdcValuePKc13adc_channel_e
        BL       _Z19getInternalAdcValuePKc13adc_channel_e
        VMOV     S16,R0
//  410 	float volts = adcToVoltsDivided(value);
        VCVT.F32.S32 S0,S16
        VLDR.W   S1,??DataTable14_1  ;; 0x3a400c01
        VMUL.F32 S0,S0,S1
        LDR.W    R0,??DataTable17_36
        LDR      R0,[R0, #+0]
        VLDR     S1,[R0, #+464]
        VMUL.F32 S17,S0,S1
//  411 	scheduleMsg(&logger, "adc voltage : %f", volts);
        VMOV     R0,S17
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.W    R1,??DataTable17_37
        LDR.N    R0,??DataTable17_11
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  412 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock19
//  413 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock20 Using cfiCommon0
          CFI Function _ZN9AdcDevice34getAdcHardwareIndexByInternalIndexEi
          CFI NoCalls
        THUMB
//  414 adc_channel_e AdcDevice::getAdcHardwareIndexByInternalIndex(int index) {
//  415 	return hardwareIndexByIndernalAdcIndex[index];
_ZN9AdcDevice34getAdcHardwareIndexByInternalIndexEi:
        ADDS     R0,R0,R1, LSL #+2
        LDR      R0,[R0, #+160]
        BX       LR               ;; return
          CFI EndBlock cfiBlock20
//  416 }
//  417 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock21 Using cfiCommon0
          CFI Function _Z18printFullAdcReportv
        THUMB
//  418 static void printFullAdcReport(void) {
_Z18printFullAdcReportv:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        VPUSH    {D8}
          CFI D8 Frame(CFA, -32)
          CFI CFA R13+32
        SUB      SP,SP,#+8
          CFI CFA R13+40
//  419 	scheduleMsg(&logger, "fast %d slow %d", fastAdc.conversionCount, slowAdc.conversionCount);
        LDR.N    R0,??DataTable17_13
        LDR      R3,[R0, #+80]
        LDR.N    R0,??DataTable17_15
        LDR      R2,[R0, #+80]
        LDR.W    R1,??DataTable17_38
        LDR.N    R0,??DataTable17_11
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  420 
//  421 	for (int index = 0; index < slowAdc.size(); index++) {
        MOVS     R4,#+0
??printFullAdcReport_0:
        LDR.N    R0,??DataTable17_13
          CFI FunCall _ZN9AdcDevice4sizeEv
        BL       _ZN9AdcDevice4sizeEv
        CMP      R4,R0
        BGE.N    ??printFullAdcReport_1
//  422 		appendMsgPrefix(&logger);
        LDR.N    R0,??DataTable17_11
          CFI FunCall appendMsgPrefix
        BL       appendMsgPrefix
//  423 
//  424 		adc_channel_e hwIndex = slowAdc.getAdcHardwareIndexByInternalIndex(index);
        MOVS     R1,R4
        LDR.N    R0,??DataTable17_13
          CFI FunCall _ZN9AdcDevice34getAdcHardwareIndexByInternalIndexEi
        BL       _ZN9AdcDevice34getAdcHardwareIndexByInternalIndexEi
        MOVS     R5,R0
//  425 		GPIO_TypeDef* port = getAdcChannelPort(hwIndex);
        MOVS     R0,R5
          CFI FunCall _Z17getAdcChannelPort13adc_channel_e
        BL       _Z17getAdcChannelPort13adc_channel_e
        MOVS     R6,R0
//  426 		int pin = getAdcChannelPin(hwIndex);
        MOVS     R0,R5
          CFI FunCall _Z16getAdcChannelPin13adc_channel_e
        BL       _Z16getAdcChannelPin13adc_channel_e
        MOVS     R7,R0
//  427 
//  428 		int adcValue = slowAdc.getAdcValueByIndex(index);
        MOVS     R1,R4
        LDR.N    R0,??DataTable17_13
          CFI FunCall _ZN9AdcDevice18getAdcValueByIndexEi
        BL       _ZN9AdcDevice18getAdcValueByIndexEi
        VMOV     S16,R0
//  429 		appendPrintf(&logger, " ch%d %s%d", index, portname(port), pin);
        MOVS     R0,R6
          CFI FunCall _Z8portnameP12GPIO_TypeDef
        BL       _Z8portnameP12GPIO_TypeDef
        MOVS     R3,R0
        STR      R7,[SP, #+0]
        MOVS     R2,R4
        LDR.N    R1,??DataTable17_39
        LDR.N    R0,??DataTable17_11
          CFI FunCall appendPrintf
        BL       appendPrintf
//  430 		appendPrintf(&logger, " ADC%d 12bit=%d", hwIndex, adcValue);
        VMOV     R3,S16
        MOVS     R2,R5
        LDR.N    R1,??DataTable17_40
        LDR.N    R0,??DataTable17_11
          CFI FunCall appendPrintf
        BL       appendPrintf
//  431 		float volts = adcToVolts(adcValue);
        VCVT.F32.S32 S0,S16
        VLDR.W   S1,??DataTable14_1  ;; 0x3a400c01
        VMUL.F32 S17,S0,S1
//  432 		appendPrintf(&logger, " v=%f", volts);
        VMOV     R0,S17
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R1,??DataTable17_41
        LDR.N    R0,??DataTable17_11
          CFI FunCall appendPrintf
        BL       appendPrintf
//  433 
//  434 		appendMsgPostfix(&logger);
        LDR.N    R0,??DataTable17_11
          CFI FunCall appendMsgPostfix
        BL       appendMsgPostfix
//  435 		scheduleLogging(&logger);
        LDR.N    R0,??DataTable17_11
          CFI FunCall scheduleLogging
        BL       scheduleLogging
//  436 	}
        ADDS     R4,R4,#+1
        B.N      ??printFullAdcReport_0
//  437 }
??printFullAdcReport_1:
        ADD      SP,SP,#+8
          CFI CFA R13+32
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+24
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock21
//  438 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock22 Using cfiCommon0
          CFI Function _Z11printStatusv
        THUMB
//  439 static void printStatus(void) {
_Z11printStatusv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  440 	scheduleMsg(&logger, "adcDebug=%d", adcDebugReporting);
        LDR.N    R0,??DataTable17_42
        LDR      R2,[R0, #+0]
        LDR.N    R1,??DataTable17_43
        LDR.N    R0,??DataTable17_11
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  441 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock22
//  442 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock23 Using cfiCommon0
          CFI Function _Z20setAdcDebugReportingi
        THUMB
//  443 static void setAdcDebugReporting(int value) {
_Z20setAdcDebugReportingi:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  444 	adcDebugReporting = value;
        LDR.N    R0,??DataTable17_42
        STR      R4,[R0, #+0]
//  445 	printStatus();
          CFI FunCall _Z11printStatusv
        BL       _Z11printStatusv
//  446 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock23
//  447 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock24 Using cfiCommon0
          CFI Function _Z17adc_callback_slowP9ADCDriverPtj
        THUMB
//  448 static void adc_callback_slow(ADCDriver *adcp, adcsample_t *buffer, size_t n) {
_Z17adc_callback_slowP9ADCDriverPtj:
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
//  449 	(void) buffer;
//  450 	(void) n;
//  451 	efiAssertVoid(getRemainingStack(chThdSelf()) > 128, "lowstck#9c");
        LDR.N    R0,??DataTable17_16
        LDR      R0,[R0, #+24]
          CFI FunCall getRemainingStack
        BL       getRemainingStack
        CMP      R0,#+129
        BGE.N    ??adc_callback_slow_0
        LDR.N    R0,??DataTable17_44
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??adc_callback_slow_1
//  452 	/* Note, only in the ADC_COMPLETE state because the ADC driver fires
//  453 	 * an intermediate callback when the buffer is half full. */
//  454 	if (adcp->state == ADC_COMPLETE) {
??adc_callback_slow_0:
        LDRB     R0,[R4, #+0]
        CMP      R0,#+4
        BNE.N    ??adc_callback_slow_2
//  455 		/* Calculates the average values from the ADC samples.*/
//  456 
//  457 		adcCallbackCounter_slow++;
        LDR.N    R0,??DataTable17_45
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+1
        LDR.N    R1,??DataTable17_45
        STR      R0,[R1, #+0]
//  458 
//  459 //		newState.time = chimeNow();
//  460 		for (int i = 0; i < slowAdc.size(); i++) {
        MOVS     R7,#+0
??adc_callback_slow_3:
        LDR.N    R0,??DataTable17_13
          CFI FunCall _ZN9AdcDevice4sizeEv
        BL       _ZN9AdcDevice4sizeEv
        CMP      R7,R0
        BGE.N    ??adc_callback_slow_2
//  461 			/**
//  462 			 * todo: No need to average since DEPTH is '1'
//  463 			 */
//  464 			int value = getAvgAdcValue(i, slowAdc.samples, ADC_BUF_DEPTH_SLOW, slowAdc.size());
        LDR.N    R0,??DataTable17_13
          CFI FunCall _ZN9AdcDevice4sizeEv
        BL       _ZN9AdcDevice4sizeEv
        MOVS     R3,R0
        MOVS     R2,#+1
        LDR.N    R1,??DataTable17_19
        MOVS     R0,R7
          CFI FunCall _Z14getAvgAdcValueiPtii
        BL       _Z14getAvgAdcValueiPtii
//  465 			slowAdc.values.adc_data[i] = value;
        LDR.N    R1,??DataTable17_13
        ADDS     R1,R1,R7, LSL #+1
        STRH     R0,[R1, #+120]
//  466 		}
        ADDS     R7,R7,#+1
        B.N      ??adc_callback_slow_3
//  467 	}
//  468 }
??adc_callback_slow_2:
??adc_callback_slow_1:
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock24
//  469 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  470 static char errorMsgBuff[10];
errorMsgBuff:
        DS8 12
//  471 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock25 Using cfiCommon0
          CFI Function _Z10addChannelPKc13adc_channel_e18adc_channel_mode_e
        THUMB
//  472 static void addChannel(const char *name, adc_channel_e setting, adc_channel_mode_e mode) {
_Z10addChannelPKc13adc_channel_e18adc_channel_mode_e:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//  473 	if (setting == EFI_ADC_NONE) {
        CMP      R5,#+16
        BEQ.N    ??addChannel_3
//  474 		return;
//  475 	}
//  476 	if (adcHwChannelEnabled[setting] != ADC_OFF) {
??addChannel_4:
        LDR.N    R0,??DataTable17_24
        LDR      R0,[R0, R5, LSL #+2]
        CMP      R0,#+0
        BEQ.N    ??addChannel_5
//  477 		getPinNameByAdcChannel(setting, errorMsgBuff);
        LDR.N    R1,??DataTable17_46
        MOVS     R0,R5
          CFI FunCall _Z22getPinNameByAdcChannel13adc_channel_ePc
        BL       _Z22getPinNameByAdcChannel13adc_channel_ePc
//  478 		firmwareError("ADC mapping error: input %s for %s already used?", errorMsgBuff, name);
        MOVS     R2,R4
        LDR.N    R1,??DataTable17_46
        LDR.N    R0,??DataTable17_47
          CFI FunCall firmwareError
        BL       firmwareError
//  479 	}
//  480 
//  481 	adcHwChannelUsage[setting] = name;
??addChannel_5:
        LDR.N    R0,??DataTable17_48
        STR      R4,[R0, R5, LSL #+2]
//  482 	adcHwChannelEnabled[setting] = mode;
        LDR.N    R0,??DataTable17_24
        STR      R6,[R0, R5, LSL #+2]
//  483 }
??addChannel_3:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock25

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14:
        DC8      0x68, 0x77, 0x00, 0x00

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_1:
        DC32     0x3a400c01
//  484 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock26 Using cfiCommon0
          CFI Function _Z15configureInputsv
        THUMB
//  485 static void configureInputs(void) {
_Z15configureInputsv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  486 	memset(adcHwChannelEnabled, 0, sizeof(adcHwChannelEnabled));
        MOVS     R2,#+64
        MOVS     R1,#+0
        LDR.N    R0,??DataTable17_24
          CFI FunCall memset
        BL       memset
//  487 	memset(adcHwChannelUsage, 0, sizeof(adcHwChannelUsage));
        MOVS     R2,#+64
        MOVS     R1,#+0
        LDR.N    R0,??DataTable17_48
          CFI FunCall memset
        BL       memset
//  488 
//  489 	addChannel("TPS", engineConfiguration->tpsAdcChannel, ADC_FAST);
        MOVS     R2,#+2
        LDR.N    R0,??DataTable17_36
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+524]
        ADR.N    R0,??DataTable17  ;; "TPS"
          CFI FunCall _Z10addChannelPKc13adc_channel_e18adc_channel_mode_e
        BL       _Z10addChannelPKc13adc_channel_e18adc_channel_mode_e
//  490 	addChannel("MAP", engineConfiguration->map.sensor.hwChannel, ADC_FAST);
        MOVS     R2,#+2
        LDR.N    R0,??DataTable17_36
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+252]
        ADR.N    R0,??DataTable17_1  ;; "MAP"
          CFI FunCall _Z10addChannelPKc13adc_channel_e18adc_channel_mode_e
        BL       _Z10addChannelPKc13adc_channel_e18adc_channel_mode_e
//  491 	addChannel("MAF", engineConfiguration->mafAdcChannel, ADC_FAST);
        MOVS     R2,#+2
        LDR.N    R0,??DataTable17_36
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+580]
        ADR.N    R0,??DataTable17_2  ;; "MAF"
          CFI FunCall _Z10addChannelPKc13adc_channel_e18adc_channel_mode_e
        BL       _Z10addChannelPKc13adc_channel_e18adc_channel_mode_e
//  492 	addChannel("hip", engineConfiguration->hipOutputChannel, ADC_FAST);
        MOVS     R2,#+2
        LDR.N    R0,??DataTable17_36
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+1500]
        ADR.N    R0,??DataTable17_3  ;; "hip"
          CFI FunCall _Z10addChannelPKc13adc_channel_e18adc_channel_mode_e
        BL       _Z10addChannelPKc13adc_channel_e18adc_channel_mode_e
//  493 
//  494 	addChannel("fuel", engineConfiguration->fuelLevelSensor, ADC_SLOW);
        MOVS     R2,#+1
        LDR.N    R0,??DataTable17_36
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+484]
        LDR.N    R0,??DataTable17_49
          CFI FunCall _Z10addChannelPKc13adc_channel_e18adc_channel_mode_e
        BL       _Z10addChannelPKc13adc_channel_e18adc_channel_mode_e
//  495 	addChannel("pPS", engineConfiguration->pedalPositionChannel, ADC_SLOW);
        MOVS     R2,#+1
        LDR.N    R0,??DataTable17_36
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+604]
        ADR.N    R0,??DataTable17_4  ;; "pPS"
          CFI FunCall _Z10addChannelPKc13adc_channel_e18adc_channel_mode_e
        BL       _Z10addChannelPKc13adc_channel_e18adc_channel_mode_e
//  496 	addChannel("VBatt", engineConfiguration->vbattAdcChannel, ADC_SLOW);
        MOVS     R2,#+1
        LDR.N    R0,??DataTable17_36
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+472]
        LDR.N    R0,??DataTable17_50
          CFI FunCall _Z10addChannelPKc13adc_channel_e18adc_channel_mode_e
        BL       _Z10addChannelPKc13adc_channel_e18adc_channel_mode_e
//  497 	addChannel("Vref", engineConfiguration->vRefAdcChannel, ADC_SLOW);
        MOVS     R2,#+1
        LDR.N    R0,??DataTable17_36
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+1536]
        LDR.N    R0,??DataTable17_51
          CFI FunCall _Z10addChannelPKc13adc_channel_e18adc_channel_mode_e
        BL       _Z10addChannelPKc13adc_channel_e18adc_channel_mode_e
//  498 	addChannel("CLT", engineConfiguration->clt.adcChannel, ADC_SLOW);
        MOVS     R2,#+1
        LDR.N    R0,??DataTable17_36
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+284]
        ADR.N    R0,??DataTable17_5  ;; "CLT"
          CFI FunCall _Z10addChannelPKc13adc_channel_e18adc_channel_mode_e
        BL       _Z10addChannelPKc13adc_channel_e18adc_channel_mode_e
//  499 	addChannel("IAT", engineConfiguration->iat.adcChannel, ADC_SLOW);
        MOVS     R2,#+1
        LDR.N    R0,??DataTable17_36
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+316]
        ADR.N    R0,??DataTable17_6  ;; "IAT"
          CFI FunCall _Z10addChannelPKc13adc_channel_e18adc_channel_mode_e
        BL       _Z10addChannelPKc13adc_channel_e18adc_channel_mode_e
//  500 	addChannel("AFR", engineConfiguration->afr.hwChannel, ADC_SLOW);
        MOVS     R2,#+1
        LDR.N    R0,??DataTable17_36
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+584]
        ADR.N    R0,??DataTable17_7  ;; "AFR"
          CFI FunCall _Z10addChannelPKc13adc_channel_e18adc_channel_mode_e
        BL       _Z10addChannelPKc13adc_channel_e18adc_channel_mode_e
//  501 	addChannel("AC", engineConfiguration->acSwitchAdc, ADC_SLOW);
        MOVS     R2,#+1
        LDR.N    R0,??DataTable17_36
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+1520]
        ADR.N    R0,??DataTable17_8  ;; 0x41, 0x43, 0x00, 0x00
          CFI FunCall _Z10addChannelPKc13adc_channel_e18adc_channel_mode_e
        BL       _Z10addChannelPKc13adc_channel_e18adc_channel_mode_e
//  502 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock26
//  503 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock27 Using cfiCommon0
          CFI Function _Z13initAdcInputsb
        THUMB
//  504 void initAdcInputs(bool boardTestMode) {
_Z13initAdcInputsb:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
//  505 	printMsg(&logger, "initAdcInputs()");
        LDR.N    R1,??DataTable17_52
        LDR.N    R0,??DataTable17_11
          CFI FunCall printMsg
        BL       printMsg
//  506 
//  507 	configureInputs();
          CFI FunCall _Z15configureInputsv
        BL       _Z15configureInputsv
//  508 
//  509 
//  510 	printStatus();
          CFI FunCall _Z11printStatusv
        BL       _Z11printStatusv
//  511 
//  512 	addConsoleActionI("adcDebug", &setAdcDebugReporting);
        LDR.N    R1,??DataTable17_53
        LDR.N    R0,??DataTable17_54
          CFI FunCall addConsoleActionI
        BL       addConsoleActionI
//  513 
//  514 #if EFI_INTERNAL_ADC
//  515 	/*
//  516 	 * Initializes the ADC driver.
//  517 	 */
//  518 	adcStart(&ADC_SLOW_DEVICE, NULL);
        MOVS     R1,#+0
        LDR.N    R0,??DataTable17_18
          CFI FunCall adcStart
        BL       adcStart
//  519 	adcStart(&ADC_FAST_DEVICE, NULL);
        MOVS     R1,#+0
        LDR.N    R0,??DataTable17_21
          CFI FunCall adcStart
        BL       adcStart
//  520 
//  521 	for (int adc = 0; adc < HW_MAX_ADC_INDEX; adc++) {
        MOVS     R5,#+0
??initAdcInputs_0:
        CMP      R5,#+16
        BGE.N    ??initAdcInputs_1
//  522 		adc_channel_mode_e mode = adcHwChannelEnabled[adc];
        LDR.N    R0,??DataTable17_24
        LDR      R6,[R0, R5, LSL #+2]
//  523 
//  524 		/**
//  525 		 * in board test mode all currently enabled ADC channels are running in slow mode
//  526 		 */
//  527 		if (mode == ADC_SLOW || (boardTestMode && mode == ADC_FAST)) {
        CMP      R6,#+1
        BEQ.N    ??initAdcInputs_2
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        CMP      R4,#+0
        BEQ.N    ??initAdcInputs_3
        CMP      R6,#+2
        BNE.N    ??initAdcInputs_3
//  528 			slowAdc.addChannel((adc_channel_e) (ADC_CHANNEL_IN0 + adc));
??initAdcInputs_2:
        MOVS     R1,R5
        LDR.N    R0,??DataTable17_13
          CFI FunCall _ZN9AdcDevice10addChannelE13adc_channel_e
        BL       _ZN9AdcDevice10addChannelE13adc_channel_e
        B.N      ??initAdcInputs_4
//  529 		} else if (mode == ADC_FAST) {
??initAdcInputs_3:
        CMP      R6,#+2
        BNE.N    ??initAdcInputs_4
//  530 			fastAdc.addChannel((adc_channel_e) (ADC_CHANNEL_IN0 + adc));
        MOVS     R1,R5
        LDR.N    R0,??DataTable17_15
          CFI FunCall _ZN9AdcDevice10addChannelE13adc_channel_e
        BL       _ZN9AdcDevice10addChannelE13adc_channel_e
//  531 		}
//  532 	}
??initAdcInputs_4:
        ADDS     R5,R5,#+1
        B.N      ??initAdcInputs_0
//  533 
//  534 	slowAdc.init();
??initAdcInputs_1:
        LDR.N    R0,??DataTable17_13
          CFI FunCall _ZN9AdcDevice4initEv
        BL       _ZN9AdcDevice4initEv
//  535 	pwmStart(EFI_INTERNAL_SLOW_ADC_PWM, &pwmcfg_slow);
        LDR.N    R1,??DataTable17_55
        LDR.N    R0,??DataTable17_56
          CFI FunCall pwmStart
        BL       pwmStart
//  536 	if (boardConfiguration->isFastAdcEnabled) {
        LDR.N    R0,??DataTable17_57
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+384]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+4
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??initAdcInputs_5
//  537 		fastAdc.init();
        LDR.N    R0,??DataTable17_15
          CFI FunCall _ZN9AdcDevice4initEv
        BL       _ZN9AdcDevice4initEv
//  538 		/*
//  539 		 * Initializes the PWM driver.
//  540 		 */
//  541 		pwmStart(EFI_INTERNAL_FAST_ADC_PWM, &pwmcfg_fast);
        LDR.N    R1,??DataTable17_58
        LDR.N    R0,??DataTable17_59
          CFI FunCall pwmStart
        BL       pwmStart
//  542 	}
//  543 
//  544 	// ADC_CHANNEL_IN0 // PA0
//  545 	// ADC_CHANNEL_IN1 // PA1
//  546 	// ADC_CHANNEL_IN2 // PA2
//  547 	// ADC_CHANNEL_IN3 // PA3
//  548 	// ADC_CHANNEL_IN4 // PA4
//  549 	// ADC_CHANNEL_IN5 // PA5 - this is also TIM2_CH1
//  550 	// ADC_CHANNEL_IN6 // PA6
//  551 	// ADC_CHANNEL_IN7 // PA7
//  552 	// ADC_CHANNEL_IN8 // PB0
//  553 	// ADC_CHANNEL_IN9 // PB1
//  554 	// ADC_CHANNEL_IN10 // PC0
//  555 	// ADC_CHANNEL_IN11 // PC1
//  556 	// ADC_CHANNEL_IN12 // PC2
//  557 	// ADC_CHANNEL_IN13 // PC3
//  558 	// ADC_CHANNEL_IN14 // PC4
//  559 	// ADC_CHANNEL_IN15 // PC5
//  560 
//  561 	//if(slowAdcChannelCount > ADC_MAX_SLOW_CHANNELS_COUNT) // todo: do we need this logic? do we need this check
//  562 
//  563 	addConsoleActionI("adc", (VoidInt) printAdcValue);
??initAdcInputs_5:
        LDR.N    R1,??DataTable17_60
        ADR.N    R0,??DataTable17_9  ;; "adc"
          CFI FunCall addConsoleActionI
        BL       addConsoleActionI
//  564 	addConsoleAction("fadc", printFullAdcReport);
        LDR.N    R1,??DataTable17_61
        LDR.N    R0,??DataTable17_62
          CFI FunCall addConsoleAction
        BL       addConsoleAction
//  565 #else
//  566 	printMsg(&logger, "ADC disabled");
//  567 #endif
//  568 }
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock27

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock28 Using cfiCommon0
          CFI Function _Z13pokeAdcInputsv
        THUMB
_Z13pokeAdcInputsv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        LDR.N    R0,??DataTable17_42
        LDR      R0,[R0, #+0]
        CMP      R0,#+0
        BEQ.N    ??pokeAdcInputs_0
??pokeAdcInputs_1:
          CFI FunCall _Z18printFullAdcReportv
        BL       _Z18printFullAdcReportv
??pokeAdcInputs_0:
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock28

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17:
        DC8      "TPS"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_1:
        DC8      "MAP"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_2:
        DC8      "MAF"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_3:
        DC8      "hip"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_4:
        DC8      "pPS"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_5:
        DC8      "CLT"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_6:
        DC8      "IAT"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_7:
        DC8      "AFR"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_8:
        DC8      0x41, 0x43, 0x00, 0x00

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_9:
        DC8      "adc"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_10:
        DC32     LOGGING_BUFFER

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_11:
        DC32     logger

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_12:
        DC32     adcgrpcfgSlow

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_13:
        DC32     slowAdc

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_14:
        DC32     adcgrpcfg_fast

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_15:
        DC32     fastAdc

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_16:
        DC32     rlist

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_17:
        DC32     `?<Constant "lwStAdcSlow">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_18:
        DC32     ADCD1

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_19:
        DC32     slowAdc+0x58

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_20:
        DC32     `?<Constant "lwStAdcFast">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_21:
        DC32     ADCD2

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_22:
        DC32     fastAdc+0x58

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_23:
        DC32     `?<Constant "ADC: should not be as...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_24:
        DC32     adcHwChannelEnabled

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_25:
        DC32     `?<Constant "ADC is off [%s] index=%d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_26:
        DC32     `?<Constant "adc %s\\r\\n">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_27:
        DC32     `?<Constant "adc input">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_28:
        DC32     0x40020000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_29:
        DC32     0x40020400

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_30:
        DC32     0x40020800

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_31:
        DC32     `?<Constant "Unknown hw channel %d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_32:
        DC32     `?<Constant "slow">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_33:
        DC32     `?<Constant "fast">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_34:
        DC32     `?<Constant "INACTIVE">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_35:
        DC32     `?<Constant "print">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_36:
        DC32     engineConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_37:
        DC32     `?<Constant "adc voltage : %f">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_38:
        DC32     `?<Constant "fast %d slow %d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_39:
        DC32     `?<Constant " ch%d %s%d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_40:
        DC32     `?<Constant " ADC%d 12bit=%d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_41:
        DC32     `?<Constant " v=%f">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_42:
        DC32     adcDebugReporting

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_43:
        DC32     `?<Constant "adcDebug=%d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_44:
        DC32     `?<Constant "lowstck#9c">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_45:
        DC32     adcCallbackCounter_slow

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_46:
        DC32     errorMsgBuff

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_47:
        DC32     `?<Constant "ADC mapping error: in...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_48:
        DC32     adcHwChannelUsage

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_49:
        DC32     `?<Constant "fuel">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_50:
        DC32     `?<Constant "VBatt">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_51:
        DC32     `?<Constant "Vref">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_52:
        DC32     `?<Constant "initAdcInputs()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_53:
        DC32     _Z20setAdcDebugReportingi

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_54:
        DC32     `?<Constant "adcDebug">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_55:
        DC32     pwmcfg_slow

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_56:
        DC32     PWMD8

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_57:
        DC32     boardConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_58:
        DC32     pwmcfg_fast

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_59:
        DC32     PWMD4

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_60:
        DC32     _Z13printAdcValue13adc_channel_e

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_61:
        DC32     _Z18printFullAdcReportv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_62:
        DC32     `?<Constant "fadc">`

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP memset
          CFI Block cfiBlock29 Using cfiCommon0
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
          CFI EndBlock cfiBlock29

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
`?<Constant "lwStAdcSlow">`:
        DATA
        DC8 "lwStAdcSlow"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "lwStAdcFast">`:
        DATA
        DC8 "lwStAdcFast"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ADC: should not be as...">`:
        DATA
        DC8 "ADC: should not be asking for NONE %s"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ADC is off [%s] index=%d">`:
        DATA
        DC8 "ADC is off [%s] index=%d"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "adc %s\\r\\n">`:
        DATA
        DC8 "adc %s\015\012"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "adc input">`:
        DATA
        DC8 "adc input"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Unknown hw channel %d">`:
        DATA
        DC8 "Unknown hw channel %d"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "slow">`:
        DATA
        DC8 "slow"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "fast">`:
        DATA
        DC8 "fast"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "INACTIVE">`:
        DATA
        DC8 "INACTIVE"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "hw"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "print">`:
        DATA
        DC8 "print"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "adc voltage : %f">`:
        DATA
        DC8 "adc voltage : %f"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "fast %d slow %d">`:
        DATA
        DC8 "fast %d slow %d"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant " ch%d %s%d">`:
        DATA
        DC8 " ch%d %s%d"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant " ADC%d 12bit=%d">`:
        DATA
        DC8 " ADC%d 12bit=%d"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant " v=%f">`:
        DATA
        DC8 " v=%f"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "adcDebug=%d">`:
        DATA
        DC8 "adcDebug=%d"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "lowstck#9c">`:
        DATA
        DC8 "lowstck#9c"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ADC mapping error: in...">`:
        DATA
        DC8 "ADC mapping error: input %s for %s already used?"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "TPS"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "MAP"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "MAF"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "hip"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "fuel">`:
        DATA
        DC8 "fuel"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "pPS"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "VBatt">`:
        DATA
        DC8 "VBatt"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Vref">`:
        DATA
        DC8 "Vref"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "CLT"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "IAT"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "AFR"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "AC"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "initAdcInputs()">`:
        DATA
        DC8 "initAdcInputs()"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "adcDebug">`:
        DATA
        DC8 "adcDebug"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "adc"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "fadc">`:
        DATA
        DC8 "fadc"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "ADC"

        END
//  569 
//  570 void pokeAdcInputs() {
//  571 	if (!adcDebugReporting)
//  572 		return;
//  573 	printFullAdcReport();
//  574 }
//  575 
//  576 #endif /* HAL_USE_ADC */
// 
// 1 188 bytes in section .bss
//   184 bytes in section .data
//     4 bytes in section .init_array
//   432 bytes in section .rodata
// 2 446 bytes in section .text
// 
// 2 428 bytes of CODE  memory (+ 22 bytes shared)
//   432 bytes of CONST memory
// 1 372 bytes of DATA  memory
//
//Errors: none
//Warnings: 1
