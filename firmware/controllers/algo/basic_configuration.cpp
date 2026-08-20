#include "pch.h"

#include "basic_configuration.h"
#include "board_overrides.h"
#include "defaults.h"
#include "tunerstudio.h"

std::optional<board_basic_configuration_type> custom_board_applyBasicConfiguration;

static void applyActuatorDefaults(bool useStepper, void (*applyEtbStepperPinDefaults)()) {
	engineConfiguration->useStepperIdle = useStepper;
	engineConfiguration->useHbridgesToDriveIdleStepper = useStepper;
	engineConfiguration->useRawOutputToDriveIdleStepper = false;
	engineConfiguration->stepper_dc_use_two_wires = false;
	engineConfiguration->stepperDcInvertedPins = false;

	engineConfiguration->etbFunctions[0] = useStepper ? DC_None : DC_Throttle1;
	engineConfiguration->etbFunctions[1] = useStepper ? DC_None : DC_Throttle2;

	applyEtbStepperPinDefaults();
}

bool applyBasicConfiguration(BasicConfigurationAction action,
	void (*applyEtbStepperPinDefaults)(),
	adc_channel_e tpsPrimary,
	adc_channel_e tpsSecondary,
	adc_channel_e ppsPrimary,
	adc_channel_e ppsSecondary) {
	switch (action) {
	case BasicConfigurationAction::EtbToStepper:
		applyActuatorDefaults(true, applyEtbStepperPinDefaults);
		break;
	case BasicConfigurationAction::StepperToEtb:
		applyActuatorDefaults(false, applyEtbStepperPinDefaults);
		break;
	case BasicConfigurationAction::CableTps:
		setTPS1Inputs(tpsPrimary, EFI_ADC_NONE);
		setPPSInputs(EFI_ADC_NONE, EFI_ADC_NONE);
		break;
	case BasicConfigurationAction::PpsTps:
		setTPS1Inputs(tpsPrimary, tpsSecondary);
		setPPSInputs(ppsPrimary, ppsSecondary);
		break;
	default:
		return false;
	}

	return true;
}

bool handleBasicConfigurationAction(uint16_t index) {
	if (index > static_cast<uint16_t>(BasicConfigurationAction::PpsTps) ||
		!custom_board_applyBasicConfiguration.has_value()) {
		return false;
	}

	if (!engine->rpmCalculator.isStopped()) {
		return true;
	}

	if (!get_board_override_result(custom_board_applyBasicConfiguration, false,
		static_cast<BasicConfigurationAction>(index))) {
		return false;
	}

#if EFI_TUNER_STUDIO
	onApplyPreset();
#endif

	return true;
}
