///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:08:52 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\controllers\sen /
//                       sors\thermistors.cpp                                 /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\controllers\sen /
//                       sors\thermistors.cpp -lCN                            /
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
//                       -On --eec++                                          /
//    MISRA C version =  2004                                                 /
//      Enabled       =  1.2-1.4 2.3 3.4 4.2 6.5 7 8.2 8.3 9.3 12.3 13.6      /
//                       13.7 15.5 17.3 19.17 20.7-20.9 20.11                 /
//      Checked       =  1.4 2.3 4.2 6.5 7 8.2 8.3 9.3 12.3 13.7 15.5         /
//                       20.7-20.9 20.11                                      /
//      Not checked   =  1.1-1.3 1.5-2.2 2.4-4.1 5-6.4 8.1 8.4-9.2 10-12.2    /
//                       12.4-13.6 14-15.4 16-20.6 20.10 20.12 21             /
//    List file       =  F:\stuff\rusefi_sourceforge\firmware\iar\Debug\List\ /
//                       thermistors.s                                        /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME thermistors

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _Z19getInternalAdcValuePKc13adc_channel_e
        EXTERN __aeabi_f2d
        EXTERN __aeabi_memcpy
        EXTERN __aeabi_memset
        EXTERN __iar_FDtest
        EXTERN addConsoleActionF
        EXTERN engine
        EXTERN engineConfiguration
        EXTERN firmwareError
        EXTERN logf
        EXTERN memcmp
        EXTERN scheduleMsg
        EXTERN warning

        PUBLIC _Z13getResistanceP14ThermistorConf
        PUBLIC _Z14setDodgeSensorP14ThermistorConf
        PUBLIC _Z15getTemperatureCP14ThermistorConfP14ThermistorMath
        PUBLIC _Z15initThermistorsP7Logging
        PUBLIC _Z17convertCelsiustoFf
        PUBLIC _Z17convertFtoCelsiusf
        PUBLIC _Z18setCommonNTCSensorP14ThermistorConf
        PUBLIC _Z20getKelvinTemperatureP14ThermistorConffP14ThermistorMath
        PUBLIC _Z21getCoolantTemperaturev
        PUBLIC _Z21getR1InVoltageDividorfff
        PUBLIC _Z21getR2InVoltageDividorfff
        PUBLIC _Z23getIntakeAirTemperaturev
        PUBLIC _Z23getVoutInVoltageDividorfff
        PUBLIC _Z25convertKelvinToFahrenheitf
        PUBLIC _Z25isValidCoolantTemperaturef
        PUBLIC _Z26setThermistorConfigurationP14ThermistorConfffffff
        PUBLIC _Z27isValidIntakeAirTemperaturef
        PUBLIC _ZN14ThermistorMath4initEP17thermistor_conf_s
        PUBLIC _ZN14ThermistorMathC1Ev
        PUBLIC _ZN14ThermistorMathC2Ev
        PUBLIC memcpy
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
        
// F:\stuff\rusefi_sourceforge\firmware\controllers\sensors\thermistors.cpp
//    1 /**
//    2  * @file thermistors.cpp
//    3  *
//    4  * @date Feb 17, 2013
//    5  * @author Andrey Belomutskiy, (c) 2012-2015
//    6  */
//    7 
//    8 /**
//    9  * http://en.wikipedia.org/wiki/Thermistor
//   10  * http://en.wikipedia.org/wiki/Steinhart%E2%80%93Hart_equation
//   11  */
//   12 
//   13 #include "main.h"
//   14 #include "thermistors.h"
//   15 #include "adc_inputs.h"
//   16 #include "engine_configuration.h"
//   17 #include "engine_math.h"
//   18 
//   19 // Celsius
//   20 #define LIMPING_MODE_IAT_TEMPERATURE 30.0f
//   21 #define LIMPING_MODE_CLT_TEMPERATURE 70.0f
//   22 
//   23 EXTERN_ENGINE
//   24 ;
//   25 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   26 static Logging *logger;
logger:
        DS8 4
//   27 

        SECTION `.bss`:DATA:REORDER:NOROOT(0)
//   28 static bool initialized = false;
initialized:
        DS8 1

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _ZN14ThermistorMathC2Ev
        THUMB
// __code __interwork __softfp ThermistorMath::subobject ThermistorMath()
_ZN14ThermistorMathC2Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN14ThermistorMathC1Ev
        BL       _ZN14ThermistorMathC1Ev
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock0
//   29 
//   30 /**
//   31  * http://en.wikipedia.org/wiki/Voltage_divider
//   32  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z21getR1InVoltageDividorfff
          CFI NoCalls
        THUMB
//   33 float getR1InVoltageDividor(float Vout, float Vin, float r2) {
//   34 	return r2 * Vin / Vout - r2;
_Z21getR1InVoltageDividorfff:
        VMUL.F32 S1,S2,S1
        VDIV.F32 S0,S1,S0
        VSUB.F32 S0,S0,S2
        BX       LR               ;; return
          CFI EndBlock cfiBlock1
//   35 }
//   36 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z21getR2InVoltageDividorfff
          CFI NoCalls
        THUMB
//   37 float getR2InVoltageDividor(float Vout, float Vin, float r1) {
_Z21getR2InVoltageDividorfff:
        VMOV.F32 S3,S0
//   38 	if (Vout == 0) {
        VCMP.F32 S3,#0.0
        FMSTAT   
        BNE.N    ??getR2InVoltageDividor_0
//   39 		return NAN;
        VLDR.W   S0,??DataTable9  ;; 0x7fffffff
        B.N      ??getR2InVoltageDividor_1
