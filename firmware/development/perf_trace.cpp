/**
 * @file perf_trace.cpp
 *
 * https://github.com/rusefi/rusefi/wiki/Developer-Performance-Tracing
 *
 * See JsonOutput.java in rusEfi console
 */


#include "pch.h"

#include "os_util.h"

#ifndef ENABLE_PERF_TRACE
#error ENABLE_PERF_TRACE must be defined!
#endif

#ifndef TRACE_BUFFER_LENGTH
#define TRACE_BUFFER_LENGTH 2048
#endif /* TRACE_BUFFER_LENGTH */

// Disable the buffer if we're not enabled at all
#if !ENABLE_PERF_TRACE
#undef TRACE_BUFFER_LENGTH
#define TRACE_BUFFER_LENGTH 1
#endif

enum class EPhase : char
{
	Start,
	End,
	InstantThread,
	InstantGlobal,
};

struct TraceEntry
{
	PE Event;
	EPhase Phase;
	int8_t IsrId;
	uint8_t ThreadId;
	uint32_t Timestamp;
};

// Ensure that the struct is the size we think it is - the binary layout is important
static_assert(sizeof(TraceEntry) == 8);

// This buffer stores a trace - we write the full buffer once, then disable tracing
static TraceEntry s_traceBuffer[TRACE_BUFFER_LENGTH];
static size_t s_nextIdx = 0;

static bool s_isTracing = false;

static void perfEventImpl(PE event, EPhase phase)
{
	// Bail if we aren't allowed to trace
	if constexpr (!ENABLE_PERF_TRACE) {
		return;
	}
	
	// Bail if we aren't tracing
	if (!s_isTracing) {
		return;
	}

	// todo: why doesn't getTimeNowLowerNt() work here?
	// It returns 0 like we're in a unit test
	uint32_t timestamp = port_rt_get_counter_value();

	size_t idx;

	// Critical section: disable interrupts to reserve an index.
	// We could lock, but this gets called a LOT - so locks could
	// significantly alter the results of the measurement.
	// In addition, if we want to trace lock/unlock events, we can't
	// be locking ourselves from the trace functionality.
	{
		uint32_t prim = __get_PRIMASK();
		__disable_irq();

		idx = s_nextIdx++;
		if (s_nextIdx >= TRACE_BUFFER_LENGTH) {
			s_nextIdx = 0;
			s_isTracing = false;
		}

		// Restore previous interrupt state - don't restore if they weren't enabled
		if (!prim) {
			__enable_irq();
		}
	}

	// We can safely write data out of the lock, our spot is reserved
	volatile TraceEntry& entry = s_traceBuffer[idx];

	entry.Event = event;
	entry.Phase = phase;
	// Get the current active interrupt - this is the "process ID"
	auto isr = static_cast<int8_t>(SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk);
	entry.IsrId = isr - 16;

	// Get the current thread (if not interrupt) and use as the thread ID
	if (isr == 0) {
		entry.ThreadId = chThdGetSelfX()->threadId;
	} else {
		// Interrupts have no thread - all are T0
		entry.ThreadId = 0;
	}

	entry.Timestamp = timestamp;
}

void perfEventBegin(PE event) {
	perfEventImpl(event, EPhase::Start);
}

void perfEventEnd(PE event) {
	perfEventImpl(event, EPhase::End);
}

void perfEventInstantGlobal(PE event) {
	perfEventImpl(event, EPhase::InstantGlobal);
}

void perfTraceEnable() {
	s_isTracing = true;
}

const TraceBufferResult perfTraceGetBuffer() {
	// stop tracing if you try to get the buffer early
	s_isTracing = false;

	return {reinterpret_cast<const uint8_t*>(s_traceBuffer), sizeof(s_traceBuffer)};
}
