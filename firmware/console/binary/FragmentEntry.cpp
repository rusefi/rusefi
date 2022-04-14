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
void copyRange(uint8_t *destination,
		FragmentEntry *fragments, int fragmentsCount,
		size_t dataOffset, size_t dataLength) {
	int fragmentIndex = 0;

	// scroll to starting fragment
	while (dataOffset > fragments[fragmentIndex].size && fragmentIndex <= fragmentsCount) {
		dataOffset -= fragments[fragmentIndex].size;
		fragmentIndex ++;
	}

	int destinationIndex = 0;

	while (dataLength > 0) {
		if (fragmentIndex > fragmentsCount) {
			// somehow we are past the end of fragments - fill with zeros
			memset(destination + destinationIndex, 0, dataLength);
			return;
		}

		int copyNowSize = minI(dataLength, fragments[fragmentIndex].size - dataOffset);
		const uint8_t *fromBase = fragments[fragmentIndex].data;
		if (fromBase == nullptr) {
			// we have no buffer for this fragment - fill with zeroes
			memset(destination + destinationIndex, 0, copyNowSize);
		} else {
			memcpy(destination + destinationIndex, fromBase + dataOffset, copyNowSize);
		}
		destinationIndex += copyNowSize;
		dataOffset = 0;
		dataLength -= copyNowSize;
		fragmentIndex++;
	}
}
