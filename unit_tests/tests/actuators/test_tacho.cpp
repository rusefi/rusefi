#include "pch.h"

extern float getTachFreq(void);
extern float getTachDuty(void);

TEST(Actuators, Tachometer) {
    // This engine has a tach pin set - we need that
    EngineTestHelper eth(FRANKENSO_MAZDA_MIATA_2003);

    // We don't actually care about ign/inj at all, just tach
    engineConfiguration->isInjectionEnabled = false;
    engineConfiguration->isIgnitionEnabled = false;

    // Configure tach pulse count
    // 5 PPR, 25% duty
    engineConfiguration->tachPulsePerRev = 4;
    engineConfiguration->tachPulseDuractionMs = 0.5f;
    engineConfiguration->tachPulseDurationAsDutyCycle = true;

    // Set predictable trigger settings
	engineConfiguration->trigger.type = trigger_type_e::TT_TOOTHED_WHEEL;
    engineConfiguration->trigger.customTotalToothCount = 8;
    engineConfiguration->trigger.customSkippedToothCount = 0;
    setCamOperationMode();
	eth.applyTriggerWaveform();

    // get the engine running - 6 revolutions
    eth.fireTriggerEvents(48);

    // ensure engine speed
	ASSERT_EQ(1500,  Sensor::getOrZero(SensorType::Rpm)) << "RPM";
    ASSERT_EQ(engine->triggerCentral.triggerState.getShaftSynchronized(), true);

	// Poke the fast callback to update the tach
	engine->periodicFastCallback();

    ASSERT_EQ(100, getTachFreq());
    ASSERT_EQ(0.5, getTachDuty());
}
