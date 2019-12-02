/**
 * @file perf_trace.cpp
 *
 * See JsonOutput.java in rusEfi console
 */


#include "efifeatures.h"
#include "perf_trace.h"
#include "efitime.h"
#include "os_util.h"


#ifndef TRACE_BUFFER_LENGTH
#define TRACE_BUFFER_LENGTH 2048
#endif /* TRACE_BUFFER_LENGTH */

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
	uint8_t Data;
	uint8_t ThreadId;
	uint32_t Timestamp;
};

// Ensure that the struct is the size we think it is - the binary layout is important
static_assert(sizeof(TraceEntry) == 8);

// This buffer stores a trace - we write the full buffer once, then disable tracing
static TraceEntry s_traceBuffer[TRACE_BUFFER_LENGTH];
static size_t s_nextIdx = 0;

static bool s_isTracing = false;

void perfEventImpl(PE event, EPhase phase, uint8_t data)
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
		__disable_irq();

		idx = s_nextIdx++;
		if (s_nextIdx >= TRACE_BUFFER_LENGTH) {
			s_nextIdx = 0;
			s_isTracing = false;
		}

		__enable_irq();
	}

	// We can safely write data out of the lock, our spot is reserved
	volatile TraceEntry& entry = s_traceBuffer[idx];

	entry.Event = event;
	entry.Phase = phase;
	// Get the current active interrupt - this is the "thread ID"
	entry.ThreadId = static_cast<uint8_t>(SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk);
	entry.Timestamp = timestamp;
	entry.Data = data;
}

void perfEventBegin(PE event, uint8_t data) {
	perfEventImpl(event, EPhase::Start, data);
}

void perfEventEnd(PE event, uint8_t data) {
	perfEventImpl(event, EPhase::End, data);
}

void perfEventInstantGlobal(PE event, uint8_t data) {
	perfEventImpl(event, EPhase::InstantGlobal, data);
}

void perfTraceEnable() {
	s_isTracing = true;
}

const TraceBufferResult perfTraceGetBuffer() {
	// stop tracing if you try to get the buffer early
	s_isTracing = false;

	return {reinterpret_cast<const uint8_t*>(s_traceBuffer), sizeof(s_traceBuffer)};
}
