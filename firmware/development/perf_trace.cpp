
#include "perf_trace.h"
#include "efitime.h"
#include "efifeatures.h"
#include "os_util.h"

#define TRACE_BUFFER_LENGTH 4096

enum class Phase : char
{
	Start,
	End,
	InstantThread,
	InstantGlobal,

};

struct TraceEntry
{
	PE Event;
	Phase Type;
	uint8_t Data;
	uint8_t ThreadId;
	uint32_t Timestamp;
};

static_assert(sizeof(TraceEntry) == 8);

static TraceEntry s_traceBuffer[TRACE_BUFFER_LENGTH];
static size_t s_nextIdx = 0;

void perfEventImpl(PE event, Phase type, uint8_t data)
{
	if constexpr (!ENABLE_PERF_TRACE)
	{
		return;
	}

	uint32_t timestamp = getTimeNowLowerNt();

	size_t idx;

	// Critical section: reserve index under lock
	{
		bool wasLocked = lockAnyContext();

		idx = s_nextIdx++;
		if (s_nextIdx >= TRACE_BUFFER_LENGTH)
		{
			s_nextIdx = 0;
		}

		if (!wasLocked) {
			unlockAnyContext();
		}
	}

	// We can safely write data out of the lock, our spot is reserved
	TraceEntry& entry = s_traceBuffer[idx];

	entry.Event = event;
	entry.Type = type;
	entry.ThreadId = 0; // TODO
	entry.Timestamp = timestamp;
	entry.Data = data;
}

void perfEventBegin(PE event, uint8_t data)
{
	perfEventImpl(event, Phase::Start, data);
}

void perfEventEnd(PE event, uint8_t data)
{
	perfEventImpl(event, Phase::End, data);
}

void perfEventInstantThread(PE event, uint8_t data)
{
	perfEventImpl(event, Phase::InstantThread, data);
}

void perfEventInstantGlobal(PE event, uint8_t data)
{
	perfEventImpl(event, Phase::InstantGlobal, data);
}
