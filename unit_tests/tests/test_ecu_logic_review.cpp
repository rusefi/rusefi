#include "pch.h"

#include "ac_control.h"
#include "dc_motor.h"
#include "electronic_throttle_impl.h"
#include "fuel_math.h"
#include "injector_model.h"
#include "gear_detector.h"
#include "high_pressure_fuel_pump.h"
#include "limp_manager.h"
#include "knock_logic.h"
#include "mocks.h"
#include "spark_logic.h"
#include "speed_density_airmass.h"
#include "stored_value_sensor.h"
#include "vvt.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

// EcuLogicReview coverage deliberately asserts current defects.
// When a fix lands, update or remove its reproduction in favor of regression coverage.
// The ranked findings and intended behavior are in order_by_severity.md.

static void reproduceInvalidDcOutput(dc_function_e function) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	StrictMock<MockMotor> motor;
	EtbController dut;
	ASSERT_TRUE(dut.init(function, &motor, &engineConfiguration->etbWastegatePid, nullptr));

	// Keep the union's float initialized so this assertion is deterministic.
	// The controller must reject the invalid flag regardless of payload bytes.
	expected<percent_t> invalidOutput = 50.0f;
	invalidOutput.Valid = false;
	EXPECT_CALL(motor, disable(_)).Times(0);
	EXPECT_CALL(motor, enable());
	EXPECT_CALL(motor, set(0.5f)).WillOnce(Return(false));
	dut.setOutput(invalidOutput);
}

TEST(EcuLogicReview, WastegateDrivesMotorFromInvalidOutput) {
	reproduceInvalidDcOutput(DC_Wastegate);
}

TEST(EcuLogicReview, IdleValveDrivesMotorFromInvalidOutput) {
	reproduceInvalidDcOutput(DC_IdleValve);
}

static void reproduceDcSensorTimeout(dc_function_e function, SensorType positionType) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	Sensor::resetMockValue(positionType);
	class PositionSensor final : public StoredValueSensor {
	public:
		explicit PositionSensor(SensorType type) : StoredValueSensor(type, MS2NT(5)) {}
		~PositionSensor() { unregister(); }
	} position(positionType);
	ASSERT_TRUE(position.Register());
	position.setValidValue(20, getTimeNowNt());
	class ObservedController final : public EtbController {
	public:
		void setOutput(expected<percent_t> output) override {
			lastOutputValid = output.Valid;
			// The real pipeline has produced failure. Initialize only the unusable
			// payload at this boundary, preserving failure, to avoid relying on UB.
			if (!output.Valid) {
				output.Value = 50;
			}
			EtbController::setOutput(output);
		}
		bool lastOutputValid = false;
	} dut;
	StrictMock<MockMotor> motor;
	ASSERT_TRUE(dut.init(function, &motor, &engineConfiguration->etbWastegatePid, nullptr));
	EXPECT_CALL(motor, enable()).Times(2);
	EXPECT_CALL(motor, disable(_)).Times(0);
	EXPECT_CALL(motor, set(_)).WillOnce(Return(false));
	advanceTimeUs(1000);
	dut.update();
	ASSERT_TRUE(dut.lastOutputValid);
	ASSERT_TRUE(dut.validPlantPosition);

	advanceTimeUs(5000);
	ASSERT_FALSE(Sensor::get(positionType));
	EXPECT_CALL(motor, set(0.5f)).WillOnce(Return(false));
	dut.update();
	EXPECT_FALSE(dut.lastOutputValid);
	EXPECT_FALSE(dut.validPlantPosition);
}

TEST(EcuLogicReview, WastegateSensorTimeoutStillEnablesMotor) {
	reproduceDcSensorTimeout(DC_Wastegate, SensorType::WastegatePosition);
}

TEST(EcuLogicReview, IdlePositionSensorTimeoutStillEnablesMotor) {
	reproduceDcSensorTimeout(DC_IdleValve, SensorType::IdlePosition);
}

namespace {

// An invalid result has no usable float payload. Initialize that storage here
// only to make misuse observable without depending on uninitialized bytes.
SensorResult invalidResultWithPayload(float payload) {
	SensorResult result = payload;
	result.Valid = false;
	return result;
}

class InvalidPayloadSensor final : public Sensor {
public:
	explicit InvalidPayloadSensor(SensorType type) : Sensor(type) {}
	~InvalidPayloadSensor() { unregister(); }
	SensorResult get() const override { return invalidResultWithPayload(payload); }
	void showInfo(const char*) const override {}
	float payload = 0;
};

class InvalidPressureInjector final : public InjectorModelPrimary {
public:
	expected<float> getFuelDifferentialPressure() const override {
		return invalidResultWithPayload(payload);
	}
	float payload = 0;
};

} // namespace

