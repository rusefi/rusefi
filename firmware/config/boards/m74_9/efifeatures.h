/**
 * @file boards/m74_9/efifeatures.h
 *
 * @brief In this header we can override efifeatures.h.
 *
 * @date Jan 01, 2023
 * @author Andrey Gusakov, 2023
 */

/* Override some settings */
#define HAL_USE_MMC_SPI FALSE
#define EFI_FILE_LOGGING FALSE

#include "../../stm32f4ems/efifeatures.h"

/* This board has no USB wired out */
#undef EFI_USB_SERIAL
#define EFI_USB_SERIAL              FALSE

/* The fast ADC (TIM6 GPT -> ADC2, adc_onchip_fast.cpp) does not run on this
 * chip: fastAdcConversionCount stays 0 and MAP (the only fast channel) reads
 * 0. Sample MAP through the slow ADC instead - the !EFI_USE_FAST_ADC stubs in
 * stm32_adc_v2.cpp keep the shared code linking. */
#undef EFI_USE_FAST_ADC
#define EFI_USE_FAST_ADC            FALSE
