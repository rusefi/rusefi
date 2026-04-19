/**
 * @file efifeatures.h
 *
 * @brief In this header we can configure which firmware modules are used.
 *
 * STM32H7 config is inherited from STM32F7 and then from STM32F4.
 * This file contains only differences between F7 and H7.
 * This is more consistent way to maintain these config 'branches' and add new features.
 *
 * @date Aug 29, 2013
 * @author Andrey Belomutskiy, (c) 2012-2024
 */

#pragma once

#ifndef EFI_USE_COMPRESSED_INI_MSD
#define EFI_USE_COMPRESSED_INI_MSD TRUE
#endif

// note order of include - first we set H7 defaults (above) and only later we apply F4 defaults
#include "../stm32f7ems/efifeatures.h"

#undef EFI_USE_FAST_ADC
// https://github.com/rusefi/rusefi/issues/3301 "H7 is currently actually using fast ADC exclusively - it just needs a bit of plumbing to make it work."
#define EFI_USE_FAST_ADC FALSE

#undef EFI_MC33816
#define EFI_MC33816 FALSE

#undef BOARD_TLE6240_COUNT
#undef BOARD_MC33972_COUNT
#undef BOARD_TLE8888_COUNT
#undef BOARD_L9779_COUNT
#define BOARD_TLE6240_COUNT	0
#define BOARD_MC33972_COUNT	0
#define BOARD_TLE8888_COUNT 	0
#define BOARD_L9779_COUNT 0

#undef EFI_MAX_31855
#define EFI_MAX_31855 FALSE

#undef ENABLE_PERF_TRACE
#define ENABLE_PERF_TRACE TRUE

// H7 runs faster "slow" ADC to make up for reduced oversampling
#define SLOW_ADC_RATE 1000
