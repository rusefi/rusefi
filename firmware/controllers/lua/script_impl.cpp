/**
 * @file script_impl.cpp
 *
 * @date Oct 5, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "script_impl.h"

static script1_Map3D_f32t scriptTable1{"script1"};
static script2_Map3D_f32t scriptTable2{"script2"};
static script3_Map3D_u8t scriptTable3{"script3"};
static script4_Map3D_u8t scriptTable4{"script4"};

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
expected<int> getCurveIndexByName(const char *name) {
	for (int i = 0;i<SCRIPT_CURVE_COUNT;i++) {
		if (strEqualCaseInsensitive(name, engineConfiguration->scriptCurveName[i])) {
			return i;
		}
	}

	return unexpected;
}

expected<int> getTableIndexByName(const char *name) {
	for (int i = 0;i<SCRIPT_TABLE_COUNT;i++) {
		if (strEqualCaseInsensitive(name, engineConfiguration->scriptTableName[i])) {
			return i;
		}
	}

	return unexpected;
}

expected<int> getSettingIndexByName(const char *name) {
	for (int i = 0;i<SCRIPT_SETTING_COUNT;i++) {
		if (strEqualCaseInsensitive(name, engineConfiguration->scriptSettingName[i])) {
			return i;
		}
	}

	return unexpected;
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
	scriptTable1.initTable(config->scriptTable1, config->scriptTable1RpmBins, config->scriptTable1LoadBins);
	scriptTable2.initTable(config->scriptTable2,	config->scriptTable2RpmBins, config->scriptTable2LoadBins);
	scriptTable3.initTable(config->scriptTable3,	config->scriptTable3RpmBins, config->scriptTable3LoadBins);
	scriptTable4.initTable(config->scriptTable4,	config->scriptTable4RpmBins, config->scriptTable4LoadBins);
}
