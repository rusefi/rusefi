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

// see useFSIO12ForIdleOffset
#define MAGIC_OFFSET_FOR_IDLE_OFFSET 12
// see useFSIO13ForIdleMinValue
#define MAGIC_OFFSET_FOR_IDLE_MIN_VALUE 13

typedef Map3D<FSIO_TABLE_8, FSIO_TABLE_8, float, float> fsio8_Map3D_f32t;
typedef Map3D<FSIO_TABLE_8, FSIO_TABLE_8, uint8_t, float> fsio8_Map3D_u8t;

#define MAGIC_OFFSET_FOR_ENGINE_WARNING 4
#define MAGIC_OFFSET_FOR_CRITICAL_ENGINE 5

float getEngineValue(le_action_e action DECLARE_ENGINE_PARAMETER_SUFFIX);
/**
 * set_fsio_output_pin 7 PE3
 * set_rpn_expression 1 "rpm 0 fsio_setting <"
 */
void setFsio(int index, brain_pin_e pin, const char * exp DECLARE_CONFIG_PARAMETER_SUFFIX);
void setFsioExt(int index, brain_pin_e pin, const char * exp, int pwmFrequency DECLARE_CONFIG_PARAMETER_SUFFIX);

void initFsioImpl(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_SUFFIX);
void runFsio(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void setFsioExpression(const char *indexStr, const char *quotedLine DECLARE_ENGINE_PARAMETER_SUFFIX);
float getFsioOutputValue(int index DECLARE_ENGINE_PARAMETER_SUFFIX);
void applyFsioConfiguration(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void onConfigurationChangeFsioCallback(engine_configuration_s *previousConfiguration DECLARE_ENGINE_PARAMETER_SUFFIX);

ValueProvider3D *getFSIOTable(int index);

#endif /* LE_FUNCTIONS_H_ */
