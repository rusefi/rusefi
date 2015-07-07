///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:10:04 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\controllers\sen /
//                       sors\tps.cpp                                         /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\controllers\sen /
//                       sors\tps.cpp -lCN F:\stuff\rusefi_sourceforge\firmwa /
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
//                       tps.s                                                /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME tps

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _Z11interpolatefffff
        EXTERN _Z19getInternalAdcValuePKc13adc_channel_e
        EXTERN __aeabi_d2f
        EXTERN __aeabi_d2iz
        EXTERN __aeabi_ddiv
        EXTERN __aeabi_dmul
        EXTERN __aeabi_f2d
        EXTERN __aeabi_i2d
        EXTERN engineConfiguration
        EXTERN maxF
        EXTERN minF
        EXTERN warning

        PUBLIC _Z11getTpsValuei
        PUBLIC _Z12saveTpsStatejf
        PUBLIC _Z13getTPSVoltagev
        PUBLIC _Z14getTPS10bitAdcv
        PUBLIC _Z16getPedalPositionv
        PUBLIC _Z18getTpsRateOfChangev
        PUBLIC _Z24convertVoltageTo10bitADCf
        PUBLIC _Z6getTPSv
        PUBLIC tpsFastAdc
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\controllers\sensors\tps.cpp
//    1 #include "main.h"
//    2 #include "tps.h"
//    3 #include "engine_configuration.h"
//    4 #include "interpolation.h"
//    5 #include "adc_inputs.h"
//    6 #include "allsensors.h"
//    7 
//    8 #if !EFI_PROD_CODE
//    9 	int mockTps;
//   10 #endif
//   11 
//   12 	EXTERN_ENGINE;
//   13 
//   14 /**
//   15  * We are using one instance for read and another for modification, this is how we get lock-free thread-safety
//   16  *
//   17  */

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   18 static tps_roc_s states[2];
states:
        DS8 40
//   19 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   20 int tpsFastAdc = 0;
tpsFastAdc:
        DS8 4
//   21 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   22 static volatile int tpsRocIndex = 0;
tpsRocIndex:
        DS8 4
//   23 
//   24 /**
//   25  * this method is lock-free thread-safe if invoked only from one thread
//   26  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _Z12saveTpsStatejf
        THUMB
//   27 void saveTpsState(time_t now, float curValue) {
_Z12saveTpsStatejf:
        PUSH     {R4-R8,LR}
          CFI R14 Frame(CFA, -4)
          CFI R8 Frame(CFA, -8)
          CFI R7 Frame(CFA, -12)
          CFI R6 Frame(CFA, -16)
          CFI R5 Frame(CFA, -20)
          CFI R4 Frame(CFA, -24)
          CFI CFA R13+24
        VPUSH    {D8}
          CFI D8 Frame(CFA, -32)
          CFI CFA R13+32
        MOVS     R4,R0
        VMOV.F32 S16,S0
//   28 	int tpsNextIndex = (tpsRocIndex + 1) % 2;
        LDR.N    R0,??DataTable7_8
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+1
        MOVS     R1,#+2
        SDIV     R5,R0,R1
        MLS      R5,R5,R1,R0
