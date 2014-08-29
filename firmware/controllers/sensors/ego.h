/**
 * @file    ego.h
 * @brief
 *
 *
 * @date Nov 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef EGO_H_
#define EGO_H_

#include "main.h"
#include "rusefi_enums.h"
#include "engine_configuration.h"

float getAfr(void);
void initEgoSensor(afr_sensor_s *sensor, ego_sensor_e type);

#endif
