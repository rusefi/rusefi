/*
 * FragmentEntry.cpp
 *
 *  Created on: Jan 5, 2022
 * @author Andrey Belomutskiy, (c) 2012-2022
 */

#include "pch.h"
#include "FragmentEntry.h"

/**
 * copy dataLength of fragmented outputs starting at dataOffset into destination starting at zero
 */
void copyRange(uint8_t *destination, FragmentEntry *fragments, size_t dataOffset, size_t dataLength) {
	int fragmentIndex = 0;

	// scroll to starting fragment
	while (dataOffset > fragments[fragmentIndex].size) {
		dataOffset -= fragments[fragmentIndex].size;
		fragmentIndex ++;
	}

	int destinationIndex = 0;

	while (dataLength > 0) {
		int copyNowSize = minI(dataLength, fragments[fragmentIndex].size - dataOffset);
		memcpy(destination + destinationIndex, fragments[fragmentIndex].data + dataOffset, copyNowSize);
		destinationIndex += copyNowSize;
		dataOffset = 0;
		dataLength -= copyNowSize;
		fragmentIndex++;
	}
}
