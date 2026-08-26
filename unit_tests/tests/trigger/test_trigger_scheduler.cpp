/**
 * @file test_trigger_scheduler.cpp
 *
 * Test-Driven-Bugfixing coverage for https://github.com/rusefi/rusefi/issues/9435
 * see https://github.com/rusefi/rusefi/wiki/TDB-Test-Driven-Bugfixing
 *
 * These are GREEN tests asserting current BROKEN behavior: TriggerScheduler::cancel()
 * walks m_angleBasedEventsHead with utlist LL_DELETE2, which dereferences the head
 * pointer without a null check when the element is not the head. So any cancel() that
 * runs while the list is empty - most notably while scheduleEventsUntilNextTriggerTooth()
 * has the list detached into its local 'keephead' - walks a null pointer.
 *
 * On the host that is an immediate death (asserted below via death tests). On STM32
 * address 0x1c (offsetof nextToothEvent) is the readable ITCM flash alias of the
 * bootloader vector table, so the walk instead loads whatever flash content lives in
 * the first reserved vector entry and chases it as a pointer: one more iteration
 * faults at <junk>+0x1c - exactly the precise bus fault of issue #9435
 * (r3 = 0x025723B8, BFAR = 0x025723D4 = r3 + 0x1c, PC inside the inlined
 * overFireSparkAndPrepareNextSchedule -> TriggerScheduler::cancel).
 *
 * Once the underlying defect is fixed, these tests MUST be flipped to assert the
 * intended behavior: a cancel that races the tooth walk takes effect (the canceled
 * event neither stays in the list nor fires), and cancel on an empty list is a no-op.
 */

#include "pch.h"

namespace {

// The scheduled actions never execute in these tests - the walk hands them to the
// mock executor below, which only records/intercepts. Integral arg keeps action_s
// happy without a real callback target.
static void dummySparkFire(uintptr_t) {
}

// Emulates SingleTimerExecutor's behavior relevant to issue #9435: on real hardware
// schedule() drains all due time-based actions inline (executeAllPendingActions), and
// the hardware timer ISR can fire between the trigger walk's unlocked steps. Either
// way a lapsed overdwell action overFireSparkAndPrepareNextSchedule() executes while
// scheduleEventsUntilNextTriggerTooth() has m_angleBasedEventsHead detached to null,
// and that action calls TriggerScheduler::cancel(&event->sparkEvent) - see
// spark_logic.cpp. This mock reproduces exactly that interleaving deterministically.
class OverdwellDuringToothWalkExecutor : public Scheduler {
public:
	TriggerScheduler* triggerScheduler = nullptr;
	// the tooth-queued event whose 1.5x-dwell overdwell deadline has lapsed
	AngleBasedEvent* overdwellVictim = nullptr;
	// seeing this scheduling_s promoted to time-based scheduling means we are
	// inside the tooth walk, list detached
	scheduling_s* promotionMarker = nullptr;
	bool overdwellFired = false;

	void schedule(const char* /*msg*/, scheduling_s* scheduling, efitick_t /*timeNt*/, action_s const& /*action*/) override {
		if (scheduling == promotionMarker && !overdwellFired) {
			overdwellFired = true;
			// what overFireSparkAndPrepareNextSchedule() does first thing:
			// "kill pending fire"
			triggerScheduler->cancel(overdwellVictim);
		}
	}

	void cancel(scheduling_s* /*scheduling*/) override {
	}
};

} // namespace

// Minimal statement of the defect: cancel of an event which is not in the (empty)
// list starts LL_DELETE2's search loop from a null head.
TEST(TriggerSchedulerDeathTest, issue9435CancelOnEmptyListWalksNullHead) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	AngleBasedEvent event;

	// BROKEN behavior [issue #9435]: this must become a harmless no-op once fixed
	EXPECT_DEATH_IF_SUPPORTED(
		engine->module<TriggerScheduler>()->cancel(&event), "");
}

// The field scenario of issue #9435: overdwell protection fires while the trigger
// tooth walk is in flight.
TEST(TriggerSchedulerDeathTest, issue9435OverdwellCancelDuringToothWalk) {
	OverdwellDuringToothWalkExecutor executor;
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engine->rpmCalculator.setRpmValue(1000);

	TriggerScheduler* ts = &engine->module<TriggerScheduler>().unmock();
	executor.triggerScheduler = ts;
	engine->scheduler.setMockExecutor(&executor);

	// Cylinder A's spark is due at the incoming tooth; cylinder B's spark was queued
	// by tooth for later in the cycle with its overdwell timer armed, and the trigger
	// slowed down enough (cranking) that B's overdwell deadline already lapsed.
	AngleBasedEvent sparkA;
	AngleBasedEvent sparkB;
	ts->schedule("sparkA", &sparkA, 90, action_s::make<dummySparkFire>(uintptr_t{0}));
	ts->schedule("sparkB", &sparkB, 270, action_s::make<dummySparkFire>(uintptr_t{1}));

	executor.overdwellVictim = &sparkB;
	executor.promotionMarker = &sparkA.eventScheduling;

	// The tooth covering sparkA's angle arrives. scheduleEventsUntilNextTriggerTooth()
	// detaches the whole list into keephead, promotes sparkA via scheduleByAngle ->
	// scheduler.schedule(), which on hardware drains sparkB's lapsed overdwell action
	// inline (emulated by OverdwellDuringToothWalkExecutor). That action's
	// TriggerScheduler::cancel(&sparkB) then walks the detached null head.
	//
	// BROKEN behavior [issue #9435]: the walk dies on the host / wild-pointer bus
	// faults on STM32. Once fixed, the cancel must instead take effect: sparkB
	// removed even mid-walk, never promoted, no crash - flip this test accordingly.
	EXPECT_DEATH_IF_SUPPORTED(
		ts->scheduleEventsUntilNextTriggerTooth(1000, getTimeNowNt(), 0, 180), "");

	// The death above ran in the death-test child process; in this parent process the
	// walk never executed and both events are still queued. Drain them while the list
	// is non-empty (which works fine) so the helper tears down on a clean list.
	ASSERT_TRUE(executor.overdwellFired == false);
	engine->scheduler.setMockExecutor(nullptr);
	ts->cancel(&sparkB);
	ts->cancel(&sparkA);
}
