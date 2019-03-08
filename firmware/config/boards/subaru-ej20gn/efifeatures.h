/**
 * @file boards/subaruej20g/efifeatures.h
 *
 * @brief In this header we can override efifeatures.h.
 *
 * @date Feb 19, 2019
 * @author Andrey Gusakov, 2019
 */

#include "../../stm32f7ems/efifeatures.h"

#ifndef EFIFEATURES_SUBARUEJ20G_H_
#define EFIFEATURES_SUBARUEJ20G_H_

/* debug console */
#undef EFI_USE_UART_FOR_CONSOLE
#define EFI_USE_UART_FOR_CONSOLE	TRUE
#undef EFI_CONSOLE_UART_DEVICE
#define EFI_CONSOLE_UART_DEVICE		(&SD1)

/* TunerStudio binary protocol */
/* do not use UART  device for console */
#undef TS_UART_DEVICE

/* do not use serial device for console */
#undef TS_SERIAL_DEVICE

#undef EFI_RTC
#define EFI_RTC						FALSE

#undef EFI_POTENTIOMETER
#define EFI_POTENTIOMETER			FALSE

#undef EFI_MAX_31855
#define EFI_MAX_31855				FALSE

#undef EFI_HD44780_LCD
#define EFI_HD44780_LCD				FALSE

#undef EFI_USB_SERIAL
#define EFI_USB_SERIAL				TRUE

#undef EFI_FUEL_PUMP
#define EFI_FUEL_PUMP				TRUE

#undef MMC_CARD_SPI
#define MMC_CARD_SPI				SPID1

#define ADC_CHANNEL_VREF			ADC_CHANNEL_IN14

#undef EFI_UART_GPS
#define EFI_UART_GPS				FALSE

#undef EFI_BLUETOOTH_SETUP
#define EFI_BLUETOOTH_SETUP			FALSE

#undef SERIAL_SPEED
#define SERIAL_SPEED				115200

#ifdef EFI_CONSOLE_UART_DEVICE
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

/* Red */
#undef LED_ERROR_BRAIN_PIN
#define LED_ERROR_BRAIN_PIN			GPIOG_7
#undef LED_ERROR_PORT
#define LED_ERROR_PORT				GPIOG
#undef LED_ERROR_PIN
#define LED_ERROR_PIN				7

/* no console switch */
#undef CONSOLE_MODE_SWITCH_PORT
#undef CONSOLE_MODE_SWITCH_PIN
#define GET_CONSOLE_MODE_VALUE()	(FALSE)

/* no reset switch */
#undef CONFIG_RESET_SWITCH_PORT
#undef CONFIG_RESET_SWITCH_PIN

//!!!!!!!!!!!!!!!
//#undef EFI_INTERNAL_ADC
//#define EFI_INTERNAL_ADC			FALSE

#undef ADC_VCC
#define ADC_VCC						2.5f

#undef EFI_MAIN_RELAY_CONTROL
#define EFI_MAIN_RELAY_CONTROL		TRUE

#undef EFI_MEMS
#define EFI_MEMS 					FALSE

#undef EFI_IDLE_INCREMENTAL_PID_CIC
#define EFI_IDLE_INCREMENTAL_PID_CIC TRUE

#undef EFI_CJ125_DIRECTLY_CONNECTED_UR
#define EFI_CJ125_DIRECTLY_CONNECTED_UR TRUE

#define RPM_LOW_THRESHOLD 			8				// RPM=8 is an empirical lower sensitivity threshold of MAX9926 for 60-2
#define NO_RPM_EVENTS_TIMEOUT_SECS	5	// (RPM < 12)

#define EFI_PRINT_ERRORS_AS_WARNINGS	TRUE

#define EFI_NARROW_EGO_AVERAGING		TRUE

#endif /* EFIFEATURES_SUBARUEJ20G_H_ */
