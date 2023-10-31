// can_filter.h

#pragma once

#include "can.h"

#define FILTER_SPECIFIC 0x1FFFFFFF
#define NO_CALLBACK -1

struct CanFilter {
	int32_t Id;
	int32_t Mask;

	CanBusIndex Bus;
	int Callback;

	bool accept(int Id) {
		return (Id & this->Mask) == this->Id;
	}
};

// Called when the user script is unloaded, resets any CAN rx filters
void resetLuaCanRx();
// Adds a frame ID to listen to
void addLuaCanRxFilter(int32_t eid, uint32_t mask, CanBusIndex bus, int callback);

CanFilter* getFilterForId(CanBusIndex busIndex, int Id);
