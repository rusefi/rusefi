#pragma once

// Highest priority (can even interrupt kernel): Fast EXTI input

#ifdef EFI_BOOTLOADER
	// Bootloader: use normal ChibiOS-defined interrupts
	#define EFI_IRQ_EXTI_PRIORITY             6

#else // not EFI_BOOTLOADER
	// Not bootloader: EXTI gets maximum priority and custom interruts
	#define EFI_IRQ_EXTI_PRIORITY             CORTEX_MAXIMUM_PRIORITY

	#define STM32_DISABLE_EXTI0_HANDLER
	#define STM32_DISABLE_EXTI1_HANDLER
	#define STM32_DISABLE_EXTI2_HANDLER
	#define STM32_DISABLE_EXTI3_HANDLER
	#define STM32_DISABLE_EXTI4_HANDLER
	#define STM32_DISABLE_EXTI5_9_HANDLER
	#define STM32_DISABLE_EXTI10_15_HANDLER
#endif // EFI_BOOTLOADER

// Fast EXTI handoff IRQ priority
// see digital_input_exti.cpp
// Trigger decode happens inside this interrupt
#define EFI_IRQ_EXTI_HANDOFF_PRIORITY     3

// Precise scheduling timer
#define EFI_IRQ_SCHEDULING_TIMER_PRIORITY 4

// Relatively high priority for ADC - needed for MAP sensor sampling
#define EFI_IRQ_ADC_PRIORITY 5

// Hardware UART serial
#define EFI_IRQ_UART_PRIORITY 6

#define EFI_IRQ_SYSTICK_PRIORITY 8

// SDMMC and SPI, used for SD card and WiFi
#define EFI_IRQ_SDMMC_PRIORITY 10
#define EFI_IRQ_SPI_PRIORITY 10

#define EFI_IRQ_CAN_PRIORITY 11

#define EFI_IRQ_USB_PRIORITY 14
