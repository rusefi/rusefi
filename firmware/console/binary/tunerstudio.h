/**
 * @file	tunerstudio.h
 *
 * @date Aug 26, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once
#include "global.h"
#include "tunerstudio_io.h"

#define TS_PAGE_SETTINGS			0x0000
// Issue TS zeroes LSB byte of pageIdentifier
#define TS_PAGE_SCATTER_OFFSETS		0x0100
#define TS_PAGE_LTFT_TRIMS			0x0200
#define TS_PAGE_LUA					0x0300

typedef struct {
	int queryCommandCounter;
	int outputChannelsCommandCounter;
	int readPageCommandsCounter;
	int readScatterCommandsCounter;
	int burnCommandCounter;
	int crc32CheckCommandCounter;
	int writeChunkCommandCounter;
	int totalCounter;
	int textCommandCounter;
	int testCommandCounter;

	// overall counter, not all of this errors are reported back to TS
	int errorCounter;
	// by type error counters reported to TS
	int errorUnderrunCounter;
	int errorOverrunCounter;
	int errorCrcCounter;
	int errorUnrecognizedCommand;
	int errorOutOfRange;
	int errorOther;
} tunerstudio_counters_s;

extern tunerstudio_counters_s tsState;

void tunerStudioDebug(TsChannelBase* tsChannel, const char *msg);
void tunerStudioError(TsChannelBase* tsChannel, const char *msg);
#define DO_NOT_LOG nullptr
void sendErrorCode(TsChannelBase *tsChannel, uint8_t code, /*empty line by default, use nullptr not to log*/const char *msg="");

void requestBurn();

// Lua script might want to know how long since last TS request to see if unit is being actively monitored
int getSecondsSinceChannelsRequest();

#if EFI_TUNER_STUDIO
#include "thread_controller.h"
#include "thread_priority.h"

void updateTunerStudioState();

bool isTuningVeNow();
void startTunerStudioConnectivity();
bool needToTriggerTsRefresh();
void onApplyPreset();

struct TunerStudioRWChunkRequest {
	uint16_t offset;
	uint16_t count;
} __attribute__((packed));
static_assert(sizeof(TunerStudioRWChunkRequest) == 4);

struct TunerStudioPageRWChunkRequest {
	uint16_t page;
	uint16_t offset;
	uint16_t count;
} __attribute__((packed));
static_assert(sizeof(TunerStudioPageRWChunkRequest) == 6);

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

bool isTouchingArea(uint16_t offset, uint16_t count, int areaStart, int areaSize);

#endif /* EFI_TUNER_STUDIO */
