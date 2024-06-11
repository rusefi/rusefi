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
#define DO_NOT_LOG nullptr
void sendErrorCode(TsChannelBase *tsChannel, uint8_t code, /*empty line by default, use nullptr not to log*/const char *msg="");

uint8_t* getWorkingPageAddr();

void requestBurn();
// Lua script might want to know how long since last TS request to see if unit is being actively monitored
int getSecondsSinceChannelsRequest();

#if EFI_TUNER_STUDIO
#include "thread_controller.h"
#include "thread_priority.h"

void updateTunerStudioState();

void startTunerStudioConnectivity(void);

typedef struct {
	short int offset;
	short int count;
} TunerStudioWriteChunkRequest;

#if EFI_PROD_CODE || EFI_SIMULATOR
#define CONNECTIVITY_THREAD_STACK (3 * UTILITY_THREAD_STACK_SIZE)

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
#endif

#endif /* EFI_TUNER_STUDIO */
