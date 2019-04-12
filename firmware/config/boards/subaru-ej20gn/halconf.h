/**
 * @file boards/subaruej20g/halconf.h
 *
 * @brief In this header we can override halconf.h.
 *
 * @date Feb 19, 2019
 * @author Andrey Gusakov, 2019
 */

#include "mcuconf.h"
#include "../../stm32f7ems/halconf.h"

#ifndef _HALCONF_SUBARUEJ20G_H_
#define _HALCONF_SUBARUEJ20G_H_

#undef HAL_USE_PAL
#define HAL_USE_PAL				TRUE
#undef PAL_USE_CALLBACKS
#define PAL_USE_CALLBACKS		TRUE

#undef HAL_USE_I2C
#define HAL_USE_I2C				FALSE

#undef HAL_USE_SERIAL
#define HAL_USE_SERIAL			TRUE

//#if TS_UART_DMA_MODE
#undef HAL_USE_UART
#define HAL_USE_UART			TRUE
#undef UART_USE_WAIT
#define UART_USE_WAIT			TRUE
//#else
//#define HAL_USE_UART			FALSE
//#endif

#undef HAL_USE_USB_MSD
#define HAL_USE_USB_MSD			FALSE

#endif /* _HALCONF_SUBARUEJ20G_H_ */
