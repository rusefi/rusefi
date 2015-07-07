///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:54 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\hw_layer\micros /
//                       econd_timer.cpp                                      /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\hw_layer\micros /
//                       econd_timer.cpp -lCN F:\stuff\rusefi_sourceforge\fir /
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
//                       microsecond_timer.s                                  /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME microsecond_timer

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN GPTD5
        EXTERN chThdCreateStatic
        EXTERN chThdSleep
        EXTERN firmwareError
        EXTERN getTimeNowNt
        EXTERN gptStart
        EXTERN gptStartOneShotI
        EXTERN gptStopTimerI
        EXTERN itoa10
        EXTERN rlist
        EXTERN strcpy

        PUBLIC globalTimerCallback
        PUBLIC initMicrosecondTimer
        PUBLIC setHardwareUsTimer
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\hw_layer\microsecond_timer.cpp
//    1 /**
//    2  * @file	microsecond_timer.cpp
//    3  *
//    4  * Here we have a 1MHz timer dedicated to event scheduling. We are using one of the 32-bit timers here,
//    5  * so this timer can schedule events up to 4B/100M ~ 4000 seconds ~ 1 hour from current time.
//    6  *
//    7  * GPT5 timer clock: 84000000Hz
//    8  * If only it was a better multiplier of 2 (84000000 = 328125 * 256)
//    9  *
//   10  * @date Apr 14, 2014
//   11  * @author Andrey Belomutskiy, (c) 2012-2015
//   12  */
//   13 
//   14 #include "main.h"
//   15 #include "microsecond_timer.h"
//   16 #include "scheduler.h"
//   17 #include "rfiutil.h"
//   18 
//   19 // https://my.st.com/public/STe2ecommunities/mcu/Lists/cortex_mx_stm32/Flat.aspx?RootFolder=https%3a%2f%2fmy.st.com%2fpublic%2fSTe2ecommunities%2fmcu%2fLists%2fcortex_mx_stm32%2fInterrupt%20on%20CEN%20bit%20setting%20in%20TIM7&FolderCTID=0x01200200770978C69A1141439FE559EB459D7580009C4E14902C3CDE46A77F0FFD06506F5B&currentviews=474
//   20 
//   21 #if (EFI_PROD_CODE && HAL_USE_GPT) || defined(__DOXYGEN__)
//   22 
//   23 #define GPTDEVICE GPTD5
//   24 

        SECTION `.bss`:DATA:REORDER:NOROOT(3)
//   25 static volatile efitick_t lastSetTimerTimeNt;
lastSetTimerTimeNt:
        DS8 8

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   26 static int lastSetTimerValue;
lastSetTimerValue:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(0)
//   27 static volatile bool isTimerPending = FALSE;
isTimerPending:
        DS8 1
//   28 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   29 static volatile int timerCallbackCounter = 0;
timerCallbackCounter:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   30 static volatile int timerRestartCounter = 0;
timerRestartCounter:
        DS8 4
//   31 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   32 schfunc_t globalTimerCallback;
globalTimerCallback:
        DS8 4
//   33 

        SECTION `.bss`:DATA:REORDER:NOROOT(3)
//   34 static THD_WORKING_AREA(mwThreadStack, UTILITY_THREAD_STACK_SIZE);
mwThreadStack:
        DS8 712
//   35 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   36 static const char * msg;
msg:
        DS8 4
//   37 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   38 static char buff[32];
buff:
        DS8 32
//   39 
//   40 /**
//   41  * sets the alarm to the specified number of microseconds from now.
//   42  * This function should be invoked under kernel lock which would disable interrupts.
//   43  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function setHardwareUsTimer
        THUMB
//   44 void setHardwareUsTimer(int32_t timeUs) {
setHardwareUsTimer:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   45 	if (timeUs == 1)
        CMP      R4,#+1
        BNE.N    ??setHardwareUsTimer_0
//   46 		timeUs = 2; // for some reason '1' does not really work
        MOVS     R0,#+2
        MOVS     R4,R0
