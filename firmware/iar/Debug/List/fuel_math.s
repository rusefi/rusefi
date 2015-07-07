///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:50 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\controllers\alg /
//                       o\fuel_math.cpp                                      /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\controllers\alg /
//                       o\fuel_math.cpp -lCN F:\stuff\rusefi_sourceforge\fir /
//                       mware\iar\Debug\List\ -lA                            /
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
//                       fuel_math.s                                          /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME fuel_math

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1
        #define SHT_INIT_ARRAY 0xe

        EXTERN _Z10getRealMafv
        EXTERN _Z13hasBaroSensorv
        EXTERN _Z13interpolate2dfPfS_i
        EXTERN _Z13interpolate3dfPfifS_iPS_
        EXTERN _Z14getEngineLoadTv
        EXTERN _Z15getBaroPressurev
        EXTERN _Z19getSpeedDensityFueli
        EXTERN _Z21getCoolantTemperaturev
        EXTERN _ZN13RpmCalculator30getRevolutionCounterSinceStartEv
        EXTERN _ZN13RpmCalculator3rpmEv
        EXTERN _ZN16AccelEnrichmemnt16getMapEnrichmentEv
        EXTERN _ZN16AccelEnrichmemnt16getTpsEnrichmentEv
        EXTERN __aeabi_d2f
        EXTERN __aeabi_ddiv
        EXTERN __aeabi_dmul
        EXTERN __aeabi_f2d
        EXTERN __aeabi_i2d
        EXTERN __aeabi_memset
        EXTERN __iar_FDtest
        EXTERN _engine
        EXTERN afrMap
        EXTERN baroCorrMap
        EXTERN config
        EXTERN engine
        EXTERN engineConfiguration
        EXTERN firmwareError
        EXTERN persistentState
        EXTERN warning

        PUBLIC _Z11getBaseFueli
        PUBLIC _Z14getInjectorLagf
        PUBLIC _Z14getRealMafFuelfi
        PUBLIC _Z14getRunningFuelfi
        PUBLIC _Z14prepareFuelMapv
        PUBLIC _Z15getCrankingFuelv
        PUBLIC _Z16getBaseTableFuelP22engine_configuration_sif
        PUBLIC _Z16getCltCorrectionf
        PUBLIC _Z16getCrankingFuel3fj
        PUBLIC _Z16getIatCorrectionf
        PUBLIC _Z17getBaroCorrectionv
        PUBLIC _Z18getinjectionOffseti
        PUBLIC _Z20getInjectorDutyCyclei
        PUBLIC _Z9getFuelMsi
        PUBLIC _ZN5Map3DILi16ELi16EE4initEPA16_fPfS3_
        PUBLIC _ZN5Map3DILi16ELi16EE8getValueEff
        PUBLIC _ZN5Map3DILi16ELi16EEC1Ev
        PUBLIC _ZN5Map3DILi16ELi16EEC2Ev
        PUBLIC _ZN5Map3DILi4ELi4EE8getValueEff
        PUBLIC _ZZN5Map3DILi16ELi16EE8getValueEffEs
        PUBLIC _ZZN5Map3DILi4ELi4EE8getValueEffEs
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
        
// F:\stuff\rusefi_sourceforge\firmware\controllers\algo\fuel_math.cpp
//    1 /**
//    2  * @file	fuel_math.cpp
//    3  * @brief	Fuel amount calculation logic
//    4  *
//    5  * While engine running, fuel amount is an interpolated value from the fuel map by getRpm() and getEngineLoad()
//    6  * On top of the value from the fuel map we also apply
//    7  * <BR>1) getInjectorLag() correction to account for fuel injector lag
//    8  * <BR>2) getCltCorrection() for warm-up
//    9  * <BR>3) getIatCorrection() to account for cold weather
//   10  *
//   11  * getCrankingFuel() depents only on getCoolantTemperature()
//   12  *
//   13  *
//   14  * @date May 27, 2013
//   15  * @author Andrey Belomutskiy, (c) 2012-2015
//   16  *
//   17  * This file is part of rusEfi - see http://rusefi.com
//   18  *
//   19  * rusEfi is free software; you can redistribute it and/or modify it under the terms of
//   20  * the GNU General Public License as published by the Free Software Foundation; either
//   21  * version 3 of the License, or (at your option) any later version.
//   22  *
//   23  * rusEfi is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
//   24  * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   25  * GNU General Public License for more details.
//   26  *
//   27  * You should have received a copy of the GNU General Public License along with this program.
//   28  * If not, see <http://www.gnu.org/licenses/>.
//   29  *
//   30  */
//   31 
//   32 #include "main.h"
//   33 #include "fuel_math.h"
//   34 #include "interpolation.h"
//   35 #include "engine_configuration.h"
//   36 #include "allsensors.h"
//   37 #include "engine_math.h"
//   38 #include "rpm_calculator.h"
//   39 #include "speed_density.h"
//   40 #if EFI_ACCEL_ENRICHMENT
//   41 #include "accel_enrichment.h"
//   42 #endif /* EFI_ACCEL_ENRICHMENT */
//   43 
//   44 EXTERN_ENGINE
//   45 ;
//   46 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function __sti__routine
        THUMB
