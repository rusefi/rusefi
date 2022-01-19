
#include "pch.h"

#include "block_cache.h"


static bool is_inserted(void* instance) {
	BlockCache* bc = reinterpret_cast<BlockCache*>(instance);

	chibios_rt::MutexLocker lock(bc->deviceMutex);

	// ask the underlying device, otherwise false
	auto backing = bc->backing;
	return bc ? backing->vmt->is_inserted(backing) : false;
}

static bool is_protected(void* instance) {
	BlockCache* bc = reinterpret_cast<BlockCache*>(instance);

	chibios_rt::MutexLocker lock(bc->deviceMutex);

	// ask the underlying device, otherwise false
	auto backing = bc->backing;
	return bc ? backing->vmt->is_inserted(backing) : false;
}

static bool connect(void* instance) {
	BlockCache* bc = reinterpret_cast<BlockCache*>(instance);

	chibios_rt::MutexLocker lock(bc->deviceMutex);

	if (auto backing = bc->backing) {
		return backing->vmt->connect(backing);
	}

	return HAL_FAILED;
}

static bool disconnect(void* instance) {
	BlockCache* bc = reinterpret_cast<BlockCache*>(instance);

	chibios_rt::MutexLocker lock(bc->deviceMutex);

	if (auto backing = bc->backing) {
		return backing->vmt->disconnect(backing);
	}

	return HAL_FAILED;
}

static bool read(void* instance, uint32_t startblk, uint8_t* buffer, uint32_t n) {
	if (n != 1) {
		return HAL_FAILED;
	}

	BlockCache* bc = reinterpret_cast<BlockCache*>(instance);

	if (!bc->backing) {
		return HAL_FAILED;
	}

	priv::handle* h;
	bc->free.fetch(&h, TIME_INFINITE);

	// copy request information
	h->startblk = startblk;
	h->buffer = buffer;

	// make the request
	bc->requests.post(h, TIME_INFINITE);
	// wait for it to complete
	bc->completed.fetch(&h, TIME_INFINITE);

	// stash the result
	auto result = h->result;

	// return the handle to the free list
	bc->free.post(h, TIME_INFINITE);

	return result;
}

static bool write(void* instance, uint32_t startblk, const uint8_t* buffer, uint32_t n) {
	BlockCache* bc = reinterpret_cast<BlockCache*>(instance);

	if (!bc->backing) {
		return HAL_FAILED;
	}

	chibios_rt::MutexLocker lock(bc->deviceMutex);
	efiPrintf("BC write %d", startblk);
	return bc->backing->vmt->write(bc->backing, startblk, buffer, n);
}

void BlockCache::thread() {
	while (true) {
		priv::handle* h;
		
		// Wait for a request to come in
		requests.fetch(&h, TIME_INFINITE);

		h->result = HAL_SUCCESS;

		auto startblk = h->startblk;

		// Did we prefetch the wrong block?
		if (startblk != cachedBlockId) {
			efiPrintf("BC miss %d", startblk);

			chibios_rt::MutexLocker lock(deviceMutex);

			// cache miss: do the read
			auto result = backing->vmt->read(backing, startblk, cachedBlockData, 1);

			if (result == HAL_SUCCESS) {
				cachedBlockId = startblk;
			} else {
				// read failed, invalidate cache
				cachedBlockId = -1;
			}

			h->result = result;
		} else {
			efiPrintf("BC hit %d", startblk);
		}

		// Copy from the cache to the output buffer
		memcpy(h->buffer, cachedBlockData, 512);

		// return the completed request
		completed.post(h, TIME_INFINITE);

		// Now that we have returned the requested block, prefetch the next block
		startblk++;

		{
			chibios_rt::MutexLocker lock(deviceMutex);

			efiPrintf("BC prefetch %d", startblk);
			auto result = backing->vmt->read(backing, startblk, cachedBlockData, 1);

			if (result == HAL_SUCCESS) {
				cachedBlockId = startblk;
			} else {
				// read failed, invalidate cache
				cachedBlockId = HAL_FAILED;
			}
		}
	}
}

static bool sync(void* instance) {
	BlockCache* bc = reinterpret_cast<BlockCache*>(instance);

	if (!bc->backing) {
		return HAL_FAILED;
	}

	chibios_rt::MutexLocker lock(bc->deviceMutex);
	return bc->backing->vmt->sync(bc->backing);
}

static bool get_info(void* instance, BlockDeviceInfo* bdip) {
	BlockCache* bc = reinterpret_cast<BlockCache*>(instance);

	if (!bc->backing) {
		return HAL_FAILED;
	}

	chibios_rt::MutexLocker lock(bc->deviceMutex);
	return bc->backing->vmt->get_info(bc->backing, bdip);
}

static const BaseBlockDeviceVMT blockCacheVmt = {
	(size_t)0, // instanceOffset
	is_inserted,
	is_protected,
	connect,
	disconnect,
	read,
	write,
	sync,
	get_info,
};

BlockCache::BlockCache() {
	vmt = &blockCacheVmt;

	// push all in to the free buffer
	for (int i = 0; i < sizeof(handles); i++) {
		free.post(&handles[i], TIME_INFINITE);
	}
}

void BlockCache::start() {
	chThdCreateStatic(wa, sizeof(wa), NORMALPRIO, [](void* instance) { reinterpret_cast<BlockCache*>(instance)->thread(); }, this);
}
