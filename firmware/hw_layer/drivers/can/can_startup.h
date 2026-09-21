#pragma once

#include <cstddef>

// Used by both initCan() and host tests. In firmware, ops configures hardware
// and starts threads. In tests, it records calls so we can check what starts
// and in which order.
template <size_t BusCount, typename Ops>
bool startCan(bool readEnabled, bool writeEnabled, Ops& ops) {
	if (!readEnabled && !writeEnabled) {
		return false;
	}

	decltype(ops.getDevice(0)) devices[BusCount];
	bool anyCan = false;
	for (size_t index = 0; index < BusCount; index++) {
		devices[index] = ops.getDevice(index);
		for (size_t j = 0; j < index; j++) {
			if (devices[index] != nullptr && devices[index] == devices[j]) {
				ops.duplicateDevice(index, j);
				return false;
			}
		}
		anyCan |= devices[index] != nullptr;
	}

	if (!anyCan) {
		return false;
	}

	for (size_t index = 0; index < BusCount; index++) {
		if (devices[index]) {
			ops.configureDevice(index, devices[index]);
		}
	}

	// This worker also sends queued Lua/ISO-TP messages, so it must run even
	// when periodic CAN broadcasts are disabled.
	ops.startWriter();

	if (readEnabled) {
		for (size_t index = 0; index < BusCount; index++) {
			ops.startReader(index, devices[index]);
		}
		ops.startSniffer();
	}

	return true;
}
