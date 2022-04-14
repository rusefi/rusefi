/*
 * FragmentEntry.h
 *
 *  Created on: Jan 5, 2022
 * @author Andrey Belomutskiy, (c) 2012-2022
 */

#pragma once

struct FragmentEntry {
	FragmentEntry(const uint8_t *data, size_t size) {
		init(data, size);
	}

	FragmentEntry() {
	}

	const uint8_t *data = nullptr;
	size_t size = 0;

	void init(const uint8_t *data, size_t size) {
		this->data = data;
		this->size = size;
	}
};

void copyRange(uint8_t *destination,
		FragmentEntry *fragments, int fragmentsCount,
		size_t dataOffset, size_t dataLength);
