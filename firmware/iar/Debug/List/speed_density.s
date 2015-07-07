///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:10:03 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\controllers\mat /
//                       h\speed_density.cpp                                  /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\controllers\mat /
//                       h\speed_density.cpp -lCN F:\stuff\rusefi_sourceforge /
//                       \firmware\iar\Debug\List\ -lA                        /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\Debug\List\ /
//                        -o F:\stuff\rusefi_sourceforge\firmware\iar\Debug\O /
//                       bj\ --no_cse --no_unroll --no_inline                 /
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
//                       speed_density.s                                      /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME speed_density

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1
        #define SHT_INIT_ARRAY 0xe

        EXTERN _Z11interpolatefffff
        EXTERN _Z12setTableBin2Pfifff
        EXTERN _Z14setRpmTableBinPfi
        EXTERN _Z6getMapv
        EXTERN _ZN16AccelEnrichmemnt16getMapEnrichmentEv
        EXTERN __aeabi_d2f
        EXTERN __aeabi_ddiv
        EXTERN __aeabi_dmul
        EXTERN __aeabi_f2d
        EXTERN __aeabi_memcpy
        EXTERN __aeabi_memset
        EXTERN __iar_FDtest
        EXTERN _engine
        EXTERN config
        EXTERN engine
        EXTERN engineConfiguration
        EXTERN firmwareError
        EXTERN warning

        PUBLIC _Z10getTChargeifff
        PUBLIC _Z16initSpeedDensityv
        PUBLIC _Z17setDefaultVETablev
        PUBLIC _Z19getSpeedDensityFueli
        PUBLIC _Z6sdMathP22engine_configuration_sffff
        PUBLIC _ZN5Map3DILi16ELi16EE4initEPA16_fPfS3_
        PUBLIC _ZN5Map3DILi16ELi16EE6setAllEf
        PUBLIC _ZN5Map3DILi16ELi16EEC1Ev
        PUBLIC _ZN5Map3DILi16ELi16EEC2Ev
        PUBLIC _ZN5Map3DILi4ELi4EE4initEPA4_fPfS3_
        PUBLIC _ZN5Map3DILi4ELi4EEC1Ev
        PUBLIC _ZN5Map3DILi4ELi4EEC2Ev
        PUBLIC _ZZN5Map3DILi16ELi16EE6setAllEfEs
        PUBLIC afrMap
        PUBLIC baroCorrMap
        PUBLIC memcpy
        PUBLIC memset
        PUBLIC ve2Map
        PUBLIC veMap
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\controllers\math\speed_density.cpp
//    1 /**
//    2  * @file	speed_density.cpp
//    3  *
//    4  * See http://rusefi.com/wiki/index.php?title=Manual:Software:Fuel_Control#Speed_Density for details
//    5  *
//    6  * @date May 29, 2014
//    7  * @author Andrey Belomutskiy, (c) 2012-2015
//    8  */
//    9 
//   10 #include "main.h"
//   11 #include "speed_density.h"
//   12 #include "interpolation.h"
//   13 #include "rpm_calculator.h"
//   14 #include "engine_math.h"
//   15 #include "engine_state.h"
//   16 
//   17 #define K_AT_MIN_RPM_MIN_TPS 0.25
//   18 #define K_AT_MIN_RPM_MAX_TPS 0.25
//   19 #define K_AT_MAX_RPM_MIN_TPS 0.25
//   20 #define K_AT_MAX_RPM_MAX_TPS 0.9
//   21 
//   22 #define rpmMin 500
//   23 #define rpmMax 8000
//   24 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function __sti__routine
        THUMB
// static __intrinsic __interwork __softfp void __sti__routine()
__sti__routine:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   25 fuel_Map3D_t veMap;
        LDR.N    R0,??DataTable5_11
          CFI FunCall _ZN5Map3DILi16ELi16EEC1Ev
        BL       _ZN5Map3DILi16ELi16EEC1Ev
//   26 fuel_Map3D_t ve2Map;
        LDR.N    R0,??DataTable5_12
          CFI FunCall _ZN5Map3DILi16ELi16EEC1Ev
        BL       _ZN5Map3DILi16ELi16EEC1Ev