TEST(EcuLogicReview, InvalidDifferentialPressureBecomesValidAbsolutePressure) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	Sensor::setMockValue(SensorType::Map, 100);
	InvalidPressureInjector dut;
	dut.payload = 300;
	ASSERT_FALSE(dut.getFuelDifferentialPressure());
	const auto absolute = dut.getFuelPressure();
	EXPECT_TRUE(absolute.Valid);
	EXPECT_FLOAT_EQ(400, absolute.Value);
}

TEST(EcuLogicReview, InvalidDifferentialPressureControlsDeadtimeAxis) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->useAbsolutePressureForLagTime = false;
	InvalidPressureInjector dut;
	dut.payload = 100;
	dut.updateState();
	EXPECT_FLOAT_EQ(100, dut.pressureCorrectionReference);
	dut.payload = 500;
	dut.updateState();
	EXPECT_FLOAT_EQ(500, dut.pressureCorrectionReference);
}

TEST(EcuLogicReview, SecondaryInjectorOverDutyDoesNotTriggerProtection) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->injectionMode = IM_SEQUENTIAL;
	engineConfiguration->enableStagedInjection = true;
	engineConfiguration->injectorFlowAsMassFlow = true;
	engineConfiguration->injector.flow = 1;
	engineConfiguration->injectorSecondary.flow = 2;
	engineConfiguration->injectorCompensationMode = ICM_None;
	engineConfiguration->secondaryInjectorCompensationMode = ICM_None;
	engineConfiguration->injectorNonlinearMode = INJ_None;
	setTable(engineConfiguration->injector.battLagCorrTable, 1.0f);
	setTable(engineConfiguration->injectorSecondary.battLagCorrTable, 1.0f);
	setTable(config->injectorStagingTable, 90);
	engineConfiguration->maxInjectorDutyInstant = 95;
	engineConfiguration->maxInjectorDutySustained = 85;
	engineConfiguration->maxInjectorDutySustainedTimeout = 0.5f;
	engine->rpmCalculator.setRpmValue(3000);

	InjectorModelPrimary primary;
	InjectorModelSecondary secondary;
	primary.prepare();
	secondary.prepare();
	const float fraction = getStage2InjectionFraction(3000, 100);
	ASSERT_NEAR(0.9f, fraction, 0.00001f);
	const float mass = 0.1f;
	engine->engineState.injectionDuration = primary.getInjectionDuration(mass * (1 - fraction));
	engine->engineState.injectionDurationStage2 = secondary.getInjectionDuration(mass * fraction);
	ASSERT_NEAR(27.5f, getInjectorDutyCycle(3000), 0.001f);
	ASSERT_NEAR(115.0f, getInjectorDutyCycleStage2(3000), 0.001f);

	LimpManager dut;
	dut.updateState(3000, getTimeNowNt());
	EXPECT_TRUE(dut.allowInjection());
	advanceTimeUs(1000000);
	dut.updateState(3000, getTimeNowNt());
	EXPECT_TRUE(dut.allowInjection());

	// The same excessive pulse on the primary is protected immediately.
	engine->engineState.injectionDuration = engine->engineState.injectionDurationStage2;
	dut.updateState(3000, getTimeNowNt());
	EXPECT_EQ(ClearReason::InjectorDutyCycle, dut.allowInjection().reason);
}

TEST(EcuLogicReview, DfcoOneSecondTimingRampEndsAfterHalfSecond) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->coastingFuelCutEnabled = true;
	engineConfiguration->coastingFuelCutRequiresGear = false;
	engineConfiguration->dfcoFuelCutMode = dfco_fuel_cut_mode_e::Overrun;
	engineConfiguration->useEngineStateMachine = false;
	engineConfiguration->coastingFuelCutRpmLow = 1300;
	engineConfiguration->coastingFuelCutRpmHigh = 1500;
	engineConfiguration->coastingFuelCutVssLow = 0;
	engineConfiguration->coastingFuelCutVssHigh = 0;
	engineConfiguration->coastingFuelCutTps = 2;
	engineConfiguration->coastingFuelCutClt = 30;
	engineConfiguration->coastingFuelCutMap = 100;
	engineConfiguration->dfcoDelay = 0;
	engineConfiguration->dfcoRetardDeg = 20;
	engineConfiguration->dfcoRetardRampInTime = 1;
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 0);
	Sensor::setMockValue(SensorType::Clt, 90);
	Sensor::setMockValue(SensorType::Map, 30);
	Sensor::setMockValue(SensorType::Rpm, 3000);
	DfcoController dut;
	dut.update();
	ASSERT_TRUE(dut.cutFuel());
	ASSERT_FLOAT_EQ(20, dut.getTimingRetard());

	Sensor::setMockValue(SensorType::DriverThrottleIntent, 50);
	dut.update();
	ASSERT_FALSE(dut.cutFuel());
	advanceTimeUs(250000);
	EXPECT_FLOAT_EQ(10, dut.getTimingRetard()); // A 1-second ramp would still remove 15 degrees.
	advanceTimeUs(250000);
	EXPECT_FLOAT_EQ(0, dut.getTimingRetard()); // Configured ramp is only halfway complete.
}

