//
// Created by kifir on 12/19/24.
//

#include "pch.h"

#include "util/test_base.h"

#include "fuel_level_func.h"

namespace {
	constexpr float calculateNextFilteredValue(float currentFilteredValue, float newValue, float alpha) {
		return currentFilteredValue + (newValue - currentFilteredValue) * alpha;
	}

	class FuelLevelFuncTest: public TestBase<> {
	protected:
		static constexpr float TEST_FUEL_LEVEL_UPDATE_PERIOD_SEC = 0.932f;
		static constexpr float TEST_FUEL_LEVEL_ALPHA = 0.17f;

		static constexpr float INPUT_VALUE_0 = 51.932f;
		static constexpr float EXPECTED_FILTERED_VALUE_0 = INPUT_VALUE_0;
		static constexpr uint8_t EXPECTED_FUEL_LEVEL_0 = 89;

		static constexpr float INPUT_VALUE_1 = 1.932f;
		static constexpr float EXPECTED_FILTERED_VALUE_1 =
			calculateNextFilteredValue(EXPECTED_FILTERED_VALUE_0, INPUT_VALUE_1, TEST_FUEL_LEVEL_ALPHA);
		static constexpr uint8_t EXPECTED_FUEL_LEVEL_1 = 78;

		static constexpr float INPUT_VALUE_2 = INPUT_VALUE_1;
		static constexpr float EXPECTED_FILTERED_VALUE_2 =
			calculateNextFilteredValue(EXPECTED_FILTERED_VALUE_1, INPUT_VALUE_2, TEST_FUEL_LEVEL_ALPHA);
		static constexpr uint8_t EXPECTED_FUEL_LEVEL_2 = 67;

		static const FuelLevelBinsCurve TEST_FUEL_LEVEL_BINS;
		static const FuelLevelValuesCurve TEST_FUEL_LEVEL_VALUES;

		void SetUp() override;

		float convert(float newValue);

		void checkThatNewValueIsIgnoredDuringUpdatePeriod(float previousValue);
	private:
		static FuelLevelBinsCurve getTestFuelLevelBins();
		static FuelLevelValuesCurve getTestFuelLevelValues();
		std::unique_ptr<FuelLevelFunc> m_fuelLevelFunc;
	};

	const FuelLevelBinsCurve FuelLevelFuncTest::TEST_FUEL_LEVEL_BINS = FuelLevelFuncTest::getTestFuelLevelBins();
	const FuelLevelValuesCurve FuelLevelFuncTest::TEST_FUEL_LEVEL_VALUES = FuelLevelFuncTest::getTestFuelLevelValues();

	void FuelLevelFuncTest::SetUp() {
		TestBase::SetUp();

		setUpEngineConfiguration(EngineConfig()
				                         .setFuelLevelUpdatePeriodSec(TEST_FUEL_LEVEL_UPDATE_PERIOD_SEC)
				                         .setFuelLevelAveragingAlpha(TEST_FUEL_LEVEL_ALPHA)
		);

		getTestPersistentConfiguration().setFuelLevelBinsCurve(TEST_FUEL_LEVEL_BINS);
		getTestPersistentConfiguration().setFuelLevelValuesCurve(TEST_FUEL_LEVEL_VALUES);

		m_fuelLevelFunc = std::make_unique<FuelLevelFunc>();
	}

	float FuelLevelFuncTest::convert(const float newValue) {
		const SensorResult result = m_fuelLevelFunc->convert(newValue);
		EXPECT_TRUE(result.Valid);
		return result.Value;
	}

	void FuelLevelFuncTest::checkThatNewValueIsIgnoredDuringUpdatePeriod(const float previousValue) {
		advanceTimeUs(1);
		EXPECT_EQ(convert(99.9f), previousValue);

		advanceTimeUs(static_cast<int>(TEST_FUEL_LEVEL_UPDATE_PERIOD_SEC * 1000000.0f) - 1);
		EXPECT_EQ(convert(11.1f), previousValue);
	}

	FuelLevelBinsCurve FuelLevelFuncTest::getTestFuelLevelBins() {
		FuelLevelBinsCurve result;
		const long binsCount = result.size();
		float nextBin = 0.0f;
		// Initialize first bins with arbitrary increasing values:
		for (int i = 0; i < binsCount - 3; i++) {
			nextBin += 0.1f;
			result[i] = nextBin;
		}
		result[binsCount - 3] = EXPECTED_FILTERED_VALUE_2;
		result[binsCount - 2] = EXPECTED_FILTERED_VALUE_1;
		result[binsCount - 1] = EXPECTED_FILTERED_VALUE_0;

		// Validate bins - they should be increasing
		float latestBin = 0.0f;
		for (const float bin: result) {
			EXPECT_TRUE(latestBin < bin);
			latestBin = bin;
		}

		return result;
	}

	FuelLevelValuesCurve FuelLevelFuncTest::getTestFuelLevelValues() {
		FuelLevelValuesCurve result;
		result.fill(12);
		const long valuesCount = result.size();
		result[valuesCount - 3] = EXPECTED_FUEL_LEVEL_2;
		result[valuesCount - 2] = EXPECTED_FUEL_LEVEL_1;
		result[valuesCount - 1] = EXPECTED_FUEL_LEVEL_0;
		return result;
	}

	TEST_F(FuelLevelFuncTest, checkConversion) {
	EXPECT_EQ(convert(INPUT_VALUE_0), EXPECTED_FUEL_LEVEL_0);
	checkThatNewValueIsIgnoredDuringUpdatePeriod(EXPECTED_FUEL_LEVEL_0);

	advanceTimeUs(1);
	EXPECT_EQ(convert(INPUT_VALUE_1), EXPECTED_FUEL_LEVEL_1);
	checkThatNewValueIsIgnoredDuringUpdatePeriod(EXPECTED_FUEL_LEVEL_1);

	advanceTimeUs(1);
	EXPECT_EQ(convert(INPUT_VALUE_2), EXPECTED_FUEL_LEVEL_2);
	checkThatNewValueIsIgnoredDuringUpdatePeriod(EXPECTED_FUEL_LEVEL_2);
}
}