#include "engine_test_helper.h"

extern WarningCodeState unitTestWarningCodeState;

#ifdef INIT_IMPL
typedef struct
{
    uint8_t PulsePerRev;
    float PulseDurationMs;
    bool DurationAsDutyCycle;
    int PulseTriggerIndex;

}tachoConfigType;

static void doExeuteTest(tachoConfigType cfg )
{
    // This engine has a tach pin set - we need that
    WITH_ENGINE_TEST_HELPER(BMW_E34);

    // We don't actually care about ign/inj at all, just tach
    engineConfiguration->isInjectionEnabled = false;
    engineConfiguration->isIgnitionEnabled = false;

    // Configure tach pulse count
    // 5 PPR, 25% duty
    engineConfiguration->tachPulsePerRev = cfg.PulsePerRev;
    engineConfiguration->tachPulseDuractionMs = cfg.PulseDurationMs;
    engineConfiguration->tachPulseDurationAsDutyCycle = cfg.DurationAsDutyCycle;
    engineConfiguration->tachPulseTriggerIndex = cfg.PulseTriggerIndex;

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
    float enginePeriod = 60000000.0 / eth.engine.rpmCalculator.rpmValue; //40000; // us
    float tachPeriod = enginePeriod / cfg.PulsePerRev;
    float tachHighTime = tachPeriod * cfg.PulseDurationMs;
    
    // Build the times we expect the edges to have occured
    float events[50];
    for (int i = 0; i < (2*cfg.PulsePerRev); i++) {
        auto periodStart = i * tachPeriod;
        events[2 * i] = periodStart;
        events[2 * i + 1] = periodStart + tachHighTime;
    }

    if ((4*cfg.PulsePerRev) <= 24)
    {
        // Check that exactly 16 events got scheduled
        std::cerr << "[          ] executor.size = " << engine->executor.size() << std::endl;
        ASSERT_EQ(engine->executor.size(), (4*cfg.PulsePerRev));

        auto start = eth.getTimeNowUs();

        // Check that everything is in the right spot
        for (int i = 0; i < (4*cfg.PulsePerRev); i++)
        {
            auto s = engine->executor.getForUnitTest(i);
            ASSERT_NE(s, nullptr);

            EXPECT_NEAR(start + events[i], s->momentX, 1);
        }   
    }else{
        std::cerr << "[          ] warining code = " << unitTestWarningCodeState.recentWarnings.get(0) << std::endl;
        ASSERT_EQ(CUSTOM_DUTY_INVALID, unitTestWarningCodeState.recentWarnings.get(0));
    }

    // Clean the slate
	eth.clearQueue();

}

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
	ASSERT_EQ(1500,  GET_RPM()) << "RPM";
	ASSERT_EQ(15,  engine->triggerCentral.triggerState.getCurrentIndex()) << "index #1";
    ASSERT_EQ(engine->triggerCentral.triggerState.shaft_is_synchronized, true);

    // Clean the slate
	eth.clearQueue();

	eth.moveTimeForwardMs(5 /*ms*/);

    // This will schedule tach events!
    eth.firePrimaryTriggerRise();
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
    ASSERT_EQ(engine->executor.size(), 16);

    auto start = eth.getTimeNowUs();

    // Check that everything is in the right spot
    for (int i = 0; i < 16; i++)
    {
        auto s = engine->executor.getForUnitTest(i);
        ASSERT_NE(s, nullptr);

        EXPECT_NEAR(start + events[i], s->momentX, 1);
    }

    // Clean the slate
	eth.clearQueue();
}

