///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:38 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\hw_layer\HIP901 /
//                       1.cpp                                                /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\hw_layer\HIP901 /
//                       1.cpp -lCN F:\stuff\rusefi_sourceforge\firmware\iar\ /
//                       Debug\List\ -lA F:\stuff\rusefi_sourceforge\firmware /
//                       \iar\Debug\List\ -o F:\stuff\rusefi_sourceforge\firm /
//                       ware\iar\Debug\Obj\ --no_cse --no_unroll             /
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
//                       HIP9011.s                                            /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME HIP9011

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1
        #define SHT_INIT_ARRAY 0xe

        EXTERN DEFAULT_OUTPUT
        EXTERN SPID2
        EXTERN _Z10hwPortname11brain_pin_e
        EXTERN _Z10turnPinLowP14NamedOutputPin
        EXTERN _Z11turnPinHighP14NamedOutputPin
        EXTERN _Z12boolToStringb
        EXTERN _Z13printSpiStateP7LoggingP21board_configuration_s
        EXTERN _Z15scheduleByAngleiP12scheduling_sfPFvPvES1_P13RpmCalculator
        EXTERN _Z19getHip9011BandIndexf
        EXTERN _Z19getHip9011GainIndexf
        EXTERN _Z20getRevolutionCounterv
        EXTERN _Z21outputPinRegisterExt2PKcP9OutputPin11brain_pin_eP17pin_output_mode_e
        EXTERN _Z22getPinNameByAdcChannel13adc_channel_ePc
        EXTERN _Z23addTriggerEventListenerPFv15trigger_event_ejEPKcP6Engine
        EXTERN _Z23prepareHip9011RpmLookupf
        EXTERN _Z24getIntegrationIndexByRpmf
        EXTERN _Z8getHwPin11brain_pin_e
        EXTERN _Z9getHwPort11brain_pin_e
        EXTERN _ZN12scheduling_sC1Ev
        EXTERN _ZN14NamedOutputPinC1EPKc
        EXTERN _ZN6Engine10knockLogicEf
        EXTERN _ZN6Engine15printKnockStateEv
        EXTERN _ZN9OutputPinC1Ev
        EXTERN __aeabi_f2d
        EXTERN __aeabi_vec_ctor_nocookie_nodtor
        EXTERN addConsoleAction
        EXTERN addConsoleActionF
        EXTERN addConsoleActionI
        EXTERN boardConfiguration
        EXTERN chThdCreateStatic
        EXTERN chThdSleep
        EXTERN engine
        EXTERN engineConfiguration
        EXTERN lastExecutionCount
        EXTERN maxF
        EXTERN rlist
        EXTERN scheduleMsg
        EXTERN spiExchange
        EXTERN spiSelect
        EXTERN spiStart
        EXTERN spiStop
        EXTERN spiUnselect
        EXTERN spi_lld_exchange
        EXTERN spi_lld_select
        EXTERN spi_lld_unselect

        PUBLIC _Z11initHip9011P7Logging
        PUBLIC _Z14hipAdcCallbackt
        PUBLIC _Z25setHip9011FrankensoPinoutv
        PUBLIC hipLastExecutionCount
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\hw_layer\HIP9011.cpp
//    1 /**
//    2  * @file	HIP9011.cpp
//    3  * @brief	HIP9011/TPIC8101 driver
//    4  *
//    5  *	pin1	VDD
//    6  *	pin2	GND
//    7  *
//    8  *	pin8	Chip Select - CS
//    9  *	pin11	Slave Data Out - MISO-
//   10  *	pin12	Slave Data In - MOSI
//   11  *	pin13	SPI clock - SCLK
//   12  *
//   13  *
//   14  *
//   15  * http://www.ti.com/lit/ds/symlink/tpic8101.pdf
//   16  * http://www.intersil.com/content/dam/Intersil/documents/hip9/hip9011.pdf
//   17  * http://www.intersil.com/content/dam/Intersil/documents/an97/an9770.pdf
//   18  * http://e2e.ti.com/cfs-file/__key/telligent-evolution-components-attachments/00-26-01-00-00-42-36-40/TPIC8101-Training.pdf
//   19  *
//   20  * max SPI frequency: 5MHz max
//   21  *
//   22  * @date Nov 27, 2013
//   23  * @author Andrey Belomutskiy, (c) 2012-2015
//   24  * @Spilly
//   25  */
//   26 
//   27 #include "main.h"
//   28 #include "engine.h"
//   29 #include "settings.h"
//   30 #include "pin_repository.h"
//   31 #include "hardware.h"
//   32 #include "rpm_calculator.h"
//   33 #include "trigger_central.h"
//   34 #include "hip9011_lookup.h"
//   35 #include "HIP9011.h"
//   36 #include "adc_inputs.h"
//   37 #include "efilib2.h"
//   38 #include "engine_controller.h"
//   39 
//   40 #if EFI_HIP_9011 || defined(__DOXYGEN__)
//   41 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function __sti__routine
        THUMB
// static __intrinsic __interwork __softfp void __sti__routine()
__sti__routine:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   42 static NamedOutputPin intHold("HIP");
        ADR.N    R1,??DataTable2  ;; "HIP"
        LDR.W    R0,??DataTable16
          CFI FunCall _ZN14NamedOutputPinC1EPKc
        BL       _ZN14NamedOutputPinC1EPKc
//   43 static OutputPin hipCs;
        LDR.W    R0,??DataTable16_1
          CFI FunCall _ZN9OutputPinC1Ev
        BL       _ZN9OutputPinC1Ev
//   44 
//   45 extern pin_output_mode_e DEFAULT_OUTPUT;
//   46 
//   47 extern uint32_t lastExecutionCount;
//   48 
//   49 uint32_t hipLastExecutionCount;
//   50 
//   51 /**
//   52  * band index is only send to HIP chip on startup
//   53  */
//   54 static int currentBandIndex;
//   55 static int currentGainIndex = -1;
//   56 static int currentIntergratorIndex = -1;
//   57 static int settingUpdateCount = 0;
//   58 static int totalKnockEventsCount = 0;
//   59 static int currentPrescaler;
//   60 static float hipValueMax = 0;
//   61 static float knockVolts = 0;
//   62 static int spiCount = 0;
//   63 
//   64 static unsigned char tx_buff[1];
//   65 static unsigned char rx_buff[1];
//   66 static int nonZeroResponse = 0;
//   67 static char pinNameBuffer[16];
//   68 static float currentAngleWindowWidth;
//   69 
//   70 /**
//   71  * Int/Hold pin is controlled from scheduler callbacks which are set according to current RPM
//   72  *
//   73  * The following state makes sure that we only have SPI communication while not integrating and that we take
//   74  * a good ADC reading after integrating.
//   75  *
//   76  * Once integtation window is over, we wait for the 2nd ADC callback and then initiate SPI communication if needed
//   77  *
//   78  * hipOutput should be set to used FAST adc device
//   79  */
//   80 static hip_state_e state = NOT_READY;
//   81 
//   82 static scheduling_s startTimer[2];
        MOVS     R3,#+2
        MOVS     R2,#+24
        LDR.W    R1,??DataTable16_2
        LDR.W    R0,??DataTable16_3
          CFI FunCall __aeabi_vec_ctor_nocookie_nodtor
        BL       __aeabi_vec_ctor_nocookie_nodtor
//   83 static scheduling_s endTimer[2];
        MOVS     R3,#+2
        MOVS     R2,#+24
        LDR.W    R1,??DataTable16_2
        LDR.W    R0,??DataTable16_4
          CFI FunCall __aeabi_vec_ctor_nocookie_nodtor
        BL       __aeabi_vec_ctor_nocookie_nodtor
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock0

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
intHold:
        DS8 20

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
hipCs:
        DS8 16

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
hipLastExecutionCount:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
currentBandIndex:
        DS8 4

        SECTION `.data`:DATA:REORDER:NOROOT(2)
currentGainIndex:
        DATA
        DC32 -1

        SECTION `.data`:DATA:REORDER:NOROOT(2)
currentIntergratorIndex:
        DATA
        DC32 -1

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
settingUpdateCount:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
currentPrescaler:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
hipValueMax:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
knockVolts:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
spiCount:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(0)
tx_buff:
        DS8 1

        SECTION `.bss`:DATA:REORDER:NOROOT(0)
