/**
 * @file fsio_impl.h
 * @brief FSIO as it's used for GPIO
 *
 * @date Oct 5, 2014
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef LE_FUNCTIONS_H_
#define LE_FUNCTIONS_H_

#include "fsio_core.h"
#include "engine.h"
#include "table_helper.h"

typedef Map3D<FSIO_TABLE_8, FSIO_TABLE_8, float> fsio8_Map3D_f32t;
typedef Map3D<FSIO_TABLE_8, FSIO_TABLE_8, uint8_t> fsio8_Map3D_u8t;

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
void setFsio(int index, brain_pin_e pin, const char * exp DECLARE_ENGINE_PARAMETER_S);
void setFsioExt(int index, brain_pin_e pin, const char * exp, int freq DECLARE_ENGINE_PARAMETER_S);

void initFsioImpl(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_S);
void runFsio(void);
void applyFsioConfiguration(DECLARE_ENGINE_PARAMETER_F);

#endif /* LE_FUNCTIONS_H_ */
