
#include "efifeatures.h"
#include "perf_trace.h"
#include "efitime.h"
#include "os_util.h"

#define TRACE_BUFFER_LENGTH 2048

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

static_assert(sizeof(TraceEntry) == 8);

static TraceEntry s_traceBuffer[TRACE_BUFFER_LENGTH];
static size_t s_nextIdx = 0;

static bool s_isTracing = true;

void perfEventImpl(PE event, EPhase phase, uint8_t data)
{
	if constexpr (!ENABLE_PERF_TRACE) {
		return;
	}
	
	if (!s_isTracing) {
		return;
	}

	uint32_t timestamp = getTimeNowLowerNt();

	size_t idx;

	// Critical section: reserve index under lock
	{
		bool wasLocked = lockAnyContext();

		idx = s_nextIdx++;
		if (s_nextIdx >= TRACE_BUFFER_LENGTH) {
			s_nextIdx = 0;
			s_isTracing = false;
		}

		if (!wasLocked) {
			unlockAnyContext();
		}
	}

	// We can safely write data out of the lock, our spot is reserved
	volatile TraceEntry& entry = s_traceBuffer[idx];

	entry.Event = event;
	entry.Phase = phase;
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

size_t perfTraceEnable() {
	s_isTracing = true;

	return sizeof(s_traceBuffer);
}

const uint8_t* getTraceBuffer() {
	return reinterpret_cast<const uint8_t*>(s_traceBuffer);
}