rx_buff:
        DS8 1

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
nonZeroResponse:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
pinNameBuffer:
        DS8 16

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
currentAngleWindowWidth:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(0)
state:
        DS8 1

        SECTION `.bss`:DATA:REORDER:NOROOT(3)
startTimer:
        DS8 48

        SECTION `.bss`:DATA:REORDER:NOROOT(3)
endTimer:
        DS8 48
//   84 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   85 static Logging *logger;
logger:
        DS8 4
//   86 
//   87 // SPI_CR1_BR_1 // 5MHz
//   88 // SPI_CR1_CPHA Clock Phase
//   89 // todo: nicer method which would mention SPI speed explicitly?
//   90 

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//   91 static SPIConfig spicfg = { NULL,
spicfg:
        DATA
        DC32 0H, 0H
        DC16 0, 61
//   92 /* HW dependent part.*/
//   93 NULL, 0,
//   94 SPI_CR1_MSTR |
//   95 //SPI_CR1_BR_1 // 5MHz
//   96 		SPI_CR1_CPHA | SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_BR_2 };
//   97 
//   98 #define SPI_SYNCHRONOUS(value) \ 
//   99 	spiSelect(driver); \ 
//  100 	tx_buff[0] = value; \ 
//  101 	spiExchange(driver, 1, tx_buff, rx_buff); \ 
//  102 	spiUnselect(driver); \ 
//  103 	if (rx_buff[0] != 0) nonZeroResponse++;
//  104 
//  105 // todo: make this configurable

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//  106 static SPIDriver *driver = &SPID2;
driver:
        DATA
        DC32 SPID2
//  107 
//  108 EXTERN_ENGINE
//  109 ;
//  110 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z7getBandv
          CFI NoCalls
        THUMB