//   27 fuel_Map3D_t afrMap;
        LDR.N    R0,??DataTable5_13
          CFI FunCall _ZN5Map3DILi16ELi16EEC1Ev
        BL       _ZN5Map3DILi16ELi16EEC1Ev
//   28 baroCorr_Map3D_t baroCorrMap;
        LDR.N    R0,??DataTable5_14
          CFI FunCall _ZN5Map3DILi4ELi4EEC1Ev
        BL       _ZN5Map3DILi4ELi4EEC1Ev
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock0

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
veMap:
        DS8 76

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
ve2Map:
        DS8 76

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
afrMap:
        DS8 76

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
baroCorrMap:
        DS8 28
//   29 
//   30 #define tpMin 0
//   31 #define tpMax 100
//   32 //  http://rusefi.com/math/t_charge.html

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z10getTChargeifff
        THUMB
//   33 float getTCharge(int rpm, float tps, float coolantTemp, float airTemp) {
_Z10getTChargeifff:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8-D10}
          CFI D10 Frame(CFA, -16)
          CFI D9 Frame(CFA, -24)
          CFI D8 Frame(CFA, -32)
          CFI CFA R13+32
        VMOV     S16,R0
        VMOV.F32 S17,S0
        VMOV.F32 S18,S1
        VMOV.F32 S19,S2
//   34 	float minRpmKcurrentTPS = interpolate(tpMin, K_AT_MIN_RPM_MIN_TPS, tpMax,
//   35 	K_AT_MIN_RPM_MAX_TPS, tps);
        VMOV.F32 S4,S17
        VMOV.F32 S3,#0.25
        VLDR.W   S2,??DataTable5  ;; 0x42c80000
        VMOV.F32 S1,#0.25
        VLDR.W   S0,??DataTable5_1  ;; 0x0
          CFI FunCall _Z11interpolatefffff
        BL       _Z11interpolatefffff
        VMOV.F32 S20,S0
//   36 	float maxRpmKcurrentTPS = interpolate(tpMin, K_AT_MAX_RPM_MIN_TPS, tpMax,
//   37 	K_AT_MAX_RPM_MAX_TPS, tps);
        VMOV.F32 S4,S17
        VLDR.W   S3,??DataTable5_2  ;; 0x3f666666
        VLDR.W   S2,??DataTable5  ;; 0x42c80000
        VMOV.F32 S1,#0.25
        VLDR.W   S0,??DataTable5_1  ;; 0x0
          CFI FunCall _Z11interpolatefffff
        BL       _Z11interpolatefffff
        VMOV.F32 S21,S0
//   38 
//   39 	float Tcharge_coff = interpolate(rpmMin, minRpmKcurrentTPS, rpmMax, maxRpmKcurrentTPS, rpm);
        VCVT.F32.S32 S4,S16
        VMOV.F32 S3,S21
        VLDR.W   S2,??DataTable5_3  ;; 0x45fa0000
        VMOV.F32 S1,S20
        VLDR.W   S0,??DataTable5_4  ;; 0x43fa0000
          CFI FunCall _Z11interpolatefffff
        BL       _Z11interpolatefffff
        VMOV.F32 S1,S0
//   40 
//   41 	float Tcharge = coolantTemp * (1 - Tcharge_coff) + airTemp * Tcharge_coff;
        VMOV.F32 S0,#1.0
        VSUB.F32 S0,S0,S1
        VMUL.F32 S0,S0,S18
        VMLA.F32 S0,S19,S1
//   42 
//   43 	return Tcharge;
        VPOP     {D8-D10}
          CFI D8 SameValue
          CFI D9 SameValue
          CFI D10 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock1
//   44 }
//   45 
//   46 /**
//   47  * is J/g*K
//   48  */
//   49 #define GAS_R 0.28705
//   50 
//   51 /**
//   52  * @return per cylinder injection time, in seconds
//   53  */

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z6sdMathP22engine_configuration_sffff
        THUMB
//   54 float sdMath(engine_configuration_s *engineConfiguration, float VE, float MAP, float AFR, float tempK) {
_Z6sdMathP22engine_configuration_sffff:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        VPUSH    {D8-D10}
          CFI D10 Frame(CFA, -24)
          CFI D9 Frame(CFA, -32)
          CFI D8 Frame(CFA, -40)
          CFI CFA R13+40
        MOVS     R6,R0
        VMOV.F32 S19,S0
        VMOV.F32 S16,S1
        VMOV.F32 S17,S2
        VMOV.F32 S18,S3
