///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:08:43 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\development\sen /
//                       sor_chart.cpp                                        /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\development\sen /
//                       sor_chart.cpp -lCN F:\stuff\rusefi_sourceforge\firmw /
//                       are\iar\Debug\List\ -lA F:\stuff\rusefi_sourceforge\ /
//                       firmware\iar\Debug\List\ -o                          /
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
//                       sensor_chart.s                                       /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME sensor_chart

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1
        #define SHT_INIT_ARRAY 0xe

        EXTERN _Z10getFullLogv
        EXTERN _Z20getRevolutionCounterv
        EXTERN _ZN7LoggingC1EPKcPci
        EXTERN __aeabi_f2d
        EXTERN addConsoleActionI
        EXTERN appendPrintf
        EXTERN engineConfiguration
        EXTERN remainingSize
        EXTERN resetLogging
        EXTERN scheduleLogging

        PUBLIC _Z15initSensorChartv
        PUBLIC _Z9scAddDataff
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\development\sensor_chart.cpp
//    1 /**
//    2  * @file	analog_chart.cpp
//    3  *
//    4  * @date Dec 20, 2013
//    5  * @author Andrey Belomutskiy, (c) 2012-2015
//    6  */
//    7 
//    8 #include <sensor_chart.h>
//    9 #include "main.h"
//   10 #include "engine.h"
//   11 #include "rpm_calculator.h"
//   12 #include "status_loop.h"
//   13 
//   14 #if EFI_ANALOG_CHART || defined(__DOXYGEN__)
//   15 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   16 static char LOGGING_BUFFER[5000];
LOGGING_BUFFER:
        DS8 5000

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function __sti__routine
        THUMB
// static __intrinsic __interwork __softfp void __sti__routine()
__sti__routine:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   17 static Logging logging("analog chart", LOGGING_BUFFER, sizeof(LOGGING_BUFFER));
        MOVW     R3,#+5000
        LDR.N    R2,??DataTable3_1
        LDR.N    R1,??DataTable3_2
        LDR.N    R0,??DataTable3_3
          CFI FunCall _ZN7LoggingC1EPKcPci
        BL       _ZN7LoggingC1EPKcPci
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock0

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
logging:
        DS8 60
//   18 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   19 static int pendingData = FALSE;
pendingData:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   20 static int initialized = FALSE;
initialized:
        DS8 4
//   21 
//   22 extern engine_configuration_s *engineConfiguration;
//   23 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z9scAddDataff
        THUMB
//   24 void scAddData(float angle, float value) {
_Z9scAddDataff:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        SUB      SP,SP,#+8
          CFI CFA R13+24
        VMOV.F32 S16,S0
        VMOV.F32 S17,S1
//   25 	if (!initialized) {
        LDR.N    R0,??DataTable3_4
        LDR      R0,[R0, #+0]
        CMP      R0,#+0
        BEQ.N    ??scAddData_0
//   26 		return; // this is possible because of initialization sequence
//   27 	}
//   28 
//   29 	if (engineConfiguration->sensorChartFrequency < 2) {
??scAddData_1:
        LDR.N    R0,??DataTable3_5
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+532]
        CMP      R0,#+2
        BLT.N    ??scAddData_0
