/**
 * @file efifeatures.h
 *
 * @brief In this header we can configure which firmware modules are used.
 * See config/boards which are often overriding some of the defaults via .mk and/or .env files
 *
 * @date Aug 29, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include <rusefi/true_false.h>

#define EFI_GPIO_HARDWARE TRUE

#ifndef SENT_CHANNELS_NUM
#define SENT_CHANNELS_NUM 1
#endif

#ifndef EFI_BOOST_CONTROL
#define EFI_BOOST_CONTROL TRUE
#endif

#ifndef EFI_BOSCH_YAW
#define EFI_BOSCH_YAW FALSE
#endif

#ifndef EFI_WIFI
#define EFI_WIFI FALSE
#endif

#ifndef EFI_WS2812
#define EFI_WS2812 FALSE
#endif

#ifndef EFI_DAC
#define EFI_DAC FALSE
#endif

#ifndef EFI_LAUNCH_CONTROL
#define EFI_LAUNCH_CONTROL TRUE
#endif

/* Long Term Fuel Trims */
#ifndef EFI_LTFT_CONTROL
#define EFI_LTFT_CONTROL TRUE
#endif

#ifndef EFI_STORAGE_SD
// Lets try saving LTFT trims on SD card by default
#define EFI_STORAGE_SD TRUE
#endif

#ifndef EFI_ANTILAG_SYSTEM
#define EFI_ANTILAG_SYSTEM TRUE
#endif

#ifndef EFI_BACKUP_SRAM
#define EFI_BACKUP_SRAM TRUE
#endif

#ifndef EFI_HELLA_OIL
#define EFI_HELLA_OIL FALSE
#endif

#ifndef EFI_USE_OPENBLT
#define EFI_USE_OPENBLT FALSE
#endif

#ifndef EFI_ETHERNET
#define EFI_ETHERNET FALSE
#endif

#ifndef EFI_DFU_JUMP
#define EFI_DFU_JUMP TRUE
#endif

#ifndef EFI_BOR_LEVEL
#define EFI_BOR_LEVEL TRUE
#endif

#ifndef EFI_DYNO_VIEW
#define EFI_DYNO_VIEW TRUE
#endif

#ifndef EFI_CDM_INTEGRATION
#define EFI_CDM_INTEGRATION FALSE
#endif

#ifndef EFI_TOOTH_LOGGER
#define EFI_TOOTH_LOGGER TRUE
#endif

#ifndef EFI_TEXT_LOGGING
#define EFI_TEXT_LOGGING TRUE
#endif

#define EFI_ACTIVE_CONFIGURATION_IN_FLASH FALSE

#ifndef EFI_MC33816
#define EFI_MC33816 FALSE
#endif

#ifndef EFI_HPFP
#define EFI_HPFP TRUE
#endif

#ifndef EFI_HD_ACR
#define EFI_HD_ACR FALSE
#endif


/**
 * if you have a 60-2 trigger, or if you just want better performance, you
 * probably want EFI_ENABLE_ASSERTS to be FALSE. Also you would probably want to FALSE
 * CH_DBG_ENABLE_CHECKS
 * CH_DBG_ENABLE_ASSERTS
 * in chconf.h
 *
 */
#if !defined(EFI_ENABLE_ASSERTS)
 #define EFI_ENABLE_ASSERTS TRUE
#endif /* EFI_ENABLE_ASSERTS */

#ifndef EFI_CLOCK_LOCKS
#define EFI_CLOCK_LOCKS TRUE
#endif

//#define EFI_UART_ECHO_TEST_MODE TRUE

/**
 * Build-in logic analyzer support. Logic analyzer viewer is one of the java console panes.
 */
#ifndef EFI_LOGIC_ANALYZER
#define EFI_LOGIC_ANALYZER TRUE
#endif

#ifndef HAL_TRIGGER_USE_PAL
#define HAL_TRIGGER_USE_PAL TRUE
#endif /* HAL_TRIGGER_USE_PAL */

#ifndef HAL_TRIGGER_USE_ADC
#define HAL_TRIGGER_USE_ADC FALSE
#endif /* HAL_TRIGGER_USE_ADC */

/**
 * TunerStudio support.
 */
#ifndef EFI_TUNER_STUDIO
#define EFI_TUNER_STUDIO TRUE
#endif

#ifndef EFI_TS_SCATTER
#define EFI_TS_SCATTER TRUE
#endif

/**
 * Bluetooth UART setup support.
 */
