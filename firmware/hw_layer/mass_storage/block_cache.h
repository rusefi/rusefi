
#pragma once

#include "ch.hpp"
#include "hal.h"
struct BlockCache {
	const BaseBlockDeviceVMT* vmt;
	_base_block_device_data

	BaseBlockDevice* backing;
	chibios_rt::Mutex deviceMutex;

	BlockCache();

	void start(BaseBlockDevice* backing);

	bool read(uint32_t startblk, uint8_t* buffer);

private:
	void thread();
	bool fetchBlock(uint32_t blockId);

	struct Handle {
		// Read request parameters
		uint32_t startblk;
		uint8_t* buffer;

		// Returned result
		bool result;
	};

	static constexpr int handleCount = 1;

	chibios_rt::Mailbox<Handle*, handleCount> m_requests;
	chibios_rt::Mailbox<Handle*, handleCount> m_completed;
	chibios_rt::Mailbox<Handle*, handleCount> m_free;

	Handle m_handles[handleCount];

	int32_t m_cachedBlockId = -1;
	uint8_t m_cachedBlockData[512];

	THD_WORKING_AREA(wa, 5 * UTILITY_THREAD_STACK_SIZE);
};
