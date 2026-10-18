#include "pch.h"

#include "basic_configuration.h"
#include "flash_main.h"
#include "board_overrides.h"
#include "defaults.h"
#include "electronic_throttle_impl.h"
#include "engine_types.h"
#include "idle_hardware.h"
#include "tunerstudio.h"

extern engine_configuration_s& activeConfiguration;

TEST(MafConfiguration, InvalidCurveWithoutAnalogInputs) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->mafAdcChannel = EFI_ADC_NONE;
	engineConfiguration->maf2AdcChannel = EFI_ADC_NONE;
	setArrayValues(config->mafDecodingBins, 0.0f);

	// An unused MAF curve must not prevent startup or be rewritten by validation.
	EXPECT_TRUE(validateConfigOnStartUpOrBurn());
	for (auto bin : config->mafDecodingBins) {
		EXPECT_FLOAT_EQ(0.0f, bin);
	}
}

TEST(MafConfiguration, InvalidCurveWithPrimaryInput) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->mafAdcChannel = EFI_ADC_0;
	engineConfiguration->maf2AdcChannel = EFI_ADC_NONE;
	setArrayValues(config->mafDecodingBins, 0.0f);

	EXPECT_FATAL_ERROR(validateConfigOnStartUpOrBurn());
}

TEST(MafConfiguration, InvalidCurveWithSecondaryInput) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->mafAdcChannel = EFI_ADC_NONE;
	engineConfiguration->maf2AdcChannel = EFI_ADC_0;
	setArrayValues(config->mafDecodingBins, 0.0f);

	EXPECT_FATAL_ERROR(validateConfigOnStartUpOrBurn());
}

TEST(MafConfiguration, ValidCurveWithBothInputs) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->mafAdcChannel = EFI_ADC_0;
	engineConfiguration->maf2AdcChannel = EFI_ADC_1;

	EXPECT_TRUE(validateConfigOnStartUpOrBurn());
}

static int pinDefaultsApplyCount;

static void applyTestPinDefaults() {
	pinDefaultsApplyCount++;
}

static bool applyTestBasicConfiguration(BasicConfigurationAction action) {
	return applyBasicConfiguration(action,
		applyTestPinDefaults,
		EFI_ADC_1,
		EFI_ADC_2,
		EFI_ADC_3,
		EFI_ADC_4);
}

class BasicConfigurationTest : public testing::Test {
protected:
	BasicConfigurationTest()
		: eth(engine_type_e::TEST_ENGINE) {
		pinDefaultsApplyCount = 0;
		custom_board_applyBasicConfiguration = applyTestBasicConfiguration;
	}

	~BasicConfigurationTest() override {
		custom_board_applyBasicConfiguration.reset();
	}

	void run(BasicConfigurationAction action) {
		ASSERT_TRUE(handleBasicConfigurationAction(static_cast<uint16_t>(action)));
	}

	EngineTestHelper eth;
};

