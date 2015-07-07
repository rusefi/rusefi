///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:08:55 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\controllers\sen /
//                       sors\map.cpp                                         /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\controllers\sen /
//                       sors\map.cpp -lCN F:\stuff\rusefi_sourceforge\firmwa /
//                       re\iar\Debug\List\ -lA F:\stuff\rusefi_sourceforge\f /
//                       irmware\iar\Debug\List\ -o                           /
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
//                       map.s                                                /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME map

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1
        #define SHT_INIT_ARRAY 0xe

        EXTERN _Z10hwPortname11brain_pin_e
        EXTERN _Z11interpolatefffff
        EXTERN _Z16startInputDriverP15digital_input_sb
        EXTERN _Z19getInternalAdcValuePKc13adc_channel_e
        EXTERN _Z22initWaveAnalyzerDriverPKc11brain_pin_e
        EXTERN _Z29getAir_pressure_sensor_type_e26air_pressure_sensor_type_e
        EXTERN _Z6getMapv
        EXTERN _ZN16IntListenerArray16registerCallbackEPFviEPv
        EXTERN _ZN17FastInterpolation4initEffff
        EXTERN _ZN17FastInterpolation8getValueEf
        EXTERN _ZN17FastInterpolationC1Effff
        EXTERN _ZN17FastInterpolationC1Ev
        EXTERN __aeabi_d2f
        EXTERN __aeabi_ddiv
        EXTERN __aeabi_f2d
        EXTERN __aeabi_l2d
        EXTERN __aeabi_ldivmod
        EXTERN addConsoleAction
        EXTERN boardConfiguration
        EXTERN engineConfiguration
        EXTERN firmwareError
        EXTERN getTimeNowNt
        EXTERN scheduleMsg

        PUBLIC _Z13hasBaroSensorv
        PUBLIC _Z14decodePressurefP28air_pressure_sensor_config_s
        PUBLIC _Z14initMapDecoderP7Logging
        PUBLIC _Z15getBaroPressurev
        PUBLIC _Z15getMapByVoltagef
        PUBLIC _Z9getRawMapv
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\controllers\sensors\map.cpp
//    1 #include "main.h"
//    2 #include "engine_configuration.h"
//    3 #include "engine_math.h"
//    4 #include "adc_inputs.h"
//    5 #include "interpolation.h"
//    6 #include "error_handling.h"
//    7 #include "map.h"
//    8 
//    9 #if EFI_PROD_CODE || defined(__DOXYGEN__)
//   10 #include "digital_input_hw.h"
//   11 #include "pin_repository.h"
//   12 #endif
//   13 
//   14 #if EFI_ANALOG_SENSORS || defined(__DOXYGEN__)
//   15 
//   16 EXTERN_ENGINE;
//   17 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   18 static Logging *logger;
logger:
        DS8 4
//   19 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function __sti__routine
        THUMB
// static __intrinsic __interwork __softfp void __sti__routine()
__sti__routine:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   20 static FastInterpolation customMap;
        LDR.W    R0,??DataTable10_1
          CFI FunCall _ZN17FastInterpolationC1Ev
        BL       _ZN17FastInterpolationC1Ev
//   21 static efitick_t prevWidthTimeNt = 0;
//   22 
//   23 static float mapFreq = 0;
//   24 
//   25 /**
//   26  * @brief	MAP value decoded for a 1.83 Honda sensor
//   27  * -6.64kPa at zero volts
//   28  * 182.78kPa at 5 volts
//   29  *
//   30  * about 3 volts at 100kPa
//   31  *
//   32  * @returns kPa value
//   33  */
//   34 static FastInterpolation denso183(0, -6.64, 5, 182.78);
        VLDR.W   S3,??DataTable5  ;; 0x4336c7ae
        VMOV.F32 S2,#5.0
        VLDR.W   S1,??DataTable5_1  ;; 0xc0d47ae1
        VLDR.W   S0,??DataTable5_2  ;; 0x0
        LDR.W    R0,??DataTable10_2
          CFI FunCall _ZN17FastInterpolationC1Effff
        BL       _ZN17FastInterpolationC1Effff
