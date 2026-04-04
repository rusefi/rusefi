#include "pch.h"

#include "airmass.h"
#include "idle_thread.h"

AirmassVeModelBase::AirmassVeModelBase(const ValueProvider3D& veTable) : m_veTable(&veTable) {}

float getVeLoadAxis(ve_override_e mode, float passedLoad) {
	switch(mode) {
		case VE_None: return passedLoad;
		case VE_MAP: return Sensor::getOrZero(SensorType::Map);
		case VE_TPS: return Sensor::getOrZero(SensorType::Tps1);
		default: return 0;
	}
}

float AirmassVeModelBase::getVe(float rpm, float load, bool postState) const {
	efiAssert(ObdCode::OBD_PCM_Processor_Fault, m_veTable != nullptr, "VE table null", 0);

	// Override the load value if necessary
	load = getVeLoadAxis(engineConfiguration->veOverrideMode, load);

	percent_t ve = m_veTable->getValue(rpm, load);

#if EFI_PROD_CODE || EFI_UNIT_TEST
	bool switchTableActive = false;

	if (engineConfiguration->enableVeSwitchTable) {
		const bool pinActive = isBrainPinValid(config->veSwitchTableInput) &&
			efiReadPin(config->veSwitchTableInput, config->veSwitchTableInputMode);

		if (pinActive) {
			// Hard switch: pin overrides everything, replace VE entirely
			ve = interpolate3d(
				config->veSwitchTable,
				config->veSwitchLoadBins, load,
				config->veSwitchRpmBins, rpm
			);
			switchTableActive = true;
		} else {
			auto result = calculateBlend(
				config->veSwitchBlendParameter,
				config->veSwitchBlendBins, config->veSwitchBlendValues,
				config->veSwitchTable,
				config->veSwitchLoadBins, load,
				config->veSwitchRpmBins, rpm
			);
			if (result.Bias > 0) {
				ve = interpolateClamped(0, ve, 100, result.Value, result.Bias);
				switchTableActive = true;
			}
			if (postState) {
				engine->outputChannels.veSwitchBlendParameter = result.BlendParameter;
				engine->outputChannels.veSwitchBlendBias = result.Bias;
			}
		}
	}
#endif

	float idleVeLoad = load;

#if EFI_IDLE_CONTROL
	auto tps = Sensor::get(SensorType::Tps1);
	// get VE from the separate table for Idle if idling
	if (engine->module<IdleController>()->isIdlingOrTaper() &&
		tps && engineConfiguration->useSeparateVeForIdle) {
		idleVeLoad = getVeLoadAxis(engineConfiguration->idleVeOverrideMode, load);

		percent_t idleVe = interpolate3d(
			config->idleVeTable,
			config->idleVeLoadBins, idleVeLoad,
			config->idleVeRpmBins, rpm
		);

		// interpolate between idle table and normal (running) table using TPS threshold
		// 0 TPS -> idle table
		// 1/2 threshold -> idle table
		// idle threshold -> normal table
		float idleThreshold = engineConfiguration->idlePidDeactivationTpsThreshold;
		ve = interpolateClamped(idleThreshold / 2, idleVe, idleThreshold, ve, tps.Value);
	}
#endif // EFI_IDLE_CONTROL

	// Add any adjustments if configured
	for (size_t i = 0; i < efi::size(config->veBlends); i++) {
		auto result = calculateBlend(config->veBlends[i], rpm, load);

		if (postState) {
			engine->outputChannels.veBlendParameter[i] = result.BlendParameter;
			engine->outputChannels.veBlendBias[i] = result.Bias;
			engine->outputChannels.veBlendOutput[i] = result.Value;
			engine->outputChannels.veBlendYAxis[i] = result.TableYAxis;
		}

		// Skip extra floating point math if we can...
		if (result.Value == 0) {
			continue;
		}

		// Apply as a multiplier, not as an adder
		// Value of +5 means add 5%, aka multiply by 1.05
		ve *= ((100 + result.Value) * 0.01f);
	}

	if (postState) {
		engine->engineState.currentVe = ve;
		engine->engineState.veTableYAxis = load;
		engine->engineState.veTableIdleYAxis = idleVeLoad;
#if EFI_PROD_CODE
		engine->engineState.isVeSwitchTableActive = switchTableActive;
#endif
	}

	return ve * PERCENT_DIV;
}
