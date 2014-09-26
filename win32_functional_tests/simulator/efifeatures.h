/*
 * efifeatures.h
 *
 *  Created on: Mar 7, 2014
 *      Author: Andrey
 */

#ifndef EFIFEATURES_H_
#define EFIFEATURES_H_

#define EFI_SIMULATOR TRUE

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

#define EFI_WAVE_CHART TRUE

#define EFI_ANALOG_CHART TRUE
#define EFI_HISTOGRAMS TRUE

#define EFI_TUNER_STUDIO TRUE

#endif /* EFIFEATURES_H_ */