TEST(EcuLogicReview, GearDetectorCanExceedBoostAdderForwardGearCapacity) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->totalGearsCount = TCU_GEAR_COUNT;
	engineConfiguration->gearDetectionUseOutputShaftSpeed = true;
	engineConfiguration->gearDetectionRpmSourceIsInputShaftSpeed = false;
	engineConfiguration->clutchDownPin = Gpio::Unassigned;
	engineConfiguration->clutchUpPin = Gpio::Unassigned;
	for (size_t i = 0; i < TCU_GEAR_COUNT; i++) {
		engineConfiguration->gearRatio[i] = TCU_GEAR_COUNT - i;
	}
	Sensor::setMockValue(SensorType::OutputShaftSpeed, 1000);
	Sensor::setMockValue(SensorType::Rpm, 1000);
	Sensor::resetMockValue(SensorType::DetectedGear);
	auto& detector = engine->module<GearDetector>().unmock();
	detector.onConfigurationChange(nullptr);
	detector.onSlowCallback();
	ASSERT_EQ(TCU_GEAR_COUNT, Sensor::getOrZero(SensorType::DetectedGear));

	// The boost table reserves the first two entries for reverse and neutral.
	// Do not perform an out-of-bounds read in this portable reproduction.
	const auto forwardGearCapacity = efi::size(engineConfiguration->gearBasedOpenLoopBoostAdder) - 2;
	EXPECT_GT(Sensor::getOrZero(SensorType::DetectedGear), forwardGearCapacity);
}

TEST(EcuLogicReview, DistributorUsesFirstCylinderTimingTrimForEveryCylinder) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->cylindersCount = 4;
	engineConfiguration->firingOrder = FO_1_3_4_2;
	engineConfiguration->ignitionMode = IM_ONE_COIL;
	engine->updateTriggerConfiguration();
	setTable(config->ignitionTable, 15);
	setTable(config->ignTrims[0].table, 0);
	setTable(config->ignTrims[1].table, -2);
	setTable(config->ignTrims[2].table, -4);
	setTable(config->ignTrims[3].table, -6);
	engine->rpmCalculator.setRpmValue(2000);
	engine->periodicFastCallback();
	ASSERT_FLOAT_EQ(15, engine->engineState.timingAdvance[0]);
	ASSERT_FLOAT_EQ(11, engine->engineState.timingAdvance[2]);
	initializeIgnitionActions();
	ASSERT_TRUE(engine->ignitionEvents.isReady);
	// Event 1 fires cylinder 3 at 180 degrees. Its intended timing is 11 BTDC,
	// but scheduling uses cylinder 1's 15 degrees because they share a coil.
	EXPECT_FLOAT_EQ(165, engine->ignitionEvents.elements[1].sparkAngle);
	EXPECT_FLOAT_EQ(15, engine->outputChannels.ignitionAdvanceCyl[1]);
}

TEST(EcuLogicReview, WastedSparkUsesPairedCylinderTimingTrim) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->cylindersCount = 4;
	engineConfiguration->firingOrder = FO_1_3_4_2;
	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
	engine->updateTriggerConfiguration();
	setTable(config->ignitionTable, 15);
	setTable(config->ignTrims[0].table, 0);
	setTable(config->ignTrims[3].table, -6);
	engine->rpmCalculator.setRpmValue(2000);
	engine->periodicFastCallback();
	ASSERT_FLOAT_EQ(9, engine->engineState.timingAdvance[3]);
	initializeIgnitionActions();
	// Event 2 fires cylinder 4 at 360 degrees, sharing cylinder 1's coil.
	EXPECT_FLOAT_EQ(345, engine->ignitionEvents.elements[2].sparkAngle);
	EXPECT_FLOAT_EQ(15, engine->outputChannels.ignitionAdvanceCyl[2]);
}

TEST(EcuLogicReview, DistributorIgnoresOddFireCylinderOffset) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->cylindersCount = 6;
	engineConfiguration->firingOrder = FO_1_2_3_4_5_6;
	engineConfiguration->ignitionMode = IM_ONE_COIL;
	// Alternate 90/150 degree intervals, as for an odd-fire six-cylinder engine.
	for (size_t i = 0; i < 6; i++) {
		engineConfiguration->timing_offset_cylinder[i] = (i % 2) ? -30 : 0;
	}
	engine->updateTriggerConfiguration();
	ASSERT_TRUE(engine->engineState.useOddFireWastedSpark);
	engine->rpmCalculator.setRpmValue(2000);
	engine->ignitionState.sparkDwell = 3;
	engine->ignitionState.dwellDurationAngle = 36;
	setArrayValues(engine->engineState.timingAdvance, 10.0f);
	initializeIgnitionActions();
	ASSERT_FLOAT_EQ(90, getPerCylinderFiringOrderOffset(1, 1));
	// Cylinder 2 should spark at 80 degrees (10 before its 90-degree TDC).
	EXPECT_FLOAT_EQ(110, engine->ignitionEvents.elements[1].sparkAngle);
}

