#include "pch.h"

#include "basic_configuration.h"
#include "board_overrides.h"
#include "defaults.h"
#include "flash_main.h"
#include "tunerstudio.h"

std::optional<board_basic_configuration_type> custom_board_applyBasicConfiguration;

template <typename T, size_t N>
static void clearPinArray(T (&pins)[N], T noPin) {
	for (auto& pin : pins) {
		pin = noPin;
	}
}

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

static void unassignOutputs() {
	// [tag:unassign_outputs] This opt-in preset clears assignments only; the UI must
	// disclose the exclusions below and require a completed save/power-cycle before pin reuse.
	// Keep this inventory explicit: these are user-configurable actuator, debug,
	// and DAC outputs. Sensor/trigger inputs and board infrastructure are not outputs
	// for this action; CAN, SD/SPI, main relay, and fixed board wiring are preserved.
	const auto noOutput = static_cast<output_pin_e>(0);
	engineConfiguration->idle.solenoidPin = noOutput;
	engineConfiguration->idle.stepperDirectionPin = Gpio::Unassigned;
	engineConfiguration->idle.stepperStepPin = Gpio::Unassigned;
	engineConfiguration->stepperEnablePin = Gpio::Unassigned;
	engineConfiguration->secondSolenoidPin = noOutput;
	clearPinArray(engineConfiguration->stepper_raw_output, noOutput);
	for (size_t i = 0; i < ETB_COUNT; i++) {
		engineConfiguration->etbIo[i].directionPin1 = Gpio::Unassigned;
		engineConfiguration->etbIo[i].directionPin2 = Gpio::Unassigned;
		engineConfiguration->etbIo[i].controlPin = Gpio::Unassigned;
		engineConfiguration->etbIo[i].disablePin = Gpio::Unassigned;
	}
	for (size_t i = 0; i < DC_PER_STEPPER; i++) {
		engineConfiguration->stepperDcIo[i].directionPin1 = Gpio::Unassigned;
		engineConfiguration->stepperDcIo[i].directionPin2 = Gpio::Unassigned;
		engineConfiguration->stepperDcIo[i].controlPin = Gpio::Unassigned;
		engineConfiguration->stepperDcIo[i].disablePin = Gpio::Unassigned;
	}
	clearPinArray(engineConfiguration->injectionPins, noOutput);
	clearPinArray(engineConfiguration->injectionPinsStage2, noOutput);
	clearPinArray(engineConfiguration->ignitionPins, noOutput);
	clearPinArray(engineConfiguration->trailingCoilPins, noOutput);
	engineConfiguration->fuelPumpPin = noOutput;
	engineConfiguration->fuelPump2Pin = noOutput;
	engineConfiguration->starterControlPin = noOutput;
	engineConfiguration->starterRelayDisablePin = noOutput;
	engineConfiguration->acRelayPin = noOutput;
	engineConfiguration->alternatorControlPin = noOutput;
	engineConfiguration->malfunctionIndicatorPin = noOutput;
	engineConfiguration->tachOutputPin = noOutput;
	engineConfiguration->speedometerOutputPin = noOutput;
	engineConfiguration->o2heaterPin = noOutput;
	engineConfiguration->hpfpValvePin = noOutput;
	engineConfiguration->acrPin = noOutput;
	engineConfiguration->acrPin2 = noOutput;
	engineConfiguration->nitrousRelayPin = noOutput;
	engineConfiguration->vvlRelayPin = noOutput;
	clearPinArray(engineConfiguration->vvtPins, noOutput);
	clearPinArray(engineConfiguration->tcu_solenoid, noOutput);
	engineConfiguration->tcu_tcc_onoff_solenoid = noOutput;
	engineConfiguration->tcu_tcc_pwm_solenoid = noOutput;
	engineConfiguration->tcu_pc_solenoid_pin = noOutput;
	engineConfiguration->tcu_32_solenoid_pin = noOutput;
	clearPinArray(engineConfiguration->luaOutputPins, noOutput);
	clearPinArray(engineConfiguration->auxValves, Gpio::Unassigned);
	for (auto& pwm : engineConfiguration->gppwm) {
		pwm.pin = noOutput;
	}
	engineConfiguration->boostControlPin = noOutput;
	engineConfiguration->fanPin = noOutput;
	engineConfiguration->fan2Pin = noOutput;
	engineConfiguration->debugTriggerSync = Gpio::Unassigned;
	engineConfiguration->debugTriggerState = Gpio::Unassigned;
	engineConfiguration->triggerErrorPin = Gpio::Unassigned;
	clearPinArray(engineConfiguration->triggerSimulatorPins, Gpio::Unassigned);
	engineConfiguration->camSimulatorPin = Gpio::Unassigned;
	clearPinArray(engineConfiguration->dacOutputPins, Gpio::Unassigned);
}

bool handleBasicConfigurationAction(uint16_t index) {
	if (index > static_cast<uint16_t>(BasicConfigurationAction::UnassignOutputs)) {
		return false;
	}

	if (index == static_cast<uint16_t>(BasicConfigurationAction::UnassignOutputs)) {
		// Recognized-but-refused actions must not fall through to legacy board hooks.
		if (!engine->rpmCalculator.isStopped() || isLockedFromUser()) {
			return true;
		}

		unassignOutputs();
		// [tag:unassign_outputs] Refresh suppresses an immediate TS Burn, so this
		// action queues its own normal save without changing active hardware ownership.
#if EFI_CONFIGURATION_STORAGE || EFI_UNIT_TEST
		setNeedToWriteConfiguration();
#endif
#if EFI_TUNER_STUDIO
		onApplyPreset();
	#endif
		return true;
	}

	if (!custom_board_applyBasicConfiguration.has_value()) {
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