// static __intrinsic __interwork __softfp void __sti__routine()
__sti__routine:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   47 static fuel_Map3D_t fuelMap;
        LDR.W    R0,??DataTable15_1
          CFI FunCall _ZN5Map3DILi16ELi16EEC1Ev
        BL       _ZN5Map3DILi16ELi16EEC1Ev
//   48 static fuel_Map3D_t fuelPhaseMap;
        LDR.W    R0,??DataTable15_2
          CFI FunCall _ZN5Map3DILi16ELi16EEC1Ev
        BL       _ZN5Map3DILi16ELi16EEC1Ev
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock0

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
fuelMap:
        DS8 76

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
fuelPhaseMap:
        DS8 76
//   49 extern fuel_Map3D_t ve2Map;
//   50 extern fuel_Map3D_t afrMap;
//   51 extern baroCorr_Map3D_t baroCorrMap;
//   52 
//   53 /**
//   54  * @return total duration of fuel injection per engine cycle, in milliseconds
//   55  */

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z14getRealMafFuelfi
        THUMB
//   56 float getRealMafFuel(float airSpeed, int rpm DECLARE_ENGINE_PARAMETER_S) {
_Z14getRealMafFuelfi:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        VPUSH    {D8-D9}
          CFI D9 Frame(CFA, -16)
          CFI D8 Frame(CFA, -24)
          CFI CFA R13+24
        VMOV.F32 S16,S0
        MOVS     R4,R0
//   57 	if (rpm == 0)
        CMP      R4,#+0
        BNE.N    ??getRealMafFuel_0
//   58 		return 0;
        VLDR.W   S0,??DataTable9  ;; 0x0
        B.N      ??getRealMafFuel_1
//   59 	// duration of engine cycle, in hours
//   60 	float engineCycleDurationHr = 1.0 / 60 / rpm;
??getRealMafFuel_0:
        MOVS     R0,R4
          CFI FunCall __aeabi_i2d
        BL       __aeabi_i2d
        MOVS     R2,R0
        MOVS     R3,R1
        MOVS     R0,#+286331153
        LDR.W    R1,??DataTable15_3  ;; 0x3f911111
          CFI FunCall __aeabi_ddiv
        BL       __aeabi_ddiv
          CFI FunCall __aeabi_d2f
        BL       __aeabi_d2f
        VMOV     S17,R0
//   61 
//   62 	float airMassKg = airSpeed * engineCycleDurationHr;
        VMUL.F32 S18,S16,S17
//   63 
//   64 	/**
//   65 	 * todo: pre-calculate gramm/second injector flow to save one multiplication
//   66 	 * open question if that's needed since that's just a multiplication
//   67 	 */
//   68 	float injectorFlowRate = cc_minute_to_gramm_second(engineConfiguration->injector.flow);
        LDR.W    R0,??DataTable15_4
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+8]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        LDR.W    R2,??DataTable15_5  ;; 0xa32f5098
        LDR.W    R3,??DataTable15_6  ;; 0x3f889359
          CFI FunCall __aeabi_dmul
        BL       __aeabi_dmul
          CFI FunCall __aeabi_d2f
        BL       __aeabi_d2f
        VMOV     S19,R0
//   69 
//   70 	float afr = afrMap.getValue(airSpeed, rpm);
        VMOV     S0,R4
        VCVT.F32.S32 S1,S0
        VMOV.F32 S0,S16
        LDR.W    R0,??DataTable15_7
          CFI FunCall _ZN5Map3DILi16ELi16EE8getValueEff
        BL       _ZN5Map3DILi16ELi16EE8getValueEff
        VMOV.F32 S1,S0
//   71 	float fuelMassGramm = airMassKg / afr * 1000;
        VDIV.F32 S0,S18,S1
        VLDR.W   S2,??DataTable12  ;; 0x447a0000
        VMUL.F32 S0,S0,S2
//   72 
//   73 	return 1000 * fuelMassGramm / injectorFlowRate;
        VLDR.W   S2,??DataTable12  ;; 0x447a0000
        VMUL.F32 S0,S0,S2
        VDIV.F32 S0,S0,S19
??getRealMafFuel_1:
        VPOP     {D8-D9}
          CFI D8 SameValue
          CFI D9 SameValue
          CFI CFA R13+8
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock1
//   74 }
//   75 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z11getBaseFueli
        THUMB
//   76 floatms_t getBaseFuel(int rpm DECLARE_ENGINE_PARAMETER_S) {
_Z11getBaseFueli:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
//   77 	floatms_t tpsAccelEnrich = ENGINE(tpsAccelEnrichment.getTpsEnrichment(PASS_ENGINE_PARAMETER_F));
        LDR.W    R0,??DataTable15_8
          CFI FunCall _ZN16AccelEnrichmemnt16getTpsEnrichmentEv
        BL       _ZN16AccelEnrichmemnt16getTpsEnrichmentEv
        VMOV.F32 S16,S0