TEST(EcuLogicReview, KnockIdentityUsesFiringOrderPositionInsteadOfCylinderNumber) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->cylindersCount = 4;
	engineConfiguration->firingOrder = FO_1_3_4_2;
	engineConfiguration->ignitionMode = IM_ONE_COIL;
	engineConfiguration->knockBankCyl2 = 0;
	engineConfiguration->knockBankCyl3 = 1;
	engine->updateTriggerConfiguration();
	engine->rpmCalculator.setRpmValue(2000);
	initializeIgnitionActions();
	const auto& event = engine->ignitionEvents.elements[1];
	ASSERT_EQ(2, getCylinderNumberAtIndex(event.cylinderIndex));
	// fireSparkAndPrepareNextSchedule passes cylinderIndex directly to knock.
	// Production software knock uses this as a physical cylinder for channel/gain.
	// That hardware sampling branch is disabled in the normal host build.
	EXPECT_EQ(1, event.cylinderIndex);
	EXPECT_EQ(0, getCylinderKnockBank(event.cylinderIndex));
	EXPECT_EQ(1, getCylinderKnockBank(2));
}

TEST(EcuLogicReview, KnockRetardTreatsWrappedNegativeTimingAsLargeAdvance) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->knockRetardAggression = 10;
	engineConfiguration->knockSuppressMinTps = 0;
	setArrayValues(config->knockBaseNoise, 20);
	setTable(config->maxKnockRetardTable, 8);
	setTable(config->ignitionTable, -10);
	engine->rpmCalculator.setRpmValue(2000);
	engine->periodicFastCallback();
	ASSERT_FLOAT_EQ(710, engine->engineState.timingAdvance[0]);
	KnockController dut;
	dut.onFastCallback();
	ASSERT_TRUE(dut.onKnockSenseCompleted(0, 30, getTimeNowNt()));
	// Ten percent of the distance from -10 to -20 is 1 degree, not the 8-degree cap.
	EXPECT_FLOAT_EQ(8, dut.getKnockRetard());
}

TEST(EcuLogicReview, InvalidFuelPressureSelectsLinearizedInjectionDuration) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->useInjectorFlowLinearizationTable = true;
	Sensor::resetMockValue(SensorType::FuelPressureInjector);
	InvalidPayloadSensor sensor(SensorType::FuelPressureInjector);
	ASSERT_TRUE(sensor.Register());
	config->injectorFlowLinearizationPressureBins[0] = 10;
	config->injectorFlowLinearizationPressureBins[1] = 100;
	config->injectorFlowLinearizationFuelMassBins[0] = 0;
	config->injectorFlowLinearizationFuelMassBins[1] = 100;
	setArrayValues(config->injectorFlowLinearization[0], 5.0f);
	setArrayValues(config->injectorFlowLinearization[1], 1.0f);
	InjectorModelPrimary dut;
	sensor.payload = 1000; // kPa; the table axis is bar.
	ASSERT_FALSE(Sensor::get(SensorType::FuelPressureInjector));
	EXPECT_FLOAT_EQ(5, dut.getInjectionDuration(0.01f));
	sensor.payload = 10000;
	ASSERT_FALSE(Sensor::get(SensorType::FuelPressureInjector));
	EXPECT_FLOAT_EQ(1, dut.getInjectionDuration(0.01f));
}

TEST(EcuLogicReview, InvalidHighPressureSelectsManualFuelMassCorrection) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->injectorCompensationMode = ICM_HPFP_Manual_Compensation;
	engineConfiguration->injectorFlowAsMassFlow = true;
	engineConfiguration->injector.flow = 1;
	engineConfiguration->injectorNonlinearMode = INJ_None;
	setTable(engineConfiguration->injector.battLagCorrTable, 1.0f);
	for (size_t i = 0; i < HPFP_FUEL_MASS_COMPENSATION_SIZE; i++) {
		config->hpfpFuelMassCompensationFuelPressure[i] = 10 * (i + 1);
		config->hpfpFuelMassCompensationFuelMass[i] = 10 * (i + 1);
		setArrayValues(config->hpfpFuelMassCompensation[i], i == 0 ? 1.0f : 2.0f);
	}
	Sensor::resetMockValue(SensorType::FuelPressureHigh);
	InvalidPayloadSensor sensor(SensorType::FuelPressureHigh);
	ASSERT_TRUE(sensor.Register());
	InjectorModelPrimary dut;
	dut.prepare();
	sensor.payload = 1000;
	ASSERT_FALSE(Sensor::get(SensorType::FuelPressureHigh));
	EXPECT_NEAR(11, dut.getInjectionDuration(0.01f), 0.001f);
	sensor.payload = 8000;
	ASSERT_FALSE(Sensor::get(SensorType::FuelPressureHigh));
	EXPECT_NEAR(21, dut.getInjectionDuration(0.01f), 0.001f);
}

