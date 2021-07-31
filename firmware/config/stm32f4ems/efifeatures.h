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

#define EFI_BOOST_CONTROL TRUE

#define EFI_LAUNCH_CONTROL TRUE

#define EFI_DYNO_VIEW TRUE

#define EFI_FSIO TRUE

#ifndef EFI_CDM_INTEGRATION
#define EFI_CDM_INTEGRATION FALSE
#endif

#ifndef EFI_TOOTH_LOGGER
#define EFI_TOOTH_LOGGER TRUE
#endif

#define EFI_TEXT_LOGGING TRUE

#define EFI_PWM_TESTER FALSE

#define EFI_ACTIVE_CONFIGURATION_IN_FLASH FALSE

#define EFI_MC33816 TRUE

#define EFI_HPFP TRUE

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

#if !defined(EFI_ENABLE_MOCK_ADC)
 #define EFI_ENABLE_MOCK_ADC TRUE
#endif /* EFI_ENABLE_MOCK_ADC */


//#define EFI_UART_ECHO_TEST_MODE TRUE

/**
 * Build-in logic analyzer support. Logic analyzer viewer is one of the java console panes.
 */
#ifndef EFI_LOGIC_ANALYZER
#define EFI_LOGIC_ANALYZER TRUE
#endif

#ifndef EFI_ICU_INPUTS
#define EFI_ICU_INPUTS TRUE
#endif

#ifndef HAL_TRIGGER_USE_PAL
#define HAL_TRIGGER_USE_PAL FALSE
#endif /* HAL_TRIGGER_USE_PAL */

#ifndef HAL_TRIGGER_USE_ADC
#define HAL_TRIGGER_USE_ADC FALSE
#endif /* HAL_TRIGGER_USE_ADC */

#ifndef HAL_VSS_USE_PAL
#define HAL_VSS_USE_PAL FALSE
#endif /* HAL_VSS_USE_PAL */

/**
 * TunerStudio support.
 */
#define EFI_TUNER_STUDIO TRUE

/**
 * Bluetooth UART setup support.
 */
#define EFI_BLUETOOTH_SETUP TRUE

/**
 * TunerStudio debug output
 */
#define EFI_TUNER_STUDIO_VERBOSE TRUE

#define EFI_DEFAILED_LOGGING FALSE

/**
 * Dev console support.
 */
#define EFI_CLI_SUPPORT TRUE

#define EFI_RTC TRUE

#define EFI_ALTERNATOR_CONTROL TRUE

#define EFI_AUX_PID TRUE

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

/**
 * MCP42010 digital potentiometer support. This could be useful if you are stimulating some
 * stock ECU
 */
#define EFI_POTENTIOMETER FALSE

#ifndef BOARD_TLE6240_COUNT
#define BOARD_TLE6240_COUNT         1
#endif

#ifndef BOARD_MC33972_COUNT
#define BOARD_MC33972_COUNT			1
#endif

#ifndef BOARD_TLE8888_COUNT
#define BOARD_TLE8888_COUNT 	1
#endif

#ifndef BOARD_DRV8860_COUNT
#define BOARD_DRV8860_COUNT         0
#endif

#ifndef BOARD_MC33810_COUNT
#define BOARD_MC33810_COUNT		0
#endif

#define EFI_ANALOG_SENSORS TRUE

#ifndef EFI_MAX_31855
#define EFI_MAX_31855 TRUE
#endif

#define EFI_MCP_3208 FALSE

#ifndef EFI_HIP_9011
// disabling for now - DMA conflict with SPI1
#define EFI_HIP_9011 FALSE
#endif

#ifndef EFI_CJ125
#define EFI_CJ125 TRUE
#endif

#if !defined(EFI_MEMS)
 #define EFI_MEMS FALSE
#endif

#ifndef EFI_INTERNAL_ADC
#define EFI_INTERNAL_ADC TRUE
#endif

#define EFI_USE_FAST_ADC TRUE

#define EFI_NARROW_EGO_AVERAGING TRUE

#ifndef EFI_CAN_SUPPORT
#define EFI_CAN_SUPPORT TRUE
#endif