//   29 	tps_roc_s *cur = &states[tpsRocIndex];
        LDR.N    R0,??DataTable7_8
        LDR      R0,[R0, #+0]
        MOVS     R1,#+20
        LDR.N    R2,??DataTable7_9
        MLA      R6,R1,R0,R2
//   30 	tps_roc_s *next = &states[tpsNextIndex];
        MOVS     R0,#+20
        LDR.N    R1,??DataTable7_9
        MLA      R7,R0,R5,R1
//   31 
//   32 	next->prevTime = cur->curTime;
        LDR      R0,[R6, #+8]
        STR      R0,[R7, #+0]
//   33 	next->prevValue = cur->curValue;
        LDR      R0,[R6, #+12]
        STR      R0,[R7, #+4]
//   34 	next->curTime = now;
        STR      R4,[R7, #+8]
//   35 	next->curValue = curValue;
        VSTR     S16,[R7, #+12]
//   36 
//   37 	int diffSysticks = overflowDiff(now, cur->curTime);
        LDR      R0,[R6, #+8]
        SUBS     R8,R4,R0
//   38 	float diffSeconds = diffSysticks * 1.0 / CH_FREQUENCY;
        MOV      R0,R8
          CFI FunCall __aeabi_i2d
        BL       __aeabi_i2d
        MOVS     R2,#+0
        LDR.N    R3,??DataTable7_10  ;; 0x3ff00000
          CFI FunCall __aeabi_dmul
        BL       __aeabi_dmul
        MOVS     R2,#+0
        LDR.N    R3,??DataTable7_11  ;; 0x408f4000
          CFI FunCall __aeabi_ddiv
        BL       __aeabi_ddiv
          CFI FunCall __aeabi_d2f
        BL       __aeabi_d2f
        VMOV     S0,R0
//   39 	next->rateOfChange = (curValue - cur->curValue) / diffSeconds;
        VLDR     S1,[R6, #+12]
        VSUB.F32 S1,S16,S1
        VDIV.F32 S1,S1,S0
        VSTR     S1,[R7, #+16]
//   40 
//   41 	// here we update volatile index
//   42 	tpsRocIndex = tpsNextIndex;
        LDR.N    R0,??DataTable7_8
        STR      R5,[R0, #+0]
//   43 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+24
        POP      {R4-R8,PC}       ;; return
          CFI EndBlock cfiBlock0
//   44 
//   45 /**
//   46  * this read-only method is lock-free thread-safe
//   47  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z18getTpsRateOfChangev
          CFI NoCalls
        THUMB
//   48 float getTpsRateOfChange(void) {
//   49 	return states[tpsRocIndex].rateOfChange;
_Z18getTpsRateOfChangev:
        LDR.N    R0,??DataTable7_8
        LDR      R0,[R0, #+0]
        MOVS     R1,#+20
        LDR.N    R2,??DataTable7_9
        MLA      R0,R1,R0,R2
        VLDR     S0,[R0, #+16]
        BX       LR               ;; return
          CFI EndBlock cfiBlock1
//   50 }
//   51 
//   52 /*
//   53  * Return current TPS position based on configured ADC levels, and adc
//   54  *
//   55  * */

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z11getTpsValuei
        THUMB
//   56 percent_t getTpsValue(int adc DECLARE_ENGINE_PARAMETER_S) {
_Z11getTpsValuei:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        VMOV     S16,R0
//   57 	if (engineConfiguration->tpsMin == engineConfiguration->tpsMax) {
        LDR.N    R0,??DataTable7_12
        LDR      R0,[R0, #+0]
        LDRSH    R0,[R0, #+82]
        LDR.N    R1,??DataTable7_12
        LDR      R1,[R1, #+0]
        LDRSH    R1,[R1, #+84]
        CMP      R0,R1
        BNE.N    ??getTpsValue_0
//   58 		warning(OBD_PCM_Processor_Fault, "Invalid TPS configuration: same value %d", engineConfiguration->tpsMin);
        LDR.N    R0,??DataTable7_12
        LDR      R0,[R0, #+0]
        LDRSH    R2,[R0, #+82]
        LDR.N    R1,??DataTable7_13
        MOVW     R0,#+606
          CFI FunCall warning
        BL       warning
//   59 		return NAN;
        VLDR.W   S0,??DataTable7  ;; 0x7fffffff
        B.N      ??getTpsValue_1
//   60 	}
//   61 	float result = interpolate(engineConfiguration->tpsMin, 0, engineConfiguration->tpsMax, 100, adc);
??getTpsValue_0:
        VCVT.F32.S32 S4,S16
        VLDR.W   S3,??DataTable7_1  ;; 0x42c80000
        LDR.N    R0,??DataTable7_12
        LDR      R0,[R0, #+0]
        LDRSH    R0,[R0, #+84]
        VMOV     S0,R0
        VCVT.F32.S32 S2,S0
        VLDR.W   S1,??DataTable7_2  ;; 0x0
        LDR.N    R0,??DataTable7_12
        LDR      R0,[R0, #+0]
        LDRSH    R0,[R0, #+82]
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
          CFI FunCall _Z11interpolatefffff
        BL       _Z11interpolatefffff
        VMOV.F32 S17,S0
//   62 	// this would put the value into the 0-100 range
//   63 	return maxF(0, minF(100, result));
        VMOV.F32 S1,S17
        VLDR.W   S0,??DataTable7_1  ;; 0x42c80000
          CFI FunCall minF
        BL       minF
        VMOV.F32 S1,S0
        VLDR.W   S0,??DataTable7_2  ;; 0x0
          CFI FunCall maxF
        BL       maxF
??getTpsValue_1:
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock2
//   64 }
//   65 
//   66 /*
//   67  * Return voltage on TPS AND channel
//   68  * */

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z13getTPSVoltagev
        THUMB
//   69 float getTPSVoltage(DECLARE_ENGINE_PARAMETER_F) {
_Z13getTPSVoltagev:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   70 	return getVoltageDivided("tps", engineConfiguration->tpsAdcChannel);
        LDR.N    R0,??DataTable7_12
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+524]
        ADR.N    R0,??DataTable7_3  ;; "tps"
          CFI FunCall _Z19getInternalAdcValuePKc13adc_channel_e
        BL       _Z19getInternalAdcValuePKc13adc_channel_e
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        VLDR.W   S1,??DataTable7_4  ;; 0x3a400c01
        VMUL.F32 S0,S0,S1
        LDR.N    R0,??DataTable7_12
        LDR      R0,[R0, #+0]
        VLDR     S1,[R0, #+464]
        VMUL.F32 S0,S0,S1
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock3
//   71 }
//   72 
//   73 /*
//   74  * Return TPS ADC readings.
//   75  * We need ADC value because TunerStudio has a nice TPS configuration wizard, and this wizard
//   76  * wants a TPS value.
//   77  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _Z14getTPS10bitAdcv
          CFI NoCalls
        THUMB
//   78 int getTPS10bitAdc(DECLARE_ENGINE_PARAMETER_F) {
//   79 #if !EFI_PROD_CODE
//   80 	if (mockTps != MOCK_UNDEFINED)
//   81 		return mockTps;
//   82 #endif
//   83 	if(engineConfiguration->tpsAdcChannel==EFI_ADC_NONE)
_Z14getTPS10bitAdcv:
        LDR.N    R0,??DataTable7_12
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+524]
        CMP      R0,#+16
        BNE.N    ??getTPS10bitAdc_0
//   84 		return -1;
        MOVS     R0,#-1
        B.N      ??getTPS10bitAdc_1
//   85 #if EFI_PROD_CODE
//   86 	return tpsFastAdc / 4;
??getTPS10bitAdc_0:
        LDR.N    R0,??DataTable7_14
        LDR      R0,[R0, #+0]
        MOVS     R1,#+4
        SDIV     R0,R0,R1
??getTPS10bitAdc_1:
        BX       LR               ;; return
          CFI EndBlock cfiBlock4
//   87 #else
//   88 	return 0;
//   89 #endif /* EFI_PROD_CODE */
//   90 }
//   91 
//   92 /**
//   93  * @brief Position on physical primary TPS
//   94  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _Z16getPrimatyRawTPSv
        THUMB
//   95 static float getPrimatyRawTPS(DECLARE_ENGINE_PARAMETER_F) {
_Z16getPrimatyRawTPSv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   96 	float tpsValue = getTpsValue(getTPS10bitAdc(PASS_ENGINE_PARAMETER_F) PASS_ENGINE_PARAMETER);
          CFI FunCall _Z14getTPS10bitAdcv
        BL       _Z14getTPS10bitAdcv
          CFI FunCall _Z11getTpsValuei
        BL       _Z11getTpsValuei
//   97 	return tpsValue;
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock5
//   98 }
//   99 
//  100 #define NO_TPS_MAGIC_VALUE 66.611
//  101 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _Z16getPedalPositionv
        THUMB
//  102 percent_t getPedalPosition(DECLARE_ENGINE_PARAMETER_F) {
_Z16getPedalPositionv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
//  103 	float voltage = getVoltageDivided("pPS", engineConfiguration->pedalPositionChannel);
        LDR.N    R0,??DataTable7_12
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+604]
        ADR.N    R0,??DataTable7_5  ;; "pPS"
          CFI FunCall _Z19getInternalAdcValuePKc13adc_channel_e
        BL       _Z19getInternalAdcValuePKc13adc_channel_e
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        VLDR.W   S1,??DataTable7_4  ;; 0x3a400c01
        VMUL.F32 S0,S0,S1
        LDR.N    R0,??DataTable7_12
        LDR      R0,[R0, #+0]
        VLDR     S1,[R0, #+464]
        VMUL.F32 S16,S0,S1
//  104 	float result = interpolate(engineConfiguration->pedalPositionMin, 0, engineConfiguration->pedalPositionMax, 100, voltage);
        VMOV.F32 S4,S16
        VLDR.W   S3,??DataTable7_1  ;; 0x42c80000
        LDR.N    R0,??DataTable7_12
        LDR      R0,[R0, #+0]
        LDRSH    R0,[R0, #+574]
        VMOV     S0,R0
        VCVT.F32.S32 S2,S0
        VLDR.W   S1,??DataTable7_2  ;; 0x0
        LDR.N    R0,??DataTable7_12
        LDR      R0,[R0, #+0]
        LDRSH    R0,[R0, #+572]
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
          CFI FunCall _Z11interpolatefffff
        BL       _Z11interpolatefffff
        VMOV.F32 S17,S0
//  105 
//  106 	// this would put the value into the 0-100 range
//  107 	return maxF(0, minF(100, result));
        VMOV.F32 S1,S17
        VLDR.W   S0,??DataTable7_1  ;; 0x42c80000
          CFI FunCall minF
        BL       minF
        VMOV.F32 S1,S0
        VLDR.W   S0,??DataTable7_2  ;; 0x0
          CFI FunCall maxF
        BL       maxF
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock6
//  108 }
//  109 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function _Z6getTPSv
        THUMB
//  110 percent_t getTPS(DECLARE_ENGINE_PARAMETER_F) {
_Z6getTPSv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  111 	if (!engineConfiguration->hasTpsSensor)
        LDR.N    R0,??DataTable7_12
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1497]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+7
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        CMP      R0,#+0
        BNE.N    ??getTPS_0
//  112 		return NO_TPS_MAGIC_VALUE;
        VLDR.W   S0,??DataTable7_6  ;; 0x428538d5
        B.N      ??getTPS_1
//  113 	// todo: if (config->isDualTps)
//  114 	// todo: blah blah
//  115 	// todo: if two TPS do not match - show OBD code via malfunction_central.c
//  116 
//  117 	return getPrimatyRawTPS(PASS_ENGINE_PARAMETER_F);
??getTPS_0:
          CFI FunCall _Z16getPrimatyRawTPSv
        BL       _Z16getPrimatyRawTPSv
??getTPS_1:
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock7
//  118 }
//  119 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function _Z24convertVoltageTo10bitADCf
        THUMB
//  120 int convertVoltageTo10bitADC(float voltage) {
_Z24convertVoltageTo10bitADCf:
        PUSH     {LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+4
//  121 	// divided by 2 because of voltage divider, then converted into 10bit ADC value (TunerStudio format)
//  122 	return (int) (voltage / 2 * 1024 / 3.3);
        VMOV.F32 S1,#2.0
        VDIV.F32 S0,S0,S1
        VLDR.W   S1,??DataTable7_7  ;; 0x44800000
        VMUL.F32 S0,S0,S1
        VMOV     R0,S0
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,#+1717986918
        LDR.N    R3,??DataTable7_15  ;; 0x400a6666
          CFI FunCall __aeabi_ddiv
        BL       __aeabi_ddiv
          CFI FunCall __aeabi_d2iz
        BL       __aeabi_d2iz
        POP      {PC}             ;; return
          CFI EndBlock cfiBlock8
//  123 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7:
        DC32     0x7fffffff

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_1:
        DC32     0x42c80000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_2:
        DC32     0x0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_3:
        DC8      "tps"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_4:
        DC32     0x3a400c01

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_5:
        DC8      "pPS"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_6:
        DC32     0x428538d5

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_7:
        DC32     0x44800000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_8:
        DC32     tpsRocIndex

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_9:
        DC32     states

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_10:
        DC32     0x3ff00000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_11:
        DC32     0x408f4000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_12:
        DC32     engineConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_13:
        DC32     `?<Constant "Invalid TPS configura...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_14:
        DC32     tpsFastAdc

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_15:
        DC32     0x400a6666

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Invalid TPS configura...">`:
        DATA
        DC8 "Invalid TPS configuration: same value %d"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "tps"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "pPS"

        END
// 
//  48 bytes in section .bss
//  52 bytes in section .rodata
// 638 bytes in section .text
// 
// 638 bytes of CODE  memory
//  52 bytes of CONST memory
//  48 bytes of DATA  memory
//
//Errors: none
//Warnings: 1
