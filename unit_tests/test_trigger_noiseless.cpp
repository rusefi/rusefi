/**
 * @file	test_trigger_noiseless.cpp
 *
 * @date Apr 20, 2018
 */

#include "main.h"
#include "test_trigger_decoder.h"
#include "trigger_decoder.h"
#include "engine_math.h"
#include "allsensors.h"
#include "rpm_calculator.h"
#include "event_queue.h"
#include "algo.h"
#include "trigger_central.h"
#include "main_trigger_callback.h"
#include "engine.h"
#include "advance_map.h"
#include "speed_density.h"
#include "fuel_math.h"
#include "spark_logic.h"
#include "trigger_universal.h"

extern int timeNowUs;
extern float unitTestValue;
extern float testMafValue;
extern int unitTestWarningCounter;
extern bool printTriggerDebug;
extern float actualSynchGap;

extern EventQueue schedulingQueue;

static void fireEvent(EngineTestHelper *eth, bool isRise) {
	// mostly we fire only rise events (useOnlyRisingEdgeForTrigger=true).
	// but for noise filtering, both edges should be processed, so we fire falling events too
	if (isRise)
		eth->firePrimaryTriggerRise();
	else if (eth->engine.engineConfiguration->bc.useNoiselessTriggerDecoder)
		eth->firePrimaryTriggerFall();
}

/*           ________          __ _ _ __
 * __|_|_|__|            OR      | | |  |________
 *   spikes   signal            spikes    signal
 */
static void noisyPulse(EngineTestHelper *eth, int idx, int durationUs, bool isRise, int noiseIdx, int durationNoiseUs, int offsetNoiseUs, int numSpikes) {
	// skip some time at the beginning
	timeNowUs += offsetNoiseUs;
	durationUs -= offsetNoiseUs;
	// add noise spikes
	if (idx == noiseIdx) {
		// calculate the distance between noise spikes (evenly spaced)
		int noiseIntervalUs = (durationUs - durationNoiseUs * numSpikes) / numSpikes;

		for (int i = 0; i < numSpikes; i++) {
			// start spike
			fireEvent(eth, isRise);
			timeNowUs += durationNoiseUs;
			durationUs -= durationNoiseUs;
			// end spike
			fireEvent(eth, !isRise);

			// add space between spikes
			timeNowUs += noiseIntervalUs;
			durationUs -= noiseIntervalUs;
		}
	}

	// add the rest of pulse period
	timeNowUs += durationUs;
	fireEvent(eth, isRise);
}

static void fireNoisyCycle60_2(EngineTestHelper *eth, int numCycles, int durationUs, int noiseIdx, int durationNoiseUs, int offsetNoiseUs, int numSpikes) {
	int idx = 0;
	for (int cycles = 0; cycles < numCycles; cycles++) {
		int count = 60 - 2 - 1;
		for (int i = 0; i < count; i++) {
			// rising
			noisyPulse(eth, idx++, durationUs, true, noiseIdx, durationNoiseUs, offsetNoiseUs, numSpikes);
			// falling
			noisyPulse(eth, idx++, durationUs, false, noiseIdx, durationNoiseUs, offsetNoiseUs, numSpikes);
		}
		// skip 2 teeth (durationUs * 4)
		// and add 1st rising teeth of the next cycle
		noisyPulse(eth, idx++, (durationUs * 4) + durationUs, true, noiseIdx, durationNoiseUs, offsetNoiseUs, numSpikes);
		// falling
		noisyPulse(eth, idx++, durationUs, false, noiseIdx, durationNoiseUs, offsetNoiseUs, numSpikes);
	}
}

static void resetTrigger(EngineTestHelper &eth) {
	timeNowUs = 0;
	eth.applyTriggerShape();
	eth.engine.triggerCentral.resetAccumSignalData();
	// reset error cnt
	eth.engine.triggerCentral.triggerState.totalTriggerErrorCounter = 0;
}

