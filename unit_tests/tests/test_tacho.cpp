#include "engine_test_helper.h"

TEST(tachometer, testThreePulsePerRev) {
    // This engine has a tach pin set - we need that
    WITH_ENGINE_TEST_HELPER(BMW_E34);

    // We don't actually care about ign/inj at all, just tach
    engineConfiguration->isInjectionEnabled = false;
    engineConfiguration->isIgnitionEnabled = false;

    // Configure tach pulse count
    // 5 PPR, 25% duty
    engineConfiguration->tachPulsePerRev = 4;
    engineConfiguration->tachPulseDuractionMs = 0.25f;
    engineConfiguration->tachPulseDurationAsDutyCycle = true;
    engineConfiguration->tachPulseTriggerIndex = 0;

    // Set predictable trigger settings
    engineConfiguration->trigger.customTotalToothCount = 8;
    engineConfiguration->trigger.customSkippedToothCount = 0;
    engineConfiguration->useOnlyRisingEdgeForTrigger = false;
    engineConfiguration->ambiguousOperationMode = FOUR_STROKE_CAM_SENSOR;
	eth.applyTriggerWaveform();

    // get the engine running - 6 revolutions
    eth.fireTriggerEvents(48);

    // ensure engine speed and position
    std::cerr << "[          ] RPM = " << GET_RPM() << std::endl;
	ASSERT_EQ(1500,  GET_RPM()) << "RPM";
    std::cerr << "[          ] Current Index = " << engine->triggerCentral.triggerState.getCurrentIndex() << std::endl;
	ASSERT_EQ(15,  engine->triggerCentral.triggerState.getCurrentIndex()) << "index #1";
    std::cerr << "[          ] isSynced = " << engine->triggerCentral.triggerState.shaft_is_synchronized << std::endl;
    ASSERT_EQ(engine->triggerCentral.triggerState.shaft_is_synchronized, true);

    // Clean the slate
	eth.clearQueue();

	eth.moveTimeForwardMs(5 /*ms*/);

    // This will schedule tach events!
    eth.firePrimaryTriggerRise();
    std::cerr << "[          ] RPM = " << eth.engine.rpmCalculator.rpmValue << std::endl;
    ASSERT_EQ(1500, eth.engine.rpmCalculator.rpmValue) << "RPM";

    // Now we verify that the correct stuff got scheduled
    float enginePeriod = 40000; // us
    float tachPeriod = enginePeriod / 4;
    float tachHighTime = tachPeriod * 0.25f;

    // Build the times we expect the edges to have occured
    float events[16];
    for (int i = 0; i < 8; i++) {
        auto periodStart = i * tachPeriod;
        events[2 * i] = periodStart;
        events[2 * i + 1] = periodStart + tachHighTime;
    }

    // Check that exactly 16 events got scheduled
    std::cerr << "[          ] executor.size = " << engine->executor.size() << std::endl;
    ASSERT_EQ(engine->executor.size(), 16);

    auto start = eth.getTimeNowUs();

    // Check that everything is in the right spot
    for (int i = 0; i < 16; i++)
    {
        auto s = engine->executor.getForUnitTest(i);
        ASSERT_NE(s, nullptr);

        EXPECT_NEAR(start + events[i], s->momentX, 1);
    }
}