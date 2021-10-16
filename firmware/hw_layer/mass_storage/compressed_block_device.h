/**
 * @file	compressed_block_device.h
 * @brief	This file implements a ChibiOS block device backed by a compressed (gzip) store.
 *
 * @date	Mar 4, 2021
 * @author	Matthew Kennedy, (c) 2021
 */

#pragma once

#include "hal.h"
#include "uzlib.h"

struct CompressedBlockDevice {
	const BaseBlockDeviceVMT* vmt;
	_base_block_device_data
	int32_t lastBlock;
	uzlib_uncomp d;
	uint8_t dictionary[32768];
	const uint8_t* source;
	size_t sourceSize;
};

void compressedBlockDeviceObjectInit(CompressedBlockDevice* cbd);
void compressedBlockDeviceStart(CompressedBlockDevice* cbd, const uint8_t* source, size_t sourceSize);
