/**
 * @file	wave_analyzer.h
 *
 * todo: rename all this 'logic analyzer' is probably much more appropriate
 *
 * @date Jan 7, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef WAVE_ANALYZER_H_
#define WAVE_ANALYZER_H_

#include "main.h"

#if EFI_WAVE_ANALYZER || defined(__DOXYGEN__)

#include "datalogging.h"

#include "digital_input_hw.h"
#include "engine_sniffer.h"

#define WA_CHANNEL_1 "input1"
#define WA_CHANNEL_2 "input2"
#define WA_CHANNEL_3 "input3"
#define WA_CHANNEL_4 "input4"

class WaveReader {
public:
	WaveReader();
	void onFallEvent();

	digital_input_s *hw;
	const char *name;
	volatile int fallEventCounter;
	volatile int riseEventCounter;

	int currentRevolutionCounter;

	/**
	 * Total ON time during last engine cycle
	 */
	efitimeus_t prevTotalOnTimeUs;

	efitimeus_t totalOnTimeAccumulatorUs;

	volatile efitimeus_t lastActivityTimeUs; // timestamp in microseconds ticks
	/**
	 * time of signal fall event, in microseconds
	 */
	volatile efitimeus_t periodEventTimeUs;
	volatile efitimeus_t widthEventTimeUs; // time of signal rise in microseconds

	volatile efitimeus_t signalPeriodUs; // period between two signal rises in microseconds

	/**
	 * offset from engine cycle start in microseconds
	 */
	volatile efitimeus_t waveOffsetUs;
	volatile efitimeus_t last_wave_low_widthUs; // time period in systimer ticks
	volatile efitimeus_t last_wave_high_widthUs; // time period in systimer ticks
};

void initWaveAnalyzer(Logging *sharedLogger);
void printWave(Logging *logging);
void showWaveInfo(void);

#endif

#endif /* WAVE_ANALYZER_H_ */

