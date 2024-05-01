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

#ifndef SENT_CHANNELS_NUM
#define SENT_CHANNELS_NUM 1
#endif

#ifndef EFI_BOOST_CONTROL
#define EFI_BOOST_CONTROL TRUE
#endif

#ifndef EFI_DAC
#define EFI_DAC FALSE
#endif

#ifndef EFI_LAUNCH_CONTROL
#define EFI_LAUNCH_CONTROL TRUE
#endif

#ifndef EFI_ANTILAG_SYSTEM
#define EFI_ANTILAG_SYSTEM TRUE
#endif

#ifndef EFI_BACKUP_SRAM
#define EFI_BACKUP_SRAM TRUE
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

#define EFI_PWM_TESTER FALSE

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

#ifndef EFI_VVT_PID
#define EFI_VVT_PID TRUE
#endif

#define EFI_SIGNAL_EXECUTOR_SLEEP FALSE
#define EFI_SIGNAL_EXECUTOR_ONE_TIMER TRUE

#define FUEL_MATH_EXTREME_LOGGING FALSE

#define SPARK_EXTREME_LOGGING FALSE

#define TRIGGER_EXTREME_LOGGING FALSE

#ifndef EFI_STORAGE_INT_FLASH
// historically we've started with low-level flash access with our own redundancy logic
// todo: migrate to EFI_STORAGE_MFS which provides same functionality and more!
#define EFI_STORAGE_INT_FLASH   TRUE
#endif

#ifndef EFI_STORAGE_MFS
// todo: this higher level API should replace EFI_STORAGE_INT_FLASH legacy implementation
#define EFI_STORAGE_MFS         FALSE
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

#define EFI_ANALOG_SENSORS TRUE

#ifndef EFI_MAX_31855
#define EFI_MAX_31855 FALSE
#endif

#define EFI_MCP_3208 FALSE

#ifndef EFI_HIP_9011
// disabling for now - DMA conflict with SPI1
#define EFI_HIP_9011 FALSE
#endif

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

#define EFI_WIDEBAND_FIRMWARE_UPDATE TRUE

#ifndef EFI_AUX_SERIAL
#define EFI_AUX_SERIAL TRUE
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
#define EFI_MAIN_RELAY_CONTROL FALSE
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
#define EFI_EMBED_INI_MSD TRUE
#endif

#ifndef EFI_USB_SERIAL
#define EFI_USB_SERIAL TRUE
#endif

#define EFI_CONSOLE_USB_DEVICE SDU1

#if defined(EFI_HAS_EXT_SDRAM)
    #ifndef ENABLE_PERF_TRACE
    #define ENABLE_PERF_TRACE TRUE
    #endif // ENABLE_PERF_TRACE
    #define LUA_USER_HEAP (1 * 1024 * 1024)
#elif defined(EFI_IS_F42x)
    // F42x has more memory, so we can:
    //  - use compressed USB MSD image (requires 32k of memory)
    //  - use perf trace (requires ~16k of memory)
	#define EFI_USE_COMPRESSED_INI_MSD
	#define ENABLE_PERF_TRACE TRUE

	#define LUA_USER_HEAP 25000
#else
    #ifndef ENABLE_PERF_TRACE
	// small memory F40x can't fit perf trace
	#define ENABLE_PERF_TRACE FALSE
    #endif // ENABLE_PERF_TRACE

	#ifndef LUA_USER_HEAP
	#define LUA_USER_HEAP 25000
	#endif
#endif

#ifndef EFI_LUA
#define EFI_LUA TRUE
#endif

#ifndef EFI_LUA_LOOKUP
#define EFI_LUA_LOOKUP TRUE
#endif

#ifndef EFI_ENGINE_SNIFFER
#define EFI_ENGINE_SNIFFER TRUE
#endif

#define EFI_HISTOGRAMS FALSE

#ifndef EFI_SENSOR_CHART
#define EFI_SENSOR_CHART TRUE
#endif

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
#define EFI_MALFUNCTION_INDICATOR TRUE
#endif

#ifndef CONSOLE_MAX_ACTIONS
#define CONSOLE_MAX_ACTIONS 180
#endif

#ifndef EFI_MAP_AVERAGING
#define EFI_MAP_AVERAGING TRUE
#endif

// todo: most of this should become configurable

// todo: switch to continuous ADC conversion for fast ADC?
#define EFI_INTERNAL_FAST_ADC_GPT	&GPTD6

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

// USART1 -> check defined STM32_SERIAL_USE_USART1
// For GPS we have USART1. We can start with PB7 USART1_RX and PB6 USART1_TX
#define GPS_SERIAL_DEVICE &SD1
#define GPS_SERIAL_SPEED 38400

#ifndef EFI_SENT_SUPPORT
#define EFI_SENT_SUPPORT        FALSE
#endif
