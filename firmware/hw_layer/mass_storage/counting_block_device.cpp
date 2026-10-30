/**
 * @file	counting_block_device.cpp
 *
 * See counting_block_device.h
 */

#include "pch.h"

#include "counting_block_device.h"

static bool cbd_is_inserted(void *instance) {
	auto device = reinterpret_cast<CountingBlockDevice *>(instance);
	return blkIsInserted(device->inner);
}

static bool cbd_is_protected(void *instance) {
	auto device = reinterpret_cast<CountingBlockDevice *>(instance);
	return blkIsWriteProtected(device->inner);
}

static bool cbd_connect(void *instance) {
	auto device = reinterpret_cast<CountingBlockDevice *>(instance);
	return blkConnect(device->inner);
}

static bool cbd_disconnect(void *instance) {
	auto device = reinterpret_cast<CountingBlockDevice *>(instance);
	return blkDisconnect(device->inner);
}

static bool cbd_read(void *instance, uint32_t startblk, uint8_t *buffer, uint32_t n) {
	auto device = reinterpret_cast<CountingBlockDevice *>(instance);
	device->readOps++;
	bool ret = blkRead(device->inner, startblk, buffer, n);
	if (ret != HAL_SUCCESS) {
		device->readFailures++;
	}
	return ret;
}

static bool cbd_write(void *instance, uint32_t startblk, const uint8_t *buffer, uint32_t n) {
	auto device = reinterpret_cast<CountingBlockDevice *>(instance);
	device->writeOps++;
	bool ret = blkWrite(device->inner, startblk, buffer, n);
	if (ret != HAL_SUCCESS) {
		device->writeFailures++;
	}
	return ret;
}

static bool cbd_sync(void *instance) {
	auto device = reinterpret_cast<CountingBlockDevice *>(instance);
	bool ret = blkSync(device->inner);
	if (ret != HAL_SUCCESS) {
		device->syncFailures++;
	}
	return ret;
}

static bool cbd_get_info(void *instance, BlockDeviceInfo *bdip) {
	auto device = reinterpret_cast<CountingBlockDevice *>(instance);
	return blkGetInfo(device->inner, bdip);
}

static const struct BaseBlockDeviceVMT cbdVmt = {
	(size_t)0, // instanceOffset
	cbd_is_inserted,
	cbd_is_protected,
	cbd_connect,
	cbd_disconnect,
	cbd_read,
	cbd_write,
	cbd_sync,
	cbd_get_info
};

BaseBlockDevice *wrapCountingBlockDevice(CountingBlockDevice *device, BaseBlockDevice *inner) {
	device->vmt = &cbdVmt;
	device->state = BLK_READY;
	device->inner = inner;

	return reinterpret_cast<BaseBlockDevice *>(device);
}

void printCountingBlockDeviceStats(const CountingBlockDevice *device, const char *name) {
	efiPrintf("%s block ops: %lu reads (%lu failed), %lu writes (%lu failed), %lu failed syncs",
			name,
			device->readOps, device->readFailures,
			device->writeOps, device->writeFailures,
			device->syncFailures);
}
