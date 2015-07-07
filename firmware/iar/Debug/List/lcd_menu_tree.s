///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:52 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\controllers\alg /
//                       o\lcd_menu_tree.cpp                                  /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\controllers\alg /
//                       o\lcd_menu_tree.cpp -lCN F:\stuff\rusefi_sourceforge /
//                       \firmware\iar\Debug\List\ -lA                        /
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
//                       lcd_menu_tree.s                                      /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME lcd_menu_tree

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        PUBLIC _ZN8MenuItem15baseConstructorEPS_10lcd_line_ePKcPFvvE
        PUBLIC _ZN8MenuItemC1EPS_10lcd_line_e
        PUBLIC _ZN8MenuItemC1EPS_PKc
        PUBLIC _ZN8MenuItemC1EPS_PKcPFvvE
        PUBLIC _ZN8MenuItemC2EPS_10lcd_line_e
        PUBLIC _ZN8MenuItemC2EPS_PKc
        PUBLIC _ZN8MenuItemC2EPS_PKcPFvvE
        PUBLIC _ZN8MenuTree12enterSubMenuEv
        PUBLIC _ZN8MenuTree4backEv
        PUBLIC _ZN8MenuTree4initEP8MenuItemi
        PUBLIC _ZN8MenuTree8nextItemEv
        PUBLIC _ZN8MenuTreeC1EP8MenuItem
        PUBLIC _ZN8MenuTreeC2EP8MenuItem
        
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
          CFI Function _ZN8MenuItemC2EPS_PKcPFvvE
        THUMB
// __code __interwork __softfp MenuItem::subobject MenuItem(MenuItem *, char const *, void (*)())
_ZN8MenuItemC2EPS_PKcPFvvE:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN8MenuItemC1EPS_PKcPFvvE
        BL       _ZN8MenuItemC1EPS_PKcPFvvE
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock0

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _ZN8MenuItemC2EPS_PKc
        THUMB
// __code __interwork __softfp MenuItem::subobject MenuItem(MenuItem *, char const *)
_ZN8MenuItemC2EPS_PKc:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN8MenuItemC1EPS_PKc
        BL       _ZN8MenuItemC1EPS_PKc
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock1

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _ZN8MenuItemC2EPS_10lcd_line_e
        THUMB
// __code __interwork __softfp MenuItem::subobject MenuItem(MenuItem *, lcd_line_e)
_ZN8MenuItemC2EPS_10lcd_line_e:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        UXTB     R2,R2            ;; ZeroExt  R2,R2,#+24,#+24
        MOVS     R0,R4
          CFI FunCall _ZN8MenuItemC1EPS_10lcd_line_e
        BL       _ZN8MenuItemC1EPS_10lcd_line_e
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock2
// F:\stuff\rusefi_sourceforge\firmware\controllers\algo\lcd_menu_tree.cpp
//    1 /**
//    2  * @file	lcd_menu_tree.cpp
//    3  *
//    4  * @date Jan 6, 2015
//    5  * @author Andrey Belomutskiy, (c) 2012-2015
//    6  */
//    7 
//    8 #include "stddef.h"
//    9 #include "lcd_menu_tree.h"
//   10 #include "error_handling.h"
//   11 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _ZN8MenuTreeC2EP8MenuItem
        THUMB
// __code __interwork __softfp MenuTree::subobject MenuTree(MenuItem *)
_ZN8MenuTreeC2EP8MenuItem:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN8MenuTreeC1EP8MenuItem
        BL       _ZN8MenuTreeC1EP8MenuItem
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock3

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _ZN8MenuTreeC1EP8MenuItem
          CFI NoCalls
        THUMB
