/**
 * @file	wave_chart.h
 * @brief	Dev console wave sniffer
 *
 * @date Jun 23, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef WAVE_CHART_H_
#define WAVE_CHART_H_

#include "global.h"

#if EFI_WAVE_CHART
#include "datalogging.h"
#endif /* EFI_WAVE_CHART */

/**
 * @brief	Dev console sniffer data buffer
 */
class WaveChart {
public:
	void init();
	void publishChart();
	void resetWaveChart();
	int isWaveChartFull();
	void publishChartIfFull();
	void addWaveChartEvent3(const char *name, const char *msg, const char *msg2);
private:
#if EFI_WAVE_CHART
	Logging logging;
#endif /* EFI_WAVE_CHART */
	uint32_t counter;
	uint64_t startTime100;
	uint64_t startTimeUs;
	volatile int isInitialized;
};

void initWaveChart(WaveChart *chart);
void showWaveChartHistogram(void);
void setChartSize(int newSize);

#endif /* WAVE_CHART_H_ */
