///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:10:00 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\rusefi.cpp      /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\rusefi.cpp      /
//                       -lCN F:\stuff\rusefi_sourceforge\firmware\iar\Debug\ /
//                       List\ -lA F:\stuff\rusefi_sourceforge\firmware\iar\D /
//                       ebug\List\ -o F:\stuff\rusefi_sourceforge\firmware\i /
//                       ar\Debug\Obj\ --no_cse --no_unroll --no_inline       /
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
//                       rusefi.s                                             /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME rusefi

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1
        #define SHT_INIT_ARRAY 0xe

        EXTERN _Z12initHardwareP7Logging
        EXTERN _Z14initStatusLoopP6Engine
        EXTERN _Z14turnAllPinsOffv
        EXTERN _Z17initPinRepositoryv
        EXTERN _Z18initEngineEmulatorP7LoggingP6Engine
        EXTERN _Z18startStatusThreadsP6Engine
        EXTERN _Z19initEngineContollerP7Logging
        EXTERN _Z21updateDevConsoleStateP6Engine
        EXTERN _Z24applyNewHardwareSettingsv
        EXTERN _Z24prepareVoidConfigurationP22engine_configuration_s
        EXTERN _ZN18LoggingWithStorageC1EPKc
        EXTERN _ZN6Engine4initEv
        EXTERN __aeabi_memcpy
        EXTERN addConsoleAction
        EXTERN boardConfiguration
        EXTERN chDbgPanic3
        EXTERN chThdSleep
        EXTERN chVTSetI
        EXTERN chvprintf
        EXTERN engine
        EXTERN engineConfiguration
        EXTERN engineConfiguration2
        EXTERN getRemainingStack
        EXTERN indexOf
        EXTERN initErrorHandling
        EXTERN initializeConsole
        EXTERN lockAnyContext
        EXTERN msObjectInit
        EXTERN print
        EXTERN rlist
        EXTERN scheduleMsg
        EXTERN strcat
        EXTERN strcpy
        EXTERN strncpy
        EXTERN unlockAnyContext

        PUBLIC _Z23chDbgStackOverflowPanicP6Thread
        PUBLIC _Z8swo_initv
        PUBLIC activeConfiguration
        PUBLIC applyNewConfiguration
        PUBLIC errorMessageBuffer
        PUBLIC firmwareError
        PUBLIC getFirmwareError
        PUBLIC getRusEfiVersion
        PUBLIC hasFirmwareErrorFlag
        PUBLIC main_loop_started
        PUBLIC memcpy
        PUBLIC runRusEfi
        
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
          CFI Function __sti__routine
        THUMB