TEST_F(BasicConfigurationTest, ActuatorPresetsDoNotChangeSensors) {
	setTPS1Inputs(EFI_ADC_5, EFI_ADC_6);
	setPPSInputs(EFI_ADC_7, EFI_ADC_8);
	engineConfiguration->useRawOutputToDriveIdleStepper = true;
	engineConfiguration->stepper_dc_use_two_wires = true;
	engineConfiguration->stepperDcInvertedPins = true;
	engine->engineTypeChangeTimer.init();

	run(BasicConfigurationAction::EtbToStepper);

	EXPECT_TRUE(engineConfiguration->useStepperIdle);
	EXPECT_TRUE(engineConfiguration->useHbridgesToDriveIdleStepper);
	EXPECT_FALSE(engineConfiguration->useRawOutputToDriveIdleStepper);
	EXPECT_FALSE(engineConfiguration->stepper_dc_use_two_wires);
	EXPECT_FALSE(engineConfiguration->stepperDcInvertedPins);
	EXPECT_EQ(DC_None, engineConfiguration->etbFunctions[0]);
	EXPECT_EQ(DC_None, engineConfiguration->etbFunctions[1]);
	EXPECT_EQ(EFI_ADC_5, engineConfiguration->tps1_1AdcChannel);
	EXPECT_EQ(EFI_ADC_6, engineConfiguration->tps1_2AdcChannel);
	EXPECT_EQ(EFI_ADC_7, engineConfiguration->throttlePedalPositionAdcChannel);
	EXPECT_EQ(EFI_ADC_8, engineConfiguration->throttlePedalPositionSecondAdcChannel);
	EXPECT_EQ(1, pinDefaultsApplyCount);
	EXPECT_TRUE(needToTriggerTsRefresh());
	EXPECT_FALSE(isIdleHardwareRestartNeeded());

	auto* etb = static_cast<EtbController*>(engine->etbControllers[0]);
	const auto runningFunction = etb->getFunction();
	activeConfiguration.etbFunctions[0] = DC_None;
	activeConfiguration.etbFunctions[1] = DC_None;

	run(BasicConfigurationAction::StepperToEtb);
	onConfigurationChangeElectronicThrottleCallback(&activeConfiguration);

	EXPECT_FALSE(engineConfiguration->useStepperIdle);
	EXPECT_FALSE(engineConfiguration->useHbridgesToDriveIdleStepper);
	EXPECT_FALSE(engineConfiguration->useRawOutputToDriveIdleStepper);
	EXPECT_EQ(DC_Throttle1, engineConfiguration->etbFunctions[0]);
	EXPECT_EQ(DC_Throttle2, engineConfiguration->etbFunctions[1]);
	EXPECT_EQ(2, pinDefaultsApplyCount);
	EXPECT_FALSE(isIdleHardwareRestartNeeded());
	EXPECT_EQ(runningFunction, etb->getFunction());
}

TEST_F(BasicConfigurationTest, SensorPresetsPreserveCalibration) {
	engineConfiguration->tpsMin = 101;
	engineConfiguration->tpsMax = 202;
	engineConfiguration->tps1SecondaryMin = 303;
	engineConfiguration->tps1SecondaryMax = 404;
	engineConfiguration->throttlePedalUpVoltage = 0.5f;
	engineConfiguration->throttlePedalWOTVoltage = 4.5f;
	engineConfiguration->throttlePedalSecondaryUpVoltage = 4.4f;
	engineConfiguration->throttlePedalSecondaryWOTVoltage = 0.6f;

	run(BasicConfigurationAction::CableTps);

	EXPECT_EQ(EFI_ADC_1, engineConfiguration->tps1_1AdcChannel);
	EXPECT_EQ(EFI_ADC_NONE, engineConfiguration->tps1_2AdcChannel);
	EXPECT_EQ(EFI_ADC_NONE, engineConfiguration->throttlePedalPositionAdcChannel);
	EXPECT_EQ(EFI_ADC_NONE, engineConfiguration->throttlePedalPositionSecondAdcChannel);
	EXPECT_EQ(0, pinDefaultsApplyCount);

	run(BasicConfigurationAction::PpsTps);

	EXPECT_EQ(EFI_ADC_1, engineConfiguration->tps1_1AdcChannel);
	EXPECT_EQ(EFI_ADC_2, engineConfiguration->tps1_2AdcChannel);
	EXPECT_EQ(EFI_ADC_3, engineConfiguration->throttlePedalPositionAdcChannel);
	EXPECT_EQ(EFI_ADC_4, engineConfiguration->throttlePedalPositionSecondAdcChannel);
	EXPECT_EQ(101, engineConfiguration->tpsMin);
	EXPECT_EQ(202, engineConfiguration->tpsMax);
	EXPECT_EQ(303, engineConfiguration->tps1SecondaryMin);
	EXPECT_EQ(404, engineConfiguration->tps1SecondaryMax);
	EXPECT_FLOAT_EQ(0.5f, engineConfiguration->throttlePedalUpVoltage);
	EXPECT_FLOAT_EQ(4.5f, engineConfiguration->throttlePedalWOTVoltage);
	EXPECT_FLOAT_EQ(4.4f, engineConfiguration->throttlePedalSecondaryUpVoltage);
	EXPECT_FLOAT_EQ(0.6f, engineConfiguration->throttlePedalSecondaryWOTVoltage);
	EXPECT_EQ(0, pinDefaultsApplyCount);
}

