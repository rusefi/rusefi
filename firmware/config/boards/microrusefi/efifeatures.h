/**
 * @file efifeatures.h
 *
 * @brief In this header we can configure which firmware modules are used.
 *
 * @date June 10, 2019
 * @author Matthew Kennedy, (c) 2019
 */

#ifndef EFIFEATURES_MICRORUSEFI_H_
#define EFIFEATURES_MICRORUSEFI_H_

#define EFI_GPIO_HARDWARE TRUE

#define EFI_FSIO TRUE

#define EFI_TEXT_LOGGING TRUE

#define EFI_PWM_TESTER FALSE

#define EFI_MC33816 FALSE

#define HAL_USE_USB_MSD FALSE

#define EFI_ENABLE_CRITICAL_ENGINE_STOP TRUE
#define EFI_ENABLE_ENGINE_WARNING TRUE

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
#define EFI_WAVE_ANALYZER TRUE

#ifndef EFI_ICU_INPUTS
#define EFI_ICU_INPUTS TRUE
#endif

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

#define EFI_ALTERNATOR_CONTROL TRUE

#define EFI_AUX_PID TRUE

#define EFI_SIGNAL_EXECUTOR_SLEEP FALSE
#define EFI_SIGNAL_EXECUTOR_ONE_TIMER TRUE
#define EFI_SIGNAL_EXECUTOR_HW_TIMER FALSE

#ifndef EFI_INTERNAL_FLASH
#define EFI_INTERNAL_FLASH TRUE
#endif

#define EFI_SHAFT_POSITION_INPUT TRUE
#define EFI_ENGINE_CONTROL TRUE
#define EFI_SPEED_DENSITY TRUE

// microrusefi has 1x tle8888
#define BOARD_TLE8888_COUNT 	1

// todo: move this outside of efifeatures.h
#define BOARD_EXT_GPIOCHIPS			(BOARD_TLE8888_COUNT)

// todo: move this outside of efifeatures.h
#define BOARD_EXT_PINREPOPINS 24

#define EFI_ANALOG_SENSORS TRUE

#define EFI_INTERNAL_ADC TRUE

#define EFI_NARROW_EGO_AVERAGING TRUE

#define EFI_CAN_SUPPORT TRUE

#define EFI_IDLE_CONTROL TRUE

#define EFI_IDLE_INCREMENTAL_PID_CIC FALSE

/**
 * Control the main power relay based on measured ignition voltage (Vbatt)
 */
#define EFI_MAIN_RELAY_CONTROL FALSE

#define EFI_PWM TRUE
#define EFI_VEHICLE_SPEED TRUE
#define EFI_FUEL_PUMP TRUE

#define EFI_ENGINE_EMULATOR TRUE
#define EFI_EMULATE_POSITION_SENSORS TRUE

/**
 * This macros is used to hide hardware-specific pieces of the code from unit tests and simulator, so it only makes
 * sense in folders exposed to the tests projects (simulator and unit tests).
 * This macros is NOT about taking out logging in general.
 * See also EFI_UNIT_TEST
 * See also EFI_SIMULATOR
 * todo: do we want to rename any of these three options?
 */
#define EFI_PROD_CODE TRUE

#ifndef EFI_USB_SERIAL
#define EFI_USB_SERIAL TRUE
#endif

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
#define EFI_SENSOR_CHART TRUE


#if defined __GNUC__
#define EFI_PERF_METRICS FALSE
#define DL_OUTPUT_BUFFER 6500
#else
#define EFI_PERF_METRICS FALSE
#define DL_OUTPUT_BUFFER 8000
#endif

#define EFI_ELECTRONIC_THROTTLE_BODY TRUE

/**
 * Do we need Malfunction Indicator blinking logic?
 */
#define EFI_MALFUNCTION_INDICATOR TRUE

#define CONSOLE_MAX_ACTIONS 180

#define EFI_MAP_AVERAGING TRUE


// todo: switch to continues ADC conversion for slow ADC?
// https://github.com/rusefi/rusefi/issues/630
#define EFI_INTERNAL_SLOW_ADC_PWM	&PWMD8
// todo: switch to continues ADC conversion for fast ADC?
#define EFI_INTERNAL_FAST_ADC_PWM	&PWMD4

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
 * Use 'HAL_USE_UART' DMA-mode driver instead of 'HAL_USE_SERIAL'
 *
 * See also
 *  STM32_SERIAL_USE_USARTx
 *  STM32_UART_USE_USARTx
 * in mcuconf.h
 */
#define TS_UART_DMA_MODE FALSE



// LED_ERROR_BRAIN_PIN should match LED_ERROR_PORT/LED_ERROR_PIN
#define LED_ERROR_BRAIN_PIN GPIOD_14
#define LED_ERROR_PORT GPIOD
#define LED_ERROR_PIN 14


#ifndef CONFIG_RESET_SWITCH_PIN
#define CONFIG_RESET_SWITCH_PIN 6
#endif

/**
 * This is the size of the MemoryStream used by chvprintf
 */
#define INTERMEDIATE_LOGGING_BUFFER_SIZE 2000

#endif /* EFIFEATURES_STM32F4_H_ */