//   78 
//   79 	if (CONFIG(algorithm) == LM_SPEED_DENSITY) {
        LDR.W    R0,??DataTable15_9
        LDR      R0,[R0, #+432]
        CMP      R0,#+3
        BNE.N    ??getBaseFuel_0
//   80 		return tpsAccelEnrich + getSpeedDensityFuel(rpm PASS_ENGINE_PARAMETER);
        MOVS     R0,R4
          CFI FunCall _Z19getSpeedDensityFueli
        BL       _Z19getSpeedDensityFueli
        VADD.F32 S0,S16,S0
        B.N      ??getBaseFuel_1
//   81 	} else if (engineConfiguration->algorithm == LM_REAL_MAF) {
??getBaseFuel_0:
        LDR.W    R0,??DataTable15_4
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+424]
        CMP      R0,#+4
        BNE.N    ??getBaseFuel_2
//   82 		float maf = getRealMaf(PASS_ENGINE_PARAMETER_F) + engine->mapAccelEnrichment.getMapEnrichment(PASS_ENGINE_PARAMETER_F);
          CFI FunCall _Z10getRealMafv
        BL       _Z10getRealMafv
        VMOV.F32 S17,S0
        LDR.W    R0,??DataTable15_10
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+56
          CFI FunCall _ZN16AccelEnrichmemnt16getMapEnrichmentEv
        BL       _ZN16AccelEnrichmemnt16getMapEnrichmentEv
        VADD.F32 S17,S17,S0
//   83 		return tpsAccelEnrich + getRealMafFuel(maf, rpm PASS_ENGINE_PARAMETER);
        MOVS     R0,R4
        VMOV.F32 S0,S17
          CFI FunCall _Z14getRealMafFuelfi
        BL       _Z14getRealMafFuelfi
        VADD.F32 S0,S16,S0
        B.N      ??getBaseFuel_1
//   84 	} else {
//   85 		return tpsAccelEnrich + engine->engineState.baseTableFuel;
??getBaseFuel_2:
        LDR.W    R0,??DataTable15_10
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+7904
        VLDR     S0,[R0, #0]
        VADD.F32 S0,S16,S0
??getBaseFuel_1:
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock2
//   86 	}
//   87 }
//   88 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z18getinjectionOffseti
        THUMB
//   89 float getinjectionOffset(int rpm DECLARE_ENGINE_PARAMETER_S) {
_Z18getinjectionOffseti:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        VMOV     S16,R0
//   90 	float engineLoad = getEngineLoadT(PASS_ENGINE_PARAMETER_F);
          CFI FunCall _Z14getEngineLoadTv
        BL       _Z14getEngineLoadTv
        VMOV.F32 S17,S0
//   91 	return fuelPhaseMap.getValue(engineLoad, rpm);
        VCVT.F32.S32 S1,S16
        VMOV.F32 S0,S17
        LDR.N    R0,??DataTable15_2
          CFI FunCall _ZN5Map3DILi16ELi16EE8getValueEff
        BL       _ZN5Map3DILi16ELi16EE8getValueEff
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock3
//   92 }
//   93 
//   94 /**
//   95  * Number of injections into each cylinder per engine cycle
//   96  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _Z21getNumberOfInjections16injection_mode_e
        THUMB
//   97 static int getNumberOfInjections(injection_mode_e mode DECLARE_ENGINE_PARAMETER_S) {
_Z21getNumberOfInjections16injection_mode_e:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   98 	switch (mode) {
        CMP      R4,#+0
        BEQ.N    ??getNumberOfInjections_0
        CMP      R4,#+2
        BEQ.N    ??getNumberOfInjections_1
        BCC.N    ??getNumberOfInjections_2
        B.N      ??getNumberOfInjections_3
//   99 	case IM_SIMULTANEOUS:
//  100 		return engineConfiguration->specs.cylindersCount;
??getNumberOfInjections_0:
        LDR.N    R0,??DataTable15_4
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+404]
        B.N      ??getNumberOfInjections_4
//  101 	case IM_SEQUENTIAL:
//  102 		return 1;
??getNumberOfInjections_2:
        MOVS     R0,#+1
        B.N      ??getNumberOfInjections_4
//  103 	case IM_BATCH:
//  104 		return engineConfiguration->specs.cylindersCount / 2;
??getNumberOfInjections_1:
        LDR.N    R0,??DataTable15_4
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+404]
        MOVS     R1,#+2
        SDIV     R0,R0,R1
        B.N      ??getNumberOfInjections_4
//  105 	default:
//  106 		firmwareError("Unexpected getFuelMultiplier %d", mode);
??getNumberOfInjections_3:
        MOVS     R1,R4
        LDR.N    R0,??DataTable15_11
          CFI FunCall firmwareError
        BL       firmwareError
//  107 		return 1;
        MOVS     R0,#+1
??getNumberOfInjections_4:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock4
//  108 	}
//  109 }
//  110 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _Z20getInjectorDutyCyclei
          CFI NoCalls
        THUMB
