/**
 * @file	zero_device.cpp
 *
 * @date Feb 12, 2021
 * @author Matthew Kennedy, (c) 2021
 *
 * This file implements a special block device that simply reports "no media"
 * Use it when you need to mount *something* but don't have an SD card available.
 */

#include "pch.h"

#include <cstring>

struct NullDevice {
	const struct BaseBlockDeviceVMT *vmt;
	_base_block_device_data
};

static bool nd_is_inserted(void*) {
	// This function is the whole point - we have no media!
	return false;
}

static bool nd_is_protected(void*) {
	return false;
}

static bool nd_return_success(void*) {
	return HAL_SUCCESS;
}

static bool nd_return_success_read(void*, uint32_t, uint8_t* buffer, uint32_t n) {
	// write zeroes to the buffer to prevent somebody reading random memory
	memset(buffer, 0, n);

	return HAL_SUCCESS;
}

static bool nd_return_success_write(void*, uint32_t, const uint8_t*, uint32_t) {
	return HAL_SUCCESS;
}

static bool nd_get_info(void*, BlockDeviceInfo* bdip) {
	// We have to report non-zero size here because Windows
	// will query the size of the block device even if we indicate
	// that the device has no media
	// If we report zeroes, it breaks USB until you unplug this device
	bdip->blk_num = 1000;
	bdip->blk_size = 512;
	return HAL_SUCCESS;
}

static const struct BaseBlockDeviceVMT ndVmt = {
	(size_t)0, // instanceOffset
	nd_is_inserted,
	nd_is_protected,

	// These functions just claim success to make the host happy
	nd_return_success,
	nd_return_success,
	nd_return_success_read,
	nd_return_success_write,
	nd_return_success,
	nd_get_info
};

// This device is always ready and has no state
NullDevice ND1 = { &ndVmt, BLK_READY };