//   40 	}
//   41 	return r1 / (Vin / Vout - 1);
??getR2InVoltageDividor_0:
        VDIV.F32 S0,S1,S3
        VMOV.F32 S1,#-1.0
        VADD.F32 S0,S0,S1
        VDIV.F32 S0,S2,S0
??getR2InVoltageDividor_1:
        BX       LR               ;; return
          CFI EndBlock cfiBlock2
//   42 }
//   43 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z23getVoutInVoltageDividorfff
          CFI NoCalls
        THUMB
//   44 float getVoutInVoltageDividor(float Vin, float r1, float r2) {
//   45 	return r2 * Vin / (r1 + r2);
_Z23getVoutInVoltageDividorfff:
        VMUL.F32 S0,S2,S0
        VADD.F32 S1,S1,S2
        VDIV.F32 S0,S0,S1
        BX       LR               ;; return
          CFI EndBlock cfiBlock3
//   46 }
//   47 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _Z20getKelvinTemperatureP14ThermistorConffP14ThermistorMath
        THUMB
//   48 float getKelvinTemperature(ThermistorConf *config, float resistance, ThermistorMath *tm) {
_Z20getKelvinTemperatureP14ThermistorConffP14ThermistorMath:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        VPUSH    {D8}
          CFI D8 Frame(CFA, -24)
          CFI CFA R13+24
        MOVS     R4,R0
        VMOV.F32 S16,S0
        MOVS     R5,R1
//   49 	tm->init(&config->config); // implementation checks if config has changed or not
        MOVS     R1,R4
        MOVS     R0,R5
          CFI FunCall _ZN14ThermistorMath4initEP17thermistor_conf_s
        BL       _ZN14ThermistorMath4initEP17thermistor_conf_s
//   50 	thermistor_curve_s * curve = &tm->curve;
        MOVS     R6,R5
//   51 	efiAssert(curve != NULL, "thermistor pointer is NULL", NAN);
        CMP      R6,#+0
        BNE.N    ??getKelvinTemperature_0
        LDR.W    R0,??DataTable15_12
          CFI FunCall firmwareError
        BL       firmwareError
        VLDR.W   S0,??DataTable9  ;; 0x7fffffff
        B.N      ??getKelvinTemperature_1
//   52 
//   53 	if (resistance <= 0) {
??getKelvinTemperature_0:
        VCMP.F32 S16,#0.0
        FMSTAT   
        BHI.N    ??getKelvinTemperature_2
//   54 		//warning("Invalid resistance in getKelvinTemperature=", resistance);
//   55 		return 0.0f;
        VLDR.W   S0,??DataTable9_1  ;; 0x0
        B.N      ??getKelvinTemperature_1
//   56 	}
//   57 	float logR = logf(resistance);
??getKelvinTemperature_2:
        VMOV.F32 S0,S16
          CFI FunCall logf
        BL       logf
