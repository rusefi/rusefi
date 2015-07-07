///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:58 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\hw_layer\pwm_ge /
//                       nerator.cpp                                          /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\hw_layer\pwm_ge /
//                       nerator.cpp -lCN F:\stuff\rusefi_sourceforge\firmwar /
//                       e\iar\Debug\List\ -lA F:\stuff\rusefi_sourceforge\fi /
//                       rmware\iar\Debug\List\ -o                            /
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
//                       pwm_generator.s                                      /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME pwm_generator

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _Z17outputPinRegisterPKcP9OutputPinP12GPIO_TypeDefj
        EXTERN _Z8getHwPin11brain_pin_e
        EXTERN _Z9getHwPort11brain_pin_e
        EXTERN _ZN9OutputPin8setValueEi
        EXTERN _ZN9PwmConfig13weComplexInitEPKciPfiPPaPFvPS_EPFvS5_iE
        EXTERN __aeabi_f2lz
        EXTERN __aeabi_l2f
        EXTERN firmwareError

        PUBLIC _Z13applyPinStateP9PwmConfigi
        PUBLIC _Z14startSimplePwmP9PwmConfigPKcP9OutputPinffPFvS0_iE
        PUBLIC _Z16initPwmGeneratorv
        PUBLIC _Z17startSimplePwmExtP9PwmConfigPKc11brain_pin_eP9OutputPinffPFvS0_iE
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\hw_layer\pwm_generator.cpp
//    1 /**
//    2  * @file    pwm_generator.cpp
//    3  * @brief   software PWM generator
//    4  *
//    5  * Software PWM implementation. Considering how low all frequencies are, we can totally afford a couple of float multiplications.
//    6  * By generating PWM programmatically we are saving the timers for better purposes. This implementation also supports generating
//    7  * synchronized waves as needed for example to emulate dual Hall-effect crankshaft position sensors.
//    8  *
//    9  *
//   10  * @date May 28, 2013
//   11  * @author Andrey Belomutskiy, (c) 2012-2015
//   12  *
//   13  */
//   14 
//   15 #include "pwm_generator.h"
//   16 
//   17 #include "pin_repository.h"
//   18 #include "datalogging.h"
//   19 
//   20 /**
//   21  * This method controls the actual hardware pins
//   22  *
//   23  * This method takes ~350 ticks.
//   24  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _Z13applyPinStateP9PwmConfigi
        THUMB