TEST(tachometer, testSixPulsePerRev) {
    // This engine has a tach pin set - we need that
    WITH_ENGINE_TEST_HELPER(BMW_E34);

    // We don't actually care about ign/inj at all, just tach
    engineConfiguration->isInjectionEnabled = false;
    engineConfiguration->isIgnitionEnabled = false;

    // Configure tach pulse count
    // 5 PPR, 50% duty
    engineConfiguration->tachPulsePerRev = 6;
    engineConfiguration->tachPulseDuractionMs = 0.5f;
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
	ASSERT_EQ(1500,  GET_RPM()) << "RPM";
	ASSERT_EQ(15,  engine->triggerCentral.triggerState.getCurrentIndex()) << "index #1";
    ASSERT_EQ(engine->triggerCentral.triggerState.shaft_is_synchronized, true);

    // Clean the slate
	eth.clearQueue();

    //move time fwd to give it a chance to schedule events
	eth.moveTimeForwardMs(5 /*ms*/);

    // This will schedule tach events!
    eth.firePrimaryTriggerRise();
    ASSERT_EQ(1500, eth.engine.rpmCalculator.rpmValue) << "RPM";

    // Now we verify that the correct stuff got scheduled
    float enginePeriod = 40000; // us
    float tachPeriod = enginePeriod / 6;
    float tachHighTime = tachPeriod * 0.50f;

    // Build the times we expect the edges to have occured
    float events[24];
    //for will go till tachPulsePerRev * 2
    for (int i = 0; i < 12; i++) {
        auto periodStart = i * tachPeriod;
        events[2 * i] = periodStart;
        events[2 * i + 1] = periodStart + tachHighTime;
    }

    // Check that exactly 16 events got scheduled
    ASSERT_EQ(engine->executor.size(), 24);

    auto start = eth.getTimeNowUs();

    // Check that everything is in the right spot
    for (int i = 0; i < 16; i++)
    {
        auto s = engine->executor.getForUnitTest(i);
        ASSERT_NE(s, nullptr);

        EXPECT_NEAR(start + events[i], s->momentX, 1);
    }
    // Clean the slate
	eth.clearQueue();
}

TEST(tachometer, testGenericPulsePerRev)
{
   tachoConfigType tconfig;

   tconfig.DurationAsDutyCycle =  true;
   tconfig.PulseDurationMs = 0.25f;
   tconfig.PulsePerRev = 4;
   tconfig.PulseTriggerIndex = 0;

    doExeuteTest(tconfig);

}

TEST(tachometer, testGenericAbusePulsePerRev)
{
   tachoConfigType tconfig;

   tconfig.DurationAsDutyCycle =  true;
   tconfig.PulseDurationMs = 0.5f;
   tconfig.PulsePerRev = 8;
   tconfig.PulseTriggerIndex = 0;

    doExeuteTest(tconfig);
}
#else


TEST(tachometer, testPulsePerRev) {
    // This engine has a tach pin set - we need that
    WITH_ENGINE_TEST_HELPER(BMW_E34);

    // We don't actually care about ign/inj at all, just tach
    engineConfiguration->isInjectionEnabled = false;
    engineConfiguration->isIgnitionEnabled = false;

    // Configure tach pulse count
    // 5 PPR, 25% duty
    engineConfiguration->tachPulsePerRev = 4;
    engineConfiguration->tachPulseDuractionMs = 0.5f;
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
	ASSERT_EQ(1500,  GET_RPM()) << "RPM";
	ASSERT_EQ(15,  engine->triggerCentral.triggerState.getCurrentIndex()) << "index #1";
    ASSERT_EQ(engine->triggerCentral.triggerState.shaft_is_synchronized, true);

    // Clean the slate
	eth.clearQueue();

	eth.moveTimeForwardMs(5 /*ms*/);

    // This will schedule tach events!
    eth.firePrimaryTriggerRise();
    ASSERT_EQ(1500, eth.engine.rpmCalculator.rpmValue) << "RPM";

    // Check that exactly 16 events got scheduled
    std::cerr << "Scheduled events: " << engine->executor.size() << "\n" << std::endl;
    ASSERT_EQ(engine->executor.size(), 2);
    
    //move to 11ms
    eth.moveTimeForwardMs(6 /*ms*/);
    eth.fireTriggerEventsWithDuration(5);
    // Check that exactly 16 events got scheduled
    std::cerr << "Scheduled events: " << engine->executor.size() << std::endl;
    ASSERT_EQ(engine->executor.size(), 2);

#ifdef ALL_TEST_bla
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
    std::cerr << "Scheduled events: " << engine->executor.size() << std::endl;
    ASSERT_EQ(engine->executor.size(), 16);

    auto start = eth.getTimeNowUs();

    // Check that everything is in the right spot
    for (int i = 0; i < 16; i++)
    {
        auto s = engine->executor.getForUnitTest(i);
        ASSERT_NE(s, nullptr);

        EXPECT_NEAR(start + events[i], s->momentX, 1);
    }
#endif
    // Clean the slate
	eth.clearQueue();
}

#endif