//   58 	return 1 / (curve->s_h_a + curve->s_h_b * logR + curve->s_h_c * logR * logR * logR);
        VLDR     S1,[R6, #+4]
        VLDR     S2,[R6, #0]
        VMLA.F32 S2,S1,S0
        VLDR     S1,[R6, #+8]
        VMUL.F32 S1,S1,S0
        VMUL.F32 S1,S1,S0
        VMLA.F32 S2,S1,S0
        VMOV.F32 S0,#1.0
        VDIV.F32 S0,S0,S2
??getKelvinTemperature_1:
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+16
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock4
//   59 }
//   60 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _Z17convertCelsiustoFf
          CFI NoCalls
        THUMB
//   61 float convertCelsiustoF(float tempC) {
//   62 	return tempC * 9 / 5 + 32;
_Z17convertCelsiustoFf:
        VMOV.F32 S1,#9.0
        VMUL.F32 S0,S0,S1
        VMOV.F32 S1,#5.0
        VDIV.F32 S0,S0,S1
        VLDR.W   S1,??DataTable9_2  ;; 0x42000000
        VADD.F32 S0,S0,S1
        BX       LR               ;; return
          CFI EndBlock cfiBlock5
//   63 }
//   64 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _Z17convertFtoCelsiusf
          CFI NoCalls
        THUMB
//   65 float convertFtoCelsius(float tempF) {
//   66 	return (tempF - 32) / 9 * 5;
_Z17convertFtoCelsiusf:
        VLDR.W   S1,??DataTable9_3  ;; 0xc2000000
        VADD.F32 S0,S0,S1
        VMOV.F32 S1,#9.0
        VDIV.F32 S0,S0,S1
        VMOV.F32 S1,#5.0
        VMUL.F32 S0,S0,S1
        BX       LR               ;; return
          CFI EndBlock cfiBlock6
//   67 }
//   68 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function _Z25convertKelvinToFahrenheitf
        THUMB
//   69 float convertKelvinToFahrenheit(float kelvin) {
_Z25convertKelvinToFahrenheitf:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        VMOV.F32 S16,S0
//   70 	float tempC = convertKelvinToCelcius(kelvin);
        VLDR.W   S0,??DataTable9_4  ;; 0xc3889333
        VADD.F32 S17,S16,S0
//   71 	return convertCelsiustoF(tempC);
        VMOV.F32 S0,S17
          CFI FunCall _Z17convertCelsiustoFf
        BL       _Z17convertCelsiustoFf
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock7
//   72 }
//   73 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function _Z13getResistanceP14ThermistorConf
        THUMB
//   74 float getResistance(ThermistorConf *config) {
_Z13getResistanceP14ThermistorConf:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
//   75 	float voltage = getVoltageDivided("term", config->adcChannel);
        LDR      R1,[R4, #+28]
        LDR.W    R0,??DataTable15_13
          CFI FunCall _Z19getInternalAdcValuePKc13adc_channel_e
        BL       _Z19getInternalAdcValuePKc13adc_channel_e
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        VLDR.W   S1,??DataTable9_5  ;; 0x3a400c01
        VMUL.F32 S0,S0,S1
        LDR.W    R0,??DataTable15_14
        LDR      R0,[R0, #+0]
        VLDR     S1,[R0, #+464]
        VMUL.F32 S16,S0,S1
//   76 	efiAssert(config != NULL, "thermistor config is null", NAN);
        CMP      R4,#+0
        BNE.N    ??getResistance_0
        LDR.W    R0,??DataTable15_15
          CFI FunCall firmwareError
        BL       firmwareError
        VLDR.W   S0,??DataTable9  ;; 0x7fffffff
        B.N      ??getResistance_1
//   77 	thermistor_conf_s *tc = &config->config;
//   78 
//   79 	float resistance = getR2InVoltageDividor(voltage, _5_VOLTS, tc->bias_resistor);
??getResistance_0:
        VLDR     S2,[R4, #+24]
        VMOV.F32 S1,#5.0
        VMOV.F32 S0,S16
          CFI FunCall _Z21getR2InVoltageDividorfff
        BL       _Z21getR2InVoltageDividorfff
//   80 	return resistance;
??getResistance_1:
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock8
//   81 }
//   82 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function _Z15getTemperatureCP14ThermistorConfP14ThermistorMath
        THUMB
//   83 float getTemperatureC(ThermistorConf *config, ThermistorMath *tm) {
_Z15getTemperatureCP14ThermistorConfP14ThermistorMath:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        VPUSH    {D8}
          CFI D8 Frame(CFA, -24)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
//   84 	if (!initialized) {
        LDR.W    R0,??DataTable15_16
        LDRB     R0,[R0, #+0]
        CMP      R0,#+0
        BNE.N    ??getTemperatureC_0
//   85 		firmwareError("thermstr not initialized");
        LDR.W    R0,??DataTable15_17
          CFI FunCall firmwareError
        BL       firmwareError
//   86 		return NAN;
        VLDR.W   S0,??DataTable9  ;; 0x7fffffff
        B.N      ??getTemperatureC_1
//   87 	}
//   88 	float resistance = getResistance(config);
??getTemperatureC_0:
        MOVS     R0,R4
          CFI FunCall _Z13getResistanceP14ThermistorConf
        BL       _Z13getResistanceP14ThermistorConf
        VMOV.F32 S16,S0
//   89 
//   90 	float kelvinTemperature = getKelvinTemperature(config, resistance, tm);
        MOVS     R1,R5
        VMOV.F32 S0,S16
        MOVS     R0,R4
          CFI FunCall _Z20getKelvinTemperatureP14ThermistorConffP14ThermistorMath
        BL       _Z20getKelvinTemperatureP14ThermistorConffP14ThermistorMath
//   91 	return convertKelvinToCelcius(kelvinTemperature);
        VLDR.W   S1,??DataTable9_4  ;; 0xc3889333
        VADD.F32 S0,S0,S1
??getTemperatureC_1:
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+16
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock9
//   92 }
//   93 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function _Z25isValidCoolantTemperaturef
        THUMB
//   94 bool isValidCoolantTemperature(float temperature) {
_Z25isValidCoolantTemperaturef:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        VMOV.F32 S16,S0
//   95 	// I hope magic constants are appropriate here
//   96 	return !cisnan(temperature) && temperature > -50 && temperature < 250;
        VMOV.F32 S0,S16
          CFI FunCall __iar_FDtest
        BL       __iar_FDtest
        CMP      R0,#+2
        BEQ.N    ??isValidCoolantTemperature_0
        VLDR.W   S0,??DataTable9_6  ;; 0xc247ffff
        VCMP.F32 S16,S0
        FMSTAT   
        BLT.N    ??isValidCoolantTemperature_0
        VLDR.W   S0,??DataTable10  ;; 0x437a0000
        VCMP.F32 S16,S0
        FMSTAT   
        BPL.N    ??isValidCoolantTemperature_0
        MOVS     R0,#+1
        B.N      ??isValidCoolantTemperature_1
??isValidCoolantTemperature_0:
        MOVS     R0,#+0
??isValidCoolantTemperature_1:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R1,PC}          ;; return
          CFI EndBlock cfiBlock10
//   97 }
//   98 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function _Z27isValidIntakeAirTemperaturef
        THUMB
//   99 bool isValidIntakeAirTemperature(float temperature) {
_Z27isValidIntakeAirTemperaturef:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        VMOV.F32 S16,S0
//  100 	// I hope magic constants are appropriate here
//  101 	return !cisnan(temperature) && temperature > -50 && temperature < 100;
        VMOV.F32 S0,S16
          CFI FunCall __iar_FDtest
        BL       __iar_FDtest
        CMP      R0,#+2
        BEQ.N    ??isValidIntakeAirTemperature_0
        VLDR.W   S0,??DataTable9_6  ;; 0xc247ffff
        VCMP.F32 S16,S0
        FMSTAT   
        BLT.N    ??isValidIntakeAirTemperature_0
        VLDR.W   S0,??DataTable10_1  ;; 0x42c80000
        VCMP.F32 S16,S0
        FMSTAT   
        BPL.N    ??isValidIntakeAirTemperature_0
        MOVS     R0,#+1
        B.N      ??isValidIntakeAirTemperature_1
??isValidIntakeAirTemperature_0:
        MOVS     R0,#+0
??isValidIntakeAirTemperature_1:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R1,PC}          ;; return
          CFI EndBlock cfiBlock11
//  102 }
//  103 
//  104 /**
//  105  * @return coolant temperature, in Celsius
//  106  */

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function _Z21getCoolantTemperaturev
        THUMB
//  107 float getCoolantTemperature(DECLARE_ENGINE_PARAMETER_F) {
_Z21getCoolantTemperaturev:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
//  108 	float temperature = getTemperatureC(&engineConfiguration->clt, &engine->engineState.cltCurve);
        LDR.W    R0,??DataTable15_18
        LDR      R0,[R0, #+0]
        ADD      R1,R0,#+7680
        ADDS     R1,R1,#+132
        LDR.W    R0,??DataTable15_14
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+256
          CFI FunCall _Z15getTemperatureCP14ThermistorConfP14ThermistorMath
        BL       _Z15getTemperatureCP14ThermistorConfP14ThermistorMath
        VMOV.F32 S16,S0
//  109 	if (!isValidCoolantTemperature(temperature)) {
        VMOV.F32 S0,S16
          CFI FunCall _Z25isValidCoolantTemperaturef
        BL       _Z25isValidCoolantTemperaturef
        CMP      R0,#+0
        BNE.N    ??getCoolantTemperature_0
//  110 		efiAssert(engineConfiguration!=NULL, "NULL engineConfiguration", NAN);
        LDR.W    R0,??DataTable15_14
        LDR      R0,[R0, #+0]
        CMP      R0,#+0
        BNE.N    ??getCoolantTemperature_1
        LDR.W    R0,??DataTable15_19
          CFI FunCall firmwareError
        BL       firmwareError
        VLDR.W   S0,??DataTable9  ;; 0x7fffffff
        B.N      ??getCoolantTemperature_2
//  111 		if (engineConfiguration->hasCltSensor) {
??getCoolantTemperature_1:
        LDR.W    R0,??DataTable15_14
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1496]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+7
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        CMP      R0,#+0
        BEQ.N    ??getCoolantTemperature_3
//  112 			warning(OBD_PCM_Processor_Fault, "unrealistic CLT %f", temperature);
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.W    R1,??DataTable15_20
        MOVW     R0,#+606
          CFI FunCall warning
        BL       warning
//  113 		}
//  114 		return LIMPING_MODE_CLT_TEMPERATURE;
??getCoolantTemperature_3:
        VLDR.W   S0,??DataTable13  ;; 0x428c0000
        B.N      ??getCoolantTemperature_2
//  115 	}
//  116 	return temperature;
??getCoolantTemperature_0:
        VMOV.F32 S0,S16
??getCoolantTemperature_2:
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock12
//  117 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9:
        DC32     0x7fffffff

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_1:
        DC32     0x0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_2:
        DC32     0x42000000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_3:
        DC32     0xc2000000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_4:
        DC32     0xc3889333

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_5:
        DC32     0x3a400c01

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_6:
        DC32     0xc247ffff
//  118 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock13 Using cfiCommon0
          CFI Function _Z26setThermistorConfigurationP14ThermistorConfffffff
          CFI NoCalls
        THUMB
//  119 void setThermistorConfiguration(ThermistorConf * thermistor, float tempC1, float r1, float tempC2, float r2, float tempC3,
//  120 		float r3) {
//  121 	thermistor_conf_s *tc = &thermistor->config;
//  122 	tc->tempC_1 = tempC1;
_Z26setThermistorConfigurationP14ThermistorConfffffff:
        VSTR     S0,[R0, #0]
//  123 	tc->resistance_1 = r1;
        VSTR     S1,[R0, #+12]
//  124 
//  125 	tc->tempC_2 = tempC2;
        VSTR     S2,[R0, #+4]
//  126 	tc->resistance_2 = r2;
        VSTR     S3,[R0, #+16]
//  127 
//  128 	tc->tempC_3 = tempC3;
        VSTR     S4,[R0, #+8]
//  129 	tc->resistance_3 = r3;
        VSTR     S5,[R0, #+20]
//  130 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock13
//  131 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock14 Using cfiCommon0
          CFI Function _Z22prepareThermistorCurveP17thermistor_conf_sP18thermistor_curve_s
        THUMB
//  132 static void prepareThermistorCurve(thermistor_conf_s *tc, thermistor_curve_s * curve) {
_Z22prepareThermistorCurveP17thermistor_conf_sP18thermistor_curve_s:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        VPUSH    {D8-D13}
          CFI D13 Frame(CFA, -24)
          CFI D12 Frame(CFA, -32)
          CFI D11 Frame(CFA, -40)
          CFI D10 Frame(CFA, -48)
          CFI D9 Frame(CFA, -56)
          CFI D8 Frame(CFA, -64)
          CFI CFA R13+64
        SUB      SP,SP,#+16
          CFI CFA R13+80
        MOVS     R4,R0
        MOVS     R5,R1
//  133 	float T1 = tc->tempC_1 + KELV;
        VLDR     S0,[R4, #0]
        VLDR.W   S1,??DataTable12  ;; 0x43889333
        VADD.F32 S16,S0,S1
//  134 	float T2 = tc->tempC_2 + KELV;
        VLDR     S0,[R4, #+4]
        VLDR.W   S1,??DataTable12  ;; 0x43889333
        VADD.F32 S17,S0,S1
//  135 	float T3 = tc->tempC_3 + KELV;
        VLDR     S0,[R4, #+8]
        VLDR.W   S1,??DataTable12  ;; 0x43889333
        VADD.F32 S18,S0,S1
//  136 	scheduleMsg(logger, "T1=%..100000f/T2=%..100000f/T3=%..100000f", T1, T2, T3);
        VMOV     R0,S18
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+8]
        VMOV     R0,S17
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.W    R1,??DataTable15_21
        LDR.W    R0,??DataTable15_22
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  137 
//  138 	float L1 = logf(tc->resistance_1);
        VLDR     S0,[R4, #+12]
          CFI FunCall logf
        BL       logf
        VMOV.F32 S19,S0
//  139 	if (L1 == tc->resistance_1) {
        VLDR     S0,[R4, #+12]
        VCMP.F32 S19,S0
        FMSTAT   
        BNE.N    ??prepareThermistorCurve_0
//  140 		firmwareError("log is broken?");
        LDR.N    R0,??DataTable15_23
          CFI FunCall firmwareError
        BL       firmwareError
//  141 	}
//  142 	float L2 = logf(tc->resistance_2);
??prepareThermistorCurve_0:
        VLDR     S0,[R4, #+16]
          CFI FunCall logf
        BL       logf
        VMOV.F32 S20,S0
//  143 	float L3 = logf(tc->resistance_3);
        VLDR     S0,[R4, #+20]
          CFI FunCall logf
        BL       logf
        VMOV.F32 S21,S0
//  144 	scheduleMsg(logger, "R1=%..100000f/R2=%..100000f/R3=%..100000f", tc->resistance_1, tc->resistance_2,
//  145 			tc->resistance_3);
        LDR      R0,[R4, #+20]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+8]
        LDR      R0,[R4, #+16]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        LDR      R0,[R4, #+12]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R1,??DataTable15_24
        LDR.N    R0,??DataTable15_22
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  146 	scheduleMsg(logger, "L1=%..100000f/L2=%..100000f/L3=%..100000f", L1, L2, L3);
        VMOV     R0,S21
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+8]
        VMOV     R0,S20
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        VMOV     R0,S19
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R1,??DataTable15_25
        LDR.N    R0,??DataTable15_22
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  147 
//  148 	float Y1 = 1 / T1;
        VMOV.F32 S0,#1.0
        VDIV.F32 S22,S0,S16
//  149 	float Y2 = 1 / T2;
        VMOV.F32 S0,#1.0
        VDIV.F32 S23,S0,S17
//  150 	float Y3 = 1 / T3;
        VMOV.F32 S0,#1.0
        VDIV.F32 S24,S0,S18
//  151 
//  152 	scheduleMsg(logger, "Y1=%..100000f/Y2=%..100000f/Y3=%..100000f", Y1, Y2, Y3);
        VMOV     R0,S24
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+8]
        VMOV     R0,S23
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        VMOV     R0,S22
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R1,??DataTable15_26
        LDR.N    R0,??DataTable15_22
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  153 
//  154 	float U2 = (Y2 - Y1) / (L2 - L1);
        VSUB.F32 S0,S23,S22
        VSUB.F32 S1,S20,S19
        VDIV.F32 S25,S0,S1
//  155 	float U3 = (Y3 - Y1) / (L3 - L1);
        VSUB.F32 S0,S24,S22
        VSUB.F32 S1,S21,S19
        VDIV.F32 S26,S0,S1
//  156 
//  157 	scheduleMsg(logger, "U2=%..100000f/U3=%..100000f", U2, U3);
        VMOV     R0,S26
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        VMOV     R0,S25
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R1,??DataTable15_27
        LDR.N    R0,??DataTable15_22
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  158 
//  159 	curve->s_h_c = (U3 - U2) / (L3 - L2) * pow(L1 + L2 + L3, -1);
        MOVS     R0,#-1
        VADD.F32 S0,S19,S20
        VADD.F32 S0,S0,S21
        MOVS     R1,R0
        CMP      R0,#+0
        BPL.N    ??prepareThermistorCurve_1
        RSBS     R1,R1,#+0
??prepareThermistorCurve_1:
        VMOV.F32 S1,#1.0
??prepareThermistorCurve_2:
        LSLS     R2,R1,#+31
        BPL.N    ??prepareThermistorCurve_3
        VMUL.F32 S1,S1,S0
??prepareThermistorCurve_3:
        LSRS     R1,R1,#+1
        CMP      R1,#+0
        BEQ.N    ??prepareThermistorCurve_4
        VMUL.F32 S0,S0,S0
        B.N      ??prepareThermistorCurve_2
??prepareThermistorCurve_4:
        CMP      R0,#+0
        BPL.N    ??prepareThermistorCurve_5
        VCMP.F32 S1,#0.0
        FMSTAT   
        BNE.N    ??prepareThermistorCurve_6
        VLDR.W   S1,??DataTable15  ;; 0x7f800000
        B.N      ??prepareThermistorCurve_7
??prepareThermistorCurve_6:
        VMOV.F32 S0,#1.0
        VDIV.F32 S1,S0,S1
        B.N      ??prepareThermistorCurve_7
??prepareThermistorCurve_5:
??prepareThermistorCurve_7:
        VSUB.F32 S0,S26,S25
        VSUB.F32 S2,S21,S20
        VDIV.F32 S0,S0,S2
        VMUL.F32 S0,S0,S1
        VSTR     S0,[R5, #+8]
//  160 	curve->s_h_b = U2 - curve->s_h_c * (L1 * L1 + L1 * L2 + L2 * L2);
        VMUL.F32 S0,S19,S19
        VMLA.F32 S0,S19,S20
        VMLA.F32 S0,S20,S20
        VLDR     S1,[R5, #+8]
        VMOV.F32 S2,S25
        VMLS.F32 S2,S0,S1
        VSTR     S2,[R5, #+4]
//  161 	curve->s_h_a = Y1 - (curve->s_h_b + L1 * L1 * curve->s_h_c) * L1;
        VMUL.F32 S0,S19,S19
        VLDR     S1,[R5, #+8]
        VLDR     S2,[R5, #+4]
        VMLA.F32 S2,S0,S1
        VMOV.F32 S0,S22
        VMLS.F32 S0,S2,S19
        VSTR     S0,[R5, #0]
//  162 
//  163 	scheduleMsg(logger, "s_h_c=%..100000f/s_h_b=%..100000f/s_h_a=%..100000f", curve->s_h_c, curve->s_h_b,
//  164 			curve->s_h_a);
        LDR      R0,[R5, #+0]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+8]
        LDR      R0,[R5, #+4]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        LDR      R0,[R5, #+8]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R1,??DataTable15_28
        LDR.N    R0,??DataTable15_22
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  165 }
        ADD      SP,SP,#+16
          CFI CFA R13+64
        VPOP     {D8-D13}
          CFI D8 SameValue
          CFI D9 SameValue
          CFI D10 SameValue
          CFI D11 SameValue
          CFI D12 SameValue
          CFI D13 SameValue
          CFI CFA R13+16
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock14

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10:
        DC32     0x437a0000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_1:
        DC32     0x42c80000
//  166 
//  167 /**
//  168  * @return Celsius value
//  169  */

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock15 Using cfiCommon0
          CFI Function _Z23getIntakeAirTemperaturev
        THUMB
//  170 float getIntakeAirTemperature(DECLARE_ENGINE_PARAMETER_F) {
_Z23getIntakeAirTemperaturev:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
//  171 	float temperature = getTemperatureC(&engineConfiguration->iat, &engine->engineState.iatCurve);
        LDR.N    R0,??DataTable15_18
        LDR      R0,[R0, #+0]
        ADD      R1,R0,#+7680
        ADDS     R1,R1,#+92
        LDR.N    R0,??DataTable15_14
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+288
          CFI FunCall _Z15getTemperatureCP14ThermistorConfP14ThermistorMath
        BL       _Z15getTemperatureCP14ThermistorConfP14ThermistorMath
        VMOV.F32 S16,S0
//  172 	if (!isValidIntakeAirTemperature(temperature)) {
        VMOV.F32 S0,S16
          CFI FunCall _Z27isValidIntakeAirTemperaturef
        BL       _Z27isValidIntakeAirTemperaturef
        CMP      R0,#+0
        BNE.N    ??getIntakeAirTemperature_0
//  173 		efiAssert(engineConfiguration!=NULL, "NULL engineConfiguration", NAN);
        LDR.N    R0,??DataTable15_14
        LDR      R0,[R0, #+0]
        CMP      R0,#+0
        BNE.N    ??getIntakeAirTemperature_1
        LDR.N    R0,??DataTable15_19
          CFI FunCall firmwareError
        BL       firmwareError
        VLDR.W   S0,??DataTable15_1  ;; 0x7fffffff
        B.N      ??getIntakeAirTemperature_2
//  174 		if (engineConfiguration->hasIatSensor) {
??getIntakeAirTemperature_1:
        LDR.N    R0,??DataTable15_14
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1496]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+1
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??getIntakeAirTemperature_3
//  175 			warning(OBD_PCM_Processor_Fault, "unrealistic IAT %f", temperature);
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R1,??DataTable15_29
        MOVW     R0,#+606
          CFI FunCall warning
        BL       warning
//  176 		}
//  177 		return LIMPING_MODE_IAT_TEMPERATURE;
??getIntakeAirTemperature_3:
        VMOV.F32 S0,#30.0
        B.N      ??getIntakeAirTemperature_2
//  178 	}
//  179 	return temperature;
??getIntakeAirTemperature_0:
        VMOV.F32 S0,S16
??getIntakeAirTemperature_2:
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock15
//  180 }
//  181 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock16 Using cfiCommon0
          CFI Function _Z14setDodgeSensorP14ThermistorConf
        THUMB
//  182 void setDodgeSensor(ThermistorConf *thermistorConf) {
_Z14setDodgeSensorP14ThermistorConf:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  183 	setThermistorConfiguration(thermistorConf, -40, 336660, 30, 7550, 120, 390);
        VLDR.W   S5,??DataTable15_2  ;; 0x43c30000
        VLDR.W   S4,??DataTable15_3  ;; 0x42f00000
        VLDR.W   S3,??DataTable15_4  ;; 0x45ebf000
        VMOV.F32 S2,#30.0
        VLDR.W   S1,??DataTable15_5  ;; 0x48a46280
        VLDR.W   S0,??DataTable15_6  ;; 0xc2200000
        MOVS     R0,R4
          CFI FunCall _Z26setThermistorConfigurationP14ThermistorConfffffff
        BL       _Z26setThermistorConfigurationP14ThermistorConfffffff
//  184 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock16

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12:
        DC32     0x43889333
//  185 
//  186 // todo: better method name?

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock17 Using cfiCommon0
          CFI Function _Z18setCommonNTCSensorP14ThermistorConf
        THUMB
//  187 void setCommonNTCSensor(ThermistorConf *thermistorConf) {
_Z18setCommonNTCSensorP14ThermistorConf:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  188 	/**
//  189 	 * 18K Ohm @ -20C
//  190 	 * 2.1K Ohm @ 24C
//  191 	 * 294 Ohm @ 80C
//  192 	 * http://www.rexbo.eu/hella/coolant-temperature-sensor-6pt009107121?c=100334&at=3130
//  193 	 */
//  194 	setThermistorConfiguration(thermistorConf, -20, 18000, 23.8889, 2100, 120.0, 100.0);
        VLDR.W   S5,??DataTable15_7  ;; 0x42c80000
        VLDR.W   S4,??DataTable15_3  ;; 0x42f00000
        VLDR.W   S3,??DataTable15_8  ;; 0x45034000
        VLDR.W   S2,??DataTable15_9  ;; 0x41bf1c78
        VLDR.W   S1,??DataTable15_10  ;; 0x468ca000
        VMOV.F32 S0,#-20.0
        MOVS     R0,R4
          CFI FunCall _Z26setThermistorConfigurationP14ThermistorConfffffff
        BL       _Z26setThermistorConfigurationP14ThermistorConfffffff
//  195 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock17

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13:
        DC32     0x428c0000
//  196 
//  197 #if EFI_PROD_CODE

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock18 Using cfiCommon0
          CFI Function _Z10testCltByRf
        THUMB
//  198 static void testCltByR(float resistance) {
_Z10testCltByRf:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        SUB      SP,SP,#+8
          CFI CFA R13+24
        VMOV.F32 S16,S0
//  199 	float kTemp = getKelvinTemperature(&engineConfiguration->clt, resistance, &engine->engineState.cltCurve);
        LDR.N    R0,??DataTable15_18
        LDR      R0,[R0, #+0]
        ADD      R1,R0,#+7680
        ADDS     R1,R1,#+132
        VMOV.F32 S0,S16
        LDR.N    R0,??DataTable15_14
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+256
          CFI FunCall _Z20getKelvinTemperatureP14ThermistorConffP14ThermistorMath
        BL       _Z20getKelvinTemperatureP14ThermistorConffP14ThermistorMath
        VMOV.F32 S17,S0
//  200 	scheduleMsg(logger, "for R=%f we have %f", resistance, (kTemp - KELV));
        VLDR.W   S0,??DataTable15_11  ;; 0xc3889333
        VADD.F32 S0,S17,S0
        VMOV     R0,S0
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R1,??DataTable15_30
        LDR.N    R0,??DataTable15_22
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  201 }
        ADD      SP,SP,#+8
          CFI CFA R13+16
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock18
//  202 #endif
//  203 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock19 Using cfiCommon0
          CFI Function _Z15initThermistorsP7Logging
        THUMB
//  204 void initThermistors(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_S) {
_Z15initThermistorsP7Logging:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  205 	logger = sharedLogger;
        LDR.N    R0,??DataTable15_22
        STR      R4,[R0, #+0]
//  206 	efiAssertVoid(engine!=NULL, "e NULL initThermistors");
        LDR.N    R0,??DataTable15_18
        LDR      R0,[R0, #+0]
        CMP      R0,#+0
        BNE.N    ??initThermistors_0
        LDR.N    R0,??DataTable15_31
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??initThermistors_1
//  207 	efiAssertVoid(engine->engineConfiguration2!=NULL, "e2 NULL initThermistors");
??initThermistors_0:
        LDR.N    R0,??DataTable15_18
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+40]
        CMP      R0,#+0
        BNE.N    ??initThermistors_2
        LDR.N    R0,??DataTable15_32
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??initThermistors_1
//  208 
//  209 #if EFI_PROD_CODE
//  210 	addConsoleActionF("test_clt_by_r", testCltByR);
??initThermistors_2:
        LDR.N    R1,??DataTable15_33
        LDR.N    R0,??DataTable15_34
          CFI FunCall addConsoleActionF
        BL       addConsoleActionF
//  211 #endif
//  212 
//  213 	initialized = true;
        LDR.N    R0,??DataTable15_16
        MOVS     R1,#+1
        STRB     R1,[R0, #+0]
//  214 }
??initThermistors_1:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock19

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15:
        DC32     0x7f800000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_1:
        DC32     0x7fffffff

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_2:
        DC32     0x43c30000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_3:
        DC32     0x42f00000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_4:
        DC32     0x45ebf000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_5:
        DC32     0x48a46280

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_6:
        DC32     0xc2200000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_7:
        DC32     0x42c80000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_8:
        DC32     0x45034000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_9:
        DC32     0x41bf1c78

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_10:
        DC32     0x468ca000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_11:
        DC32     0xc3889333

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_12:
        DC32     `?<Constant "thermistor pointer is...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_13:
        DC32     `?<Constant "term">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_14:
        DC32     engineConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_15:
        DC32     `?<Constant "thermistor config is ...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_16:
        DC32     initialized

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_17:
        DC32     `?<Constant "thermstr not initialized">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_18:
        DC32     engine

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_19:
        DC32     `?<Constant "NULL engineConfiguration">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_20:
        DC32     `?<Constant "unrealistic CLT %f">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_21:
        DC32     `?<Constant "T1=%..100000f/T2=%..1...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_22:
        DC32     logger

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_23:
        DC32     `?<Constant "log is broken?">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_24:
        DC32     `?<Constant "R1=%..100000f/R2=%..1...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_25:
        DC32     `?<Constant "L1=%..100000f/L2=%..1...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_26:
        DC32     `?<Constant "Y1=%..100000f/Y2=%..1...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_27:
        DC32     `?<Constant "U2=%..100000f/U3=%..1...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_28:
        DC32     `?<Constant "s_h_c=%..100000f/s_h_...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_29:
        DC32     `?<Constant "unrealistic IAT %f">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_30:
        DC32     `?<Constant "for R=%f we have %f">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_31:
        DC32     `?<Constant "e NULL initThermistors">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_32:
        DC32     `?<Constant "e2 NULL initThermistors">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_33:
        DC32     _Z10testCltByRf

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_34:
        DC32     `?<Constant "test_clt_by_r">`
//  215 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock20 Using cfiCommon0
          CFI Function _ZN14ThermistorMathC1Ev
        THUMB
//  216 ThermistorMath::ThermistorMath() {
_ZN14ThermistorMathC1Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  217 	memset(&currentConfig, 0, sizeof(currentConfig));
        MOVS     R2,#+28
        MOVS     R1,#+0
        ADDS     R0,R4,#+12
          CFI FunCall memset
        BL       memset
//  218 	memset(&curve, 0, sizeof(curve));
        MOVS     R2,#+12
        MOVS     R1,#+0
        MOVS     R0,R4
          CFI FunCall memset
        BL       memset
//  219 }
        MOVS     R0,R4
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock20

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock21 Using cfiCommon0
          CFI Function _ZN14ThermistorMath4initEP17thermistor_conf_s
        THUMB
_ZN14ThermistorMath4initEP17thermistor_conf_s:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R2,#+28
        ADDS     R1,R4,#+12
        MOVS     R0,R5
          CFI FunCall memcmp
        BL       memcmp
        CMP      R0,#+0
        BNE.N    ??init_0
        MOVS     R6,#+1
        B.N      ??init_1
??init_0:
        MOVS     R6,#+0
??init_1:
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        CMP      R6,#+0
        BNE.N    ??init_2
??init_3:
        MOVS     R2,#+28
        MOVS     R1,R5
        ADDS     R0,R4,#+12
          CFI FunCall memcpy
        BL       memcpy
        MOVS     R1,R4
        MOVS     R0,R5
          CFI FunCall _Z22prepareThermistorCurveP17thermistor_conf_sP18thermistor_curve_s
        BL       _Z22prepareThermistorCurveP17thermistor_conf_sP18thermistor_curve_s
??init_2:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock21

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP memcpy
          CFI Block cfiBlock22 Using cfiCommon0
          CFI Function memcpy
        THUMB
// __intrinsic __nounwind __interwork __softfp void *memcpy(void *, void const *, size_t)
memcpy:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
        MOVS     R2,R6
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall __aeabi_memcpy
        BL       __aeabi_memcpy
        MOVS     R0,R4
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock22

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP memset
          CFI Block cfiBlock23 Using cfiCommon0
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
          CFI EndBlock cfiBlock23

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "thermistor pointer is...">`:
        DATA
        DC8 "thermistor pointer is NULL"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "term">`:
        DATA
        DC8 "term"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "thermistor config is ...">`:
        DATA
        DC8 "thermistor config is null"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "thermstr not initialized">`:
        DATA
        DC8 "thermstr not initialized"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "NULL engineConfiguration">`:
        DATA
        DC8 "NULL engineConfiguration"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "unrealistic CLT %f">`:
        DATA
        DC8 "unrealistic CLT %f"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "T1=%..100000f/T2=%..1...">`:
        DATA
        DC8 "T1=%..100000f/T2=%..100000f/T3=%..100000f"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "log is broken?">`:
        DATA
        DC8 "log is broken?"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "R1=%..100000f/R2=%..1...">`:
        DATA
        DC8 "R1=%..100000f/R2=%..100000f/R3=%..100000f"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "L1=%..100000f/L2=%..1...">`:
        DATA
        DC8 "L1=%..100000f/L2=%..100000f/L3=%..100000f"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Y1=%..100000f/Y2=%..1...">`:
        DATA
        DC8 "Y1=%..100000f/Y2=%..100000f/Y3=%..100000f"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "U2=%..100000f/U3=%..1...">`:
        DATA
        DC8 "U2=%..100000f/U3=%..100000f"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "s_h_c=%..100000f/s_h_...">`:
        DATA
        DC8 73H, 5FH, 68H, 5FH, 63H, 3DH, 25H, 2EH
        DC8 2EH, 31H, 30H, 30H, 30H, 30H, 30H, 66H
        DC8 2FH, 73H, 5FH, 68H, 5FH, 62H, 3DH, 25H
        DC8 2EH, 2EH, 31H, 30H, 30H, 30H, 30H, 30H
        DC8 66H, 2FH, 73H, 5FH, 68H, 5FH, 61H, 3DH
        DC8 25H, 2EH, 2EH, 31H, 30H, 30H, 30H, 30H
        DC8 30H, 66H, 0
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "unrealistic IAT %f">`:
        DATA
        DC8 "unrealistic IAT %f"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "for R=%f we have %f">`:
        DATA
        DC8 "for R=%f we have %f"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "e NULL initThermistors">`:
        DATA
        DC8 "e NULL initThermistors"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "e2 NULL initThermistors">`:
        DATA
        DC8 "e2 NULL initThermistors"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "test_clt_by_r">`:
        DATA
        DC8 "test_clt_by_r"
        DC8 0, 0

        END
//  220 
//  221 void ThermistorMath::init(thermistor_conf_s *config) {
//  222 	bool_t isSameConfig = memcmp(config, &currentConfig, sizeof(currentConfig)) == 0;
//  223 	if (isSameConfig) {
//  224 		return;
//  225 	}
//  226 	memcpy(&currentConfig, config, sizeof(currentConfig));
//  227 	prepareThermistorCurve(config, &curve);
//  228 }
// 
//     5 bytes in section .bss
//   516 bytes in section .rodata
// 1 938 bytes in section .text
// 
// 1 894 bytes of CODE  memory (+ 44 bytes shared)
//   516 bytes of CONST memory
//     5 bytes of DATA  memory
//
//Errors: none
//Warnings: 1
