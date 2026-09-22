#pragma once

#ifdef EFI_BOOTLOADER
#define EFI_IRQ_EXTI_PRIORITY             6
#else
#define EFI_IRQ_EXTI_PRIORITY             CORTEX_MAXIMUM_PRIORITY
#endif

/* Scheduled spark/injection remains able to preempt both trigger decoding
 * and the L9779 watchdog burst. The watchdog masks priority 4 and below
 * while its multi-frame transaction is in progress. */
#define EFI_IRQ_SCHEDULING_TIMER_PRIORITY 3
#define EFI_IRQ_EXTI_HANDOFF_PRIORITY     4
#define EFI_IRQ_L9779_WDA_PRIORITY        5
#define EFI_IRQ_ADC_PRIORITY 5
#define EFI_IRQ_UART_PRIORITY 6
#define EFI_IRQ_SYSTICK_PRIORITY 8
#define EFI_IRQ_SDMMC_PRIORITY 10
#define EFI_IRQ_SPI_PRIORITY 10
#define EFI_IRQ_CAN_PRIORITY 11
#define EFI_IRQ_USB_PRIORITY 14