static void testNoiselessDecoderProcedure(EngineTestHelper &eth, int errorToleranceCnt DECLARE_ENGINE_PARAMETER_SUFFIX) {
	printf("*** (bc->useNoiselessTriggerDecoder = %s)\r\n",
			boardConfiguration->useNoiselessTriggerDecoder ? "true" : "false");

	resetTrigger(eth);
	
	// first, no noise
	fireNoisyCycle60_2(&eth, 2, 1000, -1, 0, 0, 0);

	// should be no errors anyway
	assertEquals(0, engine->triggerCentral.triggerState.totalTriggerErrorCounter);
	// check if we're imitating the 60-2 signal correctly
	assertEqualsM("index #1", 0, eth.engine.triggerCentral.triggerState.getCurrentIndex());
	// check rpm (60secs / (1000us * 60teeth)) = 1000rpm
	assertEqualsM("RPM", 1000, eth.engine.rpmCalculator.getRpm(PASS_ENGINE_PARAMETER_SIGNATURE));

	// add noise1 - 1 spike in the middle of the 2nd rising pulse
	fireNoisyCycle60_2(&eth, 2, 1000, 2, 10, 500, 1);

	assertEqualsM("noise#1", errorToleranceCnt, engine->triggerCentral.triggerState.totalTriggerErrorCounter);

	resetTrigger(eth);

	// add noise2 - 1 spike in the middle of the 2nd falling pulse
	fireNoisyCycle60_2(&eth, 2, 1000, 3, 10, 500, 1);

	//assertEqualsM("noise#2", errorToleranceCnt, engine->triggerCentral.triggerState.totalTriggerErrorCounter);

	resetTrigger(eth);

	// add noise3 - in the middle of the sync.gap,
	// so that we cannot tell for sure if it's a start of another 'extra' tooth or just a noise inside the gap,
	// that's why we used expectedEventCount[] in our filtering algo to make a prediction about gap
	fireNoisyCycle60_2(&eth, 2, 1000, 114, 10, 1500, 1);

	// so everything runs smoothly!
	assertEqualsM("noise#3", errorToleranceCnt, engine->triggerCentral.triggerState.totalTriggerErrorCounter);

	resetTrigger(eth);

	// add noise4 - too close to the start of the real next signal, so the noise spike is accepted as a signal
	// but when the real signal comes shortly afterwards, it we be treated as a noise spike,
	fireNoisyCycle60_2(&eth, 2, 1000, 4, 10, 980, 1);

	// and we won't get out of sync!
	assertEqualsM("noise#4", errorToleranceCnt, engine->triggerCentral.triggerState.totalTriggerErrorCounter);

	resetTrigger(eth);

	// add noise5 - one very long 333us noise spike
	fireNoisyCycle60_2(&eth, 2, 1000, 4, 333, 10, 1);
	// still ok
	assertEqualsM("noise#5", errorToleranceCnt, engine->triggerCentral.triggerState.totalTriggerErrorCounter);

	resetTrigger(eth);

	// add noise6 - 10 short spikes across the entire signal pulse
	const int failProofNumSpikes = 10;
	fireNoisyCycle60_2(&eth, 2, 1000, 4, 5, 10, failProofNumSpikes);

	// we barely survived this time
	assertEqualsM("noise#6", errorToleranceCnt, engine->triggerCentral.triggerState.totalTriggerErrorCounter);

	resetTrigger(eth);

	// add noise7 - 34 short spikes across the entire signal pulse
	fireNoisyCycle60_2(&eth, 2, 1000, 2, 10, 10, failProofNumSpikes + 1);

	// alas, this is a hard case even for noiseless decoder, and it fails...
	// but still we're close to 33% signal-noise ratio threshold - not bad!
	// so here's an error anyway!
	assertEqualsM("noise#7_fail_test", 1, engine->triggerCentral.triggerState.totalTriggerErrorCounter);

}

void testNoiselessDecoder(void) {
	printf("*************************************************** testNoiselessDecoder\r\n");
	timeNowUs = 0;
	schedulingQueue.clear();

	EngineTestHelper eth(TEST_ENGINE);
	EXPAND_EngineTestHelper

	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
	engineConfiguration->useOnlyRisingEdgeForTrigger = true;
	setupSimpleTestEngineWithMafAndTT_ONE_trigger(&eth);

	// we'll test on 60-2 wheel
	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL_60_2;
	incrementGlobalConfigurationVersion(PASS_ENGINE_PARAMETER_SIGNATURE);
	eth.applyTriggerShape();
	engine->updateSlowSensors(PASS_ENGINE_PARAMETER_SIGNATURE);

	assertEquals(0, engine->triggerCentral.triggerState.totalTriggerErrorCounter);
	assertEquals(0, eth.engine.rpmCalculator.getRpm(PASS_ENGINE_PARAMETER_SIGNATURE));

	//printTriggerDebug = true;

#if 0
	// try normal trigger mode, no noise filtering
	boardConfiguration->useNoiselessTriggerDecoder = false;
	// for test validation, it should be 1 trigger error
	testNoiselessDecoderProcedure(eth, 1 PASS_ENGINE_PARAMETER_SUFFIX);
#endif

	// now enable our noise filtering algo
	boardConfiguration->useNoiselessTriggerDecoder = true;
	// should be 0 errors!
	testNoiselessDecoderProcedure(eth, 0 PASS_ENGINE_PARAMETER_SUFFIX);

	//printTriggerDebug = false;
}