//  111 percent_t getInjectorDutyCycle(int rpm DECLARE_ENGINE_PARAMETER_S) {
//  112 //	float totalPerCycle = getFuelMs(rpm) * getNumberOfInjections()
//  113 	return 0;
_Z20getInjectorDutyCyclei:
        VLDR.W   S0,??DataTable9  ;; 0x0
        BX       LR               ;; return
          CFI EndBlock cfiBlock5
//  114 }
//  115 
//  116 /**
//  117  * @returns	Length of fuel injection, in milliseconds
//  118  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _Z9getFuelMsi
        THUMB
//  119 floatms_t getFuelMs(int rpm DECLARE_ENGINE_PARAMETER_S) {
_Z9getFuelMsi:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        VPUSH    {D8-D9}
          CFI D9 Frame(CFA, -16)
          CFI D8 Frame(CFA, -24)
          CFI CFA R13+24
        MOVS     R4,R0
//  120 	float theoreticalInjectionLength;
//  121 	if (isCrankingR(rpm)) {
        CMP      R4,#+1
        BLT.N    ??getFuelMs_0
        LDR.N    R0,??DataTable15_9
        LDRSH    R0,[R0, #+104]
        CMP      R4,R0
        BGE.N    ??getFuelMs_0
//  122 		theoreticalInjectionLength = getCrankingFuel(PASS_ENGINE_PARAMETER_F)
//  123 				/ getNumberOfInjections(engineConfiguration->crankingInjectionMode PASS_ENGINE_PARAMETER);
          CFI FunCall _Z15getCrankingFuelv
        BL       _Z15getCrankingFuelv
        VMOV.F32 S17,S0
        LDR.N    R0,??DataTable15_4
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+428]
          CFI FunCall _Z21getNumberOfInjections16injection_mode_e
        BL       _Z21getNumberOfInjections16injection_mode_e
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        VDIV.F32 S0,S17,S0
        VMOV.F32 S16,S0
        B.N      ??getFuelMs_1
//  124 	} else {
//  125 		float baseFuel = getBaseFuel(rpm PASS_ENGINE_PARAMETER);
??getFuelMs_0:
        MOVS     R0,R4
          CFI FunCall _Z11getBaseFueli
        BL       _Z11getBaseFueli
        VMOV.F32 S17,S0
//  126 		float fuelPerCycle = getRunningFuel(baseFuel, rpm PASS_ENGINE_PARAMETER);
        MOVS     R0,R4
        VMOV.F32 S0,S17
          CFI FunCall _Z14getRunningFuelfi
        BL       _Z14getRunningFuelfi
        VMOV.F32 S18,S0
//  127 		theoreticalInjectionLength = fuelPerCycle
//  128 				/ getNumberOfInjections(engineConfiguration->injectionMode PASS_ENGINE_PARAMETER);
        LDR.N    R0,??DataTable15_4
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+432]
          CFI FunCall _Z21getNumberOfInjections16injection_mode_e
        BL       _Z21getNumberOfInjections16injection_mode_e
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        VDIV.F32 S0,S18,S0
        VMOV.F32 S16,S0
