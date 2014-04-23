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

typedef struct {
	int queryCommandCounter;
	int outputChannelsCommandCounter;
	int readPageCommandsCounter;
	int burnCommandCounter;
	int errorCounter;
} TunerStudioState;

int tunerStudioHandleCommand(short command);

void handleTestCommand(void);
void handleQueryCommand(void);
void handleOutputChannelsCommand(void);

char *getWorkingPageAddr(int pageIndex);
int getTunerStudioPageSize(int pageIndex);
void handleValueWriteCommand(void);
void handlePageReadCommand(void);
void handleBurnCommand(void);

void tunerStudioWriteData(const uint8_t * buffer, int size);
void tunerStudioDebug(char *msg);

#endif /* TUNERSTUDIO_ALGO_H_ */
