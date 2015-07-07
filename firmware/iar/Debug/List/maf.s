///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:08:45 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\controllers\sen /
//                       sors\maf.cpp                                         /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\controllers\sen /
//                       sors\maf.cpp -lCN F:\stuff\rusefi_sourceforge\firmwa /
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
//                       maf.s                                                /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME maf

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _Z19getInternalAdcValuePKc13adc_channel_e
        EXTERN engine
        EXTERN engineConfiguration

        PUBLIC _Z10getRealMafv
        PUBLIC _Z12hasMafSensorv
        PUBLIC _Z12setDensoTODOP19persistent_config_s
        PUBLIC _Z18setBosch0280218004P19persistent_config_s
        PUBLIC _Z18setBosch0280218037P19persistent_config_s
        PUBLIC _Z18setMazdaMiataNAMafP19persistent_config_s
        PUBLIC _Z6getMafv
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\controllers\sensors\maf.cpp
//    1 #include "main.h"
//    2 #include "engine.h"
//    3 #include "adc_inputs.h"
//    4 #include "maf.h"
//    5 
//    6 EXTERN_ENGINE
//    7 ;
//    8 
//    9 /**
//   10  * @return MAF sensor voltage
//   11  */

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _Z6getMafv
        THUMB
//   12 float getMaf(DECLARE_ENGINE_PARAMETER_F) {
_Z6getMafv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   13 	return getMafT(engineConfiguration);
        LDR.W    R0,??DataTable6_55
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+580]
        ADR.N    R0,??DataTable5  ;; "maf"
          CFI FunCall _Z19getInternalAdcValuePKc13adc_channel_e
        BL       _Z19getInternalAdcValuePKc13adc_channel_e
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        VLDR.W   S1,??DataTable5_1  ;; 0x3a400c01
        VMUL.F32 S0,S0,S1
        LDR.W    R0,??DataTable6_55
        LDR      R0,[R0, #+0]
        VLDR     S1,[R0, #+464]
        VMUL.F32 S0,S0,S1
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock0
//   14 }
//   15 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z12hasMafSensorv
          CFI NoCalls
        THUMB
//   16 bool_t hasMafSensor(DECLARE_ENGINE_PARAMETER_F) {
//   17 	return engineConfiguration->hasMafSensor && engineConfiguration->mafAdcChannel != EFI_ADC_NONE;
_Z12hasMafSensorv:
        LDR.W    R0,??DataTable6_55
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1497]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+6
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??hasMafSensor_0
        LDR.W    R0,??DataTable6_55
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+580]
        CMP      R0,#+16
        BEQ.N    ??hasMafSensor_0
        MOVS     R0,#+1
        B.N      ??hasMafSensor_1
??hasMafSensor_0:
        MOVS     R0,#+0
??hasMafSensor_1:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        BX       LR               ;; return
          CFI EndBlock cfiBlock1
//   18 }
//   19 
//   20 /**
//   21  * @return kg/hour value
//   22  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z10getRealMafv
        THUMB
//   23 float getRealMaf(DECLARE_ENGINE_PARAMETER_F) {
_Z10getRealMafv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   24 	int mafAdc = getAdcValue("maf", engineConfiguration->mafAdcChannel);
        LDR.W    R0,??DataTable6_55
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+580]
        ADR.N    R0,??DataTable5  ;; "maf"
          CFI FunCall _Z19getInternalAdcValuePKc13adc_channel_e
        BL       _Z19getInternalAdcValuePKc13adc_channel_e
