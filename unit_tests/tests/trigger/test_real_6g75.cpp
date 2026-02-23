#include "pch.h"

#include "logicdata_csv_reader.h"

TEST(real6g75, real) {
    CsvReader reader(/*triggerCount*/ 1, /* vvtCount */ 1);

    reader.open("tests/trigger/resources/6g75-without-spark-crank.csv", NORMAL_ORDER, NORMAL_ORDER);

    EngineTestHelper eth(engine_type_e::TEST_ENGINE);
    setVerboseTrigger(true);

    engineConfiguration->vvtMode[0] = VVT_INACTIVE;
    eth.setTriggerType(trigger_type_e::TT_36_2_1_1);
}