/**
 * @file	engine_sniffer.h
 * @brief	rusEfi console wave sniffer
 *
 * @date Jun 23, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef WAVE_CHART_H_
#define WAVE_CHART_H_

#include "global.h"

#if EFI_ENGINE_SNIFFER
#include "datalogging.h"

/**
 * @brief	rusEfi console sniffer data buffer
 */
class WaveChart {
public:
	WaveChart();
	void init();
	void addEvent3(const char *name, const char *msg);
	void reset();
	void publishIfFull();
	void publish();
	bool isFull();
	bool isStartedTooLongAgo();
private:
	Logging logging;
	char timeBuffer[10];
	uint32_t counter;
	efitime_t startTimeNt;
	volatile int isInitialized;
};

void initWaveChart(WaveChart *chart);
void showWaveChartHistogram(void);
void setChartSize(int newSize);

#endif /* EFI_ENGINE_SNIFFER */

#endif /* WAVE_CHART_H_ */
