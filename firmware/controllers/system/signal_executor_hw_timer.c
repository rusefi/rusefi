/*
 * @file    signal_executor_hw_timer.c
 *
 * this is a future home of hardware TIM signal executor https://sourceforge.net/p/rusefi/tickets/6/
 *
 * @date Oct 28, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
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

#include "main.h"
#include "signal_executor.h"
#include "signal_executor_hw_timer.h"

#if EFI_SIGNAL_EXECUTOR_HW_TIMER || defined(__DOXYGEN__)

void initOutputSignal(char *name, OutputSignal *signal, int led, int xor) {
	// todo
}

void scheduleOutput(OutputSignal *signal, int delay, int dwell) {
	// todo
}

#endif /* EFI_SIGNAL_EXECUTOR_HW_TIMER */
