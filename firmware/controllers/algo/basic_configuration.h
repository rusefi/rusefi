#pragma once

#include "rusefi_types.h"

enum class BasicConfigurationAction : uint16_t {
	EtbToStepper = 0,
	StepperToEtb = 1,
	CableTps = 2,
	PpsTps = 3,
	// [tag:unassign_outputs] Wire index must match cmd_unassign_outputs in the INI template.
	UnassignOutputs = 4,
};

bool applyBasicConfiguration(BasicConfigurationAction action,
	void (*applyEtbStepperPinDefaults)(),
	adc_channel_e tpsPrimary,
	adc_channel_e tpsSecondary,
	adc_channel_e ppsPrimary,
	adc_channel_e ppsSecondary);
// Returns false so non-Basic TS_BOARD_ACTION indices can use the legacy board handler.
bool handleBasicConfigurationAction(uint16_t index);
