#pragma once

#include "engine_test_helper.h"
#include "logicdata_csv_reader.h"
#include <functional>

class RealTriggerHelper {
public:
    RealTriggerHelper(engine_type_e engineType = engine_type_e::TEST_ENGINE)
        : eth(engineType)
    {
        engineConfiguration->isFasterEngineSpinUpEnabled = true;
        engineConfiguration->alwaysInstantRpm = true;
    }

    void runTest(const char* csvFileName, trigger_type_e triggerType, bool flipOnRead = false) {
        runTest(csvFileName, triggerType, /*triggerCount*/1, /*vvtCount*/0, flipOnRead);
    }

    void runTest(const char* csvFileName, trigger_type_e triggerType, size_t triggerCount, size_t vvtCount, bool flipOnRead = false, double timestampOffset = 0.0, std::function<void(CsvReader&)> onLine = nullptr, const int* triggerColumnIndeces = NORMAL_ORDER, const int* vvtColumnIndeces = NORMAL_ORDER) {
        eth.setTriggerType(triggerType);

        CsvReader reader(triggerCount, vvtCount, timestampOffset);
        reader.flipOnRead = flipOnRead;
        reader.open(csvFileName, triggerColumnIndeces, vvtColumnIndeces);

        bool gotRpm = false;

        while (reader.haveMore()) {
            reader.processLine(&eth);

            auto rpm = Sensor::getOrZero(SensorType::Rpm);
            if (!gotRpm && rpm) {
                gotRpm = true;
                if (expectedFirstRpm != -1) {
                    EXPECT_NEAR(rpm, expectedFirstRpm, 1);
                }
                if (expectedFirstRpmLineIndex != -1) {
                    EXPECT_EQ(reader.lineIndex(), expectedFirstRpmLineIndex);
                }
            }

            if (onLine) {
                onLine(reader);
            }
        }
    }

    int expectedFirstRpm = -1;
    int expectedFirstRpmLineIndex = -1;

    EngineTestHelper eth;
};
