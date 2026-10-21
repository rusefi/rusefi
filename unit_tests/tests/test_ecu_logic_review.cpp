#include "pch.h"

#include "dc_motor.h"
#include "electronic_throttle_impl.h"
#include "fuel_math.h"
#include "injector_model.h"
#include "gear_detector.h"
#include "limp_manager.h"
#include "main_trigger_callback.h"
#include "knock_logic.h"
#include "mocks.h"
#include "spark_logic.h"
#include "speed_density_airmass.h"
#include "stored_value_sensor.h"
#include "rusefi_lua.h"

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

} // namespace

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

// ECU-054: supported tau/RPM combinations exceed the exponential helper's domain.
TEST(EcuLogicReview, LowRpmWallFilmUsesWrongExponentialFallback) {
	for (int rpm : {2100, 1900}) {
		SCOPED_TRACE(rpm);
		EngineTestHelper eth(engine_type_e::TEST_ENGINE);
		configureReviewPrimaryFuel(0.010f);
		engineConfiguration->complexWallModel = false;
		engineConfiguration->wwaeTau = 0.03f;
		engineConfiguration->wwaeBeta = 0.5f;
		engine->rpmCalculator.setRpmValue(rpm);
		engine->periodicFastCallback();
		auto& controller = engine->module<WallFuelController>().unmock();
		ASSERT_TRUE(controller.getEnable());
		ASSERT_FALSE(engine->rpmCalculator.isCranking());
		ASSERT_FLOAT_EQ(0, engine->injectionEvents.elements[0].getWallFuel().getWallFuel());
		ASSERT_NEAR(10, engine->engineState.injectionDuration, 0.001f);
		const float trueAlpha = std::exp(-120.0f / (rpm * 0.03f));
		// The model limits beta to alpha; both cases have true alpha below 0.5.
		const float expectedPulseMs = 10 / (1 - trueAlpha);
		scheduleReviewPrimaryPulse(eth);
		ASSERT_TRUE(enginePins.injectors[0].getLogicValue());
		if (rpm == 2100) {
			// Control inside the approximation's documented -2..0 domain.
			EXPECT_NEAR(trueAlpha, controller.getAlpha(), 0.01f);
			EXPECT_NEAR(expectedPulseMs, engine->outputChannels.actualLastInjection, 0.1f);
		} else {
			// Below -2 the helper returns 0.818 instead of tending toward zero.
			EXPECT_FLOAT_EQ(0.818f, controller.getAlpha());
			EXPECT_FLOAT_EQ(0.5f, controller.getBeta());
			EXPECT_NEAR(20, engine->outputChannels.actualLastInjection, 0.001f);
			EXPECT_GT(engine->outputChannels.actualLastInjection, 1.7f * expectedPulseMs);
		}
		// Execute the actual scheduled closure; no pulse is left pending.
		eth.moveTimeForwardUs(21000);
		eth.executeActions();
		EXPECT_FALSE(enginePins.injectors[0].getLogicValue());
		EXPECT_EQ(0, engine->scheduler.size());
	}
}

