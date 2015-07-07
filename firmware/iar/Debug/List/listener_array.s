///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:53 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\util\listener_a /
//                       rray.cpp                                             /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\util\listener_a /
//                       rray.cpp -lCN F:\stuff\rusefi_sourceforge\firmware\i /
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
//                       listener_array.s                                     /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME listener_array

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN __aeabi_memset
        EXTERN firmwareError

        PUBLIC _Z14clearCallbacksP16IntListenerArray
        PUBLIC _Z15invokeCallbacksP16IntListenerArrayi
        PUBLIC _Z21invokeArgIntCallbacksP16IntListenerArrayi
        PUBLIC _Z21invokeIntIntCallbacksP16IntListenerArrayii
        PUBLIC _Z25invokeIntIntVoidCallbacksP16IntListenerArrayii
        PUBLIC _ZN16IntListenerArray16registerCallbackEPFviEPv
        PUBLIC _ZN16IntListenerArray16registerCallbackEPFvvE
        PUBLIC _ZN16IntListenerArray22invokeJustArgCallbacksEv
        PUBLIC _ZN16IntListenerArrayC1Ev
        PUBLIC _ZN16IntListenerArrayC2Ev
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
        
// F:\stuff\rusefi_sourceforge\firmware\util\listener_array.cpp
//    1 /*
//    2  * @file listener_array.cpp
//    3  *
//    4  * @date Jan 1, 2013
//    5  * @author Andrey Belomutskiy, (c) 2012-2015
//    6  */
//    7 
//    8 #include "listener_array.h"
//    9 #include "main.h"
//   10 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _ZN16IntListenerArrayC2Ev
        THUMB
// __code __interwork __softfp IntListenerArray::subobject IntListenerArray()
_ZN16IntListenerArrayC2Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN16IntListenerArrayC1Ev
        BL       _ZN16IntListenerArrayC1Ev
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock0

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _ZN16IntListenerArrayC1Ev
        THUMB
