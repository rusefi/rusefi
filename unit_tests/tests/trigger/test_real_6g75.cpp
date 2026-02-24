#include "pch.h"

#include "logicdata_csv_reader.h"

TEST(real6g75, real) {
    CsvReader reader(/*triggerCount*/ 1, /* vvtCount */ 0);

    reader.open("tests/trigger/resources/6g75-without-spark-crank.csv", NORMAL_ORDER, NORMAL_ORDER);

    EngineTestHelper eth(engine_type_e::TEST_ENGINE);
    setVerboseTrigger(true);

    engineConfiguration->vvtMode[0] = VVT_INACTIVE;
    eth.setTriggerType(trigger_type_e::TT_36_2_1_1);

    while (reader.haveMore()) {
        reader.processLine(&eth);
    }

    ASSERT_EQ(2, eth.recentWarnings()->getCount());
    float rpm = Sensor::getOrZero(SensorType::Rpm);
    printf("Final RPM: %f\n", rpm);
    ASSERT_NEAR(144.73, rpm, 0.1);
}