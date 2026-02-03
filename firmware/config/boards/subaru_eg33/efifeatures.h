/**
 * @file boards/subaru_eg33/efifeatures.h
 *
 * @brief In this header we can override efifeatures.h.
 *
 * @date Feb 06, 2021
 * @author Andrey Gusakov, 2021
 */

#pragma once

#include "../../stm32f7ems/efifeatures.h"

/* debug console */
#define TS_PRIMARY_UxART_PORT		SD1
#define EFI_TS_PRIMARY_IS_SERIAL TRUE

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

#undef EFI_IDLE_CONTROL
#define EFI_IDLE_CONTROL			TRUE

#undef EFI_USB_SERIAL
#define EFI_USB_SERIAL				TRUE

#define EFI_SDC_DEVICE				SDCD2

#define ADC_CHANNEL_VREF			ADC_CHANNEL_IN14

#undef EFI_UART_GPS
#define EFI_UART_GPS				FALSE

#undef EFI_BLUETOOTH_SETUP
#define EFI_BLUETOOTH_SETUP			FALSE

#undef SERIAL_SPEED
#define SERIAL_SPEED				115200

#ifdef TS_PRIMARY_UxART_PORT
	#undef EFI_CONSOLE_AF
	#define EFI_CONSOLE_AF 				7
#endif

#undef TS_SERIAL_AF
#define TS_SERIAL_AF				8

#undef ADC_VCC
#define ADC_VCC						2.5f

#undef EFI_ONBOARD_MEMS
#define EFI_ONBOARD_MEMS 			FALSE

#undef EFI_IDLE_PID_CIC
#define EFI_IDLE_PID_CIC TRUE

#define RPM_LOW_THRESHOLD 			8				// RPM=8 is an empirical lower sensitivity threshold of MAX9926 for 60-2
#define NO_RPM_EVENTS_TIMEOUT_SECS	5	// (RPM < 12)