TEST(EcuLogicReview, FuelCutFreezesWallFilmUntilInjectionResumes) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	configureReviewPrimaryFuel(0.010f);
	engineConfiguration->isIgnitionEnabled = false;
	engineConfiguration->complexWallModel = false;
	engineConfiguration->wwaeTau = 0.5f;
	engineConfiguration->wwaeBeta = 0.5f;
	engine->periodicFastCallback();
	auto& controller = engine->module<WallFuelController>().unmock();
	auto& wall = engine->injectionEvents.elements[0].getWallFuel();
	auto& limp = engine->module<LimpManager>().unmock();
	ASSERT_TRUE(controller.getEnable());
	ASSERT_TRUE(limp.allowInjection());
	engine->injectionEvents.addFuelEvents();
	auto passInjectionTooth = [&] {
		// Geometry is held fixed so this exercises the real fuel-permission gate
		// and callbacks without depending on a particular crank wheel decoder.
		engine->injectionEvents.elements[0].injectionStartAngle = 0;
		mainTriggerCallback(1, getTimeNowNt(), 0, 10);
		eth.executeActions();
	};
	for (int cycle = 0; cycle < 100; cycle++) {
		passInjectionTooth();
		eth.moveTimeForwardUs(40000); // 720 degrees at 3000 RPM.
		eth.executeActions();
	}
	ASSERT_NEAR(10, engine->outputChannels.actualLastInjection, 0.01f);
	ASSERT_FALSE(enginePins.injectors[0].getLogicValue());
	const float filmBeforeCut = wall.getWallFuel();
	const int adjustmentsBeforeCut = wall.invocationCounter;
	ASSERT_GT(filmBeforeCut, 0.060f);

	// ECU-055: trigger events keep arriving during a one-second Lua fuel cut,
	// but handleFuel returns before the film can decay through zero-fuel cycles.
	engine->engineState.lua.luaFuelCut = true;
	limp.updateState(3000, getTimeNowNt());
	ASSERT_EQ(ClearReason::Lua, limp.allowInjection().reason);
	float decayedFilm = filmBeforeCut;
	for (int cycle = 0; cycle < 25; cycle++) {
		passInjectionTooth();
		EXPECT_FALSE(enginePins.injectors[0].getLogicValue());
		EXPECT_EQ(0, engine->scheduler.size());
		decayedFilm *= controller.getAlpha();
		eth.moveTimeForwardUs(40000);
	}
	EXPECT_EQ(adjustmentsBeforeCut, wall.invocationCounter);
	EXPECT_FLOAT_EQ(filmBeforeCut, wall.getWallFuel());
	ASSERT_LT(decayedFilm, 0.2f * filmBeforeCut);

	engine->engineState.lua.luaFuelCut = false;
	limp.updateState(3000, getTimeNowNt());
	ASSERT_TRUE(limp.allowInjection());
	passInjectionTooth();
	ASSERT_TRUE(enginePins.injectors[0].getLogicValue());
	const float modelPulseAfterDecayMs = 1000 *
		(0.010f - (1 - controller.getAlpha()) * decayedFilm) / (1 - controller.getBeta());
	EXPECT_GT(modelPulseAfterDecayMs, 18);
	EXPECT_NEAR(10, engine->outputChannels.actualLastInjection, 0.01f);
	EXPECT_LT(engine->outputChannels.actualLastInjection, 0.6f * modelPulseAfterDecayMs);
	eth.moveTimeForwardUs(40000);
	eth.executeActions();
	EXPECT_FALSE(enginePins.injectors[0].getLogicValue());
	EXPECT_EQ(0, engine->scheduler.size());
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

TEST(EcuLogicReview, UnburnedZeroInjectorFlowStillProducesInfinitePulse) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->useInjectorFlowLinearizationTable = false;
	ASSERT_TRUE(validateConfigOnStartUpOrBurn());
	// TunerStudio writes directly to the live settings page before burn validation.
	engineConfiguration->injector.flow = 0;
	auto& injector = engine->module<InjectorModelPrimary>().unmock();
	injector.prepare();
	EXPECT_TRUE(std::isinf(injector.getInjectionDuration(0.01f)));
	EXPECT_FATAL_ERROR(validateConfigOnStartUpOrBurn());
}

TEST(InjectorFlowRegression, SecondaryZeroFlowRejectedOnlyWhenStaged) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->injectorSecondary.flow = 0;
	engineConfiguration->enableStagedInjection = false;
	ASSERT_TRUE(validateConfigOnStartUpOrBurn());
	engineConfiguration->enableStagedInjection = true;
	EXPECT_FATAL_ERROR(validateConfigOnStartUpOrBurn());
}

