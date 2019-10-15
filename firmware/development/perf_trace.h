
#pragma once

#include <cstdint>
#include <cstddef>

// Defines different events we want to trace.  These can be an interval (begin -> end), or an
// instant.  Instants can be global, or specific to one thread.  You probably don't want to use
// each element in PE more than once, as they should each indicate that a specific thing began,
// ended, or occured.
enum class PE : uint8_t {
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
	AdcCallbackSlow,
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
	AdcCallbackFastComplete,
	SingleTimerExecutorScheduleByTimestamp,
	ScheduleByAngle,
	EventQueueExecuteCallback,
	PwmGeneratorCallback,
	TunerStudioHandleCrcCommand,
	PwmConfigTogglePwmState,
	PwmConfigStateChangeCallback,
};

void perfEventBegin(PE event, uint8_t data);
void perfEventEnd(PE event, uint8_t data);
void perfEventInstantGlobal(PE event, uint8_t data);

inline void perfEventBegin(PE event) {
	perfEventBegin(event, 0);
}

inline void perfEventEnd(PE event) {
	perfEventEnd(event, 0);
}

inline void perfEventInstantGlobal(PE event) {
	perfEventInstantGlobal(event, 0);
}

// Enable one buffer's worth of perf tracing, and retrieve the buffer size in bytes
size_t perfTraceEnable();
// Retrieve the trace buffer
const uint8_t* getTraceBuffer();

class ScopePerf
{
public:
	ScopePerf(PE event) : ScopePerf(event, 0) {}

	ScopePerf(PE event, uint8_t data) : m_event(event), m_data(data)
	{
		perfEventBegin(event, data);
	}

	~ScopePerf()
	{
		perfEventEnd(m_event, m_data);
	}

private:
	const PE m_event;
	const uint8_t m_data;
};
