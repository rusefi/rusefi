/**
 * @file	zero_device.cpp
 *
 * @date Feb 12, 2021
 * @author Matthew Kennedy, (c) 2021
 *
 * This file implements a special block device that simply reports "no media"
 * Use it when you need to mount *something* but don't have an SD card available.
 */

#include "hal.h"

struct NullDevice {
	const struct BaseBlockDeviceVMT *vmt;
	_base_block_device_data
};

extern NullDevice ND1;