// static __intrinsic __interwork __softfp void __sti__routine()
__sti__routine:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        LDR.N    R1,??DataTable8
        LDR.N    R0,??DataTable8_1
          CFI FunCall _ZN18LoggingWithStorageC1EPKc
        BL       _ZN18LoggingWithStorageC1EPKc
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock0
// F:\stuff\rusefi_sourceforge\firmware\rusefi.cpp
//    1 /**
//    2  * @file	rusefi.cpp
//    3  * @brief Initialization code and main status reporting look
//    4  *
//    5  * @date Dec 25, 2013
//    6  * @author Andrey Belomutskiy, (c) 2012-2015
//    7  */
//    8 
//    9 /**
//   10  * @mainpage
//   11  *
//   12  * @section sec_into
//   13  *
//   14  * rusEfi is implemented based on the idea that with modern 100+ MHz microprocessors the relatively
//   15  * undemanding task of internal combustion engine control could be implemented in a high-level, processor-independent
//   16  * (to some extent) manner. Thus the key concepts of rusEfi: dependency on high-level hardware abstraction layer, software-based PWM etc.
//   17  *
//   18  * @section sec_main Brief overview
//   19  *
//   20  * rusEfi runs on crankshaft or camshaft ('trigger') position sensor events.
//   21  * Once per crankshaft revolution we evaluate the amount of needed fuel and
//   22  * the spark timing. Once we have decided on the parameters for this revolution
//   23  * we schedule all the actions to be triggered by the closest trigger event.
//   24  *
//   25  * We also have some utility threads like idle control thread and communication threads.
//   26  *
//   27  *
//   28  *
//   29  * @section sec_trigger Trigger Decoding
//   30  *
//   31  * Our primary trigger decoder is based on the idea of synchronizing the primary shaft signal and simply counting events on
//   32  * the secondary signal. A typical scenario would be when camshaft positions sensor is the primary signal and crankshaft is secondary,
//   33  * but sometimes there would be two signals generated by two camshaft sensors.
//   34  * Another scenario is when we only have crankshaft position sensor, this would make it the primary signal and there would be no secondary signal.
//   35  *
//   36  * There is no software filtering so the signals are expected to be valid. TODO: in reality we are still catching engine stop noise as unrealisticly high RPM.
//   37  *
//   38  * The decoder is configured to act either on the primary signal rise or on the primary signal fall. It then compares the duration
//   39  * of time from the previous signal to the duration of time from the signal before previous, and if the ratio falls into the configurable
//   40  * range between 'syncRatioFrom' and 'syncRatioTo' this is assumed to be the synchronizing event.
//   41  *
//   42  * For instance, for a 36/1 skipped tooth wheel the ratio range for synchronization is from 1.5 to 3
//   43  *
//   44  * Some triggers do not require synchronization, this case we just count signals.
//   45  * A single tooth primary signal would be a typical example when synchronization is not needed.
//   46  *
//   47  *
//   48  *
//   49  *
//   50  *
//   51  * @section sec_scheduler Event Scheduler
//   52  *
//   53  * It is a general agreement to measure all angles in crankshaft angles. In a four stroke
//   54  * engine, a full cycle consists of two revolutions of the crankshaft, so all the angles are
//   55  * running between 0 and 720 degrees.
//   56  *
//   57  * Ignition timing is a great example of a process which highlights the need of a hybrid
//   58  * approach to event scheduling.
//   59  * The most important part of controlling ignition
//   60  * is firing up the spark at the right moment - so, for this job we need 'angle-based' timing,
//   61  * for example we would need to fire up the spark at 700 degrees. Before we can fire up the spark
//   62  * at 700 degrees, we need to charge the ignition coil, for example this dwell time is 4ms - that
//   63  * means we need to turn on the coil at '4 ms before 700 degrees'. Let's  assume that the engine is
//   64  * current at 600 RPM - that means 360 degrees would take 100ms so 4ms is 14.4 degrees at current RPM which
//   65  * means we need to start charting the coil at 685.6 degrees.
//   66  *
//   67  * The position sensors at our disposal are not providing us the current position at any moment of time -
//   68  * all we've got is a set of events which are happening at the knows positions. For instance, let's assume that
//   69  * our sensor sends as an event at 0 degrees, at 90 degrees, at 600 degrees and and 690 degrees.
//   70  *
//   71  * So, for this particular sensor the most precise scheduling would be possible if we schedule coil charting
//   72  * as '85.6 degrees after the 600 degrees position sensor event', and spark firing as
//   73  * '10 degrees after the 690 position sensor event'. Considering current RPM, we calculate that '10 degress after' is
//   74  * 2.777ms, so we schedule spark firing at '2.777ms after the 690 position sensor event', thus combining trigger events
//   75  * with time-based offset.
//   76  *
//   77  * @section config Persistent Configuration
//   78  * engine_configuration_s structure is kept in the internal flash memory, it has all the settings. Currently rusefi.ini has a direct mapping of this structure.
//   79  *
//   80  * Please note that due to TunerStudio protocol it's important to have the total structure size in synch between the firmware and TS .ini file -
//   81  * just to make sure that this is not forgotten the size of the structure is hard-coded as PAGE_0_SIZE constant. There is always some 'unused' fields added in advance so that
//   82  * one can add some fields without the pain of increasing the total config page size.
//   83  * <br>See flash_main.cpp
//   84  *
//   85  *
//   86  * @section sec_fuel_injection Fuel Injection
//   87  *
//   88  *
//   89  * @sectuion sec_misc Misc
//   90  *
//   91  * <BR>See main_trigger_callback.cpp for main trigger event handler
//   92  * <BR>See fuel_math.cpp for details on fuel amount logic
//   93  * <BR>See rpm_calculator.cpp for details on how getRpm() is calculated
//   94  *
//   95  */
//   96 
//   97 #include "main.h"

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z16NVIC_SystemResetv
          CFI NoCalls
        THUMB