namespace {

void configureReviewTrailingSpark() {
	engineConfiguration->cylindersCount = 1;
	engineConfiguration->firingOrder = FO_1;
	engineConfiguration->ignitionMode = IM_ONE_COIL;
	engineConfiguration->isInjectionEnabled = false;
	engineConfiguration->isIgnitionEnabled = true;
	engineConfiguration->enableTrailingSparks = true;
	engine->updateTriggerConfiguration();
	engine->rpmCalculator.setRpmValue(1000);
	setArrayValues(config->sparkDwellValues, 3.0f);
	setArrayValues(config->dwellVoltageCorrValues, 1.0f);
	engine->ignitionState.updateDwell(1000, false);
	setTable(config->trailingSparkTable, 6);
	engine->ignitionState.trailingSparkAngle = 6;
	setArrayValues(engine->engineState.timingAdvance, -30.0f);
	initializeIgnitionActions();
}

void checkTrailingSparkDisable(bool disableDuringDwell) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	configureReviewTrailingSpark();
	const auto startUs = getTimeNowUs();
	onTriggerEventSparkLogic(1000, getTimeNowNt(), 0, 90);
	eth.setTimeAndInvokeEventsUs(startUs + 3100);
	ASSERT_TRUE(enginePins.coils[0].getLogicValue());
	ASSERT_TRUE(enginePins.trailingCoils[0].getLogicValue());
	if (disableDuringDwell) {
		engineConfiguration->enableTrailingSparks = false;
	}
	eth.setTimeAndInvokeEventsUs(startUs + 7000);
	EXPECT_FALSE(enginePins.coils[0].getLogicValue());
	EXPECT_EQ(disableDuringDwell, enginePins.trailingCoils[0].getLogicValue());
	EXPECT_EQ(0, engine->scheduler.size());
	// With the feature disabled there is no future callback to discharge this
	// coil. Time alone does not recover it; the final stopped-engine sweep does.
	eth.setTimeAndInvokeEventsUs(startUs + 100000);
	EXPECT_EQ(disableDuringDwell, enginePins.trailingCoils[0].getLogicValue());
	engine->onEngineHasStopped();
	EXPECT_FALSE(enginePins.trailingCoils[0].getLogicValue());
}

} // namespace

TEST(EcuLogicReview, DisablingTrailingSparkDuringDwellLeavesCoilCharged) {
	checkTrailingSparkDisable(true);
}

TEST(IgnitionReview, UnchangedTrailingSparkDischargesBothCoils) {
	checkTrailingSparkDisable(false);
}

TEST(EcuLogicReview, MultisparkHoldsTrailingCoilUntilLastRestrike) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	configureReviewTrailingSpark();
	engineConfiguration->multisparkEnable = true;
	engineConfiguration->multisparkMaxRpm = 3000;
	engineConfiguration->multisparkMaxSparkingAngle = 60;
	engineConfiguration->multisparkMaxExtraSparkCount = 3;
	engineConfiguration->multisparkSparkDuration = 1;
	engineConfiguration->multisparkDwell = 2;
	engine->engineState.multispark.count = getMultiSparkCount(1000);
	ASSERT_EQ(3u, engine->engineState.multispark.count);
	const auto startUs = getTimeNowUs();
	onTriggerEventSparkLogic(1000, getTimeNowNt(), 0, 90);
	eth.setTimeAndInvokeEventsUs(startUs + 3100);
	ASSERT_TRUE(enginePins.trailingCoils[0].getLogicValue());
	eth.setTimeAndInvokeEventsUs(startUs + 5001);
	ASSERT_FALSE(enginePins.coils[0].getLogicValue());
	// Main spark at 5 ms; subsequent sparks at 8, 11, 14 ms. Trailing
	// coil begins dwelling at 3 ms but is never discharged between them.
	for (int restrike = 0; restrike < 3; restrike++) {
		SCOPED_TRACE(restrike);
		eth.setTimeAndInvokeEventsUs(startUs + 6100 + restrike * 3000);
		EXPECT_TRUE(enginePins.coils[0].getLogicValue());
		EXPECT_TRUE(enginePins.trailingCoils[0].getLogicValue());
		eth.setTimeAndInvokeEventsUs(startUs + 8100 + restrike * 3000);
		EXPECT_FALSE(enginePins.coils[0].getLogicValue());
		EXPECT_TRUE(enginePins.trailingCoils[0].getLogicValue());
	}
	eth.setTimeAndInvokeEventsUs(startUs + 14900);
	EXPECT_TRUE(enginePins.trailingCoils[0].getLogicValue());
	eth.setTimeAndInvokeEventsUs(startUs + 15100);
	EXPECT_FALSE(enginePins.trailingCoils[0].getLogicValue());
	EXPECT_EQ(0, engine->scheduler.size());
	// The real callbacks hold trailing dwell for 12 ms with a 3-ms main
	// request and 2-ms restrikes. There is no trailing overdwell watchdog.
}

