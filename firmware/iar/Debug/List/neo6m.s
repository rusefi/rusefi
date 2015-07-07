///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:55 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\hw_layer\neo6m. /
//                       cpp                                                  /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\hw_layer\neo6m. /
//                       cpp -lCN F:\stuff\rusefi_sourceforge\firmware\iar\De /
//                       bug\List\ -lA F:\stuff\rusefi_sourceforge\firmware\i /
//                       ar\Debug\List\ -o F:\stuff\rusefi_sourceforge\firmwa /
//                       re\iar\Debug\Obj\ --no_cse --no_unroll --no_inline   /
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
//                       neo6m.s                                              /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME neo6m

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1
        #define SHT_INIT_ARRAY 0xe

        EXTERN SD1
        EXTERN _Z10hwPortname11brain_pin_e
        EXTERN _Z11date_get_tmP2tm
        EXTERN _Z11date_set_tmP2tm
        EXTERN _Z13mySetPadMode2PKc11brain_pin_ej
        EXTERN _ZN18LoggingWithStorageC1EPKc
        EXTERN __aeabi_d2f
        EXTERN __aeabi_ddiv
        EXTERN __aeabi_f2d
        EXTERN __aeabi_memset
        EXTERN __aeabi_ui2d
        EXTERN addConsoleAction
        EXTERN boardConfiguration
        EXTERN chThdCreateStatic
        EXTERN currentTimeMillis
        EXTERN engineConfiguration
        EXTERN gps_location
        EXTERN print
        EXTERN rlist
        EXTERN scheduleMsg
        EXTERN sdStart

        PUBLIC getCurrentSpeed
        PUBLIC initGps
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
        
// F:\stuff\rusefi_sourceforge\firmware\hw_layer\neo6m.cpp
//    1 /**
//    2  * @file	neo6m.cpp
//    3  * @brief	Ublox 6M hardware UART driver
//    4  *
//    5  * http://www.u-blox.com/en/gps-modules/pvt-modules/previous-generations/neo-6-family.html
//    6  *
//    7  * Technically any UART GPS should work with this driver since NMEA protocol is pretty common anyway
//    8  *
//    9  * @date Dec 28, 2013
//   10  * @author Andrey Belomutskiy, (c) 2012-2015
//   11  * Kot_dnz 2014
//   12  */
//   13 
//   14 #include <string.h>
//   15 // todo: MISRA does not like time.h
//   16 #include <time.h>
//   17 #include "main.h"
//   18 
//   19 #if EFI_UART_GPS || defined(__DOXYGEN__)
//   20 
//   21 #include "console_io.h"
//   22 #include "eficonsole.h"
//   23 #include "pin_repository.h"
//   24 #include "nmea.h"
//   25 #include "neo6m.h"
//   26 #include "rtc_helper.h"
//   27 #include "engine_configuration.h"
//   28 
//   29 extern board_configuration_s *boardConfiguration;
//   30 extern engine_configuration_s *engineConfiguration;
//   31 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function __sti__routine
        THUMB
// static __intrinsic __interwork __softfp void __sti__routine()
__sti__routine:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   32 static LoggingWithStorage logging("uart gps");
        LDR.N    R1,??DataTable5
        LDR.N    R0,??DataTable5_1
          CFI FunCall _ZN18LoggingWithStorageC1EPKc
        BL       _ZN18LoggingWithStorageC1EPKc
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock0

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
logging:
        DS8 260
//   33 

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//   34 static SerialConfig GPSserialConfig = { GPS_SERIAL_SPEED, 0, USART_CR2_STOP1_BITS | USART_CR2_LINEN, 0 };
GPSserialConfig:
        DATA
        DC32 38400
        DC16 0, 16384, 0
        DC8 0, 0

        SECTION `.bss`:DATA:REORDER:NOROOT(3)
//   35 static THD_WORKING_AREA(gpsThreadStack, UTILITY_THREAD_STACK_SIZE);
gpsThreadStack:
        DS8 712
//   36 
//   37 // this field holds our current state

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   38 static loc_t GPSdata;
GPSdata:
        DS8 80
//   39 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   40 static int gpsMesagesCount = 0;
gpsMesagesCount:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   41 static int uartErrors = 0;
uartErrors:
        DS8 4
//   42 
//   43 // todo: some data structure for coordinates location
//   44 // todo:

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function getCurrentSpeed
          CFI NoCalls
        THUMB
