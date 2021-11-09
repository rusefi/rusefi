/**
 * @file efifeatures.h
 *
 * @brief In this header we can configure which firmware modules are used.
 *
 * @date Jan 27, 2020
 * @author Andrey Belomutskiy, (c) 2012-2020
 * @author andreika <prometheus.pcb@gmail.com>
 */

#ifndef EFIFEATURES_H_
#define EFIFEATURES_H_

#define EFI_GPIO_HARDWARE TRUE

#define EFI_FSIO TRUE

#define EFI_CDM_INTEGRATION FALSE

#define EFI_TOOTH_LOGGER FALSE

#define EFI_PWM_TESTER FALSE

#define EFI_ENABLE_CRITICAL_ENGINE_STOP FALSE
#define EFI_ENABLE_ENGINE_WARNING TRUE

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

#if !defined(EFI_ENABLE_MOCK_ADC) || defined(__DOXYGEN__)
 #define EFI_ENABLE_MOCK_ADC FALSE
#endif /* EFI_ENABLE_MOCK_ADC */


#define EFI_TEXT_LOGGING TRUE

//#define EFI_UART_ECHO_TEST_MODE FALSE

/**
 * Build-in logic analyzer support. Logic analyzer viewer is one of the java console panes.
 */
#ifndef EFI_LOGIC_ANALYZER
#define EFI_LOGIC_ANALYZER FALSE
#endif

#ifndef EFI_ICU_INPUTS
#define EFI_ICU_INPUTS FALSE
#endif

/**
 * TunerStudio support.
 */
#define EFI_TUNER_STUDIO TRUE

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

#define EFI_AUX_PID FALSE

#define EFI_SIGNAL_EXECUTOR_SLEEP FALSE
#define EFI_SIGNAL_EXECUTOR_ONE_TIMER TRUE
#define EFI_SIGNAL_EXECUTOR_HW_TIMER FALSE

#define FUEL_MATH_EXTREME_LOGGING FALSE

#define SPARK_EXTREME_LOGGING FALSE

#define TRIGGER_EXTREME_LOGGING FALSE

#define EFI_INTERNAL_FLASH TRUE

/**
 * Flex Non Volatile Memory is faster than flash
 * It also has smaller pages so it takes less time to erase
 *
 * There is no remote access to FlexNVM meaning that we cannot erase settings externally
 */

// Dual-Flash version:
//#define FLASH_ADDR 0x200F8000
// todo: find other location for 1Mb Flash chip versions?
//#define FLASH_ADDR_SECOND_COPY 0x20100000

// Main-Flash version:
#define FLASH_ADDR 0x000E0000
#define FLASH_ADDR_SECOND_COPY 0x000C0000

#define BACKUP_FLASH_ADDR      0x00406000
#define BACKUP_FLASH_SIZE      0x2000	// 8k

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

#define EFI_SPEED_DENSITY TRUE

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
#endif

#ifndef EFI_CJ125
#define EFI_CJ125 FALSE
#endif

#if !defined(EFI_MEMS) || defined(__DOXYGEN__)
 #define EFI_MEMS FALSE
#endif

#define EFI_INTERNAL_ADC TRUE
#define EFI_USE_FAST_ADC TRUE

#define EFI_NARROW_EGO_AVERAGING FALSE

#ifndef EFI_CAN_SUPPORT
#define EFI_CAN_SUPPORT FALSE
#endif

#define EFI_HD44780_LCD FALSE
#define EFI_LCD FALSE

#define EFI_IDLE_CONTROL TRUE

#define EFI_IDLE_PID_CIC FALSE

/**
 * Control the main power relay based on measured ignition voltage (Vbatt)
 */
#define EFI_MAIN_RELAY_CONTROL TRUE

#ifndef EFI_PWM
#define EFI_PWM FALSE
#endif

#define EFI_VEHICLE_SPEED FALSE

#define EFI_FUEL_PUMP TRUE

#define EFI_ENGINE_EMULATOR TRUE

#define EFI_EMULATE_POSITION_SENSORS TRUE

/**
 * Do we need file logging (like SD card) logic?
 */
#ifndef EFI_FILE_LOGGING
#define EFI_FILE_LOGGING FALSE
#endif


/**
 * While we embed multiple PnP configurations into the same firmware binary, these marcoses give us control
 * over which configurations go into the binary
 */
#define EFI_SUPPORT_DODGE_NEON FALSE
#define EFI_SUPPORT_FORD_ASPIRE FALSE
#define EFI_SUPPORT_NISSAN_PRIMERA FALSE
#define EFI_SUPPORT_1995_FORD_INLINE_6 FALSE

#define EFI_ENGINE_SNIFFER TRUE

#define EFI_HISTOGRAMS FALSE
#define EFI_SENSOR_CHART FALSE

#define EFI_PERF_METRICS FALSE

/**
 * Do we need GPS logic?
 */
#define EFI_UART_GPS FALSE

#define EFI_SERVO FALSE

#define EFI_ELECTRONIC_THROTTLE_BODY FALSE
//#define EFI_ELECTRONIC_THROTTLE_BODY FALSE

#define EFI_HAS_RESET FALSE

/**
 * Do we need Malfunction Indicator blinking logic?
 */
#define EFI_MALFUNCTION_INDICATOR FALSE
//#define EFI_MALFUNCTION_INDICATOR FALSE

