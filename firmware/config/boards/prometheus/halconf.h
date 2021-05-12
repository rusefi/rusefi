/**
 * @file boards/Prometheus/halconf.h
 *
 * @brief In this header we can override halconf.h.
 *
 * @date Apr 22, 2017
 * @author andreika, (c) 2017
 */
 
#include "efifeatures.h"
#include "mcuconf.h"
#include "../../../hw_layer/ports/stm32/stm32f4/cfg/halconf.h"

#ifndef _HALCONF_PROMETHEUS_H_
#define _HALCONF_PROMETHEUS_H_

#undef HAL_USE_SERIAL
#define HAL_USE_SERIAL FALSE

#endif /* _HALCONF_PROMETHEUS_H_ */
