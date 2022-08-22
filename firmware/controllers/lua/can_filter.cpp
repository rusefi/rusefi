#include "pch.h"
#include "can_filter.h"
#include "can_hw.h"

static constexpr size_t maxFilterCount = 48;

static size_t filterCount = 0;
static CanFilter filters[maxFilterCount];

CanFilter* getFilterForId(size_t busIndex, int Id) {
	for (size_t i = 0; i < filterCount; i++) {
		auto& filter = filters[i];

		if (filter.accept(Id)) {
			if (filter.Bus == ANY_BUS || filter.Bus == busIndex) {
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
    if (bus != ANY_BUS) {
        assertHwCanBusIndex(bus);
    }

	if (filterCount >= maxFilterCount) {
		firmwareError(OBD_PCM_Processor_Fault, "Too many Lua CAN RX filters");
	}

	efiPrintf("Added Lua CAN RX filter id 0x%x mask 0x%x with%s custom function", eid, mask, (callback == -1 ? "out" : ""));

	filters[filterCount].Id = eid;
	filters[filterCount].Mask = mask;
	filters[filterCount].Bus = bus;
	filters[filterCount].Callback = callback;

	filterCount++;
}