//   47 	efiAssertVoid(timeUs > 0, "not positive timeUs");
??setHardwareUsTimer_0:
        CMP      R4,#+1
        BGE.N    ??setHardwareUsTimer_1
        LDR.N    R0,??DataTable4
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??setHardwareUsTimer_2
//   48 	efiAssertVoid(timeUs < 10 * US_PER_SECOND, "setHardwareUsTimer() too large");
??setHardwareUsTimer_1:
        LDR.N    R0,??DataTable4_1  ;; 0x989680
        CMP      R4,R0
        BLT.N    ??setHardwareUsTimer_3
        LDR.N    R0,??DataTable4_2
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??setHardwareUsTimer_2
//   49 
//   50 	if (GPTDEVICE.state == GPT_ONESHOT)
??setHardwareUsTimer_3:
        LDR.N    R0,??DataTable4_3
        LDRB     R0,[R0, #+0]
        CMP      R0,#+4
        BNE.N    ??setHardwareUsTimer_4
//   51 		gptStopTimerI(&GPTDEVICE);
        LDR.N    R0,??DataTable4_3
          CFI FunCall gptStopTimerI
        BL       gptStopTimerI
//   52 	gptStartOneShotI(&GPTDEVICE, timeUs);
??setHardwareUsTimer_4:
        MOVS     R1,R4
        LDR.N    R0,??DataTable4_3
          CFI FunCall gptStartOneShotI
        BL       gptStartOneShotI
//   53 
//   54 	lastSetTimerTimeNt = getTimeNowNt();
          CFI FunCall getTimeNowNt
        BL       getTimeNowNt
        LDR.N    R2,??DataTable4_4
        STRD     R0,R1,[R2, #+0]
//   55 	lastSetTimerValue = timeUs;
        LDR.N    R0,??DataTable4_5
        STR      R4,[R0, #+0]
//   56 	isTimerPending = TRUE;
        LDR.N    R0,??DataTable4_6
        MOVS     R1,#+1
        STRB     R1,[R0, #+0]
//   57 	timerRestartCounter++;
        LDR.N    R0,??DataTable4_7
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+1
        LDR.N    R1,??DataTable4_7
        STR      R0,[R1, #+0]
//   58 }
??setHardwareUsTimer_2:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock0
//   59 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z8callbackP9GPTDriver
        THUMB
//   60 static void callback(GPTDriver *gptp) {
_Z8callbackP9GPTDriver:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   61 	(void)gptp;
//   62 	timerCallbackCounter++;
        LDR.N    R0,??DataTable4_8
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+1
        LDR.N    R1,??DataTable4_8
        STR      R0,[R1, #+0]
//   63 	if (globalTimerCallback == NULL) {
        LDR.N    R0,??DataTable4_9
        LDR      R0,[R0, #+0]
        CMP      R0,#+0
        BNE.N    ??callback_0
//   64 		firmwareError("NULL globalTimerCallback");
        LDR.N    R0,??DataTable4_10
          CFI FunCall firmwareError
        BL       firmwareError
//   65 		return;
        B.N      ??callback_1
//   66 	}
//   67 	isTimerPending = false;
??callback_0:
        LDR.N    R0,??DataTable4_6
        MOVS     R1,#+0
        STRB     R1,[R0, #+0]
//   68 
//   69 //	// test code
//   70 //	setOutputPinValue(LED_CRANKING, timerCallbackCounter % 2);
//   71 //	int mod = timerCallbackCounter % 400;
//   72 //	chSysLockFromIsr()
//   73 //	;
//   74 //	setHardwareUsTimer(400 - mod);
//   75 //	chSysUnlockFromIsr()
//   76 //	;
//   77 
//   78 	globalTimerCallback(NULL);
        MOVS     R0,#+0
        LDR.N    R1,??DataTable4_9
        LDR      R1,[R1, #+0]
          CFI FunCall
        BLX      R1
//   79 }
??callback_1:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock1
//   80 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z15usTimerWatchDogv
        THUMB
//   81 static void usTimerWatchDog(void) {
_Z15usTimerWatchDogv:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
//   82 	if (getTimeNowNt() >= lastSetTimerTimeNt + 2 * CORE_CLOCK) {
          CFI FunCall getTimeNowNt
        BL       getTimeNowNt
        LDR.N    R2,??DataTable4_4
        LDRD     R4,R5,[R2, #+0]
        LDR.N    R2,??DataTable4_11  ;; 0x1406f400
        MOVS     R3,#+0
        ADDS     R2,R4,R2
        ADCS     R3,R5,R3
        CMP      R1,R3
        BLT.N    ??usTimerWatchDog_0
        BGT.N    ??usTimerWatchDog_1
        CMP      R0,R2
        BCC.N    ??usTimerWatchDog_0
//   83 		strcpy(buff, "no_event");
??usTimerWatchDog_1:
        LDR.N    R1,??DataTable4_12
        LDR.N    R0,??DataTable4_13
          CFI FunCall strcpy
        BL       strcpy
//   84 		itoa10(&buff[8], lastSetTimerValue);
        LDR.N    R0,??DataTable4_5
        LDR      R1,[R0, #+0]
        LDR.N    R0,??DataTable4_14
          CFI FunCall itoa10
        BL       itoa10
//   85 		firmwareError(buff);
        LDR.N    R0,??DataTable4_13
          CFI FunCall firmwareError
        BL       firmwareError
//   86 		return;
        B.N      ??usTimerWatchDog_2
//   87 	}
//   88 
//   89 	msg = isTimerPending ? "No_cb too long" : "Timer not awhile";
??usTimerWatchDog_0:
        LDR.N    R0,??DataTable4_6
        LDRB     R0,[R0, #+0]
        CMP      R0,#+0
        BEQ.N    ??usTimerWatchDog_3
        LDR.N    R0,??DataTable4_15
        LDR.N    R1,??DataTable4_16
        STR      R1,[R0, #+0]
        B.N      ??usTimerWatchDog_4
??usTimerWatchDog_3:
        LDR.N    R0,??DataTable4_15
        LDR.N    R1,??DataTable4_17
        STR      R1,[R0, #+0]
//   90 	// 2 seconds of inactivity would not look right
//   91 	efiAssertVoid(getTimeNowNt() < lastSetTimerTimeNt + 2 * CORE_CLOCK, msg);
??usTimerWatchDog_4:
          CFI FunCall getTimeNowNt
        BL       getTimeNowNt
        LDR.N    R2,??DataTable4_4
        LDRD     R4,R5,[R2, #+0]
        LDR.N    R2,??DataTable4_11  ;; 0x1406f400
        MOVS     R3,#+0
        ADDS     R2,R4,R2
        ADCS     R3,R5,R3
        CMP      R1,R3
        BLT.N    ??usTimerWatchDog_5
        BGT.N    ??usTimerWatchDog_6
        CMP      R0,R2
        BCC.N    ??usTimerWatchDog_5
??usTimerWatchDog_6:
        LDR.N    R0,??DataTable4_15
        LDR      R0,[R0, #+0]
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??usTimerWatchDog_2
//   92 
//   93 }
??usTimerWatchDog_5:
??usTimerWatchDog_2:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock2
//   94 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z8mwThreadi
        THUMB
//   95 static msg_t mwThread(int param) {
_Z8mwThreadi:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   96 	(void)param;
//   97 	chRegSetThreadName("timer watchdog");
        LDR.N    R0,??DataTable4_18
        LDR      R0,[R0, #+24]
        LDR.N    R1,??DataTable4_19
        STR      R1,[R0, #+24]
//   98 
//   99 	while (true) {
//  100 		chThdSleepMilliseconds(1000); // once a second is enough
??mwThread_0:
        MOV      R0,#+1000
          CFI FunCall chThdSleep
        BL       chThdSleep
//  101 		usTimerWatchDog();
          CFI FunCall _Z15usTimerWatchDogv
        BL       _Z15usTimerWatchDogv
        B.N      ??mwThread_0
          CFI EndBlock cfiBlock3
//  102 	}
//  103 #if defined __GNUC__
//  104 	return -1;
//  105 #endif        
//  106 }

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "not positive timeUs">`:
        DATA
        DC8 "not positive timeUs"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "setHardwareUsTimer() ...">`:
        DATA
        DC8 "setHardwareUsTimer() too large"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "NULL globalTimerCallback">`:
        DATA
        DC8 "NULL globalTimerCallback"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "no_event">`:
        DATA
        DC8 "no_event"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "No_cb too long">`:
        DATA
        DC8 "No_cb too long"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Timer not awhile">`:
        DATA
        DC8 "Timer not awhile"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "timer watchdog">`:
        DATA
        DC8 "timer watchdog"
        DC8 0
//  107 
//  108 //static const GPTConfig gpt5cfg;
//  109 

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
//  110 static const GPTConfig gpt5cfg = { 1000000, /* 1 MHz timer clock.*/
gpt5cfg:
        DATA
        DC32 1000000, _Z8callbackP9GPTDriver, 0
//  111 callback, /* Timer callback.*/
//  112 0 };
//  113 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function initMicrosecondTimer
        THUMB
//  114 void initMicrosecondTimer(void) {
initMicrosecondTimer:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  115 
//  116 	gptStart(&GPTDEVICE, &gpt5cfg);
        LDR.N    R1,??DataTable4_20
        LDR.N    R0,??DataTable4_3
          CFI FunCall gptStart
        BL       gptStart
//  117 
//  118 	lastSetTimerTimeNt = getTimeNowNt();
          CFI FunCall getTimeNowNt
        BL       getTimeNowNt
        LDR.N    R2,??DataTable4_4
        STRD     R0,R1,[R2, #+0]
//  119 #if EFI_EMULATE_POSITION_SENSORS
//  120 	chThdCreateStatic(mwThreadStack, sizeof(mwThreadStack), NORMALPRIO, (tfunc_t) mwThread, NULL);
        MOVS     R0,#+0
        STR      R0,[SP, #+0]
        LDR.N    R3,??DataTable4_21
        MOVS     R2,#+64
        MOV      R1,#+712
        LDR.N    R0,??DataTable4_22
          CFI FunCall chThdCreateStatic
        BL       chThdCreateStatic
//  121 #endif /* EFI_ENGINE_EMULATOR */
//  122 
//  123 //	// test code
//  124 //	chSysLock()
//  125 //	;
//  126 //	setHardwareUsTimer(300);
//  127 //	chSysUnlock()
//  128 //	;
//  129 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock4

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4:
        DC32     `?<Constant "not positive timeUs">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_1:
        DC32     0x989680

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_2:
        DC32     `?<Constant "setHardwareUsTimer() ...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_3:
        DC32     GPTD5

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_4:
        DC32     lastSetTimerTimeNt

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_5:
        DC32     lastSetTimerValue

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_6:
        DC32     isTimerPending

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_7:
        DC32     timerRestartCounter

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_8:
        DC32     timerCallbackCounter

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_9:
        DC32     globalTimerCallback

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_10:
        DC32     `?<Constant "NULL globalTimerCallback">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_11:
        DC32     0x1406f400

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_12:
        DC32     `?<Constant "no_event">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_13:
        DC32     buff

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_14:
        DC32     buff+0x8

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_15:
        DC32     msg

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_16:
        DC32     `?<Constant "No_cb too long">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_17:
        DC32     `?<Constant "Timer not awhile">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_18:
        DC32     rlist

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_19:
        DC32     `?<Constant "timer watchdog">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_20:
        DC32     gpt5cfg

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_21:
        DC32     _Z8mwThreadi

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_22:
        DC32     mwThreadStack

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        END
//  130 
//  131 #endif /* EFI_PROD_CODE */
// 
// 773 bytes in section .bss
// 156 bytes in section .rodata
// 414 bytes in section .text
// 
// 414 bytes of CODE  memory
// 156 bytes of CONST memory
// 773 bytes of DATA  memory
//
//Errors: none
//Warnings: none
