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

typedef Map3D<SCRIPT_TABLE_8, SCRIPT_TABLE_8, float, float, float> fsio8_Map3D_f32t;
typedef Map3D<SCRIPT_TABLE_8, SCRIPT_TABLE_8, uint8_t, float, float> fsio8_Map3D_u8t;

expected<float> getEngineValue(le_action_e action);

void initFsioImpl();
void runFsio();

void onConfigurationChangeFsioCallback(engine_configuration_s *previousConfiguration);
void runHardcodedFsio();

float getCurveValue(int index, float key);
int getCurveIndexByName(const char *name);
int getTableIndexByName(const char *name);
int getSettingIndexByName(const char *name);
ValueProvider3D *getscriptTable(int index);