//  129 	}
//  130 	return theoreticalInjectionLength + ENGINE(injectorLagMs);
??getFuelMs_1:
        LDR.N    R0,??DataTable15_12
        VLDR     S0,[R0, #0]
        VADD.F32 S0,S16,S0
        VPOP     {D8-D9}
          CFI D8 SameValue
          CFI D9 SameValue
          CFI CFA R13+8
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock6
//  131 }
//  132 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function _Z14getRunningFuelfi
          CFI NoCalls
        THUMB
//  133 floatms_t getRunningFuel(floatms_t baseFuel, int rpm DECLARE_ENGINE_PARAMETER_S) {
//  134 	float iatCorrection = ENGINE(engineState.iatFuelCorrection);
_Z14getRunningFuelfi:
        LDR.N    R1,??DataTable15_13
        VLDR     S1,[R1, #0]
//  135 	float cltCorrection = ENGINE(engineState.cltFuelCorrection);
        LDR.N    R1,??DataTable15_14
        VLDR     S2,[R1, #0]
//  136 
//  137 #if EFI_ACCEL_ENRICHMENT
//  138 	float accelEnrichment = getAccelEnrichment();
//  139 	// todo: accelEnrichment
//  140 #endif /* EFI_ACCEL_ENRICHMENT */
//  141 
//  142 	return baseFuel * cltCorrection * iatCorrection;
        VMUL.F32 S0,S0,S2
        VMUL.F32 S0,S0,S1
        BX       LR               ;; return
          CFI EndBlock cfiBlock7
//  143 }
//  144 
//  145 /**
//  146  * @brief	Injector lag correction
//  147  * @param	vBatt	Battery voltage.
//  148  * @return	Time in ms for injection opening time based on current battery voltage
//  149  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function _Z14getInjectorLagf
        THUMB
//  150 floatms_t getInjectorLag(float vBatt DECLARE_ENGINE_PARAMETER_S) {
_Z14getInjectorLagf:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        VMOV.F32 S16,S0
//  151 	if (cisnan(vBatt)) {
        VMOV.F32 S0,S16
          CFI FunCall __iar_FDtest
        BL       __iar_FDtest
        CMP      R0,#+2
        BNE.N    ??getInjectorLag_0
//  152 		warning(OBD_System_Voltage_Malfunction, "vBatt=%f", vBatt);
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R1,??DataTable15_15
        MOV      R0,#+560
          CFI FunCall warning
        BL       warning
//  153 		return engineConfiguration->injector.lag;
        LDR.N    R0,??DataTable15_4
        LDR      R0,[R0, #+0]
        VLDR     S0,[R0, #+12]
        B.N      ??getInjectorLag_1
//  154 	}
//  155 	float vBattCorrection = interpolate2d(vBatt, engineConfiguration->injector.battLagCorrBins,
//  156 			engineConfiguration->injector.battLagCorr, VBAT_INJECTOR_CURVE_SIZE);
??getInjectorLag_0:
        MOVS     R2,#+8
        LDR.N    R0,??DataTable15_4
        LDR      R0,[R0, #+0]
        ADDS     R1,R0,#+48
        LDR.N    R0,??DataTable15_4
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+16
        VMOV.F32 S0,S16
          CFI FunCall _Z13interpolate2dfPfS_i
        BL       _Z13interpolate2dfPfS_i
//  157 	return engineConfiguration->injector.lag + vBattCorrection;
        LDR.N    R0,??DataTable15_4
        LDR      R0,[R0, #+0]
        VLDR     S1,[R0, #+12]
        VADD.F32 S0,S1,S0
??getInjectorLag_1:
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock8
//  158 }
//  159 
//  160 /**
//  161  * @brief	Initialize fuel map data structure
//  162  * @note this method has nothing to do with fuel map VALUES - it's job
//  163  * is to prepare the fuel map data structure for 3d interpolation
//  164  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function _Z14prepareFuelMapv
        THUMB
//  165 void prepareFuelMap(DECLARE_ENGINE_PARAMETER_F) {
_Z14prepareFuelMapv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  166 	fuelMap.init(config->fuelTable, config->fuelLoadBins, config->fuelRpmBins);
        LDR.N    R0,??DataTable15_16
        LDR      R0,[R0, #+0]
        ADD      R3,R0,#+11520
        ADDS     R3,R3,#+248
        LDR.N    R0,??DataTable15_16
        LDR      R0,[R0, #+0]
        ADD      R2,R0,#+11520
        ADDS     R2,R2,#+184
        LDR.N    R0,??DataTable15_16
        LDR      R0,[R0, #+0]
        ADD      R1,R0,#+10496
        ADDS     R1,R1,#+184
        LDR.N    R0,??DataTable15_1
          CFI FunCall _ZN5Map3DILi16ELi16EE4initEPA16_fPfS3_
        BL       _ZN5Map3DILi16ELi16EE4initEPA16_fPfS3_
//  167 	fuelPhaseMap.init(config->injectionPhase, config->injPhaseLoadBins, config->injPhaseRpmBins);
        LDR.N    R0,??DataTable15_16
        LDR      R0,[R0, #+0]
        ADD      R3,R0,#+10496
        ADDS     R3,R3,#+120
        LDR.N    R0,??DataTable15_16
        LDR      R0,[R0, #+0]
        ADD      R2,R0,#+10496
        ADDS     R2,R2,#+56
        LDR.N    R0,??DataTable15_16
        LDR      R0,[R0, #+0]
        ADD      R1,R0,#+9472
        ADDS     R1,R1,#+56
        LDR.N    R0,??DataTable15_2
          CFI FunCall _ZN5Map3DILi16ELi16EE4initEPA16_fPfS3_
        BL       _ZN5Map3DILi16ELi16EE4initEPA16_fPfS3_
//  168 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock9

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9:
        DC32     0x0
//  169 
//  170 /**
//  171  * @brief Engine warm-up fuel correction.
//  172  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function _Z16getCltCorrectionf
        THUMB
//  173 float getCltCorrection(float clt DECLARE_ENGINE_PARAMETER_S) {
_Z16getCltCorrectionf:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        VMOV.F32 S16,S0
//  174 	if (cisnan(clt))
        VMOV.F32 S0,S16
          CFI FunCall __iar_FDtest
        BL       __iar_FDtest
        CMP      R0,#+2
        BNE.N    ??getCltCorrection_0
//  175 		return 1; // this error should be already reported somewhere else, let's just handle it
        VMOV.F32 S0,#1.0
        B.N      ??getCltCorrection_1
//  176 	return interpolate2d(clt, config->cltFuelCorrBins, config->cltFuelCorr, CLT_CURVE_SIZE);
??getCltCorrection_0:
        MOVS     R2,#+16
        LDR.N    R0,??DataTable15_16
        LDR      R0,[R0, #+0]
        ADD      R1,R0,#+5632
        ADDS     R1,R1,#+248
        LDR.N    R0,??DataTable15_16
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+5632
        ADDS     R0,R0,#+184
        VMOV.F32 S0,S16
          CFI FunCall _Z13interpolate2dfPfS_i
        BL       _Z13interpolate2dfPfS_i
??getCltCorrection_1:
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock10
//  177 }
//  178 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function _Z16getIatCorrectionf
        THUMB
//  179 float getIatCorrection(float iat DECLARE_ENGINE_PARAMETER_S) {
_Z16getIatCorrectionf:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        VMOV.F32 S16,S0
//  180 	if (cisnan(iat))
        VMOV.F32 S0,S16
          CFI FunCall __iar_FDtest
        BL       __iar_FDtest
        CMP      R0,#+2
        BNE.N    ??getIatCorrection_0
//  181 		return 1; // this error should be already reported somewhere else, let's just handle it
        VMOV.F32 S0,#1.0
        B.N      ??getIatCorrection_1
//  182 	return interpolate2d(iat, config->iatFuelCorrBins, config->iatFuelCorr, IAT_CURVE_SIZE);
??getIatCorrection_0:
        MOVS     R2,#+16
        LDR.N    R0,??DataTable15_16
        LDR      R0,[R0, #+0]
        ADD      R1,R0,#+5888
        ADDS     R1,R1,#+120
        LDR.N    R0,??DataTable15_16
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+5888
        ADDS     R0,R0,#+56
        VMOV.F32 S0,S16
          CFI FunCall _Z13interpolate2dfPfS_i
        BL       _Z13interpolate2dfPfS_i
??getIatCorrection_1:
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock11
//  183 }
//  184 
//  185 /**
//  186  * @return Fuel injection duration injection as specified in the fuel map, in milliseconds
//  187  */

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function _Z16getBaseTableFuelP22engine_configuration_sif
        THUMB
//  188 floatms_t getBaseTableFuel(engine_configuration_s *engineConfiguration, int rpm, float engineLoad) {
_Z16getBaseTableFuelP22engine_configuration_sif:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        VMOV     S16,R1
        VMOV.F32 S17,S0
//  189 	if (cisnan(engineLoad)) {
        VMOV.F32 S0,S17
          CFI FunCall __iar_FDtest
        BL       __iar_FDtest
        CMP      R0,#+2
        BNE.N    ??getBaseTableFuel_0
//  190 		warning(OBD_PCM_Processor_Fault, "NaN engine load");
        LDR.N    R1,??DataTable15_17
        MOVW     R0,#+606
          CFI FunCall warning
        BL       warning
//  191 		return NAN;
        VLDR.W   S0,??DataTable15  ;; 0x7fffffff
        B.N      ??getBaseTableFuel_1
//  192 	}
//  193 	return fuelMap.getValue(engineLoad, rpm);
??getBaseTableFuel_0:
        VCVT.F32.S32 S1,S16
        VMOV.F32 S0,S17
        LDR.N    R0,??DataTable15_1
          CFI FunCall _ZN5Map3DILi16ELi16EE8getValueEff
        BL       _ZN5Map3DILi16ELi16EE8getValueEff
??getBaseTableFuel_1:
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock12
//  194 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12:
        DC32     0x447a0000
//  195 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock13 Using cfiCommon0
          CFI Function _Z17getBaroCorrectionv
        THUMB
//  196 float getBaroCorrection(DECLARE_ENGINE_PARAMETER_F) {
_Z17getBaroCorrectionv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
//  197 	if (hasBaroSensor(PASS_ENGINE_PARAMETER_F)) {
          CFI FunCall _Z13hasBaroSensorv
        BL       _Z13hasBaroSensorv
        CMP      R0,#+0
        BEQ.N    ??getBaroCorrection_0
//  198 		return baroCorrMap.getValue(getBaroPressure(PASS_ENGINE_PARAMETER_F), getRpm());
        LDR.N    R0,??DataTable15_10
        LDR      R0,[R0, #+0]
          CFI FunCall _ZN13RpmCalculator3rpmEv
        BL       _ZN13RpmCalculator3rpmEv
        VMOV     S16,R0
          CFI FunCall _Z15getBaroPressurev
        BL       _Z15getBaroPressurev
        VCVT.F32.S32 S1,S16
        LDR.N    R0,??DataTable15_18
          CFI FunCall _ZN5Map3DILi4ELi4EE8getValueEff
        BL       _ZN5Map3DILi4ELi4EE8getValueEff
        B.N      ??getBaroCorrection_1
//  199 	} else {
//  200 		return 1;
??getBaroCorrection_0:
        VMOV.F32 S0,#1.0
??getBaroCorrection_1:
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock13
//  201 	}
//  202 }
//  203 
//  204 #if EFI_ENGINE_CONTROL
//  205 /**
//  206  * @return Duration of fuel injection while craning
//  207  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock14 Using cfiCommon0
          CFI Function _Z15getCrankingFuelv
        THUMB
//  208 floatms_t getCrankingFuel(DECLARE_ENGINE_PARAMETER_F) {
_Z15getCrankingFuelv:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//  209 	return getCrankingFuel3(getCoolantTemperature(PASS_ENGINE_PARAMETER_F),
//  210 			engine->rpmCalculator.getRevolutionCounterSinceStart() PASS_ENGINE_PARAMETER);
        LDR.N    R0,??DataTable15_10
        LDR      R0,[R0, #+0]
          CFI FunCall _ZN13RpmCalculator30getRevolutionCounterSinceStartEv
        BL       _ZN13RpmCalculator30getRevolutionCounterSinceStartEv
        MOVS     R4,R0
          CFI FunCall _Z21getCoolantTemperaturev
        BL       _Z21getCoolantTemperaturev
        MOVS     R0,R4
          CFI FunCall _Z16getCrankingFuel3fj
        BL       _Z16getCrankingFuel3fj
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock14
//  211 }

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock15 Using cfiCommon0
          CFI Function _Z16getCrankingFuel3fj
        THUMB
_Z16getCrankingFuel3fj:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8-D9}
          CFI D9 Frame(CFA, -16)
          CFI D8 Frame(CFA, -24)
          CFI CFA R13+24
        VMOV.F32 S16,S0
        VMOV     S17,R0
        LDR.N    R0,??DataTable15_4
        LDR      R0,[R0, #+0]
        VLDR     S18,[R0, #+92]
        VMOV.F32 S0,S16
          CFI FunCall __iar_FDtest
        BL       __iar_FDtest
        CMP      R0,#+2
        BNE.N    ??getCrankingFuel3_0
        VMOV.F32 S0,S18
        B.N      ??getCrankingFuel3_1
??getCrankingFuel3_0:
        MOVS     R2,#+8
        LDR.N    R0,??DataTable15_16
        LDR      R0,[R0, #+0]
        ADD      R1,R0,#+5888
        ADDS     R1,R1,#+248
        LDR.N    R0,??DataTable15_16
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+6144
        ADDS     R0,R0,#+24
        VCVT.F32.U32 S0,S17
          CFI FunCall _Z13interpolate2dfPfS_i
        BL       _Z13interpolate2dfPfS_i
        VMOV.F32 S19,S0
        MOVS     R2,#+8
        LDR.N    R0,??DataTable15_16
        LDR      R0,[R0, #+0]
        ADD      R1,R0,#+5888
        ADDS     R1,R1,#+184
        LDR.N    R0,??DataTable15_16
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+5888
        ADDS     R0,R0,#+216
        VMOV.F32 S0,S16
          CFI FunCall _Z13interpolate2dfPfS_i
        BL       _Z13interpolate2dfPfS_i
        VMUL.F32 S0,S0,S18
        VMUL.F32 S0,S0,S19
??getCrankingFuel3_1:
        VPOP     {D8-D9}
          CFI D8 SameValue
          CFI D9 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock15

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15:
        DC32     0x7fffffff

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_1:
        DC32     fuelMap

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_2:
        DC32     fuelPhaseMap

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_3:
        DC32     0x3f911111

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_4:
        DC32     engineConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_5:
        DC32     0xa32f5098

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_6:
        DC32     0x3f889359

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_7:
        DC32     afrMap

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_8:
        DC32     _engine+0x14C

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_9:
        DC32     persistentState

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_10:
        DC32     engine

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_11:
        DC32     `?<Constant "Unexpected getFuelMul...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_12:
        DC32     _engine+0x1EF0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_13:
        DC32     _engine+0x1EC4

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_14:
        DC32     _engine+0x1EC8

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_15:
        DC32     `?<Constant "vBatt=%f">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_16:
        DC32     config

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_17:
        DC32     `?<Constant "NaN engine load">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_18:
        DC32     baroCorrMap

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP memset
          CFI Block cfiBlock16 Using cfiCommon0
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
          CFI EndBlock cfiBlock16

        SECTION `.text`:CODE:REORDER:NOROOT(2)
        SECTION_GROUP _ZN5Map3DILi16ELi16EE4initEPA16_fPfS3_
          CFI Block cfiBlock17 Using cfiCommon0
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
          CFI EndBlock cfiBlock17

        SECTION `.text`:CODE:REORDER:NOROOT(2)
        SECTION_GROUP _ZN5Map3DILi16ELi16EE8getValueEff
          CFI Block cfiBlock18 Using cfiCommon0
          CFI Function _ZN5Map3DILi16ELi16EE8getValueEff
        THUMB
// __interwork __vfp float Map3D<16, 16>::getValue(float, float)
_ZN5Map3DILi16ELi16EE8getValueEff:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        SUB      SP,SP,#+8
          CFI CFA R13+24
        MOVS     R4,R0
        VMOV.F32 S16,S0
        VMOV.F32 S17,S1
        LDR      R0,[R4, #+72]
        LDR.N    R1,??getValue_0+0x4  ;; 0x923f558
        CMP      R0,R1
        BEQ.N    ??getValue_1
        LDR.N    R0,??getValue_0+0x8
          CFI FunCall firmwareError
        BL       firmwareError
        VLDR.W   S0,??getValue_0  ;; 0x7fffffff
        B.N      ??getValue_2
??getValue_1:
        STR      R4,[SP, #+0]
        MOVS     R3,#+16
        LDR      R2,[R4, #+68]
        VMOV.F32 S1,S17
        MOVS     R1,#+16
        LDR      R0,[R4, #+64]
        VMOV.F32 S0,S16
          CFI FunCall _Z13interpolate3dfPfifS_iPS_
        BL       _Z13interpolate3dfPfifS_iPS_
??getValue_2:
        ADD      SP,SP,#+8
          CFI CFA R13+16
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R4,PC}          ;; return
        DATA
??getValue_0:
        DC32     0x7fffffff
        DC32     0x923f558
        DC32     _ZZN5Map3DILi16ELi16EE8getValueEffEs
          CFI EndBlock cfiBlock18

        SECTION `.text`:CODE:REORDER:NOROOT(2)
        SECTION_GROUP _ZN5Map3DILi4ELi4EE8getValueEff
          CFI Block cfiBlock19 Using cfiCommon0
          CFI Function _ZN5Map3DILi4ELi4EE8getValueEff
        THUMB
// __interwork __vfp float Map3D<4, 4>::getValue(float, float)
_ZN5Map3DILi4ELi4EE8getValueEff:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        SUB      SP,SP,#+8
          CFI CFA R13+24
        MOVS     R4,R0
        VMOV.F32 S16,S0
        VMOV.F32 S17,S1
        LDR      R0,[R4, #+24]
        LDR.N    R1,??getValue_3+0x4  ;; 0x923f558
        CMP      R0,R1
        BEQ.N    ??getValue_4
        LDR.N    R0,??getValue_3+0x8
          CFI FunCall firmwareError
        BL       firmwareError
        VLDR.W   S0,??getValue_3  ;; 0x7fffffff
        B.N      ??getValue_5
??getValue_4:
        STR      R4,[SP, #+0]
        MOVS     R3,#+4
        LDR      R2,[R4, #+20]
        VMOV.F32 S1,S17
        MOVS     R1,#+4
        LDR      R0,[R4, #+16]
        VMOV.F32 S0,S16
          CFI FunCall _Z13interpolate3dfPfifS_iPS_
        BL       _Z13interpolate3dfPfifS_iPS_
??getValue_5:
        ADD      SP,SP,#+8
          CFI CFA R13+16
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R4,PC}          ;; return
        DATA
??getValue_3:
        DC32     0x7fffffff
        DC32     0x923f558
        DC32     _ZZN5Map3DILi4ELi4EE8getValueEffEs
          CFI EndBlock cfiBlock19

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN5Map3DILi16ELi16EEC1Ev
          CFI Block cfiBlock20 Using cfiCommon0
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
          CFI EndBlock cfiBlock20

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
        SECTION_GROUP _ZZN5Map3DILi16ELi16EE8getValueEffEs
// __absolute char const <_ZZN5Map3DILi16ELi16EE8getValueEffEs>[20]
_ZZN5Map3DILi16ELi16EE8getValueEffEs:
        DATA
        DC8 "map not initialized"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        SECTION_GROUP _ZZN5Map3DILi4ELi4EE8getValueEffEs
// __absolute char const <_ZZN5Map3DILi4ELi4EE8getValueEffEs>[20]
_ZZN5Map3DILi4ELi4EE8getValueEffEs:
        DATA
        DC8 "map not initialized"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Unexpected getFuelMul...">`:
        DATA
        DC8 "Unexpected getFuelMultiplier %d"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "vBatt=%f">`:
        DATA
        DC8 "vBatt=%f"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "NaN engine load">`:
        DATA
        DC8 "NaN engine load"

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN5Map3DILi16ELi16EEC2Ev
          CFI Block cfiBlock21 Using cfiCommon0
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
          CFI EndBlock cfiBlock21

        END
//  212 #endif
//  213 
//  214 floatms_t getCrankingFuel3(float coolantTemperature,
//  215 		uint32_t revolutionCounterSinceStart DECLARE_ENGINE_PARAMETER_S) {
//  216 	// these magic constants are in Celsius
//  217 	float baseCrankingFuel = engineConfiguration->cranking.baseFuel;
//  218 	if (cisnan(coolantTemperature))
//  219 		return baseCrankingFuel;
//  220 	float durationCoef = interpolate2d(revolutionCounterSinceStart, config->crankingCycleBins,
//  221 			config->crankingCycleCoef, CRANKING_CURVE_SIZE);
//  222 
//  223 	return interpolate2d(coolantTemperature, config->crankingFuelBins,
//  224 			config->crankingFuelCoef, CRANKING_CURVE_SIZE) * baseCrankingFuel * durationCoef;
//  225 }
// 
//   152 bytes in section .bss
//     4 bytes in section .init_array
//   100 bytes in section .rodata
// 1 442 bytes in section .text
// 
// 1 182 bytes of CODE  memory (+ 264 bytes shared)
//    60 bytes of CONST memory (+  40 bytes shared)
//   152 bytes of DATA  memory
//
//Errors: none
//Warnings: 1
