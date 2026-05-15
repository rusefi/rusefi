#include "pch.h"

#include "logicdata_csv_reader.h"

extern int tooManyTeethCounter;

TEST(realChryslerPhaser, phaser_cam_1) {
    tooManyTeethCounter = 0;
    CsvReader reader(/*triggerCount*/ 1, /* vvtCount */ 0);
    reader.open("tests/trigger/resources/chrysler-phaser.csv");

    EngineTestHelper eth(engine_type_e::TEST_ENGINE);

    engineConfiguration->vvtMode[0] = VVT_INACTIVE;
    eth.setTriggerType(trigger_type_e::TT_CHRYSLER_PHASER);

    while (reader.haveMore()) {
        reader.processLine(&eth);
    }

    ASSERT_EQ(187, tooManyTeethCounter);
    
    ASSERT_EQ(0, eth.recentWarnings()->getCount());

    ASSERT_NEAR(0, Sensor::getOrZero(SensorType::Rpm), 0.1);
}


TEST(realChryslerPhaser, phaser_cam_2) {
    tooManyTeethCounter = 0;
    CsvReader reader(/*triggerCount*/ 1, /* vvtCount */ 1);
    reader.open("tests/trigger/resources/chrysler_phaser_good392log.csv");

    EngineTestHelper eth(engine_type_e::TEST_ENGINE);

    engineConfiguration->vvtMode[0] = VVT_INACTIVE;
    eth.setTriggerType(trigger_type_e::TT_CHRYSLER_PHASER);

    while (reader.haveMore()) {
        reader.processLine(&eth);
    }

    ASSERT_EQ(3980, tooManyTeethCounter);
    
    ASSERT_EQ(1, eth.recentWarnings()->getCount());

    ASSERT_NEAR(0, Sensor::getOrZero(SensorType::Rpm), 0.1);
}