//   30 		/**
//   31 		 * analog chart frequency cannot be 1 because of the way
//   32 		 * data flush is implemented, see below
//   33 		 */
//   34 		//todofirmwareError()
//   35 		return;
//   36 	}
//   37 
//   38 	if (getRevolutionCounter() % engineConfiguration->sensorChartFrequency != 0) {
??scAddData_2:
          CFI FunCall _Z20getRevolutionCounterv
        BL       _Z20getRevolutionCounterv
        LDR.N    R1,??DataTable3_5
        LDR      R1,[R1, #+0]
        LDR      R1,[R1, #+532]
        SDIV     R2,R0,R1
        MLS      R0,R1,R2,R0
        CMP      R0,#+0
        BEQ.N    ??scAddData_3
//   39 		/**
//   40 		 * We are here if we do NOT need to add an event to the analog chart
//   41 		 */
//   42 		if (pendingData) {
        LDR.N    R0,??DataTable3_6
        LDR      R0,[R0, #+0]
        CMP      R0,#+0
        BEQ.N    ??scAddData_4
//   43 			/**
//   44 			 * We are here if that's the first time we do not need to add
//   45 			 * data after we have added some data - meaning it's time to flush
//   46 			 */
//   47 			// message terminator
//   48 			appendPrintf(&logging, DELIMETER);
        ADR.N    R1,??DataTable3  ;; ","
        LDR.N    R0,??DataTable3_3
          CFI FunCall appendPrintf
        BL       appendPrintf
//   49 			// output pending data
//   50 			if (getFullLog()) {
          CFI FunCall _Z10getFullLogv
        BL       _Z10getFullLogv
        CMP      R0,#+0
        BEQ.N    ??scAddData_5
//   51 				scheduleLogging(&logging);
        LDR.N    R0,??DataTable3_3
          CFI FunCall scheduleLogging
        BL       scheduleLogging
//   52 			}
//   53 			pendingData = false;
??scAddData_5:
        LDR.N    R0,??DataTable3_6
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//   54 		}
//   55 		return;
??scAddData_4:
        B.N      ??scAddData_0
//   56 	}
//   57 	if (!pendingData) {
??scAddData_3:
        LDR.N    R0,??DataTable3_6
        LDR      R0,[R0, #+0]
        CMP      R0,#+0
        BNE.N    ??scAddData_6
//   58 		pendingData = true;
        LDR.N    R0,??DataTable3_6
        MOVS     R1,#+1
        STR      R1,[R0, #+0]
//   59 		resetLogging(&logging);
        LDR.N    R0,??DataTable3_3
          CFI FunCall resetLogging
        BL       resetLogging
//   60 		// message header
//   61 		appendPrintf(&logging, "analog_chart%s", DELIMETER);
        ADR.N    R2,??DataTable3  ;; ","
        LDR.N    R1,??DataTable3_7
        LDR.N    R0,??DataTable3_3
          CFI FunCall appendPrintf
        BL       appendPrintf
//   62 	}
//   63 
//   64 	if (remainingSize(&logging) > 100) {
??scAddData_6:
        LDR.N    R0,??DataTable3_3
          CFI FunCall remainingSize
        BL       remainingSize
        CMP      R0,#+101
        BCC.N    ??scAddData_7
//   65 		appendPrintf(&logging, "%f|%f|", angle, value);
        VMOV     R0,S17
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R1,??DataTable3_8
        LDR.N    R0,??DataTable3_3
          CFI FunCall appendPrintf
        BL       appendPrintf
//   66 	}
//   67 }
??scAddData_7:
??scAddData_0:
        ADD      SP,SP,#+8
          CFI CFA R13+16
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock1
//   68 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z23setSensorChartFrequencyi
          CFI NoCalls
        THUMB
//   69 static void setSensorChartFrequency(int value) {
//   70 	engineConfiguration->sensorChartFrequency = value;
_Z23setSensorChartFrequencyi:
        LDR.N    R1,??DataTable3_5
        LDR      R1,[R1, #+0]
        STR      R0,[R1, #+532]
//   71 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock2
//   72 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z15initSensorChartv
        THUMB
//   73 void initSensorChart(void) {
_Z15initSensorChartv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   74 	addConsoleActionI("set_sensor_chart_freq", setSensorChartFrequency);
        LDR.N    R1,??DataTable3_9
        LDR.N    R0,??DataTable3_10
          CFI FunCall addConsoleActionI
        BL       addConsoleActionI
//   75 
//   76 	initialized = true;
        LDR.N    R0,??DataTable3_4
        MOVS     R1,#+1
        STR      R1,[R0, #+0]
//   77 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock3

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3:
        DC8      ",",0x0,0x0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_1:
        DC32     LOGGING_BUFFER

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_2:
        DC32     `?<Constant "analog chart">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_3:
        DC32     logging

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_4:
        DC32     initialized

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_5:
        DC32     engineConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_6:
        DC32     pendingData

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_7:
        DC32     `?<Constant "analog_chart%s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_8:
        DC32     `?<Constant "%f|%f|">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_9:
        DC32     _Z23setSensorChartFrequencyi

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_10:
        DC32     `?<Constant "set_sensor_chart_freq">`

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

        SECTION `.rodata`:CONST:REORDER:NOROOT(1)
        DATA
        DC8 ","

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "analog_chart%s">`:
        DATA
        DC8 "analog_chart%s"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "%f|%f|">`:
        DATA
        DC8 "%f|%f|"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_sensor_chart_freq">`:
        DATA
        DC8 "set_sensor_chart_freq"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "analog chart">`:
        DATA
        DC8 "analog chart"
        DC8 0, 0, 0

        END
//   78 
//   79 #endif /* EFI_ANALOG_CHART */
// 
// 5 068 bytes in section .bss
//     4 bytes in section .init_array
//    66 bytes in section .rodata
//   268 bytes in section .text
// 
//   272 bytes of CODE  memory
//    66 bytes of CONST memory
// 5 068 bytes of DATA  memory
//
//Errors: none
//Warnings: 1
