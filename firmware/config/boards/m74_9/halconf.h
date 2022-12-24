/**
 * @file boards/m74_9/halconf.h
 *
 * @brief In this header we can override halconf.h.
 *
 * @date Jan 01, 2023
 * @author Andrey Gusakov, 2023
 */

#ifndef _HALCONF_M74_9_H_
#define _HALCONF_M74_9_H_

#define HAL_USE_SDC                     FALSE

#define HAL_USE_MMC_SPI                 FALSE

#include "../../../hw_layer/ports/stm32/stm32f4/cfg/halconf.h"

#endif /* _HALCONF_M74_9_H_ */