//   35 
//   36 /**
//   37  * MAP sensor output voltage of 3.0v = a gauge reading of 0 in. Hg
//   38  * MAP sensor output voltage of 0.5v = a gauge reading of 27 in. Hg
//   39  */
//   40 static FastInterpolation honda3bar(0.5, 91.422, 3.0, 0);
        VLDR.W   S3,??DataTable5_2  ;; 0x0
        VMOV.F32 S2,#3.0
        VLDR.W   S1,??DataTable5_3  ;; 0x42b6d810
        VMOV.F32 S0,#0.5
        LDR.W    R0,??DataTable10_3
          CFI FunCall _ZN17FastInterpolationC1Effff
        BL       _ZN17FastInterpolationC1Effff
//   41 
//   42 static FastInterpolation subyDenso(0, 0, 5, 200);
        VLDR.W   S3,??DataTable5_4  ;; 0x43480000
        VMOV.F32 S2,#5.0
        VLDR.W   S1,??DataTable5_2  ;; 0x0
        VLDR.W   S0,??DataTable5_2  ;; 0x0
        LDR.W    R0,??DataTable10_4
          CFI FunCall _ZN17FastInterpolationC1Effff
        BL       _ZN17FastInterpolationC1Effff
//   43 
//   44 static FastInterpolation gm3bar(0.631, 40, 4.914, 304);
        VLDR.W   S3,??DataTable5_5  ;; 0x43980000
        VLDR.W   S2,??DataTable5_6  ;; 0x409d3f7d
        VLDR.W   S1,??DataTable6  ;; 0x42200000
        VLDR.W   S0,??DataTable6_1  ;; 0x3f218937
        LDR.W    R0,??DataTable10_5
          CFI FunCall _ZN17FastInterpolationC1Effff
        BL       _ZN17FastInterpolationC1Effff
//   45 
//   46 static FastInterpolation mpx4250(0, 8, 5, 260);
        VLDR.W   S3,??DataTable6_2  ;; 0x43820000
        VMOV.F32 S2,#5.0
        VMOV.F32 S1,#8.0
        VLDR.W   S0,??DataTable5_2  ;; 0x0
        LDR.W    R0,??DataTable10_6
          CFI FunCall _ZN17FastInterpolationC1Effff
        BL       _ZN17FastInterpolationC1Effff
//   47 
//   48 static FastInterpolation dodgeNeon2003(0.5, 0, 4.5, 100);
        VLDR.W   S3,??DataTable6_3  ;; 0x42c80000
        VMOV.F32 S2,#4.5
        VLDR.W   S1,??DataTable5_2  ;; 0x0
        VMOV.F32 S0,#0.5
        LDR.W    R0,??DataTable10_7
          CFI FunCall _ZN17FastInterpolationC1Effff
        BL       _ZN17FastInterpolationC1Effff
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock0

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
customMap:
        DS8 8

        SECTION `.bss`:DATA:REORDER:NOROOT(3)
prevWidthTimeNt:
        DS8 8

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
mapFreq:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
denso183:
        DS8 8

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
honda3bar:
        DS8 8

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
subyDenso:
        DS8 8

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
gm3bar:
        DS8 8

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
mpx4250:
        DS8 8

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
dodgeNeon2003:
        DS8 8
//   49 
//   50 /**
//   51  * We hold a reference to current decoder to reduce code branching
//   52  * to lookup decoder each time we need to decode
//   53  */

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   54 static FastInterpolation *mapDecoder;
mapDecoder:
        DS8 4
//   55 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z14decodePressurefP28air_pressure_sensor_config_s
        THUMB
//   56 float decodePressure(float voltage, air_pressure_sensor_config_s * config) {
_Z14decodePressurefP28air_pressure_sensor_config_s:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        VMOV.F32 S16,S0
        MOVS     R4,R0
