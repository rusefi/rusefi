/**
 * @file	signal_executor_sleep.c
 * @brief   Asynchronous output signal code
 *
 * Here we have the simplest, thread-based implementation of signal executor.
 * TODO: https://sourceforge.net/p/rusefi/tickets/6/
 *
 * @date Feb 10, 2013
 * @author Andrey Belomutskiy, (c) 2012-2015
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
#include "scheduler.h"

#if EFI_SIGNAL_EXECUTOR_SLEEP || defined(__DOXYGEN__)

void scheduleTask2(const char *prefix, scheduling_s *scheduling, uint64_t time, schfunc_t callback, void *param) {
	scheduleTask(prefix, scheduling, time - getTimeNowUs(), callback, param);
}

void scheduleTask(const char *prefix, scheduling_s *scheduling, int delayUs, schfunc_t callback, void *param) {
	int delaySt = delayUs * CH_FREQUENCY / 1000000;
	if (delaySt <= 0) {
		/**
		 * in case of zero delay, we should invoke the callback
		 */
		callback(param);
		return;
	}

	lockAnyContext();
	int isArmed = chVTIsArmedI(&scheduling->timer);
	if (isArmed)
		chVTResetI(&scheduling->timer);

	chVTSetI(&scheduling->timer, delaySt, (vtfunc_t)callback, param);
	unlockAnyContext();
}

void initSignalExecutorImpl(void) {

}

#endif /* EFI_SIGNAL_EXECUTOR_SLEEP */
