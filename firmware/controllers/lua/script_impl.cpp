/**
 * @file fsio_impl.cpp
 * @brief FSIO as it's used for GPIO
 *
 * TODO: rename this file, FSIO is gone!
 *
 * @date Oct 5, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "script_impl.h"

static fsio8_Map3D_f32t scriptTable1;
static fsio8_Map3D_u8t scriptTable2;
static fsio8_Map3D_u8t scriptTable3;
static fsio8_Map3D_u8t scriptTable4;

ValueProvider3D *getscriptTable(int index) {
	switch (index) {
	default:
		return &scriptTable1;
	case 1:
		return &scriptTable2;
	case 2:
		return &scriptTable3;
	case 3:
		return &scriptTable4;
	}
}

// todo: template this copy-pasta
/**
 * @return zero-based index of curve with given name
 */
int getCurveIndexByName(const char *name) {
	for (int i = 0;i<SCRIPT_CURVE_COUNT;i++) {
		if (strEqualCaseInsensitive(name, engineConfiguration->scriptCurveName[i])) {
			return i;
		}
	}
	return EFI_ERROR_CODE;
}

int getTableIndexByName(const char *name) {
	for (int i = 0;i<SCRIPT_TABLE_COUNT;i++) {
		if (strEqualCaseInsensitive(name, engineConfiguration->scriptTableName[i])) {
			return i;
		}
	}
	return EFI_ERROR_CODE;
}

int getSettingIndexByName(const char *name) {
	for (int i = 0;i<SCRIPT_SETTING_COUNT;i++) {
		if (strEqualCaseInsensitive(name, engineConfiguration->scriptSettingName[i])) {
			return i;
		}
	}
	return EFI_ERROR_CODE;
}

float getCurveValue(int index, float key) {
	// not great code at all :(
	switch (index) {
	default:
		return interpolate2d(key, config->scriptCurve1Bins, config->scriptCurve1);
	case 1:
		return interpolate2d(key, config->scriptCurve2Bins, config->scriptCurve2);
	case 2:
		return interpolate2d(key, config->scriptCurve3Bins, config->scriptCurve3);
	case 3:
		return interpolate2d(key, config->scriptCurve4Bins, config->scriptCurve4);
	case 4:
		return interpolate2d(key, config->scriptCurve5Bins, config->scriptCurve5);
	case 5:
		return interpolate2d(key, config->scriptCurve6Bins, config->scriptCurve6);
	}
}

void initScriptImpl() {
	scriptTable1.init(config->scriptTable1, config->scriptTable1LoadBins,
			config->scriptTable1RpmBins);
	scriptTable2.init(config->scriptTable2, config->scriptTable2LoadBins,
			config->scriptTable2RpmBins);
	scriptTable3.init(config->scriptTable3, config->scriptTable3LoadBins,
			config->scriptTable3RpmBins);
	scriptTable4.init(config->scriptTable4, config->scriptTable4LoadBins,
			config->scriptTable4RpmBins);
}
