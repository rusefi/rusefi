/**
 * @file le_functions.h
 *
 * @date Oct 5, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef LE_FUNCTIONS_H_
#define LE_FUNCTIONS_H_

#include "engine.h"
#include "logic_expression.h"

#define FUEL_PUMP_LOGIC "time_since_boot 4 < rpm 0 > OR"

float getLEValue(Engine *engine, le_action_e action);

#endif /* LE_FUNCTIONS_H_ */
