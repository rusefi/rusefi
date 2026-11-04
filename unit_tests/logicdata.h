/*
 * @file logicdata.h
 *
 *  Created on: Jul 19, 2020
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include <vector>
#include <cstddef>
#include <cstdint>

struct CompositeEvent {
    uint32_t timestamp;
    bool primaryTrigger;
    bool secondaryTrigger;
    bool isTDC;
    bool sync;
    bool coil;
    bool injector;
};

int getChannelState(int ch, const CompositeEvent* event);

// .teeth CSV files from firmware with the engine phase column carry it right
// after the timestamp ("Time[s], Phase, Primary, ..."); older captures go
// straight to "Primary". Readers that index columns by position shift by one
// when this returns true for the header line - see ToothLoggerWriteCsvHeader().
inline bool teethCsvHeaderHasPhase(const char* headerLine) {
	const char prefix[] = "Time[s], Phase,";
	for (size_t i = 0; i < sizeof(prefix) - 1; i++) {
		if (headerLine[i] != prefix[i]) {
			return false;
		}
	}
	return true;
}

void writeLogicDataFile(const char * fileName, const std::vector<CompositeEvent>& events);