TEST(LimpRegression, InvalidOilPressurePayloadDoesNotTriggerMaxCut) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->minOilPressureAfterStart = 0;
	engineConfiguration->enableOilPressureProtect = false;
	engineConfiguration->maxOilPressureTimeout = 1;
	setArrayValues(config->maximumOilPressureValues, 500);
	Sensor::resetMockValue(SensorType::OilPressure);
	InvalidPayloadSensor sensor(SensorType::OilPressure);
	ASSERT_TRUE(sensor.Register());
	engine->rpmCalculator.setRpmValue(1000);
	LimpManager dut;
	sensor.payload = 400;
	ASSERT_FALSE(Sensor::get(SensorType::OilPressure));
	dut.updateState(1000, getTimeNowNt());
	ASSERT_TRUE(dut.allowInjection());
	sensor.payload = 600;
	advanceTimeUs(1100000);
	dut.updateState(1000, getTimeNowNt());
	EXPECT_TRUE(dut.allowInjection());

	// Neither result is valid; changing unusable storage cannot change protection.
	sensor.payload = 400;
	dut.updateState(1000, getTimeNowNt());
	EXPECT_TRUE(dut.allowInjection());
}

namespace {

void configureReviewPrimaryFuel(float massGrams) {
	engineConfiguration->cylindersCount = 4;
	engineConfiguration->firingOrder = FO_1_3_4_2;
	engineConfiguration->injectionMode = IM_SEQUENTIAL;
	engineConfiguration->crankingInjectionMode = IM_SEQUENTIAL;
	engineConfiguration->enableStagedInjection = false;
	engineConfiguration->injectorFlowAsMassFlow = true;
	engineConfiguration->injector.flow = 1;
	engineConfiguration->injectorCompensationMode = ICM_None;
	engineConfiguration->injectorNonlinearMode = INJ_None;
	setTable(engineConfiguration->injector.battLagCorrTable, 0.0f);
	setTable(config->lambdaTable, 1.0f);
	engineConfiguration->coastingFuelCutEnabled = false;
	engineConfiguration->maxInjectorDutyInstant = 95;
	engineConfiguration->maxInjectorDutySustained = 85;
	engineConfiguration->maxInjectorDutySustainedTimeout = 0.5f;
	engine->engineState.lua.fuelMult = 0;
	engine->engineState.lua.fuelAdd = massGrams;
	engine->updateTriggerConfiguration();
	engine->rpmCalculator.setRpmValue(3000);
	engine->periodicFastCallback();
}

void scheduleReviewPrimaryPulse(EngineTestHelper& eth) {
	engine->engineState.injectionOffset = 0;
	engine->injectionEvents.addFuelEvents();
	auto& event = engine->injectionEvents.elements[0];
	event.injectionStartAngle = 0;
	event.onTriggerTooth(getTimeNowNt(), 0, 10);
	eth.executeActions();
}

} // namespace

TEST(EcuLogicReview, CylinderFuelTrimSaturatesInjectorWithoutDutyProtection) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setTable(config->fuelTrims[0].table, 25);
	configureReviewPrimaryFuel(0.0328f);
	ASSERT_NEAR(32.8f, engine->engineState.injectionDuration, 0.001f);
	ASSERT_NEAR(82, getInjectorDutyCycle(3000), 0.001f);
	ASSERT_NEAR(0.041f, engine->engineState.injectionMass[0], 0.000001f);
	ASSERT_TRUE(engine->engineState.shouldUpdateInjectionTiming);

	LimpManager dut;
	dut.updateState(3000, getTimeNowNt());
	ASSERT_TRUE(dut.allowInjection());
	scheduleReviewPrimaryPulse(eth);
	EXPECT_TRUE(enginePins.injectors[0].getLogicValue());
	EXPECT_NEAR(41, engine->outputChannels.actualLastInjection, 0.001f);
	EXPECT_GT(100 * engine->outputChannels.actualLastInjection / getEngineCycleDuration(3000), 100);
	eth.moveTimeForwardUs(42000);
	eth.executeActions();
	EXPECT_FALSE(enginePins.injectors[0].getLogicValue());

	advanceTimeUs(1000000);
	dut.updateState(3000, getTimeNowNt());
	EXPECT_TRUE(dut.allowInjection());
}

