/**
 * @file	wave_analyzer.h
 *
 *
 * @date Jan 7, 2013
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#ifndef WAVE_ANALYZER_H_
#define WAVE_ANALYZER_H_

#include "main.h"

#if EFI_WAVE_ANALYZER || defined(__DOXYGEN__)

#include "datalogging.h"

#include "wave_analyzer_hw.h"
#include "wave_chart.h"

#define WA_CHANNEL_1 "input1"
#define WA_CHANNEL_2 "input2"
#define WA_CHANNEL_3 "input3"
#define WA_CHANNEL_4 "input4"

class WaveReader {
public:
	void onFallEvent();

	WaveReaderHw hw;
	const char *name;
	volatile int eventCounter;

	int currentRevolutionCounter;

	/**
	 * Total ON time during last engine cycle
	 */
	uint64_t prevTotalOnTimeUs;

	uint64_t totalOnTimeAccumulatorUs;

	volatile uint64_t lastActivityTimeUs; // timestamp in microseconds ticks
	/**
	 * time of signal fall event, in microseconds
	 */
	volatile uint64_t periodEventTimeUs;
	volatile uint64_t widthEventTimeUs; // time of signal rise in microseconds

	volatile uint32_t signalPeriodUs; // period between two signal rises in microseconds

	/**
	 * offset from engine cycle start in microseconds
	 */
	volatile uint64_t waveOffsetUs;
	volatile uint32_t last_wave_low_widthUs; // time period in systimer ticks
	volatile uint64_t last_wave_high_widthUs; // time period in systimer ticks
};

void initWaveAnalyzer(Logging *sharedLogger);
void printWave(Logging *logging);

#endif

#endif /* WAVE_ANALYZER_H_ */

