///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:08:42 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\util\efilib2.cp /
//                       p                                                    /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\util\efilib2.cp /
//                       p -lCN F:\stuff\rusefi_sourceforge\firmware\iar\Debu /
//                       g\List\ -lA F:\stuff\rusefi_sourceforge\firmware\iar /
//                       \Debug\List\ -o F:\stuff\rusefi_sourceforge\firmware /
//                       \iar\Debug\Obj\ --no_cse --no_unroll --no_inline     /
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
//                       efilib2.s                                            /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME efilib2

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN lockAnyContext
        EXTERN unlockAnyContext

        PUBLIC _Z12updateAndSetP7State64j
        PUBLIC _ZN17Overflow64Counter3getEv
        PUBLIC _ZN17Overflow64CounterC1Ev
        PUBLIC _ZN17Overflow64CounterC2Ev
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\util\efilib2.cpp
//    1 /**
//    2  * @file	efilib2.cpp
//    3  *
//    4  * @date Apr 14, 2014
//    5  * @author Andrey Belomutskiy, (c) 2012-2015
//    6  */
//    7 
//    8 #include "efilib2.h"
//    9 
//   10 /**
//   11  * The main use-case of this class is to keep track of a 64-bit global number of CPU ticks from reset.
//   12  *
//   13  * stm32f4 hardware has a 32-bit Cycle Count Register (CYCCNT), which is incremented with every CPU cycle.
//   14  * With 32 bits and 168MHz speed this counter overflows every 4B/168M = 23 seconds. The job of this class is to
//   15  * keep track of the current CYCCNT value, detect these overflows, and provide a nice,
//   16  * clean 64 bit global cycle counter.
//   17  *
//   18  * In order for this to function, it's your responsibility to invoke offer() method at least once a second.
//   19  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _ZN17Overflow64CounterC2Ev
        THUMB
// __code __interwork __softfp Overflow64Counter::subobject Overflow64Counter()
_ZN17Overflow64CounterC2Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN17Overflow64CounterC1Ev
        BL       _ZN17Overflow64CounterC1Ev
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock0

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _ZN17Overflow64CounterC1Ev
          CFI NoCalls
        THUMB
//   20 Overflow64Counter::Overflow64Counter() {
//   21 	state.highBits = 0;
_ZN17Overflow64CounterC1Ev:
        MOVS     R2,#+0
        MOVS     R3,#+0
        STRD     R2,R3,[R0, #+0]
//   22 	state.lowBits = 0;
        MOVS     R1,#+0
        STR      R1,[R0, #+8]
//   23 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock1
//   24 
//   25 /**
//   26  * in order to have atomic writes this should be invoked within a critical section
//   27  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z12updateAndSetP7State64j
          CFI NoCalls
        THUMB
//   28 void updateAndSet(State64 *state, uint32_t value) {
//   29 	if (value < state->lowBits) {
_Z12updateAndSetP7State64j:
        LDR      R2,[R0, #+8]
        CMP      R1,R2
        BCS.N    ??updateAndSet_0
//   30 		// new value less than previous value means there was an overflow in that 32 bit counter
//   31 		state->highBits += 0x100000000LL;
        LDRD     R2,R3,[R0, #+0]
        ADDS     R2,R2,#+0
        ADCS     R3,R3,#+1
        STRD     R2,R3,[R0, #+0]
//   32 	}
//   33 	state->lowBits = value;
??updateAndSet_0:
        STR      R1,[R0, #+8]
//   34 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock2
//   35 
//   36 #if EFI_UNIT_TEST
//   37 efitime_t Overflow64Counter::update(uint32_t value) {
//   38 	updateAndSet(&state, value);
//   39 	return state.highBits + state.lowBits;
//   40 }
//   41 #endif
//   42 
//   43 // todo: make this a macro? always inline?

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _ZN17Overflow64Counter3getEv
        THUMB
//   44 efitime_t Overflow64Counter::get() {
_ZN17Overflow64Counter3getEv:
        PUSH     {R3-R11,LR}
          CFI R14 Frame(CFA, -4)
          CFI R11 Frame(CFA, -8)
          CFI R10 Frame(CFA, -12)
          CFI R9 Frame(CFA, -16)
          CFI R8 Frame(CFA, -20)
          CFI R7 Frame(CFA, -24)
          CFI R6 Frame(CFA, -28)
          CFI R5 Frame(CFA, -32)
          CFI R4 Frame(CFA, -36)
          CFI CFA R13+40
        MOV      R9,R0
//   45 #if EFI_PROD_CODE
//   46 	bool alreadyLocked = lockAnyContext();
          CFI FunCall lockAnyContext
        BL       lockAnyContext
        MOV      R8,R0
//   47 	efitime_t localH = state.highBits;
        LDRD     R4,R5,[R9, #+0]
//   48 	uint32_t localLow = state.lowBits;
        LDR      R11,[R9, #+8]
//   49 
//   50 	uint32_t value = GET_TIMESTAMP();
        LDR.N    R0,??DataTable0  ;; 0xe0001004
        LDR      R10,[R0, #+0]
//   51 
//   52 	if (value < localLow) {
        CMP      R10,R11
        BCS.N    ??get_0
//   53 		// new value less than previous value means there was an overflow in that 32 bit counter
//   54 		localH += 0x100000000LL;
        ADDS     R4,R4,#+0
        ADCS     R5,R5,#+1
//   55 	}
//   56 
//   57 	efitime_t result = localH + value;
??get_0:
        MOV      R0,R10
        MOVS     R1,#+0
        ADDS     R6,R4,R0
        ADCS     R7,R5,R1
//   58 
//   59 	if (!alreadyLocked) {
        UXTB     R8,R8            ;; ZeroExt  R8,R8,#+24,#+24
        CMP      R8,#+0
        BNE.N    ??get_1
//   60 		unlockAnyContext();
          CFI FunCall unlockAnyContext
        BL       unlockAnyContext
//   61 	}
//   62 	return result;
??get_1:
        MOVS     R0,R6
        MOVS     R1,R7
        POP      {R2,R4-R11,PC}   ;; return
          CFI EndBlock cfiBlock3
//   63 #else
//   64 
//   65 	/**
//   66 	 * this method is lock-free and thread-safe, that's because the 'update' method
//   67 	 * is atomic with a critical zone requirement.
//   68 	 *
//   69 	 * http://stackoverflow.com/questions/5162673/how-to-read-two-32bit-counters-as-a-64bit-integer-without-race-condition
//   70 	 */
//   71 	efitime_t localH;
//   72 	uint32_t localLow;
//   73 	int counter = 0;
//   74 	while (true) {
//   75 		localH = state.highBits;
//   76 		localLow = state.lowBits;
//   77 		efitime_t localH2 = state.highBits;
//   78 		if (localH == localH2)
//   79 			break;
//   80 #if EFI_PROD_CODE || defined(__DOXYGEN__)
//   81 		if (counter++ == 10000)
//   82 			chDbgPanic("lock-free frozen");
//   83 #endif /* EFI_PROD_CODE */
//   84 	}
//   85 	/**
//   86 	 * We need to take current counter after making a local 64 bit snapshot
//   87 	 */
//   88 	uint32_t value = GET_TIMESTAMP();
//   89 
//   90 	if (value < localLow) {
//   91 		// new value less than previous value means there was an overflow in that 32 bit counter
//   92 		localH += 0x100000000LL;
//   93 	}
//   94 
//   95 	return localH + value;
//   96 #endif
//   97 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0:
        DC32     0xe0001004

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        END
// 
// 122 bytes in section .text
// 
// 122 bytes of CODE memory
//
//Errors: none
//Warnings: none