TEST(EcuLogicReview, WallFuelSaturatesInjectorWithoutDutyProtection) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	configureReviewPrimaryFuel(0.024f);
	engineConfiguration->complexWallModel = false;
	engineConfiguration->wwaeTau = 0.5f;
	engineConfiguration->wwaeBeta = 0.5f;
	auto& wallController = engine->module<WallFuelController>().unmock();
	wallController.onFastCallback();
	ASSERT_TRUE(wallController.getEnable());
	ASSERT_FLOAT_EQ(0.5f, wallController.getBeta());
	ASSERT_NEAR(60, getInjectorDutyCycle(3000), 0.001f);
	LimpManager dut;
	dut.updateState(3000, getTimeNowNt());
	ASSERT_TRUE(dut.allowInjection());

	scheduleReviewPrimaryPulse(eth);
	EXPECT_TRUE(enginePins.injectors[0].getLogicValue());
	EXPECT_NEAR(48, engine->outputChannels.actualLastInjection, 0.001f);
	EXPECT_NEAR(120, 100 * engine->outputChannels.actualLastInjection / getEngineCycleDuration(3000), 0.001f);
	EXPECT_TRUE(engine->engineState.shouldUpdateInjectionTiming);
	dut.updateState(3000, getTimeNowNt());
	EXPECT_TRUE(dut.allowInjection());
	eth.moveTimeForwardUs(49000);
	eth.executeActions();
	EXPECT_FALSE(enginePins.injectors[0].getLogicValue());
}

TEST(EcuLogicReview, PercentAccelEnrichmentIsReducedByInjectionModeTwice) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupSimpleTestEngineWithMafAndTT_ONE_trigger(&eth);
	setTable(config->lambdaTable, 1.0f);
	EXPECT_CALL(*eth.mockAirmass, getAirmass(_, _))
		.WillRepeatedly(Return(AirmassResult{1.3440001f, 50.0f}));
	engine->rpmCalculator.setRpmValue(1200);
	engineConfiguration->accelEnrichmentMode = AE_MODE_PERCENT_ADDER;
	engineConfiguration->isTuningDetectorEnabled = false;
	engineConfiguration->coastingFuelCutEnabled = false;
	engineConfiguration->tpsAccelLookback = 1;
	engineConfiguration->tpsAccelEnrichmentThreshold = 5;
	engineConfiguration->tpsAccelFractionPeriod = 1;
	engineConfiguration->tpsAccelFractionDivisor = 1;
	engineConfiguration->flexFuelTransientComp = false;
	setArrayValues(config->tpsTspCorrValues, 1.0f);
	setTable(config->tpsTpsAccelTable, 0.20f);
	initAccelEnrichment();
	auto& ae = engine->module<TpsAccelEnrichment>().unmock();
	const injection_mode_e modes[] = {IM_SEQUENTIAL, IM_BATCH, IM_SIMULTANEOUS};
	const float actualRatios[] = {1.20f, 1.10f, 1.05f};
	ASSERT_EQ(4, engineConfiguration->cylindersCount);
	for (size_t i = 0; i < efi::size(modes); i++) {
		SCOPED_TRACE(i);
		engineConfiguration->injectionMode = modes[i];
		ae.resetAE();
		ae.setLength(2);
		ae.onNewValue(0);
		const float baseline = getInjectionMass(1200);
		ASSERT_TRUE(std::isfinite(baseline));
		ASSERT_GT(baseline, 0);
		ae.onNewValue(0);
		ae.onNewValue(20);
		ASSERT_NEAR(0.20f, ae.getTpsEnrichment(), 0.00001f);
		// A 20% correction should retain its ratio when a cycle is split
		// into multiple pulses. Batch adds 10%, simultaneous only 5%.
		EXPECT_NEAR(actualRatios[i], getInjectionMass(1200) / baseline, 0.00001f);
	}
}

TEST(EcuLogicReview, CrankingBatchDutyIsUnderreportedAsRunningSequential) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->cylindersCount = 4;
	engineConfiguration->injectionMode = IM_SEQUENTIAL;
	engineConfiguration->crankingInjectionMode = IM_BATCH;
	engineConfiguration->cranking.rpm = 800;
	engineConfiguration->maxInjectorDutyInstant = 95;
	engineConfiguration->maxInjectorDutySustained = 85;
	engineConfiguration->maxInjectorDutySustainedTimeout = 0.5f;
	engine->rpmCalculator.setRpmValue(600);
	ASSERT_TRUE(engine->rpmCalculator.isCranking());
	ASSERT_EQ(IM_BATCH, getCurrentInjectionMode());
	engine->engineState.injectionDuration = 110;
	ASSERT_NEAR(200, getEngineCycleDuration(600), 0.001f);
	EXPECT_NEAR(55, getInjectorDutyCycle(600), 0.001f);
	EXPECT_NEAR(110, 100 * engine->engineState.injectionDuration
		* getNumberOfInjections(getCurrentInjectionMode()) / getEngineCycleDuration(600), 0.001f);
	LimpManager dut;
	dut.updateState(600, getTimeNowNt());
	EXPECT_TRUE(dut.allowInjection());
	advanceTimeUs(1000000);
	dut.updateState(600, getTimeNowNt());
	EXPECT_TRUE(dut.allowInjection());
}