#define EFI_WIDEBAND_FIRMWARE_UPDATE TRUE

#ifndef EFI_AUX_SERIAL
#define EFI_AUX_SERIAL TRUE
#endif

#ifndef EFI_HD44780_LCD
#define EFI_HD44780_LCD TRUE
#endif

#ifndef EFI_LCD
#define EFI_LCD TRUE
#endif

#ifndef EFI_IDLE_CONTROL
#define EFI_IDLE_CONTROL TRUE
#endif

#define EFI_IDLE_PID_CIC TRUE

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

#define EFI_FUEL_PUMP TRUE

#ifndef EFI_ENGINE_EMULATOR
#define EFI_ENGINE_EMULATOR TRUE
#endif

#ifndef EFI_EMULATE_POSITION_SENSORS
#define EFI_EMULATE_POSITION_SENSORS TRUE
#endif

/**
 * Do we need file logging (like SD card) logic?
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

// F42x has more memory, so we can:
//  - use compressed USB MSD image (requires 32k of memory)
//  - use perf trace (requires ~16k of memory)
#ifdef EFI_IS_F42x
	#define EFI_USE_COMPRESSED_INI_MSD
	#define ENABLE_PERF_TRACE TRUE
#else
	// small memory F40x can't fit perf trace
	#define ENABLE_PERF_TRACE FALSE
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
#define EFI_PERF_METRICS FALSE
#define DL_OUTPUT_BUFFER 6500
#else
#define EFI_PERF_METRICS FALSE
#define DL_OUTPUT_BUFFER 8000
#endif

/**
 * Do we need GPS logic?
 */
#define EFI_UART_GPS FALSE

#define EFI_SERVO FALSE

#define EFI_ELECTRONIC_THROTTLE_BODY TRUE
//#define EFI_ELECTRONIC_THROTTLE_BODY FALSE

/**
 * Do we need Malfunction Indicator blinking logic?
 */
#define EFI_MALFUNCTION_INDICATOR TRUE
//#define EFI_MALFUNCTION_INDICATOR FALSE

#define CONSOLE_MAX_ACTIONS 180

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

#define EFI_I2C_SCL_BRAIN_PIN GPIOB_6

#define EFI_I2C_SDA_BRAIN_PIN GPIOB_7
#define EFI_I2C_AF 4

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

#ifndef TS_PRIMARY_UART
#define TS_PRIMARY_UART UARTD3
#endif

#undef TS_SECONDARY_UART

#define AUX_SERIAL_DEVICE (&SD6)

// todo: start using consoleSerialTxPin? Not sure
#ifndef EFI_CONSOLE_TX_BRAIN_PIN
#define EFI_CONSOLE_TX_BRAIN_PIN GPIOC_10
#endif
// todo: start using consoleSerialRxPin? Not sure
#ifndef EFI_CONSOLE_RX_BRAIN_PIN
#define EFI_CONSOLE_RX_BRAIN_PIN GPIOC_11
#endif
// todo: this should be detected automatically based on pin selection
#define EFI_CONSOLE_AF 7

// todo: this should be detected automatically based on pin selection
#define TS_SERIAL_AF 7

#ifndef LED_CRITICAL_ERROR_BRAIN_PIN
#define LED_CRITICAL_ERROR_BRAIN_PIN GPIOD_14
#endif
#ifndef LED_ERROR_BRAIN_PIN_MODE
#define LED_ERROR_BRAIN_PIN_MODE DEFAULT_OUTPUT
#endif

// USART1 -> check defined STM32_SERIAL_USE_USART1
// For GPS we have USART1. We can start with PB7 USART1_RX and PB6 USART1_TX
#define GPS_SERIAL_DEVICE &SD1
#define GPS_SERIAL_SPEED 38400

#ifndef CONFIG_RESET_SWITCH_PORT
// looks like this feature is not extremely popular, we can try living without it now :)
//#define CONFIG_RESET_SWITCH_PORT GPIOD
#endif

#ifndef CONFIG_RESET_SWITCH_PIN
#define CONFIG_RESET_SWITCH_PIN 6
#endif

#define EFI_JOYSTICK TRUE
