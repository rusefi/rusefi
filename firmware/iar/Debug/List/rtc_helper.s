///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:33 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\hw_layer\rtc_he /
//                       lper.cpp                                             /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\hw_layer\rtc_he /
//                       lper.cpp -lCN F:\stuff\rusefi_sourceforge\firmware\i /
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
//                       rtc_helper.s                                         /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME rtc_helper

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1
        #define SHT_INIT_ARRAY 0xe

        EXTERN RTCD1
        EXTERN _ZN18LoggingWithStorageC1EPKc
        EXTERN __aeabi_d2uiz
        EXTERN __aeabi_f2d
        EXTERN appendMsgPostfix
        EXTERN appendMsgPrefix
        EXTERN appendPrintf
        EXTERN atoff
        EXTERN itoa10
        EXTERN printMsg
        EXTERN rtcGetTimeTm
        EXTERN rtcGetTimeUnixSec
        EXTERN rtcSetTimeTm
        EXTERN rtcSetTimeUnixSec
        EXTERN scheduleLogging
        EXTERN scheduleMsg
        EXTERN strcpy
        EXTERN strlen

        PUBLIC _Z11date_get_tmP2tm
        PUBLIC _Z11date_set_tmP2tm
        PUBLIC _Z11setDateTimePKc
        PUBLIC _Z12dateToStringPc
        PUBLIC _Z13printDateTimev
        PUBLIC _Z7initRtcv
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\hw_layer\rtc_helper.cpp
//    1 /**
//    2  * @file rtc_helper.cpp
//    3  * @brief Real Time Clock helper
//    4  *
//    5  * @date Feb 5, 2014
//    6  * @author Andrey Belomutskiy, (c) 2012-2015
//    7  */
//    8 
//    9 #include <string.h> 
//   10 #include <time.h>
//   11 #include "main.h"
//   12 #include "rfiutil.h"
//   13 #include "chrtclib.h"
//   14 #include "rtc_helper.h"
//   15 
//   16 #if EFI_RTC || defined(__DOXYGEN__)

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function __sti__routine
        THUMB
// static __intrinsic __interwork __softfp void __sti__routine()
__sti__routine:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   17 static LoggingWithStorage logger("RTC");
        ADR.N    R1,??DataTable6  ;; "RTC"
        LDR.N    R0,??DataTable6_1
          CFI FunCall _ZN18LoggingWithStorageC1EPKc
        BL       _ZN18LoggingWithStorageC1EPKc
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock0

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
logger:
        DS8 260
//   18 
//   19 #endif /* EFI_RTC */
//   20 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z11date_set_tmP2tm
        THUMB
//   21 void date_set_tm(struct tm *timp) {
_Z11date_set_tmP2tm:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   22 	(void)timp;
//   23 #if EFI_RTC || defined(__DOXYGEN__)
//   24 	rtcSetTimeTm(&RTCD1, timp);
        MOVS     R1,R4
        LDR.N    R0,??DataTable6_2
          CFI FunCall rtcSetTimeTm
        BL       rtcSetTimeTm
//   25 #endif /* EFI_RTC */
//   26 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock1
//   27 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z11date_get_tmP2tm
        THUMB
//   28 void date_get_tm(struct tm *timp) {
_Z11date_get_tmP2tm:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   29 	(void)timp;
//   30 #if EFI_RTC || defined(__DOXYGEN__)
//   31 	rtcGetTimeTm(&RTCD1, timp);
        MOVS     R1,R4
        LDR.N    R0,??DataTable6_2
          CFI FunCall rtcGetTimeTm
        BL       rtcGetTimeTm
//   32 #endif /* EFI_RTC */
//   33 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock2
//   34 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z12dateToStringPc
        THUMB
//   35 void dateToString(char *lcd_str) {
_Z12dateToStringPc:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        SUB      SP,SP,#+48
          CFI CFA R13+56
        MOVS     R4,R0
//   36 #if EFI_RTC || defined(__DOXYGEN__)
//   37 	// todo:
//   38 	// re-implement this along the lines of 	chvprintf("%04u-%02u-%02u %02u:%02u:%02u\r\n", timp.tm_year + 1900, timp.tm_mon + 1, timp.tm_mday, timp.tm_hour,
//   39 	// timp.tm_min, timp.tm_sec);
//   40 	// this would require a temporary mem stream - see datalogging and other existing usages
//   41 
//   42 	strcpy(lcd_str, "00/00 00:00:00\0");
        LDR.N    R1,??DataTable6_3
        MOVS     R0,R4
          CFI FunCall strcpy
        BL       strcpy