TEST(EcuLogicReview, CrankingSequentialDutyIsOverreportedAsRunningSimultaneous) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->cylindersCount = 4;
	engineConfiguration->injectionMode = IM_SIMULTANEOUS;
	engineConfiguration->crankingInjectionMode = IM_SEQUENTIAL;
	engineConfiguration->cranking.rpm = 800;
	engineConfiguration->maxInjectorDutyInstant = 95;
	engineConfiguration->maxInjectorDutySustained = 85;
	engineConfiguration->maxInjectorDutySustainedTimeout = 0.5f;
	engine->rpmCalculator.setRpmValue(600);
	ASSERT_TRUE(engine->rpmCalculator.isCranking());
	ASSERT_EQ(IM_SEQUENTIAL, getCurrentInjectionMode());
	engine->engineState.injectionDuration = 50;
	EXPECT_NEAR(100, getInjectorDutyCycle(600), 0.001f);
	EXPECT_NEAR(25, 100 * engine->engineState.injectionDuration
		* getNumberOfInjections(getCurrentInjectionMode()) / getEngineCycleDuration(600), 0.001f);
	LimpManager dut;
	dut.updateState(600, getTimeNowNt());
	EXPECT_EQ(ClearReason::InjectorDutyCycle, dut.allowInjection().reason);
}

namespace {

void configureReviewVvt() {
	Sensor::setMockValue(SensorType::Clt, 70);
	engineConfiguration->vvtControlMinRpm = 500;
	engineConfiguration->vvtControlMinClt = 0;
	engineConfiguration->vvtActivationDelayMs = 0;
	engineConfiguration->vvtAdvancedModeEnabled = false;
	engineConfiguration->invertVvtControlIntake = false;
	engineConfiguration->auxPid[0].pFactor = 1;
	engineConfiguration->auxPid[0].iFactor = 0;
	engineConfiguration->auxPid[0].dFactor = 0;
	engineConfiguration->auxPid[0].offset = 0;
	engineConfiguration->auxPid[0].minValue = 0;
	engineConfiguration->auxPid[0].maxValue = 100;
}

} // namespace

TEST(EcuLogicReview, VvtActivationDelayTruncatesMillisecondsToWholeSeconds) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	configureReviewVvt();
	engineConfiguration->vvtActivationDelayMs = 1500;
	engine->rpmCalculator.setRpmValue(1500);
	StrictMock<MockVp3d> targetMap;
	StrictMock<MockPwm> pwm;
	EXPECT_CALL(targetMap, getValue(_, _)).WillRepeatedly(Return(20));
	VvtController dut(0);
	dut.init(&targetMap, &pwm);
	advanceTimeUs(900000);
	// Keep cam feedback fresh so this test isolates activation-delay rounding.
	engine->triggerCentral.vvtPositionUpdateTimer[0][0].reset();
	EXPECT_CALL(pwm, setSimplePwmDutyCycle(0));
	dut.onFastCallback();
	advanceTimeUs(200000);
	ASSERT_NEAR(1.1f, engine->rpmCalculator.getSecondsSinceEngineStart(getTimeNowNt()), 0.001f);
	// Only 1100 ms have passed, but the requested 1500 ms delay becomes 1 s.
	EXPECT_CALL(pwm, setSimplePwmDutyCycle(testing::FloatNear(0.2f, 0.00001f)));
	dut.onFastCallback();
	EXPECT_FLOAT_EQ(20, dut.vvtOutput);
}

TEST(EcuLogicReview, RebuildingIgnitionScheduleRedirectsPendingCoilShutoff) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->ignitionMode = IM_TWO_COILS;
	engine->rpmCalculator.setRpmValue(1500);
	engine->ignitionState.updateDwell(1500, false);
	setArrayValues(engine->engineState.timingAdvance, 0.0f);
	initializeIgnitionActions();
	auto& event = engine->ignitionEvents.elements[1];
	auto* chargedCoil = event.outputs[0];
	ASSERT_NE(nullptr, chargedCoil);
	ASSERT_NE(&enginePins.coils[0], chargedCoil);
	ASSERT_EQ(1u, event.sparkCounter);
	turnSparkPinHighStartCharging(&event);
	ASSERT_TRUE(chargedCoil->getLogicValue());
	const auto fireTime = getTimeNowNt() + MS2NT(3);
	engine->scheduler.schedule("firing", &event.sparkEvent.eventScheduling,
		fireTime, action_s::make<fireSparkAndPrepareNextSchedule>(&event));

	// A changed trigger configuration clears isReady in mainTriggerCallback;
	// the next onTriggerEventSparkLogic rebuilds these same event objects.
	engineConfiguration->ignitionMode = IM_ONE_COIL;
	engine->ignitionEvents.isReady = false;
	onTriggerEventSparkLogic(1500, getTimeNowNt(), 0, 0);
	ASSERT_EQ(&enginePins.coils[0], event.outputs[0]);
	ASSERT_TRUE(chargedCoil->getLogicValue());

	// The pending fire callback dereferences the new mapping, so the coil
	// charged under the old mapping stays on while the engine is still running.
	eth.setTimeAndInvokeEventsUs(getTimeNowUs() + 3001);
	EXPECT_FALSE(enginePins.coils[0].getLogicValue());
	EXPECT_TRUE(chargedCoil->getLogicValue());
	chargedCoil->setLow();
}

