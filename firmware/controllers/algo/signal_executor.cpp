/**
 * @file	signal_executor.cpp
 *
 * todo: we should split this file into two:
 * one for pure scheduling and another one for signal output which would
 * use the scheduling
 *
 * @date Dec 4, 2013
 * @author Andrey Belomutskiy, (c) 2012-2016
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
#include "efiGpio.h"
#include "engine.h"

/**
 * Signal executors feed digital events right into WaveChart used by Sniffer tab of Dev Console
 */
#include "rpm_calculator.h"

EXTERN_ENGINE;

#if EFI_ENGINE_SNIFFER || defined(__DOXYGEN__)
#include "engine_sniffer.h"
extern WaveChart waveChart;
#endif /* EFI_ENGINE_SNIFFER */

#include "efiGpio.h"

OutputSignalPair::OutputSignalPair() {
	isScheduled = false;
	memset(outputs, 0, sizeof(outputs));
	event = NULL;
}

void initSignalExecutor(void) {
	initSignalExecutorImpl();
}

void turnPinHigh(NamedOutputPin *output) {
	efiAssertVoid(output!=NULL, "NULL @ turnPinHigh");
#if EFI_DEFAILED_LOGGING || defined(__DOXYGEN__)
//	signal->hi_time = hTimeNow();
#endif /* EFI_DEFAILED_LOGGING */

	// turn the output level ACTIVE
	// todo: this XOR should go inside the setOutputPinValue method
	doSetOutputPinValue2(output, true);

	// sleep for the needed duration
#if EFI_ENGINE_SNIFFER || defined(__DOXYGEN__)
	// explicit check here is a performance optimization to speed up no-chart mode
	if (ENGINE(isEngineChartEnabled)) {
		// this is a performance optimization - array index is cheaper then invoking a method with 'switch'
		const char *pinName = output->name;
//	dbgDurr = hal_lld_get_counter_value() - dbgStart;

		addEngineSniffferEvent(pinName, WC_UP);
	}
#endif /* EFI_ENGINE_SNIFFER */
//	dbgDurr = hal_lld_get_counter_value() - dbgStart;
}

void turnPinLow(NamedOutputPin *output) {
	efiAssertVoid(output!=NULL, "NULL turnPinLow");
	// turn off the output
	doSetOutputPinValue2(output, false);

#if EFI_DEFAILED_LOGGING || defined(__DOXYGEN__)
	systime_t after = hTimeNow();
	debugInt(&signal->logging, "a_time", after - signal->hi_time);
	scheduleLogging(&signal->logging);
#endif /* EFI_DEFAILED_LOGGING */

#if EFI_ENGINE_SNIFFER || defined(__DOXYGEN__)
	if (ENGINE(isEngineChartEnabled)) {
		// this is a performance optimization - array index is cheaper then invoking a method with 'switch'
		const char *pinName = output->name;

		addEngineSniffferEvent(pinName, WC_DOWN);
	}
#endif /* EFI_ENGINE_SNIFFER */
}

#if FUEL_MATH_EXTREME_LOGGING
extern LoggingWithStorage sharedLogger;
#endif /* FUEL_MATH_EXTREME_LOGGING */



