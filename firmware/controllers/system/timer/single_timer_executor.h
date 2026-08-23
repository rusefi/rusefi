/**
 * @file single_timer_executor.h
 *
 * @date: Apr 18, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "scheduler.h"
#include "event_queue.h"

// Command classes for the dispatch-lateness telemetry. The executor
// classifies each executed event by its callback (see classifyAction in
// single_timer_executor.cpp) so the lockstats output shows exactly which
// engine-timing commands float.
enum class ExecEventKind : uint8_t {
	Other = 0,
	Dwell,     // turnSparkPinHighStartCharging - coil charge start
	Spark,     // fireSparkAndPrepareNextSchedule - the spark itself
	Overdwell, // overFireSparkAndPrepareNextSchedule - overdwell watchdog
	Fuel,      // turnInjectionPinHigh / turnInjectionPinLow

	Count
};

struct ExecLatenessStats {
	// Longest single dispatch delay in NT ticks
	efitick_t maxLateNt = 0;
	uint32_t executedEventCount = 0;
	uint32_t lateEventCount = 0;
};

class SingleTimerExecutor final : public Scheduler {
public:
	SingleTimerExecutor();
	void schedule(const char *msg, scheduling_s *scheduling, efitick_t timeNt, action_s const& action) override;
	void cancel(scheduling_s* scheduling) override;

	void onTimerCallback();
	int timerCallbackCounter = 0;
	int scheduleCounter = 0;
	int maxExecuteCounter = 0;
	int executeCounter;
	int executeAllPendingActionsInvocationCounter = 0;

	// Event execution lateness stats: scheduled moment vs the instant the
	// action actually started (see EventQueue::executeOne). The executor
	// fires from the TIM5 ISR, so a large maxLateNt means the dispatch was
	// delayed (IRQ locks / long higher-priority ISRs) - i.e. spark/injection
	// commands went out late. lateEventCount counts events >= 10 us late.
	// kindStats breaks the same data down per command class. Reset via
	// resetExecutionLatenessStats() (the board lockstats command prints and
	// resets these).
	efitick_t maxLateNt = 0;
	uint32_t executedEventCount = 0;
	uint32_t lateEventCount = 0;
	uint32_t lateHistogram[7] = {}; // <1, 1-4, 4-16, 16-64, 64-256, 256-1024, >=1024 us
	ExecLatenessStats kindStats[(int)ExecEventKind::Count] = {};
	void resetExecutionLatenessStats();
private:
	EventQueue queue;
	bool reentrantFlag = false;
	void executeAllPendingActions();
	void scheduleTimerCallback();
	void recordExecutionLateness(uint8_t kind, efitick_t late);
};

void initSingleTimerExecutorHardware();
void executorStatistics();

