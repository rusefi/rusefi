/*
 * dfu_init.h
 *
 * @date Aug 3, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#ifndef HW_LAYER_PORTS_STM32_STM32F7_DFU_INIT_H_
#define HW_LAYER_PORTS_STM32_STM32F7_DFU_INIT_H_

// System control block registers base
LDR R1, =0xE000ED00

// location of system memory (DFU bootloader)
LDR R0, =0x1FF00000

// Set the vector table offset to the sysmem image
STR R0, [R1, #8]

// load the stack pointer
LDR SP, [R0, #0]

// prepare for jump to bootloader
LDR R0, [R0, #4]

#endif /* HW_LAYER_PORTS_STM32_STM32F7_DFU_INIT_H_ */
