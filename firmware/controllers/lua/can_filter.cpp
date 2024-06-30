#include "pch.h"
#include "can_filter.h"
#include "can_hw.h"

#ifndef LUA_RX_MAX_FILTER_COUNT
#define LUA_RX_MAX_FILTER_COUNT 48
#endif

static size_t filterCount = 0;
static CanFilter filters[LUA_RX_MAX_FILTER_COUNT];

CanFilter* getFilterForId(size_t busIndex, int Id) {
	for (size_t i = 0; i < filterCount; i++) {
		auto& filter = filters[i];

		if (filter.accept(Id)) {
			if (filter.Bus == ANY_BUS || filter.Bus == (int)busIndex) {
				return &filter;
			}
		}
	}

	return nullptr;
}

void resetLuaCanRx() {
	// Clear all lua filters - reloading the script will reinit them
	filterCount = 0;
}

void addLuaCanRxFilter(int32_t eid, uint32_t mask, int bus, int callback) {
	if (filterCount >= LUA_RX_MAX_FILTER_COUNT) {
		criticalError("Too many Lua CAN RX filters");
	}

	efiPrintf("Added Lua CAN RX filter id 0x%x mask 0x%x with%s custom function", (unsigned int)eid, (unsigned int)mask, (callback == -1 ? "out" : ""));

	filters[filterCount].Id = eid;
	filters[filterCount].Mask = mask;
	filters[filterCount].Bus = bus;
	filters[filterCount].Callback = callback;

	filterCount++;
}
