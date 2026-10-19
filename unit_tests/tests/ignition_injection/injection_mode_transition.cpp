/*
 * @file injection_mode_transition.cpp
 *
 *  Created on: Jul 19, 2020
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "fuel_math.h"
#include "injector_model.h"

using ::testing::_;

static void doRevolution(EngineTestHelper& eth, int periodMs) {
	float halfToothTime = (periodMs / 6.0f) / 2;

	eth.smartFireRise(halfToothTime);
	eth.fireFall(halfToothTime);
	eth.smartFireRise(halfToothTime);
	eth.fireFall(halfToothTime);
	eth.smartFireRise(halfToothTime);
	eth.fireFall(halfToothTime);

	// now missing tooth
	eth.smartFireRise(halfToothTime);
	eth.fireFall(3 * halfToothTime);

	// This tooth is the sync point!
	eth.smartFireRise(halfToothTime);
	eth.fireFall(halfToothTime);
}

// https://github.com/rusefi/rusefi/issues/1592
TEST(fuelControl, transitionIssue1592) {

	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engine->tdcMarkEnabled = false;
	setupSimpleTestEngineWithMafAndTT_ONE_trigger(&eth, IM_SEQUENTIAL);

	// fast callbacks now run while we advance time (no NoFastCall hack), so airmass
	// can be requested at other RPMs (e.g. 0 before sync) - accept those too
	EXPECT_CALL(*eth.mockAirmass, getAirmass(_, _))
		.WillRepeatedly(Return(AirmassResult{0.1008f, 50.0f}));
	EXPECT_CALL(*eth.mockAirmass, getAirmass(500, _))
		.WillRepeatedly(Return(AirmassResult{0.1008f, 50.0f}));

	// This is easiest to trip on a wheel that requires sync
	engineConfiguration->trigger.customTotalToothCount = 6;
	engineConfiguration->trigger.customSkippedToothCount = 1;
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL);
    setCamOperationMode();
	engineConfiguration->isFasterEngineSpinUpEnabled = true;

	setTable(config->injectionPhase, 0.0f);
	setArrayValues(config->crankingFuelCoef, 1.0f);


	engineConfiguration->globalTriggerAngleOffset = 20;

	// Yes, this is a ton of fuel but it makes the repro easier
	setTable(config->crankingCycleBaseFuel, 213.6);
	engineConfiguration->cranking.rpm = 501;

	// Test the transition from batch cranking to sequential running
	engineConfiguration->crankingInjectionMode = IM_BATCH;

	// First sync point will schedule cranking pulse since we're in "faster spin up" mode
	doRevolution(eth, 240);

	{
		// Injector 2 should be scheduled to open then close
		auto inj2 = &engine->injectionEvents.elements[1];
		auto const taggedPointer{TaggedPointer<decltype(this)>::make(inj2, false)};
		auto const aHigh{ action_s::make<turnInjectionPinHigh>( taggedPointer.getRaw() ) };
		auto const aLow{ action_s::make<turnInjectionPinLow>( inj2 ) };

		ASSERT_EQ(engine->scheduler.size(), 2);

		// Check that the action is correct - we don't care about the timing necessarily
		auto sched_open = engine->scheduler.getForUnitTest(0);
		ASSERT_EQ(sched_open->action.getArgumentRaw(), taggedPointer.getRaw());
		ASSERT_EQ(sched_open->action.getCallback(), aHigh.getCallback());

		auto sched_close = engine->scheduler.getForUnitTest(1);
		// Next action should be closing the same injector
		ASSERT_EQ(sched_close->action.getArgumentRaw(), taggedPointer.getRaw());
		ASSERT_EQ(sched_close->action.getCallback(), aLow.getCallback());
	}

	// Run the engine for some revs
	for (size_t i = 0; i < 10; i++) {
		doRevolution(eth, 150);
	}

	// Check that no injectors are stuck open.
	// With fast callbacks running while time advances (no NoFastCall hack), fuel is
	// recomputed continuously across the batch->sequential transition and no injector
	// overlap is produced - all overlapping counters are zero.
	EXPECT_EQ(enginePins.injectors[0].getOverlappingCounter(), 0);
	EXPECT_EQ(enginePins.injectors[1].getOverlappingCounter(), 0);
	EXPECT_EQ(enginePins.injectors[2].getOverlappingCounter(), 0);
	EXPECT_EQ(enginePins.injectors[3].getOverlappingCounter(), 0);
}

namespace {
enum class StagedBatchTransition {
	DuringPulse, // Reproduce the fault by changing modes with both stages open.
	AfterPulse, // Control: change modes only after both stages have closed.
	KeepCranking, // Control: let the batch pulse finish without changing modes.
};

// ECU-010: DuringPulse deliberately asserts the current defect. Change its
// expectations with the subsequent fix, per CLAUDE.md coverage-first guidance.
void checkStagedBatchTransition(StagedBatchTransition transition) {
	// SETUP: prepare a staged batch pulse that can span the running threshold.
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	// EnginePins::reset currently omits secondary injectors. Keep this test isolated,
	// including early assertion exits, while exposing the production transition.
	struct SecondaryPinCleanup {
		~SecondaryPinCleanup() {
			for (auto& pin : enginePins.injectorsStage2) {
				pin.reset();
			}
		}
	} cleanup;
	for (auto& pin : enginePins.injectorsStage2) {
		pin.reset();
	}
	// Batch mode pairs cylinders 360 crank degrees apart. With firing order
	// 1-3-4-2, event 0 opens cylinders 1 and 4 (pin indices 0 and 3).
	// Crossing 400 RPM switches from this paired batch mapping to one output
	// per event in sequential mode. Staging is supported during batch cranking.
	engineConfiguration->cylindersCount = 4;
	engineConfiguration->firingOrder = FO_1_3_4_2;
	engineConfiguration->cranking.rpm = 400;
	engineConfiguration->crankingInjectionMode = IM_BATCH;
	engineConfiguration->injectionMode = IM_SEQUENTIAL;
	engineConfiguration->enableStagedInjection = true;
	// Use identical real injector models: 1 g/s flow and a constant 1 ms
	// deadtime, with pressure compensation and primary nonlinearity disabled.
	// Splitting 0.02 g equally between stages gives each stage 0.01 g:
	// 10 ms of flow plus 1 ms deadtime produces an 11 ms pulse on both stages.
	engineConfiguration->injectorFlowAsMassFlow = true;
	engineConfiguration->injector.flow = 1;
	engineConfiguration->injectorSecondary.flow = 1;
	engineConfiguration->injectorCompensationMode = ICM_None;
	engineConfiguration->secondaryInjectorCompensationMode = ICM_None;
	engineConfiguration->injectorNonlinearMode = INJ_None;
	setTable(engineConfiguration->injector.battLagCorrTable, 1.0f);
	setTable(engineConfiguration->injectorSecondary.battLagCorrTable, 1.0f);
	setTable(config->injectorStagingTable, 50);
	engine->updateTriggerConfiguration();
	engine->rpmCalculator.setRpmValue(390);
	ASSERT_TRUE(engine->rpmCalculator.isCranking());
	// First RPM invokes the fast callback. Set the event's input snapshot after it.
	// The staging fraction comes from the configured table; supply a fixed fuel
	// mass and nominal timing so this case isolates scheduling and output state.
	engine->engineState.injectionStage2Fraction = getStage2InjectionFraction(390, 100);
	ASSERT_FLOAT_EQ(0.5f, engine->engineState.injectionStage2Fraction);
	engine->engineState.injectionDuration = 11;
	engine->engineState.injectionOffset = 0;
	engine->engineState.shouldUpdateInjectionTiming = true;
	setArrayValues(engine->engineState.injectionMass, 0.02f);
	engine->module<InjectorModelPrimary>().unmock().prepare();
	engine->module<InjectorModelSecondary>().unmock().prepare();
	ASSERT_NEAR(11, engine->module<InjectorModelPrimary>()->getInjectionDuration(0.01f), 0.001f);
	ASSERT_NEAR(11, engine->module<InjectorModelSecondary>()->getInjectionDuration(0.01f), 0.001f);
	engine->injectionEvents.addFuelEvents();
	auto& event = engine->injectionEvents.elements[0];
	ASSERT_EQ(&enginePins.injectorsStage2[3], event.outputsStage2[1]);
	event.injectionStartAngle = 0;
	// END OF SETUP. All three cases start with the same configured batch event.

	// RUNTIME: feed the event a tooth interval containing its opening angle.
	// The real scheduler opens both stages immediately and retains two closing
	// callbacks, one per stage, for the end of the 11 ms pulse.
	event.onTriggerTooth(getTimeNowNt(), 0, 10);
	ASSERT_EQ(1, eth.executeActions());
	ASSERT_EQ(2, engine->scheduler.size());
	ASSERT_TRUE(enginePins.injectorsStage2[0].getLogicValue());
	ASSERT_TRUE(enginePins.injectorsStage2[3].getLogicValue());

	const bool orphanedSecondary = transition == StagedBatchTransition::DuringPulse;
	if (orphanedSecondary) {
		// Only 1 ms has passed, so neither closing callback has run. The real
		// RPM transition resets primary outputs and rebuilds the event mapping.
		// Cylinder 4 disappears from outputsStage2, although its secondary pin
		// is still open and its queued close reads this same mutable event.
		eth.moveTimeForwardUs(1000);
		engine->rpmCalculator.setRpmValue(410);
		ASSERT_FALSE(engine->rpmCalculator.isCranking());
		ASSERT_EQ(nullptr, event.outputsStage2[1]);
	}
	// Advance beyond the pulse and explicitly run both closing callbacks.
	// An empty queue establishes that a remaining high output is not simply
	// waiting for a scheduled close. Primary outputs must all be low here.
	eth.moveTimeForwardUs(20000);
	ASSERT_EQ(2, eth.executeActions());
	ASSERT_EQ(nullptr, engine->scheduler.getHead());
	for (const auto& pin : enginePins.injectors) {
		EXPECT_FALSE(pin.getLogicValue());
	}
	// DEFECT ASSERTIONS: DuringPulse expects cylinder 4's secondary pin to
	// remain high with overlap count 1 despite the completed close callbacks.
	// That is the reproduced bug; correct behavior would be low/count 0 for
	// every secondary pin. Both controls already expect those correct values.
	for (size_t i = 0; i < efi::size(enginePins.injectorsStage2); i++) {
		const bool remainsOpen = orphanedSecondary && i == 3;
		EXPECT_EQ(remainsOpen, enginePins.injectorsStage2[i].getLogicValue()) << i;
		EXPECT_EQ(remainsOpen ? 1 : 0, enginePins.injectorsStage2[i].getOverlappingCounter()) << i;
	}

	if (transition == StagedBatchTransition::KeepCranking) {
		ASSERT_TRUE(engine->rpmCalculator.isCranking());
		return;
	}
	if (transition == StagedBatchTransition::AfterPulse) {
		// This control makes the same RPM/mapping change after the batch
		// outputs have closed, leaving no open secondary output to orphan.
		engine->rpmCalculator.setRpmValue(410);
		ASSERT_FALSE(engine->rpmCalculator.isCranking());
		ASSERT_EQ(nullptr, event.outputsStage2[1]);
	}

	// Schedule a normal sequential pulse for cylinder 4 (firing-order index 2).
	// Its open increments the orphaned counter from 1 to 2 in the reproduction;
	// in the AfterPulse control, the counter starts at 0 and increments to 1.
	auto& nextEvent = engine->injectionEvents.elements[2];
	ASSERT_EQ(&enginePins.injectorsStage2[3], nextEvent.outputsStage2[0]);
	nextEvent.injectionStartAngle = 0;
	nextEvent.onTriggerTooth(getTimeNowNt(), 0, 10);
	ASSERT_EQ(1, eth.executeActions());
	ASSERT_TRUE(enginePins.injectorsStage2[3].getLogicValue());
	ASSERT_EQ(orphanedSecondary ? 2 : 1, enginePins.injectorsStage2[3].getOverlappingCounter());
	eth.moveTimeForwardUs(20000);
	ASSERT_EQ(2, eth.executeActions());
	EXPECT_EQ(nullptr, engine->scheduler.getHead());
	// PERSISTENCE ASSERTIONS: the matching sequential close only undoes its
	// own open. DuringPulse is still high/count 1 with no callbacks pending;
	// AfterPulse returns to low/count 0. A later normal pulse cannot repair
	// the unmatched batch open. Update the defect expectations with the fix.
	EXPECT_EQ(orphanedSecondary, enginePins.injectorsStage2[3].getLogicValue());
	EXPECT_EQ(orphanedSecondary ? 1 : 0, enginePins.injectorsStage2[3].getOverlappingCounter());
}
} // namespace

TEST(fuelControl, CrankingToRunningLeavesSecondaryBatchInjectorOpen) {
	checkStagedBatchTransition(StagedBatchTransition::DuringPulse);
}

TEST(fuelControl, StagedBatchPulseClosesWhileCranking) {
	checkStagedBatchTransition(StagedBatchTransition::KeepCranking);
}

TEST(fuelControl, CrankingToRunningAfterStagedBatchPulseClosesSecondaryInjector) {
	checkStagedBatchTransition(StagedBatchTransition::AfterPulse);
}
