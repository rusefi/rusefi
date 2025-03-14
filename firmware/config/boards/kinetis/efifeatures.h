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

#define EFI_BOOST_CONTROL FALSE

#define EFI_LAUNCH_CONTROL FALSE

#define EFI_MC33816 FALSE

#define EFI_CDM_INTEGRATION FALSE

#define EFI_TOOTH_LOGGER FALSE

#define EFI_PWM_TESTER FALSE

#define EFI_HPFP FALSE

#define ATOFF_BUFFER_SIZE 4

#define ATOFF_BUFFER_SIZE 4

/**
 * if you have a 60-2 trigger, or if you just want better performance, you
 * probably want EFI_ENABLE_ASSERTS to be FALSE. Also you would probably want to FALSE
 * CH_DBG_ENABLE_CHECKS
 * CH_DBG_ENABLE_ASSERTS
 * in chconf.h
 *
 */
#if !defined(EFI_ENABLE_ASSERTS) || defined(__DOXYGEN__)
 #define EFI_ENABLE_ASSERTS FALSE
#endif /* EFI_ENABLE_ASSERTS */

#define EFI_TEXT_LOGGING FALSE

//#define EFI_UART_ECHO_TEST_MODE FALSE

/**
 * Build-in logic analyzer support. Logic analyzer viewer is one of the java console panes.
 */
#ifndef EFI_LOGIC_ANALYZER
#define EFI_LOGIC_ANALYZER FALSE
#endif

/**
 * TunerStudio support.
 */
#define EFI_TUNER_STUDIO FALSE

/**
 * Bluetooth UART setup support.
 */
#define EFI_BLUETOOTH_SETUP FALSE

/**
 * TunerStudio debug output
 */
#define EFI_TUNER_STUDIO_VERBOSE FALSE

#define EFI_DEFAILED_LOGGING FALSE

/**
 * Dev console support.
 */
#define EFI_CLI_SUPPORT FALSE

#define EFI_RTC FALSE

#define EFI_ALTERNATOR_CONTROL FALSE

#define EFI_VVT_PID FALSE

#define EFI_SIGNAL_EXECUTOR_SLEEP FALSE
#define EFI_SIGNAL_EXECUTOR_ONE_TIMER TRUE

#define FUEL_MATH_EXTREME_LOGGING FALSE

#define SPARK_EXTREME_LOGGING FALSE

#define TRIGGER_EXTREME_LOGGING FALSE

#define EFI_STORAGE_INT_FLASH TRUE

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
//#define EFI_POTENTIOMETER FALSE
#define EFI_POTENTIOMETER FALSE

#define EFI_ANALOG_SENSORS TRUE

#ifndef EFI_MAX_31855
#define EFI_MAX_31855 FALSE
#endif

#define EFI_MCP_3208 FALSE

#ifndef EFI_HIP_9011
#define EFI_HIP_9011 FALSE
#define EFI_HIP_9011_LOGIC				FALSE
#endif

#if !defined(EFI_ONBOARD_MEMS) || defined(__DOXYGEN__)
 #define EFI_ONBOARD_MEMS FALSE
#endif

#ifndef EFI_INTERNAL_ADC
#define EFI_INTERNAL_ADC TRUE
#endif

#define EFI_USE_FAST_ADC TRUE

#define EFI_NARROW_EGO_AVERAGING FALSE

#ifndef EFI_CAN_SUPPORT
#define EFI_CAN_SUPPORT FALSE
#endif

#ifndef EFI_IDLE_CONTROL
#define EFI_IDLE_CONTROL FALSE
#endif

#define EFI_IDLE_PID_CIC FALSE

/**
 * Control the main power relay based on measured ignition voltage (Vbatt)
 */
#define EFI_MAIN_RELAY_CONTROL FALSE

#ifndef EFI_VEHICLE_SPEED
#define EFI_VEHICLE_SPEED FALSE
#endif

#ifndef EFI_TCU
#define EFI_TCU FALSE
#endif

#ifndef EFI_ENGINE_EMULATOR
#define EFI_ENGINE_EMULATOR FALSE
#endif

#ifndef EFI_EMULATE_POSITION_SENSORS
#define EFI_EMULATE_POSITION_SENSORS FALSE
#endif

/**
 * Do we need file logging (like SD card) logic?
 */
#ifndef EFI_FILE_LOGGING
#define EFI_FILE_LOGGING FALSE
#endif

#ifndef EFI_USB_SERIAL
#define EFI_USB_SERIAL FALSE
#endif

#ifndef EFI_ENGINE_SNIFFER
#define EFI_ENGINE_SNIFFER FALSE
#endif

#define EFI_HISTOGRAMS FALSE

#define EFI_PERF_METRICS FALSE

/**
 * Do we need GPS logic?
 */
#define EFI_UART_GPS FALSE

#define EFI_ELECTRONIC_THROTTLE_BODY FALSE
//#define EFI_ELECTRONIC_THROTTLE_BODY FALSE

#define EFI_HAS_RESET FALSE

/**
 * Do we need Malfunction Indicator blinking logic?
 */
#define EFI_MALFUNCTION_INDICATOR FALSE
//#define EFI_MALFUNCTION_INDICATOR FALSE

#define CONSOLE_MAX_ACTIONS 1
#define EFI_DISABLE_CONSOLE_ACTIONS FALSE

