/*
 * @file rusefi_hw_enums.h
 *
 * @date Jun 2, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 *
 */

#pragma once

/*
 * A platform can supply its own Gpio and adc_channel_e enums: set
 * HW_PIN_ENUMS_FILE / HW_ADC_ENUMS_FILE in the board meta-info.env (paths
 * relative to the board directory). rusefi.mk then defines
 * EFI_HW_PIN_ENUMS_FILE / EFI_HW_ADC_ENUMS_FILE for the compiler and
 * gen_config_board.sh passes the same files to the config generator, so
 * firmware and TunerStudio pin lists stay consistent.
 */
#ifdef EFI_HW_PIN_ENUMS_FILE
#include EFI_HW_PIN_ENUMS_FILE
#else
#include "rusefi_hw_stm32_enums.h"
#endif

#ifdef EFI_HW_ADC_ENUMS_FILE
#include EFI_HW_ADC_ENUMS_FILE
#else
#include "rusefi_hw_adc_enums.h"
#endif
