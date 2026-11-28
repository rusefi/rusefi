#include "pch.h"

#include "engine_csv_reader.h"
#include "trigger_decoder.h"

extern int tooManyTeethCounter;

TEST(realChryslerPhaser, syntheticV6ReferenceEdges) {
    EngineTestHelper eth(engine_type_e::TEST_ENGINE);
    eth.setTriggerType(trigger_type_e::TT_CHRYSLER_PHASER_v6);
    const auto& shape = eth.engine.triggerCentral.triggerShape;
    ASSERT_FALSE(shape.shapeDefinitionError);
    ASSERT_EQ(24u, shape.getSize());
    EXPECT_EQ(SyncEdge::RiseOnly, shape.syncEdge);
    EXPECT_EQ(10, shape.getTriggerWaveformSynchPointIndex()); // rise at 358 crank degrees
    constexpr int rises[] = {19, 40, 118, 139, 238, 358, 379, 400, 478, 598, 619, 718};
    for (int i = 0; i < 12; i++) {
        EXPECT_FLOAT_EQ(rises[i], shape.getSwitchAngle(2 * i));
    }
}

TEST(realChryslerPhaser, syntheticV6AcquiresFromEveryTooth) {
    constexpr int gapBefore[] = {21, 21, 78, 21, 99, 120, 21, 21, 78, 120, 21, 99};
    for (int start = 0; start < 12; start++) {
        EngineTestHelper eth(engine_type_e::TEST_ENGINE);
        eth.setTriggerType(trigger_type_e::TT_CHRYSLER_PHASER_v6);
        TriggerDecoderBase decoder("Chrysler V6");
        efitick_t now = 0;
        auto& tc = eth.engine.triggerCentral;
        for (int n = 0; n < 48; n++) {
            int tooth = (start + n) % 12;
            now += US2NT(100 * gapBefore[tooth]);
            setTimeNowNt(now);
            decoder.decodeTriggerEvent("V6", tc.triggerShape, nullptr,
                tc.primaryTriggerConfiguration, SHAFT_PRIMARY_RISING, now);
            if (n >= 16) {
                ASSERT_TRUE(decoder.shaft_is_synchronized);
            }
            if (decoder.shaft_is_synchronized) {
                EXPECT_EQ(2 * ((tooth + 12 - 5) % 12), decoder.getCurrentIndex());
            }
        }
        EXPECT_EQ(0u, decoder.totalTriggerErrorCounter);
    }
}

TEST(realChryslerPhaser, phaser_cam_1) {
    tooManyTeethCounter = 0;
    EngineCsvReader reader(/*triggerCount*/ 1, /* vvtCount */ 0);
    reader.open("tests/trigger/resources/chrysler-phaser.csv");

    EngineTestHelper eth(engine_type_e::TEST_ENGINE);

    engineConfiguration->vvtMode[0] = VVT_INACTIVE;
    eth.setTriggerType(trigger_type_e::TT_CHRYSLER_PHASER_v8);

    while (reader.haveMore()) {
        reader.processLine(&eth);
    }

    ASSERT_EQ(6, tooManyTeethCounter);
    
    ASSERT_EQ(0, eth.recentWarnings()->getCount());

    ASSERT_NEAR(808.3, Sensor::getOrZero(SensorType::Rpm), 0.1);
}


TEST(realChryslerPhaser, phaser_cam_2) {
    tooManyTeethCounter = 0;
    EngineCsvReader reader(/*triggerCount*/ 1, /* vvtCount */ 0);
    reader.setReadingOffset(1);
    reader.open("tests/trigger/resources/chrysler_phaser_good392log.csv");

    EngineTestHelper eth(engine_type_e::TEST_ENGINE);

    engineConfiguration->vvtMode[0] = VVT_INACTIVE;
    eth.setTriggerType(trigger_type_e::TT_CHRYSLER_PHASER_v8);

    while (reader.haveMore()) {
        reader.processLine(&eth);
    }

    ASSERT_EQ(6, tooManyTeethCounter);
    
    ASSERT_EQ(0, eth.recentWarnings()->getCount());

    ASSERT_NEAR(774.4, Sensor::getOrZero(SensorType::Rpm), 0.1);
}
