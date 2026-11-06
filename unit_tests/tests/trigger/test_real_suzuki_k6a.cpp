/**
 * @file test_real_suzuki_k6a.cpp
 *
 * Suzuki K6A distributor on an EA21R Cappuccino, see #7080.
 * Single VR pickup, 10 teeth per cam revolution in a 4-3-3 arrangement.
 *
 * Saleae Logic 1 captures of the raw VR signal while cranking, from #7080.
 * Each CSV is a section of the original recording, shifted to start at zero:
 *   suzuki-k6a-ea21r-cranking-no-spark-plugs: 3.45 s to 9.65 s, about 470 rpm
 *   suzuki-k6a-ea21r-cranking-with-spark-plugs: 0.90 s to 6.60 s, about 425 rpm
 *
 * The recordings are not clean. Some revolutions have a missing tooth, a tooth
 * about 20 degrees early, or a short burst of noise. Every trigger error counted
 * below happens in one of those revolutions.
 */

#include "pch.h"
#include "real_trigger_helper.h"

namespace {

struct K6aCrankingResult {
	int firstSyncLine = -1;
	// sync points reached with the expected tooth count since the previous one
	int fullCycles = 0;
	int longestRun = 0;
	float minRpm = 1e6;
	float maxRpm = 0;
	uint32_t errors = 0;
};

K6aCrankingResult runK6aCranking(const char* fileName) {
	RealTriggerHelper helper;
	const auto& state = engine->triggerCentral.triggerState;

	K6aCrankingResult result;
	int run = 0;
	int prevSyncCounter = 0;

	helper.runTest(fileName, trigger_type_e::TT_SUZUKI_K6A, /*triggerCount*/1, /*vvtCount*/0,
			/*flipOnRead*/false, /*timestampOffset*/0.0, [&](CsvReader& reader) {
		if (result.firstSyncLine < 0 && state.getShaftSynchronized()) {
			result.firstSyncLine = reader.lineIndex();
		}

		bool newError = state.totalTriggerErrorCounter != result.errors;
		if (newError) {
			result.errors = state.totalTriggerErrorCounter;
			run = 0;
		}

		// The counter also advances at a sync point with the wrong tooth count, so skip those
		int syncCounter = state.getSynchronizationCounter();
		if (syncCounter > prevSyncCounter && !newError) {
			result.fullCycles++;
			run++;
			result.longestRun = std::max(result.longestRun, run);

			float rpm = Sensor::getOrZero(SensorType::Rpm);
			result.minRpm = std::min(result.minRpm, rpm);
			result.maxRpm = std::max(result.maxRpm, rpm);
		}
		prevSyncCounter = syncCounter;
	});

	EXPECT_FALSE(engine->triggerCentral.triggerShape.shapeDefinitionError);
	return result;
}

} // namespace

TEST(realSuzukiK6A, waveform) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	eth.setTriggerType(trigger_type_e::TT_SUZUKI_K6A);
	const auto& shape = engine->triggerCentral.triggerShape;

	ASSERT_FALSE(shape.shapeDefinitionError);
	EXPECT_EQ(FOUR_STROKE_CAM_SENSOR, shape.getWheelOperationMode());
	EXPECT_EQ(SyncEdge::RiseOnly, shape.syncEdge);

	// Rising edges in crank degrees after the sync tooth, measured from the captures:
	// three-tooth groups at 0/30/86, four-tooth group at 0/30/87/115, groups 240 apart.
	static const float rises[] = { 0, 30, 86, 240, 270, 326, 480, 510, 567, 595 };
	std::vector<float> actual;
	for (size_t i = 0; i < shape.getLength(); i++) {
		int index = (shape.getTriggerWaveformSynchPointIndex() + i) % shape.getSize();
		if (shape.triggerSignalStates[index] == TriggerValue::RISE) {
			actual.push_back(engine->triggerCentral.triggerFormDetails.eventAngles[i]);
		}
	}
	ASSERT_EQ(efi::size(rises), actual.size());
	for (size_t i = 0; i < efi::size(rises); i++) {
		EXPECT_NEAR(rises[i], actual[i], 0.01f) << i;
	}

	EXPECT_NEAR(3.3f, shape.synchronizationRatioFrom[0], 0.001f);
	EXPECT_NEAR(6.0f, shape.synchronizationRatioTo[0], 0.001f);
	EXPECT_NEAR(0.3f, shape.synchronizationRatioFrom[1], 0.001f);
	EXPECT_NEAR(0.8f, shape.synchronizationRatioTo[1], 0.001f);
}

TEST(realSuzukiK6A, crankingWithoutSparkPlugs) {
	auto result = runK6aCranking("tests/trigger/resources/suzuki-k6a-ea21r-cranking-no-spark-plugs.csv");

	EXPECT_EQ(2u, result.errors);
	EXPECT_EQ(22, result.firstSyncLine);
	EXPECT_EQ(19, result.fullCycles);
	EXPECT_EQ(11, result.longestRun);
	EXPECT_GT(result.minRpm, 400);
	EXPECT_LT(result.maxRpm, 500);
}

TEST(realSuzukiK6A, crankingWithSparkPlugs) {
	auto result = runK6aCranking("tests/trigger/resources/suzuki-k6a-ea21r-cranking-with-spark-plugs.csv");

	EXPECT_EQ(3u, result.errors);
	EXPECT_EQ(21, result.firstSyncLine);
	EXPECT_EQ(12, result.fullCycles);
	EXPECT_EQ(8, result.longestRun);
	EXPECT_GT(result.minRpm, 400);
	EXPECT_LT(result.maxRpm, 500);
}
