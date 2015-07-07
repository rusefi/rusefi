///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:47 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\controllers\mat /
//                       h\engine_math.cpp                                    /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\controllers\mat /
//                       h\engine_math.cpp -lCN F:\stuff\rusefi_sourceforge\f /
//                       irmware\iar\Debug\List\ -lA                          /
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
//                       engine_math.s                                        /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME engine_math

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _Z10getRealMafv
        EXTERN _Z11setTableBinPfiff
        EXTERN _Z12hasMafSensorv
        EXTERN _Z13interpolate2dfPfS_i
        EXTERN _Z19getInternalAdcValuePKc13adc_channel_e
        EXTERN _Z6getMapv
        EXTERN _Z6getTPSv
        EXTERN _ZN14InjectionEventC1Ev
        EXTERN _ZNK12TriggerShape7getSizeEv
        EXTERN __aeabi_f2d
        EXTERN __aeabi_i2d
        EXTERN __aeabi_memset
        EXTERN __aeabi_vec_ctor_nocookie_nodtor
        EXTERN __iar_Dtest
        EXTERN __iar_FDtest
        EXTERN _engine
        EXTERN config
        EXTERN engine
        EXTERN engineConfiguration
        EXTERN enginePins
        EXTERN firmwareError
        EXTERN persistentState
        EXTERN warning

        PUBLIC _Z12setAlgorithm18engine_load_mode_e
        PUBLIC _Z13getCylinderId14firing_order_ei
        PUBLIC _Z13setFuelRpmBinff
        PUBLIC _Z14getEngineLoadTv
        PUBLIC _Z14setFuelLoadBinff
        PUBLIC _Z15setTimingRpmBinff
        PUBLIC _Z16getSparkDwellMsTi
        PUBLIC _Z16setTimingLoadBinff
        PUBLIC _Z18isInjectionEnabledP22engine_configuration_s
        PUBLIC _Z18setSingleCoilDwellP22engine_configuration_s
        PUBLIC _Z19findTriggerPositionP24event_trigger_position_sf
        PUBLIC _Z20prepareOutputSignalsv
        PUBLIC _Z25initializeIgnitionActionsffP9ArrayListI13IgnitionEventLi80EE
        PUBLIC _Z29getCrankshaftRevolutionTimeMsi
        PUBLIC _ZN12FuelSchedule13addFuelEventsE16injection_mode_e
        PUBLIC _ZN12FuelSchedule22registerInjectionEventEP14NamedOutputPinfb
        PUBLIC _ZN12FuelSchedule5clearEv
        PUBLIC _ZN12FuelScheduleC1Ev
        PUBLIC _ZN12FuelScheduleC2Ev
        PUBLIC _ZN9ArrayListI13IgnitionEventLi80EE3addEv
        PUBLIC _ZN9ArrayListI13IgnitionEventLi80EE5resetEv
        PUBLIC _ZN9ArrayListI14InjectionEventLi45EE3addEv
        PUBLIC _ZN9ArrayListI14InjectionEventLi45EE5resetEv
        PUBLIC _ZN9ArrayListI14InjectionEventLi45EEC1Ev
        PUBLIC _ZN9ArrayListI14InjectionEventLi45EEC2Ev
        PUBLIC _ZZN9ArrayListI13IgnitionEventLi80EE3addEvEs
        PUBLIC _ZZN9ArrayListI14InjectionEventLi45EE3addEvEs
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
        

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _ZN12FuelScheduleC2Ev
        THUMB
// __code __interwork __softfp FuelSchedule::subobject FuelSchedule()
_ZN12FuelScheduleC2Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN12FuelScheduleC1Ev
        BL       _ZN12FuelScheduleC1Ev
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock0
// F:\stuff\rusefi_sourceforge\firmware\controllers\math\engine_math.cpp
//    1 /**
//    2  * @file	engine_math.cpp
//    3  * @brief
//    4  *
//    5  * @date Jul 13, 2013
//    6  * @author Andrey Belomutskiy, (c) 2012-2015
//    7  *
//    8  * This file is part of rusEfi - see http://rusefi.com
//    9  *
//   10  * rusEfi is free software; you can redistribute it and/or modify it under the terms of
//   11  * the GNU General Public License as published by the Free Software Foundation; either
//   12  * version 3 of the License, or (at your option) any later version.
//   13  *
//   14  * rusEfi is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
//   15  * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   16  * GNU General Public License for more details.
//   17  *
//   18  * You should have received a copy of the GNU General Public License along with this program.
//   19  * If not, see <http://www.gnu.org/licenses/>.
//   20  */
//   21 
//   22 #include "main.h"
//   23 #include "engine_math.h"
//   24 #include "engine_configuration.h"
//   25 #include "interpolation.h"
//   26 #include "allsensors.h"
//   27 #include "io_pins.h"
//   28 #include "trigger_decoder.h"
//   29 #include "event_registry.h"
//   30 #include "efiGpio.h"
//   31 #include "fuel_math.h"
//   32 
//   33 EXTERN_ENGINE
//   34 ;
//   35 
//   36 extern engine_pins_s enginePins;
//   37 
//   38 /**
//   39  * @return number of milliseconds in one crankshaft revolution
//   40  */

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z29getCrankshaftRevolutionTimeMsi
          CFI NoCalls
        THUMB
//   41 float getCrankshaftRevolutionTimeMs(int rpm) {
_Z29getCrankshaftRevolutionTimeMsi:
        VMOV     S0,R0
//   42 	return 360 * getOneDegreeTimeMs(rpm);
        VCVT.F32.S32 S0,S0
        VLDR.W   S1,??DataTable4  ;; 0x4326aaab
        VDIV.F32 S0,S1,S0
        VLDR.W   S1,??DataTable4_1  ;; 0x43b40000
        VMUL.F32 S0,S0,S1
        BX       LR               ;; return
          CFI EndBlock cfiBlock1
//   43 }
//   44 
//   45 /**
//   46  * @brief Returns engine load according to selected engine_load_mode
//   47  *
//   48  */

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z14getEngineLoadTv
        THUMB
