/**
 * @file	counting_block_device.h
 *
 * A pass-through BaseBlockDevice wrapper that counts operations and failures of the
 * device it wraps. The SCSI layer (lib_scsi) swallows blkRead/blkWrite errors, so this
 * wrapper is the only place where SD card I/O failures in USB MSD mode are observable.
 */

#pragma once

#include "hal.h"

struct CountingBlockDevice {
	const struct BaseBlockDeviceVMT *vmt;
	_base_block_device_data

	// device all calls are forwarded to
	BaseBlockDevice *inner;

	// cumulative since boot, survive LUN re-attach
	uint32_t readOps;
	uint32_t writeOps;
	uint32_t readFailures;
	uint32_t writeFailures;
	uint32_t syncFailures;
};

// Point 'device' at 'inner' and return it as a plain BaseBlockDevice.
// Counters are intentionally not reset - they accumulate across re-attaches.
BaseBlockDevice *wrapCountingBlockDevice(CountingBlockDevice *device, BaseBlockDevice *inner);

void printCountingBlockDeviceStats(const CountingBlockDevice *device, const char *name);