// static __interwork __softfp void NVIC_SystemReset()
_Z16NVIC_SystemResetv:
        DSB      
        LDR.N    R0,??DataTable8_2  ;; 0xe000ed0c
        LDR      R0,[R0, #+0]
        ANDS     R0,R0,#0x700
        LDR.N    R1,??DataTable8_3  ;; 0x5fa0004
        ORRS     R0,R1,R0
        LDR.N    R1,??DataTable8_2  ;; 0xe000ed0c
        STR      R0,[R1, #+0]
        DSB      
??NVIC_SystemReset_0:
        B.N      ??NVIC_SystemReset_0
          CFI EndBlock cfiBlock1
//   98 #include "trigger_structure.h"
//   99 #include "hardware.h"
//  100 #include "engine_controller.h"
//  101 #include "efiGpio.h"
//  102 
//  103 #include "global.h"
//  104 #include "rfi_perftest.h"
//  105 #include "rusefi.h"
//  106 #include "memstreams.h"
//  107 
//  108 #include "eficonsole.h"
//  109 #include "status_loop.h"
//  110 #include "pin_repository.h"
//  111 
//  112 #if EFI_HD44780_LCD
//  113 #include "lcd_HD44780.h"
//  114 #endif /* EFI_HD44780_LCD */
//  115 
//  116 #if EFI_ENGINE_EMULATOR || defined(__DOXYGEN__)
//  117 #include "engine_emulator.h"
//  118 #endif /* EFI_ENGINE_EMULATOR */
//  119 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  120 static LoggingWithStorage sharedLogger("main");
sharedLogger:
        DS8 260
//  121 

        SECTION `.bss`:DATA:REORDER:NOROOT(0)
//  122 bool_t main_loop_started = false;
main_loop_started:
        DS8 1
//  123 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  124 static MemoryStream firmwareErrorMessageStream;
firmwareErrorMessageStream:
        DS8 20

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  125 static char panicMessage[200];
panicMessage:
        DS8 200
//  126 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  127 uint8_t errorMessageBuffer[200];
errorMessageBuffer:
        DS8 200

        SECTION `.bss`:DATA:REORDER:NOROOT(0)
//  128 bool hasFirmwareErrorFlag = false;
hasFirmwareErrorFlag:
        DS8 1
//  129 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  130 static virtual_timer_t resetTimer;
resetTimer:
        DS8 20
//  131 
//  132 EXTERN_ENGINE
//  133 ;
//  134 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function getFirmwareError
          CFI NoCalls
        THUMB
//  135 char *getFirmwareError(void) {
//  136 	return (char*) errorMessageBuffer;
getFirmwareError:
        LDR.N    R0,??DataTable8_4
        BX       LR               ;; return
          CFI EndBlock cfiBlock2
//  137 }
//  138 
//  139 // todo: move this into a hw-specific file

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z9rebootNowv
        THUMB
//  140 static void rebootNow(void) {
_Z9rebootNowv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  141 	NVIC_SystemReset();
          CFI FunCall _Z16NVIC_SystemResetv
        BL       _Z16NVIC_SystemResetv
//  142 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock3
//  143 
//  144 /**
//  145  * Some configuration changes require full firmware reset.
//  146  * Once day we will write graceful shutdown, but that would be one day.
//  147  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _Z14scheduleRebootv
        THUMB
//  148 static void scheduleReboot(void) {
_Z14scheduleRebootv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  149 	scheduleMsg(&sharedLogger, "Rebooting in 5 seconds...");
        LDR.N    R1,??DataTable8_5
        LDR.N    R0,??DataTable8_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  150 	lockAnyContext();
          CFI FunCall lockAnyContext
        BL       lockAnyContext
//  151 	chVTSetI(&resetTimer, 5 * CH_FREQUENCY, (vtfunc_t) rebootNow, NULL);
        MOVS     R3,#+0
        LDR.N    R2,??DataTable8_6
        MOVW     R1,#+5000
        LDR.N    R0,??DataTable8_7
          CFI FunCall chVTSetI
        BL       chVTSetI
//  152 	unlockAnyContext();
          CFI FunCall unlockAnyContext
        BL       unlockAnyContext
//  153 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock4
//  154 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _Z8swo_initv
          CFI NoCalls
        THUMB
//  155 void swo_init() {
//  156 	// todo: make SWO work
//  157 //     uint32_t SWOSpeed = 2000000; //2000kbps, default for ST-LINK
//  158 //     // todo: use a macro to access clock speed
//  159 //     uint32_t SWOPrescaler = (168000000 / SWOSpeed) - 1; // SWOSpeed in Hz, note that F_CPU is expected to be 96000000 in this case
//  160 //     CoreDebug->DEMCR = CoreDebug_DEMCR_TRCENA_Msk;
//  161 //     *((volatile unsigned *)(ITM_BASE + 0x400F0)) = 0x00000002; // "Selected PIN Protocol Register": Select which protocol to use for trace output (2: SWO)
//  162 //     *((volatile unsigned *)(ITM_BASE + 0x40010)) = SWOPrescaler; // "Async Clock Prescaler Register". Scale the baud rate of the asynchronous output
//  163 //     *((volatile unsigned *)(ITM_BASE + 0x00FB0)) = 0xC5ACCE55; // ITM Lock Access Register, C5ACCE55 enables more write access to Control Register 0xE00 :: 0xFFC
//  164 //     ITM->TCR = ITM_TCR_TraceBusID_Msk | ITM_TCR_SWOENA_Msk | ITM_TCR_SYNCENA_Msk | ITM_TCR_ITMENA_Msk; // ITM Trace Control Register
//  165 //     ITM->TPR = ITM_TPR_PRIVMASK_Msk; // ITM Trace Privilege Register
//  166 //     ITM->TER = 0x00000001; // ITM Trace Enable Register. Enabled tracing on stimulus ports. One bit per stimulus port.
//  167 //     *((volatile unsigned *)(ITM_BASE + 0x01000)) = 0x400003FE; // DWT_CTRL
//  168 //     *((volatile unsigned *)(ITM_BASE + 0x40304)) = 0x00000100; // Formatter and Flush Control Register
//  169 }
_Z8swo_initv:
        BX       LR               ;; return
          CFI EndBlock cfiBlock5
//  170 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  171 engine_configuration_s activeConfiguration;
activeConfiguration:
        DS8 2216
//  172 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _Z28rememberCurrentConfigurationv
        THUMB
//  173 static void rememberCurrentConfiguration(void) {
_Z28rememberCurrentConfigurationv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  174 	memcpy(&activeConfiguration, engineConfiguration, sizeof(engine_configuration_s));
        MOVW     R2,#+2216
        LDR.N    R0,??DataTable8_8
        LDR      R1,[R0, #+0]
        LDR.N    R0,??DataTable8_9
          CFI FunCall memcpy
        BL       memcpy
//  175 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock6
//  176 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function applyNewConfiguration
        THUMB
//  177 void applyNewConfiguration(void) {
applyNewConfiguration:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  178 	applyNewHardwareSettings();
          CFI FunCall _Z24applyNewHardwareSettingsv
        BL       _Z24applyNewHardwareSettingsv
//  179 	rememberCurrentConfiguration();
          CFI FunCall _Z28rememberCurrentConfigurationv
        BL       _Z28rememberCurrentConfigurationv
//  180 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock7
//  181 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function runRusEfi
        THUMB
//  182 void runRusEfi(void) {
runRusEfi:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  183 	msObjectInit(&firmwareErrorMessageStream, errorMessageBuffer, sizeof(errorMessageBuffer), 0);
        MOVS     R3,#+0
        MOVS     R2,#+200
        LDR.N    R1,??DataTable8_4
        LDR.N    R0,??DataTable8_10
          CFI FunCall msObjectInit
        BL       msObjectInit
//  184 
//  185 #if EFI_ENGINE_CONTROL || defined(__DOXYGEN__)
//  186 	engine->engineConfiguration2 = engineConfiguration2;
        LDR.N    R0,??DataTable8_11
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable8_12
        LDR      R1,[R1, #+0]
        STR      R1,[R0, #+40]
//  187 #endif
//  188 
//  189 	initErrorHandling();
          CFI FunCall initErrorHandling
        BL       initErrorHandling
//  190 
//  191 	swo_init();
          CFI FunCall _Z8swo_initv
        BL       _Z8swo_initv
//  192 
//  193 	prepareVoidConfiguration(&activeConfiguration);
        LDR.N    R0,??DataTable8_9
          CFI FunCall _Z24prepareVoidConfigurationP22engine_configuration_s
        BL       _Z24prepareVoidConfigurationP22engine_configuration_s
//  194 
//  195 	/**
//  196 	 * First data structure keeps track of which hardware I/O pins are used by whom
//  197 	 */
//  198 	initPinRepository();
          CFI FunCall _Z17initPinRepositoryv
        BL       _Z17initPinRepositoryv
//  199 
//  200 	/**
//  201 	 * Next we should initialize serial port console, it's important to know what's going on
//  202 	 */
//  203 	initializeConsole(&sharedLogger);
        LDR.N    R0,??DataTable8_1
          CFI FunCall initializeConsole
        BL       initializeConsole
//  204 
//  205 	engine->init();
        LDR.N    R0,??DataTable8_11
        LDR      R0,[R0, #+0]
          CFI FunCall _ZN6Engine4initEv
        BL       _ZN6Engine4initEv
//  206 
//  207 	addConsoleAction("reboot", scheduleReboot);
        LDR.N    R1,??DataTable8_13
        LDR.N    R0,??DataTable8_14
          CFI FunCall addConsoleAction
        BL       addConsoleAction
//  208 
//  209 	/**
//  210 	 * Initialize hardware drivers
//  211 	 */
//  212 	initHardware(&sharedLogger);
        LDR.N    R0,??DataTable8_1
          CFI FunCall _Z12initHardwareP7Logging
        BL       _Z12initHardwareP7Logging
//  213 
//  214 	initStatusLoop(engine);
        LDR.N    R0,??DataTable8_11
        LDR      R0,[R0, #+0]
          CFI FunCall _Z14initStatusLoopP6Engine
        BL       _Z14initStatusLoopP6Engine
//  215 	/**
//  216 	 * Now let's initialize actual engine control logic
//  217 	 * todo: should we initialize some? most? controllers before hardware?
//  218 	 */
//  219 	initEngineContoller(&sharedLogger PASS_ENGINE_PARAMETER_F);
        LDR.N    R0,??DataTable8_1
          CFI FunCall _Z19initEngineContollerP7Logging
        BL       _Z19initEngineContollerP7Logging
//  220 
//  221 #if EFI_PERF_METRICS || defined(__DOXYGEN__)
//  222 	initTimePerfActions(&sharedLogger);
//  223 #endif
//  224 
//  225 #if EFI_ENGINE_EMULATOR || defined(__DOXYGEN__)
//  226 	initEngineEmulator(&sharedLogger, engine);
        LDR.N    R0,??DataTable8_11
        LDR      R1,[R0, #+0]
        LDR.N    R0,??DataTable8_1
          CFI FunCall _Z18initEngineEmulatorP7LoggingP6Engine
        BL       _Z18initEngineEmulatorP7LoggingP6Engine
//  227 #endif
//  228 	startStatusThreads(engine);
        LDR.N    R0,??DataTable8_11
        LDR      R0,[R0, #+0]
          CFI FunCall _Z18startStatusThreadsP6Engine
        BL       _Z18startStatusThreadsP6Engine
//  229 
//  230 	rememberCurrentConfiguration();
          CFI FunCall _Z28rememberCurrentConfigurationv
        BL       _Z28rememberCurrentConfigurationv
//  231 
//  232 	print("Running main loop\r\n");
        LDR.N    R0,??DataTable8_15
          CFI FunCall print
        BL       print
//  233 	main_loop_started = true;
        LDR.N    R0,??DataTable8_16
        MOVS     R1,#+1
        STRB     R1,[R0, #+0]
//  234 	/**
//  235 	 * This loop is the closes we have to 'main loop' - but here we only publish the status. The main logic of engine
//  236 	 * control is around main_trigger_callback
//  237 	 */
//  238 	while (true) {
//  239 		efiAssertVoid(getRemainingStack(chThdSelf()) > 128, "stack#1");
??runRusEfi_0:
        LDR.N    R0,??DataTable8_17
        LDR      R0,[R0, #+24]
          CFI FunCall getRemainingStack
        BL       getRemainingStack
        CMP      R0,#+129
        BGE.N    ??runRusEfi_1
        LDR.N    R0,??DataTable8_18
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??runRusEfi_2
//  240 
//  241 #if (EFI_CLI_SUPPORT && !EFI_UART_ECHO_TEST_MODE) || defined(__DOXYGEN__)
//  242 		// sensor state + all pending messages for our own dev console
//  243 		updateDevConsoleState(engine);
??runRusEfi_1:
        LDR.N    R0,??DataTable8_11
        LDR      R0,[R0, #+0]
          CFI FunCall _Z21updateDevConsoleStateP6Engine
        BL       _Z21updateDevConsoleStateP6Engine
//  244 #endif /* EFI_CLI_SUPPORT */
//  245 
//  246 		chThdSleepMilliseconds(boardConfiguration->consoleLoopPeriod);
        LDR.N    R0,??DataTable8_19
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+316]
        MOV      R1,#+1000
        MULS     R0,R1,R0
        SUBS     R0,R0,#+1
        MOV      R1,#+1000
        UDIV     R0,R0,R1
        ADDS     R0,R0,#+1
          CFI FunCall chThdSleep
        BL       chThdSleep
        B.N      ??runRusEfi_0
//  247 	}
??runRusEfi_2:
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock8
//  248 }
//  249 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function _Z23chDbgStackOverflowPanicP6Thread
        THUMB
//  250 void chDbgStackOverflowPanic(Thread *otp) {
_Z23chDbgStackOverflowPanicP6Thread:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  251 	strcpy(panicMessage, "stack overflow: ");
        LDR.N    R1,??DataTable8_20
        LDR.N    R0,??DataTable8_21
          CFI FunCall strcpy
        BL       strcpy
//  252 #ifdef CH_USE_REGISTRY
//  253 	strcat(panicMessage, otp->p_name);
        LDR      R1,[R4, #+24]
          CFI FunCall strcat
        BL       strcat
//  254 #endif
//  255 	chDbgPanic3(panicMessage, __FILE__, __LINE__);
        MOVS     R2,#+255
        LDR.N    R1,??DataTable8_22
        LDR.N    R0,??DataTable8_21
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  256 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock9
//  257 
//  258 extern engine_pins_s enginePins;
//  259 
//  260 // todo: why is this method here and not in error_handling.cpp ?

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function firmwareError
        THUMB
//  261 void firmwareError(const char *errorMsg, ...) {
firmwareError:
        PUSH     {R1-R3}
          CFI CFA R13+12
        PUSH     {R4,R5,LR}
          CFI R14 Frame(CFA, -16)
          CFI R5 Frame(CFA, -20)
          CFI R4 Frame(CFA, -24)
          CFI CFA R13+24
        MOVS     R4,R0
//  262 	if (hasFirmwareErrorFlag)
        LDR.N    R0,??DataTable8_23
        LDRB     R0,[R0, #+0]
        CMP      R0,#+0
        BNE.N    ??firmwareError_0
//  263 		return;
//  264 	ON_FATAL_ERROR()
??firmwareError_1:
        LDR.N    R0,??DataTable8_24  ;; 0x40020c18
        MOV      R1,#+16384
        STR      R1,[R0, #+0]
          CFI FunCall _Z14turnAllPinsOffv
        BL       _Z14turnAllPinsOffv
//  265 	;
//  266 	hasFirmwareErrorFlag = true;
        LDR.N    R0,??DataTable8_23
        MOVS     R1,#+1
        STRB     R1,[R0, #+0]
//  267 	if (indexOf(errorMsg, '%') == -1) {
        MOVS     R1,#+37
        MOVS     R0,R4
          CFI FunCall indexOf
        BL       indexOf
        CMN      R0,#+1
        BNE.N    ??firmwareError_2
//  268 		/**
//  269 		 * in case of simple error message let's reduce stack usage
//  270 		 * because chvprintf might be causing an error
//  271 		 */
//  272 		strncpy((char*) errorMessageBuffer, errorMsg, sizeof(errorMessageBuffer) - 1);
        MOVS     R2,#+199
        MOVS     R1,R4
        LDR.N    R0,??DataTable8_4
          CFI FunCall strncpy
        BL       strncpy
//  273 		errorMessageBuffer[sizeof(errorMessageBuffer) - 1] = 0; // just to be sure
        LDR.N    R0,??DataTable8_4
        MOVS     R1,#+0
        STRB     R1,[R0, #+199]
        B.N      ??firmwareError_3
//  274 	} else {
//  275 		firmwareErrorMessageStream.eos = 0; // reset
??firmwareError_2:
        LDR.N    R0,??DataTable8_10
        MOVS     R1,#+0
        STR      R1,[R0, #+12]
//  276 		va_list ap;
//  277 		va_start(ap, errorMsg);
        ADD      R0,SP,#+12
        MOVS     R5,R0
//  278 		chvprintf((BaseSequentialStream *) &firmwareErrorMessageStream, errorMsg, ap);
        MOVS     R2,R5
        MOVS     R1,R4
        LDR.N    R0,??DataTable8_10
          CFI FunCall chvprintf
        BL       chvprintf
//  279 		va_end(ap);
//  280 
//  281 		firmwareErrorMessageStream.buffer[firmwareErrorMessageStream.eos] = 0; // need to terminate explicitly
        LDR.N    R0,??DataTable8_10
        LDR      R0,[R0, #+12]
        LDR.N    R1,??DataTable8_10
        LDR      R1,[R1, #+4]
        MOVS     R2,#+0
        STRB     R2,[R0, R1]
//  282 	}
//  283 }
??firmwareError_3:
??firmwareError_0:
        POP      {R4,R5}
          CFI R4 SameValue
          CFI R5 SameValue
          CFI CFA R13+16
        LDR      PC,[SP], #+16    ;; return
          CFI EndBlock cfiBlock10
//  284 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  285 static char UNUSED_RAM_SIZE[200];
UNUSED_RAM_SIZE:
        DS8 200
//  286 

        SECTION `.ccm`:DATA:REORDER:NOROOT(2)
//  287 static char UNUSED_CCM_SIZE[3600] CCM_OPTIONAL;
UNUSED_CCM_SIZE:
        DS8 3600

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function getRusEfiVersion
          CFI NoCalls
        THUMB
getRusEfiVersion:
        LDR.N    R0,??DataTable8_25
        LDRB     R0,[R0, #+0]
        CMP      R0,#+0
        BEQ.N    ??getRusEfiVersion_0
        MOVS     R0,#+123
        B.N      ??getRusEfiVersion_1
??getRusEfiVersion_0:
        LDR.N    R0,??DataTable8_26
        LDRB     R0,[R0, #+0]
        MOVS     R1,#+0
        MULS     R0,R1,R0
        CMP      R0,#+0
        BEQ.N    ??getRusEfiVersion_2
        MOVW     R0,#+3211
        B.N      ??getRusEfiVersion_1
??getRusEfiVersion_2:
        LDR.N    R0,??DataTable8_27  ;; 0x1337965
??getRusEfiVersion_1:
        BX       LR               ;; return
          CFI EndBlock cfiBlock11

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8:
        DC32     `?<Constant "main">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_1:
        DC32     sharedLogger

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_2:
        DC32     0xe000ed0c

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_3:
        DC32     0x5fa0004

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_4:
        DC32     errorMessageBuffer

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_5:
        DC32     `?<Constant "Rebooting in 5 second...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_6:
        DC32     _Z9rebootNowv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_7:
        DC32     resetTimer

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_8:
        DC32     engineConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_9:
        DC32     activeConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_10:
        DC32     firmwareErrorMessageStream

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_11:
        DC32     engine

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_12:
        DC32     engineConfiguration2

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_13:
        DC32     _Z14scheduleRebootv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_14:
        DC32     `?<Constant "reboot">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_15:
        DC32     `?<Constant "Running main loop\\r\\n">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_16:
        DC32     main_loop_started

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_17:
        DC32     rlist

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_18:
        DC32     `?<Constant "stack#1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_19:
        DC32     boardConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_20:
        DC32     `?<Constant "stack overflow: ">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_21:
        DC32     panicMessage

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_22:
        DC32     `?<Constant "F:\\\\stuff\\\\rusefi_sourc...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_23:
        DC32     hasFirmwareErrorFlag

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_24:
        DC32     0x40020c18

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_25:
        DC32     UNUSED_RAM_SIZE

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_26:
        DC32     UNUSED_CCM_SIZE

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_27:
        DC32     0x1337965

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP memcpy
          CFI Block cfiBlock12 Using cfiCommon0
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

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Rebooting in 5 second...">`:
        DATA
        DC8 "Rebooting in 5 seconds..."
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "reboot">`:
        DATA
        DC8 "reboot"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Running main loop\\r\\n">`:
        DATA
        DC8 "Running main loop\015\012"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "stack#1">`:
        DATA
        DC8 "stack#1"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "stack overflow: ">`:
        DATA
        DC8 "stack overflow: "
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "F:\\\\stuff\\\\rusefi_sourc...">`:
        DATA
        DC8 "F:\\stuff\\rusefi_sourceforge\\firmware\\rusefi.cpp"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "main">`:
        DATA
        DC8 "main"
        DC8 0, 0, 0

        END
//  288 
//  289 int getRusEfiVersion(void) {
//  290 	if (UNUSED_RAM_SIZE[0] != 0)
//  291 		return 123; // this is here to make the compiler happy about the unused array
//  292 	if (UNUSED_CCM_SIZE[0] * 0 != 0)
//  293 		return 3211; // this is here to make the compiler happy about the unused array
//  294 	return 20150629;
//  295 }
// 
// 3 118 bytes in section .bss
// 3 600 bytes in section .ccm
//     4 bytes in section .init_array
//   140 bytes in section .rodata
//   598 bytes in section .text
// 
//   580 bytes of CODE  memory (+ 22 bytes shared)
//   140 bytes of CONST memory
// 6 718 bytes of DATA  memory
//
//Errors: none
//Warnings: 1