#ifndef EFI_BLUETOOTH_SETUP
#define EFI_BLUETOOTH_SETUP TRUE
#endif /* EFI_BLUETOOTH_SETUP */

/**
 * TunerStudio debug output
 */
#define EFI_TUNER_STUDIO_VERBOSE TRUE

#define EFI_DETAILED_LOGGING FALSE

/**
 * Dev console support.
 */
#ifndef EFI_CLI_SUPPORT
#define EFI_CLI_SUPPORT TRUE
#endif

#ifndef EFI_RTC
#define EFI_RTC TRUE
#endif

#ifndef EFI_ALTERNATOR_CONTROL
#define EFI_ALTERNATOR_CONTROL TRUE
#endif

#ifndef EFI_VVT_PID
#define EFI_VVT_PID TRUE
#endif

#define EFI_SIGNAL_EXECUTOR_SLEEP FALSE
#define EFI_SIGNAL_EXECUTOR_ONE_TIMER TRUE

#define FUEL_MATH_EXTREME_LOGGING FALSE

#define SPARK_EXTREME_LOGGING FALSE

#define TRIGGER_EXTREME_LOGGING FALSE

/**
 * Store configuration as raw binary to internal flash (two copies if there is enough flash)
 * No wear leveling, one copy occupie whole sector erased on each update
 * Legacy, should be replaced with EFI_STORAGE_MFS
 */
#ifndef EFI_STORAGE_INT_FLASH
// historically we've started with low-level flash access with our own redundancy logic
// todo: migrate to EFI_STORAGE_MFS which provides same functionality and more!
#define EFI_STORAGE_INT_FLASH   TRUE
#endif

/**
 * ChibiOS Managed Flash Storage
 * Can store finite number of records, can update/add while there is free space in bank.
 * Recover from power loss
 */
#ifndef EFI_STORAGE_MFS
// todo: this higher level API should replace EFI_STORAGE_INT_FLASH legacy implementation
#define EFI_STORAGE_MFS         FALSE
#endif

/**
 * Store settings/calibrations/learning to SD card as a files
 */
#ifndef EFI_STORAGE_SD
#define EFI_STORAGE_SD	        FALSE
#endif

/**
 * Controlled defined to TRUE by USE_FATFS=yes in board mk file
 */
#ifndef EFI_SUPPORT_FATFS
#define EFI_SUPPORT_FATFS		FALSE
#endif

/**
 * Usually you need shaft position input, but maybe you do not need it?
 */
#ifndef EFI_SHAFT_POSITION_INPUT
#define EFI_SHAFT_POSITION_INPUT TRUE
#endif

/**
 * Maybe we are just sniffing what's going on?
 * EFI_ENGINE_CONTROL is covering injectors and spark control
 */
#ifndef EFI_ENGINE_CONTROL
#define EFI_ENGINE_CONTROL TRUE
#endif

/**
 * MCP42010 digital potentiometer support. This could be useful if you are stimulating some
 * stock ECU
 */
#ifndef EFI_POTENTIOMETER
#define EFI_POTENTIOMETER FALSE
#endif

#ifndef BOARD_TLE6240_COUNT
#define BOARD_TLE6240_COUNT         0
#endif

#ifndef BOARD_MC33972_COUNT
#define BOARD_MC33972_COUNT			0
#endif

#ifndef BOARD_TLE8888_COUNT
#define BOARD_TLE8888_COUNT 	0
#endif

#ifndef BOARD_L9779_COUNT
#define BOARD_L9779_COUNT 	0
#endif

#ifndef BOARD_DRV8860_COUNT
#define BOARD_DRV8860_COUNT         0
#endif

#ifndef BOARD_MC33810_COUNT
#define BOARD_MC33810_COUNT		0
#endif

#ifndef BOARD_TLE9104_COUNT
#define BOARD_TLE9104_COUNT 0
#endif

#ifndef BOARD_TLE9201_COUNT
#define BOARD_TLE9201_COUNT 0
#endif

#define EFI_ANALOG_SENSORS TRUE

#ifndef EFI_MAX_31855
#define EFI_MAX_31855 FALSE
#endif

#define EFI_MCP_3208 FALSE

#if !defined(EFI_ONBOARD_MEMS)
 #define EFI_ONBOARD_MEMS FALSE
#endif

#ifndef EFI_INTERNAL_ADC
#define EFI_INTERNAL_ADC TRUE
#endif

#define EFI_USE_FAST_ADC TRUE

