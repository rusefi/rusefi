///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:28 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\hw_layer\lcd\lc /
//                       d_HD44780.cpp                                        /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\hw_layer\lcd\lc /
//                       d_HD44780.cpp -lCN F:\stuff\rusefi_sourceforge\firmw /
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
//                       lcd_HD44780.s                                        /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME lcd_HD44780

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _Z10hwPortname11brain_pin_e
        EXTERN _Z13mySetPadMode2PKc11brain_pin_ej
        EXTERN _Z8getHwPin11brain_pin_e
        EXTERN _Z9getHwPort11brain_pin_e
        EXTERN addConsoleAction
        EXTERN boardConfiguration
        EXTERN chThdSleep
        EXTERN engineConfiguration
        EXTERN firmwareError
        EXTERN printMsg
        EXTERN scheduleMsg

        PUBLIC _Z22lcd_HD44780_write_datah
        PUBLIC _Z25lcd_HD44780_write_commandh
        PUBLIC getCurrentHD44780column
        PUBLIC getCurrentHD44780row
        PUBLIC lcdShowFatalMessage
        PUBLIC lcd_HD44780_command
        PUBLIC lcd_HD44780_init
        PUBLIC lcd_HD44780_print_char
        PUBLIC lcd_HD44780_print_string
        PUBLIC lcd_HD44780_set_position
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\hw_layer\lcd\lcd_HD44780.cpp
//    1 /**
//    2  * @file lcd_HD44780.cpp
//    3  * @brief HD44780 character display driver
//    4  *
//    5  * see http://en.wikipedia.org/wiki/Hitachi_HD44780_LCD_controller
//    6  * @date 13.12.2013
//    7  * @author Andrey Belomutskiy, (c) 2012-2015
//    8  */
//    9 
//   10 #include "main.h"
//   11 
//   12 #if EFI_HD44780_LCD
//   13 
//   14 #include "lcd_HD44780.h"
//   15 #include "pin_repository.h"
//   16 #include "string.h"
//   17 
//   18 #include "engine_configuration.h"
//   19 
//   20 extern engine_configuration_s *engineConfiguration;
//   21 extern board_configuration_s *boardConfiguration;
//   22 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   23 static Logging *logger;
logger:
        DS8 4
//   24 
//   25 enum {
//   26 	LCD_HD44780_DISPLAY_CLEAR = 0x01,
//   27 	LCD_HD44780_SHIFT_CURSOR_RIGHT = 0x06,
//   28 	LCD_HD44780_DISPLAY_ON = 0x0C,
//   29 	LCD_HD44780_4_BIT_BUS = 0x20,
//   30 	LCD_HD44780_RESET = 0x30,
//   31 	LCD_HD44780_DDRAM_ADDR = 0x80,
//   32 
//   33 //	LCD_2X16_8_BIT_BUS = 0x30,
//   34 //	LCD_2X16_LINE_ONE = 0x20,
//   35 //	LCD_2X16_LINES_TWO = 0x28,
//   36 //	LCD_2X16_FONT_5X8 = 0x20,
//   37 //	LCD_2X16_FONT_5X10 = 0x24,
//   38 //	LCD_2X16_DISPLAY_HOME = 0x02,
//   39 //	LCD_2X16_DISPLAY_RIGHT = 0x1C,
//   40 //	LCD_2X16_DISPLAY_LEFT = 0x18,
//   41 //	LCD_2X16_DISPLAY_SHIFT = 0x05,
//   42 //	LCD_2X16_CURSOR_ON = 0x0A,
//   43 //	LCD_2X16_CURSOR_BLINK = 0x09,
//   44 //	LCD_2X16_CURSOR_RIGHT = 0x14,
//   45 //	LCD_2X16_CURSOR_LEFT = 0x10,
//   46 //	LCD_2X16_SHIFT_LEFT = 0x04,
//   47 //	LCD_2X16_CGRAM_ADDR = 0x40,
//   48 //	LCD_2X16_BUSY_FLAG = 0x80,
//   49 //	LCD_2X16_COMMAND = 0x01,
//   50 //	LCD_2X16_DATA = 0x00,

        SECTION `.bss`:DATA:REORDER:NOROOT(0)