TEST(EcuLogicReview, LeanSecondBankDoesNotTriggerLambdaProtection) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->lambdaProtectionEnable = true;
	engineConfiguration->lambdaProtectionMinRpm = 2000;
	engineConfiguration->lambdaProtectionMinLoad = 60;
	engineConfiguration->lambdaProtectionMinTps = 20;
	engineConfiguration->lambdaProtectionTimeout = 0.5f;
	engineConfiguration->lambdaProtectionRestoreRpm = 1500;
	engineConfiguration->lambdaProtectionRestoreLoad = 30;
	engineConfiguration->lambdaProtectionRestoreTps = 5;
	engineConfiguration->cylinderBankSelect[2] = 1;
	engineConfiguration->cylinderBankSelect[3] = 1;
	engineConfiguration->noFuelTrimAfterDfcoTime = 0;
	setTable(config->lambdaMaxDeviationTable, 0.1f);
	engine->rpmCalculator.setRpmValue(3000);
	engine->fuelComputer.targetLambda = 1;
	Sensor::setMockValue(SensorType::Tps1, 80);
	Sensor::setMockValue(SensorType::Lambda1, 0.95f);
	Sensor::setMockValue(SensorType::Lambda2, 1.3f);
	auto& limp = engine->module<LimpManager>().unmock();
	advanceTimeUs(3000000); // Exceed the inhibit time after any previous cut.
	limp.updateState(3000, getTimeNowNt());
	ASSERT_TRUE(limp.allowInjection());
	auto& monitor = engine->lambdaMonitor;
	monitor.update(3000, 100);
	ASSERT_TRUE(monitor.lambdaCurrentlyGood);
	advanceTimeUs(600000);
	monitor.update(3000, 100);
	limp.updateState(3000, getTimeNowNt());
	EXPECT_FALSE(monitor.isCut());
	EXPECT_TRUE(limp.allowInjection());

	// Identical lean feedback on bank 1 causes the configured fuel cut.
	Sensor::setMockValue(SensorType::Lambda1, 1.3f);
	monitor.update(3000, 100);
	ASSERT_FALSE(monitor.lambdaCurrentlyGood);
	ASSERT_FALSE(monitor.isCut());
	advanceTimeUs(600000);
	monitor.update(3000, 100);
	limp.updateState(3000, getTimeNowNt());
	EXPECT_TRUE(monitor.isCut());
	EXPECT_EQ(ClearReason::LambdaProtection, limp.allowInjection().reason);

	// A lift below all configured restore thresholds clears the cut.
	Sensor::setMockValue(SensorType::Tps1, 0);
	monitor.update(1000, 20);
	limp.updateState(1000, getTimeNowNt());
	EXPECT_FALSE(monitor.isCut());
	EXPECT_TRUE(limp.allowInjection());
}

TEST(EcuLogicReview, LuaResetRetainsSparkSkipAndPreventsDwell) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	configureReviewTrailingSpark();
	engineConfiguration->enableTrailingSparks = false;
	testLuaExecString("setSparkSkipRatio(0.25); setSparkHardSkipRatio(1)");
	ASSERT_FLOAT_EQ(1, engine->hardSparkLimiter.getTargetSkipRatio());

	// This is the reset called after the old Lua VM is destroyed on reload.
	engine->resetLua();
	for (int i = 0; i < 10; i++) {
		engine->engineState.updateSparkSkip();
	}
	EXPECT_FLOAT_EQ(0.25f, engine->softSparkLimiter.getTargetSkipRatio());
	EXPECT_FLOAT_EQ(1, engine->hardSparkLimiter.getTargetSkipRatio());
	onTriggerEventSparkLogic(1000, getTimeNowNt(), 0, 90);
	EXPECT_TRUE(engine->ignitionState.luaIgnitionSkip);
	EXPECT_EQ(0, engine->scheduler.size());
	EXPECT_FALSE(enginePins.coils[0].getLogicValue());

	// Only a replacement script's explicit clear restores scheduled ignition.
	testLuaExecString("setSparkSkipRatio(0); setSparkHardSkipRatio(0)");
	const auto startUs = getTimeNowUs();
	onTriggerEventSparkLogic(1000, getTimeNowNt(), 0, 90);
	ASSERT_EQ(2, engine->scheduler.size());
	eth.setTimeAndInvokeEventsUs(startUs + 2500);
	EXPECT_TRUE(enginePins.coils[0].getLogicValue());
	eth.setTimeAndInvokeEventsUs(startUs + 6000);
	EXPECT_FALSE(enginePins.coils[0].getLogicValue());
	EXPECT_EQ(0, engine->scheduler.size());
}

