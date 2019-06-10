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
#include "system_fsio.h"

typedef Map3D<FSIO_TABLE_8, FSIO_TABLE_8, float, float> fsio8_Map3D_f32t;
typedef Map3D<FSIO_TABLE_8, FSIO_TABLE_8, uint8_t, float> fsio8_Map3D_u8t;

#define MAGIC_OFFSET_FOR_ENGINE_WARNING 4
#define MAGIC_OFFSET_FOR_CRITICAL_ENGINE 5

float getEngineValue(le_action_e action DECLARE_ENGINE_PARAMETER_SUFFIX);
void setFsio(int index, brain_pin_e pin, const char * exp DECLARE_CONFIG_PARAMETER_SUFFIX);
void setFsioExt(int index, brain_pin_e pin, const char * exp, int pwmFrequency DECLARE_CONFIG_PARAMETER_SUFFIX);

void initFsioImpl(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_SUFFIX);
void runFsio(DECLARE_ENGINE_PARAMETER_SIGNATURE);
float getFsioOutputValue(int index DECLARE_ENGINE_PARAMETER_SUFFIX);
void applyFsioConfiguration(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void onConfigurationChangeFsioCallback(engine_configuration_s *previousConfiguration DECLARE_ENGINE_PARAMETER_SUFFIX);

#endif /* LE_FUNCTIONS_H_ */
