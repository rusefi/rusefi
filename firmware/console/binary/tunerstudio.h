/**
 * @file	tunerstudio.h
 *
 * @date Aug 26, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once
#include "global.h"
#include "tunerstudio_io.h"

typedef struct {
	int queryCommandCounter;
	int outputChannelsCommandCounter;
	int readPageCommandsCounter;
	int burnCommandCounter;
	int pageCommandCounter;
	int writeValueCommandCounter;
	int crc32CheckCommandCounter;
	int writeChunkCommandCounter;
	int errorCounter;
	int totalCounter;
	int textCommandCounter;
	int testCommandCounter;
} tunerstudio_counters_s;

extern tunerstudio_counters_s tsState;

void tunerStudioDebug(TsChannelBase* tsChannel, const char *msg);
void tunerStudioError(TsChannelBase* tsChannel, const char *msg);

char *getWorkingPageAddr();

#if EFI_TUNER_STUDIO
#include "thread_controller.h"
#include "thread_priority.h"


#define CONNECTIVITY_THREAD_STACK (2 * UTILITY_THREAD_STACK_SIZE)

/**
 * handle non CRC wrapped command
 */
bool handlePlainCommand(TsChannelBase* tsChannel, uint8_t command);

/**
 * this command is part of protocol initialization
 */
void handleQueryCommand(TsChannelBase* tsChannel, ts_response_format_e mode);

void updateTunerStudioState(TunerStudioOutputChannels *tsOutputChannels);
void printTsStats(void);
void requestBurn(void);

void startTunerStudioConnectivity(void);
void syncTunerStudioCopy(void);

#if defined __GNUC__
// GCC
#define pre_packed
#define post_packed __attribute__((packed))
#else
// IAR
#define pre_packed __packed
#define post_packed
#endif

typedef pre_packed struct
post_packed {
	short int offset;
	short int count;
} TunerStudioWriteChunkRequest;

class TunerstudioThread : public ThreadController<CONNECTIVITY_THREAD_STACK> {
public:
	TunerstudioThread(const char* name)
		: ThreadController(name, PRIO_CONSOLE)
	{
	}

	// Initialize and return the channel to use for this thread.
	virtual TsChannelBase* setupChannel() = 0;

	void ThreadTask() override;
};

#endif /* EFI_TUNER_STUDIO */

void handleWriteChunkCommand(TsChannelBase* tsChannel, ts_response_format_e mode, uint16_t offset, uint16_t count, void *content);
void handleBurnCommand(TsChannelBase* tsChannel, ts_response_format_e mode);
