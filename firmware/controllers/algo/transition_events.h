// transition_events.h
// also known as 'special event counter'

#pragma once

#include "pch.h"

enum class TransitionEvent : char
{
	None = 0,
	BurnRequest = 1,
	TriggerError = 2,
	EngineResync = 3,
	GlobalConfigurationVersion = 4,
	EtbPidReset = 5,
	CruiseControl = 6,
};

inline void onTransitionEvent(TransitionEvent event) {
#if EFI_PROD_CODE
  engine->outputChannels.transitionEventCode = (int)event;
  engine->outputChannels.transitionEventsCounter++;
#endif
}
