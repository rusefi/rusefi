/**
 * @file rusEfiStartup.S
 * rusEfi startup assembly
 *
 * We have ChibiOS '_crt0_entry' jumping here and then we jump back.
 * There is probably no stack at this point to invoke a method.
 */

// todo: I have no idea which lines are doing what
// todo: one of these is resolving 'Error: lo register required -- `ldr SP,[R0,#0]'' :)
    .syntax unified
	.cpu    cortex-m3
	.thumb
	.align  2
	.thumb_func
// end of 'I have no idea'

	// ChibiOS defines it's Reset_Handler 'weak' - by defining out handler this becomes firmware entry point
	.global Reset_Handler
Reset_Handler:
// [rusefi][DFU][start]
// Clive Two.Zero is the God of ST community forum
// Device specific, if in doubt RTFM
LDR R0, =0x2001FFF0 // End of SRAM for your CPU
LDR R1, =0xDEADBEEF // magic value
LDR R2, [R0, #0]
STR R0, [R0, #0] // Invalidate
CMP R2, R1
BEQ UseDFU
	// DFU bootloader not needed, jump back to normal ChibiOS startup
         bl          _crt0_entry
UseDFU:
	// AN2606 Application note
	// STM32 microcontroller system memory boot mode
#include "dfu_init.h"
BX R0 // this jumps to DFU bootloader
	// I believe we are never executing this line
// [rusefi][DFU][end]
