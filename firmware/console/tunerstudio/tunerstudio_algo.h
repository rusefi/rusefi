/**
 * @file	tunerstudio_algo.h
 *
 * @date Oct 22, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef TUNERSTUDIO_ALGO_H_
#define TUNERSTUDIO_ALGO_H_

#define TS_SIGNATURE "MShift v0.01"

#include <stdint.h>

// response codes

#define TS_RESPONSE_OK 0x00
#define TS_RESPONSE_BURN_OK 0x04
#define TS_RESPONSE_CRC_FAILURE 0x82

typedef enum {
	TS_PLAIN = 0,
	TS_CRC = 1
} ts_response_format_e;

typedef struct {
	int queryCommandCounter;
	int outputChannelsCommandCounter;
	int readPageCommandsCounter;
	int burnCommandCounter;
	int pageCommandCounter;
	int writeValueCommandCounter;
	int writeChunkCommandCounter;
	int errorCounter;
	/**
	 * For some reason I could not get the 'pages' feature of Tuner Studio working - as
	 * a workaround the whole configuration is just one page
	 *
	 * this field is in the end to simply aligning situation
	 */
	short currentPageId;
} TunerStudioState;

int tunerStudioHandleCrcCommand(uint8_t *data, int incomingPacketSize);

void handleTestCommand(void);
void handleQueryCommand(ts_response_format_e mode);
void tsSendResponse(ts_response_format_e mode, const uint8_t * buffer, int size);
void handleOutputChannelsCommand(ts_response_format_e mode);

char *getWorkingPageAddr(int pageIndex);
int getTunerStudioPageSize(int pageIndex);
void handleWriteValueCommand(ts_response_format_e mode, uint16_t page, uint16_t offset, uint8_t value);
void handleWriteChunkCommand(ts_response_format_e mode, short offset, short count, void *content);
void handlePageSelectCommand(ts_response_format_e mode, uint16_t pageId);
void handlePageReadCommand(ts_response_format_e mode, uint16_t pageId, uint16_t offset, uint16_t count);
void handleBurnCommand(ts_response_format_e mode, uint16_t page);

void tunerStudioWriteData(const uint8_t * buffer, int size);
void tunerStudioDebug(const char *msg);

void tunerStudioError(const char *msg);

#define TS_HELLO_COMMAND 'H'
#define TS_OUTPUT_COMMAND 'O'
#define TS_READ_COMMAND 'R'
#define TS_PAGE_COMMAND 'P'

#define TS_SINGLE_WRITE_COMMAND 'W'
#define TS_CHUNK_WRITE_COMMAND 'C'
#define TS_BURN_COMMAND 'B'

#endif /* TUNERSTUDIO_ALGO_H_ */