//   43 	static char buff[4];
//   44 	struct tm timp;
//   45 	rtcGetTimeTm(&RTCD1, &timp);			// get RTC date/time
        ADD      R1,SP,#+0
        LDR.N    R0,??DataTable6_2
          CFI FunCall rtcGetTimeTm
        BL       rtcGetTimeTm
//   46 	
//   47 	itoa10(buff, timp.tm_mon + 1);
        LDR      R0,[SP, #+16]
        ADDS     R1,R0,#+1
        LDR.N    R0,??DataTable6_4
          CFI FunCall itoa10
        BL       itoa10
//   48 	if(timp.tm_mon < 9) { 
        LDR      R0,[SP, #+16]
        CMP      R0,#+9
        BGE.N    ??dateToString_0
//   49 		lcd_str[0] = '0';
        MOVS     R0,#+48
        STRB     R0,[R4, #+0]
//   50 		lcd_str[1] = buff[0];
        LDR.N    R0,??DataTable6_4
        LDRB     R0,[R0, #+0]
        STRB     R0,[R4, #+1]
        B.N      ??dateToString_1
//   51 	} else {
//   52 		lcd_str[0] = buff[0];
??dateToString_0:
        LDR.N    R0,??DataTable6_4
        LDRB     R0,[R0, #+0]
        STRB     R0,[R4, #+0]
//   53 		lcd_str[1] = buff[1];
        LDR.N    R0,??DataTable6_4
        LDRB     R0,[R0, #+1]
        STRB     R0,[R4, #+1]
//   54 	}
//   55 	itoa10(buff, timp.tm_mday);
??dateToString_1:
        LDR      R1,[SP, #+12]
        LDR.N    R0,??DataTable6_4
          CFI FunCall itoa10
        BL       itoa10
//   56 	if(timp.tm_mday < 10) { 
        LDR      R0,[SP, #+12]
        CMP      R0,#+10
        BGE.N    ??dateToString_2
//   57 		lcd_str[3] = '0';
        MOVS     R0,#+48
        STRB     R0,[R4, #+3]
//   58 		lcd_str[4] = buff[0];
        LDR.N    R0,??DataTable6_4
        LDRB     R0,[R0, #+0]
        STRB     R0,[R4, #+4]
        B.N      ??dateToString_3
//   59 	} else {
//   60 		lcd_str[3] = buff[0];
??dateToString_2:
        LDR.N    R0,??DataTable6_4
        LDRB     R0,[R0, #+0]
        STRB     R0,[R4, #+3]
//   61 		lcd_str[4] = buff[1];
        LDR.N    R0,??DataTable6_4
        LDRB     R0,[R0, #+1]
        STRB     R0,[R4, #+4]
//   62 	}
//   63 	itoa10(buff, timp.tm_hour);
??dateToString_3:
        LDR      R1,[SP, #+8]
        LDR.N    R0,??DataTable6_4
          CFI FunCall itoa10
        BL       itoa10
//   64 	if(timp.tm_hour < 10) { 
        LDR      R0,[SP, #+8]
        CMP      R0,#+10
        BGE.N    ??dateToString_4
//   65 		lcd_str[6] = '0';
        MOVS     R0,#+48
        STRB     R0,[R4, #+6]
//   66 		lcd_str[7] = buff[0];
        LDR.N    R0,??DataTable6_4
        LDRB     R0,[R0, #+0]
        STRB     R0,[R4, #+7]
        B.N      ??dateToString_5
//   67 	} else {
//   68 		lcd_str[6] = buff[0];
??dateToString_4:
        LDR.N    R0,??DataTable6_4
        LDRB     R0,[R0, #+0]
        STRB     R0,[R4, #+6]
//   69 		lcd_str[7] = buff[1];
        LDR.N    R0,??DataTable6_4
        LDRB     R0,[R0, #+1]
        STRB     R0,[R4, #+7]
//   70 	}
//   71 	itoa10(buff, timp.tm_min);
??dateToString_5:
        LDR      R1,[SP, #+4]
        LDR.N    R0,??DataTable6_4
          CFI FunCall itoa10
        BL       itoa10
//   72 	if(timp.tm_min < 10) { 
        LDR      R0,[SP, #+4]
        CMP      R0,#+10
        BGE.N    ??dateToString_6
//   73 		lcd_str[9] = '0';
        MOVS     R0,#+48
        STRB     R0,[R4, #+9]
//   74 		lcd_str[10] = buff[0];
        LDR.N    R0,??DataTable6_4
        LDRB     R0,[R0, #+0]
        STRB     R0,[R4, #+10]
        B.N      ??dateToString_7
//   75 	} else {
//   76 		lcd_str[9] = buff[0];
??dateToString_6:
        LDR.N    R0,??DataTable6_4
        LDRB     R0,[R0, #+0]
        STRB     R0,[R4, #+9]
//   77 		lcd_str[10] = buff[1];
        LDR.N    R0,??DataTable6_4
        LDRB     R0,[R0, #+1]
        STRB     R0,[R4, #+10]
//   78 	}
//   79 	itoa10(buff, timp.tm_sec);
??dateToString_7:
        LDR      R1,[SP, #+0]
        LDR.N    R0,??DataTable6_4
          CFI FunCall itoa10
        BL       itoa10
//   80 	if(timp.tm_sec < 10) { 
        LDR      R0,[SP, #+0]
        CMP      R0,#+10
        BGE.N    ??dateToString_8
//   81 		lcd_str[12] = '0';
        MOVS     R0,#+48
        STRB     R0,[R4, #+12]
//   82 		lcd_str[13] = buff[0];
        LDR.N    R0,??DataTable6_4
        LDRB     R0,[R0, #+0]
        STRB     R0,[R4, #+13]
        B.N      ??dateToString_9
//   83 	} else {
//   84 		lcd_str[12] = buff[0];
??dateToString_8:
        LDR.N    R0,??DataTable6_4
        LDRB     R0,[R0, #+0]
        STRB     R0,[R4, #+12]
//   85 		lcd_str[13] = buff[1];
        LDR.N    R0,??DataTable6_4
        LDRB     R0,[R0, #+1]
        STRB     R0,[R4, #+13]
//   86 	}
//   87 #else
//   88 	lcd_str[0] = 0;
//   89 #endif /* EFI_RTC */
//   90 }
??dateToString_9:
        ADD      SP,SP,#+48
          CFI CFA R13+8
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock3

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
??buff:
        DS8 4
//   91 
//   92 #if EFI_RTC || defined(__DOXYGEN__)

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _Z13printDateTimev
        THUMB
//   93 void printDateTime(void) {
_Z13printDateTimev:
        PUSH     {LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+4
        SUB      SP,SP,#+60
          CFI CFA R13+64
//   94 	static time_t unix_time;
//   95 	struct tm timp;
//   96 	
//   97 	unix_time = rtcGetTimeUnixSec(&RTCD1);
        LDR.N    R0,??DataTable6_2
          CFI FunCall rtcGetTimeUnixSec
        BL       rtcGetTimeUnixSec
        LDR.N    R1,??DataTable6_5
        STR      R0,[R1, #+0]
//   98 	if (unix_time == -1) {
        LDR.N    R0,??DataTable6_5
        LDR      R0,[R0, #+0]
        CMN      R0,#+1
        BNE.N    ??printDateTime_0
//   99 		scheduleMsg(&logger, "incorrect time in RTC cell");
        LDR.N    R1,??DataTable6_6
        LDR.N    R0,??DataTable6_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
        B.N      ??printDateTime_1
//  100 	} else {
//  101 		scheduleMsg(&logger, "%D - unix time", unix_time);
??printDateTime_0:
        LDR.N    R0,??DataTable6_5
        LDR      R2,[R0, #+0]
        LDR.N    R1,??DataTable6_7
        LDR.N    R0,??DataTable6_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  102 		rtcGetTimeTm(&RTCD1, &timp);
        ADD      R1,SP,#+16
        LDR.N    R0,??DataTable6_2
          CFI FunCall rtcGetTimeTm
        BL       rtcGetTimeTm
//  103 
//  104 		appendMsgPrefix(&logger);
        LDR.N    R0,??DataTable6_1
          CFI FunCall appendMsgPrefix
        BL       appendMsgPrefix
//  105 		appendPrintf(&logger, "Current RTC time in GMT is: %04u-%02u-%02u %02u:%02u:%02u", timp.tm_year + 1900, timp.tm_mon + 1, timp.tm_mday, timp.tm_hour,
//  106 				timp.tm_min, timp.tm_sec);
        LDR      R0,[SP, #+16]
        STR      R0,[SP, #+12]
        LDR      R0,[SP, #+20]
        STR      R0,[SP, #+8]
        LDR      R0,[SP, #+24]
        STR      R0,[SP, #+4]
        LDR      R0,[SP, #+28]
        STR      R0,[SP, #+0]
        LDR      R0,[SP, #+32]
        ADDS     R3,R0,#+1
        LDR      R0,[SP, #+36]
        ADDW     R2,R0,#+1900
        LDR.N    R1,??DataTable6_8
        LDR.N    R0,??DataTable6_1
          CFI FunCall appendPrintf
        BL       appendPrintf
//  107 		appendMsgPostfix(&logger);
        LDR.N    R0,??DataTable6_1
          CFI FunCall appendMsgPostfix
        BL       appendMsgPostfix
//  108 		scheduleLogging(&logger);
        LDR.N    R0,??DataTable6_1
          CFI FunCall scheduleLogging
        BL       scheduleLogging
//  109 	}
//  110 }
??printDateTime_1:
        ADD      SP,SP,#+60
          CFI CFA R13+4
        POP      {PC}             ;; return
          CFI EndBlock cfiBlock4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
??unix_time:
        DS8 4
//  111 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _Z11setDateTimePKc
        THUMB
//  112 void setDateTime(const char *strDate) {
_Z11setDateTimePKc:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  113 	if (strlen(strDate) > 0) {
        MOVS     R0,R4
          CFI FunCall strlen
        BL       strlen
        CMP      R0,#+0
        BEQ.N    ??setDateTime_0
//  114 		time_t unix_time = (double) atoff(strDate);
        MOVS     R0,R4
          CFI FunCall atoff
        BL       atoff
        VMOV     R0,S0
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
          CFI FunCall __aeabi_d2uiz
        BL       __aeabi_d2uiz
        MOVS     R5,R0
//  115 		if (unix_time > 0) {
        CMP      R5,#+0
        BEQ.N    ??setDateTime_0
//  116 			rtcSetTimeUnixSec(&RTCD1, unix_time);
        MOVS     R1,R5
        LDR.N    R0,??DataTable6_2
          CFI FunCall rtcSetTimeUnixSec
        BL       rtcSetTimeUnixSec
//  117 			printDateTime();
          CFI FunCall _Z13printDateTimev
        BL       _Z13printDateTimev
//  118 			return;
        B.N      ??setDateTime_1
//  119 		}
//  120 	}
//  121 	scheduleMsg(&logger, "date_set Date parameter %s is wrong\r\n", strDate);
??setDateTime_0:
        MOVS     R2,R4
        LDR.N    R1,??DataTable6_9
        LDR.N    R0,??DataTable6_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  122 }
??setDateTime_1:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock5
//  123 #endif /* EFI_RTC */
//  124 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _Z7initRtcv
        THUMB
//  125 void initRtc(void) {
_Z7initRtcv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  126 #if EFI_RTC || defined(__DOXYGEN__)
//  127 	printMsg(&logger, "initRtc()");
        LDR.N    R1,??DataTable6_10
        LDR.N    R0,??DataTable6_1
          CFI FunCall printMsg
        BL       printMsg
//  128 
//  129 	// yes, it's my begin time  and we always start from this one 1391894433 - 2014-02-08 21:20:03
//  130 //	rtcSetTimeUnixSec(&RTCD1, 1391894433);
//  131 #endif
//  132 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock6

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6:
        DC8      "RTC"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_1:
        DC32     logger

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_2:
        DC32     RTCD1

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_3:
        DC32     `?<Constant "00/00 00:00:00\\000">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_4:
        DC32     ??buff

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_5:
        DC32     ??unix_time

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_6:
        DC32     `?<Constant "incorrect time in RTC...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_7:
        DC32     `?<Constant "%D - unix time">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_8:
        DC32     `?<Constant "Current RTC time in G...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_9:
        DC32     `?<Constant "date_set Date paramet...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_10:
        DC32     `?<Constant "initRtc()">`

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
`?<Constant "00/00 00:00:00\\000">`:
        DATA
        DC8 30H, 30H, 2FH, 30H, 30H, 20H, 30H, 30H
        DC8 3AH, 30H, 30H, 3AH, 30H, 30H, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "incorrect time in RTC...">`:
        DATA
        DC8 "incorrect time in RTC cell"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "%D - unix time">`:
        DATA
        DC8 "%D - unix time"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Current RTC time in G...">`:
        DATA
        DC8 43H, 75H, 72H, 72H, 65H, 6EH, 74H, 20H
        DC8 52H, 54H, 43H, 20H, 74H, 69H, 6DH, 65H
        DC8 20H, 69H, 6EH, 20H, 47H, 4DH, 54H, 20H
        DC8 69H, 73H, 3AH, 20H, 25H, 30H, 34H, 75H
        DC8 2DH, 25H, 30H, 32H, 75H, 2DH, 25H, 30H
        DC8 32H, 75H, 20H, 25H, 30H, 32H, 75H, 3AH
        DC8 25H, 30H, 32H, 75H, 3AH, 25H, 30H, 32H
        DC8 75H, 0
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "date_set Date paramet...">`:
        DATA
        DC8 "date_set Date parameter %s is wrong\015\012"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "initRtc()">`:
        DATA
        DC8 "initRtc()"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "RTC"

        END
// 
// 268 bytes in section .bss
//   4 bytes in section .init_array
// 176 bytes in section .rodata
// 488 bytes in section .text
// 
// 492 bytes of CODE  memory
// 176 bytes of CONST memory
// 268 bytes of DATA  memory
//
//Errors: none
//Warnings: 1
