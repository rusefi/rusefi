/*
 * @file test_spark_outputs.cpp
 *
 * End-to-end spark output behavior: a real trigger wheel is spun through the trigger
 * decoder and every coil charge/fire is recorded and checked.
 */

#include "pch.h"

#include <cmath>
#include <vector>

namespace {

struct CoilEdge {
	efitick_t timeNt;
	int cylinderIndex;
	bool charging;
	// state of the coil output just before this edge was applied
	bool pinWasHigh;
};

// Spins a crank-mounted 36-2 wheel at constant speed, executing scheduled actions and the
// periodic fast callback in between the teeth.
class Wheel36_2 {
public:
	Wheel36_2(EngineTestHelper& eth, float rpm)
		: m_eth(eth)
		, m_toothUs(60e6 / rpm / TOOTH_COUNT)
		, m_startUs(getTimeNowUs())
	{
	}

	void runRevolutions(int count) {
		for (int i = 0; i < count * TOOTH_COUNT; i++) {
			if (m_tooth % TOOTH_COUNT < TOOTH_COUNT - 2) {
				m_eth.setTimeAndInvokeEventsUs(timeOfToothUs(m_tooth));
				m_eth.firePrimaryTriggerRise();
				m_eth.setTimeAndInvokeEventsUs(timeOfToothUs(m_tooth + 0.5));
				m_eth.firePrimaryTriggerFall();
			}
			m_tooth++;
		}
		m_eth.setTimeAndInvokeEventsUs(timeOfToothUs(m_tooth) - 1);
	}

	// Crank position, in degrees since the wheel started spinning
	double angleAt(efitick_t timeNt) const {
		return (NT2US(timeNt) - m_startUs) / m_toothUs * (360.0 / TOOTH_COUNT);
	}

private:
	static constexpr int TOOTH_COUNT = 36;

	int timeOfToothUs(double tooth) const {
		return m_startUs + static_cast<int>(std::lround(tooth * m_toothUs));
	}

	EngineTestHelper& m_eth;
	const double m_toothUs;
	const int m_startUs;
	int m_tooth = 0;
};

// Angular distance between two crank positions, for events repeating every 'period' degrees
double phaseDifference(double a, double b, double period) {
	double diff = std::fmod(a - b, period);
	if (diff > period / 2) {
		diff -= period;
	} else if (diff < -period / 2) {
		diff += period;
	}
	return diff;
}

} // namespace

