/**
 * @file	wave_analyzer.h
 *
 *
 * @date Jan 7, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef WAVE_ANALYZER_H_
#define WAVE_ANALYZER_H_

#include "main.h"
#include "datalogging.h"

#include "wave_analyzer_hw.h"
#include "wave_chart.h"

typedef struct {
	WaveReaderHw hw;
	char *name;
	volatile int eventCounter;

	volatile uint64_t lastActivityTimeUs; // timestamp in microseconds ticks
	volatile uint64_t periodEventTimeUs; // time of signal fall in microseconds
	volatile uint64_t widthEventTimeUs; // time of signal rise in microseconds

	volatile uint64_t signalPeriodUs; // period between two signal rises in microseconds

	volatile uint64_t waveOffsetUs; // offset from CKP signal in systimer ticks
	volatile uint64_t last_wave_low_widthUs; // time period in systimer ticks
	volatile uint64_t last_wave_high_widthUs; // time period in systimer ticks
} WaveReader;

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

void initWaveAnalyzer(void);
void pokeWaveInfo(void);
void reportWaveInfo(void);
int getWaveLowWidth(int index);
float getWaveHighWidthMs(int index);
uint64_t getWaveOffset(int index);

int getWaveMode(int index);

int getEventCounter(int index);

float getSignalPeriodMs(int index);
int getWidthEventTime(int index);
uint64_t getPeriodEventTime(int index);

//int getCrankStart();
//int getCrankPeriod();

void printWave(Logging *logging);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* WAVE_ANALYZER_H_ */
