/**
 * @file boards/subarue_eg33/halconf.h
 *
 * @brief In this header we can override halconf.h.
 *
 * @date Feb 06, 2021
 * @author Andrey Gusakov, 2021
 */

#ifndef _HALCONF_SUBARUEG33_H_
#define _HALCONF_SUBARUEG33_H_

/* this file is exist just to include mcuconf.h from THIS directory */
#include "mcuconf.h"

#define HAL_USE_WSPI                    TRUE

#define SNOR_SHARED_BUS                 FALSE

#define WSPI_USE_MUTUAL_EXCLUSION       FALSE

#include "../../../hw_layer/ports/stm32/stm32f7/cfg/halconf.h"

#endif /* _HALCONF_SUBARUEG33_H_ */