//   55 	if (MAP < 0.001 || cisnan(MAP)) {
        VLDR.W   S0,??DataTable5_5  ;; 0x3a83126f
        VCMP.F32 S16,S0
        FMSTAT   
        BMI.N    ??sdMath_0
        VMOV.F32 S0,S16
          CFI FunCall __iar_FDtest
        BL       __iar_FDtest
        CMP      R0,#+2
        BNE.N    ??sdMath_1
//   56 		warning(OBD_PCM_Processor_Fault, "invalid MAP value");
??sdMath_0:
        LDR.N    R1,??DataTable5_15
        MOVW     R0,#+606
          CFI FunCall warning
        BL       warning
//   57 		return 0;
        VLDR.W   S0,??DataTable5_1  ;; 0x0
        B.N      ??sdMath_2
//   58 	}
//   59 
//   60 	/**
//   61 	 * todo: pre-calculate gramm/second injector flow to save one multiplication
//   62 	 * open question if that's needed since that's just a multiplication
//   63 	 */
//   64 	float injectorFlowRate = cc_minute_to_gramm_second(engineConfiguration->injector.flow);
??sdMath_1:
        LDR      R0,[R6, #+8]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        LDR.N    R2,??DataTable5_16  ;; 0xa32f5098
        LDR.N    R3,??DataTable5_17  ;; 0x3f889359
          CFI FunCall __aeabi_dmul
        BL       __aeabi_dmul
          CFI FunCall __aeabi_d2f
        BL       __aeabi_d2f
        VMOV     S20,R0
//   65 	// todo: pre-calculate cylinder displacement to save one division
//   66 	float cylinderDisplacement = engineConfiguration->specs.displacement / engineConfiguration->specs.cylindersCount;
        VLDR     S0,[R6, #+404]
        VCVT.F32.S32 S0,S0
        VLDR     S1,[R6, #+400]
        VDIV.F32 S21,S1,S0
//   67 	float airMass = (cylinderDisplacement * VE * MAP) / (GAS_R * tempK);
        VMUL.F32 S0,S21,S19
        VMUL.F32 S0,S0,S16
        VMOV     R0,S0
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R4,R0
        MOVS     R5,R1
        VMOV     R0,S18
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        LDR.N    R2,??DataTable5_18  ;; 0xf6944674
        LDR.N    R3,??DataTable5_19  ;; 0x3fd25f06
          CFI FunCall __aeabi_dmul
        BL       __aeabi_dmul
        MOVS     R2,R0
        MOVS     R3,R1
        MOVS     R0,R4
        MOVS     R1,R5
          CFI FunCall __aeabi_ddiv
        BL       __aeabi_ddiv
          CFI FunCall __aeabi_d2f
        BL       __aeabi_d2f
        VMOV     S0,R0
//   68 	/**
//   69 	 * injection_pulse_duration = fuel_mass / injector_flow
//   70 	 * fuel_mass = air_mass / target_afr
//   71 	 *
//   72 	 * injection_pulse_duration = (air_mass / target_afr) / injector_flow
//   73 	 */
//   74 	return airMass / (AFR * injectorFlowRate);
        VMUL.F32 S1,S17,S20
        VDIV.F32 S0,S0,S1
??sdMath_2:
        VPOP     {D8-D10}
          CFI D8 SameValue
          CFI D9 SameValue
          CFI D10 SameValue
          CFI CFA R13+16
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock2
//   75 }
//   76 
//   77 EXTERN_ENGINE;
//   78 
//   79 /**
//   80  * @return per cylinder injection time, in Milliseconds
//   81  */

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z19getSpeedDensityFueli
        THUMB
//   82 float getSpeedDensityFuel(int rpm DECLARE_ENGINE_PARAMETER_S) {
_Z19getSpeedDensityFueli:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        VPUSH    {D8-D9}
          CFI D9 Frame(CFA, -16)
          CFI D8 Frame(CFA, -24)
          CFI CFA R13+24
        MOVS     R4,R0
//   83 	//int rpm = engine->rpmCalculator->rpm();
//   84 
//   85 	/**
//   86 	 * most of the values are pre-calculated for performance reasons
//   87 	 */
//   88 	float tChargeK = ENGINE(engineState.tChargeK);
        LDR.N    R0,??DataTable5_20
        VLDR     S16,[R0, #0]
//   89 	float map = getMap();
          CFI FunCall _Z6getMapv
        BL       _Z6getMapv
        VMOV.F32 S17,S0
//   90 
//   91 	float adjMap = map + engine->mapAccelEnrichment.getMapEnrichment(PASS_ENGINE_PARAMETER_F);
        LDR.N    R0,??DataTable5_21
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+56
          CFI FunCall _ZN16AccelEnrichmemnt16getMapEnrichmentEv
        BL       _ZN16AccelEnrichmemnt16getMapEnrichmentEv
        VADD.F32 S18,S17,S0
//   92 
//   93 	return sdMath(engineConfiguration, ENGINE(engineState.currentVE), adjMap, ENGINE(engineState.targerAFR), tChargeK) * 1000;
        VMOV.F32 S3,S16
        LDR.N    R0,??DataTable5_22
        VLDR     S2,[R0, #0]
        VMOV.F32 S1,S18
        LDR.N    R0,??DataTable5_23
        VLDR     S0,[R0, #0]
        LDR.N    R0,??DataTable5_24
        LDR      R0,[R0, #+0]
          CFI FunCall _Z6sdMathP22engine_configuration_sffff
        BL       _Z6sdMathP22engine_configuration_sffff
        VLDR.W   S1,??DataTable5_6  ;; 0x447a0000
        VMUL.F32 S0,S0,S1
        VPOP     {D8-D9}
          CFI D8 SameValue
          CFI D9 SameValue
          CFI CFA R13+8
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock3
//   94 }

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        SECTION_GROUP _ZZN5Map3DILi16ELi16EE6setAllEfEs
// __absolute char const <_ZZN5Map3DILi16ELi16EE6setAllEfEs>[20]
_ZZN5Map3DILi16ELi16EE6setAllEfEs:
        DATA
        DC8 "map not initialized"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "invalid MAP value">`:
        DATA
        DC8 "invalid MAP value"
        DC8 0, 0
//   95 

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
//   96 static const baro_corr_table_t default_baro_corr = {
default_baro_corr:
        DATA
        DC32 3F920C4AH, 3F8B020CH, 3F84FDF4H, 3F800000H, 3F920C4AH, 3F8B020CH
        DC32 3F84FDF4H, 3F800000H, 3F920C4AH, 3F8B020CH, 3F84FDF4H, 3F800000H
        DC32 3F920C4AH, 3F8B020CH, 3F84FDF4H, 3F800000H
//   97 		{1.141, 1.086, 1.039, 1},
//   98 		{1.141, 1.086, 1.039, 1},
//   99 		{1.141, 1.086, 1.039, 1},
//  100 		{1.141, 1.086, 1.039, 1}
//  101 };
//  102 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _Z17setDefaultVETablev
        THUMB
//  103 void setDefaultVETable(DECLARE_ENGINE_PARAMETER_F) {
_Z17setDefaultVETablev:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  104 	setRpmTableBin(config->veRpmBins, FUEL_RPM_COUNT);
        MOVS     R1,#+16
        LDR.N    R0,??DataTable5_25
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+13824
        ADDS     R0,R0,#+248
          CFI FunCall _Z14setRpmTableBinPfi
        BL       _Z14setRpmTableBinPfi
//  105 	veMap.setAll(80);
        VLDR.W   S0,??DataTable5_7  ;; 0x42a00000
        LDR.N    R0,??DataTable5_11
          CFI FunCall _ZN5Map3DILi16ELi16EE6setAllEf
        BL       _ZN5Map3DILi16ELi16EE6setAllEf
//  106 
//  107 //	setRpmTableBin(engineConfiguration->ve2RpmBins, FUEL_RPM_COUNT);
//  108 //	setTableBin2(engineConfiguration->ve2LoadBins, FUEL_LOAD_COUNT, 10, 300, 1);
//  109 //	ve2Map.setAll(0.81);
//  110 
//  111 	setRpmTableBin(config->afrRpmBins, FUEL_RPM_COUNT);
        MOVS     R1,#+16
        LDR.N    R0,??DataTable5_25
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+15104
        ADDS     R0,R0,#+120
          CFI FunCall _Z14setRpmTableBinPfi
        BL       _Z14setRpmTableBinPfi
//  112 	afrMap.setAll(14.7);
        VLDR.W   S0,??DataTable5_8  ;; 0x416b3333
        LDR.N    R0,??DataTable5_13
          CFI FunCall _ZN5Map3DILi16ELi16EE6setAllEf
        BL       _ZN5Map3DILi16ELi16EE6setAllEf
//  113 
//  114 	setRpmTableBin(engineConfiguration->baroCorrRpmBins, BARO_CORR_SIZE);
        MOVS     R1,#+4
        LDR.N    R0,??DataTable5_24
        LDR      R0,[R0, #+0]
        ADDW     R0,R0,#+1668
          CFI FunCall _Z14setRpmTableBinPfi
        BL       _Z14setRpmTableBinPfi
//  115 	setTableBin2(engineConfiguration->baroCorrPressureBins, BARO_CORR_SIZE, 75, 105, 1);
        VMOV.F32 S2,#1.0
        VLDR.W   S1,??DataTable5_9  ;; 0x42d20000
        VLDR.W   S0,??DataTable5_10  ;; 0x42960000
        MOVS     R1,#+4
        LDR.N    R0,??DataTable5_24
        LDR      R0,[R0, #+0]
        ADDW     R0,R0,#+1652
          CFI FunCall _Z12setTableBin2Pfifff
        BL       _Z12setTableBin2Pfifff
//  116 	memcpy(engineConfiguration->baroCorrTable, default_baro_corr, sizeof(default_baro_corr));
        MOVS     R2,#+64
        LDR.N    R1,??DataTable5_26
        LDR.N    R0,??DataTable5_24
        LDR      R0,[R0, #+0]
        ADDW     R0,R0,#+1684
          CFI FunCall memcpy
        BL       memcpy
//  117 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock4

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _Z16initSpeedDensityv
        THUMB
_Z16initSpeedDensityv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        LDR.N    R0,??DataTable5_25
        LDR      R0,[R0, #+0]
        ADD      R3,R0,#+13824
        ADDS     R3,R3,#+248
        LDR.N    R0,??DataTable5_25
        LDR      R0,[R0, #+0]
        ADD      R2,R0,#+13824
        ADDS     R2,R2,#+184
        LDR.N    R0,??DataTable5_25
        LDR      R0,[R0, #+0]
        ADD      R1,R0,#+12800
        ADDS     R1,R1,#+184
        LDR.N    R0,??DataTable5_11
          CFI FunCall _ZN5Map3DILi16ELi16EE4initEPA16_fPfS3_
        BL       _ZN5Map3DILi16ELi16EE4initEPA16_fPfS3_
        LDR.N    R0,??DataTable5_25
        LDR      R0,[R0, #+0]
        ADD      R3,R0,#+15104
        ADDS     R3,R3,#+120
        LDR.N    R0,??DataTable5_25
        LDR      R0,[R0, #+0]
        ADD      R2,R0,#+15104
        ADDS     R2,R2,#+56
        LDR.N    R0,??DataTable5_25
        LDR      R0,[R0, #+0]
        ADD      R1,R0,#+14080
        ADDS     R1,R1,#+56
        LDR.N    R0,??DataTable5_13
          CFI FunCall _ZN5Map3DILi16ELi16EE4initEPA16_fPfS3_
        BL       _ZN5Map3DILi16ELi16EE4initEPA16_fPfS3_
        LDR.N    R0,??DataTable5_24
        LDR      R0,[R0, #+0]
        ADDW     R3,R0,#+1668
        LDR.N    R0,??DataTable5_24
        LDR      R0,[R0, #+0]
        ADDW     R2,R0,#+1652
        LDR.N    R0,??DataTable5_24
        LDR      R0,[R0, #+0]
        ADDW     R1,R0,#+1684
        LDR.N    R0,??DataTable5_14
          CFI FunCall _ZN5Map3DILi4ELi4EE4initEPA4_fPfS3_
        BL       _ZN5Map3DILi4ELi4EE4initEPA4_fPfS3_
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock5

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5:
        DC32     0x42c80000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_1:
        DC32     0x0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_2:
        DC32     0x3f666666

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_3:
        DC32     0x45fa0000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_4:
        DC32     0x43fa0000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_5:
        DC32     0x3a83126f

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_6:
        DC32     0x447a0000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_7:
        DC32     0x42a00000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_8:
        DC32     0x416b3333

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_9:
        DC32     0x42d20000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_10:
        DC32     0x42960000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_11:
        DC32     veMap

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_12:
        DC32     ve2Map

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_13:
        DC32     afrMap

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_14:
        DC32     baroCorrMap

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_15:
        DC32     `?<Constant "invalid MAP value">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_16:
        DC32     0xa32f5098

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_17:
        DC32     0x3f889359

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_18:
        DC32     0xf6944674

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_19:
        DC32     0x3fd25f06

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_20:
        DC32     _engine+0x1ED4

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_21:
        DC32     engine

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_22:
        DC32     _engine+0x1EDC

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_23:
        DC32     _engine+0x1ED8

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_24:
        DC32     engineConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_25:
        DC32     config

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_26:
        DC32     default_baro_corr

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP memcpy
          CFI Block cfiBlock6 Using cfiCommon0
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
          CFI EndBlock cfiBlock6

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP memset
          CFI Block cfiBlock7 Using cfiCommon0
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
          CFI EndBlock cfiBlock7

        SECTION `.text`:CODE:REORDER:NOROOT(2)
        SECTION_GROUP _ZN5Map3DILi16ELi16EE4initEPA16_fPfS3_
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function _ZN5Map3DILi16ELi16EE4initEPA16_fPfS3_
          CFI NoCalls
        THUMB
// __interwork __softfp void Map3D<16, 16>::init(float (*)[16], float *, float *)
_ZN5Map3DILi16ELi16EE4initEPA16_fPfS3_:
        PUSH     {R4,R5}
          CFI R5 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,#+0
??init_0:
        CMP      R4,#+16
        BGE.N    ??init_1
        MOVS     R5,#+64
        MLA      R5,R5,R4,R1
        STR      R5,[R0, R4, LSL #+2]
        ADDS     R4,R4,#+1
        B.N      ??init_0
??init_1:
        LDR.N    R4,??init_2      ;; 0x923f558
        STR      R4,[R0, #+72]
        STR      R2,[R0, #+64]
        STR      R3,[R0, #+68]
        POP      {R4,R5}
          CFI R4 SameValue
          CFI R5 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
        Nop      
        DATA
??init_2:
        DC32     0x923f558
          CFI EndBlock cfiBlock8

        SECTION `.text`:CODE:REORDER:NOROOT(2)
        SECTION_GROUP _ZN5Map3DILi4ELi4EE4initEPA4_fPfS3_
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function _ZN5Map3DILi4ELi4EE4initEPA4_fPfS3_
          CFI NoCalls
        THUMB
// __interwork __softfp void Map3D<4, 4>::init(float (*)[4], float *, float *)
_ZN5Map3DILi4ELi4EE4initEPA4_fPfS3_:
        PUSH     {R4,R5}
          CFI R5 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,#+0
??init_3:
        CMP      R4,#+4
        BGE.N    ??init_4
        ADDS     R5,R1,R4, LSL #+4
        STR      R5,[R0, R4, LSL #+2]
        ADDS     R4,R4,#+1
        B.N      ??init_3
??init_4:
        LDR.N    R4,??init_5      ;; 0x923f558
        STR      R4,[R0, #+24]
        STR      R2,[R0, #+16]
        STR      R3,[R0, #+20]
        POP      {R4,R5}
          CFI R4 SameValue
          CFI R5 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
        DATA
??init_5:
        DC32     0x923f558
          CFI EndBlock cfiBlock9

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN5Map3DILi16ELi16EEC1Ev
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function _ZN5Map3DILi16ELi16EEC1Ev
        THUMB
// __code __interwork __softfp Map3D<16, 16>::Map3D()
_ZN5Map3DILi16ELi16EEC1Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,#+0
        STR      R0,[R4, #+72]
        MOVS     R2,#+64
        MOVS     R1,#+0
        MOVS     R0,R4
          CFI FunCall memset
        BL       memset
        MOVS     R0,#+0
        STR      R0,[R4, #+64]
        MOVS     R0,#+0
        STR      R0,[R4, #+68]
        MOVS     R0,R4
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock10

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN5Map3DILi4ELi4EEC1Ev
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function _ZN5Map3DILi4ELi4EEC1Ev
        THUMB
// __code __interwork __softfp Map3D<4, 4>::Map3D()
_ZN5Map3DILi4ELi4EEC1Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,#+0
        STR      R0,[R4, #+24]
        MOVS     R2,#+16
        MOVS     R1,#+0
        MOVS     R0,R4
          CFI FunCall memset
        BL       memset
        MOVS     R0,#+0
        STR      R0,[R4, #+16]
        MOVS     R0,#+0
        STR      R0,[R4, #+20]
        MOVS     R0,R4
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock11

        SECTION `.text`:CODE:REORDER:NOROOT(2)
        SECTION_GROUP _ZN5Map3DILi16ELi16EE6setAllEf
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function _ZN5Map3DILi16ELi16EE6setAllEf
        THUMB
// __interwork __vfp void Map3D<16, 16>::setAll(float)
_ZN5Map3DILi16ELi16EE6setAllEf:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        VMOV.F32 S16,S0
        LDR      R0,[R4, #+72]
        LDR.N    R1,??setAll_0    ;; 0x923f558
        CMP      R0,R1
        BEQ.N    ??setAll_1
        LDR.N    R0,??setAll_0+0x4
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??setAll_2
??setAll_1:
        MOVS     R0,#+0
??setAll_3:
        CMP      R0,#+16
        BGE.N    ??setAll_4
        MOVS     R1,#+0
??setAll_5:
        CMP      R1,#+16
        BGE.N    ??setAll_6
        LDR      R2,[R4, R0, LSL #+2]
        ADDS     R2,R2,R1, LSL #+2
        VSTR     S16,[R2, #0]
        ADDS     R1,R1,#+1
        B.N      ??setAll_5
??setAll_6:
        ADDS     R0,R0,#+1
        B.N      ??setAll_3
??setAll_4:
??setAll_2:
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R4,PC}          ;; return
        Nop      
        DATA
??setAll_0:
        DC32     0x923f558
        DC32     _ZZN5Map3DILi16ELi16EE6setAllEfEs
          CFI EndBlock cfiBlock12

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

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN5Map3DILi16ELi16EEC2Ev
          CFI Block cfiBlock13 Using cfiCommon0
          CFI Function _ZN5Map3DILi16ELi16EEC2Ev
        THUMB
// __code __interwork __softfp Map3D<16, 16>::subobject Map3D()
_ZN5Map3DILi16ELi16EEC2Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN5Map3DILi16ELi16EEC1Ev
        BL       _ZN5Map3DILi16ELi16EEC1Ev
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock13

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN5Map3DILi4ELi4EEC2Ev
          CFI Block cfiBlock14 Using cfiCommon0
          CFI Function _ZN5Map3DILi4ELi4EEC2Ev
        THUMB
// __code __interwork __softfp Map3D<4, 4>::subobject Map3D()
_ZN5Map3DILi4ELi4EEC2Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN5Map3DILi4ELi4EEC1Ev
        BL       _ZN5Map3DILi4ELi4EEC1Ev
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock14

        END
//  118 
//  119 void initSpeedDensity(DECLARE_ENGINE_PARAMETER_F) {
//  120 	veMap.init(config->veTable, config->veLoadBins, config->veRpmBins);
//  121 //	ve2Map.init(engineConfiguration->ve2Table, engineConfiguration->ve2LoadBins, engineConfiguration->ve2RpmBins);
//  122 	afrMap.init(config->afrTable, config->afrLoadBins, config->afrRpmBins);
//  123 	baroCorrMap.init(engineConfiguration->baroCorrTable, engineConfiguration->baroCorrPressureBins, engineConfiguration->baroCorrRpmBins);
//  124 }
// 
//   256 bytes in section .bss
//     4 bytes in section .init_array
//   104 bytes in section .rodata
// 1 014 bytes in section .text
// 
// 738 bytes of CODE  memory (+ 280 bytes shared)
//  84 bytes of CONST memory (+  20 bytes shared)
// 256 bytes of DATA  memory
//
//Errors: none
//Warnings: 1
