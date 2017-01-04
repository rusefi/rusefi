/**
 * @file efifeatures.h
 *
 * @brief In this header we can configure which firmware modules are used.
 *
 * @date Aug 29, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef EFIFEATURES_H_
#define EFIFEATURES_H_

#define EFI_GPIO TRUE

#define EFI_FSIO TRUE

#define EFI_PWM_TESTER FALSE

#define EFI_USE_CCM TRUE

/**
 * if you have a 60-2 trigger, or if you just want better performance, you
 * probably want EFI_ENABLE_ASSERTS to be FALSE. Also you would probably want to FALSE
 * CH_DBG_ENABLE_CHECKS
 * CH_DBG_ENABLE_ASSERTS
 * CH_DBG_ENABLE_TRACE
 * in chconf.h
 *
 */
#if !defined(EFI_ENABLE_ASSERTS) || defined(__DOXYGEN__)
 #define EFI_ENABLE_ASSERTS TRUE
#endif /* EFI_ENABLE_ASSERTS */

#if !defined(EFI_ENABLE_MOCK_ADC) || defined(__DOXYGEN__)
 #define EFI_ENABLE_MOCK_ADC TRUE
#endif /* EFI_ENABLE_MOCK_ADC */


//#define EFI_UART_ECHO_TEST_MODE TRUE

#define EFI_USE_UART_FOR_CONSOLE FALSE

/**
 * Build-in logic analyzer support. Logic analyzer viewer is one of the java console panes.
 */
#define EFI_WAVE_ANALYZER TRUE

//#define SERIAL_SPEED (8 * 115200)
//#define SERIAL_SPEED (2 * 115200)
//todo: make this configurable via Tuner Studio
//todo: see uartConsoleSerialSpeed
#define SERIAL_SPEED 115200

/**
 * TunerStudio support.
 */
#define EFI_TUNER_STUDIO TRUE

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

#define EFI_INTERNAL_FLASH TRUE

/**
 * Usually you need shaft position input, but maybe you do not need it?
 */
#define EFI_SHAFT_POSITION_INPUT TRUE

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
#define EFI_POTENTIOMETER TRUE

#define EFI_ANALOG_SENSORS TRUE

#define EFI_MAX_31855 TRUE

#define EFI_MCP_3208 FALSE

#define EFI_HIP_9011 TRUE

#define EFI_CJ125 TRUE

#define EFI_INTERNAL_ADC TRUE

#define EFI_DENSO_ADC FALSE

#define EFI_CAN_SUPPORT TRUE

#define EFI_HD44780_LCD TRUE

#define EFI_IDLE_CONTROL TRUE

#define EFI_PWM TRUE

#define EFI_VEHICLE_SPEED TRUE

#define EFI_FUEL_PUMP TRUE

#define EFI_ENGINE_EMULATOR TRUE
#define EFI_EMULATE_POSITION_SENSORS TRUE

/**
 * This macros is used to hide pieces of the code from unit tests, so it only makes sense in folders exposed to the tests project.
 * This macros is NOT about taking out logging in general.
 */
#define EFI_PROD_CODE TRUE

/**
 * Do we need file logging (like SD card) logic?
 */
#define EFI_FILE_LOGGING TRUE

#define EFI_USB_SERIAL TRUE

/**
 * While we embed multiple PnP configurations into the same firmware binary, these marcoses give us control
 * over which configurations go into the binary
 */
#define EFI_SUPPORT_DODGE_NEON TRUE
#define EFI_SUPPORT_FORD_ASPIRE TRUE
#define EFI_SUPPORT_FORD_FIESTA TRUE
#define EFI_SUPPORT_NISSAN_PRIMERA TRUE
#define EFI_SUPPORT_1995_FORD_INLINE_6 TRUE

#define EFI_ENGINE_SNIFFER TRUE

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
#define EFI_UART_GPS TRUE
//#define EFI_UART_GPS FALSE

#define EFI_ELECTRONIC_THROTTLE_BODY TRUE
//#define EFI_ELECTRONIC_THROTTLE_BODY FALSE

/**
 * Do we need Malfunction Indicator blinking logic?
 */
#define EFI_MALFUNCTION_INDICATOR TRUE
//#define EFI_MALFUNCTION_INDICATOR FALSE

#define CONSOLE_MAX_ACTIONS 196

#define EFI_MAP_AVERAGING TRUE
//#define EFI_MAP_AVERAGING FALSE

// todo: most of this should become configurable

// todo: switch to continues ADC conversion for slow ADC?
#define EFI_INTERNAL_SLOW_ADC_PWM	&PWMD8
// todo: switch to continues ADC conversion for fast ADC?
#define EFI_INTERNAL_FAST_ADC_PWM	&PWMD4

#define EFI_CAN_DEVICE CAND2
#define EFI_CAN_RX_AF 9
#define EFI_CAN_TX_AF 9

//#define EFI_CAN_DEVICE CAND1
//#define EFI_CAN_RX_AF 9
//#define EFI_CAN_TX_AF 9

/**
 * This section is for bottom-left corner SPI
 */
//#define SPI_CS1_PORT GPIOE
//#define SPI_CS1_PIN 13
//#define SPI_CS2_PORT GPIOE
//#define SPI_CS2_PIN 14
//#define SPI_CS3_PORT GPIOE
//#define SPI_CS3_PIN 15
//#define SPI_CS4_PORT GPIOD
//#define SPI_CS4_PIN 10

#define EFI_SPI1_AF 5

#define EFI_SPI2_AF 5


/**
 * This section is for right-side center SPI
 */
// this is pointing into the sky for now - conflict with I2C
#define SPI_CS2_PORT GPIOH
// this is pointing into the sky for now - conflict with I2C
#define SPI_CS2_PIN 0
#define SPI_CS4_PORT GPIOD
#define SPI_CS4_PIN 3
#define EFI_SPI3_AF 6
#define MMC_CARD_SPI SPID3

#define EFI_I2C_SCL_PORT GPIOB
#define EFI_I2C_SCL_PIN 6
#define EFI_I2C_SDA_PORT GPIOB
#define EFI_I2C_SDA_PIN 7
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


// todo: start using consoleUartDevice
#define EFI_CONSOLE_UART_DEVICE (&SD3)

// todo: start using console_tx_pin
#define EFI_CONSOLE_TX_PORT GPIOC
#define EFI_CONSOLE_TX_PIN 10
// todo: start using console_rx_pin
#define EFI_CONSOLE_RX_PORT GPIOC
#define EFI_CONSOLE_RX_PIN 11
// todo: this should be detected automatically based on pin selection
#define EFI_CONSOLE_AF 7

// todo: this should be detected automatically based on pin selection
#define TS_SERIAL_AF 7

#define LED_WARNING_PORT GPIOD
#define LED_WARNING_PIN 13

#define LED_ERROR_PORT GPIOD
#define LED_ERROR_PIN 14

#define EFI_WARNING_LED TRUE

// USART1 -> check defined STM32_SERIAL_USE_USART1
// For GPS we have USART1. We can start with PB7 USART1_RX and PB6 USART1_TX
#define GPS_SERIAL_DEVICE &SD1
#define GPS_SERIAL_SPEED 38400

#define CONFIG_RESET_SWITCH_PORT GPIOD
#define CONFIG_RESET_SWITCH_PIN 6

/**
 * This is the size of the MemoryStream used by chvprintf
 */
#define INTERMEDIATE_LOGGING_BUFFER_SIZE 2000

#endif /* EFIFEATURES_H_ */