#ifndef EFI_CAN_SUPPORT
#define EFI_CAN_SUPPORT TRUE
#endif

#if !defined(EFI_CAN_SERIAL) && EFI_CAN_SUPPORT
#define EFI_CAN_SERIAL TRUE
#endif

#if !defined(EFI_CAN_GPIO) && EFI_CAN_SUPPORT
// see CAN_PIN_0
#define EFI_CAN_GPIO TRUE
#endif

#ifndef EFI_WIDEBAND_FIRMWARE_UPDATE
#define EFI_WIDEBAND_FIRMWARE_UPDATE TRUE
#endif

#ifndef EFI_IDLE_CONTROL
#define EFI_IDLE_CONTROL TRUE
#endif

#ifndef EFI_IDLE_PID_CIC
#define EFI_IDLE_PID_CIC TRUE
#endif

/**
 * Control the main power relay based on measured ignition voltage (Vbatt)
 */
#ifndef EFI_MAIN_RELAY_CONTROL
#define EFI_MAIN_RELAY_CONTROL TRUE
#endif

#ifndef EFI_VEHICLE_SPEED
#define EFI_VEHICLE_SPEED TRUE
#endif

#ifndef EFI_TCU
#define EFI_TCU FALSE
#endif

#ifndef EFI_ENGINE_EMULATOR
#define EFI_ENGINE_EMULATOR TRUE
#endif

#ifndef EFI_EMULATE_POSITION_SENSORS
#define EFI_EMULATE_POSITION_SENSORS TRUE
#endif

/**
 * Do we need file logging (like SD card) logic?
 * See also USE_FATFS
 */
#ifndef EFI_FILE_LOGGING
#define EFI_FILE_LOGGING TRUE
#endif

#ifndef EFI_EMBED_INI_MSD
#define EFI_EMBED_INI_MSD FALSE
#endif

#ifndef EFI_USB_SERIAL
#define EFI_USB_SERIAL TRUE
#endif

#define EFI_CONSOLE_USB_DEVICE SDU1

#if defined(EFI_HAS_EXT_SDRAM)
	#ifndef ENABLE_PERF_TRACE
	  #define ENABLE_PERF_TRACE TRUE
	#endif // ENABLE_PERF_TRACE
	#define LUA_EXTRA_HEAP (1 * 1024 * 1024)
#elif defined(EFI_IS_F42x)
	// F42x has more memory, so we can:
	//  - use compressed USB MSD image (requires 32k of memory)
	//  - use perf trace (requires ~16k of memory)
	//  - use spectorgram (requires ~12k of memory), need disable perf trace or compressed USB MSD image
	#ifndef KNOCK_SPECTROGRAM
	  #ifndef EFI_USE_COMPRESSED_INI_MSD
		#define EFI_USE_COMPRESSED_INI_MSD TRUE
		#endif
	#endif
	#define ENABLE_PERF_TRACE TRUE
#else
	#ifndef ENABLE_PERF_TRACE
	  // small memory F40x can't fit perf trace
	  #define ENABLE_PERF_TRACE FALSE
	#endif // ENABLE_PERF_TRACE
#endif

#ifndef EFI_USE_COMPRESSED_INI_MSD
#define EFI_USE_COMPRESSED_INI_MSD FALSE
#endif

#ifndef EFI_LUA
#define EFI_LUA TRUE
#endif

#ifndef FULL_SD_LOGS
// reduce RAM usage? todo: optimize RAM consumption so that all builds have full logs?
#define FULL_SD_LOGS FALSE
#endif

#ifndef EFI_LUA_LOOKUP
#define EFI_LUA_LOOKUP TRUE
#endif

#ifndef EFI_ENGINE_SNIFFER
#define EFI_ENGINE_SNIFFER TRUE
#endif

#define EFI_HISTOGRAMS FALSE


#ifndef EFI_PERF_METRICS
#define EFI_PERF_METRICS FALSE
#endif

#ifndef DL_OUTPUT_BUFFER
#define DL_OUTPUT_BUFFER 6500
#endif

/**
 * Do we need GPS logic?
 */
#ifndef EFI_UART_GPS
#define EFI_UART_GPS FALSE
#endif

#ifndef EFI_ELECTRONIC_THROTTLE_BODY
#define EFI_ELECTRONIC_THROTTLE_BODY TRUE
#endif

/**
 * Do we need Malfunction Indicator blinking logic?
 */
