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
typedef struct {
#if EFI_WAVE_CHART
	Logging logging;
#endif /* EFI_WAVE_CHART */
	int counter;
	volatile int isInitialized;
} WaveChart;

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

void addWaveChartEvent3(WaveChart *chart, const char *name, const char *msg, const char *msg2);
void publishChart(WaveChart *chart);
void initWaveChart(WaveChart *chart);
void resetWaveChart(WaveChart *chart);
void setChartSize(int newSize);
int isWaveChartFull(WaveChart *chart);
void publishChartIfFull(WaveChart *chart);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* WAVE_CHART_H_ */
