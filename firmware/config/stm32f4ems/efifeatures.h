/**
 * @file efifeatures.h
 *
 * @brief In this header we can configure which firmware modules are used.
 *
 * @date Aug 29, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#define EFI_GPIO_HARDWARE TRUE

#define SENT_CHANNELS_NUM 1

#ifndef EFI_BOOST_CONTROL
#define EFI_BOOST_CONTROL TRUE
#endif

#ifndef EFI_LAUNCH_CONTROL
#define EFI_LAUNCH_CONTROL TRUE
#endif

#ifndef EFI_ANTILAG_SYSTEM
#define EFI_ANTILAG_SYSTEM TRUE
#endif

#define EFI_DYNO_VIEW TRUE

#ifndef EFI_TOOTH_LOGGER
#define EFI_TOOTH_LOGGER TRUE
#endif

#define EFI_TEXT_LOGGING TRUE

#define EFI_PWM_TESTER FALSE

#define EFI_ACTIVE_CONFIGURATION_IN_FLASH FALSE

#ifndef EFI_MC33816
#define EFI_MC33816 TRUE
#endif

#ifndef EFI_HPFP
#define EFI_HPFP TRUE
#endif

#define EFI_ENABLE_CRITICAL_ENGINE_STOP TRUE
#define EFI_ENABLE_ENGINE_WARNING TRUE

#ifndef SC_BUFFER_SIZE
#define SC_BUFFER_SIZE 4000
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

#define EFI_DEFAILED_LOGGING FALSE

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

#ifndef EFI_AUX_PID
#define EFI_AUX_PID TRUE
#endif

#define EFI_SIGNAL_EXECUTOR_SLEEP FALSE
#define EFI_SIGNAL_EXECUTOR_ONE_TIMER TRUE
#define EFI_SIGNAL_EXECUTOR_HW_TIMER FALSE

#define FUEL_MATH_EXTREME_LOGGING FALSE

#define SPARK_EXTREME_LOGGING FALSE

#define TRIGGER_EXTREME_LOGGING FALSE

#ifndef EFI_INTERNAL_FLASH
#define EFI_INTERNAL_FLASH TRUE
#endif

/**
 * Usually you need shaft position input, but maybe you do not need it?
 */
#ifndef EFI_SHAFT_POSITION_INPUT
#define EFI_SHAFT_POSITION_INPUT TRUE
#endif

/**
 * Maybe we are just sniffing what's going on?
 */
#define EFI_ENGINE_CONTROL TRUE

#ifndef BOARD_TLE6240_COUNT
#define BOARD_TLE6240_COUNT         0
#endif

#ifndef BOARD_MC33972_COUNT
#define BOARD_MC33972_COUNT			0
#endif

#ifndef BOARD_TLE8888_COUNT
#define BOARD_TLE8888_COUNT 	1
#endif

#ifndef BOARD_L9779_COUNT
#define BOARD_L9779_COUNT 	1
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

#define EFI_ANALOG_SENSORS TRUE

#ifndef EFI_MAX_31855
#define EFI_MAX_31855 TRUE
#endif

#if !defined(EFI_MEMS)
 #define EFI_MEMS FALSE
#endif

#ifndef EFI_INTERNAL_ADC
#define EFI_INTERNAL_ADC TRUE
#endif

#define EFI_USE_FAST_ADC TRUE

#ifndef EFI_CAN_SUPPORT
#define EFI_CAN_SUPPORT TRUE
#endif

#ifndef EFI_CAN_SERIAL
#define EFI_CAN_SERIAL TRUE
#endif

#define EFI_WIDEBAND_FIRMWARE_UPDATE TRUE

#ifndef EFI_AUX_SERIAL
#define EFI_AUX_SERIAL TRUE
#endif

#ifndef EFI_IDLE_CONTROL
#define EFI_IDLE_CONTROL TRUE
#endif

/**
 * Control the main power relay based on measured ignition voltage (Vbatt)
 */
#ifndef EFI_MAIN_RELAY_CONTROL
#define EFI_MAIN_RELAY_CONTROL FALSE
#endif

#ifndef EFI_PWM
#define EFI_PWM TRUE
#endif