#define CONSOLE_MAX_ACTIONS 180
//#define EFI_DISABLE_CONSOLE_ACTIONS TRUE

#define EFI_MAP_AVERAGING TRUE

#define EFI_INTERNAL_FAST_ADC_GPT	&GPTD2
#define ADC_MAX_CHANNELS_COUNT 32

#define EFI_FASTER_UNIFORM_ADC TRUE
#define ADC_BUF_NUM_AVG        4
#define ADC_BUF_DEPTH_FAST     1

//ADC freq = ~40kHz
#define GPT_FREQ_FAST   400000   /* PWM clock frequency. */
#define GPT_PERIOD_FAST     10   /* PWM period (in PWM ticks).    */

#define EFI_SPI1_AF PAL_MODE_ALTERNATIVE_SPI

#define EFI_SPI2_AF PAL_MODE_ALTERNATIVE_SPI

/**
 * This section is for right-side center SPI
 */

#define EFI_SPI3_AF PAL_MODE_ALTERNATIVE_SPI

#define EFI_I2C_SCL_BRAIN_PIN GPIOB_6

#define EFI_I2C_SDA_BRAIN_PIN GPIOB_7
#define EFI_I2C_AF 4

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

#undef TS_SECONDARY_UART
#undef TS_SECONDARY_SERIAL

// todo: add CAN support
//#define TS_CAN_DEVICE CAND1
#define TS_CAN_AF PAL_MODE_ALTERNATIVE_CAN
#define TS_CAN_DEVICE_SHORT_PACKETS_IN_ONE_FRAME

#undef TS_PRIMARY_SERIAL
#undef TS_PRIMARY_UART

#define EFI_USB_SERIAL TRUE
#define EFI_CONSOLE_USB_DEVICE SDU1
// Cypress uses a fake USB device that's just a plain channel
#define SerialUSBDriver BaseChannel

#define EFI_CONSOLE_TX_PORT GPIOA
#define EFI_CONSOLE_TX_PIN 10
#define EFI_CONSOLE_RX_PORT GPIOA
#define EFI_CONSOLE_RX_PIN 11
#define EFI_CONSOLE_AF 3

#define TS_SERIAL_AF 2

#undef SERIAL_SPEED
#define SERIAL_SPEED 115200

//#define SR5_WRITE_TIMEOUT TIME_MS2I(3000)
//#define SR5_READ_TIMEOUT TIME_MS2I(3000)

#define HAL_TRIGGER_USE_PAL FALSE
#define HAL_TRIGGER_USE_ADC TRUE

//#define EFI_COMP_PRIMARY_DEVICE (&COMPD3)
//#define EFI_COMP_TRIGGER_CHANNEL 6		// =E7
//#define EFI_TRIGGER_DEBUG_BLINK TRUE
//#define EFI_TRIGGER_COMP_ADAPTIVE_HYSTERESIS TRUE

// LED1 = GPIOJ_15
// LED2 = GPIOJ_0
// LED3 = GPIOJ_12
// LED4 = GPIOA_0

#define LED_WARNING_BRAIN_PIN GPIOA_0

#define LED_CRITICAL_ERROR_BRAIN_PIN GPIOJ_15
#define LED_ERROR_BRAIN_PIN_MODE INVERTED_OUTPUT

#define EFI_WARNING_LED FALSE

#undef CONSOLE_MODE_SWITCH_PORT
#undef CONFIG_RESET_SWITCH_PORT

/**
 * This is the size of the MemoryStream used by chvprintf
 */
#define STATUS_LOGGING_BUFFER_SIZE 1800
#define SETTINGS_LOGGING_BUFFER_SIZE 1000
#define DL_OUTPUT_BUFFER 6500

//#define UTILITY_THREAD_STACK_SIZE 270 /*400*/

//#define CONSOLE_THREAD_STACK_SIZE UTILITY_THREAD_STACK_SIZE

#define BOARD_TLE6240_COUNT 0
#define BOARD_MC33972_COUNT 0
#define BOARD_TLE8888_COUNT 0
#define BOARD_DRV8860_COUNT 1
#define BOARD_MC33810_COUNT 0

#define DRV8860_SS_PORT GPIOH
#define DRV8860_SS_PAD  11U
#define DRV8860_RESET_PORT NULL
#define DRV8860_RESET_PAD  0
#define DRV8860_DIRECT_IO \
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

#define EFI_PRINT_ERRORS_AS_WARNINGS TRUE
//#define EFI_PRINT_MESSAGES_TO_TERMINAL TRUE

//#define PWM_PHASE_MAX_COUNT 122

//!!!!!!!!!!!!!!!!!!!!!!
#define debugLog(fmt,...) { \
	extern int __debugEnabled; \
	if (__debugEnabled) { \
		extern SerialUSBDriver EFI_CONSOLE_USB_DEVICE; \
		extern char __debugBuffer[200]; \
		chsnprintf(__debugBuffer, sizeof(__debugBuffer), fmt, ##__VA_ARGS__); \
		chnWriteTimeout(&EFI_CONSOLE_USB_DEVICE, (const uint8_t *)__debugBuffer, strlen(__debugBuffer), TIME_MS2I(1000)); \
		chThdSleepMilliseconds(20); \
	} \
}



#endif /* EFIFEATURES_H_ */