TEST(EcuLogicReview, LuaResetRetainsLaunchFuelAndSparkRequests) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->launchControlEnabled = true;
	engineConfiguration->launchActivationMode = LUA_LAUNCH;
	engineConfiguration->launchRpm = 3000;
	engineConfiguration->launchRpmThreshold = 0;
	engineConfiguration->launchSpeedThreshold = 0;
	engineConfiguration->launchTpsThreshold = 5;
	engineConfiguration->launchSparkCutEnable = true;
	engineConfiguration->launchFuelAdderPercent = 20;
	engine->rpmCalculator.setRpmValue(3500);
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 80);
	testLuaExecString("setLaunchTrigger(1)");
	engine->launchController.update();
	ASSERT_TRUE(engine->launchController.isLaunchCondition);
	engine->resetLua();
	engine->launchController.update();
	engine->engineState.updateSparkSkip();
	EXPECT_TRUE(engine->launchController.isLaunchCondition);
	EXPECT_FLOAT_EQ(1, engine->hardSparkLimiter.getTargetSkipRatio());
	EXPECT_FLOAT_EQ(1.2f, engine->launchController.getFuelCoefficient());

	testLuaExecString("setLaunchTrigger(0)");
	engine->launchController.update();
	engine->engineState.updateSparkSkip();
	EXPECT_FALSE(engine->launchController.isLaunchCondition);
	EXPECT_FLOAT_EQ(0, engine->hardSparkLimiter.getTargetSkipRatio());
	EXPECT_FLOAT_EQ(1, engine->launchController.getFuelCoefficient());
}

TEST(EcuLogicReview, FixedTimingRetainsReleasedLaunchSparkCut) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->cylindersCount = 1;
	engineConfiguration->firingOrder = FO_1;
	engineConfiguration->ignitionMode = IM_ONE_COIL;
	engineConfiguration->enableTrailingSparks = false;
	engineConfiguration->timingMode = TM_DYNAMIC;
	engineConfiguration->launchControlEnabled = true;
	engineConfiguration->launchActivationMode = LUA_LAUNCH;
	engineConfiguration->launchRpm = 3000;
	engineConfiguration->launchRpmThreshold = 0;
	engineConfiguration->launchSpeedThreshold = 0;
	engineConfiguration->launchTpsThreshold = 5;
	engineConfiguration->launchSparkCutEnable = true;
	engine->updateTriggerConfiguration();
	engine->rpmCalculator.setRpmValue(3500);
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 80);
	testLuaExecString("setLaunchTrigger(1)");
	engine->periodicFastCallback();
	ASSERT_FLOAT_EQ(1, engine->hardSparkLimiter.getTargetSkipRatio());

	engineConfiguration->timingMode = TM_FIXED;
	engineConfiguration->fixedTiming = 10;
	testLuaExecString("setLaunchTrigger(0)");
	for (int i = 0; i < 10; i++) engine->periodicFastCallback();
	ASSERT_FALSE(engine->launchController.isLaunchCondition);
	ASSERT_FLOAT_EQ(0, engine->launchController.getSparkSkipRatio());
	EXPECT_FLOAT_EQ(1, engine->hardSparkLimiter.getTargetSkipRatio());

	// Use a known dwell/spark plan to observe the retained skip at its consumer.
	engine->ignitionState.sparkDwell = 3;
	engine->ignitionState.dwellDurationAngle = 63;
	setArrayValues(engine->engineState.timingAdvance, -90.0f);
	initializeIgnitionActions();
	onTriggerEventSparkLogic(3500, getTimeNowNt(), 0, 180);
	EXPECT_EQ(0, engine->scheduler.size());
	EXPECT_FALSE(enginePins.coils[0].getLogicValue());

	// Dynamic timing reaches the skip refresh and restores normal scheduling.
	engineConfiguration->timingMode = TM_DYNAMIC;
	engine->periodicFastCallback();
	ASSERT_FLOAT_EQ(0, engine->hardSparkLimiter.getTargetSkipRatio());
	engine->ignitionState.sparkDwell = 3;
	engine->ignitionState.dwellDurationAngle = 63;
	setArrayValues(engine->engineState.timingAdvance, -90.0f);
	initializeIgnitionActions();
	onTriggerEventSparkLogic(3500, getTimeNowNt(), 0, 180);
	ASSERT_EQ(2, engine->scheduler.size());
	eth.moveTimeForwardUs(2000);
	ASSERT_EQ(1, eth.executeActions());
	EXPECT_TRUE(enginePins.coils[0].getLogicValue());
	eth.moveTimeForwardUs(3000);
	ASSERT_EQ(1, eth.executeActions());
	EXPECT_FALSE(enginePins.coils[0].getLogicValue());
}