TEST(EcuLogicReview, PredictiveMapBlendReplaysAfterQuickEngineRestart) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->accelEnrichmentMode = AE_MODE_PREDICTIVE_MAP;
	engineConfiguration->tpsAccelEnrichmentThreshold = 5;
	setArrayValues(config->predictiveMapBlendDurationValues, 2.0f);
	StrictMock<MockVp3d> veTable;
	StrictMock<MockVp3d> mapFallback;
	EXPECT_CALL(mapFallback, getValue(1500, 30)).WillRepeatedly(Return(100));
	SpeedDensityAirmass dut(veTable, mapFallback);
	Sensor::setMockValue(SensorType::Map, 40);
	Sensor::setMockValue(SensorType::Tps1, 30);
	engine->rpmCalculator.setRpmValue(1500);
	auto& ae = engine->module<TpsAccelEnrichment>().unmock();
	ae.m_accelEventJustOccurred = true;
	ASSERT_FLOAT_EQ(100, dut.getMap(1500, false));
	ASSERT_TRUE(engine->outputChannels.isMapPredictionActive);

	engine->rpmCalculator.setStopSpinning();
	ASSERT_FALSE(engine->rpmCalculator.isRunning());
	eth.moveTimeForwardMs(200);
	engine->rpmCalculator.setRpmValue(1500);
	ASSERT_TRUE(engine->rpmCalculator.isRunning());
	ASSERT_FALSE(ae.m_accelEventJustOccurred);
	// No new TPS acceleration in this run, but the previous blend still
	// substitutes a 94-kPa estimate for the valid 40-kPa MAP measurement.
	EXPECT_NEAR(94, dut.getMap(1500, false), 0.01f);
	EXPECT_TRUE(engine->outputChannels.isMapPredictionActive);
}

TEST(EcuLogicReview, ZeroHpfpLobesProduceNonfiniteFuelRequest) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->hpfpCamLobes = 0;
	engineConfiguration->hpfpPumpVolume = 0.29f;
	ASSERT_TRUE(validateConfigOnStartUpOrBurn());
	engine->engineState.injectionMass[0] = 0.01f * fuelDensity;
	HpfpQuantity quantity;
	EXPECT_FALSE(std::isfinite(quantity.calcFuelPercent(2000)));
	HpfpController controller;
	const auto requestedAngle = quantity.pumpAngleFuel(2000, &controller);
	EXPECT_FALSE(std::isfinite(controller.fuel_requested_percent));
	EXPECT_GT(requestedAngle, engineConfiguration->hpfpMinAngle);
}

TEST(EcuLogicReview, ZeroInjectorFlowPassesValidationAndProducesInfinitePulse) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->injector.flow = 0;
	engineConfiguration->useInjectorFlowLinearizationTable = false;
	ASSERT_TRUE(validateConfigOnStartUpOrBurn());
	auto& injector = engine->module<InjectorModelPrimary>().unmock();
	injector.prepare();
	EXPECT_TRUE(std::isinf(injector.getInjectionDuration(0.01f)));
}

TEST(EcuLogicReview, FailedAcPressureSensorReleasesCompressorPressureCut) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->acDelay = 0;
	engineConfiguration->maxAcRpm = 0;
	engineConfiguration->maxAcTps = 0;
	engineConfiguration->maxAcClt = 120;
	engineConfiguration->minAcPressure = 100;
	engineConfiguration->maxAcPressure = 200;
	Sensor::setMockValue(SensorType::Rpm, 1000);
	Sensor::setMockValue(SensorType::Clt, 80);
	auto& ac = engine->module<AcController>().unmock();
	ac.acButtonState = true;
	Sensor::setMockValue(SensorType::AcPressure, 250);
	ac.onSlowCallback();
	ASSERT_TRUE(ac.acPressureTooHigh);
	ASSERT_FALSE(enginePins.acRelay.getLogicValue());

	Sensor::setInvalidMockValue(SensorType::AcPressure);
	ASSERT_FALSE(Sensor::get(SensorType::AcPressure));
	ac.onSlowCallback();
	EXPECT_TRUE(ac.isAcEnabled());
	EXPECT_TRUE(enginePins.acRelay.getLogicValue());
}
