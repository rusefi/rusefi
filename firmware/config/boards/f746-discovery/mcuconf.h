/**
 * @file boards/f746-discovery/mcuconf.h
 *
 * @brief STM32F746G-DISCO overrides of the common F7 mcuconf.h
 *
 * Clock and USB settings follow the ChibiOS STM32F746G-Discovery configs
 * (testhal/STM32/multi/USB_CDC/cfg/stm32f746_discovery/mcuconf.h).
 */

#pragma once

#include "../../../hw_layer/ports/stm32/stm32f7/cfg/mcuconf.h"

/*
 * Clocks: HSE is a 25 MHz clock supplied by the ST-LINK MCO (bypass mode,
 * STM32_HSECLK/STM32_HSE_BYPASS come from the stock ChibiOS board.h).
 * Because board.h defines STM32_HSECLK, runtime HSE auto-detection is off, so
 * the PLL source is chosen here: 25 MHz / 25 * 432 / 2 = 216 MHz SYSCLK,
 * 432 / 9 = 48 MHz PLL48CLK.
 */
#undef STM32_PLLSRC
#define STM32_PLLSRC                        STM32_PLLSRC_HSE
#undef STM32_PLLM_VALUE
#define STM32_PLLM_VALUE                    25

/*
 * USB: CN13 "USB HS" connector, USB3320 ULPI PHY on OTG_HS (USBD2).
 * BOARD_OTG2_USES_ULPI in board.h makes the OTGv1 driver use the ULPI PHY.
 * The CN12 "USB FS" connector (OTG_FS, PA11/PA12) is not used.
 */
#undef STM32_USB_USE_OTG1
#define STM32_USB_USE_OTG1                  FALSE
#undef STM32_USB_USE_OTG2
#define STM32_USB_USE_OTG2                  TRUE
