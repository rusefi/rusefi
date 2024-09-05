/**
 * @file	signal_executor_sleep.cpp
 * @brief   Asynchronous output signal code
 *
 * Here we have the simplest, thread-based implementation of signal executor.
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

#include "scheduler.h"
#include "main_trigger_callback.h"

#if EFI_PRINTF_FUEL_DETAILS
bool printSchedulerDebug = true;
#endif // EFI_PRINTF_FUEL_DETAILS

struct CallbackContext
{
	scheduling_s* scheduling = nullptr;
	bool shouldFree = false;
};

static void doScheduleForLater(scheduling_s *scheduling, int delayUs, action_s action);

void SleepExecutor::schedule(const char *msg, scheduling_s* scheduling, efitick_t timeNt, action_s action) {
	doScheduleForLater(scheduling, NT2US(timeNt) - getTimeNowUs(), action);
}

static void timerCallback(CallbackContext* ctx) {
#if EFI_PRINTF_FUEL_DETAILS
	if (printSchedulerDebug) {
		if (ctx->scheduling->action.getCallback() == (schfunc_t)&turnInjectionPinLow) {
			printf("executing cb=turnInjectionPinLow p=%d sch=%d now=%d\r\n", (int)ctx->scheduling->action.getArgument(), (int)scheduling,
				time2print(getTimeNowUs()));
		} else {
//		printf("exec cb=%d p=%d\r\n", (int)scheduling->callback, (int)scheduling->param);
		}
	}
#endif // EFI_PRINTF_FUEL_DETAILS

	// Grab the action but clear it in the event so we can reschedule from the action's execution
	action_s action = ctx->scheduling->action;
	ctx->scheduling->action = {};

	// Clean up any memory we allocated
	if (ctx->shouldFree) {
		delete ctx->scheduling;
	}
	delete ctx;

	// Lastly, actually execute the action
	action.execute();
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

  // ECU firmware does not use heap. this sleep-based executor is used only by simulator
  // todo: probably move this executor to 'simulator' folder to make it official?
	auto ctx = new CallbackContext;
	if (!scheduling) {
		scheduling = new scheduling_s;
		chVTObjectInit(&scheduling->timer);
		ctx->shouldFree = true;
	}
	ctx->scheduling = scheduling;

	scheduling->action = action;
	int isArmed = chVTIsArmedI(&scheduling->timer);
	if (isArmed) {
		/**
		 * timer reuse is normal for example in case of sudden RPM increase
		 */
		chVTResetI(&scheduling->timer);
	}

	chVTSetI(&scheduling->timer, delaySt, (vtfunc_t)timerCallback, ctx);
}

void SleepExecutor::cancel(scheduling_s* s) {
	chibios_rt::CriticalSectionLocker csl;

	if (chVTIsArmedI(&s->timer)) {
		chVTResetI(&s->timer);
	}

	s->action = {};
}