#ifndef EFI_MALFUNCTION_INDICATOR
#define EFI_MALFUNCTION_INDICATOR FALSE
#endif

#ifndef CONSOLE_MAX_ACTIONS
#define CONSOLE_MAX_ACTIONS 180
#endif

#ifndef EFI_MAP_AVERAGING
#define EFI_MAP_AVERAGING TRUE
#endif

// todo: most of this should become configurable

// TODO: switch to continuous ADC conversion for fast ADC?
// NOTE: GPT mode triggers ADC convertion through IRQ
#define EFI_INTERNAL_FAST_ADC_GPT	&GPTD6
// NOTE: PWM mode triggers ADC convertion through hardware ADC trigger
//#define EFI_INTERNAL_FAST_ADC_PWM	&PWMD8

// Continuously run internal ADC in background for all channels
// Do averaging in thread with no sync with convertion end
#define EFI_INTERNAL_SLOW_ADC_BACKGROUND	TRUE

#define EFI_SPI1_AF 5
#define EFI_SPI2_AF 5
#define EFI_SPI3_AF 6
#define EFI_SPI4_AF 5
#define EFI_SPI5_AF 5
#define EFI_SPI6_AF 6

/**
 * Patched version of ChibiOS/RT support extra details in the system error messages
 */
#define EFI_CUSTOM_PANIC_METHOD TRUE

#define ADC_CHANNEL_VREF ADC_CHANNEL_IN14

/**
 * currently ChibiOS uses only first and second channels of each timer for input capture
 *
 * So, our options are:
 *
 * TIM2_CH1
 *  PA5
 *
 * TIM4_CH1
 *  PB6
 * 	PD12
 *
 * TIM9_CH1
 *  PE5
 */

#ifndef EFI_USE_UART_DMA
#define EFI_USE_UART_DMA FALSE
#endif

#ifndef AUX_SERIAL_DEVICE
#define AUX_SERIAL_DEVICE (&SD6)
#endif

#ifndef EFI_CONSOLE_TX_BRAIN_PIN
#define EFI_CONSOLE_TX_BRAIN_PIN Gpio::C10
#endif

#ifndef EFI_CONSOLE_RX_BRAIN_PIN
#define EFI_CONSOLE_RX_BRAIN_PIN Gpio::C11
#endif
// todo: this should be detected automatically based on pin selection
// https://github.com/rusefi/rusefi/issues/3536
#ifndef EFI_CONSOLE_AF
#define EFI_CONSOLE_AF 7
#endif

// Rx pin should have either internal either external pull up to avoid floating and receiving random garbage
#ifndef EFI_CONSOLE_RX_BRAIN_PIN_MODE
#define EFI_CONSOLE_RX_BRAIN_PIN_MODE (PAL_MODE_ALTERNATE(EFI_CONSOLE_AF) | PAL_STM32_PUPDR_PULLUP)
#endif

#ifndef EFI_CONSOLE_TX_BRAIN_PIN_MODE
#define EFI_CONSOLE_TX_BRAIN_PIN_MODE (PAL_MODE_ALTERNATE(EFI_CONSOLE_AF))
#endif

// todo: this should be detected automatically based on pin selection
// https://github.com/rusefi/rusefi/issues/3536
#ifndef TS_SERIAL_AF
#define TS_SERIAL_AF 7
#endif

#ifndef TS_SERIAL_RX_BRAIN_PIN_MODE
#define TS_SERIAL_RX_BRAIN_PIN_MODE (PAL_MODE_ALTERNATE(TS_SERIAL_AF) | PAL_STM32_PUPDR_PULLUP)
#endif

#ifndef TS_SERIAL_TX_BRAIN_PIN_MODE
#define TS_SERIAL_TX_BRAIN_PIN_MODE (PAL_MODE_ALTERNATE(TS_SERIAL_AF))
#endif

#ifndef LED_CRITICAL_ERROR_BRAIN_PIN
#define LED_CRITICAL_ERROR_BRAIN_PIN Gpio::D14
#endif

// USART1 -> check defined STM32_SERIAL_USE_USART1
// For GPS we have USART1. We can start with PB7 USART1_RX and PB6 USART1_TX
#define GPS_SERIAL_DEVICE &SD1
#define GPS_SERIAL_SPEED 38400

#ifndef EFI_SENT_SUPPORT
#define EFI_SENT_SUPPORT        FALSE
#endif

#ifndef EFI_PULSE_INPUT
#define EFI_PULSE_INPUT    TRUE
#endif
