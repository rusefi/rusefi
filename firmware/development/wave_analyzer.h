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

#include "global.h"

#if EFI_WAVE_ANALYZER

#include "digital_input_icu.h"
#include "engine_sniffer.h"

class WaveReader {
public:
	WaveReader();
	void onFallEvent();

	digital_input_s *hw;
	const char *name = nullptr;
	volatile int fallEventCounter = 0;
	volatile int riseEventCounter = 0;

	int currentRevolutionCounter = 0;

	/**
	 * Total ON time during last engine cycle
	 */
	efitimeus_t prevTotalOnTimeUs = 0;

	efitimeus_t totalOnTimeAccumulatorUs = 0;

	volatile efitimeus_t lastActivityTimeUs = 0; // timestamp in microseconds ticks
	/**
	 * time of signal fall event, in microseconds
	 */
	volatile efitimeus_t periodEventTimeUs = 0;
	volatile efitimeus_t widthEventTimeUs = 0; // time of signal rise in microseconds

	volatile efitimeus_t signalPeriodUs = 0; // period between two signal rises in microseconds

	/**
	 * offset from engine cycle start in microseconds
	 */
	volatile efitimeus_t waveOffsetUs = 0;
	volatile efitimeus_t last_wave_low_widthUs = 0; // time period in systimer ticks
	volatile efitimeus_t last_wave_high_widthUs = 0; // time period in systimer ticks
};

void initWaveAnalyzer(Logging *sharedLogger);
void printWave(Logging *logging);
void showWaveInfo(void);

#endif

#endif /* WAVE_ANALYZER_H_ */

