#include "pch.h"

#include "logicdata_csv_reader.h"

extern int tooManyTeethCounter;

TEST(real6g75without, real) {
    tooManyTeethCounter = 0;
    CsvReader reader(/*triggerCount*/ 1, /* vvtCount */ 0);

    reader.open("tests/trigger/resources/6g75-without-spark-crank.csv", NORMAL_ORDER, NORMAL_ORDER);

    EngineTestHelper eth(engine_type_e::TEST_ENGINE);
//    setVerboseTrigger(true);

    engineConfiguration->vvtMode[0] = VVT_INACTIVE;
    eth.setTriggerType(trigger_type_e::TT_36_2_1_1);

    while (reader.haveMore()) {
        reader.processLine(&eth);
    }

    ASSERT_EQ(3, tooManyTeethCounter);
    ASSERT_EQ(2, eth.recentWarnings()->getCount());
    ASSERT_NEAR(144.73, Sensor::getOrZero(SensorType::Rpm), 0.1);
}

TEST(real6g75, realWithSparkPlugs) {
    tooManyTeethCounter = 0;
    CsvReader reader(/*triggerCount*/ 1, /* vvtCount */ 0);

    reader.open("tests/trigger/resources/6g75-withsparkplugs-cranking.csv", NORMAL_ORDER, NORMAL_ORDER);

    EngineTestHelper eth(engine_type_e::TEST_ENGINE);
    setVerboseTrigger(true);

    engineConfiguration->vvtMode[0] = VVT_INACTIVE;
    eth.setTriggerType(trigger_type_e::TT_36_2_1_1);

    while (reader.haveMore()) {
        reader.processLine(&eth);
    }

    ASSERT_EQ(382, tooManyTeethCounter);
    ASSERT_EQ(1, eth.recentWarnings()->getCount());
    ASSERT_NEAR(143.40, Sensor::getOrZero(SensorType::Rpm), 0.1);
}