//   57 	switch (config->type) {
        LDR      R0,[R4, #+8]
        CMP      R0,#+0
        BEQ.N    ??decodePressure_0
        CMP      R0,#+2
        BEQ.N    ??decodePressure_1
        BCC.N    ??decodePressure_2
        CMP      R0,#+4
        BEQ.N    ??decodePressure_3
        BCC.N    ??decodePressure_4
        CMP      R0,#+6
        BEQ.N    ??decodePressure_5
        BCC.N    ??decodePressure_6
        B.N      ??decodePressure_7
//   58 	case MT_CUSTOM:
//   59 		// todo: migrate to 'FastInterpolation customMap'
//   60 		return interpolate(0, config->valueAt0, 5, config->valueAt5, voltage);
??decodePressure_0:
        VMOV.F32 S4,S16
        VLDR     S3,[R4, #+4]
        VMOV.F32 S2,#5.0
        VLDR     S1,[R4, #0]
        VLDR.W   S0,??DataTable5_2  ;; 0x0
          CFI FunCall _Z11interpolatefffff
        BL       _Z11interpolatefffff
        B.N      ??decodePressure_8
//   61 	case MT_DENSO183:
//   62 		return denso183.getValue(voltage);
??decodePressure_2:
        VMOV.F32 S0,S16
        LDR.W    R0,??DataTable10_2
          CFI FunCall _ZN17FastInterpolation8getValueEf
        BL       _ZN17FastInterpolation8getValueEf
        B.N      ??decodePressure_8
//   63 	case MT_MPX4250:
//   64 		return mpx4250.getValue(voltage);
??decodePressure_1:
        VMOV.F32 S0,S16
        LDR.W    R0,??DataTable10_6
          CFI FunCall _ZN17FastInterpolation8getValueEf
        BL       _ZN17FastInterpolation8getValueEf
        B.N      ??decodePressure_8
//   65 	case MT_HONDA3BAR:
//   66 		return honda3bar.getValue(voltage);
??decodePressure_4:
        VMOV.F32 S0,S16
        LDR.W    R0,??DataTable10_3
          CFI FunCall _ZN17FastInterpolation8getValueEf
        BL       _ZN17FastInterpolation8getValueEf
        B.N      ??decodePressure_8
//   67 	case MT_DODGE_NEON_2003:
//   68 		return dodgeNeon2003.getValue(voltage);
??decodePressure_3:
        VMOV.F32 S0,S16
        LDR.W    R0,??DataTable10_7
          CFI FunCall _ZN17FastInterpolation8getValueEf
        BL       _ZN17FastInterpolation8getValueEf
        B.N      ??decodePressure_8
//   69 	case MT_SUBY_DENSO:
//   70 		return subyDenso.getValue(voltage);
??decodePressure_6:
        VMOV.F32 S0,S16
        LDR.W    R0,??DataTable10_4
          CFI FunCall _ZN17FastInterpolation8getValueEf
        BL       _ZN17FastInterpolation8getValueEf
        B.N      ??decodePressure_8
//   71 	case MT_GM_3_BAR:
//   72 		return gm3bar.getValue(voltage);
??decodePressure_5:
        VMOV.F32 S0,S16
        LDR.W    R0,??DataTable10_5
          CFI FunCall _ZN17FastInterpolation8getValueEf
        BL       _ZN17FastInterpolation8getValueEf
        B.N      ??decodePressure_8
//   73 	default:
//   74 		firmwareError("Unknown MAP type: %d", config->type);
??decodePressure_7:
        LDR      R1,[R4, #+8]
        LDR.W    R0,??DataTable10_8
          CFI FunCall firmwareError
        BL       firmwareError
//   75 		return NAN;
        VLDR.W   S0,??DataTable8  ;; 0x7fffffff
??decodePressure_8:
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock1
//   76 	}
//   77 }
//   78 
//   79 /**
//   80  * @brief	MAP value decoded according to current settings
//   81  * @returns kPa value
//   82  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z15getMapByVoltagef
        THUMB
//   83 float getMapByVoltage(float voltage DECLARE_ENGINE_PARAMETER_S) {
_Z15getMapByVoltagef:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        VMOV.F32 S16,S0
//   84 	// todo: migrate to mapDecoder once parameter listeners are ready
//   85 	air_pressure_sensor_config_s * apConfig = &engineConfiguration->map.sensor;
        LDR.W    R0,??DataTable10_9
        LDR      R0,[R0, #+0]
        ADDS     R4,R0,#+240
//   86 	return decodePressure(voltage, apConfig);
        MOVS     R0,R4
        VMOV.F32 S0,S16
          CFI FunCall _Z14decodePressurefP28air_pressure_sensor_config_s
        BL       _Z14decodePressurefP28air_pressure_sensor_config_s
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock2
//   87 }
//   88 
//   89 /**
//   90  * @return Manifold Absolute Pressure, in kPa
//   91  */

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z9getRawMapv
        THUMB
//   92 float getRawMap(DECLARE_ENGINE_PARAMETER_F) {
_Z9getRawMapv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
//   93 	if (engineConfiguration->hasFrequencyReportingMapSensor) {
        LDR.W    R0,??DataTable10_9
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1498]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+2
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??getRawMap_0
//   94 		return interpolate(boardConfiguration->mapFrequency0Kpa, 0, boardConfiguration->mapFrequency100Kpa, 100, mapFreq);
        LDR.W    R0,??DataTable10_10
        VLDR     S4,[R0, #0]
        VLDR.W   S3,??DataTable6_3  ;; 0x42c80000
        LDR.W    R0,??DataTable10_11
        LDR      R0,[R0, #+0]
        VLDR     S2,[R0, #+156]
        VLDR.W   S1,??DataTable5_2  ;; 0x0
        LDR.W    R0,??DataTable10_11
        LDR      R0,[R0, #+0]
        VLDR     S0,[R0, #+152]
          CFI FunCall _Z11interpolatefffff
        BL       _Z11interpolatefffff
        B.N      ??getRawMap_1
//   95 	}
//   96 
//   97 	float voltage = getVoltageDivided("map", engineConfiguration->map.sensor.hwChannel);
??getRawMap_0:
        LDR.N    R0,??DataTable10_9
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+252]
        ADR.N    R0,??DataTable8_1  ;; "map"
          CFI FunCall _Z19getInternalAdcValuePKc13adc_channel_e
        BL       _Z19getInternalAdcValuePKc13adc_channel_e
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        VLDR.W   S1,??DataTable8_2  ;; 0x3a400c01
        VMUL.F32 S0,S0,S1
        LDR.N    R0,??DataTable10_9
        LDR      R0,[R0, #+0]
        VLDR     S1,[R0, #+464]
        VMUL.F32 S16,S0,S1
//   98 	return getMapByVoltage(voltage PASS_ENGINE_PARAMETER);
        VMOV.F32 S0,S16
          CFI FunCall _Z15getMapByVoltagef
        BL       _Z15getMapByVoltagef
??getRawMap_1:
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock3
//   99 }
//  100 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _Z13hasBaroSensorv
          CFI NoCalls
        THUMB
//  101 bool_t hasBaroSensor(DECLARE_ENGINE_PARAMETER_F) {
//  102 	return engineConfiguration->hasBaroSensor && engineConfiguration->baroSensor.hwChannel != EFI_ADC_NONE;
_Z13hasBaroSensorv:
        LDR.N    R0,??DataTable10_9
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1496]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+2
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??hasBaroSensor_0
        LDR.N    R0,??DataTable10_9
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+620]
        CMP      R0,#+16
        BEQ.N    ??hasBaroSensor_0
        MOVS     R0,#+1
        B.N      ??hasBaroSensor_1
??hasBaroSensor_0:
        MOVS     R0,#+0
??hasBaroSensor_1:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        BX       LR               ;; return
          CFI EndBlock cfiBlock4
//  103 }
//  104 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _Z15getBaroPressurev
        THUMB
//  105 float getBaroPressure(DECLARE_ENGINE_PARAMETER_F) {
_Z15getBaroPressurev:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
//  106 	float voltage = getVoltageDivided("baro", engineConfiguration->baroSensor.hwChannel);
        LDR.N    R0,??DataTable10_9
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+620]
        LDR.N    R0,??DataTable10_12
          CFI FunCall _Z19getInternalAdcValuePKc13adc_channel_e
        BL       _Z19getInternalAdcValuePKc13adc_channel_e
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        VLDR.W   S1,??DataTable8_2  ;; 0x3a400c01
        VMUL.F32 S0,S0,S1
        LDR.N    R0,??DataTable10_9
        LDR      R0,[R0, #+0]
        VLDR     S1,[R0, #+464]
        VMUL.F32 S16,S0,S1
//  107 	return decodePressure(voltage, &engineConfiguration->baroSensor);
        LDR.N    R0,??DataTable10_9
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+608
        VMOV.F32 S0,S16
          CFI FunCall _Z14decodePressurefP28air_pressure_sensor_config_s
        BL       _Z14decodePressurefP28air_pressure_sensor_config_s
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock5
//  108 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5:
        DC32     0x4336c7ae

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_1:
        DC32     0xc0d47ae1

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_2:
        DC32     0x0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_3:
        DC32     0x42b6d810

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_4:
        DC32     0x43480000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_5:
        DC32     0x43980000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_6:
        DC32     0x409d3f7d
//  109 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _Z10getDecoder26air_pressure_sensor_type_e
        THUMB
//  110 static FastInterpolation *getDecoder(air_pressure_sensor_type_e type) {
_Z10getDecoder26air_pressure_sensor_type_e:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  111 	switch (type) {
        CMP      R4,#+0
        BEQ.N    ??getDecoder_0
        CMP      R4,#+2
        BEQ.N    ??getDecoder_1
        BCC.N    ??getDecoder_2
        CMP      R4,#+4
        BEQ.N    ??getDecoder_3
        BCC.N    ??getDecoder_4
        CMP      R4,#+6
        BEQ.N    ??getDecoder_5
        BCC.N    ??getDecoder_6
        B.N      ??getDecoder_7
//  112 	case MT_CUSTOM:
//  113 		return &customMap;
??getDecoder_0:
        LDR.N    R0,??DataTable10_1
        B.N      ??getDecoder_8
//  114 	case MT_DENSO183:
//  115 		return &denso183;
??getDecoder_2:
        LDR.N    R0,??DataTable10_2
        B.N      ??getDecoder_8
//  116 	case MT_MPX4250:
//  117 		return &mpx4250;
??getDecoder_1:
        LDR.N    R0,??DataTable10_6
        B.N      ??getDecoder_8
//  118 	case MT_HONDA3BAR:
//  119 		return &honda3bar;
??getDecoder_4:
        LDR.N    R0,??DataTable10_3
        B.N      ??getDecoder_8
//  120 	case MT_DODGE_NEON_2003:
//  121 		return &dodgeNeon2003;
??getDecoder_3:
        LDR.N    R0,??DataTable10_7
        B.N      ??getDecoder_8
//  122 	case MT_SUBY_DENSO:
//  123 		return &subyDenso;
??getDecoder_6:
        LDR.N    R0,??DataTable10_4
        B.N      ??getDecoder_8
//  124 	case MT_GM_3_BAR:
//  125 		return &gm3bar;
??getDecoder_5:
        LDR.N    R0,??DataTable10_5
        B.N      ??getDecoder_8
//  126 	default:
//  127 		firmwareError("Unknown MAP type: %d", type);
??getDecoder_7:
        MOVS     R1,R4
        LDR.N    R0,??DataTable10_8
          CFI FunCall firmwareError
        BL       firmwareError
//  128 		return &customMap;
        LDR.N    R0,??DataTable10_1
??getDecoder_8:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock6
//  129 	}
//  130 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6:
        DC32     0x42200000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_1:
        DC32     0x3f218937

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_2:
        DC32     0x43820000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_3:
        DC32     0x42c80000
//  131 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function _Z18applyConfigurationv
        THUMB
//  132 static void applyConfiguration(DECLARE_ENGINE_PARAMETER_F) {
_Z18applyConfigurationv:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//  133 	air_pressure_sensor_config_s * apConfig = &engineConfiguration->map.sensor;
        LDR.N    R0,??DataTable10_9
        LDR      R0,[R0, #+0]
        ADDS     R4,R0,#+240
//  134 	customMap.init(0, apConfig->valueAt0, 5, apConfig->valueAt5);
        VLDR     S3,[R4, #+4]
        VMOV.F32 S2,#5.0
        VLDR     S1,[R4, #0]
        VLDR.W   S0,??DataTable10  ;; 0x0
        LDR.N    R0,??DataTable10_1
          CFI FunCall _ZN17FastInterpolation4initEffff
        BL       _ZN17FastInterpolation4initEffff
//  135 	mapDecoder = getDecoder(engineConfiguration->map.sensor.type);
        LDR.N    R0,??DataTable10_9
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+248]
          CFI FunCall _Z10getDecoder26air_pressure_sensor_type_e
        BL       _Z10getDecoder26air_pressure_sensor_type_e
        LDR.N    R1,??DataTable10_13
        STR      R0,[R1, #+0]
//  136 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock7
//  137 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function _Z23digitalMapWidthCallbackv
        THUMB
//  138 static void digitalMapWidthCallback(void) {
_Z23digitalMapWidthCallbackv:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
//  139 	efitick_t nowNt = getTimeNowNt();
          CFI FunCall getTimeNowNt
        BL       getTimeNowNt
        MOVS     R4,R0
        MOVS     R5,R1
//  140 
//  141 	mapFreq = 1000000.0 / NT2US(nowNt - prevWidthTimeNt);
        LDR.N    R0,??DataTable10_14
        LDRD     R2,R3,[R0, #+0]
        SUBS     R0,R4,R2
        SBCS     R1,R5,R3
        MOVS     R2,#+168
        MOVS     R3,#+0
          CFI FunCall __aeabi_ldivmod
        BL       __aeabi_ldivmod
          CFI FunCall __aeabi_l2d
        BL       __aeabi_l2d
        MOVS     R2,R0
        MOVS     R3,R1
        MOVS     R0,#+0
        LDR.N    R1,??DataTable10_15  ;; 0x412e8480
          CFI FunCall __aeabi_ddiv
        BL       __aeabi_ddiv
          CFI FunCall __aeabi_d2f
        BL       __aeabi_d2f
        LDR.N    R1,??DataTable10_10
        STR      R0,[R1, #+0]
//  142 
//  143 	prevWidthTimeNt = nowNt;
        LDR.N    R0,??DataTable10_14
        STRD     R4,R5,[R0, #+0]
//  144 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock8

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8:
        DC32     0x7fffffff

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_1:
        DC8      "map"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_2:
        DC32     0x3a400c01
//  145 
//  146 #if EFI_PROD_CODE || defined(__DOXYGEN__)

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function _Z12printMAPInfov
        THUMB
//  147 static void printMAPInfo(void) {
_Z12printMAPInfov:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        SUB      SP,SP,#+8
          CFI CFA R13+24
//  148 #if EFI_ANALOG_SENSORS || defined(__DOXYGEN__)
//  149 	scheduleMsg(logger, "instant value=%fkPa", getRawMap());
          CFI FunCall _Z9getRawMapv
        BL       _Z9getRawMapv
        VMOV     R0,S0
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R1,??DataTable10_16
        LDR.N    R0,??DataTable10_17
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  150 
//  151 	if (engineConfiguration->hasFrequencyReportingMapSensor) {
        LDR.N    R0,??DataTable10_9
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1498]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+2
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??printMAPInfo_0
//  152 		scheduleMsg(logger, "instant value=%fHz @ %s", mapFreq, hwPortname(boardConfiguration->frequencyReportingMapInputPin));
        LDR.N    R0,??DataTable10_11
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+868]
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        STR      R0,[SP, #+0]
        LDR.N    R0,??DataTable10_10
        LDR      R0,[R0, #+0]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R1,??DataTable10_18
        LDR.N    R0,??DataTable10_17
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
        B.N      ??printMAPInfo_1
//  153 	} else {
//  154 		scheduleMsg(logger, "map type=%d/%s MAP=%fkPa", engineConfiguration->map.sensor.type,
//  155 				getAir_pressure_sensor_type_e(engineConfiguration->map.sensor.type),
//  156 				getMap());
??printMAPInfo_0:
          CFI FunCall _Z6getMapv
        BL       _Z6getMapv
        VMOV.F32 S16,S0
        LDR.N    R0,??DataTable10_9
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+248]
          CFI FunCall _Z29getAir_pressure_sensor_type_e26air_pressure_sensor_type_e
        BL       _Z29getAir_pressure_sensor_type_e26air_pressure_sensor_type_e
        MOVS     R4,R0
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        MOVS     R3,R4
        LDR.N    R0,??DataTable10_9
        LDR      R0,[R0, #+0]
        LDR      R2,[R0, #+248]
        LDR.N    R1,??DataTable10_19
        LDR.N    R0,??DataTable10_17
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  157 
//  158 		if (engineConfiguration->map.sensor.type == MT_CUSTOM) {
        LDR.N    R0,??DataTable10_9
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+248]
        CMP      R0,#+0
        BNE.N    ??printMAPInfo_1
//  159 			scheduleMsg(logger, "at0=%f at5=%f", engineConfiguration->map.sensor.valueAt0,
//  160 					engineConfiguration->map.sensor.valueAt5);
        LDR.N    R0,??DataTable10_9
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+244]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        LDR.N    R0,??DataTable10_9
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+240]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R1,??DataTable10_20
        LDR.N    R0,??DataTable10_17
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  161 		}
//  162 	}
//  163 
//  164 	scheduleMsg(logger, "baro type=%d value=%f", engineConfiguration->baroSensor.type, getBaroPressure());
??printMAPInfo_1:
          CFI FunCall _Z15getBaroPressurev
        BL       _Z15getBaroPressurev
        VMOV     R0,S0
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        LDR.N    R0,??DataTable10_9
        LDR      R0,[R0, #+0]
        LDR      R2,[R0, #+616]
        LDR.N    R1,??DataTable10_21
        LDR.N    R0,??DataTable10_17
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  165 	if (engineConfiguration->baroSensor.type == MT_CUSTOM) {
        LDR.N    R0,??DataTable10_9
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+616]
        CMP      R0,#+0
        BNE.N    ??printMAPInfo_2
//  166 		scheduleMsg(logger, "min=%f max=%f", engineConfiguration->baroSensor.valueAt0,
//  167 				engineConfiguration->baroSensor.valueAt5);
        LDR.N    R0,??DataTable10_9
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+612]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        LDR.N    R0,??DataTable10_9
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+608]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R1,??DataTable10_22
        LDR.N    R0,??DataTable10_17
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  168 	}
//  169 #endif /* EFI_ANALOG_SENSORS */
//  170 }
??printMAPInfo_2:
        ADD      SP,SP,#+8
          CFI CFA R13+16
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock9
//  171 #endif /* EFI_PROD_CODE */
//  172 
//  173 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function _Z14initMapDecoderP7Logging
        THUMB
//  174 void initMapDecoder(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_S) {
_Z14initMapDecoderP7Logging:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  175 	logger = sharedLogger;
        LDR.N    R0,??DataTable10_17
        STR      R4,[R0, #+0]
//  176 	applyConfiguration(PASS_ENGINE_PARAMETER_F);
          CFI FunCall _Z18applyConfigurationv
        BL       _Z18applyConfigurationv
//  177 	//engine->configurationListeners.registerCallback(applyConfiguration);
//  178 
//  179 #if EFI_PROD_CODE || defined(__DOXYGEN__)
//  180 	if (engineConfiguration->hasFrequencyReportingMapSensor) {
        LDR.N    R0,??DataTable10_9
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1498]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+2
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??initMapDecoder_0
//  181 		digital_input_s* digitalMapInput = initWaveAnalyzerDriver("map freq", boardConfiguration->frequencyReportingMapInputPin);
        LDR.N    R0,??DataTable10_11
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+868]
        LDR.N    R0,??DataTable10_23
          CFI FunCall _Z22initWaveAnalyzerDriverPKc11brain_pin_e
        BL       _Z22initWaveAnalyzerDriverPKc11brain_pin_e
        MOVS     R5,R0
//  182 		startInputDriver(digitalMapInput, true);
        MOVS     R1,#+1
        MOVS     R0,R5
          CFI FunCall _Z16startInputDriverP15digital_input_sb
        BL       _Z16startInputDriverP15digital_input_sb
//  183 
//  184 		digitalMapInput->widthListeners.registerCallback((VoidInt) digitalMapWidthCallback, NULL);
        MOVS     R2,#+0
        LDR.N    R1,??DataTable10_24
        ADDS     R0,R5,#+8
          CFI FunCall _ZN16IntListenerArray16registerCallbackEPFviEPv
        BL       _ZN16IntListenerArray16registerCallbackEPFviEPv
//  185 	}
//  186 
//  187 	addConsoleAction("mapinfo", printMAPInfo);
??initMapDecoder_0:
        LDR.N    R1,??DataTable10_25
        LDR.N    R0,??DataTable10_26
          CFI FunCall addConsoleAction
        BL       addConsoleAction
//  188 #endif
//  189 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock10

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10:
        DC32     0x0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_1:
        DC32     customMap

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_2:
        DC32     denso183

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_3:
        DC32     honda3bar

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_4:
        DC32     subyDenso

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_5:
        DC32     gm3bar

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_6:
        DC32     mpx4250

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_7:
        DC32     dodgeNeon2003

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_8:
        DC32     `?<Constant "Unknown MAP type: %d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_9:
        DC32     engineConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_10:
        DC32     mapFreq

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_11:
        DC32     boardConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_12:
        DC32     `?<Constant "baro">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_13:
        DC32     mapDecoder

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_14:
        DC32     prevWidthTimeNt

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_15:
        DC32     0x412e8480

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_16:
        DC32     `?<Constant "instant value=%fkPa">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_17:
        DC32     logger

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_18:
        DC32     `?<Constant "instant value=%fHz @ %s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_19:
        DC32     `?<Constant "map type=%d/%s MAP=%fkPa">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_20:
        DC32     `?<Constant "at0=%f at5=%f">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_21:
        DC32     `?<Constant "baro type=%d value=%f">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_22:
        DC32     `?<Constant "min=%f max=%f">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_23:
        DC32     `?<Constant "map freq">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_24:
        DC32     _Z23digitalMapWidthCallbackv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_25:
        DC32     _Z12printMAPInfov

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_26:
        DC32     `?<Constant "mapinfo">`

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
`?<Constant "Unknown MAP type: %d">`:
        DATA
        DC8 "Unknown MAP type: %d"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "map"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "baro">`:
        DATA
        DC8 "baro"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "instant value=%fkPa">`:
        DATA
        DC8 "instant value=%fkPa"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "instant value=%fHz @ %s">`:
        DATA
        DC8 "instant value=%fHz @ %s"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "map type=%d/%s MAP=%fkPa">`:
        DATA
        DC8 "map type=%d/%s MAP=%fkPa"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "at0=%f at5=%f">`:
        DATA
        DC8 "at0=%f at5=%f"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "baro type=%d value=%f">`:
        DATA
        DC8 "baro type=%d value=%f"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "min=%f max=%f">`:
        DATA
        DC8 "min=%f max=%f"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "map freq">`:
        DATA
        DC8 "map freq"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "mapinfo">`:
        DATA
        DC8 "mapinfo"

        END
//  190 
//  191 #else /* EFI_ANALOG_SENSORS */
//  192 
//  193 void initMapDecoder(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_S) {
//  194 }
//  195 
//  196 #endif /* EFI_ANALOG_SENSORS */
// 
//    76 bytes in section .bss
//     4 bytes in section .init_array
//   184 bytes in section .rodata
// 1 316 bytes in section .text
// 
// 1 320 bytes of CODE  memory
//   184 bytes of CONST memory
//    76 bytes of DATA  memory
//
//Errors: none
//Warnings: 2
