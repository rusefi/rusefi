/**
 * @file	signal_executor_sleep.cpp
 * @brief   Asynchronous output signal code
 *
 * Here we have the simplest, thread-based implementation of signal executor.
 * TODO: https://sourceforge.net/p/rusefi/tickets/6/
 *
 * @date Feb 10, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
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

#include "pch.h"
#include "os_access.h"
#include "scheduler.h"
#include "main_trigger_callback.h"

#if EFI_PRINTF_FUEL_DETAILS
bool printSchedulerDebug = true;
#endif // EFI_PRINTF_FUEL_DETAILS

#if EFI_SIGNAL_EXECUTOR_SLEEP

void SleepExecutor::scheduleByTimestamp(const char *msg, scheduling_s *scheduling, efitimeus_t timeUs, action_s action) {
	scheduleForLater(scheduling, timeUs - getTimeNowUs(), action);
}

void SleepExecutor::scheduleByTimestampNt(const char *msg, scheduling_s* scheduling, efitick_t timeNt, action_s action) {
	scheduleByTimestamp(msg, scheduling, NT2US(timeNt), action);
}

static void timerCallback(scheduling_s *scheduling) {
#if EFI_PRINTF_FUEL_DETAILS
	if (printSchedulerDebug) {
		if (scheduling->action.getCallback() == (schfunc_t)&turnInjectionPinLow) {
			printf("executing cb=turnInjectionPinLow p=%d sch=%d now=%d\r\n", (int)scheduling->action.getArgument(), (int)scheduling,
				(int)getTimeNowUs());
		} else {
//		printf("exec cb=%d p=%d\r\n", (int)scheduling->callback, (int)scheduling->param);
		}
	}
#endif // EFI_PRINTF_FUEL_DETAILS
	scheduling->action.execute();
}

static void doScheduleForLater(scheduling_s *scheduling, int delayUs, action_s action) {
	int delaySt = MY_US2ST(delayUs);
	if (delaySt <= 0) {
		/**
		 * in case of zero delay, we should invoke the callback
		 */
		action.execute();
		return;
	}

	chibios_rt::CriticalSectionLocker csl;

	scheduling->action = action;
	int isArmed = chVTIsArmedI(&scheduling->timer);
	if (isArmed) {
		/**
		 * timer reuse is normal for example in case of sudden RPM increase
		 */
		chVTResetI(&scheduling->timer);
	}

#if EFI_SIMULATOR
	if (action.getCallback() == (schfunc_t)&turnInjectionPinLow) {
		//printf("setTime cb=turnInjectionPinLow p=%d\r\n", (int)action.getArgument());
	} else {
//		printf("setTime cb=%d p=%d\r\n", (int)callback, (int)param);
	}
#endif /* EFI_SIMULATOR */

	chVTSetI(&scheduling->timer, delaySt, (vtfunc_t)timerCallback, scheduling);
}

void SleepExecutor::scheduleForLater(scheduling_s *scheduling, int delayUs, action_s action) {
	doScheduleForLater(scheduling, delayUs, action);
}

void SleepExecutor::cancel(scheduling_s* s) {
	chibios_rt::CriticalSectionLocker csl;

	if (chVTIsArmedI(&s->timer)) {
		chVTResetI(&s->timer);
	}

	s->action = {};
}

#endif /* EFI_SIGNAL_EXECUTOR_SLEEP */
