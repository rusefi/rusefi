/**
 * @file boards/subaru_eg33/efifeatures.h
 *
 * @brief In this header we can override efifeatures.h.
 *
 * @date Feb 06, 2021
 * @author Andrey Gusakov, 2021
 */

#include "../../stm32f7ems/efifeatures.h"

#ifndef EFIFEATURES_SUBARUEG33_H_
#define EFIFEATURES_SUBARUEG33_H_

/* LEDs */
#undef LED_ERROR_BRAIN_PIN_MODE
#define LED_ERROR_BRAIN_PIN_MODE 		INVERTED_OUTPUT
#undef LED_WARNING_BRAIN_PIN_MODE
#define LED_WARNING_BRAIN_PIN_MODE		INVERTED_OUTPUT
#undef LED_RUNING_BRAIN_PIN_MODE
#define LED_RUNING_BRAIN_PIN_MODE		INVERTED_OUTPUT

/* debug console */
#undef TS_PRIMARY_SERIAL
#undef TS_PRIMARY_UART
#define TS_PRIMARY_SERIAL		SD1

// no secondary channel
#undef TS_SECONDARY_UART
#undef TS_SECONDARY_SERIAL

/* Knock detection */
#undef EFI_HIP_9011
#define EFI_HIP_9011				TRUE

#undef BOARD_TLE6240_COUNT
#define BOARD_TLE6240_COUNT			1

#define TLE6240_RESET_PORT			GPIOH
#define TLE6240_RESET_PAD			6
#define TLE6240_DIRECT_IO				\
		/* IN1..3 grounded */			\
		[0] = {.port = NULL, .pad = 0},	\
		[1] = {.port = NULL, .pad = 0},	\
		/* IN3  - D_TACH_OUT */			\
		[2] = {.port = GPIOG, .pad = 2},\
		/* IN4  - grounded */			\
		[3] = {.port = NULL, .pad = 0},	\
		/* IN9..10 grounded */			\
		[4] = {.port = NULL, .pad = 15},\
		[5] = {.port = NULL, .pad = 14},\
		/* IN11 - D_IDLE_OPEN */		\
		[6] = {.port = GPIOC, .pad = 6},\
		/* IN12 - D_IDLE_CLOSE */		\
		[7] = {.port = GPIOC, .pad = 8}

#undef BOARD_MC33972_COUNT
#define BOARD_MC33972_COUNT			1

#undef BOARD_MC33810_COUNT
#define BOARD_MC33810_COUNT			2

#undef BOARD_TLE8888_COUNT
#define BOARD_TLE8888_COUNT			0

#undef EFI_RTC
#define EFI_RTC						FALSE

#undef EFI_POTENTIOMETER
#define EFI_POTENTIOMETER			FALSE

#undef EFI_MAX_31855
#define EFI_MAX_31855				FALSE

#undef EFI_HD44780_LCD
#define EFI_HD44780_LCD				FALSE

#undef EFI_IDLE_CONTROL
#define EFI_IDLE_CONTROL			TRUE

#undef EFI_USB_SERIAL
#define EFI_USB_SERIAL				TRUE

#undef EFI_FUEL_PUMP
#define EFI_FUEL_PUMP				TRUE

#define EFI_SDC_DEVICE				SDCD2

#define ADC_CHANNEL_VREF			ADC_CHANNEL_IN14

#undef EFI_UART_GPS
#define EFI_UART_GPS				FALSE

#undef EFI_BLUETOOTH_SETUP
#define EFI_BLUETOOTH_SETUP			FALSE

#undef SERIAL_SPEED
#define SERIAL_SPEED				115200

#ifdef TS_PRIMARY_SERIAL
	#undef EFI_CONSOLE_TX_PORT
	#define EFI_CONSOLE_TX_PORT 		GPIOA
	#undef EFI_CONSOLE_TX_PIN
	#define EFI_CONSOLE_TX_PIN			9
	#undef EFI_CONSOLE_RX_PORT
	#define EFI_CONSOLE_RX_PORT			GPIOA
	#undef EFI_CONSOLE_RX_PIN
	#define EFI_CONSOLE_RX_PIN 			10
	#undef EFI_CONSOLE_AF
	#define EFI_CONSOLE_AF 				7
#endif

#undef TS_SERIAL_AF
#define TS_SERIAL_AF				8

/* no console switch */
#undef CONSOLE_MODE_SWITCH_PORT
#undef CONSOLE_MODE_SWITCH_PIN
#define GET_CONSOLE_MODE_VALUE()	(FALSE)

/* no reset switch */
#undef CONFIG_RESET_SWITCH_PORT
#undef CONFIG_RESET_SWITCH_PIN

#undef ADC_VCC
#define ADC_VCC						2.5f

#undef EFI_MAIN_RELAY_CONTROL
#define EFI_MAIN_RELAY_CONTROL		FALSE

#undef EFI_MEMS
#define EFI_MEMS 					FALSE

#undef EFI_IDLE_PID_CIC
#define EFI_IDLE_PID_CIC TRUE

#define RPM_LOW_THRESHOLD 			8				// RPM=8 is an empirical lower sensitivity threshold of MAX9926 for 60-2
#define NO_RPM_EVENTS_TIMEOUT_SECS	5	// (RPM < 12)

#define EFI_NARROW_EGO_AVERAGING		TRUE

/* this board has external QSPI NOR flash */
#undef EFI_STORAGE_EXT_SNOR
#define EFI_STORAGE_EXT_SNOR		TRUE

#undef EFI_STORAGE_INT_FLASH
#define EFI_STORAGE_INT_FLASH   	FALSE

#endif /* EFIFEATURES_SUBARUEG33_H_ */
