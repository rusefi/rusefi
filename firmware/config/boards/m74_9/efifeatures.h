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

/* Fast ADC (TIM6 -> ADC2, MAP sampling) is enabled (default TRUE from
 * stm32f4ems) and functional: the AT32 DMA1/DMA2 DMAMUX (TBL_SEL flexible
 * mapping) is fixed in the ChibiOS fork - see
 * ChibiOS/os/hal/ports/AT32/AT32F4xx/hal_lld.c (DMA clocks before dmaInit)
 * and at32_registry.h (STM32_DMA_HAS_DMAMUXSEL). */
