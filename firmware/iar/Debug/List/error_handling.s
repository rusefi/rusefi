///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:47 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\controllers\err /
//                       or_handling.cpp                                      /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\controllers\err /
//                       or_handling.cpp -lCN F:\stuff\rusefi_sourceforge\fir /
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
//                       error_handling.s                                     /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME error_handling

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1
        #define SHT_INIT_ARRAY 0xe

        EXTERN _Z14turnAllPinsOffv
        EXTERN _ZN18LoggingWithStorageC1EPKc
        EXTERN absI
        EXTERN append
        EXTERN appendMsgPrefix
        EXTERN chThdSleep
        EXTERN chvprintf
        EXTERN dbg_panic_msg
        EXTERN getTimeNowSeconds
        EXTERN lcdShowFatalMessage
        EXTERN main_loop_started
        EXTERN msObjectInit
        EXTERN port_halt
        EXTERN print
        EXTERN resetLogging
        EXTERN scheduleLogging
        EXTERN vtlist
        EXTERN warningEnabled

        PUBLIC _Z10onLockHookv
        PUBLIC _Z12onUnlockHookv
        PUBLIC allReady
        PUBLIC assertVtList
        PUBLIC chDbgPanic3
        PUBLIC dbg_panic_file
        PUBLIC dbg_panic_line
        PUBLIC getVtSizeEstimate
        PUBLIC getWarninig
        PUBLIC globalVt
        PUBLIC initErrorHandling
        PUBLIC isInsideTriggerHandler
        PUBLIC lastLockTime
        PUBLIC maxLockTime
        PUBLIC warning
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\controllers\error_handling.cpp
//    1 /**
//    2  * @file error_handling.cpp
//    3  *
//    4  * @date Apr 1, 2014
//    5  * @author Andrey Belomutskiy, (c) 2012-2015
//    6  */
//    7 
//    8 #include "main.h"
//    9 #include "error_handling.h"
//   10 #include "io_pins.h"
//   11 #include "memstreams.h"
//   12 #include "efilib2.h"
//   13 
//   14 #if EFI_HD44780_LCD
//   15 #include "lcd_HD44780.h"
//   16 #endif /* EFI_HD44780_LCD */
//   17 

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//   18 static time_t timeOfPreviousWarning = -10;
timeOfPreviousWarning:
        DATA
        DC32 4294967286

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function __sti__routine
        THUMB
// static __intrinsic __interwork __softfp void __sti__routine()
__sti__routine:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   19 static LoggingWithStorage logger("error handling");
        LDR.N    R1,??DataTable8_1
        LDR.N    R0,??DataTable8_2
          CFI FunCall _ZN18LoggingWithStorageC1EPKc
        BL       _ZN18LoggingWithStorageC1EPKc
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock0

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
logger:
        DS8 260
//   20 
//   21 #define WARNING_PREFIX "WARNING: "
//   22 
//   23 extern int warningEnabled;
//   24 extern bool_t main_loop_started;
//   25 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   26 const char *dbg_panic_file;
dbg_panic_file:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   27 int dbg_panic_line;
dbg_panic_line:
        DS8 4
//   28 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function chDbgPanic3
        THUMB
