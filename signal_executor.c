/**
 * @file	signal_executor.c
 *
 * todo: we should split this file into two:
 * one for pure scheduling and another one for signal output which would
 * use the scheduling
 *
 * @date Dec 4, 2013
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

#if EFI_WAVE_CHART
#include "rpm_calculator.h"
#endif

#if EFI_WAVE_ANALYZER

/**
 * Signal executors feed digital events right into WaveChart used by Sniffer tab of Dev Console
 */
#include "wave_analyzer.h"

#endif /* EFI_WAVE_ANALYZER */

#if EFI_PROD_CODE || EFI_SIMULATOR
	static Logging logger;
#endif

void initSignalExecutor(void) {
#if EFI_PROD_CODE || EFI_SIMULATOR
	initLogging(&logger, "s exec");
#endif
	initSignalExecutorImpl();
}

void initOutputSignalBase(OutputSignal *signal) {
	signal->status = IDLE;
//	signal->last_scheduling_time = 0;
	signal->initialized = TRUE;
}

static void turnHigh(OutputSignal *signal) {
#if EFI_DEFAILED_LOGGING
//	signal->hi_time = hTimeNow();
#endif /* EFI_DEFAILED_LOGGING */
	io_pin_e pin = signal->io_pin;
	// turn the output level ACTIVE
	// todo: this XOR should go inside the setOutputPinValue method
	setOutputPinValue(pin, TRUE);
	// sleep for the needed duration

#if EFI_PROD_CODE || EFI_SIMULATOR
	if(
			pin == SPARKOUT_1_OUTPUT ||
			pin == SPARKOUT_3_OUTPUT) {
//		time_t now = hTimeNow();
//		float an = getCrankshaftAngle(now);
//		scheduleMsg(&logger, "spark up%d %d", pin, now);
//		scheduleMsg(&logger, "spark angle %d %f", (int)an, an);
	}
#endif

#if EFI_WAVE_CHART
	addWaveChartEvent(signal->name, "up", "");
#endif /* EFI_WAVE_ANALYZER */
}

static void turnLow(OutputSignal *signal) {
	// turn off the output
	// todo: this XOR should go inside the setOutputPinValue method
	setOutputPinValue(signal->io_pin, FALSE);

#if EFI_DEFAILED_LOGGING
	systime_t after = hTimeNow();
	debugInt(&signal->logging, "a_time", after - signal->hi_time);
	scheduleLogging(&signal->logging);
#endif /* EFI_DEFAILED_LOGGING */

#if EFI_WAVE_CHART
	addWaveChartEvent(signal->name, "down", "");
#endif /* EFI_WAVE_ANALYZER */
}

/**
 *
 * @param	delay	the number of ticks before the output signal
 * 					immediate output if delay is zero
 * @param	dwell	the number of ticks of output duration
 *
 */

int getRevolutionCounter(void);

void scheduleOutput(OutputSignal *signal, float delayMs, float durationMs) {
	if (durationMs < 0) {
		firmwareError("duration cannot be negative: %d", durationMs);
		return;
	}

	scheduleOutputBase(signal, delayMs, durationMs);

	int index = getRevolutionCounter() % 2;
	scheduling_s * sUp = &signal->signalTimerUp[index];
	scheduling_s * sDown = &signal->signalTimerDown[index];

	scheduleTask(sUp, MS2US(delayMs), (schfunc_t) &turnHigh, (void *) signal);
	scheduleTask(sDown, MS2US(delayMs + durationMs), (schfunc_t) &turnLow, (void*)signal);

//	signal->last_scheduling_time = now;
}

void scheduleOutputBase(OutputSignal *signal, float delayMs, float durationMs) {
	/**
	 * it's better to check for the exact 'TRUE' value since otherwise
	 * we would accept any memory garbage
	 */
	chDbgCheck(signal->initialized == TRUE, "Signal not initialized");
//	signal->offset = offset;
//	signal->duration = duration;
}


char *getPinName(io_pin_e io_pin) {
	switch (io_pin) {
	case SPARKOUT_1_OUTPUT:
		return "Spark 1";
	case SPARKOUT_2_OUTPUT:
		return "Spark 2";
	case SPARKOUT_3_OUTPUT:
		return "Spark 3";
	case SPARKOUT_4_OUTPUT:
		return "Spark 4";

	case INJECTOR_1_OUTPUT:
		return "Injector 1";
	case INJECTOR_2_OUTPUT:
		return "Injector 2";
	case INJECTOR_3_OUTPUT:
		return "Injector 3";
	case INJECTOR_4_OUTPUT:
		return "Injector 4";
	case INJECTOR_5_OUTPUT:
		return "Injector 5";
	default:
		return "No name";
	}
}
