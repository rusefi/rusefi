
#pragma once

#include "ch.hpp"
#include "hal.h"

namespace priv
{
struct handle {
	msg_t result;

	// Read request parameters
	uint32_t startblk;
	uint8_t* buffer;
};
};

struct BlockCache {
	const BaseBlockDeviceVMT* vmt;
	_base_block_device_data

	BaseBlockDevice* backing;

	BlockCache();

	static constexpr int handleCount = 1;

	chibios_rt::Mailbox<priv::handle*, handleCount> requests;
	chibios_rt::Mailbox<priv::handle*, handleCount> completed;
	chibios_rt::Mailbox<priv::handle*, handleCount> free;

	priv::handle handles[handleCount];

	chibios_rt::Mutex deviceMutex;

	void thread();
	void start();



	int32_t cachedBlockId = -1;
	uint8_t cachedBlockData[512];

	THD_WORKING_AREA(wa, UTILITY_THREAD_STACK_SIZE);
};
