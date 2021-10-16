/**
 * @file perf_trace.h
 *
 */
#pragma once

#include <cstdint>
#include <cstddef>

// Defines different events we want to trace.  These can be an interval (begin -> end), or an
// instant.  Instants can be global, or specific to one thread.  You probably don't want to use
// each element in PE more than once, as they should each indicate that a specific thing began,
// ended, or occurred.
enum class PE : uint8_t {
	// The tag below is consumed by PerfTraceTool.java which generates EnumNames.java
	// enum_start_tag
	INVALID,
	ISR,
	ContextSwitch,
	OutputPinSetValue,
	DecodeTriggerEvent,
	EnginePeriodicFastCallback,
	EnginePeriodicSlowCallback,
	EngineStatePeriodicFastCallback,
	HandleShaftSignal,
	EventQueueInsertTask,
	EventQueueExecuteAll,
	SingleTimerExecutorDoExecute,
	SingleTimerExecutorScheduleTimerCallback,
	PeriodicControllerPeriodicTask,
	PeriodicTimerControllerPeriodicTask,
	AdcCallbackFast,
	AdcProcessSlow,
	AdcConversionSlow,
	AdcConversionFast,
	AdcSubscriptionUpdateSubscribers,
	GetRunningFuel,
	GetInjectionDuration,
	HandleFuel,
	MainTriggerCallback,
	OnTriggerEventSparkLogic,
	ShaftPositionListeners,
	GetBaseFuel,
	GetTpsEnrichment,
	GetSpeedDensityFuel,
	WallFuelAdjust,
	MapAveragingTriggerCallback,
	Unused1,
	SingleTimerExecutorScheduleByTimestamp,
	GetTimeNowUs,
	EventQueueExecuteCallback,
	PwmGeneratorCallback,
	TunerStudioHandleCrcCommand,
	Unused,
	PwmConfigStateChangeCallback,
	Temporary1,
	Temporary2,
	Temporary3,
	Temporary4,
	EngineSniffer,
	PrepareIgnitionSchedule,
	Hip9011IntHoldCallback,
	GlobalLock,
	GlobalUnlock,
	SoftwareKnockProcess,
	LogTriggerTooth,
	LuaTickFunction,
	// enum_end_tag
	// The tag above is consumed by PerfTraceTool.java
	// please note that the tool requires a comma at the end of last value
};

void perfEventBegin(PE event);
void perfEventEnd(PE event);
void perfEventInstantGlobal(PE event);

// Enable one buffer's worth of perf tracing, and retrieve the buffer size in bytes
void perfTraceEnable();

struct TraceBufferResult
{
	const uint8_t* const Buffer;
	const size_t Size;
};

// Retrieve the trace buffer
const TraceBufferResult perfTraceGetBuffer();

#if ENABLE_PERF_TRACE
class ScopePerf
{
public:
	ScopePerf(PE event) : m_event(event) {
		perfEventBegin(event);
	}

	~ScopePerf()
	{
		perfEventEnd(m_event);
	}

private:
	const PE m_event;
};

#else /* if ENABLE_PERF_TRACE */

struct ScopePerf {
	ScopePerf(PE) {}
};

#endif /* ENABLE_PERF_TRACE */
