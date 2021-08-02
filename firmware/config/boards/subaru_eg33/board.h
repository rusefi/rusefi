/**
 * @file boards/subaru_eg33/board.c
 *
 * @date Feb 06, 2021
 * @author Andrey Gusakov, 2021
 */

#ifndef BOARD_H
#define BOARD_H

/*
 * Board identifier.
 */
#define BOARD_NAME                  "Subaru EG33/STM32F765 for RusEFI"

#define EFI_USB_AF					10U
//#define EFI_USB_SERIAL_ID			GPIOA_10
#define EFI_USB_SERIAL_DM			GPIOA_11
#define EFI_USB_SERIAL_DP			GPIOA_12

/*
 * Board oscillators-related settings.
 * NOTE: LSE not fitted.
 */
#if !defined(STM32_LSECLK)
#define STM32_LSECLK                32768U
#endif

#define STM32_LSEDRV                (3U << 3U)

/*
 * Board voltages.
 * Required for performance limits calculation.
 */
#define STM32_VDD                   330U

/*
 * MCU type as defined in the ST header.
 * this declaration for stm32_registry.h
 */
#ifndef STM32F765xx
#define STM32F765xx
#endif

#endif /* BOARD_H */
