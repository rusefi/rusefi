/*
 * efifeatures.h
 *
 * @date Mar 7, 2014
 * @author Andrey Belomutskiy, (c) 2012-2016
 */

#ifndef EFIFEATURES_H_
#define EFIFEATURES_H_

#define EFI_ENABLE_ASSERTS TRUE

#define EFI_ENABLE_MOCK_ADC TRUE

#define EFI_PRINTF_FUEL_DETAILS TRUE

#define EFI_GPIO TRUE

#define EFI_FSIO TRUE

#define CONSOLE_MAX_ACTIONS 256

#define EFI_SIMULATOR TRUE
#define EFI_PROD_CODE FALSE

#define EFI_MAP_AVERAGING TRUE

#define EFI_SUPPORT_DODGE_NEON TRUE
#define EFI_SUPPORT_FORD_ASPIRE TRUE
#define EFI_SUPPORT_FORD_FIESTA TRUE
#define EFI_SUPPORT_NISSAN_PRIMERA TRUE
#define EFI_SUPPORT_1995_FORD_INLINE_6 TRUE

/**
 * simulator works via self-stimulation which works via trigger emulation
 */
#define EFI_EMULATE_POSITION_SENSORS TRUE

#define EFI_SIGNAL_EXECUTOR_SLEEP TRUE
#define EFI_INTERNAL_ADC FALSE

#define EFI_SHAFT_POSITION_INPUT TRUE

#define EFI_ENGINE_CONTROL TRUE

#define EFI_ENGINE_SNIFFER TRUE

#define EFI_ANALOG_SENSORS TRUE

#define EFI_SENSOR_CHART TRUE
#define EFI_HISTOGRAMS FALSE

#define EFI_TUNER_STUDIO TRUE

/**
 * This is the size of the MemoryStream used by chvprintf
 */
#define INTERMEDIATE_LOGGING_BUFFER_SIZE 2000

#endif /* EFIFEATURES_H_ */
