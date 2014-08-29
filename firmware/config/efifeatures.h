/**
 * @file efifeatures.h
 *
 * @brief In this header we can configure which firmware modules are used.
 *
 * @date Aug 29, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef EFIFEATURES_H_
#define EFIFEATURES_H_

#define EFI_USE_CCM TRUE

//#define EFI_UART_ECHO_TEST_MODE TRUE

#define EFI_USE_UART_FOR_CONSOLE FALSE

/**
 * Build-in logic analyzer support. Logic analyzer viewer is one of the java console panes.
 */
#define EFI_WAVE_ANALYZER TRUE

//#define SERIAL_SPEED (8 * 115200)
//#define SERIAL_SPEED (2 * 115200)
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

#define EFI_INTERNAL_FLASH TRUE

/**
 * Usually you need shaft position input, but maybe you do not need it?
 */
#define EFI_SHAFT_POSITION_INPUT TRUE

#define EFI_ANALOG_INPUTS TRUE

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

#define EFI_INTERNAL_ADC TRUE

#define EFI_DENSO_ADC FALSE

#define EFI_CAN_SUPPORT FALSE

#define EFI_HD44780_LCD TRUE

#define EFI_IDLE_CONTROL TRUE

#define EFI_FUEL_PUMP TRUE

#define EFI_ENGINE_EMULATOR TRUE
#define EFI_EMULATE_POSITION_SENSORS TRUE

/**
 * This macros is used to hide pieces of the code from unit tests, so it only makes sense in folders exposed to the tests project.
 * This macros is NOT about taking out logging in general.
 */
#define EFI_PROD_CODE TRUE

/**
 * Do we need Malfunction Indicator blinking logic?
 */
#define EFI_MALFUNCTIONAL_INDICATOR TRUE
/**
 * Do we need file logging (like SD card) logic?
 */
#define EFI_FILE_LOGGING TRUE

/**
 * While we embed multiple PnP configurations into the same firmware binary, these marcoses give us control
 * over which configurations go into the binary
 */
#define EFI_SUPPORT_DODGE_NEON TRUE
#define EFI_SUPPORT_FORD_ASPIRE TRUE
#define EFI_SUPPORT_FORD_FIESTA TRUE
#define EFI_SUPPORT_NISSAN_PRIMERA TRUE
#define EFI_SUPPORT_1995_FORD_INLINE_6 TRUE

#define EFI_WAVE_CHART TRUE

#define EFI_HISTOGRAMS TRUE
#define EFI_ANALOG_CHART TRUE

#if defined __GNUC__
#define EFI_PERF_METRICS TRUE
#define DL_OUTPUT_BUFFER 9000
#else
#define EFI_PERF_METRICS FALSE
#define DL_OUTPUT_BUFFER 9000
#endif

/**
 * Do we need GPS logic?
 */
#define EFI_UART_GPS TRUE
//#define EFI_UART_GPS FALSE

//#define EFI_ELECTRONIC_THROTTLE_BODY TRUE
#define EFI_ELECTRONIC_THROTTLE_BODY FALSE

#define EFI_MALFUNCTION_INDICATOR TRUE
//#define EFI_MALFUNCTION_INDICATOR FALSE

#define EFI_MAP_AVERAGING TRUE
//#define EFI_MAP_AVERAGING FALSE

#endif /* EFIFEATURES_H_ */
