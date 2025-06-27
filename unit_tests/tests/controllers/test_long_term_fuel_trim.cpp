/*
 * @file	test_long_term_fuel_trim.cpp
 *
 * @date Jun 6, 2025
 * @author Andrey Gusakov, 2025
 * @author Andrey Belomutskiy, (c) 2012-2025
 */

#include "pch.h"

#define ITERATE_TIME(time, action)	\
	for (size_t i = 0; i < (time * 1000 / FAST_CALLBACK_PERIOD_MS); i++) { \
		(action); \
	}

#define ASSERT_LTFT_RESULT(action, val0, val1)	\
	{ \
		ClosedLoopFuelResult clResult = action;	\
		ASSERT_FLOAT_EQ(clResult.banks[0], val0);	\
		ASSERT_FLOAT_EQ(clResult.banks[1], val1);	\
	}

TEST(LTFT, testLearning)
{
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	LtftState ltftState;
	LongTermFuelTrim ltft;

	// reset to zero
	ltftState.load();
	ltft.init(&ltftState);

	engineConfiguration->ltft.enabled = false;
	engineConfiguration->ltft.correctionEnabled = false;
	engineConfiguration->ltft.deadband = 0.5; // %
	engineConfiguration->ltft.maxAdd = 15.0; // %
	engineConfiguration->ltft.maxRemove = 5; // %
	engineConfiguration->ltft.timeConstant = 30; // seconds

	float rpm = config->veRpmBins[0];
	float load = config->veLoadBins[0];
	ClosedLoopFuelResult clInput;
	ClosedLoopFuelResult clResult;

	clInput.banks[0] = clInput.banks[1] = 1.5;
	// Run for 100 sec
	ITERATE_TIME(100, ltft.learn(clInput, rpm, load))

	// Disabled should not affect
	ASSERT_LTFT_RESULT(ltft.getTrims(rpm, load), 1.0, 1.0);

	// Enabling
	engineConfiguration->ltft.enabled = true;
	engineConfiguration->ltft.correctionEnabled = true;

	// Run for 100 sec with positive correction
	clInput.banks[0] = clInput.banks[1] = 1.5;
	ITERATE_TIME(100, ltft.learn(clInput, rpm, load))

	// test hiting maxAdd limit
	ASSERT_LTFT_RESULT(ltft.getTrims(rpm, load), 1.15, 1.15);

	// Other cells should not be affected
	ASSERT_LTFT_RESULT(ltft.getTrims(5000, 50), 1.0, 1.0);

	// Run for 100 sec with negative correction
	clInput.banks[0] = clInput.banks[1] = 0.5;
	ITERATE_TIME(100, ltft.learn(clInput, rpm, load))

	// test hiting maxRemove limit
	ASSERT_LTFT_RESULT(ltft.getTrims(rpm, load), 0.95, 0.95);

	// Other cells should not be affected
	ASSERT_LTFT_RESULT(ltft.getTrims(5000, 50), 1.0, 1.0);

#if 0
	rpm = config->veRpmBins[0] - (config->veRpmBins[1] - config->veRpmBins[0]) - 1;
	load = config->veLoadBins[0] - (config->veLoadBins[1] - config->veLoadBins[0]) - 1;
	// we are not learning, buts but still calculating correction if far outside table
	// test far outside
	ASSERT_LTFT_RESULT(ltft.getTrims(rpm, load), 1.0, 1.0);
	ASSERT_LTFT_RESULT(ltft.getTrims(10000, 1000), 1.0, 1.0);
#endif
}

#include "mlq_reader.h"

static constexpr bool verbose = false;

static constexpr float executor_dt = FAST_CALLBACK_PERIOD_MS * 0.001f;

TEST(LTFT, testRealLogLearning)
{
	EngineTestHelper eth(engine_type_e::FORD_ESCORT_GT);

	// Allow learning only
	engineConfiguration->ltft.enabled = true;
	engineConfiguration->ltft.correctionEnabled = false;
	engineConfiguration->ltft.deadband = 0.5; // %
	engineConfiguration->ltft.maxAdd = 15.0; // %
	engineConfiguration->ltft.maxRemove = 5; // %
	engineConfiguration->ltft.timeConstant = 30; // seconds

	BinarySensorReader reader;
	reader.openMlq("pretty-happy-reference.mlg");

	bool firstRun = true;
	float prevTime = 0;
	float dt = 0;

	while (!reader.eof()) {
		auto snapshot = reader.readBlock();

		float time = snapshot["Time"];

		if (!firstRun) {
			// TODO:
			// dt = time - prevTime;
			// eth.moveTimeForwardSec(dt);
			// eth.executeActions();

			dt += time - prevTime;

			size_t counter = 0;
			while (dt > executor_dt) {
				// TODO: adjust time
				eth.moveTimeForwardSec(executor_dt);
				// run the ignition math
				engine->periodicFastCallback();
				dt -= executor_dt;
				counter++;
			}

			if (verbose) {
				if (counter) {
					std::cout << time << ": periodicFastCallback() executed " << counter << " times\n";
				}

				printf("%.3f: CLT: %3.0f, RPM %3.0f, MAP %3.0f, Lambdas: %f %f\n",
					time,
					snapshot["CLT"],
					snapshot["RPM"],
					snapshot["MAP"],
					snapshot["Front Lambda"],
					snapshot["Rear Lambda"]);
			}
		}

		// TODO: find way to mock target AFR?

		// now update sensors with new snapshot
		Sensor::setMockValue(SensorType::Clt, snapshot["CLT"]);
		Sensor::setMockValue(SensorType::Lambda1, snapshot["Front Lambda"]);
		Sensor::setMockValue(SensorType::Lambda2, snapshot["Rear Lambda"]);
		Sensor::setMockValue(SensorType::Rpm, snapshot["RPM"]);
		Sensor::setMockValue(SensorType::Map, snapshot["MAP"]);

		prevTime = time;
		firstRun = false;
	}

	printf("LTFT test: miss: %d, hit %d, deadband %d\n",
		engine->module<LongTermFuelTrim>()->ltftCntMiss,
		engine->module<LongTermFuelTrim>()->ltftCntHit,
		engine->module<LongTermFuelTrim>()->ltftCntDeadband);
}