.syntax unified
.cpu    cortex-m3
.thumb
.align  2
.thumb_func

.type HardFault_Handler, %function
.type UsageFault_Handler, %function
.type MemManage_Handler, %function

.global HardFault_Handler
.global BusFault_Handler
HardFault_Handler:
BusFault_Handler:
	tst LR, #4
	ite EQ
	mrseq R0, MSP
	mrsne R0, PSP
	b HardFault_Handler_C

.global UsageFault_Handler
UsageFault_Handler:
	tst LR, #4
	ite EQ
	mrseq R0, MSP
	mrsne R0, PSP
	b UsageFault_Handler_C

.global MemManage_Handler
MemManage_Handler:
	tst LR, #4
	ite EQ
	mrseq R0, MSP
	mrsne R0, PSP
	b MemManage_Handler_C
