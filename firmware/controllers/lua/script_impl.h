/**
 * @file script_impl.h
 *
 * @date Oct 5, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include <rusefi/expected.h>

void initScriptImpl();

float getCurveValue(int index, float key);
expected<int> getCurveIndexByName(const char *name);
expected<int> getTableIndexByName(const char *name);
expected<int> getSettingIndexByName(const char *name);
ValueProvider3D *getscriptTable(int index);
