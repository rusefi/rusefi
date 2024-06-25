/**
 * @file script_impl.h
 *
 * @date Oct 5, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include <rusefi/expected.h>

typedef Map3D<SCRIPT_TABLE_8, SCRIPT_TABLE_8, float, int16_t, int16_t> script1_Map3D_f32t;
typedef Map3D<TABLE_2_RPM_SIZE, TABLE_2_LOAD_SIZE, float, int16_t, int16_t> script2_Map3D_f32t;
typedef Map3D<SCRIPT_TABLE_8, SCRIPT_TABLE_8, uint8_t, int16_t, int16_t> script3_Map3D_u8t;
typedef Map3D<TABLE_4_RPM, SCRIPT_TABLE_8, uint8_t, int16_t, int16_t> script4_Map3D_u8t;

void initScriptImpl();

float getCurveValue(int index, float key);
expected<int> getCurveIndexByName(const char *name);
expected<int> getTableIndexByName(const char *name);
expected<int> getSettingIndexByName(const char *name);
ValueProvider3D *getscriptTable(int index);