//   25 	/**
//   26 	 * here we drop from 12 bit ADC to 8 bit index
//   27 	 */
//   28 	return engine->mafDecodingLookup[mafAdc >> 4];
        ASRS     R0,R0,#+4
        LDR.W    R1,??DataTable6_56
        LDR      R1,[R1, #+0]
        ADDS     R0,R1,R0, LSL #+2
        ADD      R0,R0,#+7936
        ADDS     R0,R0,#+68
        VLDR     S0,[R0, #0]
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock2
//   29 }
//   30 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z11fillTheRestP19persistent_config_si
          CFI NoCalls
        THUMB
//   31 static void fillTheRest(persistent_config_s *e, int i) {
_Z11fillTheRestP19persistent_config_si:
        PUSH     {R4}
          CFI R4 Frame(CFA, -4)
          CFI CFA R13+4
//   32 	/**
//   33 	 * unrealistic values just to make binary search happy
//   34 	 */
//   35 	while (i < MAF_DECODING_COUNT) {
??fillTheRest_0:
        CMP      R1,#+255
        BGT.N    ??fillTheRest_1
//   36 		e->mafDecoding[i] = 200;
        MOVW     R2,#+6328
        ADDS     R3,R0,R1, LSL #+2
        LDR.W    R4,??DataTable6_57  ;; 0x43480000
        STR      R4,[R2, R3]
//   37 		e->mafDecodingBins[i] = 10 + i;
        ADDS     R2,R1,#+10
        VMOV     S0,R2
        VCVT.F32.S32 S0,S0
        ADDS     R2,R0,R1, LSL #+2
        ADD      R2,R2,#+7168
        ADDS     R2,R2,#+184
        VSTR     S0,[R2, #0]
//   38 		i++;
        ADDS     R1,R1,#+1
        B.N      ??fillTheRest_0
//   39 	}
//   40 }
??fillTheRest_1:
        POP      {R4}
          CFI R4 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock3
//   41 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _Z11addMafPointP19persistent_config_siff
          CFI NoCalls
        THUMB
//   42 static int addMafPoint(persistent_config_s *e, int i, float kgHrValue, float voltage) {
_Z11addMafPointP19persistent_config_siff:
        MOVS     R2,R0
//   43 	e->mafDecoding[i] = kgHrValue;
        ADDS     R0,R2,R1, LSL #+2
        ADD      R0,R0,#+6144
        ADDS     R0,R0,#+184
        VSTR     S0,[R0, #0]
//   44 	e->mafDecodingBins[i] = voltage;
        ADDS     R0,R2,R1, LSL #+2
        ADD      R0,R0,#+7168
        ADDS     R0,R0,#+184
        VSTR     S1,[R0, #0]
//   45 	return i + 1;
        ADDS     R0,R1,#+1
        BX       LR               ;; return
          CFI EndBlock cfiBlock4
//   46 }
//   47 
//   48 /**
//   49  * Hot-film air-mass meter, Type HFM 5
//   50  */

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _Z18setBosch0280218037P19persistent_config_s
        THUMB
//   51 void setBosch0280218037(persistent_config_s *e) {
_Z18setBosch0280218037P19persistent_config_s:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//   52 	int i = 0;
        MOVS     R5,#+0
//   53 	i = addMafPoint(e, i, -34.5, 0);
        VLDR.W   S1,??DataTable5_2  ;; 0x0
        VLDR.W   S0,??DataTable5_3  ;; 0xc20a0000
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//   54 	i = addMafPoint(e, i, -6, 0.78125);
        VMOV.F32 S1,#0.78125
        VMOV.F32 S0,#-6.0
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//   55 
//   56 	e->mafDecoding[i] = 10.5;
        MOVW     R0,#+6328
        ADDS     R1,R4,R5, LSL #+2
        LDR.W    R2,??DataTable6_58  ;; 0x41280000
        STR      R2,[R0, R1]
//   57 	e->mafDecodingBins[i++] = 1.38671875;
        MOVW     R0,#+7352
        ADDS     R1,R4,R5, LSL #+2
        LDR.W    R2,??DataTable6_59  ;; 0x3fb18000
        STR      R2,[R0, R1]
        ADDS     R5,R5,#+1
//   58 
//   59 	e->mafDecoding[i] = 105.3;
        MOVW     R0,#+6328
        ADDS     R1,R4,R5, LSL #+2
        LDR.W    R2,??DataTable6_60  ;; 0x42d2999a
        STR      R2,[R0, R1]
//   60 	e->mafDecodingBins[i++] = 2.91015625;
        MOVW     R0,#+7352
        ADDS     R1,R4,R5, LSL #+2
        LDR.W    R2,??DataTable6_61  ;; 0x403a4000
        STR      R2,[R0, R1]
        ADDS     R5,R5,#+1
//   61 
//   62 	e->mafDecoding[i] = 387.5;
        MOVW     R0,#+6328
        ADDS     R1,R4,R5, LSL #+2
        LDR.W    R2,??DataTable6_62  ;; 0x43c1c000
        STR      R2,[R0, R1]
//   63 	e->mafDecodingBins[i++] = 4.2578125;
        MOVW     R0,#+7352
        ADDS     R1,R4,R5, LSL #+2
        LDR.W    R2,??DataTable6_63  ;; 0x40884000
        STR      R2,[R0, R1]
        ADDS     R5,R5,#+1
//   64 
//   65 	e->mafDecoding[i] = 738;
        MOVW     R0,#+6328
        ADDS     R1,R4,R5, LSL #+2
        LDR.W    R2,??DataTable6_64  ;; 0x44388000
        STR      R2,[R0, R1]
//   66 	e->mafDecodingBins[i++] = 4.98046875;
        MOVW     R0,#+7352
        ADDS     R1,R4,R5, LSL #+2
        LDR.W    R2,??DataTable6_65  ;; 0x409f6000
        STR      R2,[R0, R1]
        ADDS     R5,R5,#+1
//   67 
//   68 	fillTheRest(e, i);
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11fillTheRestP19persistent_config_si
        BL       _Z11fillTheRestP19persistent_config_si
//   69 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock5
//   70 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _Z18setBosch0280218004P19persistent_config_s
        THUMB
//   71 void setBosch0280218004(persistent_config_s *e) {
_Z18setBosch0280218004P19persistent_config_s:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//   72 	int i = 0;
        MOVS     R5,#+0
//   73 
//   74 	fillTheRest(e, i);
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11fillTheRestP19persistent_config_si
        BL       _Z11fillTheRestP19persistent_config_si
//   75 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock6
//   76 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function _Z12setDensoTODOP19persistent_config_s
        THUMB
//   77 void setDensoTODO(persistent_config_s *e) {
_Z12setDensoTODOP19persistent_config_s:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//   78 	int i = 0;
        MOVS     R5,#+0
//   79 
//   80 	i = addMafPoint(e, i, 116.3, 0.8);
        VLDR.W   S1,??DataTable5_4  ;; 0x3f4ccccd
        VLDR.W   S0,??DataTable5_5  ;; 0x42e8999a
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//   81 	i = addMafPoint(e, i, 76.3, 1.04);
        VLDR.W   S1,??DataTable5_6  ;; 0x3f851eb8
        VLDR.W   S0,??DataTable5_7  ;; 0x4298999a
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//   82 	i = addMafPoint(e, i, 40.5, 1.4);
        VLDR.W   S1,??DataTable5_8  ;; 0x3fb33333
        VLDR.W   S0,??DataTable5_9  ;; 0x42220000
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//   83 	i = addMafPoint(e, i, 32.5, 1.60);
        VLDR.W   S1,??DataTable5_10  ;; 0x3fcccccd
        VLDR.W   S0,??DataTable5_11  ;; 0x42020000
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//   84 	i = addMafPoint(e, i, 23.8, 1.83);
        VLDR.W   S1,??DataTable5_12  ;; 0x3fea3d71
        VLDR.W   S0,??DataTable5_13  ;; 0x41be6666
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//   85 	i = addMafPoint(e, i, 0, 3);
        VMOV.F32 S1,#3.0
        VLDR.W   S0,??DataTable5_2  ;; 0x0
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//   86 
//   87 	while (i < MAF_DECODING_COUNT) {
??setDensoTODO_0:
        CMP      R5,#+255
        BGT.N    ??setDensoTODO_1
//   88 		e->mafDecoding[i] = 0;
        MOVW     R0,#+6328
        ADDS     R1,R4,R5, LSL #+2
        MOVS     R2,#+0
        STR      R2,[R0, R1]
//   89 		e->mafDecodingBins[i] = 10 + i;
        ADDS     R0,R5,#+10
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        ADDS     R0,R4,R5, LSL #+2
        ADD      R0,R0,#+7168
        ADDS     R0,R0,#+184
        VSTR     S0,[R0, #0]
//   90 		i++;
        ADDS     R5,R5,#+1
        B.N      ??setDensoTODO_0
//   91 	}
//   92 }
??setDensoTODO_1:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock7

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5:
        DC8      "maf"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_1:
        DC32     0x3a400c01

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_2:
        DC32     0x0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_3:
        DC32     0xc20a0000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_4:
        DC32     0x3f4ccccd

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_5:
        DC32     0x42e8999a

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_6:
        DC32     0x3f851eb8

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_7:
        DC32     0x4298999a

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_8:
        DC32     0x3fb33333

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_9:
        DC32     0x42220000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_10:
        DC32     0x3fcccccd

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_11:
        DC32     0x42020000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_12:
        DC32     0x3fea3d71

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_13:
        DC32     0x41be6666
//   93 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function _Z18setMazdaMiataNAMafP19persistent_config_s
        THUMB
//   94 void setMazdaMiataNAMaf(persistent_config_s *e) {
_Z18setMazdaMiataNAMafP19persistent_config_s:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//   95   int i = 0;
        MOVS     R5,#+0
//   96   i= addMafPoint(e, i, 0.000000, 0.000000);
        VLDR.W   S1,??setMazdaMiataNAMaf_0  ;; 0x0
        VLDR.W   S0,??setMazdaMiataNAMaf_0  ;; 0x0
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//   97   i= addMafPoint(e, i, 0.000028, 0.100000);
        VLDR.W   S1,??setMazdaMiataNAMaf_0+0x4  ;; 0x3dcccccd
        VLDR.W   S0,??setMazdaMiataNAMaf_0+0x8  ;; 0x37eae18b
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//   98   i= addMafPoint(e, i, 0.000387, 0.200000);
        VLDR.W   S1,??setMazdaMiataNAMaf_0+0xC  ;; 0x3e4ccccd
        VLDR.W   S0,??setMazdaMiataNAMaf_0+0x10  ;; 0x39cae643
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//   99   i= addMafPoint(e, i, 0.001809, 0.300000);
        VLDR.W   S1,??setMazdaMiataNAMaf_0+0x14  ;; 0x3e99999a
        VLDR.W   S0,??setMazdaMiataNAMaf_0+0x18  ;; 0x3aed1bf8
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//  100   i= addMafPoint(e, i, 0.005409, 0.400000);
        VLDR.W   S1,??setMazdaMiataNAMaf_0+0x1C  ;; 0x3ecccccd
        VLDR.W   S0,??setMazdaMiataNAMaf_0+0x20  ;; 0x3bb13dfb
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//  101   i= addMafPoint(e, i, 0.012648, 0.500000);
        VMOV.F32 S1,#0.5
        VLDR.W   S0,??setMazdaMiataNAMaf_0+0x24  ;; 0x3c4f398f
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//  102   i= addMafPoint(e, i, 0.025320, 0.600000);
        VLDR.W   S1,??setMazdaMiataNAMaf_0+0x28  ;; 0x3f19999a
        VLDR.W   S0,??setMazdaMiataNAMaf_1  ;; 0x3ccf6be3
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//  103   i= addMafPoint(e, i, 0.045532, 0.700000);
        VLDR.W   S1,??setMazdaMiataNAMaf_1+0x4  ;; 0x3f333333
        VLDR.W   S0,??setMazdaMiataNAMaf_1+0x8  ;; 0x3d3a7fc3
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//  104   i= addMafPoint(e, i, 0.075698, 0.800000);
        VLDR.W   S1,??setMazdaMiataNAMaf_1+0xC  ;; 0x3f4ccccd
        VLDR.W   S0,??setMazdaMiataNAMaf_1+0x10  ;; 0x3d9b078e
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//  105   i= addMafPoint(e, i, 0.118525, 0.900000);
        VLDR.W   S1,??setMazdaMiataNAMaf_1+0x14  ;; 0x3f666666
        VLDR.W   S0,??setMazdaMiataNAMaf_1+0x18  ;; 0x3df2bd3c
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//  106   i= addMafPoint(e, i, 0.177011, 1.000000);
        VMOV.F32 S1,#1.0
        VLDR.W   S0,??setMazdaMiataNAMaf_1+0x1C  ;; 0x3e35425f
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//  107   i= addMafPoint(e, i, 0.254434, 1.100000);
        VLDR.W   S1,??setMazdaMiataNAMaf_1+0x20  ;; 0x3f8ccccd
        VLDR.W   S0,??setMazdaMiataNAMaf_1+0x24  ;; 0x3e82452c
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//  108   i= addMafPoint(e, i, 0.354347, 1.200000);
        VLDR.W   S1,??setMazdaMiataNAMaf_1+0x28  ;; 0x3f99999a
        VLDR.W   S0,??setMazdaMiataNAMaf_1+0x2C  ;; 0x3eb56cf8
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//  109   i= addMafPoint(e, i, 0.480576, 1.300000);
        VLDR.W   S1,??setMazdaMiataNAMaf_1+0x30  ;; 0x3fa66666
        VLDR.W   S0,??setMazdaMiataNAMaf_1+0x34  ;; 0x3ef60e0f
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//  110   i= addMafPoint(e, i, 0.637210, 1.400000);
        VLDR.W   S1,??setMazdaMiataNAMaf_1+0x38  ;; 0x3fb33333
        VLDR.W   S0,??setMazdaMiataNAMaf_1+0x3C  ;; 0x3f232032
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//  111   i= addMafPoint(e, i, 0.828604, 1.500000);
        VMOV.F32 S1,#1.5
        VLDR.W   S0,??setMazdaMiataNAMaf_1+0x40  ;; 0x3f541f64
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//  112   i= addMafPoint(e, i, 1.059369, 1.600000);
        VLDR.W   S1,??setMazdaMiataNAMaf_1+0x44  ;; 0x3fcccccd
        VLDR.W   S0,??setMazdaMiataNAMaf_1+0x48  ;; 0x3f879967
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//  113   i= addMafPoint(e, i, 1.334371, 1.700000);
        VLDR.W   S1,??setMazdaMiataNAMaf_1+0x4C  ;; 0x3fd9999a
        VLDR.W   S0,??setMazdaMiataNAMaf_1+0x50  ;; 0x3faaccab
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//  114   i= addMafPoint(e, i, 1.658728, 1.800000);
        VLDR.W   S1,??setMazdaMiataNAMaf_1+0x54  ;; 0x3fe66666
        VLDR.W   S0,??setMazdaMiataNAMaf_1+0x58  ;; 0x3fd45133
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//  115   i= addMafPoint(e, i, 2.037809, 1.900000);
        VLDR.W   S1,??setMazdaMiataNAMaf_1+0x5C  ;; 0x3ff33333
        VLDR.W   S0,??setMazdaMiataNAMaf_1+0x60  ;; 0x40026b76
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//  116   i= addMafPoint(e, i, 2.477225, 2.000000);
        VMOV.F32 S1,#2.0
        VLDR.W   S0,??DataTable6  ;; 0x401e8adb
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//  117   i= addMafPoint(e, i, 2.982835, 2.100000);
        VLDR.W   S1,??DataTable6_1  ;; 0x40066666
        VLDR.W   S0,??DataTable6_2  ;; 0x403ee6c5
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//  118   i= addMafPoint(e, i, 3.560736, 2.200000);
        VLDR.W   S1,??DataTable6_3  ;; 0x400ccccd
        VLDR.W   S0,??DataTable6_4  ;; 0x4063e319
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//  119   i= addMafPoint(e, i, 4.217265, 2.300000);
        VLDR.W   S1,??DataTable6_5  ;; 0x40133333
        VLDR.W   S0,??DataTable6_6  ;; 0x4086f3d6
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//  120   i= addMafPoint(e, i, 4.958995, 2.400000);
        VLDR.W   S1,??DataTable6_7  ;; 0x4019999a
        VLDR.W   S0,??DataTable6_8  ;; 0x409eb016
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//  121   i= addMafPoint(e, i, 5.792737, 2.500000);
        VMOV.F32 S1,#2.5
        VLDR.W   S0,??DataTable6_9  ;; 0x40b95e1a
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//  122   i= addMafPoint(e, i, 6.725531, 2.600000);
        VLDR.W   S1,??DataTable6_10  ;; 0x40266666
        VLDR.W   S0,??DataTable6_11  ;; 0x40d7378d
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//  123   i= addMafPoint(e, i, 7.764648, 2.700000);
        VLDR.W   S1,??DataTable6_12  ;; 0x402ccccd
        VLDR.W   S0,??DataTable6_13  ;; 0x40f877ff
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//  124   i= addMafPoint(e, i, 8.917592, 2.800000);
        VLDR.W   S1,??DataTable6_14  ;; 0x40333333
        VLDR.W   S0,??DataTable6_15  ;; 0x410eae75
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//  125   i= addMafPoint(e, i, 10.192092, 2.900000);
        VLDR.W   S1,??DataTable6_16  ;; 0x4039999a
        VLDR.W   S0,??DataTable6_17  ;; 0x412312cf
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//  126   i= addMafPoint(e, i, 11.596102, 3.000000);
        VMOV.F32 S1,#3.0
        VLDR.W   S0,??DataTable6_18  ;; 0x413989a2
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//  127   i= addMafPoint(e, i, 13.137802, 3.100000);
        VLDR.W   S1,??DataTable6_19  ;; 0x40466666
        VLDR.W   S0,??DataTable6_20  ;; 0x41523470
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//  128   i= addMafPoint(e, i, 14.825595, 3.200000);
        VLDR.W   S1,??DataTable6_21  ;; 0x404ccccd
        VLDR.W   S0,??DataTable6_22  ;; 0x416d35a3
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//  129   i= addMafPoint(e, i, 16.668105, 3.300000);
        VLDR.W   S1,??DataTable6_23  ;; 0x40533333
        VLDR.W   S0,??DataTable6_24  ;; 0x41855847
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//  130   i= addMafPoint(e, i, 18.674177, 3.400000);
        VLDR.W   S1,??DataTable6_25  ;; 0x4059999a
        VLDR.W   S0,??DataTable6_26  ;; 0x419564b7
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//  131   i= addMafPoint(e, i, 20.852874, 3.500000);
        VMOV.F32 S1,#3.5
        VLDR.W   S0,??DataTable6_27  ;; 0x41a6d2b0
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//  132   i= addMafPoint(e, i, 23.213478, 3.600000);
        VLDR.W   S1,??DataTable6_28  ;; 0x40666666
        VLDR.W   S0,??DataTable6_29  ;; 0x41b9b534
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//  133   i= addMafPoint(e, i, 25.765485, 3.700000);
        VLDR.W   S1,??DataTable6_30  ;; 0x406ccccd
        VLDR.W   S0,??DataTable6_31  ;; 0x41ce1fb7
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//  134   i= addMafPoint(e, i, 28.518610, 3.800000);
        VLDR.W   S1,??DataTable6_32  ;; 0x40733333
        VLDR.W   S0,??DataTable6_33  ;; 0x41e4261d
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//  135   i= addMafPoint(e, i, 31.482778, 3.900000);
        VLDR.W   S1,??DataTable6_34  ;; 0x4079999a
        VLDR.W   S0,??DataTable6_35  ;; 0x41fbdcbb
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//  136   i= addMafPoint(e, i, 34.668130, 4.000000);
        VMOV.F32 S1,#4.0
        VLDR.W   S0,??DataTable6_36  ;; 0x420aac2a
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//  137   i= addMafPoint(e, i, 38.085019, 4.100000);
        VLDR.W   S1,??DataTable6_37  ;; 0x40833333
        VLDR.W   S0,??DataTable6_38  ;; 0x4218570f
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//  138   i= addMafPoint(e, i, 41.744006, 4.200000);
        VLDR.W   S1,??DataTable6_39  ;; 0x40866666
        VLDR.W   S0,??DataTable6_40  ;; 0x4226f9dd
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//  139   i= addMafPoint(e, i, 45.655865, 4.300000);
        VLDR.W   S1,??DataTable6_41  ;; 0x4089999a
        VLDR.W   S0,??DataTable6_42  ;; 0x42369f9b
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//  140   i= addMafPoint(e, i, 49.831577, 4.400000);
        VLDR.W   S1,??DataTable6_43  ;; 0x408ccccd
        VLDR.W   S0,??DataTable6_44  ;; 0x42475389
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//  141   i= addMafPoint(e, i, 54.282334, 4.500000);
        VMOV.F32 S1,#4.5
        VLDR.W   S0,??DataTable6_45  ;; 0x4259211c
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//  142   i= addMafPoint(e, i, 59.019531, 4.600000);
        VLDR.W   S1,??DataTable6_46  ;; 0x40933333
        VLDR.W   S0,??DataTable6_47  ;; 0x426c1400
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//  143   i= addMafPoint(e, i, 64.054771, 4.700000);
        VLDR.W   S1,??DataTable6_48  ;; 0x40966666
        VLDR.W   S0,??DataTable6_49  ;; 0x42801c0b
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//  144   i= addMafPoint(e, i, 69.399864, 4.800000);
        VLDR.W   S1,??DataTable6_50  ;; 0x4099999a
        VLDR.W   S0,??DataTable6_51  ;; 0x428accbb
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//  145   i= addMafPoint(e, i, 75.066822, 4.900000);
        VLDR.W   S1,??DataTable6_52  ;; 0x409ccccd
        VLDR.W   S0,??DataTable6_53  ;; 0x42962236
        B.N      ??setMazdaMiataNAMaf_2
        Nop      
        DATA
??setMazdaMiataNAMaf_0:
        DC32     0x0
        DC32     0x3dcccccd
        DC32     0x37eae18b
        DC32     0x3e4ccccd
        DC32     0x39cae643
        DC32     0x3e99999a
        DC32     0x3aed1bf8
        DC32     0x3ecccccd
        DC32     0x3bb13dfb
        DC32     0x3c4f398f
        DC32     0x3f19999a
        THUMB
??setMazdaMiataNAMaf_2:
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//  146   i= addMafPoint(e, i, 81.067862, 5.000000);
        VMOV.F32 S1,#5.0
        VLDR.W   S0,??DataTable6_54  ;; 0x42a222bf
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11addMafPointP19persistent_config_siff
        BL       _Z11addMafPointP19persistent_config_siff
        MOVS     R5,R0
//  147   
//  148   fillTheRest(e, i);
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11fillTheRestP19persistent_config_si
        BL       _Z11fillTheRestP19persistent_config_si
//  149 }
        POP      {R0,R4,R5,PC}    ;; return
        Nop      
        DATA
??setMazdaMiataNAMaf_1:
        DC32     0x3ccf6be3
        DC32     0x3f333333
        DC32     0x3d3a7fc3
        DC32     0x3f4ccccd
        DC32     0x3d9b078e
        DC32     0x3f666666
        DC32     0x3df2bd3c
        DC32     0x3e35425f
        DC32     0x3f8ccccd
        DC32     0x3e82452c
        DC32     0x3f99999a
        DC32     0x3eb56cf8
        DC32     0x3fa66666
        DC32     0x3ef60e0f
        DC32     0x3fb33333
        DC32     0x3f232032
        DC32     0x3f541f64
        DC32     0x3fcccccd
        DC32     0x3f879967
        DC32     0x3fd9999a
        DC32     0x3faaccab
        DC32     0x3fe66666
        DC32     0x3fd45133
        DC32     0x3ff33333
        DC32     0x40026b76
          CFI EndBlock cfiBlock8

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6:
        DC32     0x401e8adb

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_1:
        DC32     0x40066666

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_2:
        DC32     0x403ee6c5

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_3:
        DC32     0x400ccccd

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_4:
        DC32     0x4063e319

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_5:
        DC32     0x40133333

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_6:
        DC32     0x4086f3d6

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_7:
        DC32     0x4019999a

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_8:
        DC32     0x409eb016

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_9:
        DC32     0x40b95e1a

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_10:
        DC32     0x40266666

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_11:
        DC32     0x40d7378d

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_12:
        DC32     0x402ccccd

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_13:
        DC32     0x40f877ff

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_14:
        DC32     0x40333333

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_15:
        DC32     0x410eae75

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_16:
        DC32     0x4039999a

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_17:
        DC32     0x412312cf

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_18:
        DC32     0x413989a2

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_19:
        DC32     0x40466666

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_20:
        DC32     0x41523470

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_21:
        DC32     0x404ccccd

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_22:
        DC32     0x416d35a3

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_23:
        DC32     0x40533333

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_24:
        DC32     0x41855847

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_25:
        DC32     0x4059999a

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_26:
        DC32     0x419564b7

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_27:
        DC32     0x41a6d2b0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_28:
        DC32     0x40666666

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_29:
        DC32     0x41b9b534

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_30:
        DC32     0x406ccccd

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_31:
        DC32     0x41ce1fb7

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_32:
        DC32     0x40733333

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_33:
        DC32     0x41e4261d

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_34:
        DC32     0x4079999a

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_35:
        DC32     0x41fbdcbb

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_36:
        DC32     0x420aac2a

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_37:
        DC32     0x40833333

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_38:
        DC32     0x4218570f

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_39:
        DC32     0x40866666

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_40:
        DC32     0x4226f9dd

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_41:
        DC32     0x4089999a

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_42:
        DC32     0x42369f9b

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_43:
        DC32     0x408ccccd

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_44:
        DC32     0x42475389

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_45:
        DC32     0x4259211c

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_46:
        DC32     0x40933333

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_47:
        DC32     0x426c1400

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_48:
        DC32     0x40966666

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_49:
        DC32     0x42801c0b

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_50:
        DC32     0x4099999a

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_51:
        DC32     0x428accbb

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_52:
        DC32     0x409ccccd

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_53:
        DC32     0x42962236

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_54:
        DC32     0x42a222bf

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_55:
        DC32     engineConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_56:
        DC32     engine

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_57:
        DC32     0x43480000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_58:
        DC32     0x41280000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_59:
        DC32     0x3fb18000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_60:
        DC32     0x42d2999a

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_61:
        DC32     0x403a4000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_62:
        DC32     0x43c1c000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_63:
        DC32     0x40884000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_64:
        DC32     0x44388000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_65:
        DC32     0x409f6000

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "maf"

        END
// 
//     4 bytes in section .rodata
// 1 980 bytes in section .text
// 
// 1 980 bytes of CODE  memory
//     4 bytes of CONST memory
//
//Errors: none
//Warnings: 1