//   49 float getEngineLoadT(DECLARE_ENGINE_PARAMETER_F) {
_Z14getEngineLoadTv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   50 	efiAssert(engine!=NULL, "engine 2NULL", NAN);
        LDR.W    R0,??DataTable16_3
        LDR      R0,[R0, #+0]
        CMP      R0,#+0
        BNE.N    ??getEngineLoadT_0
        LDR.W    R0,??DataTable16_4
          CFI FunCall firmwareError
        BL       firmwareError
        VLDR.W   S0,??DataTable4_2  ;; 0x7fffffff
        B.N      ??getEngineLoadT_1
//   51 	efiAssert(engineConfiguration!=NULL, "engineConfiguration 2NULL", NAN);
??getEngineLoadT_0:
        LDR.W    R0,??DataTable16_5
        LDR      R0,[R0, #+0]
        CMP      R0,#+0
        BNE.N    ??getEngineLoadT_2
        LDR.W    R0,??DataTable16_6
          CFI FunCall firmwareError
        BL       firmwareError
        VLDR.W   S0,??DataTable4_2  ;; 0x7fffffff
        B.N      ??getEngineLoadT_1
//   52 	switch (engineConfiguration->algorithm) {
??getEngineLoadT_2:
        LDR.W    R0,??DataTable16_5
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+424]
        CMP      R0,#+0
        BEQ.N    ??getEngineLoadT_3
        CMP      R0,#+2
        BEQ.N    ??getEngineLoadT_4
        BCC.N    ??getEngineLoadT_5
        CMP      R0,#+4
        BEQ.N    ??getEngineLoadT_6
        BCC.N    ??getEngineLoadT_4
        B.N      ??getEngineLoadT_7
//   53 	case LM_PLAIN_MAF:
//   54 		if (!hasMafSensor(PASS_ENGINE_PARAMETER_F)) {
??getEngineLoadT_3:
          CFI FunCall _Z12hasMafSensorv
        BL       _Z12hasMafSensorv
        CMP      R0,#+0
        BNE.N    ??getEngineLoadT_8
//   55 			warning(OBD_PCM_Processor_Fault, "MAF sensor needed for current fuel algorithm");
        LDR.W    R1,??DataTable16_7
        MOVW     R0,#+606
          CFI FunCall warning
        BL       warning
//   56 			return NAN;
        VLDR.W   S0,??DataTable4_2  ;; 0x7fffffff
        B.N      ??getEngineLoadT_1
//   57 		}
//   58 		return getMafT(engineConfiguration);
??getEngineLoadT_8:
        LDR.W    R0,??DataTable16_5
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+580]
        ADR.N    R0,??DataTable4_3  ;; "maf"
          CFI FunCall _Z19getInternalAdcValuePKc13adc_channel_e
        BL       _Z19getInternalAdcValuePKc13adc_channel_e
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        VLDR.W   S1,??DataTable4_4  ;; 0x3a400c01
        VMUL.F32 S0,S0,S1
        LDR.W    R0,??DataTable16_5
        LDR      R0,[R0, #+0]
        VLDR     S1,[R0, #+464]
        VMUL.F32 S0,S0,S1
        B.N      ??getEngineLoadT_1
//   59 	case LM_SPEED_DENSITY:
//   60 		// SD engine load is used for timing lookup but not for fuel calculation
//   61 	case LM_MAP:
//   62 		return getMap();
??getEngineLoadT_4:
          CFI FunCall _Z6getMapv
        BL       _Z6getMapv
        B.N      ??getEngineLoadT_1
//   63 	case LM_ALPHA_N:
//   64 		return getTPS(PASS_ENGINE_PARAMETER_F);
??getEngineLoadT_5:
          CFI FunCall _Z6getTPSv
        BL       _Z6getTPSv
        B.N      ??getEngineLoadT_1
//   65 	case LM_REAL_MAF: {
//   66 		return getRealMaf(PASS_ENGINE_PARAMETER_F);
??getEngineLoadT_6:
          CFI FunCall _Z10getRealMafv
        BL       _Z10getRealMafv
        B.N      ??getEngineLoadT_1
//   67 	}
//   68 	default:
//   69 		warning(OBD_PCM_Processor_Fault, "Unexpected engine load parameter: %d", engineConfiguration->algorithm);
??getEngineLoadT_7:
        LDR.W    R0,??DataTable16_5
        LDR      R0,[R0, #+0]
        LDR      R2,[R0, #+424]
        LDR.W    R1,??DataTable16_8
        MOVW     R0,#+606
          CFI FunCall warning
        BL       warning
//   70 		return -1;
        VMOV.F32 S0,#-1.0
??getEngineLoadT_1:
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock2
//   71 	}
//   72 }
//   73 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z18setSingleCoilDwellP22engine_configuration_s
          CFI NoCalls
        THUMB
//   74 void setSingleCoilDwell(engine_configuration_s *engineConfiguration) {
//   75 	for (int i = 0; i < DWELL_CURVE_SIZE; i++) {
_Z18setSingleCoilDwellP22engine_configuration_s:
        MOVS     R1,#+0
??setSingleCoilDwell_0:
        CMP      R1,#+8
        BGE.N    ??setSingleCoilDwell_1
//   76 		engineConfiguration->sparkDwellBins[i] = -10 + i;
        SUBS     R2,R1,#+10
        VMOV     S0,R2
        VCVT.F32.S32 S0,S0
        ADDS     R2,R0,R1, LSL #+2
        VSTR     S0,[R2, #+336]
//   77 		engineConfiguration->sparkDwell[i] = -1;
        ADDS     R2,R0,R1, LSL #+2
        LDR.W    R3,??DataTable16_9  ;; 0xbf800000
        STR      R3,[R2, #+368]
//   78 	}
        ADDS     R1,R1,#+1
        B.N      ??setSingleCoilDwell_0
//   79 
//   80 	engineConfiguration->sparkDwellBins[5] = 1;
??setSingleCoilDwell_1:
        MOVS     R1,#+1065353216
        STR      R1,[R0, #+356]
//   81 	engineConfiguration->sparkDwell[5] = 4;
        MOVS     R1,#+1082130432
        STR      R1,[R0, #+388]
//   82 
//   83 	engineConfiguration->sparkDwellBins[6] = 4500;
        LDR.W    R1,??DataTable16_10  ;; 0x458ca000
        STR      R1,[R0, #+360]
//   84 	engineConfiguration->sparkDwell[6] = 4;
        MOVS     R1,#+1082130432
        STR      R1,[R0, #+392]
//   85 
//   86 	engineConfiguration->sparkDwellBins[7] = 12500;
        LDR.W    R1,??DataTable16_11  ;; 0x46435000
        STR      R1,[R0, #+364]
//   87 	engineConfiguration->sparkDwell[7] = 0;
        MOVS     R1,#+0
        STR      R1,[R0, #+396]
//   88 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock3
//   89 
//   90 #if EFI_ENGINE_CONTROL || defined(__DOXYGEN__)
//   91 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _Z25initializeIgnitionActionsffP9ArrayListI13IgnitionEventLi80EE
        THUMB
//   92 void initializeIgnitionActions(angle_t advance, angle_t dwellAngle,
//   93 		IgnitionEventList *list DECLARE_ENGINE_PARAMETER_S) {
_Z25initializeIgnitionActionsffP9ArrayListI13IgnitionEventLi80EE:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        VPUSH    {D8-D9}
          CFI D9 Frame(CFA, -32)
          CFI D8 Frame(CFA, -40)
          CFI CFA R13+40
        VMOV.F32 S17,S0
        VMOV.F32 S16,S1
        MOVS     R4,R0
//   94 	efiAssertVoid(engineConfiguration->specs.cylindersCount > 0, "cylindersCount");
        LDR.W    R0,??DataTable16_5
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+404]
        CMP      R0,#+1
        BGE.N    ??initializeIgnitionActions_0
        LDR.W    R0,??DataTable16_12
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??initializeIgnitionActions_1
//   95 
//   96 	list->reset();
??initializeIgnitionActions_0:
        MOVS     R0,R4
          CFI FunCall _ZN9ArrayListI13IgnitionEventLi80EE5resetEv
        BL       _ZN9ArrayListI13IgnitionEventLi80EE5resetEv
//   97 
//   98 	for (int i = 0; i < CONFIG(specs.cylindersCount); i++) {
        MOVS     R5,#+0
??initializeIgnitionActions_2:
        LDR.W    R0,??DataTable16_13
        LDR      R0,[R0, #+412]
        CMP      R5,R0
        BGE.N    ??initializeIgnitionActions_3
//   99 		angle_t localAdvance = advance + ENGINE(angleExtra[i]);
        LDR.W    R0,??DataTable16_14
        ADDS     R0,R0,R5, LSL #+2
        ADD      R0,R0,#+7424
        ADDS     R0,R0,#+240
        VLDR     S0,[R0, #0]
        VADD.F32 S18,S17,S0
//  100 		NamedOutputPin *output = ENGINE(ignitionPin[i]);
        LDR.W    R0,??DataTable16_14
        ADDS     R0,R0,R5, LSL #+2
        ADDS     R0,R0,#+7712
        LDR      R6,[R0, #+0]
//  101 
//  102 		IgnitionEvent *event = list->add();
        MOVS     R0,R4
          CFI FunCall _ZN9ArrayListI13IgnitionEventLi80EE3addEv
        BL       _ZN9ArrayListI13IgnitionEventLi80EE3addEv
        MOVS     R7,R0
//  103 
//  104 		if (!isPinAssigned(output)) {
        LDR      R0,[R6, #+0]
        CMP      R0,#+0
        BNE.N    ??initializeIgnitionActions_4
//  105 			// todo: extact method for this index math
//  106 			warning(OBD_PCM_Processor_Fault, "no_pin_cl #%s", output->name);
        LDR      R2,[R6, #+16]
        LDR.W    R1,??DataTable16_15
        MOVW     R0,#+606
          CFI FunCall warning
        BL       warning
//  107 		}
//  108 		event->output = output;
??initializeIgnitionActions_4:
        STR      R6,[R7, #+0]
//  109 		event->advance = localAdvance;
        VSTR     S18,[R7, #+56]
//  110 
//  111 		findTriggerPosition(&event->dwellPosition, localAdvance - dwellAngle PASS_ENGINE_PARAMETER);
        VSUB.F32 S0,S18,S16
        ADDS     R0,R7,#+60
          CFI FunCall _Z19findTriggerPositionP24event_trigger_position_sf
        BL       _Z19findTriggerPositionP24event_trigger_position_sf
//  112 	}
        ADDS     R5,R5,#+1
        B.N      ??initializeIgnitionActions_2
//  113 }
??initializeIgnitionActions_3:
??initializeIgnitionActions_1:
        VPOP     {D8-D9}
          CFI D8 SameValue
          CFI D9 SameValue
          CFI CFA R13+24
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock4
//  114 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _ZN12FuelSchedule22registerInjectionEventEP14NamedOutputPinfb
        THUMB
//  115 void FuelSchedule::registerInjectionEvent(NamedOutputPin *output, float angle,
//  116 		bool_t isSimultanious DECLARE_ENGINE_PARAMETER_S) {
_ZN12FuelSchedule22registerInjectionEventEP14NamedOutputPinfb:
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
        MOVS     R4,R0
        MOVS     R5,R1
        VMOV.F32 S16,S0
        MOVS     R6,R2
//  117 	if (!isSimultanious && !isPinAssigned(output)) {
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        CMP      R6,#+0
        BNE.N    ??registerInjectionEvent_0
        LDR      R0,[R5, #+0]
        CMP      R0,#+0
        BNE.N    ??registerInjectionEvent_0
//  118 		// todo: extact method for this index math
//  119 		warning(OBD_PCM_Processor_Fault, "no_pin_inj #%s", output->name);
        LDR      R2,[R5, #+16]
        LDR.W    R1,??DataTable16_16
        MOVW     R0,#+606
          CFI FunCall warning
        BL       warning
//  120 	}
//  121 
//  122 	InjectionEvent *ev = events.add();
??registerInjectionEvent_0:
        MOVS     R0,R4
          CFI FunCall _ZN9ArrayListI14InjectionEventLi45EE3addEv
        BL       _ZN9ArrayListI14InjectionEventLi45EE3addEv
        MOVS     R7,R0
//  123 	if (ev == NULL) {
        CMP      R7,#+0
        BEQ.N    ??registerInjectionEvent_1
//  124 		// error already reported
//  125 		return;
//  126 	}
//  127 
//  128 	ev->actuator.output = output;
??registerInjectionEvent_2:
        STR      R5,[R7, #+16]
//  129 
//  130 	ev->isSimultanious = isSimultanious;
        STRB     R6,[R7, #+120]
//  131 
//  132 	efiAssertVoid(TRIGGER_SHAPE(getSize()) > 0, "uninitialized TriggerShape");
        LDR.W    R0,??DataTable16_17
          CFI FunCall _ZNK12TriggerShape7getSizeEv
        BL       _ZNK12TriggerShape7getSizeEv
        CMP      R0,#+1
        BGE.N    ??registerInjectionEvent_3
        LDR.W    R0,??DataTable16_18
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??registerInjectionEvent_1
//  133 
//  134 	findTriggerPosition(&ev->injectionStart, angle PASS_ENGINE_PARAMETER);
??registerInjectionEvent_3:
        VMOV.F32 S0,S16
        MOVS     R0,R7
          CFI FunCall _Z19findTriggerPositionP24event_trigger_position_sf
        BL       _Z19findTriggerPositionP24event_trigger_position_sf
//  135 	if (!hasEvents[ev->injectionStart.eventIndex]) {
        MOVW     R0,#+5768
        LDR      R1,[R7, #+0]
        ADDS     R1,R1,R4
        LDRB     R0,[R0, R1]
        CMP      R0,#+0
        BNE.N    ??registerInjectionEvent_4
//  136 		hasEvents[ev->injectionStart.eventIndex] = true;
        MOVW     R0,#+5768
        LDR      R1,[R7, #+0]
        ADDS     R1,R1,R4
        MOVS     R2,#+1
        STRB     R2,[R0, R1]
//  137 		eventsCount++;
        MOVW     R0,#+6020
        LDR      R0,[R0, R4]
        ADDS     R0,R0,#+1
        MOVW     R1,#+6020
        STR      R0,[R1, R4]
//  138 	}
//  139 }
??registerInjectionEvent_4:
??registerInjectionEvent_1:
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+24
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock5

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4:
        DC32     0x4326aaab

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_1:
        DC32     0x43b40000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_2:
        DC32     0x7fffffff

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_3:
        DC8      "maf"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_4:
        DC32     0x3a400c01
//  140 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _ZN12FuelScheduleC1Ev
        THUMB
//  141 FuelSchedule::FuelSchedule() {
_ZN12FuelScheduleC1Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN9ArrayListI14InjectionEventLi45EEC1Ev
        BL       _ZN9ArrayListI14InjectionEventLi45EEC1Ev
//  142 	clear();
        MOVS     R0,R4
          CFI FunCall _ZN12FuelSchedule5clearEv
        BL       _ZN12FuelSchedule5clearEv
//  143 }
        MOVS     R0,R4
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock6
//  144 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function _ZN12FuelSchedule5clearEv
        THUMB
//  145 void FuelSchedule::clear() {
_ZN12FuelSchedule5clearEv:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  146 	memset(hasEvents, 0, sizeof(hasEvents));
        MOVS     R2,#+252
        MOVS     R1,#+0
        ADD      R0,R4,#+5632
        ADDS     R0,R0,#+136
          CFI FunCall memset
        BL       memset
//  147 	eventsCount = 0;
        MOVW     R0,#+6020
        MOVS     R1,#+0
        STR      R1,[R0, R4]
//  148 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock7
//  149 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function _ZN12FuelSchedule13addFuelEventsE16injection_mode_e
        THUMB
//  150 void FuelSchedule::addFuelEvents(injection_mode_e mode DECLARE_ENGINE_PARAMETER_S) {
_ZN12FuelSchedule13addFuelEventsE16injection_mode_e:
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
        MOVS     R4,R0
        MOVS     R5,R1
//  151 	clear(); // this method is relatively heavy
        MOVS     R0,R4
          CFI FunCall _ZN12FuelSchedule5clearEv
        BL       _ZN12FuelSchedule5clearEv
//  152 //	sourceList->reset();
//  153 
//  154 	events.reset();
        MOVS     R0,R4
          CFI FunCall _ZN9ArrayListI14InjectionEventLi45EE5resetEv
        BL       _ZN9ArrayListI14InjectionEventLi45EE5resetEv
//  155 
//  156 	efiAssertVoid(engine!=NULL, "engine is NULL");
        LDR.W    R0,??DataTable16_3
        LDR      R0,[R0, #+0]
        CMP      R0,#+0
        BNE.N    ??addFuelEvents_0
        LDR.W    R0,??DataTable16_19
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??addFuelEvents_1
//  157 
//  158 	if (cisnan(engine->rpmCalculator.oneDegreeUs))
??addFuelEvents_0:
        LDR.W    R0,??DataTable16_3
        LDR      R0,[R0, #+0]
        VLDR     S0,[R0, #+8]
          CFI FunCall __iar_FDtest
        BL       __iar_FDtest
        CMP      R0,#+2
        BEQ.W    ??addFuelEvents_1
//  159 		return;
//  160 
//  161 	efiAssertVoid(!cisnan(engine->rpmCalculator.oneDegreeUs), "NAN one deg");
??addFuelEvents_2:
        LDR.W    R0,??DataTable16_3
        LDR      R0,[R0, #+0]
        VLDR     S0,[R0, #+8]
          CFI FunCall __iar_FDtest
        BL       __iar_FDtest
        CMP      R0,#+2
        BNE.N    ??addFuelEvents_3
        LDR.W    R0,??DataTable16_20
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??addFuelEvents_1
//  162 
//  163 	/**
//  164 	 * injection phase is scheduled by injection end, so we need to step the angle back
//  165 	 * for the duration of the injection
//  166 	 */
//  167 	float baseAngle = ENGINE(engineState.injectionOffset)
//  168 			+ CONFIG(injectionOffset) - MS2US(ENGINE(fuelMs)) / ENGINE(rpmCalculator.oneDegreeUs);
??addFuelEvents_3:
        LDR.W    R0,??DataTable16_21
        VLDR     S0,[R0, #0]
        LDR.W    R0,??DataTable16_13
        VLDR     S1,[R0, #+444]
        VADD.F32 S0,S0,S1
        LDR.W    R0,??DataTable16_14
        VLDR     S1,[R0, #+608]
        VLDR.W   S2,??DataTable7  ;; 0x447a0000
        VMUL.F32 S1,S1,S2
        LDR.W    R0,??DataTable16_14
        VLDR     S2,[R0, #+8]
        VDIV.F32 S1,S1,S2
        VSUB.F32 S16,S0,S1
//  169 
//  170 	switch (mode) {
        CMP      R5,#+0
        BEQ.N    ??addFuelEvents_4
        CMP      R5,#+2
        BEQ.N    ??addFuelEvents_5
        BCS.W    ??addFuelEvents_6
//  171 	case IM_SEQUENTIAL:
//  172 		for (int i = 0; i < CONFIG(specs.cylindersCount); i++) {
??addFuelEvents_7:
        MOVS     R6,#+0
??addFuelEvents_8:
        LDR.W    R0,??DataTable16_13
        LDR      R0,[R0, #+412]
        CMP      R6,R0
        BGE.N    ??addFuelEvents_9
//  173 			int index = getCylinderId(engineConfiguration->specs.firingOrder, i) - 1;
        MOVS     R1,R6
        LDR.W    R0,??DataTable16_5
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+408]
          CFI FunCall _Z13getCylinderId14firing_order_ei
        BL       _Z13getCylinderId14firing_order_ei
        SUBS     R7,R0,#+1
//  174 			float angle = baseAngle
//  175 					+ (float) CONFIG(engineCycle) * i / CONFIG(specs.cylindersCount);
        LDR.W    R0,??DataTable16_13
        VLDR     S0,[R0, #+424]
        VCVT.F32.S32 S0,S0
        VMOV     S1,R6
        VCVT.F32.S32 S1,S1
        VMUL.F32 S0,S0,S1
        LDR.W    R0,??DataTable16_13
        VLDR     S1,[R0, #+412]
        VCVT.F32.S32 S1,S1
        VDIV.F32 S0,S0,S1
        VADD.F32 S17,S0,S16
//  176 			registerInjectionEvent(&enginePins.injectors[index], angle, false PASS_ENGINE_PARAMETER);
        MOVS     R2,#+0
        VMOV.F32 S0,S17
        MOVS     R0,#+20
        LDR.W    R1,??DataTable16_22
        MLA      R0,R0,R7,R1
        ADDS     R1,R0,#+96
        MOVS     R0,R4
          CFI FunCall _ZN12FuelSchedule22registerInjectionEventEP14NamedOutputPinfb
        BL       _ZN12FuelSchedule22registerInjectionEventEP14NamedOutputPinfb
//  177 		}
        ADDS     R6,R6,#+1
        B.N      ??addFuelEvents_8
//  178 		break;
??addFuelEvents_9:
        B.N      ??addFuelEvents_10
//  179 	case IM_SIMULTANEOUS:
//  180 		for (int i = 0; i < CONFIG(specs.cylindersCount); i++) {
??addFuelEvents_4:
        MOVS     R6,#+0
??addFuelEvents_11:
        LDR.W    R0,??DataTable16_13
        LDR      R0,[R0, #+412]
        CMP      R6,R0
        BGE.N    ??addFuelEvents_12
//  181 			float angle = baseAngle
//  182 					+ (float) CONFIG(engineCycle) * i / CONFIG(specs.cylindersCount);
        LDR.W    R0,??DataTable16_13
        VLDR     S0,[R0, #+424]
        VCVT.F32.S32 S0,S0
        VMOV     S1,R6
        VCVT.F32.S32 S1,S1
        VMUL.F32 S0,S0,S1
        LDR.W    R0,??DataTable16_13
        VLDR     S1,[R0, #+412]
        VCVT.F32.S32 S1,S1
        VDIV.F32 S0,S0,S1
        VADD.F32 S17,S0,S16
//  183 
//  184 			/**
//  185 			 * We do not need injector pin here because we will control all injectors
//  186 			 * simultaneously
//  187 			 */
//  188 			registerInjectionEvent(NULL, angle, true PASS_ENGINE_PARAMETER);
        MOVS     R2,#+1
        VMOV.F32 S0,S17
        MOVS     R1,#+0
        MOVS     R0,R4
          CFI FunCall _ZN12FuelSchedule22registerInjectionEventEP14NamedOutputPinfb
        BL       _ZN12FuelSchedule22registerInjectionEventEP14NamedOutputPinfb
//  189 		}
        ADDS     R6,R6,#+1
        B.N      ??addFuelEvents_11
//  190 		break;
??addFuelEvents_12:
        B.N      ??addFuelEvents_10
//  191 	case IM_BATCH:
//  192 		for (int i = 0; i < CONFIG(specs.cylindersCount); i++) {
??addFuelEvents_5:
        MOVS     R6,#+0
??addFuelEvents_13:
        LDR.W    R0,??DataTable16_13
        LDR      R0,[R0, #+412]
        CMP      R6,R0
        BGE.N    ??addFuelEvents_14
//  193 			int index = i % (engineConfiguration->specs.cylindersCount / 2);
        LDR.W    R0,??DataTable16_5
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+404]
        MOVS     R1,#+2
        SDIV     R0,R0,R1
        SDIV     R1,R6,R0
        MLS      R7,R0,R1,R6
//  194 			float angle = baseAngle
//  195 					+ i * (float) CONFIG(engineCycle) / CONFIG(specs.cylindersCount);
        VMOV     S0,R6
        VCVT.F32.S32 S0,S0
        LDR.W    R0,??DataTable16_13
        VLDR     S1,[R0, #+424]
        VCVT.F32.S32 S1,S1
        VMUL.F32 S0,S0,S1
        LDR.W    R0,??DataTable16_13
        VLDR     S1,[R0, #+412]
        VCVT.F32.S32 S1,S1
        VDIV.F32 S0,S0,S1
        VADD.F32 S17,S0,S16
//  196 			registerInjectionEvent(&enginePins.injectors[index], angle, false PASS_ENGINE_PARAMETER);
        MOVS     R2,#+0
        VMOV.F32 S0,S17
        MOVS     R0,#+20
        LDR.W    R1,??DataTable16_22
        MLA      R0,R0,R7,R1
        ADDS     R1,R0,#+96
        MOVS     R0,R4
          CFI FunCall _ZN12FuelSchedule22registerInjectionEventEP14NamedOutputPinfb
        BL       _ZN12FuelSchedule22registerInjectionEventEP14NamedOutputPinfb
//  197 
//  198 			if (CONFIG(twoWireBatch)) {
        LDR.W    R0,??DataTable16_13
        LDRB     R0,[R0, #+1517]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+5
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??addFuelEvents_15
//  199 
//  200 				/**
//  201 				 * also fire the 2nd half of the injectors so that we can implement a batch mode on individual wires
//  202 				 */
//  203 				index = index + (CONFIG(specs.cylindersCount) / 2);
        LDR.W    R0,??DataTable16_13
        LDR      R0,[R0, #+412]
        MOVS     R1,#+2
        SDIV     R0,R0,R1
        ADDS     R7,R0,R7
//  204 				registerInjectionEvent(&enginePins.injectors[index], angle, false PASS_ENGINE_PARAMETER);
        MOVS     R2,#+0
        VMOV.F32 S0,S17
        MOVS     R0,#+20
        LDR.W    R1,??DataTable16_22
        MLA      R0,R0,R7,R1
        ADDS     R1,R0,#+96
        MOVS     R0,R4
          CFI FunCall _ZN12FuelSchedule22registerInjectionEventEP14NamedOutputPinfb
        BL       _ZN12FuelSchedule22registerInjectionEventEP14NamedOutputPinfb
//  205 			}
//  206 		}
??addFuelEvents_15:
        ADDS     R6,R6,#+1
        B.N      ??addFuelEvents_13
//  207 		break;
??addFuelEvents_14:
        B.N      ??addFuelEvents_10
//  208 	default:
//  209 		warning(OBD_PCM_Processor_Fault, "Unexpected injection mode %d", mode);
??addFuelEvents_6:
        MOVS     R2,R5
        LDR.W    R1,??DataTable16_23
        MOVW     R0,#+606
          CFI FunCall warning
        BL       warning
//  210 	}
//  211 }
??addFuelEvents_10:
??addFuelEvents_1:
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+24
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock8
//  212 
//  213 #endif
//  214 
//  215 /**
//  216  * @return Spark dwell time, in milliseconds.
//  217  */

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function _Z16getSparkDwellMsTi
        THUMB
//  218 float getSparkDwellMsT(int rpm DECLARE_ENGINE_PARAMETER_S) {
_Z16getSparkDwellMsTi:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  219 	if (isCrankingR(rpm)) {
        CMP      R4,#+1
        BLT.N    ??getSparkDwellMsT_0
        LDR.W    R0,??DataTable16_13
        LDRSH    R0,[R0, #+104]
        CMP      R4,R0
        BGE.N    ??getSparkDwellMsT_0
//  220 		if (engineConfiguration->useConstantDwellDuringCranking) {
        LDR.W    R0,??DataTable16_5
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1496]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+4
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??getSparkDwellMsT_1
//  221 			return engineConfiguration->ignitionDwellForCrankingMs;
        LDR.W    R0,??DataTable16_5
        LDR      R0,[R0, #+0]
        VLDR     S0,[R0, #+104]
        B.N      ??getSparkDwellMsT_2
//  222 		} else {
//  223 			// technically this could be implemented via interpolate2d
//  224 			float angle = engineConfiguration->crankingChargeAngle;
??getSparkDwellMsT_1:
        LDR.W    R0,??DataTable16_5
        LDR      R0,[R0, #+0]
        VLDR     S0,[R0, #+108]
//  225 			return getOneDegreeTimeMs(rpm) * angle;
        VMOV     S1,R4
        VCVT.F32.S32 S1,S1
        VLDR.W   S2,??DataTable10  ;; 0x4326aaab
        VDIV.F32 S1,S2,S1
        VMUL.F32 S0,S1,S0
        B.N      ??getSparkDwellMsT_2
//  226 		}
//  227 	}
//  228 	efiAssert(!cisnan(rpm), "invalid rpm", NAN);
??getSparkDwellMsT_0:
        MOVS     R0,R4
          CFI FunCall __aeabi_i2d
        BL       __aeabi_i2d
        VMOV     D0,R0,R1
          CFI FunCall __iar_Dtest
        BL       __iar_Dtest
        CMP      R0,#+2
        BNE.N    ??getSparkDwellMsT_3
        LDR.W    R0,??DataTable16_24
          CFI FunCall firmwareError
        BL       firmwareError
        VLDR.W   S0,??DataTable10_1  ;; 0x7fffffff
        B.N      ??getSparkDwellMsT_2
//  229 
//  230 	return interpolate2d(rpm, engineConfiguration->sparkDwellBins, engineConfiguration->sparkDwell, DWELL_CURVE_SIZE);
??getSparkDwellMsT_3:
        MOVS     R2,#+8
        LDR.W    R0,??DataTable16_5
        LDR      R0,[R0, #+0]
        ADDS     R1,R0,#+368
        LDR.W    R0,??DataTable16_5
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+336
        VMOV     S0,R4
        VCVT.F32.S32 S0,S0
          CFI FunCall _Z13interpolate2dfPfS_i
        BL       _Z13interpolate2dfPfS_i
??getSparkDwellMsT_2:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock9
//  231 }
//  232 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function _Z14findAngleIndexf
        THUMB
//  233 static int findAngleIndex(float angleOffset DECLARE_ENGINE_PARAMETER_S) {
_Z14findAngleIndexf:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        VMOV.F32 S16,S0
//  234 	/**
//  235 	 * Here we rely on this to be pre-calculated, that's a performance optimization
//  236 	 */
//  237 	int engineCycleEventCount = engine->engineCycleEventCount;
        MOVW     R0,#+7932
        LDR.W    R1,??DataTable16_3
        LDR      R1,[R1, #+0]
        LDR      R4,[R0, R1]
//  238 
//  239 	efiAssert(engineCycleEventCount > 0, "engineCycleEventCount", 0);
        CMP      R4,#+1
        BGE.N    ??findAngleIndex_0
        LDR.W    R0,??DataTable16_25
          CFI FunCall firmwareError
        BL       firmwareError
        MOVS     R0,#+0
        B.N      ??findAngleIndex_1
//  240 
//  241 	uint32_t middle;
//  242 	uint32_t left = 0;
??findAngleIndex_0:
        MOVS     R1,#+0
//  243 	uint32_t right = engineCycleEventCount - 1;
        SUBS     R2,R4,#+1
//  244 
//  245 	/**
//  246 	 * Let's find the last trigger angle which is less or equal to the desired angle
//  247 	 * todo: extract binary search as template method?
//  248 	 */
//  249 	while (true) {
//  250 		middle = (left + right) / 2;
??findAngleIndex_2:
        ADDS     R3,R2,R1
        LSRS     R3,R3,#+1
        MOVS     R0,R3
//  251 		float eventAngle = TRIGGER_SHAPE(eventAngles[middle]);
        LDR.W    R3,??DataTable16_14
        ADDS     R3,R3,R0, LSL #+2
        LDR      R3,[R3, #+3600]
        VMOV     S0,R3
//  252 
//  253 		if (middle == left) {
        CMP      R0,R1
        BEQ.N    ??findAngleIndex_1
//  254 			return middle;
//  255 		}
//  256 		if (angleOffset < eventAngle) {
??findAngleIndex_3:
        VCMP.F32 S16,S0
        FMSTAT   
        BPL.N    ??findAngleIndex_4
//  257 			right = middle;
        MOVS     R2,R0
        B.N      ??findAngleIndex_2
//  258 		} else if (angleOffset > eventAngle) {
??findAngleIndex_4:
        VCMP.F32 S0,S16
        FMSTAT   
        BPL.N    ??findAngleIndex_5
//  259 			left = middle;
        MOVS     R1,R0
        B.N      ??findAngleIndex_2
//  260 		} else {
//  261 			return middle;
??findAngleIndex_5:
??findAngleIndex_1:
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock10
//  262 		}
//  263 	}
//  264 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7:
        DC32     0x447a0000
//  265 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function _Z19findTriggerPositionP24event_trigger_position_sf
        THUMB
//  266 void findTriggerPosition(event_trigger_position_s *position, angle_t angleOffset DECLARE_ENGINE_PARAMETER_S) {
_Z19findTriggerPositionP24event_trigger_position_sf:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        VPUSH    {D8}
          CFI D8 Frame(CFA, -24)
          CFI CFA R13+24
        SUB      SP,SP,#+8
          CFI CFA R13+32
        MOVS     R4,R0
        VMOV.F32 S16,S0
//  267 	angleOffset += tdcPosition();
        LDR.W    R0,??DataTable16_26
        VLDR     S0,[R0, #0]
        LDR.W    R0,??DataTable16_13
        VLDR     S1,[R0, #+468]
        VADD.F32 S0,S0,S1
        VADD.F32 S16,S0,S16
//  268 	fixAngle(angleOffset);
        LDR.W    R0,??DataTable16_13
        LDR      R0,[R0, #+424]
        CMP      R0,#+0
        BNE.N    ??findTriggerPosition_0
        LDR.W    R0,??DataTable16_27
          CFI FunCall firmwareError
        BL       firmwareError
??findTriggerPosition_0:
        VCMP.F32 S16,#0.0
        FMSTAT   
        BPL.N    ??findTriggerPosition_1
        LDR.W    R0,??DataTable16_13
        VLDR     S0,[R0, #+424]
        VCVT.F32.S32 S0,S0
        VADD.F32 S16,S0,S16
        B.N      ??findTriggerPosition_0
??findTriggerPosition_1:
        LDR.W    R0,??DataTable16_13
        VLDR     S0,[R0, #+424]
        VCVT.F32.S32 S0,S0
        VCMP.F32 S16,S0
        FMSTAT   
        BLT.N    ??findTriggerPosition_2
        LDR.W    R0,??DataTable16_13
        VLDR     S0,[R0, #+424]
        VCVT.F32.S32 S0,S0
        VSUB.F32 S16,S16,S0
        B.N      ??findTriggerPosition_1
//  269 
//  270 	int index = TRIGGER_SHAPE(triggerIndexByAngle[(int)angleOffset]);
??findTriggerPosition_2:
        VCVT.S32.F32 S0,S16
        VMOV     R0,S0
        LDR.N    R1,??DataTable16_14
        ADDS     R0,R1,R0, LSL #+2
        LDR      R5,[R0, #+668]
//  271 	angle_t eventAngle = TRIGGER_SHAPE(eventAngles[index]);
        LDR.N    R0,??DataTable16_14
        ADDS     R0,R0,R5, LSL #+2
        LDR      R0,[R0, #+3600]
        VMOV     S17,R0
//  272 	if (angleOffset < eventAngle) {
        VCMP.F32 S16,S17
        FMSTAT   
        BPL.N    ??findTriggerPosition_3
//  273 		warning(OBD_PCM_Processor_Fault, "angle constraint violation in findTriggerPosition(): %f/%f", angleOffset, eventAngle);
        VMOV     R0,S17
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R1,??DataTable16_28
        MOVW     R0,#+606
          CFI FunCall warning
        BL       warning
//  274 		return;
        B.N      ??findTriggerPosition_4
//  275 	}
//  276 
//  277 	position->eventIndex = index;
??findTriggerPosition_3:
        STR      R5,[R4, #+0]
//  278 	position->eventAngle = eventAngle;
        VSTR     S17,[R4, #+4]
//  279 	position->angleOffset = angleOffset - eventAngle;
        VSUB.F32 S0,S16,S17
        VSTR     S0,[R4, #+8]
//  280 }
??findTriggerPosition_4:
        ADD      SP,SP,#+8
          CFI CFA R13+24
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+16
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock11
//  281 

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//  282 static int order_1_THEN_3_THEN_4_THEN2[] = { 1, 3, 4, 2 };
order_1_THEN_3_THEN_4_THEN2:
        DATA
        DC32 1, 3, 4, 2

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//  283 static int order_1_THEN_2_THEN_4_THEN3[] = { 1, 2, 4, 3 };
order_1_THEN_2_THEN_4_THEN3:
        DATA
        DC32 1, 2, 4, 3

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//  284 static int order_1_THEN_3_THEN_2_THEN4[] = { 1, 3, 2, 4 };
order_1_THEN_3_THEN_2_THEN4:
        DATA
        DC32 1, 3, 2, 4
//  285 

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//  286 static int order_1_2_4_5_3[] = {1, 2, 4, 5, 3};
order_1_2_4_5_3:
        DATA
        DC32 1, 2, 4, 5, 3
//  287 

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//  288 static int order_1_THEN_5_THEN_3_THEN_6_THEN_2_THEN_4[] = { 1, 5, 3, 6, 2, 4 };
order_1_THEN_5_THEN_3_THEN_6_THEN_2_THEN_4:
        DATA
        DC32 1, 5, 3, 6, 2, 4

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//  289 static int order_1_THEN_4_THEN_2_THEN_5_THEN_3_THEN_6[] = { 1, 4, 2, 5, 3, 6 };
order_1_THEN_4_THEN_2_THEN_5_THEN_3_THEN_6:
        DATA
        DC32 1, 4, 2, 5, 3, 6
//  290 

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//  291 static int order_1_8_4_3_6_5_7_2[] = { 1, 8, 4, 3, 6, 5, 7, 2 };
order_1_8_4_3_6_5_7_2:
        DATA
        DC32 1, 8, 4, 3, 6, 5, 7, 2
//  292 

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//  293 static int order_1_2[] = {1, 2};
order_1_2:
        DATA
        DC32 1, 2
//  294 
//  295 /**
//  296  * @param index from zero to cylindersCount - 1
//  297  * @return cylinderId from one to cylindersCount
//  298  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function _Z13getCylinderId14firing_order_ei
        THUMB
//  299 int getCylinderId(firing_order_e firingOrder, int index) {
_Z13getCylinderId14firing_order_ei:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  300 
//  301 	switch (firingOrder) {
        CMP      R4,#+0
        BEQ.N    ??getCylinderId_0
        CMP      R4,#+2
        BEQ.N    ??getCylinderId_1
        BCC.N    ??getCylinderId_2
        CMP      R4,#+4
        BEQ.N    ??getCylinderId_3
        BCC.N    ??getCylinderId_4
        CMP      R4,#+6
        BEQ.N    ??getCylinderId_5
        BCC.N    ??getCylinderId_6
        CMP      R4,#+8
        BEQ.N    ??getCylinderId_7
        BCC.N    ??getCylinderId_8
        B.N      ??getCylinderId_9
//  302 	case FO_ONE_CYLINDER:
//  303 		return 1;
??getCylinderId_0:
        MOVS     R0,#+1
        B.N      ??getCylinderId_10
//  304 
//  305 	case FO_1_THEN_2:
//  306 		return order_1_2[index];
??getCylinderId_7:
        LDR.N    R0,??DataTable16_29
        LDR      R0,[R0, R5, LSL #+2]
        B.N      ??getCylinderId_10
//  307 // 4 cylinder
//  308 	case FO_1_THEN_3_THEN_4_THEN2:
//  309 		return order_1_THEN_3_THEN_4_THEN2[index];
??getCylinderId_2:
        LDR.N    R0,??DataTable16_30
        LDR      R0,[R0, R5, LSL #+2]
        B.N      ??getCylinderId_10
//  310 	case FO_1_THEN_2_THEN_4_THEN3:
//  311 		return order_1_THEN_2_THEN_4_THEN3[index];
??getCylinderId_1:
        LDR.N    R0,??DataTable16_31
        LDR      R0,[R0, R5, LSL #+2]
        B.N      ??getCylinderId_10
//  312 	case FO_1_THEN_3_THEN_2_THEN4:
//  313 		return order_1_THEN_3_THEN_2_THEN4[index];
??getCylinderId_4:
        LDR.N    R0,??DataTable16_32
        LDR      R0,[R0, R5, LSL #+2]
        B.N      ??getCylinderId_10
//  314 // 5 cylinder
//  315 	case FO_1_2_4_5_3:
//  316 		return order_1_2_4_5_3[index];
??getCylinderId_5:
        LDR.N    R0,??DataTable16_33
        LDR      R0,[R0, R5, LSL #+2]
        B.N      ??getCylinderId_10
//  317 
//  318 // 6 cylinder
//  319 	case FO_1_THEN_5_THEN_3_THEN_6_THEN_2_THEN_4:
//  320 		return order_1_THEN_5_THEN_3_THEN_6_THEN_2_THEN_4[index];
??getCylinderId_3:
        LDR.N    R0,??DataTable16_34
        LDR      R0,[R0, R5, LSL #+2]
        B.N      ??getCylinderId_10
//  321 	case FO_1_THEN_4_THEN_2_THEN_5_THEN_3_THEN_6:
//  322 		return order_1_THEN_4_THEN_2_THEN_5_THEN_3_THEN_6[index];
??getCylinderId_8:
        LDR.N    R0,??DataTable16_35
        LDR      R0,[R0, R5, LSL #+2]
        B.N      ??getCylinderId_10
//  323 // 8 cylinder
//  324 	case FO_1_8_4_3_6_5_7_2:
//  325 		return order_1_8_4_3_6_5_7_2[index];
??getCylinderId_6:
        LDR.N    R0,??DataTable16_36
        LDR      R0,[R0, R5, LSL #+2]
        B.N      ??getCylinderId_10
//  326 
//  327 	default:
//  328 		warning(OBD_PCM_Processor_Fault, "getCylinderId not supported for %d", firingOrder);
??getCylinderId_9:
        MOVS     R2,R4
        LDR.N    R1,??DataTable16_37
        MOVW     R0,#+606
          CFI FunCall warning
        BL       warning
//  329 	}
//  330 	return 1;
        MOVS     R0,#+1
??getCylinderId_10:
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock12
//  331 }
//  332 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock13 Using cfiCommon0
          CFI Function _Z22getIgnitionPinForIndexi
        THUMB
//  333 static NamedOutputPin * getIgnitionPinForIndex(int i DECLARE_ENGINE_PARAMETER_S
//  334 ) {
_Z22getIgnitionPinForIndexi:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  335 	switch (CONFIG(ignitionMode)) {
        LDR.N    R0,??DataTable16_13
        LDR      R0,[R0, #+452]
        CMP      R0,#+0
        BEQ.N    ??getIgnitionPinForIndex_0
        CMP      R0,#+2
        BEQ.N    ??getIgnitionPinForIndex_1
        BCC.N    ??getIgnitionPinForIndex_2
        B.N      ??getIgnitionPinForIndex_3
//  336 	case IM_ONE_COIL:
//  337 		return &enginePins.coils[0];
??getIgnitionPinForIndex_0:
        LDR.N    R0,??DataTable16_38
        B.N      ??getIgnitionPinForIndex_4
//  338 		break;
//  339 	case IM_WASTED_SPARK: {
//  340 		int wastedIndex = i % (CONFIG(specs.cylindersCount) / 2);
??getIgnitionPinForIndex_1:
        LDR.N    R0,??DataTable16_13
        LDR      R0,[R0, #+412]
        MOVS     R1,#+2
        SDIV     R0,R0,R1
        SDIV     R1,R4,R0
        MLS      R5,R0,R1,R4
//  341 		int id = getCylinderId(CONFIG(specs.firingOrder), wastedIndex);
        MOVS     R1,R5
        LDR.N    R0,??DataTable16_13
        LDR      R0,[R0, #+416]
          CFI FunCall _Z13getCylinderId14firing_order_ei
        BL       _Z13getCylinderId14firing_order_ei
//  342 		return &enginePins.coils[ID2INDEX(id)];
        MOVS     R1,#+20
        LDR.N    R2,??DataTable16_22
        MLA      R0,R1,R0,R2
        ADDS     R0,R0,#+316
        B.N      ??getIgnitionPinForIndex_4
//  343 	}
//  344 		break;
//  345 	case IM_INDIVIDUAL_COILS:
//  346 		return &enginePins.coils[ID2INDEX(getCylinderId(CONFIG(specs.firingOrder), i))];
??getIgnitionPinForIndex_2:
        MOVS     R1,R4
        LDR.N    R0,??DataTable16_13
        LDR      R0,[R0, #+416]
          CFI FunCall _Z13getCylinderId14firing_order_ei
        BL       _Z13getCylinderId14firing_order_ei
        MOVS     R1,#+20
        LDR.N    R2,??DataTable16_22
        MLA      R0,R1,R0,R2
        ADDS     R0,R0,#+316
        B.N      ??getIgnitionPinForIndex_4
//  347 		break;
//  348 
//  349 	default:
//  350 		warning(OBD_PCM_Processor_Fault, "unsupported ignitionMode %d in initializeIgnitionActions()", engineConfiguration->ignitionMode);
??getIgnitionPinForIndex_3:
        LDR.N    R0,??DataTable16_5
        LDR      R0,[R0, #+0]
        LDR      R2,[R0, #+444]
        LDR.N    R1,??DataTable16_39
        MOVW     R0,#+606
          CFI FunCall warning
        BL       warning
//  351 		return &enginePins.coils[0];
        LDR.N    R0,??DataTable16_38
??getIgnitionPinForIndex_4:
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock13
//  352 	}
//  353 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10:
        DC32     0x4326aaab

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_1:
        DC32     0x7fffffff
//  354 
//  355 #if EFI_ENGINE_CONTROL || defined(__DOXYGEN__)
//  356 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock14 Using cfiCommon0
          CFI Function _Z20prepareOutputSignalsv
        THUMB
//  357 void prepareOutputSignals(DECLARE_ENGINE_PARAMETER_F) {
_Z20prepareOutputSignalsv:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
//  358 
//  359 	engine_configuration2_s *engineConfiguration2 = engine->engineConfiguration2;
        LDR.N    R0,??DataTable16_3
        LDR      R0,[R0, #+0]
        LDR      R4,[R0, #+40]
//  360 
//  361 	for (int i = 0; i < CONFIG(specs.cylindersCount); i++) {
        MOVS     R5,#+0
??prepareOutputSignals_0:
        LDR.N    R0,??DataTable16_13
        LDR      R0,[R0, #+412]
        CMP      R5,R0
        BGE.N    ??prepareOutputSignals_1
//  362 		ENGINE(angleExtra[i])= (float) CONFIG(engineCycle) * i / CONFIG(specs.cylindersCount);
        LDR.N    R0,??DataTable16_13
        VLDR     S0,[R0, #+424]
        VCVT.F32.S32 S0,S0
        VMOV     S1,R5
        VCVT.F32.S32 S1,S1
        VMUL.F32 S0,S0,S1
        LDR.N    R0,??DataTable16_13
        VLDR     S1,[R0, #+412]
        VCVT.F32.S32 S1,S1
        VDIV.F32 S0,S0,S1
        LDR.N    R0,??DataTable16_14
        ADDS     R0,R0,R5, LSL #+2
        ADD      R0,R0,#+7424
        ADDS     R0,R0,#+240
        VSTR     S0,[R0, #0]
//  363 
//  364 		ENGINE(ignitionPin[i]) = getIgnitionPinForIndex(i PASS_ENGINE_PARAMETER);
        MOVS     R0,R5
          CFI FunCall _Z22getIgnitionPinForIndexi
        BL       _Z22getIgnitionPinForIndexi
        LDR.N    R1,??DataTable16_14
        ADDS     R1,R1,R5, LSL #+2
        ADDS     R1,R1,#+7712
        STR      R0,[R1, #+0]
//  365 
//  366 	}
        ADDS     R5,R5,#+1
        B.N      ??prepareOutputSignals_0
//  367 
//  368 	for (int angle = 0; angle < CONFIG(engineCycle); angle++) {
??prepareOutputSignals_1:
        MOVS     R5,#+0
??prepareOutputSignals_2:
        LDR.N    R0,??DataTable16_13
        LDR      R0,[R0, #+424]
        CMP      R5,R0
        BGE.N    ??prepareOutputSignals_3
//  369 		int triggerShapeIndex = findAngleIndex(angle PASS_ENGINE_PARAMETER);
        VMOV     S0,R5
        VCVT.F32.S32 S0,S0
          CFI FunCall _Z14findAngleIndexf
        BL       _Z14findAngleIndexf
//  370 		if (engineConfiguration->useOnlyFrontForTrigger)
        LDR.N    R1,??DataTable16_5
        LDR      R1,[R1, #+0]
        LDRB     R1,[R1, #+1509]
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        LSRS     R1,R1,#+6
        ANDS     R1,R1,#0x1
        CMP      R1,#+0
        BEQ.N    ??prepareOutputSignals_4
//  371 			triggerShapeIndex = triggerShapeIndex & 0xFFFFFFFE; // we need even index for front_only
        BICS     R0,R0,#0x1
//  372 		TRIGGER_SHAPE(triggerIndexByAngle[angle]) = triggerShapeIndex;
??prepareOutputSignals_4:
        LDR.N    R1,??DataTable16_14
        ADDS     R1,R1,R5, LSL #+2
        STR      R0,[R1, #+668]
//  373 	}
        ADDS     R5,R5,#+1
        B.N      ??prepareOutputSignals_2
//  374 
//  375 	engineConfiguration2->crankingInjectionEvents.addFuelEvents(
//  376 			engineConfiguration->crankingInjectionMode PASS_ENGINE_PARAMETER);
??prepareOutputSignals_3:
        LDR.N    R0,??DataTable16_5
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+428]
        MOVS     R0,R4
          CFI FunCall _ZN12FuelSchedule13addFuelEventsE16injection_mode_e
        BL       _ZN12FuelSchedule13addFuelEventsE16injection_mode_e
//  377 	engineConfiguration2->injectionEvents.addFuelEvents(
//  378 			engineConfiguration->injectionMode PASS_ENGINE_PARAMETER);
        LDR.N    R0,??DataTable16_5
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+432]
        ADD      R0,R4,#+5888
        ADDS     R0,R0,#+136
          CFI FunCall _ZN12FuelSchedule13addFuelEventsE16injection_mode_e
        BL       _ZN12FuelSchedule13addFuelEventsE16injection_mode_e
//  379 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock14
//  380 
//  381 #endif
//  382 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock15 Using cfiCommon0
          CFI Function _Z13setFuelRpmBinff
        THUMB
//  383 void setFuelRpmBin(float from, float to DECLARE_ENGINE_PARAMETER_S) {
_Z13setFuelRpmBinff:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        VMOV.F32 S16,S0
        VMOV.F32 S17,S1
//  384 	setTableBin(config->fuelRpmBins, FUEL_RPM_COUNT, from, to);
        VMOV.F32 S1,S17
        VMOV.F32 S0,S16
        MOVS     R1,#+16
        LDR.N    R0,??DataTable16_40
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+11520
        ADDS     R0,R0,#+248
          CFI FunCall _Z11setTableBinPfiff
        BL       _Z11setTableBinPfiff
//  385 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock15
//  386 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock16 Using cfiCommon0
          CFI Function _Z14setFuelLoadBinff
        THUMB
//  387 void setFuelLoadBin(float from, float to DECLARE_ENGINE_PARAMETER_S) {
_Z14setFuelLoadBinff:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        VMOV.F32 S16,S0
        VMOV.F32 S17,S1
//  388 	setTableBin(config->fuelLoadBins, FUEL_LOAD_COUNT, from, to);
        VMOV.F32 S1,S17
        VMOV.F32 S0,S16
        MOVS     R1,#+16
        LDR.N    R0,??DataTable16_40
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+11520
        ADDS     R0,R0,#+184
          CFI FunCall _Z11setTableBinPfiff
        BL       _Z11setTableBinPfiff
//  389 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock16
//  390 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock17 Using cfiCommon0
          CFI Function _Z15setTimingRpmBinff
        THUMB
//  391 void setTimingRpmBin(float from, float to DECLARE_ENGINE_PARAMETER_S) {
_Z15setTimingRpmBinff:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        VMOV.F32 S16,S0
        VMOV.F32 S17,S1
//  392 	setTableBin(config->ignitionRpmBins, IGN_RPM_COUNT, from, to);
        VMOV.F32 S1,S17
        VMOV.F32 S0,S16
        MOVS     R1,#+16
        LDR.N    R0,??DataTable16_40
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+12800
        ADDS     R0,R0,#+120
          CFI FunCall _Z11setTableBinPfiff
        BL       _Z11setTableBinPfiff
//  393 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock17
//  394 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock18 Using cfiCommon0
          CFI Function _Z16setTimingLoadBinff
        THUMB
//  395 void setTimingLoadBin(float from, float to DECLARE_ENGINE_PARAMETER_S) {
_Z16setTimingLoadBinff:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        VMOV.F32 S16,S0
        VMOV.F32 S17,S1
//  396 	setTableBin(config->ignitionLoadBins, IGN_LOAD_COUNT, from, to);
        VMOV.F32 S1,S17
        VMOV.F32 S0,S16
        MOVS     R1,#+16
        LDR.N    R0,??DataTable16_40
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+12800
        ADDS     R0,R0,#+56
          CFI FunCall _Z11setTableBinPfiff
        BL       _Z11setTableBinPfiff
//  397 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock18
//  398 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock19 Using cfiCommon0
          CFI Function _Z18isInjectionEnabledP22engine_configuration_s
          CFI NoCalls
        THUMB
//  399 int isInjectionEnabled(engine_configuration_s *engineConfiguration) {
//  400 	// todo: is this worth a method? should this be inlined?
//  401 	return engineConfiguration->isInjectionEnabled;
_Z18isInjectionEnabledP22engine_configuration_s:
        LDRB     R0,[R0, #+1508]
        ANDS     R0,R0,#0x1
        BX       LR               ;; return
          CFI EndBlock cfiBlock19
//  402 }

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock20 Using cfiCommon0
          CFI Function _Z12setAlgorithm18engine_load_mode_e
        THUMB
_Z12setAlgorithm18engine_load_mode_e:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        LDR.N    R0,??DataTable16_5
        LDR      R0,[R0, #+0]
        STR      R4,[R0, #+424]
        CMP      R4,#+1
        BNE.N    ??setAlgorithm_0
        VLDR.W   S1,??DataTable16  ;; 0x42c80000
        VLDR.W   S0,??DataTable16_1  ;; 0x0
          CFI FunCall _Z16setTimingLoadBinff
        BL       _Z16setTimingLoadBinff
        B.N      ??setAlgorithm_1
??setAlgorithm_0:
        CMP      R4,#+3
        BNE.N    ??setAlgorithm_1
        VLDR.W   S1,??DataTable16_2  ;; 0x43200000
        VLDR.W   S0,??DataTable16_1  ;; 0x0
          CFI FunCall _Z16setTimingLoadBinff
        BL       _Z16setTimingLoadBinff
??setAlgorithm_1:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock20

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16:
        DC32     0x42c80000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_1:
        DC32     0x0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_2:
        DC32     0x43200000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_3:
        DC32     engine

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_4:
        DC32     `?<Constant "engine 2NULL">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_5:
        DC32     engineConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_6:
        DC32     `?<Constant "engineConfiguration 2...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_7:
        DC32     `?<Constant "MAF sensor needed for...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_8:
        DC32     `?<Constant "Unexpected engine loa...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_9:
        DC32     0xbf800000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_10:
        DC32     0x458ca000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_11:
        DC32     0x46435000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_12:
        DC32     `?<Constant "cylindersCount">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_13:
        DC32     persistentState

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_14:
        DC32     _engine

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_15:
        DC32     `?<Constant "no_pin_cl #%s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_16:
        DC32     `?<Constant "no_pin_inj #%s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_17:
        DC32     _engine+0x284

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_18:
        DC32     `?<Constant "uninitialized Trigger...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_19:
        DC32     `?<Constant "engine is NULL">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_20:
        DC32     `?<Constant "NAN one deg">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_21:
        DC32     _engine+0x1EE4

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_22:
        DC32     enginePins

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_23:
        DC32     `?<Constant "Unexpected injection ...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_24:
        DC32     `?<Constant "invalid rpm">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_25:
        DC32     `?<Constant "engineCycleEventCount">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_26:
        DC32     _engine+0xDE4

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_27:
        DC32     `?<Constant "zero engineCycle">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_28:
        DC32     `?<Constant "angle constraint viol...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_29:
        DC32     order_1_2

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_30:
        DC32     order_1_THEN_3_THEN_4_THEN2

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_31:
        DC32     order_1_THEN_2_THEN_4_THEN3

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_32:
        DC32     order_1_THEN_3_THEN_2_THEN4

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_33:
        DC32     order_1_2_4_5_3

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_34:
        DC32     order_1_THEN_5_THEN_3_THEN_6_THEN_2_THEN_4

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_35:
        DC32     order_1_THEN_4_THEN_2_THEN_5_THEN_3_THEN_6

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_36:
        DC32     order_1_8_4_3_6_5_7_2

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_37:
        DC32     `?<Constant "getCylinderId not sup...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_38:
        DC32     enginePins+0x150

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_39:
        DC32     `?<Constant "unsupported ignitionM...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_40:
        DC32     config

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP memset
          CFI Block cfiBlock21 Using cfiCommon0
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
          CFI EndBlock cfiBlock21

        SECTION `.text`:CODE:REORDER:NOROOT(2)
        SECTION_GROUP _ZN9ArrayListI14InjectionEventLi45EEC1Ev
          CFI Block cfiBlock22 Using cfiCommon0
          CFI Function _ZN9ArrayListI14InjectionEventLi45EEC1Ev
        THUMB
// __code __interwork __softfp ArrayList<InjectionEvent, 45>::ArrayList()
_ZN9ArrayListI14InjectionEventLi45EEC1Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R3,#+45
        MOVS     R2,#+128
        LDR.N    R1,??ArrayList_0
        ADDS     R0,R4,#+8
          CFI FunCall __aeabi_vec_ctor_nocookie_nodtor
        BL       __aeabi_vec_ctor_nocookie_nodtor
        MOV      R2,#+5760
        MOVS     R1,#+0
        ADDS     R0,R4,#+8
          CFI FunCall memset
        BL       memset
        MOVS     R0,R4
          CFI FunCall _ZN9ArrayListI14InjectionEventLi45EE5resetEv
        BL       _ZN9ArrayListI14InjectionEventLi45EE5resetEv
        MOVS     R0,R4
        POP      {R4,PC}          ;; return
        Nop      
        DATA
??ArrayList_0:
        DC32     _ZN14InjectionEventC1Ev
          CFI EndBlock cfiBlock22

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN9ArrayListI14InjectionEventLi45EE5resetEv
          CFI Block cfiBlock23 Using cfiCommon0
          CFI Function _ZN9ArrayListI14InjectionEventLi45EE5resetEv
          CFI NoCalls
        THUMB
// __interwork __softfp void ArrayList<InjectionEvent, 45>::reset()
_ZN9ArrayListI14InjectionEventLi45EE5resetEv:
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
        BX       LR               ;; return
          CFI EndBlock cfiBlock23

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN9ArrayListI13IgnitionEventLi80EE5resetEv
          CFI Block cfiBlock24 Using cfiCommon0
          CFI Function _ZN9ArrayListI13IgnitionEventLi80EE5resetEv
          CFI NoCalls
        THUMB
// __interwork __softfp void ArrayList<IgnitionEvent, 80>::reset()
_ZN9ArrayListI13IgnitionEventLi80EE5resetEv:
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
        BX       LR               ;; return
          CFI EndBlock cfiBlock24

        SECTION `.text`:CODE:REORDER:NOROOT(2)
        SECTION_GROUP _ZN9ArrayListI14InjectionEventLi45EE3addEv
          CFI Block cfiBlock25 Using cfiCommon0
          CFI Function _ZN9ArrayListI14InjectionEventLi45EE3addEv
        THUMB
// __interwork __softfp InjectionEvent *ArrayList<InjectionEvent, 45>::add()
_ZN9ArrayListI14InjectionEventLi45EE3addEv:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        LDR      R0,[R4, #+0]
        CMP      R0,#+45
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
        MOVS     R1,#+128
        MLA      R0,R1,R0,R4
        ADDS     R0,R0,#+8
??add_2:
        POP      {R4,PC}          ;; return
        DATA
??add_1:
        DC32     _ZZN9ArrayListI14InjectionEventLi45EE3addEvEs
          CFI EndBlock cfiBlock25

        SECTION `.text`:CODE:REORDER:NOROOT(2)
        SECTION_GROUP _ZN9ArrayListI13IgnitionEventLi80EE3addEv
          CFI Block cfiBlock26 Using cfiCommon0
          CFI Function _ZN9ArrayListI13IgnitionEventLi80EE3addEv
        THUMB
// __interwork __softfp IgnitionEvent *ArrayList<IgnitionEvent, 80>::add()
_ZN9ArrayListI13IgnitionEventLi80EE3addEv:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        LDR      R0,[R4, #+0]
        CMP      R0,#+80
        BLT.N    ??add_3
        LDR.N    R0,??add_4
          CFI FunCall firmwareError
        BL       firmwareError
        MOVS     R0,#+0
        B.N      ??add_5
??add_3:
        LDR      R0,[R4, #+0]
        ADDS     R1,R0,#+1
        STR      R1,[R4, #+0]
        MOVS     R1,#+96
        MLA      R0,R1,R0,R4
        ADDS     R0,R0,#+8
??add_5:
        POP      {R4,PC}          ;; return
        DATA
??add_4:
        DC32     _ZZN9ArrayListI13IgnitionEventLi80EE3addEvEs
          CFI EndBlock cfiBlock26

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        SECTION_GROUP _ZZN9ArrayListI14InjectionEventLi45EE3addEvEs
// __absolute char const <_ZZN9ArrayListI14InjectionEventLi45EE3addEvEs>[24]
_ZZN9ArrayListI14InjectionEventLi45EE3addEvEs:
        DATA
        DC8 "add() too many elements"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        SECTION_GROUP _ZZN9ArrayListI13IgnitionEventLi80EE3addEvEs
// __absolute char const <_ZZN9ArrayListI13IgnitionEventLi80EE3addEvEs>[24]
_ZZN9ArrayListI13IgnitionEventLi80EE3addEvEs:
        DATA
        DC8 "add() too many elements"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "engine 2NULL">`:
        DATA
        DC8 "engine 2NULL"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "engineConfiguration 2...">`:
        DATA
        DC8 "engineConfiguration 2NULL"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "MAF sensor needed for...">`:
        DATA
        DC8 "MAF sensor needed for current fuel algorithm"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "maf"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Unexpected engine loa...">`:
        DATA
        DC8 "Unexpected engine load parameter: %d"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "cylindersCount">`:
        DATA
        DC8 "cylindersCount"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "no_pin_cl #%s">`:
        DATA
        DC8 "no_pin_cl #%s"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "invalid rpm">`:
        DATA
        DC8 "invalid rpm"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "engineCycleEventCount">`:
        DATA
        DC8 "engineCycleEventCount"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "zero engineCycle">`:
        DATA
        DC8 "zero engineCycle"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "angle constraint viol...">`:
        DATA
        DC8 61H, 6EH, 67H, 6CH, 65H, 20H, 63H, 6FH
        DC8 6EH, 73H, 74H, 72H, 61H, 69H, 6EH, 74H
        DC8 20H, 76H, 69H, 6FH, 6CH, 61H, 74H, 69H
        DC8 6FH, 6EH, 20H, 69H, 6EH, 20H, 66H, 69H
        DC8 6EH, 64H, 54H, 72H, 69H, 67H, 67H, 65H
        DC8 72H, 50H, 6FH, 73H, 69H, 74H, 69H, 6FH
        DC8 6EH, 28H, 29H, 3AH, 20H, 25H, 66H, 2FH
        DC8 25H, 66H, 0
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "getCylinderId not sup...">`:
        DATA
        DC8 "getCylinderId not supported for %d"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "unsupported ignitionM...">`:
        DATA
        DC8 75H, 6EH, 73H, 75H, 70H, 70H, 6FH, 72H
        DC8 74H, 65H, 64H, 20H, 69H, 67H, 6EH, 69H
        DC8 74H, 69H, 6FH, 6EH, 4DH, 6FH, 64H, 65H
        DC8 20H, 25H, 64H, 20H, 69H, 6EH, 20H, 69H
        DC8 6EH, 69H, 74H, 69H, 61H, 6CH, 69H, 7AH
        DC8 65H, 49H, 67H, 6EH, 69H, 74H, 69H, 6FH
        DC8 6EH, 41H, 63H, 74H, 69H, 6FH, 6EH, 73H
        DC8 28H, 29H, 0
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "engine is NULL">`:
        DATA
        DC8 "engine is NULL"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "NAN one deg">`:
        DATA
        DC8 "NAN one deg"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Unexpected injection ...">`:
        DATA
        DC8 "Unexpected injection mode %d"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "no_pin_inj #%s">`:
        DATA
        DC8 "no_pin_inj #%s"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "uninitialized Trigger...">`:
        DATA
        DC8 "uninitialized TriggerShape"
        DC8 0

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN9ArrayListI14InjectionEventLi45EEC2Ev
          CFI Block cfiBlock27 Using cfiCommon0
          CFI Function _ZN9ArrayListI14InjectionEventLi45EEC2Ev
        THUMB
// __code __interwork __softfp ArrayList<InjectionEvent, 45>::subobject ArrayList()
_ZN9ArrayListI14InjectionEventLi45EEC2Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN9ArrayListI14InjectionEventLi45EEC1Ev
        BL       _ZN9ArrayListI14InjectionEventLi45EEC1Ev
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock27

        END
//  403 
//  404 /**
//  405  * this method sets algorithm and ignition table scale
//  406  */
//  407 void setAlgorithm(engine_load_mode_e algo DECLARE_ENGINE_PARAMETER_S) {
//  408 	engineConfiguration->algorithm = algo;
//  409 	if (algo == LM_ALPHA_N) {
//  410 		setTimingLoadBin(0, 100 PASS_ENGINE_PARAMETER);
//  411 	} else if (algo == LM_SPEED_DENSITY) {
//  412 		setTimingLoadBin(0, 160 PASS_ENGINE_PARAMETER);
//  413 	}
//  414 }
// 
//   156 bytes in section .data
//   532 bytes in section .rodata
// 2 716 bytes in section .text
// 
// 2 542 bytes of CODE  memory (+ 174 bytes shared)
//   484 bytes of CONST memory (+  48 bytes shared)
//   156 bytes of DATA  memory
//
//Errors: none
//Warnings: 1
