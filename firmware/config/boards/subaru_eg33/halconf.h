/**
 * @file boards/subarue_eg33/halconf.h
 *
 * @brief In this header we can override halconf.h.
 *
 * @date Feb 06, 2021
 * @author Andrey Gusakov, 2021
 */

#include "mcuconf.h"
#include "../../stm32f7ems/halconf.h"

#ifndef _HALCONF_SUBARUEG33_H_
#define _HALCONF_SUBARUEG33_H_

#undef HAL_TRIGGER_USE_PAL
#define HAL_TRIGGER_USE_PAL		TRUE

#undef HAL_USE_I2C
#define HAL_USE_I2C				FALSE

#undef HAL_USE_SERIAL
#define HAL_USE_SERIAL			TRUE

//#if TS_UART_DMA_MODE
#undef HAL_USE_UART
#define HAL_USE_UART			FALSE
#undef UART_USE_WAIT
#define UART_USE_WAIT			FALSE
//#else
//#define HAL_USE_UART			FALSE
//#endif

#endif /* _HALCONF_SUBARUEG33_H_ */