TEST_F(BasicConfigurationTest, RejectsRunningEngineAndUnknownAction) {
	engineConfiguration->useStepperIdle = false;
	engine->rpmCalculator.setRpmValue(1000);
	engine->engineTypeChangeTimer.init();

	run(BasicConfigurationAction::EtbToStepper);

	EXPECT_FALSE(engineConfiguration->useStepperIdle);
	EXPECT_EQ(0, pinDefaultsApplyCount);
	EXPECT_FALSE(needToTriggerTsRefresh());

	engine->rpmCalculator.setRpmValue(0);
	EXPECT_FALSE(handleBasicConfigurationAction(0xffff));

	EXPECT_FALSE(engineConfiguration->useStepperIdle);
	EXPECT_EQ(0, pinDefaultsApplyCount);
	EXPECT_FALSE(needToTriggerTsRefresh());

	custom_board_applyBasicConfiguration.reset();
	EXPECT_FALSE(handleBasicConfigurationAction(static_cast<uint16_t>(BasicConfigurationAction::EtbToStepper)));
}

TEST_F(BasicConfigurationTest, UnassignOutputsClearsActuatorDebugAndDacPinsOnly) {
	// [tag:unassign_outputs] Cover the explicit clearing inventory and preserved infrastructure.
	const auto assignedGpio = Gpio::A0;
	const auto assignedOutput = static_cast<output_pin_e>(Gpio::A0);
	for (auto& pin : engineConfiguration->injectionPins) {
		pin = assignedOutput;
	}
	for (auto& pin : engineConfiguration->injectionPinsStage2) {
		pin = assignedOutput;
	}
	for (auto& pin : engineConfiguration->ignitionPins) {
		pin = assignedOutput;
	}
	for (auto& pin : engineConfiguration->trailingCoilPins) {
		pin = assignedOutput;
	}
	for (auto& pin : engineConfiguration->stepper_raw_output) {
		pin = assignedOutput;
	}
	for (auto& pin : engineConfiguration->vvtPins) {
		pin = assignedOutput;
	}
	for (auto& pin : engineConfiguration->tcu_solenoid) {
		pin = assignedOutput;
	}
	for (auto& pin : engineConfiguration->luaOutputPins) {
		pin = assignedOutput;
	}
	for (auto& pin : engineConfiguration->auxValves) {
		pin = assignedGpio;
	}
	const auto lastAuxValve = sizeof(engineConfiguration->auxValves) / sizeof(engineConfiguration->auxValves[0]) - 1;
	engineConfiguration->auxValves[lastAuxValve] = Gpio::TLE6240_PIN_1;
	for (auto& pin : engineConfiguration->triggerSimulatorPins) {
		pin = assignedGpio;
	}
	for (auto& pin : engineConfiguration->dacOutputPins) {
		pin = assignedGpio;
	}
	for (size_t i = 0; i < ETB_COUNT; i++) {
		engineConfiguration->etbIo[i].directionPin1 = assignedGpio;
		engineConfiguration->etbIo[i].directionPin2 = assignedGpio;
		engineConfiguration->etbIo[i].controlPin = assignedGpio;
		engineConfiguration->etbIo[i].disablePin = assignedGpio;
	}
	for (size_t i = 0; i < DC_PER_STEPPER; i++) {
		engineConfiguration->stepperDcIo[i].directionPin1 = assignedGpio;
		engineConfiguration->stepperDcIo[i].directionPin2 = assignedGpio;
		engineConfiguration->stepperDcIo[i].controlPin = assignedGpio;
		engineConfiguration->stepperDcIo[i].disablePin = assignedGpio;
	}
	engineConfiguration->stepperEnablePin = assignedGpio;
	engineConfiguration->idle.solenoidPin = assignedOutput;
	engineConfiguration->idle.stepperDirectionPin = assignedGpio;
	engineConfiguration->idle.stepperStepPin = assignedGpio;
	engineConfiguration->secondSolenoidPin = assignedOutput;
	engineConfiguration->fuelPumpPin = assignedOutput;
	engineConfiguration->fuelPump2Pin = assignedOutput;
	engineConfiguration->starterControlPin = assignedOutput;
	engineConfiguration->starterRelayDisablePin = assignedOutput;
	engineConfiguration->acRelayPin = assignedOutput;
	engineConfiguration->alternatorControlPin = assignedOutput;
	engineConfiguration->malfunctionIndicatorPin = assignedOutput;
	engineConfiguration->tachOutputPin = assignedOutput;
	engineConfiguration->speedometerOutputPin = assignedOutput;
	engineConfiguration->o2heaterPin = assignedOutput;
	engineConfiguration->hpfpValvePin = assignedOutput;
	engineConfiguration->acrPin = assignedOutput;
	engineConfiguration->acrPin2 = assignedOutput;
	engineConfiguration->nitrousRelayPin = assignedOutput;
	engineConfiguration->vvlRelayPin = assignedOutput;
	engineConfiguration->tcu_tcc_onoff_solenoid = assignedOutput;
	engineConfiguration->tcu_tcc_pwm_solenoid = assignedOutput;
	engineConfiguration->tcu_pc_solenoid_pin = assignedOutput;
	engineConfiguration->tcu_32_solenoid_pin = assignedOutput;
	for (auto& pwm : engineConfiguration->gppwm) {
		pwm.pin = assignedOutput;
	}
	engineConfiguration->boostControlPin = assignedOutput;
	engineConfiguration->fanPin = assignedOutput;
	engineConfiguration->fan2Pin = assignedOutput;
	engineConfiguration->debugTriggerSync = assignedGpio;
	engineConfiguration->debugTriggerState = assignedGpio;
	engineConfiguration->triggerErrorPin = assignedGpio;
	engineConfiguration->camSimulatorPin = assignedGpio;
	engineConfiguration->mainRelayPin = assignedOutput;
	engineConfiguration->sdCardCsPin = assignedGpio;
	engineConfiguration->canTxPin = assignedGpio;
	engineConfiguration->canRxPin = assignedGpio;
	engineConfiguration->sdCardSpiDevice = static_cast<spi_device_e>(1);
	engineConfiguration->is_enabled_spi_1 = true;
	engineConfiguration->tps1_1AdcChannel = EFI_ADC_5;
	engineConfiguration->tps1SecondaryMin = 303;
	engineConfiguration->injectionMode = IM_BATCH;
	engineConfiguration->triggerInputPins[0] = static_cast<brain_input_pin_e>(Gpio::A0);
	engineConfiguration->spi1mosiPin = assignedGpio;
	engine->engineTypeChangeTimer.init();

	EXPECT_TRUE(handleBasicConfigurationAction(static_cast<uint16_t>(BasicConfigurationAction::UnassignOutputs)));

	for (auto pin : engineConfiguration->injectionPins) {
		EXPECT_EQ(static_cast<output_pin_e>(0), pin);
	}
	for (auto pin : engineConfiguration->injectionPinsStage2) {
		EXPECT_EQ(static_cast<output_pin_e>(0), pin);
	}
	for (auto pin : engineConfiguration->ignitionPins) {
		EXPECT_EQ(static_cast<output_pin_e>(0), pin);
	}
	for (auto pin : engineConfiguration->trailingCoilPins) {
		EXPECT_EQ(static_cast<output_pin_e>(0), pin);
	}
	for (auto pin : engineConfiguration->stepper_raw_output) {
		EXPECT_EQ(static_cast<output_pin_e>(0), pin);
	}
	for (auto pin : engineConfiguration->vvtPins) {
		EXPECT_EQ(static_cast<output_pin_e>(0), pin);
	}
	for (auto pin : engineConfiguration->tcu_solenoid) {
		EXPECT_EQ(static_cast<output_pin_e>(0), pin);
	}
	for (auto pin : engineConfiguration->luaOutputPins) {
		EXPECT_EQ(static_cast<output_pin_e>(0), pin);
	}
	for (auto pin : engineConfiguration->auxValves) {
		EXPECT_EQ(Gpio::Unassigned, pin);
	}
	for (auto pin : engineConfiguration->triggerSimulatorPins) {
		EXPECT_EQ(Gpio::Unassigned, pin);
	}
	for (auto pin : engineConfiguration->dacOutputPins) {
		EXPECT_EQ(Gpio::Unassigned, pin);
	}
	for (size_t i = 0; i < ETB_COUNT; i++) {
		EXPECT_EQ(Gpio::Unassigned, engineConfiguration->etbIo[i].directionPin1);
		EXPECT_EQ(Gpio::Unassigned, engineConfiguration->etbIo[i].directionPin2);
		EXPECT_EQ(Gpio::Unassigned, engineConfiguration->etbIo[i].controlPin);
		EXPECT_EQ(Gpio::Unassigned, engineConfiguration->etbIo[i].disablePin);
	}
	for (size_t i = 0; i < DC_PER_STEPPER; i++) {
		EXPECT_EQ(Gpio::Unassigned, engineConfiguration->stepperDcIo[i].directionPin1);
		EXPECT_EQ(Gpio::Unassigned, engineConfiguration->stepperDcIo[i].directionPin2);
		EXPECT_EQ(Gpio::Unassigned, engineConfiguration->stepperDcIo[i].controlPin);
		EXPECT_EQ(Gpio::Unassigned, engineConfiguration->stepperDcIo[i].disablePin);
	}
	EXPECT_EQ(Gpio::Unassigned, engineConfiguration->stepperEnablePin);
	EXPECT_EQ(static_cast<output_pin_e>(0), engineConfiguration->idle.solenoidPin);
	EXPECT_EQ(Gpio::Unassigned, engineConfiguration->idle.stepperDirectionPin);
	EXPECT_EQ(Gpio::Unassigned, engineConfiguration->idle.stepperStepPin);
	EXPECT_EQ(static_cast<output_pin_e>(0), engineConfiguration->secondSolenoidPin);
	EXPECT_EQ(static_cast<output_pin_e>(0), engineConfiguration->fuelPumpPin);
	EXPECT_EQ(Gpio::Unassigned, engineConfiguration->fuelPump2Pin);
	EXPECT_EQ(static_cast<output_pin_e>(0), engineConfiguration->starterControlPin);
	EXPECT_EQ(static_cast<output_pin_e>(0), engineConfiguration->starterRelayDisablePin);
	EXPECT_EQ(static_cast<output_pin_e>(0), engineConfiguration->acRelayPin);
	EXPECT_EQ(static_cast<output_pin_e>(0), engineConfiguration->alternatorControlPin);
	EXPECT_EQ(static_cast<output_pin_e>(0), engineConfiguration->malfunctionIndicatorPin);
	EXPECT_EQ(static_cast<output_pin_e>(0), engineConfiguration->tachOutputPin);
	EXPECT_EQ(static_cast<output_pin_e>(0), engineConfiguration->speedometerOutputPin);
	EXPECT_EQ(static_cast<output_pin_e>(0), engineConfiguration->o2heaterPin);
	EXPECT_EQ(static_cast<output_pin_e>(0), engineConfiguration->hpfpValvePin);
	EXPECT_EQ(static_cast<output_pin_e>(0), engineConfiguration->acrPin);
	EXPECT_EQ(static_cast<output_pin_e>(0), engineConfiguration->acrPin2);
	EXPECT_EQ(static_cast<output_pin_e>(0), engineConfiguration->nitrousRelayPin);
	EXPECT_EQ(static_cast<output_pin_e>(0), engineConfiguration->vvlRelayPin);
	EXPECT_EQ(static_cast<output_pin_e>(0), engineConfiguration->tcu_tcc_onoff_solenoid);
	EXPECT_EQ(static_cast<output_pin_e>(0), engineConfiguration->tcu_tcc_pwm_solenoid);
	EXPECT_EQ(static_cast<output_pin_e>(0), engineConfiguration->tcu_pc_solenoid_pin);
	EXPECT_EQ(static_cast<output_pin_e>(0), engineConfiguration->tcu_32_solenoid_pin);
	for (auto& pwm : engineConfiguration->gppwm) {
		EXPECT_EQ(static_cast<output_pin_e>(0), pwm.pin);
	}
	EXPECT_EQ(static_cast<output_pin_e>(0), engineConfiguration->boostControlPin);
	EXPECT_EQ(static_cast<output_pin_e>(0), engineConfiguration->fanPin);
	EXPECT_EQ(static_cast<output_pin_e>(0), engineConfiguration->fan2Pin);
	EXPECT_EQ(Gpio::Unassigned, engineConfiguration->debugTriggerSync);
	EXPECT_EQ(Gpio::Unassigned, engineConfiguration->debugTriggerState);
	EXPECT_EQ(Gpio::Unassigned, engineConfiguration->triggerErrorPin);
	EXPECT_EQ(Gpio::Unassigned, engineConfiguration->camSimulatorPin);
	EXPECT_EQ(assignedOutput, engineConfiguration->mainRelayPin);
	EXPECT_EQ(assignedGpio, engineConfiguration->sdCardCsPin);
	EXPECT_EQ(assignedGpio, engineConfiguration->canTxPin);
	EXPECT_EQ(assignedGpio, engineConfiguration->canRxPin);
	EXPECT_EQ(static_cast<spi_device_e>(1), engineConfiguration->sdCardSpiDevice);
	EXPECT_TRUE(engineConfiguration->is_enabled_spi_1);
	EXPECT_EQ(EFI_ADC_5, engineConfiguration->tps1_1AdcChannel);
	EXPECT_EQ(static_cast<brain_input_pin_e>(Gpio::A0), engineConfiguration->triggerInputPins[0]);
	EXPECT_EQ(303, engineConfiguration->tps1SecondaryMin);
	EXPECT_EQ(IM_BATCH, engineConfiguration->injectionMode);
	EXPECT_EQ(assignedGpio, engineConfiguration->spi1mosiPin);
	EXPECT_EQ(0, pinDefaultsApplyCount);
	EXPECT_TRUE(needToTriggerTsRefresh());
	EXPECT_TRUE(validateConfigOnStartUpOrBurn());

	EXPECT_TRUE(handleBasicConfigurationAction(static_cast<uint16_t>(BasicConfigurationAction::UnassignOutputs)));
	EXPECT_EQ(0, pinDefaultsApplyCount);
}

