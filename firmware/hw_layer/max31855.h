/**
 * @file max31855.h
 *
 * @date Sep 17, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef MAX31855_H_
#define MAX31855_H_

#include "main.h"
#include "engine_configuration.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

void initMax31855(SPIDriver *drv, egt_cs_array_t max31855_cs);
uint16_t getEgtValue(int egtChannel);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MAX31855_H_ */