TEST(EcuLogicReview, LuaResetRetainsAntilagThrottleOpening) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->antiLagEnabled = true;
	engineConfiguration->antiLagActivationMode = LUA_ANTILAG;
	engineConfiguration->ALSMinRPM = 400;
	engineConfiguration->ALSMaxRPM = 3200;
	engineConfiguration->ALSMinCLT = 50;
	engineConfiguration->ALSMaxCLT = 105;
	engineConfiguration->ALSMaxTPS = 5;
	engineConfiguration->ALSMaxDuration = 0;
	engineConfiguration->ALSEtbAdd = 20;
	engineConfiguration->etbIdleThrottleRange = 0;
	engineConfiguration->etbMinimumPosition = 1;
	engineConfiguration->etbMaximumPosition = 100;
	setTable(config->ALSFuelAdjustment, 50);
	Sensor::setMockValue(SensorType::Rpm, 2000);
	Sensor::setMockValue(SensorType::Clt, 80);
	Sensor::setMockValue(SensorType::Tps1, 10, true);
	Sensor::setMockValue(SensorType::AcceleratorPedal, 0, true);
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 0);
	StrictMock<MockVp3d> pedalMap;
	EXPECT_CALL(pedalMap, getValue(_, _)).WillRepeatedly(Return(0));
	StrictMock<MockMotor> motor;
	EtbController etb;
	ASSERT_TRUE(etb.init(DC_Throttle1, &motor, &engineConfiguration->etb, &pedalMap));
	testLuaExecString("setRollingIdleTrigger(1)");
	engine->antilagController.update();
	ASSERT_TRUE(engine->antilagController.isAntilagCondition);
	ASSERT_NEAR(20, etb.getSetpoint().value_or(-1), 0.01f);

	engine->resetLua();
	engine->antilagController.update();
	EXPECT_TRUE(engine->antilagController.isAntilagCondition);
	EXPECT_FLOAT_EQ(50, engine->antilagController.fuelALSCorrection);
	EXPECT_NEAR(20, etb.getSetpoint().value_or(-1), 0.01f);
	// A controller update with the retained request commands the motor open.
	EXPECT_CALL(motor, enable());
	EXPECT_CALL(motor, set(::testing::Gt(0.0f))).WillOnce(Return(false));
	etb.update();

	testLuaExecString("setRollingIdleTrigger(0)");
	engine->antilagController.update();
	EXPECT_FALSE(engine->antilagController.isAntilagCondition);
	EXPECT_FLOAT_EQ(0, engine->antilagController.fuelALSCorrection);
	EXPECT_NEAR(1, etb.getSetpoint().value_or(-1), 0.01f);
}