//  111 static float getBand(void) {
//  112 	return engineConfiguration->knockBandCustom == 0 ?
//  113 			BAND(engineConfiguration->cylinderBore) : engineConfiguration->knockBandCustom;
_Z7getBandv:
        LDR.W    R0,??DataTable16_5
        LDR      R0,[R0, #+0]
        VLDR     S0,[R0, #+332]
        VCMP.F32 S0,#0.0
        FMSTAT   
        BNE.N    ??getBand_0
        LDR.W    R0,??DataTable16_5
        LDR      R0,[R0, #+0]
        VLDR     S0,[R0, #+412]
        VLDR.W   S1,??DataTable3  ;; 0x40490fd0
        VMUL.F32 S0,S0,S1
        VMOV.F32 S1,#2.0
        VDIV.F32 S0,S0,S1
        VLDR.W   S1,??DataTable3_1  ;; 0x44610000
        VDIV.F32 S0,S1,S0
        B.N      ??getBand_1
??getBand_0:
        LDR.W    R0,??DataTable16_5
        LDR      R0,[R0, #+0]
        VLDR     S0,[R0, #+332]
??getBand_1:
        BX       LR               ;; return
          CFI EndBlock cfiBlock1
//  114 }
//  115 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z11showHipInfov
        THUMB
//  116 static void showHipInfo(void) {
_Z11showHipInfov:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        SUB      SP,SP,#+24
          CFI CFA R13+40
//  117 	if (!boardConfiguration->isHip9011Enabled) {
        LDR.W    R0,??DataTable16_6
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+384]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+6
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BNE.N    ??showHipInfo_0
//  118 		scheduleMsg(logger, "hip9011 driver not active");
        LDR.W    R1,??DataTable16_7
        LDR.W    R0,??DataTable16_8
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  119 		return;
        B.N      ??showHipInfo_1
//  120 	}
//  121 
//  122 	printSpiState(logger, boardConfiguration);
??showHipInfo_0:
        LDR.W    R0,??DataTable16_6
        LDR      R1,[R0, #+0]
        LDR.W    R0,??DataTable16_8
        LDR      R0,[R0, #+0]
          CFI FunCall _Z13printSpiStateP7LoggingP21board_configuration_s
        BL       _Z13printSpiStateP7LoggingP21board_configuration_s
//  123 	scheduleMsg(logger, "enabled=%s state=%d bore=%fmm freq=%fkHz PaSDO=%d",
//  124 			boolToString(boardConfiguration->isHip9011Enabled),
//  125 			state,
//  126 			engineConfiguration->cylinderBore, getBand(),
//  127 			engineConfiguration->hip9011PrescalerAndSDO);
          CFI FunCall _Z7getBandv
        BL       _Z7getBandv
        VMOV.F32 S16,S0
        LDR.W    R0,??DataTable16_6
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+384]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+6
        ANDS     R0,R0,#0x1
          CFI FunCall _Z12boolToStringb
        BL       _Z12boolToStringb
        MOVS     R4,R0
        LDR.W    R0,??DataTable16_5
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+328]
        STR      R0,[SP, #+16]
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+8]
        LDR.W    R0,??DataTable16_5
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+412]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        LDR.W    R0,??DataTable16_9
        LDRB     R3,[R0, #+0]
        MOVS     R2,R4
        LDR.W    R1,??DataTable16_10
        LDR.W    R0,??DataTable16_8
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  128 
//  129 	scheduleMsg(logger, "band_index=%d gain %f/index=%d", currentBandIndex, boardConfiguration->hip9011Gain, currentGainIndex);
        LDR.W    R0,??DataTable16_11
        LDR      R0,[R0, #+0]
        STR      R0,[SP, #+8]
        LDR.W    R0,??DataTable16_6
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+760]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        LDR.W    R0,??DataTable16_12
        LDR      R2,[R0, #+0]
        LDR.W    R1,??DataTable16_13
        LDR.W    R0,??DataTable16_8
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  130 	scheduleMsg(logger, "integrator index=%d knockVThreshold=%f knockCount=%d maxKnockSubDeg=%f",
//  131 	            currentIntergratorIndex, engineConfiguration->knockVThreshold,
//  132 	            engine->knockCount, engineConfiguration->maxKnockSubDeg);
        LDR.W    R0,??DataTable16_5
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+576]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+16]
        LDR.W    R0,??DataTable16_14
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+620]
        STR      R0,[SP, #+8]
        LDR.W    R0,??DataTable16_5
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+1552]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        LDR.W    R0,??DataTable16_15
        LDR      R2,[R0, #+0]
        LDR.W    R1,??DataTable16_16
        LDR.W    R0,??DataTable16_8
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  133 
//  134 	scheduleMsg(logger, "spi= IntHold@%s response count=%d", hwPortname(boardConfiguration->hip9011IntHoldPin),
//  135 			nonZeroResponse);
        LDR.W    R0,??DataTable16_6
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+652]
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        LDR.W    R1,??DataTable16_17
        LDR      R3,[R1, #+0]
        MOVS     R2,R0
        LDR.W    R1,??DataTable16_18
        LDR.W    R0,??DataTable16_8
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  136 	scheduleMsg(logger, "CS@%s updateCount=%d", hwPortname(boardConfiguration->hip9011CsPin), settingUpdateCount);
        LDR.W    R0,??DataTable16_6
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+648]
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R2,R0
        LDR.W    R0,??DataTable16_19
        LDR      R3,[R0, #+0]
        LDR.W    R1,??DataTable16_20
        LDR.W    R0,??DataTable16_8
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  137 
//  138 	scheduleMsg(logger, "hip %fv@%s/max=%f spiCount=%d adv=%d",
//  139 			knockVolts,
//  140 			getPinNameByAdcChannel(engineConfiguration->hipOutputChannel, pinNameBuffer),
//  141 			hipValueMax,
//  142 			spiCount, boardConfiguration->useTpicAdvancedMode);
        LDR.W    R1,??DataTable16_21
        LDR.W    R0,??DataTable16_5
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+1500]
          CFI FunCall _Z22getPinNameByAdcChannel13adc_channel_ePc
        BL       _Z22getPinNameByAdcChannel13adc_channel_ePc
        MOVS     R4,R0
        LDR.W    R0,??DataTable16_6
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+385]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+3
        ANDS     R0,R0,#0x1
        STR      R0,[SP, #+20]
        LDR.W    R0,??DataTable16_22
        LDR      R0,[R0, #+0]
        STR      R0,[SP, #+16]
        LDR.W    R0,??DataTable16_23
        LDR      R0,[R0, #+0]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+8]
        STR      R4,[SP, #+0]
        LDR.W    R0,??DataTable16_24
        LDR      R0,[R0, #+0]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.W    R1,??DataTable16_25
        LDR.W    R0,??DataTable16_8
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  143 
//  144 	scheduleMsg(logger, "start %f end %f", engineConfiguration->knockDetectionWindowStart,
//  145 			engineConfiguration->knockDetectionWindowEnd);
        LDR.W    R0,??DataTable16_5
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+1544]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        LDR.W    R0,??DataTable16_5
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+1540]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.W    R1,??DataTable16_26
        LDR.W    R0,??DataTable16_8
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  146 
//  147 	hipValueMax = 0;
        LDR.W    R0,??DataTable16_23
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//  148 	engine->printKnockState();
        LDR.W    R0,??DataTable16_14
        LDR      R0,[R0, #+0]
          CFI FunCall _ZN6Engine15printKnockStateEv
        BL       _ZN6Engine15printKnockStateEv
//  149 }
??showHipInfo_1:
        ADD      SP,SP,#+24
          CFI CFA R13+16
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock2

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2:
        DC8      "HIP"
//  150 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z25setHip9011FrankensoPinoutv
          CFI NoCalls
        THUMB
//  151 void setHip9011FrankensoPinout(void) {
//  152 	/**
//  153 	 * SPI on PB13/14/15
//  154 	 */
//  155 	boardConfiguration->isHip9011Enabled = true;
_Z25setHip9011FrankensoPinoutv:
        LDR.W    R0,??DataTable16_6
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+384]
        ORRS     R0,R0,#0x40
        LDR.W    R1,??DataTable16_6
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+384]
//  156 	boardConfiguration->hip9011CsPin = GPIOD_0;
        LDR.W    R0,??DataTable16_6
        LDR      R0,[R0, #+0]
        MOVS     R1,#+48
        STR      R1,[R0, #+648]
//  157 	boardConfiguration->hip9011IntHoldPin = GPIOB_11;
        LDR.W    R0,??DataTable16_6
        LDR      R0,[R0, #+0]
        MOVS     R1,#+27
        STR      R1,[R0, #+652]
//  158 	boardConfiguration->is_enabled_spi_2 = true;
        LDR.W    R0,??DataTable16_6
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+384]
        ORRS     R0,R0,#0x2
        LDR.W    R1,??DataTable16_6
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+384]
//  159 
//  160 	boardConfiguration->hip9011Gain = 1;
        LDR.W    R0,??DataTable16_6
        LDR      R0,[R0, #+0]
        MOVS     R1,#+1065353216
        STR      R1,[R0, #+760]
//  161 	engineConfiguration->knockVThreshold = 4;
        LDR.W    R0,??DataTable16_5
        LDR      R0,[R0, #+0]
        MOVS     R1,#+1082130432
        STR      R1,[R0, #+1552]
//  162 	engineConfiguration->maxKnockSubDeg = 20;
        LDR.W    R0,??DataTable16_5
        LDR      R0,[R0, #+0]
        LDR.W    R1,??DataTable16_27  ;; 0x41a00000
        STR      R1,[R0, #+576]
//  163 
//  164 
//  165 	if (!boardConfiguration->useTpicAdvancedMode) {
        LDR.W    R0,??DataTable16_6
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+385]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+3
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BNE.N    ??setHip9011FrankensoPinout_0
//  166 	    engineConfiguration->hipOutputChannel = EFI_ADC_10; // PC0
        LDR.W    R0,??DataTable16_5
        LDR      R0,[R0, #+0]
        MOVS     R1,#+10
        STR      R1,[R0, #+1500]
//  167 	}
//  168 }
??setHip9011FrankensoPinout_0:
        BX       LR               ;; return
          CFI EndBlock cfiBlock3

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3:
        DC32     0x40490fd0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_1:
        DC32     0x44610000
//  169 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _Z16startIntegrationv
        THUMB
//  170 static void startIntegration(void) {
_Z16startIntegrationv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  171 	if (state == READY_TO_INTEGRATE) {
        LDR.W    R0,??DataTable16_9
        LDRB     R0,[R0, #+0]
        CMP      R0,#+1
        BNE.N    ??startIntegration_0
//  172 		/**
//  173 		 * SPI communication is only allowed while not integrating, so we postpone the exchange
//  174 		 * until we are done integrating
//  175 		 */
//  176 		state = IS_INTEGRATING;
        LDR.W    R0,??DataTable16_9
        MOVS     R1,#+2
        STRB     R1,[R0, #+0]
//  177 		turnPinHigh(&intHold);
        LDR.W    R0,??DataTable16
          CFI FunCall _Z11turnPinHighP14NamedOutputPin
        BL       _Z11turnPinHighP14NamedOutputPin
//  178 	}
//  179 }
??startIntegration_0:
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock4
//  180 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _Z14endIntegrationv
        THUMB
//  181 static void endIntegration(void) {
_Z14endIntegrationv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  182 	/**
//  183 	 * isIntegrating could be 'false' if an SPI command was pending thus we did not integrate during this
//  184 	 * engine cycle
//  185 	 */
//  186 	if (state == IS_INTEGRATING) {
        LDR.W    R0,??DataTable16_9
        LDRB     R0,[R0, #+0]
        CMP      R0,#+2
        BNE.N    ??endIntegration_0
//  187 		turnPinLow(&intHold);
        LDR.W    R0,??DataTable16
          CFI FunCall _Z10turnPinLowP14NamedOutputPin
        BL       _Z10turnPinLowP14NamedOutputPin
//  188 		state = WAITING_FOR_ADC_TO_SKIP;
        LDR.W    R0,??DataTable16_9
        MOVS     R1,#+3
        STRB     R1,[R0, #+0]
//  189 	}
//  190 }
??endIntegration_0:
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock5
//  191 
//  192 /**
//  193  * Shaft Position callback used to start or finish HIP integration
//  194  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _Z15intHoldCallback15trigger_event_ej
        THUMB
//  195 static void intHoldCallback(trigger_event_e ckpEventType, uint32_t index DECLARE_ENGINE_PARAMETER_S) {
_Z15intHoldCallback15trigger_event_ej:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
//  196 	// this callback is invoked on interrupt thread
//  197 	engine->m.beforeHipCb = GET_TIMESTAMP();
        LDR.W    R0,??DataTable16_14
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+9216
        LDR.W    R1,??DataTable16_28  ;; 0xe0001004
        LDR      R1,[R1, #+0]
        STR      R1,[R0, #+0]
//  198 	if (index != 0)
        CMP      R5,#+0
        BNE.N    ??intHoldCallback_0
//  199 		return;
//  200 
//  201 	int rpm = engine->rpmCalculator.rpmValue;
??intHoldCallback_1:
        LDR.W    R0,??DataTable16_14
        LDR      R0,[R0, #+0]
        LDR      R6,[R0, #+0]
//  202 	if (!isValidRpm(rpm))
        CMP      R6,#+1
        BLT.N    ??intHoldCallback_2
        MOVW     R0,#+30000
        CMP      R6,R0
        BLT.N    ??intHoldCallback_3
//  203 		return;
??intHoldCallback_2:
        B.N      ??intHoldCallback_0
//  204 
//  205 	int structIndex = getRevolutionCounter() % 2;
??intHoldCallback_3:
          CFI FunCall _Z20getRevolutionCounterv
        BL       _Z20getRevolutionCounterv
        MOVS     R1,#+2
        SDIV     R7,R0,R1
        MLS      R7,R7,R1,R0
//  206 	// todo: schedule this based on closest trigger event, same as ignition works
//  207 	scheduleByAngle(rpm, &startTimer[structIndex], engineConfiguration->knockDetectionWindowStart,
//  208 			(schfunc_t) &startIntegration, NULL, &engine->rpmCalculator);
        LDR.W    R0,??DataTable16_14
        LDR      R0,[R0, #+0]
        STR      R0,[SP, #+0]
        MOVS     R3,#+0
        LDR.W    R2,??DataTable16_29
        LDR.W    R0,??DataTable16_5
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+1540]
        VMOV     S0,R0
        MOVS     R0,#+24
        LDR.W    R1,??DataTable16_3
        MLA      R1,R0,R7,R1
        MOVS     R0,R6
          CFI FunCall _Z15scheduleByAngleiP12scheduling_sfPFvPvES1_P13RpmCalculator
        BL       _Z15scheduleByAngleiP12scheduling_sfPFvPvES1_P13RpmCalculator
//  209 	hipLastExecutionCount = lastExecutionCount;
        LDR.W    R0,??DataTable16_30
        LDR.W    R1,??DataTable16_31
        LDR      R1,[R1, #+0]
        STR      R1,[R0, #+0]
//  210 	scheduleByAngle(rpm, &endTimer[structIndex], engineConfiguration->knockDetectionWindowEnd,
//  211 			(schfunc_t) &endIntegration,
//  212 			NULL, &engine->rpmCalculator);
        LDR.W    R0,??DataTable16_14
        LDR      R0,[R0, #+0]
        STR      R0,[SP, #+0]
        MOVS     R3,#+0
        LDR.W    R2,??DataTable16_32
        LDR.W    R0,??DataTable16_5
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+1544]
        VMOV     S0,R0
        MOVS     R0,#+24
        LDR.W    R1,??DataTable16_4
        MLA      R1,R0,R7,R1
        MOVS     R0,R6
          CFI FunCall _Z15scheduleByAngleiP12scheduling_sfPFvPvES1_P13RpmCalculator
        BL       _Z15scheduleByAngleiP12scheduling_sfPFvPvES1_P13RpmCalculator
//  213 	engine->m.hipCbTime = GET_TIMESTAMP() - engine->m.beforeHipCb;
        LDR.W    R0,??DataTable16_28  ;; 0xe0001004
        LDR      R0,[R0, #+0]
        LDR.W    R1,??DataTable16_14
        LDR      R1,[R1, #+0]
        ADDS     R1,R1,#+9216
        LDR      R1,[R1, #+0]
        SUBS     R0,R0,R1
        MOVW     R1,#+9220
        LDR.W    R2,??DataTable16_14
        LDR      R2,[R2, #+0]
        STR      R0,[R1, R2]
//  214 }
??intHoldCallback_0:
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock6
//  215 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function _Z17setMaxKnockSubDegi
        THUMB
//  216 static void setMaxKnockSubDeg(int value){
_Z17setMaxKnockSubDegi:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        VMOV     S16,R0
//  217     engineConfiguration->maxKnockSubDeg = value;
        VCVT.F32.S32 S0,S16
        LDR.W    R0,??DataTable16_5
        LDR      R0,[R0, #+0]
        VSTR     S0,[R0, #+576]
//  218     showHipInfo();
          CFI FunCall _Z11showHipInfov
        BL       _Z11showHipInfov
//  219 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock7
//  220 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function _Z14setKnockThreshf
        THUMB
//  221 static void setKnockThresh(float value){
_Z14setKnockThreshf:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        VMOV.F32 S16,S0
//  222     engineConfiguration->knockVThreshold = value;
        LDR.W    R0,??DataTable16_5
        LDR      R0,[R0, #+0]
        ADDW     R0,R0,#+1552
        VSTR     S16,[R0, #0]
//  223     showHipInfo();
          CFI FunCall _Z11showHipInfov
        BL       _Z11showHipInfov
//  224 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock8
//  225 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function _Z18setPrescalerAndSDOi
          CFI NoCalls
        THUMB
//  226 static void setPrescalerAndSDO(int value) {
//  227 	engineConfiguration->hip9011PrescalerAndSDO = value;
_Z18setPrescalerAndSDOi:
        LDR.W    R1,??DataTable16_5
        LDR      R1,[R1, #+0]
        STR      R0,[R1, #+328]
//  228 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock9
//  229 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function _Z7setBandf
        THUMB
//  230 static void setBand(float value) {
_Z7setBandf:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        VMOV.F32 S16,S0
//  231 	engineConfiguration->knockBandCustom = value;
        LDR.W    R0,??DataTable16_5
        LDR      R0,[R0, #+0]
        VSTR     S16,[R0, #+332]
//  232 	showHipInfo();
          CFI FunCall _Z11showHipInfov
        BL       _Z11showHipInfov
//  233 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock10
//  234 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function _Z7setGainf
        THUMB
//  235 static void setGain(float value) {
_Z7setGainf:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        VMOV.F32 S16,S0
//  236 	boardConfiguration->hip9011Gain = value;
        LDR.W    R0,??DataTable16_6
        LDR      R0,[R0, #+0]
        VSTR     S16,[R0, #+760]
//  237 	showHipInfo();
          CFI FunCall _Z11showHipInfov
        BL       _Z11showHipInfov
//  238 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock11
//  239 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function _Z16endOfSpiExchangeP9SPIDriver
        THUMB
//  240 static void endOfSpiExchange(SPIDriver *spip) {
_Z16endOfSpiExchangeP9SPIDriver:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  241 	spiUnselectI(driver);
        LDR.W    R0,??DataTable16_33
        LDR      R0,[R0, #+0]
          CFI FunCall spi_lld_unselect
        BL       spi_lld_unselect
//  242 	state = READY_TO_INTEGRATE;
        LDR.W    R0,??DataTable16_9
        MOVS     R1,#+1
        STRB     R1,[R0, #+0]
//  243 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock12
//  244 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock13 Using cfiCommon0
          CFI Function _Z12getBandIndexv
        THUMB
//  245 static int getBandIndex(void) {
_Z12getBandIndexv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
//  246 	float freq = getBand();
          CFI FunCall _Z7getBandv
        BL       _Z7getBandv
        VMOV.F32 S16,S0
//  247 	return getHip9011BandIndex(freq);
        VMOV.F32 S0,S16
          CFI FunCall _Z19getHip9011BandIndexf
        BL       _Z19getHip9011BandIndexf
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R1,PC}          ;; return
          CFI EndBlock cfiBlock13
//  248 }
//  249 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock14 Using cfiCommon0
          CFI Function _Z14hipAdcCallbackt
        THUMB
//  250 void hipAdcCallback(adcsample_t value) {
_Z14hipAdcCallbackt:
        PUSH     {R4-R8,LR}
          CFI R14 Frame(CFA, -4)
          CFI R8 Frame(CFA, -8)
          CFI R7 Frame(CFA, -12)
          CFI R6 Frame(CFA, -16)
          CFI R5 Frame(CFA, -20)
          CFI R4 Frame(CFA, -24)
          CFI CFA R13+24
        VPUSH    {D8}
          CFI D8 Frame(CFA, -32)
          CFI CFA R13+32
        MOVS     R4,R0
//  251 	if (state == WAITING_FOR_ADC_TO_SKIP) {
        LDR.W    R0,??DataTable16_9
        LDRB     R0,[R0, #+0]
        CMP      R0,#+3
        BNE.N    ??hipAdcCallback_0
//  252 		state = WAITING_FOR_RESULT_ADC;
        LDR.W    R0,??DataTable16_9
        MOVS     R1,#+4
        STRB     R1,[R0, #+0]
        B.N      ??hipAdcCallback_1
//  253 	} else if (state == WAITING_FOR_RESULT_ADC) {
??hipAdcCallback_0:
        LDR.W    R0,??DataTable16_9
        LDRB     R0,[R0, #+0]
        CMP      R0,#+4
        BNE.W    ??hipAdcCallback_1
//  254 		knockVolts = adcToVoltsDivided(value);
        UXTH     R4,R4            ;; ZeroExt  R4,R4,#+16,#+16
        VMOV     S0,R4
        VCVT.F32.U32 S0,S0
        VLDR.W   S1,??DataTable13  ;; 0x3a400c01
        VMUL.F32 S0,S0,S1
        LDR.W    R0,??DataTable16_5
        LDR      R0,[R0, #+0]
        VLDR     S1,[R0, #+464]
        VMUL.F32 S0,S0,S1
        LDR.W    R0,??DataTable16_24
        VSTR     S0,[R0, #0]
//  255 		hipValueMax = maxF(knockVolts, hipValueMax);
        LDR.W    R0,??DataTable16_23
        VLDR     S1,[R0, #0]
        LDR.W    R0,??DataTable16_24
        VLDR     S0,[R0, #0]
          CFI FunCall maxF
        BL       maxF
        LDR.W    R0,??DataTable16_23
        VSTR     S0,[R0, #0]
//  256 		engine->knockLogic(knockVolts);
        LDR.W    R0,??DataTable16_24
        VLDR     S0,[R0, #0]
        LDR.W    R0,??DataTable16_14
        LDR      R0,[R0, #+0]
          CFI FunCall _ZN6Engine10knockLogicEf
        BL       _ZN6Engine10knockLogicEf
//  257 
//  258 		float angleWindowWidth =
//  259 		engineConfiguration->knockDetectionWindowEnd - engineConfiguration->knockDetectionWindowStart;
        LDR.W    R0,??DataTable16_5
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+1544]
        VMOV     S0,R0
        LDR.W    R0,??DataTable16_5
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+1540]
        VMOV     S1,R0
        VSUB.F32 S16,S0,S1
//  260 
//  261 		if (angleWindowWidth != currentAngleWindowWidth) {
        LDR.W    R0,??DataTable16_34
        VLDR     S0,[R0, #0]
        VCMP.F32 S16,S0
        FMSTAT   
        BEQ.N    ??hipAdcCallback_2
//  262 			currentAngleWindowWidth = angleWindowWidth;
        LDR.W    R0,??DataTable16_34
        VSTR     S16,[R0, #0]
//  263 		prepareHip9011RpmLookup(currentAngleWindowWidth);
        LDR.W    R0,??DataTable16_34
        VLDR     S0,[R0, #0]
          CFI FunCall _Z23prepareHip9011RpmLookupf
        BL       _Z23prepareHip9011RpmLookupf
//  264 		}
//  265 
//  266 		int integratorIndex = getIntegrationIndexByRpm(engine->rpmCalculator.rpmValue);
??hipAdcCallback_2:
        LDR.W    R0,??DataTable16_14
        LDR      R0,[R0, #+0]
        VLDR     S0,[R0, #0]
        VCVT.F32.S32 S0,S0
          CFI FunCall _Z24getIntegrationIndexByRpmf
        BL       _Z24getIntegrationIndexByRpmf
        MOVS     R5,R0
//  267 		int gainIndex = getHip9011GainIndex(boardConfiguration->hip9011Gain);
        LDR.W    R0,??DataTable16_6
        LDR      R0,[R0, #+0]
        VLDR     S0,[R0, #+760]
          CFI FunCall _Z19getHip9011GainIndexf
        BL       _Z19getHip9011GainIndexf
        MOVS     R6,R0
//  268 		int bandIndex = getBandIndex();
          CFI FunCall _Z12getBandIndexv
        BL       _Z12getBandIndexv
        MOVS     R7,R0
//  269 		int prescalerIndex = engineConfiguration->hip9011PrescalerAndSDO;
        LDR.W    R0,??DataTable16_5
        LDR      R0,[R0, #+0]
        LDR      R8,[R0, #+328]
//  270 
//  271 
//  272 		if (currentGainIndex != gainIndex) {
        LDR.W    R0,??DataTable16_11
        LDR      R0,[R0, #+0]
        CMP      R0,R6
        BEQ.N    ??hipAdcCallback_3
//  273 			currentGainIndex = gainIndex;
        LDR.W    R0,??DataTable16_11
        STR      R6,[R0, #+0]
//  274 			tx_buff[0] = SET_GAIN_CMD + gainIndex;
        MOVS     R0,R6
        SUBS     R0,R0,#+128
        LDR.W    R1,??DataTable16_35
        STRB     R0,[R1, #+0]
//  275 
//  276 			state = IS_SENDING_SPI_COMMAND;
        LDR.W    R0,??DataTable16_9
        MOVS     R1,#+5
        STRB     R1,[R0, #+0]
//  277 			spiSelectI(driver);
        LDR.W    R0,??DataTable16_33
        LDR      R0,[R0, #+0]
          CFI FunCall spi_lld_select
        BL       spi_lld_select
//  278 			spiStartExchangeI(driver, 1, tx_buff, rx_buff);
        LDR.W    R0,??DataTable16_33
        LDR      R0,[R0, #+0]
        MOVS     R1,#+3
        STRB     R1,[R0, #+0]
        LDR.W    R3,??DataTable16_36
        LDR.W    R2,??DataTable16_35
        MOVS     R1,#+1
        LDR.W    R0,??DataTable16_33
        LDR      R0,[R0, #+0]
          CFI FunCall spi_lld_exchange
        BL       spi_lld_exchange
        B.N      ??hipAdcCallback_1
//  279 		} else if (currentIntergratorIndex != integratorIndex) {
??hipAdcCallback_3:
        LDR.W    R0,??DataTable16_15
        LDR      R0,[R0, #+0]
        CMP      R0,R5
        BEQ.N    ??hipAdcCallback_4
//  280 			currentIntergratorIndex = integratorIndex;
        LDR.W    R0,??DataTable16_15
        STR      R5,[R0, #+0]
//  281 			tx_buff[0] = SET_INTEGRATOR_CMD + integratorIndex;
        MOVS     R0,R5
        SUBS     R0,R0,#+64
        LDR.W    R1,??DataTable16_35
        STRB     R0,[R1, #+0]
//  282 
//  283 			state = IS_SENDING_SPI_COMMAND;
        LDR.W    R0,??DataTable16_9
        MOVS     R1,#+5
        STRB     R1,[R0, #+0]
//  284 			spiSelectI(driver);
        LDR.W    R0,??DataTable16_33
        LDR      R0,[R0, #+0]
          CFI FunCall spi_lld_select
        BL       spi_lld_select
//  285 			spiStartExchangeI(driver, 1, tx_buff, rx_buff);
        LDR.W    R0,??DataTable16_33
        LDR      R0,[R0, #+0]
        MOVS     R1,#+3
        STRB     R1,[R0, #+0]
        LDR.W    R3,??DataTable16_36
        LDR.W    R2,??DataTable16_35
        MOVS     R1,#+1
        LDR.W    R0,??DataTable16_33
        LDR      R0,[R0, #+0]
          CFI FunCall spi_lld_exchange
        BL       spi_lld_exchange
        B.N      ??hipAdcCallback_1
//  286 		} else if (currentBandIndex != bandIndex) {
??hipAdcCallback_4:
        LDR.W    R0,??DataTable16_12
        LDR      R0,[R0, #+0]
        CMP      R0,R7
        BEQ.N    ??hipAdcCallback_5
//  287 			currentBandIndex = bandIndex;
        LDR.W    R0,??DataTable16_12
        STR      R7,[R0, #+0]
//  288 			tx_buff[0] = SET_BAND_PASS_CMD + bandIndex;
        LDR.W    R0,??DataTable16_35
        STRB     R7,[R0, #+0]
//  289 
//  290 			state = IS_SENDING_SPI_COMMAND;
        LDR.N    R0,??DataTable16_9
        MOVS     R1,#+5
        STRB     R1,[R0, #+0]
//  291 			spiSelectI(driver);
        LDR.W    R0,??DataTable16_33
        LDR      R0,[R0, #+0]
          CFI FunCall spi_lld_select
        BL       spi_lld_select
//  292 			spiStartExchangeI(driver, 1, tx_buff, rx_buff);
        LDR.W    R0,??DataTable16_33
        LDR      R0,[R0, #+0]
        MOVS     R1,#+3
        STRB     R1,[R0, #+0]
        LDR.W    R3,??DataTable16_36
        LDR.W    R2,??DataTable16_35
        MOVS     R1,#+1
        LDR.W    R0,??DataTable16_33
        LDR      R0,[R0, #+0]
          CFI FunCall spi_lld_exchange
        BL       spi_lld_exchange
        B.N      ??hipAdcCallback_1
//  293 		} else if (currentPrescaler != prescalerIndex) {
??hipAdcCallback_5:
        LDR.W    R0,??DataTable16_37
        LDR      R0,[R0, #+0]
        CMP      R0,R8
        BEQ.N    ??hipAdcCallback_6
//  294 			currentPrescaler = prescalerIndex;
        LDR.W    R0,??DataTable16_37
        STR      R8,[R0, #+0]
//  295 			tx_buff[0] = SET_PRESCALER_CMD + prescalerIndex;
        ADDS     R0,R8,#+64
        LDR.W    R1,??DataTable16_35
        STRB     R0,[R1, #+0]
//  296 
//  297 			state = IS_SENDING_SPI_COMMAND;
        LDR.N    R0,??DataTable16_9
        MOVS     R1,#+5
        STRB     R1,[R0, #+0]
//  298 			spiSelectI(driver);
        LDR.N    R0,??DataTable16_33
        LDR      R0,[R0, #+0]
          CFI FunCall spi_lld_select
        BL       spi_lld_select
//  299 			spiStartExchangeI(driver, 1, tx_buff, rx_buff);
        LDR.N    R0,??DataTable16_33
        LDR      R0,[R0, #+0]
        MOVS     R1,#+3
        STRB     R1,[R0, #+0]
        LDR.N    R3,??DataTable16_36
        LDR.N    R2,??DataTable16_35
        MOVS     R1,#+1
        LDR.N    R0,??DataTable16_33
        LDR      R0,[R0, #+0]
          CFI FunCall spi_lld_exchange
        BL       spi_lld_exchange
        B.N      ??hipAdcCallback_1
//  300 		} else {
//  301 			state = READY_TO_INTEGRATE;
??hipAdcCallback_6:
        LDR.N    R0,??DataTable16_9
        MOVS     R1,#+1
        STRB     R1,[R0, #+0]
//  302 		}
//  303 	}
//  304 }
??hipAdcCallback_1:
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+24
        POP      {R4-R8,PC}       ;; return
          CFI EndBlock cfiBlock14

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13:
        DC32     0x3a400c01

        SECTION `.data`:DATA:REORDER:NOROOT(0)
//  305 static bool_t needToInit = true;
needToInit:
        DATA
        DC8 1
//  306 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock15 Using cfiCommon0
          CFI Function _Z14hipStartupCodev
        THUMB
//  307 static void hipStartupCode(void) {
_Z14hipStartupCodev:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  308 //	D[4:1] = 0000 : 4 MHz
//  309 //	D[4:1] = 0001 : 5 MHz
//  310 //	D[4:1] = 0010 : 6 MHz
//  311 //	D[4:1] = 0011 ; 8 MHz
//  312 //	D[4:1] = 0100 ; 10 MHz
//  313 //	D[4:1] = 0101 ; 12 MHz
//  314 //	D[4:1] = 0110 : 16 MHz
//  315 //	D[4:1] = 0111 : 20 MHz
//  316 //	D[4:1] = 1000 : 24 MHz
//  317 
//  318 
//  319 // '0' for 4MHz
//  320 	currentPrescaler = engineConfiguration->hip9011PrescalerAndSDO;
        LDR.N    R0,??DataTable16_5
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+328]
        LDR.N    R1,??DataTable16_37
        STR      R0,[R1, #+0]
//  321 	SPI_SYNCHRONOUS(SET_PRESCALER_CMD + currentPrescaler);
        LDR.N    R0,??DataTable16_33
        LDR      R0,[R0, #+0]
          CFI FunCall spiSelect
        BL       spiSelect
        LDR.N    R0,??DataTable16_37
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+64
        LDR.N    R1,??DataTable16_35
        STRB     R0,[R1, #+0]
        LDR.N    R3,??DataTable16_36
        LDR.N    R2,??DataTable16_35
        MOVS     R1,#+1
        LDR.N    R0,??DataTable16_33
        LDR      R0,[R0, #+0]
          CFI FunCall spiExchange
        BL       spiExchange
        LDR.N    R0,??DataTable16_33
        LDR      R0,[R0, #+0]
          CFI FunCall spiUnselect
        BL       spiUnselect
        LDR.N    R0,??DataTable16_36
        LDRB     R0,[R0, #+0]
        CMP      R0,#+0
        BEQ.N    ??hipStartupCode_0
        LDR.N    R0,??DataTable16_17
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+1
        LDR.N    R1,??DataTable16_17
        STR      R0,[R1, #+0]
//  322 
//  323 	chThdSleepMilliseconds(10);
??hipStartupCode_0:
        MOVS     R0,#+10
          CFI FunCall chThdSleep
        BL       chThdSleep
//  324 
//  325 	// '0' for channel #1
//  326 	SPI_SYNCHRONOUS(SET_CHANNEL_CMD + 0);
        LDR.N    R0,??DataTable16_33
        LDR      R0,[R0, #+0]
          CFI FunCall spiSelect
        BL       spiSelect
        LDR.N    R0,??DataTable16_35
        MOVS     R1,#+224
        STRB     R1,[R0, #+0]
        LDR.N    R3,??DataTable16_36
        LDR.N    R2,??DataTable16_35
        MOVS     R1,#+1
        LDR.N    R0,??DataTable16_33
        LDR      R0,[R0, #+0]
          CFI FunCall spiExchange
        BL       spiExchange
        LDR.N    R0,??DataTable16_33
        LDR      R0,[R0, #+0]
          CFI FunCall spiUnselect
        BL       spiUnselect
        LDR.N    R0,??DataTable16_36
        LDRB     R0,[R0, #+0]
        CMP      R0,#+0
        BEQ.N    ??hipStartupCode_1
        LDR.N    R0,??DataTable16_17
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+1
        LDR.N    R1,??DataTable16_17
        STR      R0,[R1, #+0]
//  327 
//  328 	chThdSleepMilliseconds(10);
??hipStartupCode_1:
        MOVS     R0,#+10
          CFI FunCall chThdSleep
        BL       chThdSleep
//  329 
//  330 	// band index depends on cylinder bore
//  331 	SPI_SYNCHRONOUS(SET_BAND_PASS_CMD + currentBandIndex);
        LDR.N    R0,??DataTable16_33
        LDR      R0,[R0, #+0]
          CFI FunCall spiSelect
        BL       spiSelect
        LDR.N    R0,??DataTable16_35
        LDR.N    R1,??DataTable16_12
        LDR      R1,[R1, #+0]
        STRB     R1,[R0, #+0]
        LDR.N    R3,??DataTable16_36
        LDR.N    R2,??DataTable16_35
        MOVS     R1,#+1
        LDR.N    R0,??DataTable16_33
        LDR      R0,[R0, #+0]
          CFI FunCall spiExchange
        BL       spiExchange
        LDR.N    R0,??DataTable16_33
        LDR      R0,[R0, #+0]
          CFI FunCall spiUnselect
        BL       spiUnselect
        LDR.N    R0,??DataTable16_36
        LDRB     R0,[R0, #+0]
        CMP      R0,#+0
        BEQ.N    ??hipStartupCode_2
        LDR.N    R0,??DataTable16_17
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+1
        LDR.N    R1,??DataTable16_17
        STR      R0,[R1, #+0]
//  332 
//  333 	chThdSleepMilliseconds(10);
??hipStartupCode_2:
        MOVS     R0,#+10
          CFI FunCall chThdSleep
        BL       chThdSleep
//  334 
//  335 	if (boardConfiguration->useTpicAdvancedMode) {
        LDR.N    R0,??DataTable16_6
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+385]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+3
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??hipStartupCode_3
//  336 		// enable advanced mode for digital integrator output
//  337 		SPI_SYNCHRONOUS(SET_ADVANCED_MODE);
        LDR.N    R0,??DataTable16_33
        LDR      R0,[R0, #+0]
          CFI FunCall spiSelect
        BL       spiSelect
        LDR.N    R0,??DataTable16_35
        MOVS     R1,#+113
        STRB     R1,[R0, #+0]
        LDR.N    R3,??DataTable16_36
        LDR.N    R2,??DataTable16_35
        MOVS     R1,#+1
        LDR.N    R0,??DataTable16_33
        LDR      R0,[R0, #+0]
          CFI FunCall spiExchange
        BL       spiExchange
        LDR.N    R0,??DataTable16_33
        LDR      R0,[R0, #+0]
          CFI FunCall spiUnselect
        BL       spiUnselect
        LDR.N    R0,??DataTable16_36
        LDRB     R0,[R0, #+0]
        CMP      R0,#+0
        BEQ.N    ??hipStartupCode_4
        LDR.N    R0,??DataTable16_17
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+1
        LDR.N    R1,??DataTable16_17
        STR      R0,[R1, #+0]
//  338 
//  339     	chThdSleepMilliseconds(10);
??hipStartupCode_4:
        MOVS     R0,#+10
          CFI FunCall chThdSleep
        BL       chThdSleep
//  340 	}
//  341 
//  342 	/**
//  343 	 * Let's restart SPI to switch it from synchronous mode into
//  344 	 * asynchronous mode
//  345 	 */
//  346 	spiStop(driver);
??hipStartupCode_3:
        LDR.N    R0,??DataTable16_33
        LDR      R0,[R0, #+0]
          CFI FunCall spiStop
        BL       spiStop
//  347 	spicfg.end_cb = endOfSpiExchange;
        LDR.N    R0,??DataTable16_38
        LDR.N    R1,??DataTable16_39
        STR      R1,[R0, #+0]
//  348 	spiStart(driver, &spicfg);
        LDR.N    R1,??DataTable16_38
        LDR.N    R0,??DataTable16_33
        LDR      R0,[R0, #+0]
          CFI FunCall spiStart
        BL       spiStart
//  349 	state = READY_TO_INTEGRATE;
        LDR.N    R0,??DataTable16_9
        MOVS     R1,#+1
        STRB     R1,[R0, #+0]
//  350 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock15
//  351 

        SECTION `.bss`:DATA:REORDER:NOROOT(3)
//  352 static THD_WORKING_AREA(hipTreadStack, UTILITY_THREAD_STACK_SIZE);
hipTreadStack:
        DS8 712
//  353 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock16 Using cfiCommon0
          CFI Function _Z9hipThreadPv
        THUMB
//  354 static msg_t hipThread(void *arg) {
_Z9hipThreadPv:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  355 	chRegSetThreadName("hip9011 init");
        LDR.N    R0,??DataTable16_40
        LDR      R0,[R0, #+24]
        LDR.N    R1,??DataTable16_41
        STR      R1,[R0, #+24]
//  356 	while (true) {
//  357 		// some time to let the hardware start
//  358 		chThdSleepMilliseconds(500);
??hipThread_0:
        MOV      R0,#+500
          CFI FunCall chThdSleep
        BL       chThdSleep
//  359 		if (needToInit) {
        LDR.N    R0,??DataTable16_42
        LDRB     R0,[R0, #+0]
        CMP      R0,#+0
        BEQ.N    ??hipThread_0
//  360 			hipStartupCode();
          CFI FunCall _Z14hipStartupCodev
        BL       _Z14hipStartupCodev
//  361 			needToInit = false;
        LDR.N    R0,??DataTable16_42
        MOVS     R1,#+0
        STRB     R1,[R0, #+0]
        B.N      ??hipThread_0
          CFI EndBlock cfiBlock16
//  362 		}
//  363 	}
//  364 	return -1;
//  365 }
//  366 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock17 Using cfiCommon0
          CFI Function _Z11initHip9011P7Logging
        THUMB
//  367 void initHip9011(Logging *sharedLogger) {
_Z11initHip9011P7Logging:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        SUB      SP,SP,#+8
          CFI CFA R13+16
        MOVS     R4,R0
//  368 	logger = sharedLogger;
        LDR.N    R0,??DataTable16_8
        STR      R4,[R0, #+0]
//  369 	addConsoleAction("hipinfo", showHipInfo);
        LDR.N    R1,??DataTable16_43
        LDR.N    R0,??DataTable16_44
          CFI FunCall addConsoleAction
        BL       addConsoleAction
//  370 	if (!boardConfiguration->isHip9011Enabled)
        LDR.N    R0,??DataTable16_6
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+384]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+6
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??initHip9011_0
//  371 		return;
//  372 
//  373 
//  374 	currentAngleWindowWidth =
//  375 	engineConfiguration->knockDetectionWindowEnd - engineConfiguration->knockDetectionWindowStart;
??initHip9011_1:
        LDR.N    R0,??DataTable16_5
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+1544]
        VMOV     S0,R0
        LDR.N    R0,??DataTable16_5
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+1540]
        VMOV     S1,R0
        VSUB.F32 S0,S0,S1
        LDR.N    R0,??DataTable16_34
        VSTR     S0,[R0, #0]
//  376 
//  377 	prepareHip9011RpmLookup(currentAngleWindowWidth);
        LDR.N    R0,??DataTable16_34
        VLDR     S0,[R0, #0]
          CFI FunCall _Z23prepareHip9011RpmLookupf
        BL       _Z23prepareHip9011RpmLookupf
//  378 
//  379 	// todo: configurable
//  380 //	driver = getSpiDevice(boardConfiguration->hip9011SpiDevice);
//  381 
//  382 	spicfg.ssport = getHwPort(boardConfiguration->hip9011CsPin);
        LDR.N    R0,??DataTable16_6
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+648]
          CFI FunCall _Z9getHwPort11brain_pin_e
        BL       _Z9getHwPort11brain_pin_e
        LDR.N    R1,??DataTable16_38
        STR      R0,[R1, #+4]
//  383 	spicfg.sspad = getHwPin(boardConfiguration->hip9011CsPin);
        LDR.N    R0,??DataTable16_6
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+648]
          CFI FunCall _Z8getHwPin11brain_pin_e
        BL       _Z8getHwPin11brain_pin_e
        LDR.N    R1,??DataTable16_38
        STRH     R0,[R1, #+8]
//  384 
//  385 	outputPinRegisterExt2("hip int/hold", &intHold, boardConfiguration->hip9011IntHoldPin, &DEFAULT_OUTPUT);
        LDR.N    R3,??DataTable16_45
        LDR.N    R0,??DataTable16_6
        LDR      R0,[R0, #+0]
        LDR      R2,[R0, #+652]
        LDR.N    R1,??DataTable16
        LDR.N    R0,??DataTable16_46
          CFI FunCall _Z21outputPinRegisterExt2PKcP9OutputPin11brain_pin_eP17pin_output_mode_e
        BL       _Z21outputPinRegisterExt2PKcP9OutputPin11brain_pin_eP17pin_output_mode_e
//  386 	outputPinRegisterExt2("hip CS", &hipCs, boardConfiguration->hip9011CsPin, &DEFAULT_OUTPUT);
        LDR.N    R3,??DataTable16_45
        LDR.N    R0,??DataTable16_6
        LDR      R0,[R0, #+0]
        LDR      R2,[R0, #+648]
        LDR.N    R1,??DataTable16_1
        LDR.N    R0,??DataTable16_47
          CFI FunCall _Z21outputPinRegisterExt2PKcP9OutputPin11brain_pin_eP17pin_output_mode_e
        BL       _Z21outputPinRegisterExt2PKcP9OutputPin11brain_pin_eP17pin_output_mode_e
//  387 
//  388 	scheduleMsg(logger, "Starting HIP9011/TPIC8101 driver");
        LDR.N    R1,??DataTable16_48
        LDR.N    R0,??DataTable16_8
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  389 	spiStart(driver, &spicfg);
        LDR.N    R1,??DataTable16_38
        LDR.N    R0,??DataTable16_33
        LDR      R0,[R0, #+0]
          CFI FunCall spiStart
        BL       spiStart
//  390 
//  391 	currentBandIndex = getBandIndex();
          CFI FunCall _Z12getBandIndexv
        BL       _Z12getBandIndexv
        LDR.N    R1,??DataTable16_12
        STR      R0,[R1, #+0]
//  392 
//  393 	/**
//  394 	 * this engine cycle callback would be scheduling actual integration start and end callbacks
//  395 	 */
//  396 	addTriggerEventListener(&intHoldCallback, "DD int/hold", engine);
        LDR.N    R0,??DataTable16_14
        LDR      R2,[R0, #+0]
        LDR.N    R1,??DataTable16_49
        LDR.N    R0,??DataTable16_50
          CFI FunCall _Z23addTriggerEventListenerPFv15trigger_event_ejEPKcP6Engine
        BL       _Z23addTriggerEventListenerPFv15trigger_event_ejEPKcP6Engine
//  397 
//  398 	// MISO PB14
//  399 //	palSetPadMode(GPIOB, 14, PAL_MODE_ALTERNATE(EFI_SPI2_AF) | PAL_STM32_PUDR_PULLUP);
//  400 	// MOSI PB15
//  401 //	palSetPadMode(GPIOB, 15, PAL_MODE_ALTERNATE(EFI_SPI2_AF) | PAL_STM32_OTYPE_OPENDRAIN);
//  402 
//  403 	addConsoleActionF("set_gain", setGain);
        LDR.N    R1,??DataTable16_51
        LDR.N    R0,??DataTable16_52
          CFI FunCall addConsoleActionF
        BL       addConsoleActionF
//  404 	addConsoleActionF("set_band", setBand);
        LDR.N    R1,??DataTable16_53
        LDR.N    R0,??DataTable16_54
          CFI FunCall addConsoleActionF
        BL       addConsoleActionF
//  405 	addConsoleActionI("set_hip_prescalerandsdo", setPrescalerAndSDO);
        LDR.N    R1,??DataTable16_55
        LDR.N    R0,??DataTable16_56
          CFI FunCall addConsoleActionI
        BL       addConsoleActionI
//  406     addConsoleActionF("set_knock_threshold", setKnockThresh);
        LDR.N    R1,??DataTable16_57
        LDR.N    R0,??DataTable16_58
          CFI FunCall addConsoleActionF
        BL       addConsoleActionF
//  407     addConsoleActionI("set_max_knock_sub_deg", setMaxKnockSubDeg);
        LDR.N    R1,??DataTable16_59
        LDR.N    R0,??DataTable16_60
          CFI FunCall addConsoleActionI
        BL       addConsoleActionI
//  408 	chThdCreateStatic(hipTreadStack, sizeof(hipTreadStack), NORMALPRIO, (tfunc_t) hipThread, NULL);
        MOVS     R0,#+0
        STR      R0,[SP, #+0]
        LDR.N    R3,??DataTable16_61
        MOVS     R2,#+64
        MOV      R1,#+712
        LDR.N    R0,??DataTable16_62
          CFI FunCall chThdCreateStatic
        BL       chThdCreateStatic
//  409 }
??initHip9011_0:
        POP      {R0,R1,R4,PC}    ;; return
          CFI EndBlock cfiBlock17

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16:
        DC32     intHold

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_1:
        DC32     hipCs

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_2:
        DC32     _ZN12scheduling_sC1Ev

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_3:
        DC32     startTimer

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_4:
        DC32     endTimer

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_5:
        DC32     engineConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_6:
        DC32     boardConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_7:
        DC32     `?<Constant "hip9011 driver not ac...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_8:
        DC32     logger

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_9:
        DC32     state

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_10:
        DC32     `?<Constant "enabled=%s state=%d b...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_11:
        DC32     currentGainIndex

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_12:
        DC32     currentBandIndex

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_13:
        DC32     `?<Constant "band_index=%d gain %f...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_14:
        DC32     engine

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_15:
        DC32     currentIntergratorIndex

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_16:
        DC32     `?<Constant "integrator index=%d k...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_17:
        DC32     nonZeroResponse

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_18:
        DC32     `?<Constant "spi= IntHold@%s respo...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_19:
        DC32     settingUpdateCount

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_20:
        DC32     `?<Constant "CS@%s updateCount=%d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_21:
        DC32     pinNameBuffer

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_22:
        DC32     spiCount

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_23:
        DC32     hipValueMax

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_24:
        DC32     knockVolts

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_25:
        DC32     `?<Constant "hip %fv@%s/max=%f spi...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_26:
        DC32     `?<Constant "start %f end %f">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_27:
        DC32     0x41a00000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_28:
        DC32     0xe0001004

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_29:
        DC32     _Z16startIntegrationv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_30:
        DC32     hipLastExecutionCount

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_31:
        DC32     lastExecutionCount

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_32:
        DC32     _Z14endIntegrationv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_33:
        DC32     driver

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_34:
        DC32     currentAngleWindowWidth

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_35:
        DC32     tx_buff

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_36:
        DC32     rx_buff

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_37:
        DC32     currentPrescaler

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_38:
        DC32     spicfg

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_39:
        DC32     _Z16endOfSpiExchangeP9SPIDriver

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_40:
        DC32     rlist

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_41:
        DC32     `?<Constant "hip9011 init">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_42:
        DC32     needToInit

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_43:
        DC32     _Z11showHipInfov

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_44:
        DC32     `?<Constant "hipinfo">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_45:
        DC32     DEFAULT_OUTPUT

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_46:
        DC32     `?<Constant "hip int/hold">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_47:
        DC32     `?<Constant "hip CS">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_48:
        DC32     `?<Constant "Starting HIP9011/TPIC...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_49:
        DC32     `?<Constant "DD int/hold">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_50:
        DC32     _Z15intHoldCallback15trigger_event_ej

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_51:
        DC32     _Z7setGainf

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_52:
        DC32     `?<Constant "set_gain">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_53:
        DC32     _Z7setBandf

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_54:
        DC32     `?<Constant "set_band">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_55:
        DC32     _Z18setPrescalerAndSDOi

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_56:
        DC32     `?<Constant "set_hip_prescalerandsdo">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_57:
        DC32     _Z14setKnockThreshf

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_58:
        DC32     `?<Constant "set_knock_threshold">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_59:
        DC32     _Z17setMaxKnockSubDegi

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_60:
        DC32     `?<Constant "set_max_knock_sub_deg">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_61:
        DC32     _Z9hipThreadPv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_62:
        DC32     hipTreadStack

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
`?<Constant "hip9011 driver not ac...">`:
        DATA
        DC8 "hip9011 driver not active"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "enabled=%s state=%d b...">`:
        DATA
        DC8 "enabled=%s state=%d bore=%fmm freq=%fkHz PaSDO=%d"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "band_index=%d gain %f...">`:
        DATA
        DC8 "band_index=%d gain %f/index=%d"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "integrator index=%d k...">`:
        DATA
        DC8 69H, 6EH, 74H, 65H, 67H, 72H, 61H, 74H
        DC8 6FH, 72H, 20H, 69H, 6EH, 64H, 65H, 78H
        DC8 3DH, 25H, 64H, 20H, 6BH, 6EH, 6FH, 63H
        DC8 6BH, 56H, 54H, 68H, 72H, 65H, 73H, 68H
        DC8 6FH, 6CH, 64H, 3DH, 25H, 66H, 20H, 6BH
        DC8 6EH, 6FH, 63H, 6BH, 43H, 6FH, 75H, 6EH
        DC8 74H, 3DH, 25H, 64H, 20H, 6DH, 61H, 78H
        DC8 4BH, 6EH, 6FH, 63H, 6BH, 53H, 75H, 62H
        DC8 44H, 65H, 67H, 3DH, 25H, 66H, 0
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "spi= IntHold@%s respo...">`:
        DATA
        DC8 "spi= IntHold@%s response count=%d"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "CS@%s updateCount=%d">`:
        DATA
        DC8 "CS@%s updateCount=%d"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "hip %fv@%s/max=%f spi...">`:
        DATA
        DC8 "hip %fv@%s/max=%f spiCount=%d adv=%d"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "start %f end %f">`:
        DATA
        DC8 "start %f end %f"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "hip9011 init">`:
        DATA
        DC8 "hip9011 init"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "hipinfo">`:
        DATA
        DC8 "hipinfo"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "hip int/hold">`:
        DATA
        DC8 "hip int/hold"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "hip CS">`:
        DATA
        DC8 "hip CS"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Starting HIP9011/TPIC...">`:
        DATA
        DC8 "Starting HIP9011/TPIC8101 driver"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "DD int/hold">`:
        DATA
        DC8 "DD int/hold"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_gain">`:
        DATA
        DC8 "set_gain"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_band">`:
        DATA
        DC8 "set_band"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_hip_prescalerandsdo">`:
        DATA
        DC8 "set_hip_prescalerandsdo"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_knock_threshold">`:
        DATA
        DC8 "set_knock_threshold"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_max_knock_sub_deg">`:
        DATA
        DC8 "set_max_knock_sub_deg"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "HIP"

        END
//  410 
//  411 #endif
// 
//   903 bytes in section .bss
//    25 bytes in section .data
//     4 bytes in section .init_array
//   492 bytes in section .rodata
// 2 656 bytes in section .text
// 
// 2 660 bytes of CODE  memory
//   492 bytes of CONST memory
//   928 bytes of DATA  memory
//
//Errors: none
//Warnings: 3