TEST_F(BasicConfigurationTest, UnassignOutputsDoesNotRequireBoardHookAndRefusesLockedOrRunningEngine) {
	resetConfigurationWriteRequestCountForUnitTest();
	custom_board_applyBasicConfiguration.reset();
	engineConfiguration->fuelPumpPin = static_cast<output_pin_e>(Gpio::A0);
	engine->engineTypeChangeTimer.init();
	EXPECT_TRUE(handleBasicConfigurationAction(static_cast<uint16_t>(BasicConfigurationAction::UnassignOutputs)));
	EXPECT_EQ(static_cast<output_pin_e>(0), engineConfiguration->fuelPumpPin);
	EXPECT_TRUE(needToTriggerTsRefresh());
	EXPECT_EQ(1, getConfigurationWriteRequestCountForUnitTest());

	resetConfigurationWriteRequestCountForUnitTest();
	engine->engineTypeChangeTimer.init();
	engineConfiguration->fuelPumpPin = static_cast<output_pin_e>(Gpio::A0);
	engineConfiguration->tuneHidingKey = 7;
	EXPECT_FATAL_ERROR(handleBasicConfigurationAction(static_cast<uint16_t>(BasicConfigurationAction::UnassignOutputs)));
	EXPECT_EQ(static_cast<output_pin_e>(Gpio::A0), engineConfiguration->fuelPumpPin);
	EXPECT_FALSE(needToTriggerTsRefresh());
	EXPECT_EQ(0, getConfigurationWriteRequestCountForUnitTest());

	engineConfiguration->tuneHidingKey = 0;
	engine->rpmCalculator.setRpmValue(1000);
	EXPECT_TRUE(handleBasicConfigurationAction(static_cast<uint16_t>(BasicConfigurationAction::UnassignOutputs)));
	EXPECT_EQ(static_cast<output_pin_e>(Gpio::A0), engineConfiguration->fuelPumpPin);
	EXPECT_FALSE(needToTriggerTsRefresh());
	EXPECT_EQ(0, getConfigurationWriteRequestCountForUnitTest());
}