// Advance jumping by more than the dwell angle in one step, as when the idle timing controller
// switches from its lower to its upper limit on entering idle (1450 RPM: 12 to 35 degrees with
// about 16 degrees of dwell). Every coil must keep firing exactly once per charge and at the
// requested angle, and no spark may stay queued by angle after it has fired some other way:
// that is reported as CUSTOM_RE_ADDING_INTO_EXECUTION_QUEUE and can fire a coil twice.
// [tag:overdwell]
TEST(SparkOutputs, AdvanceStepLargerThanDwellAngle) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setCrankOperationMode();
	setupSimpleTestEngineWithMaf(&eth, IM_SEQUENTIAL, trigger_type_e::TT_TOOTHED_WHEEL_36_2);
	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
	engineConfiguration->isIgnitionEnabled = true;
	engineConfiguration->isInjectionEnabled = false;
	setArrayValues(config->sparkDwellValues, 1.9f);

	std::vector<CoilEdge> edges;
	engine->onIgnitionEvent = [&](IgnitionEvent* event, bool charging) {
		edges.push_back({
			getTimeNowNt(),
			static_cast<int>(event->cylinderIndex),
			charging,
			event->outputs[0]->getLogicValue()
		});
	};

	constexpr float rpm = 1450;
	constexpr float oldAdvance = 12;
	constexpr float newAdvance = 35;
	// each cylinder's ignition event is scheduled once per 720 degree engine cycle; in wasted
	// spark the coil's other spark of the cycle belongs to the paired cylinder's event
	constexpr double sparkPeriod = 720;
	constexpr int cylinderCount = 4;

	Wheel36_2 wheel(eth, rpm);
	setWholeTimingTable(oldAdvance);
	wheel.runRevolutions(20);

	ASSERT_NEAR(rpm, Sensor::getOrZero(SensorType::Rpm), 1);
	ASSERT_TRUE(engine->triggerCentral.triggerState.getShaftSynchronized());

	const float dwellMs = engine->ignitionState.getDwell();
	const float dwellAngle = dwellMs * rpm * 360 / 60e3f;
	ASSERT_LT(dwellAngle, newAdvance - oldAdvance) << "the step has to be bigger than the dwell angle";

	// where each cylinder sparks before the step: the reference for the angle check below
	double oldSparkAngle[cylinderCount];
	bool hasOldSparkAngle[cylinderCount] = {};
	for (const auto& edge : edges) {
		if (!edge.charging) {
			oldSparkAngle[edge.cylinderIndex] = wheel.angleAt(edge.timeNt);
			hasOldSparkAngle[edge.cylinderIndex] = true;
		}
	}

	const efitick_t from = getTimeNowNt();
	const uint32_t triggerErrorsBefore = engine->triggerCentral.triggerState.totalTriggerErrorCounter;
	const int syncCounterBefore = engine->triggerCentral.triggerState.getSynchronizationCounter();
	const auto overdwellSparksBefore = engine->engineState.overDwellCanceledCounter;
	const int warningsBefore = eth.getWarningCounter();

	constexpr int revolutionsToSettle = 4;
	constexpr int revolutionsSettled = 8;
	setWholeTimingTable(newAdvance);
	wheel.runRevolutions(revolutionsToSettle);
	const efitick_t settled = getTimeNowNt();
	wheel.runRevolutions(revolutionsSettled);

	// Each prepared spark keeps the dwell and spark angles it was planned with as a pair, so
	// the step is taken up between two sparks and the overdwell backup has nothing to do
	EXPECT_EQ(overdwellSparksBefore, engine->engineState.overDwellCanceledCounter);

	// Nothing re-added to the angle queue
	EXPECT_FALSE(hasRecentWarningCode(ObdCode::CUSTOM_RE_ADDING_INTO_EXECUTION_QUEUE));
	EXPECT_EQ(warningsBefore, eth.getWarningCounter());

	EXPECT_EQ(triggerErrorsBefore, engine->triggerCentral.triggerState.totalTriggerErrorCounter);
	EXPECT_TRUE(engine->triggerCentral.triggerState.getShaftSynchronized());
	EXPECT_EQ(syncCounterBefore + revolutionsToSettle + revolutionsSettled,
		engine->triggerCentral.triggerState.getSynchronizationCounter());

	int sparkCount[cylinderCount] = {};
	int settledSparkCount[cylinderCount] = {};
	efitick_t chargeStart[cylinderCount] = {};
	bool isCharging[cylinderCount] = {};

	for (const auto& edge : edges) {
		const int cylinder = edge.cylinderIndex;
		ASSERT_LT(cylinder, cylinderCount);

		if (edge.charging) {
			EXPECT_FALSE(isCharging[cylinder]) << "cylinder " << cylinder << " charged twice";
			isCharging[cylinder] = true;
			chargeStart[cylinder] = edge.timeNt;
			continue;
		}

		// Nothing fired twice, and no spark without charge
		EXPECT_TRUE(isCharging[cylinder] && edge.pinWasHigh)
			<< "cylinder " << cylinder << " fired without charge at " << wheel.angleAt(edge.timeNt);
		isCharging[cylinder] = false;

		if (edge.timeNt < from) {
			continue;
		}

		sparkCount[cylinder]++;
		const float actualDwellMs = NT2US(edge.timeNt - chargeStart[cylinder]) / 1000.0f;
		EXPECT_GT(actualDwellMs, 0.75f * dwellMs) << "cylinder " << cylinder;
		EXPECT_LT(actualDwellMs, 1.8f * dwellMs) << "cylinder " << cylinder;

		if (edge.timeNt >= settled) {
			// Once the step has gone through, every spark is where it was asked for
			settledSparkCount[cylinder]++;
			ASSERT_TRUE(hasOldSparkAngle[cylinder]);
			EXPECT_NEAR(
				newAdvance - oldAdvance,
				phaseDifference(oldSparkAngle[cylinder], wheel.angleAt(edge.timeNt), sparkPeriod),
				0.5
			) << "cylinder " << cylinder;
		}
	}

	for (int cylinder = 0; cylinder < cylinderCount; cylinder++) {
		EXPECT_NEAR((revolutionsToSettle + revolutionsSettled) / 2, sparkCount[cylinder], 1) << "cylinder " << cylinder;
		EXPECT_NEAR(revolutionsSettled / 2, settledSparkCount[cylinder], 1) << "cylinder " << cylinder;
	}

	engine->onIgnitionEvent = nullptr;
}
