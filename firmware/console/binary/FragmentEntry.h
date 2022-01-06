/*
 * FragmentEntry.h
 *
 *  Created on: Jan 5, 2022
 * @author Andrey Belomutskiy, (c) 2012-2022
 */

#pragma once

struct FragmentEntry {
	FragmentEntry(uint8_t *data, size_t size) {
		this->data = data;
		this->size = size;

	}

	uint8_t *data;
	size_t size;
};

void copyRange(uint8_t *destination, FragmentEntry *fragments, size_t dataOffset, size_t dataLength);
