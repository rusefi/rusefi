#include "pch.h"

#include "basic_configuration.h"
#include "board_overrides.h"
#include "defaults.h"
#include "electronic_throttle_impl.h"
#include "engine_types.h"
#include "idle_hardware.h"
#include "tunerstudio.h"

extern engine_configuration_s& activeConfiguration;

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
