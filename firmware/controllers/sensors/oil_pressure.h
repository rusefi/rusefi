/**
 * @date Nov 9, 2017
 * @author Matthew Kennedy, (c) 2017
 */

#ifndef OIL_PRESSURE_H_
#define OIL_PRESSURE_H_

#include "global.h"
#include "rusefi_enums.h"
#include "engine_configuration.h"

float getOilPressure(DECLARE_ENGINE_PARAMETER_SIGNATURE);
bool hasOilPressureSensor(DECLARE_ENGINE_PARAMETER_SIGNATURE);


#endif
