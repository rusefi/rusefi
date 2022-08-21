// can_filter.h

#pragma once

#define FILTER_SPECIFIC 0x1FFFFFFF
#define ANY_BUS -1

struct CanFilter {
	int32_t Id;
	int32_t Mask;

	int Bus;
	int Callback;

	bool accept(int Id) {
	    return (Id & this->Mask) == this->Id;
	}
};

// Called when the user script is unloaded, resets any CAN rx filters
void resetLuaCanRx();
// Adds a frame ID to listen to
void addLuaCanRxFilter(int32_t eid, uint32_t mask, int bus, int callback);

CanFilter* getFilterForId(size_t busIndex, int Id);