//   11 IntListenerArray::IntListenerArray() {
_ZN16IntListenerArrayC1Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   12 	currentListenersCount = 0;
        MOVS     R0,#+0
        STR      R0,[R4, #+0]
//   13 	memset(&args, 0, sizeof(args));
        MOVS     R2,#+60
        MOVS     R1,#+0
        ADDS     R0,R4,#+64
          CFI FunCall memset
        BL       memset
//   14 	memset(&callbacks, 0, sizeof(callbacks));
        MOVS     R2,#+60
        MOVS     R1,#+0
        ADDS     R0,R4,#+4
          CFI FunCall memset
        BL       memset
//   15 }
        MOVS     R0,R4
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock1
//   16 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _ZN16IntListenerArray16registerCallbackEPFviEPv
        THUMB
//   17 void IntListenerArray::registerCallback(VoidInt handler, void *arg) {
_ZN16IntListenerArray16registerCallbackEPFviEPv:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//   18 	efiAssertVoid(currentListenersCount < MAX_INT_LISTENER_COUNT, "Too many callbacks");
        LDR      R0,[R4, #+0]
        CMP      R0,#+15
        BLT.N    ??registerCallback_0
        LDR.N    R0,??DataTable0
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??registerCallback_1
//   19 	int index = currentListenersCount++;
??registerCallback_0:
        LDR      R0,[R4, #+0]
        ADDS     R1,R0,#+1
        STR      R1,[R4, #+0]
//   20 	callbacks[index] = handler;
        ADDS     R1,R4,R0, LSL #+2
        STR      R5,[R1, #+4]
//   21 	args[index] = arg;
        ADDS     R1,R4,R0, LSL #+2
        STR      R6,[R1, #+64]
//   22 }
??registerCallback_1:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock2

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0:
        DC32     `?<Constant "Too many callbacks">`
//   23 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _ZN16IntListenerArray16registerCallbackEPFvvE
        THUMB
//   24 void IntListenerArray::registerCallback(Void listener) {
_ZN16IntListenerArray16registerCallbackEPFvvE:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//   25 	registerCallback((VoidInt)listener, NULL);
        MOVS     R2,#+0
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _ZN16IntListenerArray16registerCallbackEPFviEPv
        BL       _ZN16IntListenerArray16registerCallbackEPFviEPv
//   26 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock3
//   27 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _Z15invokeCallbacksP16IntListenerArrayi
        THUMB
//   28 void invokeCallbacks(IntListenerArray *array, int value) {
_Z15invokeCallbacksP16IntListenerArrayi:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//   29 	for (int i = 0; i < array->currentListenersCount; i++)
        MOVS     R6,#+0
??invokeCallbacks_0:
        LDR      R0,[R4, #+0]
        CMP      R6,R0
        BGE.N    ??invokeCallbacks_1
//   30 		(array->callbacks[i])(value);
        MOVS     R0,R5
        ADDS     R1,R4,R6, LSL #+2
        LDR      R1,[R1, #+4]
          CFI FunCall
        BLX      R1
        ADDS     R6,R6,#+1
        B.N      ??invokeCallbacks_0
//   31 }
??invokeCallbacks_1:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock4
//   32 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _ZN16IntListenerArray22invokeJustArgCallbacksEv
        THUMB
//   33 void IntListenerArray::invokeJustArgCallbacks() {
_ZN16IntListenerArray22invokeJustArgCallbacksEv:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
//   34 	for (int i = 0; i < currentListenersCount; i++) {
        MOVS     R5,#+0
??invokeJustArgCallbacks_0:
        LDR      R0,[R4, #+0]
        CMP      R5,R0
        BGE.N    ??invokeJustArgCallbacks_1
//   35 		VoidPtr listener = (VoidPtr)callbacks[i];
        ADDS     R0,R4,R5, LSL #+2
        LDR      R6,[R0, #+4]
//   36 		void *arg = args[i];
        ADDS     R0,R4,R5, LSL #+2
        LDR      R7,[R0, #+64]
//   37 		(listener)(arg);
        MOVS     R0,R7
          CFI FunCall
        BLX      R6
//   38 	}
        ADDS     R5,R5,#+1
        B.N      ??invokeJustArgCallbacks_0
//   39 }
??invokeJustArgCallbacks_1:
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock5
//   40 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _Z21invokeArgIntCallbacksP16IntListenerArrayi
        THUMB
//   41 void invokeArgIntCallbacks(IntListenerArray *array, int value) {
_Z21invokeArgIntCallbacksP16IntListenerArrayi:
        PUSH     {R4-R8,LR}
          CFI R14 Frame(CFA, -4)
          CFI R8 Frame(CFA, -8)
          CFI R7 Frame(CFA, -12)
          CFI R6 Frame(CFA, -16)
          CFI R5 Frame(CFA, -20)
          CFI R4 Frame(CFA, -24)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
//   42 	for (int i = 0; i < array->currentListenersCount; i++) {
        MOVS     R6,#+0
??invokeArgIntCallbacks_0:
        LDR      R0,[R4, #+0]
        CMP      R6,R0
        BGE.N    ??invokeArgIntCallbacks_1
//   43 		ArgIntListener listener = (ArgIntListener)array->callbacks[i];
        ADDS     R0,R4,R6, LSL #+2
        LDR      R7,[R0, #+4]
//   44 		void *arg = array->args[i];
        ADDS     R0,R4,R6, LSL #+2
        LDR      R8,[R0, #+64]
//   45 		(listener)(arg, value);
        MOVS     R1,R5
        MOV      R0,R8
          CFI FunCall
        BLX      R7
//   46 	}
        ADDS     R6,R6,#+1
        B.N      ??invokeArgIntCallbacks_0
//   47 }
??invokeArgIntCallbacks_1:
        POP      {R4-R8,PC}       ;; return
          CFI EndBlock cfiBlock6
//   48 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function _Z21invokeIntIntCallbacksP16IntListenerArrayii
        THUMB
//   49 void invokeIntIntCallbacks(IntListenerArray *array, int value, int value2) {
_Z21invokeIntIntCallbacksP16IntListenerArrayii:
        PUSH     {R4-R8,LR}
          CFI R14 Frame(CFA, -4)
          CFI R8 Frame(CFA, -8)
          CFI R7 Frame(CFA, -12)
          CFI R6 Frame(CFA, -16)
          CFI R5 Frame(CFA, -20)
          CFI R4 Frame(CFA, -24)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//   50 	for (int i = 0; i < array->currentListenersCount; i++) {
        MOVS     R7,#+0
??invokeIntIntCallbacks_0:
        LDR      R0,[R4, #+0]
        CMP      R7,R0
        BGE.N    ??invokeIntIntCallbacks_1
//   51 		VoidIntInt listener = (VoidIntInt)array->callbacks[i];
        ADDS     R0,R4,R7, LSL #+2
        LDR      R8,[R0, #+4]
//   52 		(listener)(value, value2);
        MOVS     R1,R6
        MOVS     R0,R5
          CFI FunCall
        BLX      R8
//   53 	}
        ADDS     R7,R7,#+1
        B.N      ??invokeIntIntCallbacks_0
//   54 }
??invokeIntIntCallbacks_1:
        POP      {R4-R8,PC}       ;; return
          CFI EndBlock cfiBlock7
//   55 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function _Z25invokeIntIntVoidCallbacksP16IntListenerArrayii
        THUMB
//   56 void invokeIntIntVoidCallbacks(IntListenerArray *array, int value, int value2) {
_Z25invokeIntIntVoidCallbacksP16IntListenerArrayii:
        PUSH     {R4-R8,LR}
          CFI R14 Frame(CFA, -4)
          CFI R8 Frame(CFA, -8)
          CFI R7 Frame(CFA, -12)
          CFI R6 Frame(CFA, -16)
          CFI R5 Frame(CFA, -20)
          CFI R4 Frame(CFA, -24)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//   57 	for (int i = 0; i < array->currentListenersCount; i++) {
        MOVS     R7,#+0
??invokeIntIntVoidCallbacks_0:
        LDR      R0,[R4, #+0]
        CMP      R7,R0
        BGE.N    ??invokeIntIntVoidCallbacks_1
//   58 		IntIntVoidListener listener = (IntIntVoidListener)array->callbacks[i];
        ADDS     R0,R4,R7, LSL #+2
        LDR      R8,[R0, #+4]
//   59 		(listener)(value, value2, array->args[i]);
        ADDS     R0,R4,R7, LSL #+2
        LDR      R2,[R0, #+64]
        MOVS     R1,R6
        MOVS     R0,R5
          CFI FunCall
        BLX      R8
//   60 	}
        ADDS     R7,R7,#+1
        B.N      ??invokeIntIntVoidCallbacks_0
//   61 }
??invokeIntIntVoidCallbacks_1:
        POP      {R4-R8,PC}       ;; return
          CFI EndBlock cfiBlock8

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function _Z14clearCallbacksP16IntListenerArray
          CFI NoCalls
        THUMB
_Z14clearCallbacksP16IntListenerArray:
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
        BX       LR               ;; return
          CFI EndBlock cfiBlock9

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP memset
          CFI Block cfiBlock10 Using cfiCommon0
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
          CFI EndBlock cfiBlock10

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Too many callbacks">`:
        DATA
        DC8 "Too many callbacks"
        DC8 0

        END
//   62 
//   63 void clearCallbacks(IntListenerArray *array) {
//   64 	array->currentListenersCount = 0;
//   65 }
// 
//  20 bytes in section .rodata
// 332 bytes in section .text
// 
// 310 bytes of CODE  memory (+ 22 bytes shared)
//  20 bytes of CONST memory
//
//Errors: none
//Warnings: none
