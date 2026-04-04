/**
 * @file	engine_math.h
 *
 * @date Jul 13, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "rusefi_enums.h"
#include "gppwm_channel_reader.h"

void setAlgorithm(engine_load_mode_e algo);

void setFlatInjectorLag(float value);

/**
 * @return time needed to rotate crankshaft by one degree, in milliseconds.
 * @deprecated use at least Us, maybe even Nt
 */
#define getOneDegreeTimeMs(rpm) (1000.0f * 60 / 360 / (rpm))

/**
 * @return float, time needed to rotate crankshaft by one degree, in microseconds.
 * See also engine->rpmCalculator.oneDegreeUs
 */
#define getOneDegreeTimeUs(rpm) (1000000.0f * 60 / 360 / (rpm))

floatms_t getCrankshaftRevolutionTimeMs(float rpm);
floatms_t getEngineCycleDuration(float rpm);

float getFuelingLoad();
float getIgnitionLoad();

ignition_mode_e getCurrentIgnitionMode();

void setTimingRpmBin(float from, float to);

void setSingleCoilDwell();

// we combine trigger-defined triggerShape.tdcPosition with user-defined engineConfiguration->globalTriggerAngleOffset
// expectation is that for well-known triggers engineConfiguration->globalTriggerAngleOffset would usually be zero
// while for toothed wheels user would have to provide a value
// If camSyncOnSecondCrankRevolution is active, 360° are added
#define tdcPosition() \
		(getTriggerCentral()->triggerShape.tdcPosition \
   			+ ( engineConfiguration->camSyncOnSecondCrankRevolution ? engineConfiguration->globalTriggerAngleOffset + 360 : engineConfiguration->globalTriggerAngleOffset ))

/** Gets phase offset for a particular cylinder's ID and number
 * For example on 4 cylinder engine with firing order 1-3-4-2, this
 * returns 0-180-360-540 for index 0-1-2-3
 * Cylinder number is used for per-cylinder adjustment, if you have
 * an odd-fire engine (v-twin, V10, some v6, etc)
 */
angle_t getPerCylinderFiringOrderOffset(uint8_t cylinderIndex, uint8_t cylinderNumber);

// Table blending helpers
struct BlendResult {
	// Input blend parameter (lookup to bias table)
	float BlendParameter;

	// Bias in percent (0-100%)
	float Bias;

	// Result value (bias * table value)
	float Value;

	// Y axis value used for lookup (to show the dot correctly in TunerStudio)
	float TableYAxis;
};

BlendResult calculateBlend(blend_table_s& cfg, float rpm, float load);

// Overload for tables with flat (non-struct) blend config, e.g. veSwitchTable.
// Unlike the blend_table_s overload, BlendResult::Value is the raw table lookup
// (not bias-scaled)
template<typename TBlendBins, typename TBlendValues, typename TTable, typename TLoadBins, typename TRpmBins>
BlendResult calculateBlend(
    gppwm_channel_e blendParameter,
    const TBlendBins& blendBins,
    const TBlendValues& blendValues,
    const TTable& table,
    const TLoadBins& loadBins, float load,
    const TRpmBins& rpmBins, float rpm)
{
    if (blendParameter == GPPWM_Zero) {
        return { 0, 0, 0, 0 };
    }

    auto value = readGppwmChannel(blendParameter);
    if (!value) {
        return { 0, 0, 0, 0 };
    }

    float tableValue = interpolate3d(table, loadBins, load, rpmBins, rpm);
    float blendFactor = interpolate2d(value.Value, blendBins, blendValues);

    return { value.Value, blendFactor, tableValue, load };
}