//   25 void applyPinState(PwmConfig *state, int stateIndex) {
_Z13applyPinStateP9PwmConfigi:
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
//   26 	efiAssertVoid(stateIndex < PWM_PHASE_MAX_COUNT, "invalid stateIndex");
        CMP      R5,#+252
        BLT.N    ??applyPinState_0
        LDR.N    R0,??DataTable1_2
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??applyPinState_1
//   27 	efiAssertVoid(state->multiWave.waveCount <= PWM_PHASE_MAX_WAVE_PER_PWM, "invalid waveCount");
??applyPinState_0:
        LDR      R0,[R4, #+12]
        CMP      R0,#+4
        BLT.N    ??applyPinState_2
        LDR.N    R0,??DataTable1_3
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??applyPinState_1
//   28 	for (int waveIndex = 0; waveIndex < state->multiWave.waveCount; waveIndex++) {
??applyPinState_2:
        MOVS     R6,#+0
??applyPinState_3:
        LDR      R0,[R4, #+12]
        CMP      R6,R0
        BGE.N    ??applyPinState_4
//   29 		OutputPin *output = state->outputPins[waveIndex];
        LDR      R7,[R4, R6, LSL #+2]
//   30 		int value = state->multiWave.waves[waveIndex].pinStates[stateIndex];
        LDR      R0,[R4, #+16]
        LDR      R0,[R0, R6, LSL #+2]
        LDRSB    R8,[R5, R0]
//   31 		output->setValue(value);
        MOV      R1,R8
        MOVS     R0,R7
          CFI FunCall _ZN9OutputPin8setValueEi
        BL       _ZN9OutputPin8setValueEi
//   32 	}
        ADDS     R6,R6,#+1
        B.N      ??applyPinState_3
//   33 }
??applyPinState_4:
??applyPinState_1:
        POP      {R4-R8,PC}       ;; return
          CFI EndBlock cfiBlock0
//   34 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z14startSimplePwmP9PwmConfigPKcP9OutputPinffPFvS0_iE
        THUMB
//   35 void startSimplePwm(PwmConfig *state, const char *msg, OutputPin *output, float frequency, float dutyCycle, pwm_gen_callback *stateChangeCallback) {
_Z14startSimplePwmP9PwmConfigPKcP9OutputPinffPFvS0_iE:
        PUSH     {R3-R9,LR}
          CFI R14 Frame(CFA, -4)
          CFI R9 Frame(CFA, -8)
          CFI R8 Frame(CFA, -12)
          CFI R7 Frame(CFA, -16)
          CFI R6 Frame(CFA, -20)
          CFI R5 Frame(CFA, -24)
          CFI R4 Frame(CFA, -28)
          CFI CFA R13+32
        VPUSH    {D8}
          CFI D8 Frame(CFA, -40)
          CFI CFA R13+40
        SUB      SP,SP,#+32
          CFI CFA R13+72
        MOVS     R4,R0
        MOVS     R7,R1
        MOVS     R5,R2
        VMOV.F32 S16,S0
        VMOV.F32 S17,S1
        MOVS     R6,R3
//   36 	efiAssertVoid(dutyCycle >= 0 && dutyCycle <= 1, "dutyCycle");
        VCMP.F32 S17,#0.0
        FMSTAT   
        BLT.N    ??startSimplePwm_0
        VLDR.W   S0,??DataTable1  ;; 0x3f800001
        VCMP.F32 S17,S0
        FMSTAT   
        BMI.N    ??startSimplePwm_1
??startSimplePwm_0:
        LDR.N    R0,??DataTable1_4
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??startSimplePwm_2
//   37 
//   38 	float switchTimes[] = { dutyCycle, 1 };
??startSimplePwm_1:
        ADD      R0,SP,#+24
        LDR.N    R1,??DataTable1_5
        LDM      R1!,{R2,R3}
        STM      R0!,{R2,R3}
        SUBS     R1,R1,#+8
        SUBS     R0,R0,#+8
        VSTR     S17,[SP, #+24]
//   39 	pin_state_t pinStates0[] = { 0, 1 };
        LDR.N    R0,??DataTable1_6
        LDRH     R0,[R0, #+0]
        STRH     R0,[SP, #+16]
//   40 
//   41 	pin_state_t *pinStates[1] = { pinStates0 };
        ADD      R0,SP,#+16
        STR      R0,[SP, #+20]
//   42 
//   43 	state->outputPins[0] = output;
        STR      R5,[R4, #+0]
//   44 
//   45 	state->periodNt = US2NT(frequency2periodUs(frequency));
        VLDR.W   S0,??DataTable1_1  ;; 0x49742400
        VDIV.F32 S0,S0,S16
        VMOV     R0,S0
          CFI FunCall __aeabi_f2lz
        BL       __aeabi_f2lz
        MOVS     R2,#+168
        MOVS     R3,#+0
        MOV      R8,R0
        MOV      R9,R1
        UMULL    R0,R1,R2,R8
        MLA      R1,R2,R9,R1
        MLA      R1,R3,R8,R1
          CFI FunCall __aeabi_l2f
        BL       __aeabi_l2f
        STR      R0,[R4, #+24]
//   46 	state->weComplexInit(msg, 2, switchTimes, 1, pinStates, NULL, stateChangeCallback);
        STR      R6,[SP, #+12]
        MOVS     R0,#+0
        STR      R0,[SP, #+8]
        ADD      R0,SP,#+20
        STR      R0,[SP, #+4]
        MOVS     R0,#+1
        STR      R0,[SP, #+0]
        ADD      R3,SP,#+24
        MOVS     R2,#+2
        MOVS     R1,R7
        MOVS     R0,R4
          CFI FunCall _ZN9PwmConfig13weComplexInitEPKciPfiPPaPFvPS_EPFvS5_iE
        BL       _ZN9PwmConfig13weComplexInitEPKciPfiPPaPFvPS_EPFvS5_iE
//   47 }
??startSimplePwm_2:
        ADD      SP,SP,#+32
          CFI CFA R13+40
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+32
        POP      {R0,R4-R9,PC}    ;; return
          CFI EndBlock cfiBlock1

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1:
        DC32     0x3f800001

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_1:
        DC32     0x49742400

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_2:
        DC32     `?<Constant "invalid stateIndex">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_3:
        DC32     `?<Constant "invalid waveCount">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_4:
        DC32     `?<Constant "dutyCycle">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_5:
        DC32     `?<Constant {(0.0F), (1.0F)}>`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_6:
        DC32     `?<Constant {(int8_t)'\\000', (int8_t)'\\001'}>`
//   48 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z17startSimplePwmExtP9PwmConfigPKc11brain_pin_eP9OutputPinffPFvS0_iE
        THUMB
//   49 void startSimplePwmExt(PwmConfig *state, const char *msg, brain_pin_e brainPin, OutputPin *output, float frequency,
//   50 		float dutyCycle, pwm_gen_callback *stateChangeCallback) {
_Z17startSimplePwmExtP9PwmConfigPKc11brain_pin_eP9OutputPinffPFvS0_iE:
        PUSH     {R4-R10,LR}
          CFI R14 Frame(CFA, -4)
          CFI R10 Frame(CFA, -8)
          CFI R9 Frame(CFA, -12)
          CFI R8 Frame(CFA, -16)
          CFI R7 Frame(CFA, -20)
          CFI R6 Frame(CFA, -24)
          CFI R5 Frame(CFA, -28)
          CFI R4 Frame(CFA, -32)
          CFI CFA R13+32
        VPUSH    {D8}
          CFI D8 Frame(CFA, -40)
          CFI CFA R13+40
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
        MOVS     R7,R3
        VMOV.F32 S16,S0
        VMOV.F32 S17,S1
        LDR      R8,[SP, #+40]
//   51 
//   52 	GPIO_TypeDef * port = getHwPort(brainPin);
        MOVS     R0,R6
          CFI FunCall _Z9getHwPort11brain_pin_e
        BL       _Z9getHwPort11brain_pin_e
        MOV      R9,R0
//   53 	int pin = getHwPin(brainPin);
        MOVS     R0,R6
          CFI FunCall _Z8getHwPin11brain_pin_e
        BL       _Z8getHwPin11brain_pin_e
        MOV      R10,R0
//   54 	outputPinRegister(msg, output, port, pin);
        MOV      R3,R10
        MOV      R2,R9
        MOVS     R1,R7
        MOVS     R0,R5
          CFI FunCall _Z17outputPinRegisterPKcP9OutputPinP12GPIO_TypeDefj
        BL       _Z17outputPinRegisterPKcP9OutputPinP12GPIO_TypeDefj
//   55 
//   56 	startSimplePwm(state, msg, output, frequency, dutyCycle, stateChangeCallback);
        MOV      R3,R8
        VMOV.F32 S1,S17
        VMOV.F32 S0,S16
        MOVS     R2,R7
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z14startSimplePwmP9PwmConfigPKcP9OutputPinffPFvS0_iE
        BL       _Z14startSimplePwmP9PwmConfigPKcP9OutputPinffPFvS0_iE
//   57 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+32
        POP      {R4-R10,PC}      ;; return
          CFI EndBlock cfiBlock2
//   58 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z16initPwmGeneratorv
          CFI NoCalls
        THUMB
//   59 void initPwmGenerator(void) {
//   60 }
_Z16initPwmGeneratorv:
        BX       LR               ;; return
          CFI EndBlock cfiBlock3

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "invalid stateIndex">`:
        DATA
        DC8 "invalid stateIndex"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "invalid waveCount">`:
        DATA
        DC8 "invalid waveCount"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "dutyCycle">`:
        DATA
        DC8 "dutyCycle"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant {(0.0F), (1.0F)}>`:
        DATA
        DC32 0H, 3F800000H

        SECTION `.rodata`:CONST:REORDER:NOROOT(1)
`?<Constant {(int8_t)'\\000', (int8_t)'\\001'}>`:
        DATA
        DC8 0, 1

        END
//   61 
// 
//  62 bytes in section .rodata
// 352 bytes in section .text
// 
// 352 bytes of CODE  memory
//  62 bytes of CONST memory
//
//Errors: none
//Warnings: 1
