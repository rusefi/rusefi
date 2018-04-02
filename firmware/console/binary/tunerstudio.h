/**
 * @file	tunerstudio.h
 *
 * @date Aug 26, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef TUNERSTUDIO_H_
#define TUNERSTUDIO_H_

#include "main.h"
#include "tunerstudio_io.h"

#if EFI_TUNER_STUDIO
#include "tunerstudio_configuration.h"
#include "engine.h"

#include <stdint.h>

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

/**
 * handle non CRC wrapped command
 */
bool handlePlainCommand(ts_channel_s *tsChannel, uint8_t command);
int tunerStudioHandleCrcCommand(ts_channel_s *tsChannel, char *data, int incomingPacketSize);

/**
 * rusEfi own test command
 */
void handleTestCommand(ts_channel_s *tsChannel);
/**
 * this command is part of protocol initialization
 */
void handleQueryCommand(ts_channel_s *tsChannel, ts_response_format_e mode);
/**
 * Gauges refresh
 */
void handleOutputChannelsCommand(ts_channel_s *tsChannel, ts_response_format_e mode);

char *getWorkingPageAddr(int pageIndex);
int getTunerStudioPageSize(int pageIndex);
void handleWriteValueCommand(ts_channel_s *tsChannel, ts_response_format_e mode, uint16_t page, uint16_t offset, uint8_t value);
void handleWriteChunkCommand(ts_channel_s *tsChannel, ts_response_format_e mode, short offset, short count, void *content);
void handlePageSelectCommand(ts_channel_s *tsChannel, ts_response_format_e mode, uint16_t pageId);
void handlePageReadCommand(ts_channel_s *tsChannel, ts_response_format_e mode, uint16_t pageId, uint16_t offset, uint16_t count);
void handleBurnCommand(ts_channel_s *tsChannel, ts_response_format_e mode, uint16_t page);

void tunerStudioDebug(const char *msg);
void tunerStudioError(const char *msg);

void updateTunerStudioState(TunerStudioOutputChannels *tsOutputChannels DECLARE_ENGINE_PARAMETER_SUFFIX);
void printTsStats(void);
void requestBurn(void);

void startTunerStudioConnectivity(void);
void syncTunerStudioCopy(void);
void runBinaryProtocolLoop(ts_channel_s *tsChannel, bool isConsoleRedirect);

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
	} TunerStudioOchRequest;


	typedef pre_packed struct
	post_packed {
		short int page;
		short int offset;
		short int count;
	} TunerStudioWriteChunkRequest;

	typedef pre_packed struct
		post_packed {
			short int page;
			short int offset;
			short int count;
		} TunerStudioReadRequest;

		typedef pre_packed struct
			post_packed {
				short int offset;
				unsigned char value;
			} TunerStudioWriteValueRequest;

#endif /* EFI_TUNER_STUDIO */

#endif /* TUNERSTUDIO_H_ */