//   45 float getCurrentSpeed(void) {
//   46 	return GPSdata.speed;
getCurrentSpeed:
        LDR.N    R0,??DataTable5_2
        VLDR     S0,[R0, #+8]
        BX       LR               ;; return
          CFI EndBlock cfiBlock1
//   47 }
//   48 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z12printGpsInfov
        THUMB
//   49 static void printGpsInfo(void) {
_Z12printGpsInfov:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        SUB      SP,SP,#+8
          CFI CFA R13+24
//   50 	scheduleMsg(&logging, "GPS RX %s", hwPortname(boardConfiguration->gps_rx_pin));
        LDR.N    R0,??DataTable5_3
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+200]
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R2,R0
        LDR.N    R1,??DataTable5_4
        LDR.N    R0,??DataTable5_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   51 	scheduleMsg(&logging, "GPS TX %s", hwPortname(boardConfiguration->gps_tx_pin));
        LDR.N    R0,??DataTable5_3
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+204]
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R2,R0
        LDR.N    R1,??DataTable5_5
        LDR.N    R0,??DataTable5_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   52 
//   53 	scheduleMsg(&logging, "m=%d,e=%d: vehicle speed = %f\r\n", gpsMesagesCount, uartErrors, getCurrentSpeed());
          CFI FunCall getCurrentSpeed
        BL       getCurrentSpeed
        VMOV     R0,S0
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        LDR.N    R0,??DataTable5_6
        LDR      R3,[R0, #+0]
        LDR.N    R0,??DataTable5_7
        LDR      R2,[R0, #+0]
        LDR.N    R1,??DataTable5_8
        LDR.N    R0,??DataTable5_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   54 
//   55 	float sec = currentTimeMillis() / 1000.0;
          CFI FunCall currentTimeMillis
        BL       currentTimeMillis
          CFI FunCall __aeabi_ui2d
        BL       __aeabi_ui2d
        MOVS     R2,#+0
        LDR.N    R3,??DataTable5_9  ;; 0x408f4000
          CFI FunCall __aeabi_ddiv
        BL       __aeabi_ddiv
          CFI FunCall __aeabi_d2f
        BL       __aeabi_d2f
        VMOV     S16,R0
//   56 	scheduleMsg(&logging, "communication speed: %f", gpsMesagesCount / sec);
        LDR.N    R0,??DataTable5_7
        VLDR     S0,[R0, #0]
        VCVT.F32.S32 S0,S0
        VDIV.F32 S0,S0,S16
        VMOV     R0,S0
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R1,??DataTable5_10
        LDR.N    R0,??DataTable5_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   57 
//   58 	print("GPS latitude = %f\r\n", GPSdata.latitude);
        LDR.N    R0,??DataTable5_2
        LDR      R0,[R0, #+0]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R0,??DataTable5_11
          CFI FunCall print
        BL       print
//   59 	print("GPS longitude = %f\r\n", GPSdata.longitude);
        LDR.N    R0,??DataTable5_2
        LDR      R0,[R0, #+4]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R0,??DataTable5_12
          CFI FunCall print
        BL       print
//   60 }
        ADD      SP,SP,#+8
          CFI CFA R13+16
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock2
//   61 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   62 static struct tm curTm;
curTm:
        DS8 44
//   63 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z12onGpsMessagePc
        THUMB
//   64 static void onGpsMessage(char *buffer) {
_Z12onGpsMessagePc:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   65 
//   66 	gps_location(&GPSdata, buffer);
        MOVS     R1,R4
        LDR.N    R0,??DataTable5_2
          CFI FunCall gps_location
        BL       gps_location
//   67 	date_get_tm(&curTm);
        LDR.N    R0,??DataTable5_13
          CFI FunCall _Z11date_get_tmP2tm
        BL       _Z11date_get_tmP2tm
//   68 
//   69 	if (GPSdata.quality == 4 && GPSdata.GPStm.tm_year > 0 && GPSdata.GPStm.tm_sec != curTm.tm_sec) {
        LDR.N    R0,??DataTable5_2
        LDR      R0,[R0, #+68]
        CMP      R0,#+4
        BNE.N    ??onGpsMessage_0
        LDR.N    R0,??DataTable5_2
        LDR      R0,[R0, #+40]
        CMP      R0,#+1
        BLT.N    ??onGpsMessage_0
        LDR.N    R0,??DataTable5_2
        LDR      R0,[R0, #+20]
        LDR.N    R1,??DataTable5_13
        LDR      R1,[R1, #+0]
        CMP      R0,R1
        BEQ.N    ??onGpsMessage_0
//   70 		// quality =4 (valis GxRMC), year > 0, and difference more then second
//   71 		date_set_tm(&GPSdata.GPStm);					// set GPS time
        LDR.N    R0,??DataTable5_14
          CFI FunCall _Z11date_set_tmP2tm
        BL       _Z11date_set_tmP2tm
//   72 		//}
//   73 	}
//   74 	gpsMesagesCount++;
??onGpsMessage_0:
        LDR.N    R0,??DataTable5_7
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+1
        LDR.N    R1,??DataTable5_7
        STR      R0,[R1, #+0]
//   75 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock3
//   76 
//   77 // we do not want this on stack, right?

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   78 static char gps_str[GPS_MAX_STRING];
gps_str:
        DS8 256
//   79 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _Z19GpsThreadEntryPointPv
        THUMB
//   80 static msg_t GpsThreadEntryPoint(void *arg) {
_Z19GpsThreadEntryPointPv:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
//   81 	(void) arg;
//   82 	chRegSetThreadName("GPS thread");
        LDR.N    R0,??DataTable5_15
        LDR      R0,[R0, #+24]
        LDR.N    R1,??DataTable5_16
        STR      R1,[R0, #+24]
//   83 
//   84 	int count = 0;
        MOVS     R5,#+0
//   85 
//   86 	while (true) {
//   87 		msg_t charbuf = chSequentialStreamGet(GPS_SERIAL_DEVICE);
??GpsThreadEntryPoint_0:
        LDR.N    R0,??DataTable5_17
        LDR.N    R1,??DataTable5_17
        LDR      R1,[R1, #+0]
        LDR      R1,[R1, #+12]
          CFI FunCall
        BLX      R1
        MOVS     R6,R0
//   88 		if (charbuf == 10 || count == GPS_MAX_STRING) {					// if 0xD,0xA or limit
        CMP      R6,#+10
        BEQ.N    ??GpsThreadEntryPoint_1
        CMP      R5,#+256
        BNE.N    ??GpsThreadEntryPoint_2
//   89 			if (count >= 1)
??GpsThreadEntryPoint_1:
        CMP      R5,#+1
        BLT.N    ??GpsThreadEntryPoint_3
//   90 				gps_str[--count] = '\0';					// delete 0xD
        SUBS     R5,R5,#+1
        LDR.N    R0,??DataTable5_18
        MOVS     R1,#+0
        STRB     R1,[R5, R0]
//   91 
//   92 //			scheduleMsg(&logger, "got GPS [%s]", gps_str);
//   93 
//   94 			// 'gps_str' string completed
//   95 			onGpsMessage(gps_str);
??GpsThreadEntryPoint_3:
        LDR.N    R0,??DataTable5_18
          CFI FunCall _Z12onGpsMessagePc
        BL       _Z12onGpsMessagePc
//   96 			memset(&gps_str, '\0', GPS_MAX_STRING);			// clear buffer
        MOV      R2,#+256
        MOVS     R1,#+0
        LDR.N    R0,??DataTable5_18
          CFI FunCall memset
        BL       memset
//   97 			count = 0;
        MOVS     R0,#+0
        MOVS     R5,R0
        B.N      ??GpsThreadEntryPoint_0
//   98 		} else {
//   99 			gps_str[count++] = charbuf;
??GpsThreadEntryPoint_2:
        LDR.N    R0,??DataTable5_18
        STRB     R6,[R5, R0]
        ADDS     R5,R5,#+1
        B.N      ??GpsThreadEntryPoint_0
          CFI EndBlock cfiBlock4
//  100 		}
//  101 	}
//  102 #if defined __GNUC__
//  103 	return 0;
//  104 #endif
//  105 }

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function initGps
        THUMB
initGps:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        LDR.N    R0,??DataTable5_19
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1497]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+4
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??initGps_0
??initGps_1:
        LDR.N    R1,??DataTable5_20
        LDR.N    R0,??DataTable5_17
          CFI FunCall sdStart
        BL       sdStart
        MOVW     R2,#+898
        LDR.N    R0,??DataTable5_3
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+204]
        LDR.N    R0,??DataTable5_21
          CFI FunCall _Z13mySetPadMode2PKc11brain_pin_ej
        BL       _Z13mySetPadMode2PKc11brain_pin_ej
        MOVW     R2,#+898
        LDR.N    R0,??DataTable5_3
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+200]
        LDR.N    R0,??DataTable5_22
          CFI FunCall _Z13mySetPadMode2PKc11brain_pin_ej
        BL       _Z13mySetPadMode2PKc11brain_pin_ej
        MOVS     R0,#+0
        STR      R0,[SP, #+0]
        LDR.N    R3,??DataTable5_23
        MOVS     R2,#+2
        MOV      R1,#+712
        LDR.N    R0,??DataTable5_24
          CFI FunCall chThdCreateStatic
        BL       chThdCreateStatic
        LDR.N    R1,??DataTable5_25
        LDR.N    R0,??DataTable5_26
          CFI FunCall addConsoleAction
        BL       addConsoleAction
??initGps_0:
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock5

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5:
        DC32     `?<Constant "uart gps">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_1:
        DC32     logging

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_2:
        DC32     GPSdata

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_3:
        DC32     boardConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_4:
        DC32     `?<Constant "GPS RX %s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_5:
        DC32     `?<Constant "GPS TX %s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_6:
        DC32     uartErrors

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_7:
        DC32     gpsMesagesCount

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_8:
        DC32     `?<Constant "m=%d,e=%d: vehicle sp...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_9:
        DC32     0x408f4000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_10:
        DC32     `?<Constant "communication speed: %f">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_11:
        DC32     `?<Constant "GPS latitude = %f\\r\\n">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_12:
        DC32     `?<Constant "GPS longitude = %f\\r\\n">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_13:
        DC32     curTm

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_14:
        DC32     GPSdata+0x14

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_15:
        DC32     rlist

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_16:
        DC32     `?<Constant "GPS thread">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_17:
        DC32     SD1

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_18:
        DC32     gps_str

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_19:
        DC32     engineConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_20:
        DC32     GPSserialConfig

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_21:
        DC32     `?<Constant "GPS tx">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_22:
        DC32     `?<Constant "GPS rx">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_23:
        DC32     _Z19GpsThreadEntryPointPv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_24:
        DC32     gpsThreadStack

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_25:
        DC32     _Z12printGpsInfov

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_26:
        DC32     `?<Constant "gpsinfo">`

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP memset
          CFI Block cfiBlock6 Using cfiCommon0
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
          CFI EndBlock cfiBlock6

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
`?<Constant "GPS RX %s">`:
        DATA
        DC8 "GPS RX %s"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPS TX %s">`:
        DATA
        DC8 "GPS TX %s"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "m=%d,e=%d: vehicle sp...">`:
        DATA
        DC8 "m=%d,e=%d: vehicle speed = %f\015\012"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "communication speed: %f">`:
        DATA
        DC8 "communication speed: %f"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPS latitude = %f\\r\\n">`:
        DATA
        DC8 "GPS latitude = %f\015\012"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPS longitude = %f\\r\\n">`:
        DATA
        DC8 "GPS longitude = %f\015\012"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPS thread">`:
        DATA
        DC8 "GPS thread"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPS tx">`:
        DATA
        DC8 "GPS tx"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPS rx">`:
        DATA
        DC8 "GPS rx"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "gpsinfo">`:
        DATA
        DC8 "gpsinfo"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "uart gps">`:
        DATA
        DC8 "uart gps"
        DC8 0, 0, 0

        END
//  106 
//  107 void initGps(void) {
//  108 	if(!engineConfiguration->isGpsEnabled)
//  109 		return;
//  110 
//  111 
//  112 	sdStart(GPS_SERIAL_DEVICE, &GPSserialConfig);
//  113 //  GPS we have USART1: PB7 -> USART1_RX and PB6 -> USART1_TX
//  114 	mySetPadMode2("GPS tx", boardConfiguration->gps_tx_pin, PAL_MODE_ALTERNATE(7));
//  115 	mySetPadMode2("GPS rx", boardConfiguration->gps_rx_pin, PAL_MODE_ALTERNATE(7));
//  116 
//  117 // todo: add a thread which would save location. If the GPS 5Hz - we should save the location each 200 ms
//  118 	chThdCreateStatic(gpsThreadStack, sizeof(gpsThreadStack), LOWPRIO, GpsThreadEntryPoint, NULL);
//  119 
//  120 	addConsoleAction("gpsinfo", &printGpsInfo);
//  121 }
//  122 
//  123 #endif /* EFI_UART_GPS */
// 
// 1 360 bytes in section .bss
//    12 bytes in section .data
//     4 bytes in section .init_array
//   172 bytes in section .rodata
//   574 bytes in section .text
// 
//   556 bytes of CODE  memory (+ 22 bytes shared)
//   172 bytes of CONST memory
// 1 372 bytes of DATA  memory
//
//Errors: none
//Warnings: 1
