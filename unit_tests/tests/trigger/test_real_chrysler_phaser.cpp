#include "pch.h"

#include "engine_csv_reader.h"

extern int tooManyTeethCounter;

TEST(realChryslerPhaser, phaser_cam_1) {
    tooManyTeethCounter = 0;
    EngineCsvReader reader(/*triggerCount*/ 1, /* vvtCount */ 0);
    reader.open("tests/trigger/resources/chrysler-phaser.csv");

    EngineTestHelper eth(engine_type_e::TEST_ENGINE);

    engineConfiguration->vvtMode[0] = VVT_INACTIVE;
    eth.setTriggerType(trigger_type_e::TT_CHRYSLER_PHASER);

    while (reader.haveMore()) {
        reader.processLine(&eth);
    }

    ASSERT_EQ(6, tooManyTeethCounter);
    
    // todo: address later: false positive CUSTOM_OBD_impossibly_short_INJECTION - injection events run with zero fuel mass in this test
    ASSERT_EQ(1, eth.recentWarnings()->getCount());
    ASSERT_EQ(ObdCode::CUSTOM_OBD_impossibly_short_INJECTION, eth.recentWarnings()->get(0).Code);

    ASSERT_NEAR(808.3, Sensor::getOrZero(SensorType::Rpm), 0.1);
}


TEST(realChryslerPhaser, phaser_cam_2) {
    tooManyTeethCounter = 0;
    EngineCsvReader reader(/*triggerCount*/ 1, /* vvtCount */ 0);
    reader.setReadingOffset(1);
    reader.open("tests/trigger/resources/chrysler_phaser_good392log.csv");

    EngineTestHelper eth(engine_type_e::TEST_ENGINE);

    engineConfiguration->vvtMode[0] = VVT_INACTIVE;
    eth.setTriggerType(trigger_type_e::TT_CHRYSLER_PHASER);

    while (reader.haveMore()) {
        reader.processLine(&eth);
    }

    ASSERT_EQ(6, tooManyTeethCounter);
    
    // todo: address later: false positive CUSTOM_OBD_impossibly_short_INJECTION - injection events run with zero fuel mass in this test
    ASSERT_EQ(1, eth.recentWarnings()->getCount());
    ASSERT_EQ(ObdCode::CUSTOM_OBD_impossibly_short_INJECTION, eth.recentWarnings()->get(0).Code);

    ASSERT_NEAR(774.4, Sensor::getOrZero(SensorType::Rpm), 0.1);
}