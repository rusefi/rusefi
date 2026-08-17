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

/* Console log buffer (double-buffered, 2x this, static). The stm32f4ems
 * default of 6500 truncates the 'pins' output mid-line: the pin report +
 * L9779 SPI frame history exceed it and LogBuffer::writeInternal writes
 * partial lines when full. 16384 x 2 = 32 KB static - fine on the 384 KB
 * AT32F435 (the memcore pool still has ~200 KB left). */
#define DL_OUTPUT_BUFFER 16384

#include "../../stm32f4ems/efifeatures.h"

/* This board has no USB wired out */
#undef EFI_USB_SERIAL
#define EFI_USB_SERIAL              FALSE

/* Fast ADC (TIM6 -> ADC2, MAP sampling) is enabled (default TRUE from
 * stm32f4ems) and functional: the AT32 DMA1/DMA2 DMAMUX (TBL_SEL flexible
 * mapping) is fixed in the ChibiOS fork - see
 * ChibiOS/os/hal/ports/AT32/AT32F4xx/hal_lld.c (DMA clocks before dmaInit)
 * and at32_registry.h (STM32_DMA_HAS_DMAMUXSEL). */

/* CLT/IAT (and O2/AC-pressure) live on ADC3-only pins (PF5/PF6/F10/F3) - the
 * slow ADC (ADC1) cannot see them. Sample ADC3 channels EFI_ADC_32..39 in the
 * slow loop (stm32_adc_v2.cpp convGroupSlowAdc3). */
#define EFI_ADC3_SLOW                TRUE
