/**
 * @file	sensor_chart.h
 *
 * @date Dec 20, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "global.h"

void scAddData(float angle, float value);
void initSensorChart(void);
void publishSensorChartIfFull();
