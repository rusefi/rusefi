/*
 * @file	algo.c
 *
 * @date Mar 2, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 *
 *
 * This file is part of rusEfi - see http://rusefi.com
 *
 * rusEfi is free software; you can redistribute it and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * rusEfi is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include "global.h"
#include "algo.h"
//#include "rpm_calculator.h"
#include "advance_map.h"
#include "fuel_math.h"
#include "wave_chart.h"
#include "settings.h"
#include "signal_executor.h"

WaveChart waveChart;

void initAlgo(void) {
#if EFI_PROD_CODE || EFI_SIMULATOR
	initSettings();
	initSignalExecutor();
#endif

#if EFI_WAVE_CHART
	initWaveChart(&waveChart);
#endif


	prepareFuelMap();
	prepareTimingMap();


}