//   12 MenuTree::MenuTree(MenuItem *root) {
//   13 	this->root = root;
_ZN8MenuTreeC1EP8MenuItem:
        STR      R1,[R0, #+0]
//   14 	current = NULL;
        MOVS     R2,#+0
        STR      R2,[R0, #+8]
//   15 	linesCount = 0;
        MOVS     R2,#+0
        STR      R2,[R0, #+4]
//   16 	topVisible = NULL;
        MOVS     R2,#+0
        STR      R2,[R0, #+12]
//   17 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock4
//   18 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _ZN8MenuTree4initEP8MenuItemi
          CFI NoCalls
        THUMB
//   19 void MenuTree::init(MenuItem *first, int linesCount) {
//   20 	this->linesCount = linesCount;
_ZN8MenuTree4initEP8MenuItemi:
        STR      R2,[R0, #+4]
//   21 	current = first;
        STR      R1,[R0, #+8]
//   22 	topVisible = first;
        STR      R1,[R0, #+12]
//   23 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock5
//   24 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _ZN8MenuTree12enterSubMenuEv
        THUMB
//   25 void MenuTree::enterSubMenu(void) {
_ZN8MenuTree12enterSubMenuEv:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//   26 	if (current->firstChild != NULL) {
        LDR      R0,[R4, #+8]
        LDR      R0,[R0, #+20]
        CMP      R0,#+0
        BEQ.N    ??enterSubMenu_0
//   27 		current = topVisible = current->firstChild;
        LDR      R0,[R4, #+8]
        LDR      R0,[R0, #+20]
        STR      R0,[R4, #+12]
        STR      R0,[R4, #+8]
        B.N      ??enterSubMenu_1
//   28 	} else if (current->callback != NULL) {
??enterSubMenu_0:
        LDR      R0,[R4, #+8]
        LDR      R0,[R0, #+32]
        CMP      R0,#+0
        BEQ.N    ??enterSubMenu_1
//   29 		VoidCallback cb = current->callback;
        LDR      R0,[R4, #+8]
        LDR      R5,[R0, #+32]
//   30 		cb();
          CFI FunCall
        BLX      R5
//   31 	}
//   32 }
??enterSubMenu_1:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock6
//   33 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function _ZN8MenuTree4backEv
          CFI NoCalls
        THUMB
//   34 void MenuTree::back(void) {
//   35 	if (current->parent == root)
_ZN8MenuTree4backEv:
        LDR      R1,[R0, #+8]
        LDR      R1,[R1, #+12]
        LDR      R2,[R0, #+0]
        CMP      R1,R2
        BEQ.N    ??back_0
//   36 		return; // we are on the top level already
//   37 	current = topVisible = current->parent->topOfTheList;
??back_1:
        LDR      R1,[R0, #+8]
        LDR      R1,[R1, #+12]
        LDR      R1,[R1, #+16]
        STR      R1,[R0, #+12]
        STR      R1,[R0, #+8]
//   38 }
??back_0:
        BX       LR               ;; return
          CFI EndBlock cfiBlock7
//   39 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function _ZN8MenuTree8nextItemEv
          CFI NoCalls
        THUMB
//   40 void MenuTree::nextItem(void) {
//   41 	if (current->next == NULL) {
_ZN8MenuTree8nextItemEv:
        LDR      R1,[R0, #+8]
        LDR      R1,[R1, #+28]
        CMP      R1,#+0
        BNE.N    ??nextItem_0
//   42 		current = topVisible = current->topOfTheList;
        LDR      R1,[R0, #+8]
        LDR      R1,[R1, #+16]
        STR      R1,[R0, #+12]
        STR      R1,[R0, #+8]
//   43 		return;
        B.N      ??nextItem_1
//   44 	}
//   45 	current = current->next;
??nextItem_0:
        LDR      R1,[R0, #+8]
        LDR      R1,[R1, #+28]
        STR      R1,[R0, #+8]
//   46 	if (current->index - topVisible->index == linesCount)
        LDR      R1,[R0, #+8]
        LDR      R1,[R1, #+8]
        LDR      R2,[R0, #+12]
        LDR      R2,[R2, #+8]
        SUBS     R1,R1,R2
        LDR      R2,[R0, #+4]
        CMP      R1,R2
        BNE.N    ??nextItem_2
//   47 		topVisible = topVisible->next;
        LDR      R1,[R0, #+12]
        LDR      R1,[R1, #+28]
        STR      R1,[R0, #+12]
//   48 }
??nextItem_2:
??nextItem_1:
        BX       LR               ;; return
          CFI EndBlock cfiBlock8
//   49 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function _ZN8MenuItemC1EPS_PKcPFvvE
        THUMB
//   50 MenuItem::MenuItem(MenuItem * parent, const char *text, VoidCallback callback) {
_ZN8MenuItemC1EPS_PKcPFvvE:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
        MOVS     R7,R3
//   51 	baseConstructor(parent, LL_STRING, text, callback);
        STR      R7,[SP, #+0]
        MOVS     R3,R6
        MOVS     R2,#+0
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _ZN8MenuItem15baseConstructorEPS_10lcd_line_ePKcPFvvE
        BL       _ZN8MenuItem15baseConstructorEPS_10lcd_line_ePKcPFvvE
//   52 }
        MOVS     R0,R4
        POP      {R1,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock9
//   53 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function _ZN8MenuItemC1EPS_PKc
        THUMB
//   54 MenuItem::MenuItem(MenuItem * parent, const char *text) {
_ZN8MenuItemC1EPS_PKc:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        SUB      SP,SP,#+8
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//   55 	baseConstructor(parent, LL_STRING, text, NULL);
        MOVS     R0,#+0
        STR      R0,[SP, #+0]
        MOVS     R3,R6
        MOVS     R2,#+0
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _ZN8MenuItem15baseConstructorEPS_10lcd_line_ePKcPFvvE
        BL       _ZN8MenuItem15baseConstructorEPS_10lcd_line_ePKcPFvvE
//   56 }
        MOVS     R0,R4
        POP      {R1,R2,R4-R6,PC}  ;; return
          CFI EndBlock cfiBlock10
//   57 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function _ZN8MenuItemC1EPS_10lcd_line_e
        THUMB
//   58 MenuItem::MenuItem(MenuItem * parent, lcd_line_e lcdLine)  {
_ZN8MenuItemC1EPS_10lcd_line_e:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        SUB      SP,SP,#+8
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//   59 	baseConstructor(parent, lcdLine, NULL, NULL);
        MOVS     R0,#+0
        STR      R0,[SP, #+0]
        MOVS     R3,#+0
        MOVS     R2,R6
        UXTB     R2,R2            ;; ZeroExt  R2,R2,#+24,#+24
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _ZN8MenuItem15baseConstructorEPS_10lcd_line_ePKcPFvvE
        BL       _ZN8MenuItem15baseConstructorEPS_10lcd_line_ePKcPFvvE
//   60 }
        MOVS     R0,R4
        POP      {R1,R2,R4-R6,PC}  ;; return
          CFI EndBlock cfiBlock11
//   61 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function _ZN8MenuItem15baseConstructorEPS_10lcd_line_ePKcPFvvE
          CFI NoCalls
        THUMB
//   62 void MenuItem::baseConstructor(MenuItem * parent, lcd_line_e lcdLine, const char *text, VoidCallback callback) {
_ZN8MenuItem15baseConstructorEPS_10lcd_line_ePKcPFvvE:
        PUSH     {R4,R5}
          CFI R5 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        LDR      R4,[SP, #+8]
//   63 	this->parent = parent;
        STR      R1,[R0, #+12]
//   64 	this->lcdLine = lcdLine;
        STRB     R2,[R0, #+4]
//   65 	this->text = text;
        STR      R3,[R0, #+0]
//   66 	this->callback = callback;
        STR      R4,[R0, #+32]
//   67 
//   68 	firstChild = NULL;
        MOVS     R5,#+0
        STR      R5,[R0, #+20]
//   69 	lastChild = NULL;
        MOVS     R5,#+0
        STR      R5,[R0, #+24]
//   70 	topOfTheList = NULL;
        MOVS     R5,#+0
        STR      R5,[R0, #+16]
//   71 	next = NULL;
        MOVS     R5,#+0
        STR      R5,[R0, #+28]
//   72 	index = 0;
        MOVS     R5,#+0
        STR      R5,[R0, #+8]
//   73 
//   74 	// root element has NULL parent
//   75 	if (parent != NULL) {
        CMP      R1,#+0
        BEQ.N    ??baseConstructor_0
//   76 		if (parent->firstChild == NULL) {
        LDR      R5,[R1, #+20]
        CMP      R5,#+0
        BNE.N    ??baseConstructor_1
//   77 			topOfTheList = this;
        STR      R0,[R0, #+16]
//   78 			parent->firstChild = this;
        STR      R0,[R1, #+20]
//   79 		}
//   80 		if (parent->lastChild != NULL) {
??baseConstructor_1:
        LDR      R5,[R1, #+24]
        CMP      R5,#+0
        BEQ.N    ??baseConstructor_2
//   81 			index = parent->lastChild->index + 1;
        LDR      R5,[R1, #+24]
        LDR      R5,[R5, #+8]
        ADDS     R5,R5,#+1
        STR      R5,[R0, #+8]
//   82 			topOfTheList = parent->lastChild->topOfTheList;
        LDR      R5,[R1, #+24]
        LDR      R5,[R5, #+16]
        STR      R5,[R0, #+16]
//   83 			parent->lastChild->next = this;
        LDR      R5,[R1, #+24]
        STR      R0,[R5, #+28]
//   84 		}
//   85 		parent->lastChild = this;
??baseConstructor_2:
        STR      R0,[R1, #+24]
//   86 	}
//   87 }
??baseConstructor_0:
        POP      {R4,R5}
          CFI R4 SameValue
          CFI R5 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock12

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
// 348 bytes in section .text
// 
// 348 bytes of CODE memory
//
//Errors: none
//Warnings: none
