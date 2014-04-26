/**
 * @file	signal_executor_single_timer_algo.c
 *
 * @date Nov 28, 2013
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

#include "signal_executor.h"
#include "signal_executor_single_timer_algo.h"
#include "main.h"
#include "utlist.h"
#include "io_pins.h"

#if EFI_WAVE_ANALYZER
#include "wave_analyzer.h"
#include "wave_chart.h"
extern WaveChart waveChart;
#endif

#if EFI_SIGNAL_EXECUTOR_SINGLE_TIMER
/**
 * @brief Output list
 *
 * List of all active output signals
 * This is actually the head of the list.
 * When the list is empty (initial state) the head of the list should be NULL.
 * This is by design.
 */
OutputSignal *st_output_list = NULL;

inline void registerSignal(OutputSignal *signal) {
	LL_APPEND(st_output_list, signal);
}

void setOutputPinValue(io_pin_e pin, int value);

/**
 * @return time of next event within for this signal
 * @todo Find better name.
 */
inline time_t toggleSignalIfNeeded(OutputSignal *out, time_t now) {
//	chDbgCheck(out!=NULL, "out is NULL");
//	chDbgCheck(out->io_pin < IO_PIN_COUNT, "pin assertion");
	time_t last = out->last_scheduling_time;
	//estimated = last + out->timing[out->status];
	time_t estimated = last + GET_DURATION(out);
	if (now >= estimated) {
		out->status ^= 1; /* toggle status */
		//setOutputPinValue(out->io_pin, out->status); /* Toggle output */
		palWritePad(GPIOE, 5, out->status);
#if EFI_WAVE_ANALYZER
//		addWaveChartEvent(out->name, out->status ? "up" : "down", "");
#endif /* EFI_WAVE_ANALYZER */

//		out->last_scheduling_time = now; /* store last update */
		estimated = now + GET_DURATION(out); /* update estimation */
	}
	return estimated - now;
}
#endif /* EFI_SIGNAL_EXECUTOR_SINGLE_TIMER */
