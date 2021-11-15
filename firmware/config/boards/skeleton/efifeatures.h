/**
 * @file efifeatures.h
 *
 * @brief Configure which firmware modules are used.
 * @author Donald Becker October 2019
 * @author Hugo Becker November 2019
 *
 * This configuration is a "skeleton" example for RusEFI boards.
 *
 */
 
#pragma once

// General software features

// Use the GPIO port setup code -- almost always set.
#define EFI_GPIO_HARDWARE TRUE
// Internal ADC -- almost always set.
#define EFI_INTERNAL_ADC TRUE
#define EFI_ANALOG_SENSORS TRUE

// Console I/O features to monitor formulas and pin state
#define EFI_FSIO TRUE

// Log crank/cam sensor events, a frequently needed diag for new installations
#define EFI_TOOTH_LOGGER TRUE

// Log other events, see also EFI_PRINT_MESSAGES_TO_TERMINAL
#define EFI_TEXT_LOGGING TRUE

// Monitor changes to Default settings that create failures -- note spelling
#define EFI_DEFAILED_LOGGING FALSE

// Build the logic analyzer support.
// A logic analyzer viewer is included in the java console.
#define EFI_LOGIC_ANALYZER TRUE

// A development feature to test output jitter and consistency
#define EFI_PWM_TESTER FALSE

#define EFI_ENABLE_CRITICAL_ENGINE_STOP TRUE
#define EFI_ENABLE_ENGINE_WARNING TRUE

#define EFI_CAN_SUPPORT TRUE

// Internal MCU features

// Hardware feature and chip support
// Some require a non-zero count to include support, others are TRUE/FALSE
// Other inconsistencies, such as naming, abound.

// Capacitive Discharge Module ion sense for detontation/knock detection
#define EFI_CDM_INTEGRATION FALSE

// MCP42010 digital potentiometer
#define EFI_POTENTIOMETER FALSE
// MC33816 Programmable Gate Driver over SPI
#define EFI_MC33816 FALSE

#define EFI_HPFP FALSE
// MAX31855 Thermocouple interface over SPI
#define EFI_MAX_31855 FALSE
// MCP3208 ADC over SPI
#define EFI_MCP_3208 FALSE
// HIP9011 Knock / Detonation Detector SPI config
#define EFI_HIP_9011 FALSE
// Bosch CJ125 Wideband Exhaust Gas Oxygen Sensor interface 
#define EFI_CJ125 FALSE
// LIS302DL MEMS Accelerometer over SPI, as on F4 Discovery board.
#define EFI_MEMS FALSE

// HD44780 Character LCD, the only client of EFI_LCD
#define EFI_HD44780_LCD FALSE
#define EFI_LCD FALSE
// and the closely related joystick input for the LCD menu system.
#define EFI_JOYSTICK FALSE


#define BOARD_TLE6240_COUNT	0
#define BOARD_MC33972_COUNT	0
#define BOARD_TLE8888_COUNT 	0
#define BOARD_MC33810_COUNT	0

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


#define EFI_ICU_INPUTS FALSE

#ifndef HAL_TRIGGER_USE_PAL
#define HAL_TRIGGER_USE_PAL TRUE
#endif /* HAL_TRIGGER_USE_PAL */

#ifndef HAL_TRIGGER_USE_ADC
#define HAL_TRIGGER_USE_ADC FALSE
#endif /* HAL_TRIGGER_USE_ADC */

// TunerStudio support.
#define EFI_TUNER_STUDIO TRUE
#define EFI_TUNER_STUDIO_VERBOSE TRUE		// Debugging output

// HC-06 Bluetooth module UART setup (output an initial configuration string)
#define EFI_BLUETOOTH_SETUP FALSE
// Serial port NMEA GPS reporting
#define EFI_UART_GPS FALSE


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

#define EFI_ENGINE_CONTROL TRUE

#define EFI_ANALOG_SENSORS TRUE


#define EFI_NARROW_EGO_AVERAGING TRUE

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

#define EFI_USB_SERIAL TRUE

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
//#define EFI_UART_GPS FALSE

#define EFI_SERVO TRUE

#define EFI_ELECTRONIC_THROTTLE_BODY TRUE


// MIL Malfunction Indicator Lamp logic

#define EFI_MALFUNCTION_INDICATOR TRUE

#define CONSOLE_MAX_ACTIONS 180

#define EFI_MAP_AVERAGING TRUE

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


// Future: Consistently use consoleUartDevice
#ifndef EFI_CONSOLE_SERIAL_DEVICE
#define EFI_CONSOLE_SERIAL_DEVICE (&SD3)
#endif

/**
 * Use 'HAL_USE_UART' DMA-mode driver instead of 'HAL_USE_SERIAL'
 *
 * See also
 *  STM32_SERIAL_USE_USARTx
 *  STM32_UART_USE_USARTx
 * in mcuconf.h
 */
#define TS_UART_DMA_MODE FALSE

#define PRIMARY_UART_DMA_MODE FALSE

//#define TS_UART_DEVICE (&UARTD3)
#define TS_SERIAL_DEVICE (&SD3)

// todo: add DMA-mode for Console?
#if (TS_UART_DMA_MODE || TS_UART_MODE)
#undef EFI_CONSOLE_SERIAL_DEVICE
#endif

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

// Enable file logging (like SD card) logic
#define EFI_FILE_LOGGING FALSE

#define EFI_USB_SERIAL TRUE

// GPS reporting NMEA protocol on a serial port
#undef EFI_UART_GPS
#define EFI_UART_GPS FALSE

// consoleUartDevice is unused but provided on UART4 Tx:PC10 Rx:PC11
// USART3 would work on the same pins but is reserved for future LIN bus use.
//  ToDo: Fix so that UART4 will work here.

#define HAL_USE_SERIAL_USB TRUE
#undef EFI_CONSOLE_SERIAL_DEVICE
#undef TS_UART_DEVICE
#undef TS_SERIAL_DEVICE
#undef TS_UART_MODE
#define EFI_CONSOLE_SERIAL_DEVICE (&SD1)
#define EFI_UART_ECHO_TEST_MODE TRUE

// USART3 is Alternate Function 7, UART4 is AF8
// todo: start using consoleSerial{Tx,Rx}Pin
#define EFI_CONSOLE_AF 7
#define TS_SERIAL_AF 7
#define EFI_CONSOLE_TX_BRAIN_PIN GPIOC_10
#define EFI_CONSOLE_RX_BRAIN_PIN GPIOC_11

// todo: document the limitations of DMA mode for the UART.
#undef TS_UART_DMA_MODE
#define TS_UART_DMA_MODE FALSE

// todo: add DMA-mode for Console?
#if (TS_UART_DMA_MODE || TS_UART_MODE)
#undef EFI_CONSOLE_SERIAL_DEVICE
#endif
