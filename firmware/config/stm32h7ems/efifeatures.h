#include "../stm32f7ems/efifeatures.h"

#pragma once

#undef EFI_USE_FAST_ADC
// https://github.com/rusefi/rusefi/issues/3301 "H7 is currently actually using fast ADC exclusively - it just needs a bit of plumbing to make it work."
#define EFI_USE_FAST_ADC FALSE

#undef EFI_MC33816
#define EFI_MC33816 FALSE

#undef EFI_CJ125
#define EFI_CJ125 FALSE

#undef BOARD_TLE6240_COUNT
#undef BOARD_MC33972_COUNT
#undef BOARD_TLE8888_COUNT
#define BOARD_TLE6240_COUNT	0
#define BOARD_MC33972_COUNT	0
#define BOARD_TLE8888_COUNT 	0

#undef EFI_MAX_31855
#define EFI_MAX_31855 FALSE

#undef BOARD_EXT_GPIOCHIPS
#define BOARD_EXT_GPIOCHIPS			(BOARD_TLE6240_COUNT + BOARD_MC33972_COUNT + BOARD_TLE8888_COUNT + BOARD_DRV8860_COUNT + BOARD_MC33810_COUNT)

#define EFI_USE_COMPRESSED_INI_MSD

// H7 has dual bank, so flash on its own (low priority) thread so as to not block any other operations
#define EFI_FLASH_WRITE_THREAD TRUE

#undef ENABLE_PERF_TRACE
#define ENABLE_PERF_TRACE TRUE

// H7 runs faster "slow" ADC to make up for reduced oversampling
#define SLOW_ADC_RATE 1000

#undef EFI_ICU_INPUTS
#define EFI_ICU_INPUTS FALSE

#undef HAL_TRIGGER_USE_PAL
#define HAL_TRIGGER_USE_PAL TRUE

#undef EFI_LOGIC_ANALYZER
#define EFI_LOGIC_ANALYZER FALSE

#undef HAL_VSS_USE_PAL
#define HAL_VSS_USE_PAL TRUE
