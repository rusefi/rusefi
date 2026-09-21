/**
 * @file test_trigger_scheduler.cpp
 *
 * Test-Driven-Bugfixing coverage for https://github.com/rusefi/rusefi/issues/9435
 * see https://github.com/rusefi/rusefi/wiki/TDB-Test-Driven-Bugfixing
 *
 * Merge one of this file carried GREEN death tests asserting the then-broken
 * behavior: TriggerScheduler::cancel() walked m_angleBasedEventsHead with utlist
 * LL_DELETE2, which dereferences the head pointer without a null check when the
 * element is not the head. Any cancel() racing scheduleEventsUntilNextTriggerTooth()
 * - which used to detach the whole list into a local while walking it - hit a null
 * head. On the host that was an immediate death; on STM32 address 0x1c (offsetof
 * nextToothEvent) is the readable ITCM flash alias of the bootloader vector table,
 * so the walk instead chased flash junk into a precise bus fault
 * (r3 = 0x025723B8, BFAR = 0x025723D4 = r3 + 0x1c, PC inside the inlined
 * overFireSparkAndPrepareNextSchedule -> TriggerScheduler::cancel).
 *
 * These are now the merge-two versions asserting the FIXED semantics: cancel of an
 * event that is in no list is a harmless no-op, and a cancel arriving while the
 * tooth walk is in flight takes effect - the canceled event is neither left in the
 * list nor promoted to the time-based scheduler.
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
// scheduleEventsUntilNextTriggerTooth() is mid-walk, and that action calls
// TriggerScheduler::cancel(&event->sparkEvent) - see spark_logic.cpp. This mock
// reproduces exactly that interleaving deterministically.
class OverdwellDuringToothWalkExecutor : public Scheduler {
public:
	TriggerScheduler* triggerScheduler = nullptr;
	// the tooth-queued event whose 1.5x-dwell overdwell deadline has lapsed
	AngleBasedEvent* overdwellVictim = nullptr;
	// seeing this scheduling_s promoted to time-based scheduling means we are
	// inside the tooth walk
	scheduling_s* promotionMarker = nullptr;
	bool overdwellFired = false;

	std::vector<scheduling_s*> scheduled;

	void schedule(const char* /*msg*/, scheduling_s* scheduling, efitick_t /*timeNt*/, action_s const& /*action*/) override {
		scheduled.push_back(scheduling);

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

// Minimal statement of the fixed defect: cancel of an event which is not in the
// (empty) list is a harmless no-op instead of an LL_DELETE2 null-head walk.
TEST(TriggerScheduler, issue9435CancelOnEmptyListIsNoOp) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	AngleBasedEvent event;

	// with the issue #9435 defect present this call dies on the host
	// (wild-pointer bus faults on STM32)
	engine->module<TriggerScheduler>()->cancel(&event);
}

// The field scenario of issue #9435: overdwell protection fires while the trigger
// tooth walk is in flight and cancels an event still queued for a later tooth.
TEST(TriggerScheduler, issue9435OverdwellCancelDuringToothWalk) {
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

	// The tooth covering sparkA's angle arrives. The walk promotes sparkA via
	// scheduleByAngle -> scheduler.schedule(), which on hardware drains sparkB's
	// lapsed overdwell action inline (emulated by OverdwellDuringToothWalkExecutor),
	// so TriggerScheduler::cancel(&sparkB) runs mid-walk. Before the fix this walked
	// the detached null list head and crashed.
	ts->scheduleEventsUntilNextTriggerTooth(1000, getTimeNowNt(), 0, 180);

	ASSERT_TRUE(executor.overdwellFired);
	// sparkA was promoted, exactly once
	ASSERT_EQ(1u, executor.scheduled.size());
	EXPECT_EQ(&sparkA.eventScheduling, executor.scheduled[0]);

	// the mid-walk cancel took effect: sparkB is gone from the tooth list, so the
	// tooth covering its angle promotes nothing
	ts->scheduleEventsUntilNextTriggerTooth(1000, getTimeNowNt(), 180, 360);
	EXPECT_EQ(1u, executor.scheduled.size()) << "canceled sparkB must not fire";

	engine->scheduler.setMockExecutor(nullptr);
}

// Same interleaving, but the canceled event is due at the SAME tooth as the one
// being promoted - it is sitting on the pending-promotion list when the cancel
// arrives, and must still be removed rather than fired.
TEST(TriggerScheduler, issue9435CancelOfPendingPromotionDuringToothWalk) {
	OverdwellDuringToothWalkExecutor executor;
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engine->rpmCalculator.setRpmValue(1000);

	TriggerScheduler* ts = &engine->module<TriggerScheduler>().unmock();
	executor.triggerScheduler = ts;
	engine->scheduler.setMockExecutor(&executor);

	// both events are due in the same tooth window
	AngleBasedEvent sparkA;
	AngleBasedEvent sparkB;
	ts->schedule("sparkA", &sparkA, 90, action_s::make<dummySparkFire>(uintptr_t{0}));
	ts->schedule("sparkB", &sparkB, 120, action_s::make<dummySparkFire>(uintptr_t{1}));

	executor.overdwellVictim = &sparkB;
	executor.promotionMarker = &sparkA.eventScheduling;

	ts->scheduleEventsUntilNextTriggerTooth(1000, getTimeNowNt(), 0, 180);

	ASSERT_TRUE(executor.overdwellFired);
	// only sparkA reached the time-based scheduler; sparkB was canceled while
	// awaiting promotion and must not have been promoted
	ASSERT_EQ(1u, executor.scheduled.size());
	EXPECT_EQ(&sparkA.eventScheduling, executor.scheduled[0]);

	// and it is not lingering in the tooth list either
	ts->scheduleEventsUntilNextTriggerTooth(1000, getTimeNowNt(), 0, 180);
	EXPECT_EQ(1u, executor.scheduled.size()) << "canceled sparkB must not fire";

	engine->scheduler.setMockExecutor(nullptr);
}
