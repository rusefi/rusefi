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

/**
 * In human language that's
 * (time_since_boot < 4) OR (rpm > 0)
 */
// todo: the delay should probably be configurable?
#define FUEL_PUMP_LOGIC "time_since_boot 4 < rpm 0 > OR"

#define AC_RELAY_LOGIC "ac_on_switch"

#define ALTERNATOR_LOGIC "vbatt 14.5 <"

/**
 * In human language that's
 * (fan and (coolant > fan_off_setting)) OR (coolant > fan_on_setting)
 */
#define FAN_CONTROL_LOGIC "fan coolant fan_off_setting > & coolant fan_on_setting > OR"

float getLEValue(Engine *engine, calc_stack_t *s, le_action_e action);
void setFsio(engine_configuration_s *engineConfiguration, int index, brain_pin_e pin, const char * exp);

#endif /* LE_FUNCTIONS_H_ */
