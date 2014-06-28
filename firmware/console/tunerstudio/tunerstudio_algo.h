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

// http://en.wikipedia.org/wiki/Endianness

#define SWAP_UINT16(x) ((x) << 8) | ((x) >> 8)

#define SWAP_UINT32(x) (((x) >> 24) & 0xff) | (((x) << 8) & 0xff0000) | (((x) >> 8) & 0xff00) | (((x) << 24) & 0xff000000)

// response codes

#define TS_RESPONSE_OK 0x00
#define TS_RESPONSE_BURN_OK 0x04
#define TS_RESPONSE_CRC_FAILURE 0x82

typedef struct {
	int queryCommandCounter;
	int outputChannelsCommandCounter;
	int readPageCommandsCounter;
	int burnCommandCounter;
	int pageCommandCounter;
	int writeValueCommandCounter;
	int writeChunkCommandCounter;
	int errorCounter;
	// this field is in the end to simply aligning situation
	short currentPageId;
} TunerStudioState;

int tunerStudioHandleCommand(char *data, int incomingPacketSize);

void handleTestCommand(void);
void handleQueryCommand(int needCrc);
void handleOutputChannelsCommand(void);

char *getWorkingPageAddr(int pageIndex);
int getTunerStudioPageSize(int pageIndex);
void handleWriteValueCommand(uint16_t page, uint16_t offset, uint8_t value);
void handleWriteChunkCommand(short offset, short count, void *content);
void handlePageSelectCommand(uint16_t pageId);
void handlePageReadCommand(uint16_t pageId, uint16_t offset, uint16_t count);
void handleBurnCommand(uint16_t page);

void tunerStudioWriteData(const uint8_t * buffer, int size);
void tunerStudioDebug(char *msg);

#define TS_HELLO_COMMAND 'H'
#define TS_OUTPUT_COMMAND 'O'
#define TS_READ_COMMAND 'R'
#define TS_PAGE_COMMAND 'P'

#define TS_SINGLE_WRITE_COMMAND 'W'
#define TS_CHUNK_WRITE_COMMAND 'C'
#define TS_BURN_COMMAND 'B'

#endif /* TUNERSTUDIO_ALGO_H_ */