#ifndef EFI_VEHICLE_SPEED
#define EFI_VEHICLE_SPEED TRUE
#endif

#ifndef EFI_TCU
#define EFI_TCU TRUE
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
#define EFI_EMBED_INI_MSD TRUE
#endif

#ifndef EFI_USB_SERIAL
#define EFI_USB_SERIAL TRUE
#endif

#define EFI_CONSOLE_USB_DEVICE SDU1

#if defined(EFI_HAS_EXT_SDRAM)
    #define ENABLE_PERF_TRACE TRUE
    #define LUA_USER_HEAP (1 * 1024 * 1024)
#elif defined(EFI_IS_F42x)
    // F42x has more memory, so we can:
    //  - use compressed USB MSD image (requires 32k of memory)
    //  - use perf trace (requires ~16k of memory)
	#define EFI_USE_COMPRESSED_INI_MSD
	#define ENABLE_PERF_TRACE TRUE

	#if EFI_ETHERNET
		// F4 ethernet needs some extra space
		#define LUA_USER_HEAP 40000
	#else // EFI_ETHERNET
		#define LUA_USER_HEAP 50000
	#endif
#else
	// small memory F40x can't fit perf trace
	#define ENABLE_PERF_TRACE FALSE

	#define LUA_USER_HEAP 25000
#endif

#ifndef EFI_LUA
#define EFI_LUA TRUE
#endif

#ifndef EFI_ENGINE_SNIFFER
#define EFI_ENGINE_SNIFFER TRUE
#endif

#define EFI_HISTOGRAMS FALSE
#define EFI_SENSOR_CHART TRUE

#if defined __GNUC__
#define DL_OUTPUT_BUFFER 6500
#else
#define DL_OUTPUT_BUFFER 8000
#endif

#define EFI_ELECTRONIC_THROTTLE_BODY TRUE
//#define EFI_ELECTRONIC_THROTTLE_BODY FALSE

/**
 * Do we need Malfunction Indicator blinking logic?
 */
#ifndef EFI_MALFUNCTION_INDICATOR
#define EFI_MALFUNCTION_INDICATOR TRUE
//#define EFI_MALFUNCTION_INDICATOR FALSE
#endif

#ifndef CONSOLE_MAX_ACTIONS
#define CONSOLE_MAX_ACTIONS 180
#endif

#define EFI_MAP_AVERAGING TRUE
//#define EFI_MAP_AVERAGING FALSE

// todo: most of this should become configurable

// todo: switch to continuous ADC conversion for fast ADC?
#define EFI_INTERNAL_FAST_ADC_GPT	&GPTD6

#define EFI_SPI1_AF 5

#define EFI_SPI2_AF 5

/**
 * This section is for right-side center SPI
 */

#define EFI_SPI3_AF 6

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

// allow override of EFI_USE_UART_DMA from cmdline passed defs
#ifndef EFI_USE_UART_DMA
#define EFI_USE_UART_DMA TRUE
#endif

#define AUX_SERIAL_DEVICE (&SD6)

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

// todo: this should be detected automatically based on pin selection
// https://github.com/rusefi/rusefi/issues/3536
#ifndef TS_SERIAL_AF
#define TS_SERIAL_AF 7
#endif

#ifndef LED_CRITICAL_ERROR_BRAIN_PIN
#define LED_CRITICAL_ERROR_BRAIN_PIN Gpio::D14
#endif
#ifndef LED_ERROR_BRAIN_PIN_MODE
#define LED_ERROR_BRAIN_PIN_MODE OM_DEFAULT
#endif

#ifndef CONFIG_RESET_SWITCH_PORT
// looks like this feature is not extremely popular, we can try living without it now :)
//#define CONFIG_RESET_SWITCH_PORT GPIOD
#endif

#ifndef CONFIG_RESET_SWITCH_PIN
#define CONFIG_RESET_SWITCH_PIN 6
#endif

#ifndef EFI_STORAGE_INT_FLASH
#define EFI_STORAGE_INT_FLASH   TRUE
#endif

#ifndef EFI_STORAGE_EXT_SNOR
#define EFI_STORAGE_EXT_SNOR    FALSE
#endif

#ifndef EFI_SENT_SUPPORT
#define EFI_SENT_SUPPORT        FALSE
#endif