//   51 } lcd_HD44780_command;
lcd_HD44780_command:
        DS8 1

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "true">`:
        DATA
        DC8 "true"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "invalid row">`:
        DATA
        DC8 "invalid row"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "HD44780 RS=%s">`:
        DATA
        DC8 "HD44780 RS=%s"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "HD44780 E=%s">`:
        DATA
        DC8 "HD44780 E=%s"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "HD44780 D4=%s">`:
        DATA
        DC8 "HD44780 D4=%s"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "HD44780 D5=%s">`:
        DATA
        DC8 "HD44780 D5=%s"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "HD44780 D6=%s">`:
        DATA
        DC8 "HD44780 D6=%s"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "HD44780 D7=%s">`:
        DATA
        DC8 "HD44780 D7=%s"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "lcdinfo">`:
        DATA
        DC8 "lcdinfo"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Unexpected displayMod...">`:
        DATA
        DC8 "Unexpected displayMode %d"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "lcd_HD44780_init %d">`:
        DATA
        DC8 "lcd_HD44780_init %d"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "lcd RS">`:
        DATA
        DC8 "lcd RS"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "lcd E">`:
        DATA
        DC8 "lcd E"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "lcd DB4">`:
        DATA
        DC8 "lcd DB4"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "lcd DB6">`:
        DATA
        DC8 "lcd DB6"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "lcd DB7">`:
        DATA
        DC8 "lcd DB7"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "lcd DB8">`:
        DATA
        DC8 "lcd DB8"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "lcd_HD44780_init() done">`:
        DATA
        DC8 "lcd_HD44780_init() done"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "fatal\\n">`:
        DATA
        DC8 "fatal\012"
        DC8 0
//   52 
//   53 // http://web.alfredstate.edu/weimandn/lcd/lcd_addressing/lcd_addressing_index.html

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
//   54 static const int lineStart[] = { 0, 0x40, 0x14, 0x54 };
lineStart:
        DATA
        DC32 0, 64, 20, 84
//   55 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   56 static int BUSY_WAIT_DELAY = FALSE;
BUSY_WAIT_DELAY:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   57 static int currentRow = 0;
currentRow:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   58 static int currentColumn = 0;
currentColumn:
        DS8 4
//   59 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _Z8lcdSleepi
        THUMB
//   60 static void lcdSleep(int period) {
_Z8lcdSleepi:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
//   61 	if (BUSY_WAIT_DELAY) {
        LDR.W    R0,??DataTable10
        LDR      R0,[R0, #+0]
        CMP      R0,#+0
        BEQ.N    ??lcdSleep_0
//   62 		// this mode is useful for displaying messages to report OS fatal issues
//   63 
//   64 		int ticks = 168000000 / 1000000 * period;
        MOVS     R0,#+168
        MUL      R5,R0,R4
//   65 		int a = 0;
        MOVS     R6,#+0
//   66 		for (int i = 0; i < ticks; i++)
        MOVS     R0,#+0
??lcdSleep_1:
        CMP      R0,R5
        BGE.N    ??lcdSleep_2
//   67 			a += i;
        ADDS     R6,R0,R6
        ADDS     R0,R0,#+1
        B.N      ??lcdSleep_1
//   68 		// the purpose of this code is to fool the compiler so that the loop is not optimized away
//   69 		efiAssertVoid(a != 0, "true");
??lcdSleep_2:
        CMP      R6,#+0
        BNE.N    ??lcdSleep_3
        LDR.W    R0,??DataTable10_1
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??lcdSleep_4
//   70 
//   71 	} else {
//   72 		chThdSleepMicroseconds(period);
??lcdSleep_0:
        MOV      R0,#+1000
        MUL      R0,R0,R4
        SUBS     R0,R0,#+1
        LDR.W    R1,??DataTable10_2  ;; 0xf4240
        UDIV     R0,R0,R1
        ADDS     R0,R0,#+1
          CFI FunCall chThdSleep
        BL       chThdSleep
//   73 	}
//   74 }
??lcdSleep_3:
??lcdSleep_4:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock0
//   75 
//   76 //static char txbuf[1];
//   77 #define LCD_PORT_EXP_ADDR 0x20
//   78 
//   79 //-----------------------------------------------------------------------------

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z17lcd_HD44780_writeh
        THUMB
//   80 static void lcd_HD44780_write(uint8_t data) {
_Z17lcd_HD44780_writeh:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
//   81 	if (engineConfiguration->displayMode == DM_HD44780) {
        LDR.W    R0,??DataTable10_3
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+504]
        CMP      R0,#+1
        BNE.W    ??lcd_HD44780_write_0
//   82 		palWritePad(getHwPort(boardConfiguration->HD44780_db7), getHwPin(boardConfiguration->HD44780_db7),
//   83 				data & 0x80 ? 1 : 0);
        LDR.W    R0,??DataTable10_4
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+196]
          CFI FunCall _Z9getHwPort11brain_pin_e
        BL       _Z9getHwPort11brain_pin_e
        MOVS     R5,R0
        UBFX     R0,R4,#+7,#+1
        ANDS     R6,R0,#0x1
        LDR.W    R0,??DataTable10_4
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+196]
          CFI FunCall _Z8getHwPin11brain_pin_e
        BL       _Z8getHwPin11brain_pin_e
        UBFX     R1,R4,#+7,#+1
        ANDS     R1,R1,#0x1
        MVNS     R2,R6
        ANDS     R2,R2,#0x1
        ADDS     R0,R0,#+16
        LSLS     R6,R2,R0
        ANDS     R7,R1,#0x1
        LDR.W    R0,??DataTable10_4
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+196]
          CFI FunCall _Z8getHwPin11brain_pin_e
        BL       _Z8getHwPin11brain_pin_e
        LSLS     R0,R7,R0
        ORRS     R0,R0,R6
        STR      R0,[R5, #+24]
//   84 		palWritePad(getHwPort(boardConfiguration->HD44780_db6), getHwPin(boardConfiguration->HD44780_db6),
//   85 				data & 0x40 ? 1 : 0);
        LDR.W    R0,??DataTable10_4
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+192]
          CFI FunCall _Z9getHwPort11brain_pin_e
        BL       _Z9getHwPort11brain_pin_e
        MOVS     R5,R0
        UBFX     R0,R4,#+6,#+1
        ANDS     R6,R0,#0x1
        LDR.W    R0,??DataTable10_4
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+192]
          CFI FunCall _Z8getHwPin11brain_pin_e
        BL       _Z8getHwPin11brain_pin_e
        UBFX     R1,R4,#+6,#+1
        ANDS     R1,R1,#0x1
        MVNS     R2,R6
        ANDS     R2,R2,#0x1
        ADDS     R0,R0,#+16
        LSLS     R6,R2,R0
        ANDS     R7,R1,#0x1
        LDR.W    R0,??DataTable10_4
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+192]
          CFI FunCall _Z8getHwPin11brain_pin_e
        BL       _Z8getHwPin11brain_pin_e
        LSLS     R0,R7,R0
        ORRS     R0,R0,R6
        STR      R0,[R5, #+24]
//   86 		palWritePad(getHwPort(boardConfiguration->HD44780_db5), getHwPin(boardConfiguration->HD44780_db5),
//   87 				data & 0x20 ? 1 : 0);
        LDR.W    R0,??DataTable10_4
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+188]
          CFI FunCall _Z9getHwPort11brain_pin_e
        BL       _Z9getHwPort11brain_pin_e
        MOVS     R5,R0
        UBFX     R0,R4,#+5,#+1
        ANDS     R6,R0,#0x1
        LDR.W    R0,??DataTable10_4
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+188]
          CFI FunCall _Z8getHwPin11brain_pin_e
        BL       _Z8getHwPin11brain_pin_e
        UBFX     R1,R4,#+5,#+1
        ANDS     R1,R1,#0x1
        MVNS     R2,R6
        ANDS     R2,R2,#0x1
        ADDS     R0,R0,#+16
        LSLS     R6,R2,R0
        ANDS     R7,R1,#0x1
        LDR.W    R0,??DataTable10_4
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+188]
          CFI FunCall _Z8getHwPin11brain_pin_e
        BL       _Z8getHwPin11brain_pin_e
        LSLS     R0,R7,R0
        ORRS     R0,R0,R6
        STR      R0,[R5, #+24]
//   88 		palWritePad(getHwPort(boardConfiguration->HD44780_db4), getHwPin(boardConfiguration->HD44780_db4),
//   89 				data & 0x10 ? 1 : 0);
        LDR.W    R0,??DataTable10_4
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+184]
          CFI FunCall _Z9getHwPort11brain_pin_e
        BL       _Z9getHwPort11brain_pin_e
        MOVS     R5,R0
        UBFX     R0,R4,#+4,#+1
        ANDS     R6,R0,#0x1
        LDR.W    R0,??DataTable10_4
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+184]
          CFI FunCall _Z8getHwPin11brain_pin_e
        BL       _Z8getHwPin11brain_pin_e
        UBFX     R1,R4,#+4,#+1
        ANDS     R1,R1,#0x1
        MVNS     R2,R6
        ANDS     R2,R2,#0x1
        ADDS     R0,R0,#+16
        LSLS     R6,R2,R0
        ANDS     R7,R1,#0x1
        LDR.W    R0,??DataTable10_4
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+184]
          CFI FunCall _Z8getHwPin11brain_pin_e
        BL       _Z8getHwPin11brain_pin_e
        LSLS     R0,R7,R0
        ORRS     R0,R0,R6
        STR      R0,[R5, #+24]
//   90 
//   91 		palSetPad(getHwPort(boardConfiguration->HD44780_e), getHwPin(boardConfiguration->HD44780_e)); // En high
        LDR.W    R0,??DataTable10_4
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+180]
          CFI FunCall _Z9getHwPort11brain_pin_e
        BL       _Z9getHwPort11brain_pin_e
        MOVS     R5,R0
        MOVS     R6,#+1
        LDR.W    R0,??DataTable10_4
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+180]
          CFI FunCall _Z8getHwPin11brain_pin_e
        BL       _Z8getHwPin11brain_pin_e
        LSLS     R0,R6,R0
        STRH     R0,[R5, #+24]
//   92 		lcdSleep(10); // enable pulse must be >450ns
        MOVS     R0,#+10
          CFI FunCall _Z8lcdSleepi
        BL       _Z8lcdSleepi
//   93 		palClearPad(getHwPort(boardConfiguration->HD44780_e), getHwPin(boardConfiguration->HD44780_e)); // En low
        LDR.W    R0,??DataTable10_4
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+180]
          CFI FunCall _Z9getHwPort11brain_pin_e
        BL       _Z9getHwPort11brain_pin_e
        MOVS     R5,R0
        MOVS     R6,#+1
        LDR.W    R0,??DataTable10_4
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+180]
          CFI FunCall _Z8getHwPin11brain_pin_e
        BL       _Z8getHwPin11brain_pin_e
        LSLS     R0,R6,R0
        STRH     R0,[R5, #+26]
//   94 		lcdSleep(40); // commands need > 37us to settle
        MOVS     R0,#+40
          CFI FunCall _Z8lcdSleepi
        BL       _Z8lcdSleepi
//   95 	} else {
//   96 
//   97 		//	LCD D4_pin -> P4
//   98 		//	LCD D5_pin -> P5
//   99 		//	LCD D6_pin -> P6
//  100 		//	LCD D7_pin -> P7
//  101 		//	LCD Pin RS -> P0
//  102 		//	LCD Pin RW -> P1
//  103 		//	LCD Pin E  -> P2
//  104 
//  105 		// todo: finish all this stuff
//  106 //				i2cAcquireBus(&I2CD1);
//  107 //
//  108 //				txbuf[0] = 4;
//  109 //				i2cMasterTransmit(&I2CD1, LCD_PORT_EXP_ADDR, txbuf, 1, NULL, 0);
//  110 //				lcdSleep(10); // enable pulse must be >450ns
//  111 //
//  112 //				txbuf[0] = 0;
//  113 //				i2cMasterTransmit(&I2CD1, LCD_PORT_EXP_ADDR, txbuf, 1, NULL, 0);
//  114 //
//  115 //				i2cReleaseBus(&I2CD1);
//  116 
//  117 	}
//  118 }
??lcd_HD44780_write_0:
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock1
//  119 
//  120 //-----------------------------------------------------------------------------

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z25lcd_HD44780_write_commandh
        THUMB
//  121 void lcd_HD44780_write_command(uint8_t data) {
_Z25lcd_HD44780_write_commandh:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
//  122 	palClearPad(getHwPort(boardConfiguration->HD44780_rs), getHwPin(boardConfiguration->HD44780_rs));
        LDR.W    R0,??DataTable10_4
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+176]
          CFI FunCall _Z9getHwPort11brain_pin_e
        BL       _Z9getHwPort11brain_pin_e
        MOVS     R5,R0
        MOVS     R6,#+1
        LDR.W    R0,??DataTable10_4
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+176]
          CFI FunCall _Z8getHwPin11brain_pin_e
        BL       _Z8getHwPin11brain_pin_e
        LSLS     R0,R6,R0
        STRH     R0,[R5, #+26]
//  123 
//  124 	lcd_HD44780_write(data);
        MOVS     R0,R4
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
          CFI FunCall _Z17lcd_HD44780_writeh
        BL       _Z17lcd_HD44780_writeh
//  125 	lcd_HD44780_write(data << 4);
        LSLS     R0,R4,#+4
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
          CFI FunCall _Z17lcd_HD44780_writeh
        BL       _Z17lcd_HD44780_writeh
//  126 }
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock2
//  127 
//  128 //-----------------------------------------------------------------------------

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z22lcd_HD44780_write_datah
        THUMB
//  129 void lcd_HD44780_write_data(uint8_t data) {
_Z22lcd_HD44780_write_datah:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
//  130 	palSetPad(getHwPort(boardConfiguration->HD44780_rs), getHwPin(boardConfiguration->HD44780_rs));
        LDR.W    R0,??DataTable10_4
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+176]
          CFI FunCall _Z9getHwPort11brain_pin_e
        BL       _Z9getHwPort11brain_pin_e
        MOVS     R5,R0
        MOVS     R6,#+1
        LDR.W    R0,??DataTable10_4
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+176]
          CFI FunCall _Z8getHwPin11brain_pin_e
        BL       _Z8getHwPin11brain_pin_e
        LSLS     R0,R6,R0
        STRH     R0,[R5, #+24]
//  131 
//  132 	lcd_HD44780_write(data);
        MOVS     R0,R4
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
          CFI FunCall _Z17lcd_HD44780_writeh
        BL       _Z17lcd_HD44780_writeh
//  133 	lcd_HD44780_write(data << 4);
        LSLS     R0,R4,#+4
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
          CFI FunCall _Z17lcd_HD44780_writeh
        BL       _Z17lcd_HD44780_writeh
//  134 	currentColumn++;
        LDR.W    R0,??DataTable10_5
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+1
        LDR.W    R1,??DataTable10_5
        STR      R0,[R1, #+0]
//  135 
//  136 	palClearPad(getHwPort(boardConfiguration->HD44780_rs), getHwPin(boardConfiguration->HD44780_rs));
        LDR.W    R0,??DataTable10_4
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+176]
          CFI FunCall _Z9getHwPort11brain_pin_e
        BL       _Z9getHwPort11brain_pin_e
        MOVS     R5,R0
        MOVS     R6,#+1
        LDR.W    R0,??DataTable10_4
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+176]
          CFI FunCall _Z8getHwPin11brain_pin_e
        BL       _Z8getHwPin11brain_pin_e
        LSLS     R0,R6,R0
        STRH     R0,[R5, #+26]
//  137 }
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock3
//  138 
//  139 //-----------------------------------------------------------------------------

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function lcd_HD44780_set_position
        THUMB
//  140 void lcd_HD44780_set_position(uint8_t row, uint8_t column) {
lcd_HD44780_set_position:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  141 	efiAssertVoid(row <= engineConfiguration->HD44780height, "invalid row");
        LDR.W    R0,??DataTable10_3
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+520]
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        CMP      R0,R4
        BGE.N    ??lcd_HD44780_set_position_0
        LDR.W    R0,??DataTable10_6
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??lcd_HD44780_set_position_1
//  142 	currentRow = row;
??lcd_HD44780_set_position_0:
        LDR.W    R0,??DataTable10_7
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        STR      R4,[R0, #+0]
//  143 	currentColumn = column;
        LDR.W    R0,??DataTable10_5
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        STR      R5,[R0, #+0]
//  144 	lcd_HD44780_write_command(LCD_HD44780_DDRAM_ADDR + lineStart[row] + column);
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        LDR.W    R0,??DataTable10_8
        LDR      R0,[R0, R4, LSL #+2]
        ADDS     R0,R5,R0
        SUBS     R0,R0,#+128
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
          CFI FunCall _Z25lcd_HD44780_write_commandh
        BL       _Z25lcd_HD44780_write_commandh
//  145 }
??lcd_HD44780_set_position_1:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock4
//  146 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function getCurrentHD44780row
          CFI NoCalls
        THUMB
//  147 int getCurrentHD44780row(void) {
//  148 	return currentRow;
getCurrentHD44780row:
        LDR.W    R0,??DataTable10_7
        LDR      R0,[R0, #+0]
        BX       LR               ;; return
          CFI EndBlock cfiBlock5
//  149 }
//  150 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function getCurrentHD44780column
          CFI NoCalls
        THUMB
//  151 int getCurrentHD44780column(void) {
//  152 	return currentColumn;
getCurrentHD44780column:
        LDR.W    R0,??DataTable10_5
        LDR      R0,[R0, #+0]
        BX       LR               ;; return
          CFI EndBlock cfiBlock6
//  153 }
//  154 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function lcd_HD44780_print_char
        THUMB
//  155 void lcd_HD44780_print_char(char data) {
lcd_HD44780_print_char:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  156 	if (data == '\n') {
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        CMP      R4,#+10
        BNE.N    ??lcd_HD44780_print_char_0
//  157 		lcd_HD44780_set_position(++currentRow, 0);
        LDR.W    R0,??DataTable10_7
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+1
        LDR.W    R1,??DataTable10_7
        STR      R0,[R1, #+0]
        MOVS     R1,#+0
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
          CFI FunCall lcd_HD44780_set_position
        BL       lcd_HD44780_set_position
        B.N      ??lcd_HD44780_print_char_1
//  158 	} else {
//  159 		lcd_HD44780_write_data(data);
??lcd_HD44780_print_char_0:
        MOVS     R0,R4
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
          CFI FunCall _Z22lcd_HD44780_write_datah
        BL       _Z22lcd_HD44780_write_datah
//  160 	}
//  161 }
??lcd_HD44780_print_char_1:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock7
//  162 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function lcd_HD44780_print_string
        THUMB
//  163 void lcd_HD44780_print_string(const char* string) {
lcd_HD44780_print_string:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  164 	while (*string != 0x00)
??lcd_HD44780_print_string_0:
        LDRB     R0,[R4, #+0]
        CMP      R0,#+0
        BEQ.N    ??lcd_HD44780_print_string_1
//  165 		lcd_HD44780_print_char(*string++);
        LDRB     R0,[R4, #+0]
          CFI FunCall lcd_HD44780_print_char
        BL       lcd_HD44780_print_char
        ADDS     R4,R4,#+1
        B.N      ??lcd_HD44780_print_string_0
//  166 }
??lcd_HD44780_print_string_1:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock8
//  167 //getHwPin(boardConfiguration->HD44780_db7)

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function _Z7lcdInfov
        THUMB
//  168 static void lcdInfo(void) {
_Z7lcdInfov:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  169 	scheduleMsg(logger, "HD44780 RS=%s", hwPortname(boardConfiguration->HD44780_rs));
        LDR.N    R0,??DataTable10_4
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+176]
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R2,R0
        LDR.N    R1,??DataTable10_9
        LDR.N    R0,??DataTable10_10
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  170 	scheduleMsg(logger, "HD44780 E=%s", hwPortname(boardConfiguration->HD44780_e));
        LDR.N    R0,??DataTable10_4
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+180]
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R2,R0
        LDR.N    R1,??DataTable10_11
        LDR.N    R0,??DataTable10_10
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  171 	scheduleMsg(logger, "HD44780 D4=%s", hwPortname(boardConfiguration->HD44780_db4));
        LDR.N    R0,??DataTable10_4
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+184]
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R2,R0
        LDR.N    R1,??DataTable10_12
        LDR.N    R0,??DataTable10_10
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  172 	scheduleMsg(logger, "HD44780 D5=%s", hwPortname(boardConfiguration->HD44780_db5));
        LDR.N    R0,??DataTable10_4
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+188]
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R2,R0
        LDR.N    R1,??DataTable10_13
        LDR.N    R0,??DataTable10_10
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  173 	scheduleMsg(logger, "HD44780 D6=%s", hwPortname(boardConfiguration->HD44780_db6));
        LDR.N    R0,??DataTable10_4
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+192]
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R2,R0
        LDR.N    R1,??DataTable10_14
        LDR.N    R0,??DataTable10_10
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  174 	scheduleMsg(logger, "HD44780 D7=%s", hwPortname(boardConfiguration->HD44780_db7));
        LDR.N    R0,??DataTable10_4
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+196]
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R2,R0
        LDR.N    R1,??DataTable10_15
        LDR.N    R0,??DataTable10_10
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  175 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock9
//  176 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function lcd_HD44780_init
        THUMB
//  177 void lcd_HD44780_init(Logging *sharedLogger) {
lcd_HD44780_init:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
//  178 	logger = sharedLogger;
        LDR.N    R0,??DataTable10_10
        STR      R4,[R0, #+0]
//  179 
//  180 	addConsoleAction("lcdinfo", lcdInfo);
        LDR.N    R1,??DataTable10_16
        LDR.N    R0,??DataTable10_17
          CFI FunCall addConsoleAction
        BL       addConsoleAction
//  181 
//  182 	if (engineConfiguration->displayMode > DM_HD44780_OVER_PCF8574) {
        LDR.N    R0,??DataTable10_3
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+504]
        CMP      R0,#+3
        BLT.N    ??lcd_HD44780_init_0
//  183 		firmwareError("Unexpected displayMode %d", engineConfiguration->displayMode);
        LDR.N    R0,??DataTable10_3
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+504]
        LDR.N    R0,??DataTable10_18
          CFI FunCall firmwareError
        BL       firmwareError
//  184 		return;
        B.N      ??lcd_HD44780_init_1
//  185 	}
//  186 
//  187 	printMsg(logger, "lcd_HD44780_init %d", engineConfiguration->displayMode);
??lcd_HD44780_init_0:
        LDR.N    R0,??DataTable10_3
        LDR      R0,[R0, #+0]
        LDR      R2,[R0, #+504]
        LDR.N    R1,??DataTable10_19
        LDR.N    R0,??DataTable10_10
        LDR      R0,[R0, #+0]
          CFI FunCall printMsg
        BL       printMsg
//  188 
//  189 	if (engineConfiguration->displayMode == DM_HD44780) {
        LDR.N    R0,??DataTable10_3
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+504]
        CMP      R0,#+1
        BNE.W    ??lcd_HD44780_init_2
//  190 		// initialize hardware lines
//  191 		mySetPadMode2("lcd RS", boardConfiguration->HD44780_rs, PAL_MODE_OUTPUT_PUSHPULL);
        MOVS     R2,#+1
        LDR.N    R0,??DataTable10_4
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+176]
        LDR.N    R0,??DataTable10_20
          CFI FunCall _Z13mySetPadMode2PKc11brain_pin_ej
        BL       _Z13mySetPadMode2PKc11brain_pin_ej
//  192 		mySetPadMode2("lcd E", boardConfiguration->HD44780_e, PAL_MODE_OUTPUT_PUSHPULL);
        MOVS     R2,#+1
        LDR.N    R0,??DataTable10_4
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+180]
        LDR.N    R0,??DataTable10_21
          CFI FunCall _Z13mySetPadMode2PKc11brain_pin_ej
        BL       _Z13mySetPadMode2PKc11brain_pin_ej
//  193 		mySetPadMode2("lcd DB4", boardConfiguration->HD44780_db4, PAL_MODE_OUTPUT_PUSHPULL);
        MOVS     R2,#+1
        LDR.N    R0,??DataTable10_4
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+184]
        LDR.N    R0,??DataTable10_22
          CFI FunCall _Z13mySetPadMode2PKc11brain_pin_ej
        BL       _Z13mySetPadMode2PKc11brain_pin_ej
//  194 		mySetPadMode2("lcd DB6", boardConfiguration->HD44780_db5, PAL_MODE_OUTPUT_PUSHPULL);
        MOVS     R2,#+1
        LDR.N    R0,??DataTable10_4
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+188]
        LDR.N    R0,??DataTable10_23
          CFI FunCall _Z13mySetPadMode2PKc11brain_pin_ej
        BL       _Z13mySetPadMode2PKc11brain_pin_ej
//  195 		mySetPadMode2("lcd DB7", boardConfiguration->HD44780_db6, PAL_MODE_OUTPUT_PUSHPULL);
        MOVS     R2,#+1
        LDR.N    R0,??DataTable10_4
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+192]
        LDR.N    R0,??DataTable10_24
          CFI FunCall _Z13mySetPadMode2PKc11brain_pin_ej
        BL       _Z13mySetPadMode2PKc11brain_pin_ej
//  196 		mySetPadMode2("lcd DB8", boardConfiguration->HD44780_db7, PAL_MODE_OUTPUT_PUSHPULL);
        MOVS     R2,#+1
        LDR.N    R0,??DataTable10_4
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+196]
        LDR.N    R0,??DataTable10_25
          CFI FunCall _Z13mySetPadMode2PKc11brain_pin_ej
        BL       _Z13mySetPadMode2PKc11brain_pin_ej
//  197 		// and zero values
//  198 		palWritePad(getHwPort(boardConfiguration->HD44780_rs), getHwPin(boardConfiguration->HD44780_rs), 0);
        LDR.N    R0,??DataTable10_4
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+176]
          CFI FunCall _Z9getHwPort11brain_pin_e
        BL       _Z9getHwPort11brain_pin_e
        MOVS     R5,R0
        LDR.N    R0,??DataTable10_4
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+176]
          CFI FunCall _Z8getHwPin11brain_pin_e
        BL       _Z8getHwPin11brain_pin_e
        MOVS     R1,#+1
        ADDS     R0,R0,#+16
        LSLS     R6,R1,R0
        MOVS     R7,#+0
        LDR.N    R0,??DataTable10_4
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+176]
          CFI FunCall _Z8getHwPin11brain_pin_e
        BL       _Z8getHwPin11brain_pin_e
        LSLS     R0,R7,R0
        ORRS     R0,R0,R6
        STR      R0,[R5, #+24]
//  199 		palWritePad(getHwPort(boardConfiguration->HD44780_e), getHwPin(boardConfiguration->HD44780_e), 0);
        LDR.N    R0,??DataTable10_4
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+180]
          CFI FunCall _Z9getHwPort11brain_pin_e
        BL       _Z9getHwPort11brain_pin_e
        MOVS     R5,R0
        LDR.N    R0,??DataTable10_4
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+180]
          CFI FunCall _Z8getHwPin11brain_pin_e
        BL       _Z8getHwPin11brain_pin_e
        MOVS     R1,#+1
        ADDS     R0,R0,#+16
        LSLS     R6,R1,R0
        MOVS     R7,#+0
        LDR.N    R0,??DataTable10_4
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+180]
          CFI FunCall _Z8getHwPin11brain_pin_e
        BL       _Z8getHwPin11brain_pin_e
        LSLS     R0,R7,R0
        ORRS     R0,R0,R6
        STR      R0,[R5, #+24]
//  200 		palWritePad(getHwPort(boardConfiguration->HD44780_db4), getHwPin(boardConfiguration->HD44780_db4), 0);
        LDR.N    R0,??DataTable10_4
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+184]
          CFI FunCall _Z9getHwPort11brain_pin_e
        BL       _Z9getHwPort11brain_pin_e
        MOVS     R5,R0
        LDR.N    R0,??DataTable10_4
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+184]
          CFI FunCall _Z8getHwPin11brain_pin_e
        BL       _Z8getHwPin11brain_pin_e
        MOVS     R1,#+1
        ADDS     R0,R0,#+16
        LSLS     R6,R1,R0
        MOVS     R7,#+0
        LDR.N    R0,??DataTable10_4
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+184]
          CFI FunCall _Z8getHwPin11brain_pin_e
        BL       _Z8getHwPin11brain_pin_e
        LSLS     R0,R7,R0
        ORRS     R0,R0,R6
        STR      R0,[R5, #+24]
//  201 		palWritePad(getHwPort(boardConfiguration->HD44780_db5), getHwPin(boardConfiguration->HD44780_db5), 0);
        LDR.N    R0,??DataTable10_4
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+188]
          CFI FunCall _Z9getHwPort11brain_pin_e
        BL       _Z9getHwPort11brain_pin_e
        MOVS     R5,R0
        LDR.N    R0,??DataTable10_4
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+188]
          CFI FunCall _Z8getHwPin11brain_pin_e
        BL       _Z8getHwPin11brain_pin_e
        MOVS     R1,#+1
        ADDS     R0,R0,#+16
        LSLS     R6,R1,R0
        MOVS     R7,#+0
        LDR.N    R0,??DataTable10_4
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+188]
          CFI FunCall _Z8getHwPin11brain_pin_e
        BL       _Z8getHwPin11brain_pin_e
        LSLS     R0,R7,R0
        ORRS     R0,R0,R6
        STR      R0,[R5, #+24]
//  202 		palWritePad(getHwPort(boardConfiguration->HD44780_db6), getHwPin(boardConfiguration->HD44780_db6), 0);
        LDR.N    R0,??DataTable10_4
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+192]
          CFI FunCall _Z9getHwPort11brain_pin_e
        BL       _Z9getHwPort11brain_pin_e
        MOVS     R5,R0
        LDR.N    R0,??DataTable10_4
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+192]
          CFI FunCall _Z8getHwPin11brain_pin_e
        BL       _Z8getHwPin11brain_pin_e
        MOVS     R1,#+1
        ADDS     R0,R0,#+16
        LSLS     R6,R1,R0
        MOVS     R7,#+0
        LDR.N    R0,??DataTable10_4
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+192]
          CFI FunCall _Z8getHwPin11brain_pin_e
        BL       _Z8getHwPin11brain_pin_e
        LSLS     R0,R7,R0
        ORRS     R0,R0,R6
        STR      R0,[R5, #+24]
//  203 		palWritePad(getHwPort(boardConfiguration->HD44780_db7), getHwPin(boardConfiguration->HD44780_db7), 0);
        LDR.N    R0,??DataTable10_4
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+196]
          CFI FunCall _Z9getHwPort11brain_pin_e
        BL       _Z9getHwPort11brain_pin_e
        MOVS     R5,R0
        LDR.N    R0,??DataTable10_4
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+196]
          CFI FunCall _Z8getHwPin11brain_pin_e
        BL       _Z8getHwPin11brain_pin_e
        MOVS     R1,#+1
        ADDS     R0,R0,#+16
        LSLS     R6,R1,R0
        MOVS     R7,#+0
        LDR.N    R0,??DataTable10_4
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+196]
          CFI FunCall _Z8getHwPin11brain_pin_e
        BL       _Z8getHwPin11brain_pin_e
        LSLS     R0,R7,R0
        ORRS     R0,R0,R6
        STR      R0,[R5, #+24]
//  204 	}
//  205 
//  206 	chThdSleepMilliseconds(20); // LCD needs some time to wake up
??lcd_HD44780_init_2:
        MOVS     R0,#+20
          CFI FunCall chThdSleep
        BL       chThdSleep
//  207 	lcd_HD44780_write(LCD_HD44780_RESET); // reset 1x
        MOVS     R0,#+48
          CFI FunCall _Z17lcd_HD44780_writeh
        BL       _Z17lcd_HD44780_writeh
//  208 	chThdSleepMilliseconds(1);
        MOVS     R0,#+1
          CFI FunCall chThdSleep
        BL       chThdSleep
//  209 	lcd_HD44780_write(LCD_HD44780_RESET); // reset 2x
        MOVS     R0,#+48
          CFI FunCall _Z17lcd_HD44780_writeh
        BL       _Z17lcd_HD44780_writeh
//  210 	lcd_HD44780_write(LCD_HD44780_RESET); // reset 3x
        MOVS     R0,#+48
          CFI FunCall _Z17lcd_HD44780_writeh
        BL       _Z17lcd_HD44780_writeh
//  211 
//  212 	lcd_HD44780_write(LCD_HD44780_4_BIT_BUS);	// 4 bit, 2 line
        MOVS     R0,#+32
          CFI FunCall _Z17lcd_HD44780_writeh
        BL       _Z17lcd_HD44780_writeh
//  213 	chThdSleepMicroseconds(40);
        MOVS     R0,#+1
          CFI FunCall chThdSleep
        BL       chThdSleep
//  214 
//  215 	lcd_HD44780_write(LCD_HD44780_4_BIT_BUS);	// 4 bit, 2 line
        MOVS     R0,#+32
          CFI FunCall _Z17lcd_HD44780_writeh
        BL       _Z17lcd_HD44780_writeh
//  216 	lcd_HD44780_write(0x80);
        MOVS     R0,#+128
          CFI FunCall _Z17lcd_HD44780_writeh
        BL       _Z17lcd_HD44780_writeh
//  217 	chThdSleepMicroseconds(40);
        MOVS     R0,#+1
          CFI FunCall chThdSleep
        BL       chThdSleep
//  218 
//  219 	lcd_HD44780_write_command(0x08);	// display and cursor control
        MOVS     R0,#+8
          CFI FunCall _Z25lcd_HD44780_write_commandh
        BL       _Z25lcd_HD44780_write_commandh
//  220 	chThdSleepMicroseconds(40);
        MOVS     R0,#+1
          CFI FunCall chThdSleep
        BL       chThdSleep
//  221 
//  222 	lcd_HD44780_write_command(LCD_HD44780_DISPLAY_CLEAR);
        MOVS     R0,#+1
          CFI FunCall _Z25lcd_HD44780_write_commandh
        BL       _Z25lcd_HD44780_write_commandh
//  223 	chThdSleepMilliseconds(2);
        MOVS     R0,#+2
          CFI FunCall chThdSleep
        BL       chThdSleep
//  224 
//  225 	lcd_HD44780_write_command(LCD_HD44780_SHIFT_CURSOR_RIGHT);
        MOVS     R0,#+6
          CFI FunCall _Z25lcd_HD44780_write_commandh
        BL       _Z25lcd_HD44780_write_commandh
//  226 	chThdSleepMilliseconds(2);
        MOVS     R0,#+2
          CFI FunCall chThdSleep
        BL       chThdSleep
//  227 
//  228 	lcd_HD44780_write_command(LCD_HD44780_DISPLAY_ON);
        MOVS     R0,#+12
          CFI FunCall _Z25lcd_HD44780_write_commandh
        BL       _Z25lcd_HD44780_write_commandh
//  229 
//  230 	lcd_HD44780_set_position(0, 0);
        MOVS     R1,#+0
        MOVS     R0,#+0
          CFI FunCall lcd_HD44780_set_position
        BL       lcd_HD44780_set_position
//  231 	printMsg(logger, "lcd_HD44780_init() done");
        LDR.N    R1,??DataTable10_26
        LDR.N    R0,??DataTable10_10
        LDR      R0,[R0, #+0]
          CFI FunCall printMsg
        BL       printMsg
//  232 }
??lcd_HD44780_init_1:
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock10
//  233 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function lcdShowFatalMessage
        THUMB
//  234 void lcdShowFatalMessage(char *message) {
lcdShowFatalMessage:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  235 	BUSY_WAIT_DELAY = TRUE;
        LDR.N    R0,??DataTable10
        MOVS     R1,#+1
        STR      R1,[R0, #+0]
//  236 	lcd_HD44780_set_position(0, 0);
        MOVS     R1,#+0
        MOVS     R0,#+0
          CFI FunCall lcd_HD44780_set_position
        BL       lcd_HD44780_set_position
//  237 	lcd_HD44780_print_string("fatal\n");
        LDR.N    R0,??DataTable10_27
          CFI FunCall lcd_HD44780_print_string
        BL       lcd_HD44780_print_string
//  238 	lcd_HD44780_print_string(message);
        MOVS     R0,R4
          CFI FunCall lcd_HD44780_print_string
        BL       lcd_HD44780_print_string
//  239 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock11

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10:
        DC32     BUSY_WAIT_DELAY

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_1:
        DC32     `?<Constant "true">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_2:
        DC32     0xf4240

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_3:
        DC32     engineConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_4:
        DC32     boardConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_5:
        DC32     currentColumn

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_6:
        DC32     `?<Constant "invalid row">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_7:
        DC32     currentRow

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_8:
        DC32     lineStart

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_9:
        DC32     `?<Constant "HD44780 RS=%s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_10:
        DC32     logger

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_11:
        DC32     `?<Constant "HD44780 E=%s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_12:
        DC32     `?<Constant "HD44780 D4=%s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_13:
        DC32     `?<Constant "HD44780 D5=%s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_14:
        DC32     `?<Constant "HD44780 D6=%s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_15:
        DC32     `?<Constant "HD44780 D7=%s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_16:
        DC32     _Z7lcdInfov

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_17:
        DC32     `?<Constant "lcdinfo">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_18:
        DC32     `?<Constant "Unexpected displayMod...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_19:
        DC32     `?<Constant "lcd_HD44780_init %d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_20:
        DC32     `?<Constant "lcd RS">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_21:
        DC32     `?<Constant "lcd E">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_22:
        DC32     `?<Constant "lcd DB4">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_23:
        DC32     `?<Constant "lcd DB6">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_24:
        DC32     `?<Constant "lcd DB7">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_25:
        DC32     `?<Constant "lcd DB8">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_26:
        DC32     `?<Constant "lcd_HD44780_init() done">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_27:
        DC32     `?<Constant "fatal\\n">`

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        END
//  240 
//  241 #endif /* EFI_HD44780_LCD */
// 
//    17 bytes in section .bss
//   268 bytes in section .rodata
// 1 766 bytes in section .text
// 
// 1 766 bytes of CODE  memory
//   268 bytes of CONST memory
//    17 bytes of DATA  memory
//
//Errors: none
//Warnings: 1