//   29 void chDbgPanic3(const char *msg, const char * file, int line) {
chDbgPanic3:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//   30 	if (hasFatalError())
        LDR.N    R0,??DataTable8_3
        LDR      R0,[R0, #+0]
        CMP      R0,#+0
        BNE.N    ??chDbgPanic3_0
//   31 		return;
//   32 	dbg_panic_file = file;
??chDbgPanic3_1:
        LDR.N    R0,??DataTable8_4
        STR      R5,[R0, #+0]
//   33 	dbg_panic_line = line;
        LDR.N    R0,??DataTable8_5
        STR      R6,[R0, #+0]
//   34 #if CH_DBG_SYSTEM_STATE_CHECK
//   35 	dbg_panic_msg = msg;
        LDR.N    R0,??DataTable8_3
        STR      R4,[R0, #+0]
//   36 #endif /* CH_DBG_SYSTEM_STATE_CHECK */
//   37 
//   38 	ON_FATAL_ERROR();
        LDR.N    R0,??DataTable8_6  ;; 0x40020c18
        MOV      R1,#+16384
        STR      R1,[R0, #+0]
          CFI FunCall _Z14turnAllPinsOffv
        BL       _Z14turnAllPinsOffv
//   39 #if EFI_HD44780_LCD
//   40 	lcdShowFatalMessage((char *) msg);
        MOVS     R0,R4
          CFI FunCall lcdShowFatalMessage
        BL       lcdShowFatalMessage
//   41 #endif /* EFI_HD44780_LCD */
//   42 
//   43 	if (!main_loop_started) {
        LDR.N    R0,??DataTable8_7
        LDRB     R0,[R0, #+0]
        CMP      R0,#+0
        BNE.N    ??chDbgPanic3_2
//   44 		print("fatal %s %s:%d\r\n", msg, file, line);
        MOVS     R3,R6
        MOVS     R2,R5
        MOVS     R1,R4
        LDR.N    R0,??DataTable8_8
          CFI FunCall print
        BL       print
//   45 		chThdSleepSeconds(1);
        MOV      R0,#+1000
          CFI FunCall chThdSleep
        BL       chThdSleep
//   46 		chSysHalt();
          CFI FunCall port_halt
        BL       port_halt
//   47 	}
//   48 }
??chDbgPanic3_2:
??chDbgPanic3_0:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock1
//   49 
//   50 #define WARNING_BUFFER_SIZE 80

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   51 static char warningBuffer[WARNING_BUFFER_SIZE];
warningBuffer:
        DS8 80

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   52 static MemoryStream warningStream;
warningStream:
        DS8 20
//   53 
//   54 /**
//   55  * OBD_PCM_Processor_Fault is the general error code for now
//   56  *
//   57  * @returns TRUE in case there are too many warnings
//   58  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function warning
        THUMB
//   59 int warning(obd_code_e code, const char *fmt, ...) {
warning:
        PUSH     {R2,R3}
          CFI CFA R13+8
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -12)
          CFI R7 Frame(CFA, -16)
          CFI R6 Frame(CFA, -20)
          CFI R5 Frame(CFA, -24)
          CFI R4 Frame(CFA, -28)
          CFI CFA R13+32
        MOVS     R4,R0
        MOVS     R5,R1
//   60         UNUSED(code);
//   61   
//   62 	int now = getTimeNowSeconds();
          CFI FunCall getTimeNowSeconds
        BL       getTimeNowSeconds
        MOVS     R6,R0
//   63 	if (absI(now - timeOfPreviousWarning) < 10 || !warningEnabled)
        LDR.N    R0,??DataTable8_9
        LDR      R0,[R0, #+0]
        SUBS     R0,R6,R0
          CFI FunCall absI
        BL       absI
        CMP      R0,#+10
        BLT.N    ??warning_0
        LDR.N    R0,??DataTable8_10
        LDR      R0,[R0, #+0]
        CMP      R0,#+0
        BNE.N    ??warning_1
//   64 		return true; // we just had another warning, let's not spam
??warning_0:
        MOVS     R0,#+1
        B.N      ??warning_2
//   65 	timeOfPreviousWarning = now;
??warning_1:
        LDR.N    R0,??DataTable8_9
        STR      R6,[R0, #+0]
//   66 
//   67 	resetLogging(&logger); // todo: is 'reset' really needed here?
        LDR.N    R0,??DataTable8_2
          CFI FunCall resetLogging
        BL       resetLogging
//   68 	appendMsgPrefix(&logger);
        LDR.N    R0,??DataTable8_2
          CFI FunCall appendMsgPrefix
        BL       appendMsgPrefix
//   69 
//   70 	va_list ap;
//   71 	va_start(ap, fmt);
        ADD      R0,SP,#+24
        MOVS     R7,R0
//   72 	append(&logger, WARNING_PREFIX);
        LDR.N    R1,??DataTable8_11
        LDR.N    R0,??DataTable8_2
          CFI FunCall append
        BL       append
//   73 	warningStream.eos = 0; // reset
        LDR.N    R0,??DataTable8_12
        MOVS     R1,#+0
        STR      R1,[R0, #+12]
//   74 	chvprintf((BaseSequentialStream *) &warningStream, fmt, ap);
        MOVS     R2,R7
        MOVS     R1,R5
        LDR.N    R0,??DataTable8_12
          CFI FunCall chvprintf
        BL       chvprintf
//   75 	warningStream.buffer[warningStream.eos] = 0;
        LDR.N    R0,??DataTable8_12
        LDR      R0,[R0, #+12]
        LDR.N    R1,??DataTable8_12
        LDR      R1,[R1, #+4]
        MOVS     R2,#+0
        STRB     R2,[R0, R1]
//   76 	va_end(ap);
//   77 
//   78 	append(&logger, warningBuffer);
        LDR.N    R1,??DataTable8_13
        LDR.N    R0,??DataTable8_2
          CFI FunCall append
        BL       append
//   79 	append(&logger, DELIMETER);
        ADR.N    R1,??DataTable8  ;; ","
        LDR.N    R0,??DataTable8_2
          CFI FunCall append
        BL       append
//   80 	scheduleLogging(&logger);
        LDR.N    R0,??DataTable8_2
          CFI FunCall scheduleLogging
        BL       scheduleLogging
//   81 
//   82 	return FALSE;
        MOVS     R0,#+0
??warning_2:
        POP      {R1,R4-R7}
          CFI R4 SameValue
          CFI R5 SameValue
          CFI R6 SameValue
          CFI R7 SameValue
          CFI CFA R13+12
        LDR      PC,[SP], #+12    ;; return
          CFI EndBlock cfiBlock2
//   83 }
//   84 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function getWarninig
          CFI NoCalls
        THUMB
//   85 char *getWarninig(void) {
//   86 	return warningBuffer;
getWarninig:
        LDR.N    R0,??DataTable8_13
        BX       LR               ;; return
          CFI EndBlock cfiBlock3
//   87 }
//   88 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   89 uint32_t lastLockTime;
lastLockTime:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   90 uint32_t maxLockTime = 0;
maxLockTime:
        DS8 4
//   91 

        SECTION `.bss`:DATA:REORDER:NOROOT(0)
//   92 bool isInsideTriggerHandler = false;
isInsideTriggerHandler:
        DS8 1
//   93 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _Z10onLockHookv
          CFI NoCalls
        THUMB
//   94 void onLockHook(void) {
//   95 	lastLockTime = GET_TIMESTAMP();
_Z10onLockHookv:
        LDR.N    R0,??DataTable8_14
        LDR.N    R1,??DataTable8_15  ;; 0xe0001004
        LDR      R1,[R1, #+0]
        STR      R1,[R0, #+0]
//   96 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock4
//   97 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _Z12onUnlockHookv
          CFI NoCalls
        THUMB
//   98 void onUnlockHook(void) {
//   99 	uint32_t t = GET_TIMESTAMP() - lastLockTime;
_Z12onUnlockHookv:
        LDR.N    R0,??DataTable8_15  ;; 0xe0001004
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable8_14
        LDR      R1,[R1, #+0]
        SUBS     R0,R0,R1
//  100 	if (t > maxLockTime) {
        LDR.N    R1,??DataTable8_16
        LDR      R1,[R1, #+0]
        CMP      R1,R0
        BCS.N    ??onUnlockHook_0
//  101 		maxLockTime = t;
        LDR.N    R1,??DataTable8_16
        STR      R0,[R1, #+0]
//  102 	}
//  103 //	if (t > 2800) {
//  104 //		// un-comment this if you want a nice stop for a breakpoint
//  105 //		maxLockTime = t + 1;
//  106 //	}
//  107 }
??onUnlockHook_0:
        BX       LR               ;; return
          CFI EndBlock cfiBlock5
//  108 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function initErrorHandling
        THUMB
//  109 void initErrorHandling(void) {
initErrorHandling:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  110 	msObjectInit(&warningStream, (uint8_t *) warningBuffer, WARNING_BUFFER_SIZE, 0);
        MOVS     R3,#+0
        MOVS     R2,#+80
        LDR.N    R1,??DataTable8_13
        LDR.N    R0,??DataTable8_12
          CFI FunCall msObjectInit
        BL       msObjectInit
//  111 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock6
//  112 
//  113 extern VTList vtlist;
//  114 extern bool_t main_loop_started;
//  115 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function getVtSizeEstimate
          CFI NoCalls
        THUMB
//  116 int getVtSizeEstimate(void) {
//  117 	VirtualTimer *first = vtlist.vt_next;
getVtSizeEstimate:
        LDR.N    R0,??DataTable8_17
        LDR      R1,[R0, #+0]
//  118 	VirtualTimer *cur = first->vt_next;
        LDR      R2,[R1, #+0]
//  119 	int c = 0;
        MOVS     R0,#+0
//  120 	while (c++ < 20 && cur != first) {
??getVtSizeEstimate_0:
        MOVS     R3,R0
        ADDS     R0,R3,#+1
        CMP      R3,#+20
        BGE.N    ??getVtSizeEstimate_1
        CMP      R2,R1
        BEQ.N    ??getVtSizeEstimate_1
//  121 		cur = cur->vt_next;
        LDR      R2,[R2, #+0]
        B.N      ??getVtSizeEstimate_0
//  122 	}
//  123 	return c;
??getVtSizeEstimate_1:
        BX       LR               ;; return
          CFI EndBlock cfiBlock7
//  124 }
//  125 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  126 int globalVt;
globalVt:
        DS8 4
//  127 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  128 int allReady = 0;
allReady:
        DS8 4

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function assertVtList
        THUMB
//  129 void assertVtList(void) {
assertVtList:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  130 	if (!main_loop_started)
        LDR.N    R0,??DataTable8_7
        LDRB     R0,[R0, #+0]
        CMP      R0,#+0
        BEQ.N    ??assertVtList_0
//  131 		return;
//  132 	globalVt = getVtSizeEstimate();
??assertVtList_1:
          CFI FunCall getVtSizeEstimate
        BL       getVtSizeEstimate
        LDR.N    R1,??DataTable8_18
        STR      R0,[R1, #+0]
//  133 	//efiAssertVoid(globalVt > 3, "VT list?");
//  134 	if(globalVt <=3 ) {
        LDR.N    R0,??DataTable8_18
        LDR      R0,[R0, #+0]
        CMP      R0,#+4
        BGE.N    ??assertVtList_2
//  135 		allReady++;
        LDR.N    R0,??DataTable8_19
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+1
        LDR.N    R1,??DataTable8_19
        STR      R0,[R1, #+0]
//  136 	}
//  137 }
??assertVtList_2:
??assertVtList_0:
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock8

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8:
        DC8      ",",0x0,0x0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_1:
        DC32     `?<Constant "error handling">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_2:
        DC32     logger

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_3:
        DC32     dbg_panic_msg

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_4:
        DC32     dbg_panic_file

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_5:
        DC32     dbg_panic_line

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_6:
        DC32     0x40020c18

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_7:
        DC32     main_loop_started

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_8:
        DC32     `?<Constant "fatal %s %s:%d\\r\\n">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_9:
        DC32     timeOfPreviousWarning

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_10:
        DC32     warningEnabled

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_11:
        DC32     `?<Constant "WARNING: ">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_12:
        DC32     warningStream

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_13:
        DC32     warningBuffer

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_14:
        DC32     lastLockTime

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_15:
        DC32     0xe0001004

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_16:
        DC32     maxLockTime

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_17:
        DC32     vtlist

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_18:
        DC32     globalVt

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_19:
        DC32     allReady

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
`?<Constant "fatal %s %s:%d\\r\\n">`:
        DATA
        DC8 "fatal %s %s:%d\015\012"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "WARNING: ">`:
        DATA
        DC8 "WARNING: "
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(1)
        DATA
        DC8 ","

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "error handling">`:
        DATA
        DC8 "error handling"
        DC8 0

        END
// 
// 385 bytes in section .bss
//   4 bytes in section .data
//   4 bytes in section .init_array
//  50 bytes in section .rodata
// 416 bytes in section .text
// 
// 420 bytes of CODE  memory
//  50 bytes of CONST memory
// 389 bytes of DATA  memory
//
//Errors: none
//Warnings: 1