#define EFI_MAP_AVERAGING FALSE
//#define EFI_MAP_AVERAGING FALSE

// todo: most of this should become configurable

// todo: switch to continues ADC conversion for fast ADC?
#define EFI_INTERNAL_FAST_ADC_GPT	&GPTD2

// todo: why 64 SPLL prescaler doesn't work?
// 168000000/64/128/1025 = ~20Hz
// 168000000/64/16/16 = ~10.25kHz

// todo: warning! these numbers are "tricky"! need to investigate further!
//168000000/128/131 = ~10kHz
#define GPT_FREQ_FAST 20507/*164062*/  /* PWM clock frequency. */
#define GPT_PERIOD_FAST 131   /* PWM period (in PWM ticks).    */

#define EFI_SPI1_AF 3

#define EFI_SPI2_AF 3

/**
 * This section is for right-side center SPI
 */

#define EFI_SPI3_AF 3

/**
 * Patched version of ChibiOS/RT support extra details in the system error messages
 */
#define EFI_CUSTOM_PANIC_METHOD FALSE

#define ADC_CHANNEL_VREF ADC_CHANNEL_IN14

/**
 * Use 'HAL_USE_UART' DMA-mode driver instead of 'HAL_USE_SERIAL'
 *
 * See also
 *  STM32_SERIAL_USE_USARTx
 *  STM32_UART_USE_USARTx
 * in mcuconf.h
 */
#define EFI_USE_UART_DMA FALSE

#define TS_PRIMARY_UxART_PORT UARTD2

#define EFI_CONSOLE_TX_BRAIN_PIN Gpio::A10
#define EFI_CONSOLE_RX_BRAIN_PIN Gpio::A11
#define EFI_CONSOLE_AF 3

#define TS_SERIAL_AF 2

#undef SERIAL_SPEED
#define SERIAL_SPEED 115200

//#define SR5_WRITE_TIMEOUT TIME_MS2I(3000)
//#define SR5_READ_TIMEOUT TIME_MS2I(3000)

#define EFI_COMP_PRIMARY_DEVICE (&COMPD3)
#define EFI_COMP_TRIGGER_CHANNEL 6		// =E7
//#define EFI_TRIGGER_DEBUG_BLINK TRUE
//#define EFI_TRIGGER_COMP_ADAPTIVE_HYSTERESIS TRUE

#define LED_WARNING_BRAIN_PIN Gpio::D13

#define LED_CRITICAL_ERROR_BRAIN_PIN Gpio::D14
#define LED_ERROR_BRAIN_PIN_MODE DEFAULT_OUTPUT

#define EFI_WARNING_LED FALSE

/**
 * This is the size of the MemoryStream used by chvprintf
 */
#define LOGIC_ANALYZER_BUFFER_SIZE 10 /*1800*/
#define DL_OUTPUT_BUFFER 10 /*6500*/

#define UTILITY_THREAD_STACK_SIZE 20 /*400*/

//#define CONSOLE_THREAD_STACK_SIZE UTILITY_THREAD_STACK_SIZE

#define BOARD_TLE6240_COUNT 0
#define BOARD_MC33972_COUNT 0
#define BOARD_TLE8888_COUNT 0
#define BOARD_DRV8860_COUNT 0
#define BOARD_MC33810_COUNT 0
#define BOARD_L9779_COUNT 0

#define TLE6240_SS_PORT GPIOB
#define TLE6240_SS_PAD  0U
#define TLE6240_RESET_PORT NULL
#define TLE6240_RESET_PAD  0
#define TLE6240_DIRECT_IO \
		/* IN1..4 grounded */ \
		[0] = {.port = NULL, .pad = 0}, \
		[1] = {.port = NULL, .pad = 0}, \
		[2] = {.port = NULL, .pad = 0}, \
		[3] = {.port = NULL, .pad = 0}, \
		/* IN9..12 */ \
		[4] = {.port = NULL, .pad = 0}, \
		[5] = {.port = NULL, .pad = 0}, \
		[6] = {.port = NULL, .pad = 0}, \
		[7] = {.port = NULL, .pad = 0},

#define EFI_BOSCH_YAW FALSE

#define EFI_BOARD_TEST FALSE
#define EFI_JOYSTICK FALSE
#define EFI_ENGINE_AUDI_AAN FALSE
#define EFI_ENGINE_SNOW_BLOWER FALSE
#define DEBUG_FUEL FALSE
#define EFI_UART_ECHO_TEST_MODE FALSE
#define EXTREME_TERM_LOGGING FALSE
#define EFI_PRINTF_FUEL_DETAILS FALSE
#define ENABLE_PERF_TRACE FALSE

#define RAM_UNUSED_SIZE 1
#define CCM_UNUSED_SIZE 1

#define EFI_BACKUP_SRAM FALSE

#undef EFI_ACTIVE_CONFIGURATION_IN_FLASH
#define EFI_ACTIVE_CONFIGURATION_IN_FLASH TRUE

//!!!!!!!!!!!!!!!!!!!!!!
#define debugLog(fmt,...) { \
	extern int __debugEnabled; \
	if (__debugEnabled) { \
		extern char __debugBuffer[80]; \
		chsnprintf(__debugBuffer, sizeof(__debugBuffer), fmt, ##__VA_ARGS__); \
		uart_lld_blocking_send(TS_PRIMARY_UxART_PORT, strlen(__debugBuffer), (void *)__debugBuffer); \
	} \
}
