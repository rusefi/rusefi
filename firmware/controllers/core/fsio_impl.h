/**
 * @file fsio_impl.h
 * @brief FSIO as it's used for GPIO
 *
 * @date Oct 5, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "fsio_core.h"
#include "expected.h"
#include "system_fsio.h"

// see useFSIO4ForSeriousEngineWarning
#define MAGIC_OFFSET_FOR_ENGINE_WARNING 4
// see useFSIO5ForCriticalIssueEngineStop
#define MAGIC_OFFSET_FOR_CRITICAL_ENGINE 5

typedef Map3D<SCRIPT_TABLE__8, SCRIPT_TABLE__8, float, float, float> fsio8_Map3D_f32t;
typedef Map3D<SCRIPT_TABLE__8, SCRIPT_TABLE__8, uint8_t, float, float> fsio8_Map3D_u8t;

expected<float> getEngineValue(le_action_e action DECLARE_ENGINE_PARAMETER_SUFFIX);

void initFsioImpl(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void runFsio(DECLARE_ENGINE_PARAMETER_SIGNATURE);

void onConfigurationChangeFsioCallback(engine_configuration_s *previousConfiguration DECLARE_ENGINE_PARAMETER_SUFFIX);
void runHardcodedFsio(DECLARE_ENGINE_PARAMETER_SIGNATURE);

float getCurveValue(int index, float key DECLARE_ENGINE_PARAMETER_SUFFIX);
int getCurveIndexByName(const char *name DECLARE_ENGINE_PARAMETER_SUFFIX);
int getTableIndexByName(const char *name DECLARE_ENGINE_PARAMETER_SUFFIX);
ValueProvider3D *getscriptTable(int index);

