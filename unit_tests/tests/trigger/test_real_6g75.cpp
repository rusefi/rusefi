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

    // https://github.com/rusefi/rusefi/issues/8827
    // huge 'tooManyTeethCounter' is the indicator of trouble
    //
    // Investigation via TEST(trigger, finderRealData) (see test_trigger_sequence_finder.cpp)
    // shows: at cranking RPM the real wheel exposes ONLY ONE big gap per crank revolution
    // (32 visible rising edges between gaps) at ratio ~2.37, followed by a tooth at ratio
    // ~0.44. The textbook 36-2-1-1 pattern (three separate gaps with theoretical ratios
    // 3.0/2.0/2.0) does NOT match the captured signal: the two single-tooth gaps are
    // masked by sensor ringing / fill-in. Brute-forcing setTriggerSynchronizationGap*()
    // against the existing synthetic waveform yields zero "happy" sync sequences for any
    // real-data ratio set, so fixing this requires redesigning the synthetic waveform in
    // initialize36_2_1_1() (likely as 36-4 single-block-gap for cranking) AND only then
    // widening the gap window to 1.8-4.5 / 0.3-0.7. Pinned at 382 until that lands.
    ASSERT_EQ(382, tooManyTeethCounter);
    ASSERT_EQ(1, eth.recentWarnings()->getCount());
    ASSERT_NEAR(143.40, Sensor::getOrZero(SensorType::Rpm), 0.1);
}
