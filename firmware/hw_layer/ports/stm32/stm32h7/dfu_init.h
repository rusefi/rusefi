
// https://community.st.com/s/question/0D50X00009bNBluSAG/boot-stm32h7-after-power-up
// https://community.st.com/s/article/STM32H7-bootloader-jump-from-application

#ifndef HW_LAYER_PORTS_STM32_STM32F7_DFU_INIT_H_
#define HW_LAYER_PORTS_STM32_STM32F7_DFU_INIT_H_

// System control block registers base
LDR R1, =0xE000ED00

// location of system memory (DFU bootloader)
LDR R0, =0x1FF09800

// Set the vector table offset to the sysmem image
STR R0, [R1, #8]

// load the stack pointer
LDR SP, [R0, #0]

// prepare for jump to bootloader
LDR R0, [R0, #4]

#endif /* HW_LAYER_PORTS_STM32_STM32F7_DFU_INIT_H_ */
