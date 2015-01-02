/**
 * @file	signal_executor.cpp
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
#include "efiGpio.h"
#include "engine.h"

/**
 * Signal executors feed digital events right into WaveChart used by Sniffer tab of Dev Console
 */
#include "rpm_calculator.h"

EXTERN_ENGINE;

#if EFI_WAVE_CHART
#include "wave_chart.h"
extern WaveChart waveChart;
#endif

static Logging logger;

extern OutputPin outputs[IO_PIN_COUNT];
extern pin_output_mode_e *pinDefaultState[IO_PIN_COUNT];

void initSignalExecutor(void) {
	initLogging(&logger, "s exec");
	initSignalExecutorImpl();
}

void initOutputSignal(OutputSignal *signal, io_pin_e ioPin) {
	signal->io_pin = ioPin;
}

uint32_t dbgStart;
uint32_t dbgDurr;

extern const char *namedPinsArray[NAMED_PIN_COUNT];

void turnPinHigh(io_pin_e pin) {
#if EFI_DEFAILED_LOGGING
//	signal->hi_time = hTimeNow();
#endif /* EFI_DEFAILED_LOGGING */

#if EFI_GPIO
	// turn the output level ACTIVE
	// todo: this XOR should go inside the setOutputPinValue method
	doSetOutputPinValue(pin, true);
	// sleep for the needed duration
#endif
#if EFI_WAVE_CHART
	// explicit check here is a performance optimization to speed up no-chart mode
	if (CONFIG(isDigitalChartEnabled)) {
		// this is a performance optimization - array index is cheaper then invoking a method with 'switch'
		const char *pinName = namedPinsArray[pin];
//	dbgDurr = hal_lld_get_counter_value() - dbgStart;

		addWaveChartEvent(pinName, WC_UP);
	}
#endif /* EFI_WAVE_ANALYZER */
//	dbgDurr = hal_lld_get_counter_value() - dbgStart;
}

void turnPinLow(io_pin_e pin) {
#if EFI_GPIO
	// turn off the output
	doSetOutputPinValue(pin, false);
#endif

#if EFI_DEFAILED_LOGGING
	systime_t after = hTimeNow();
	debugInt(&signal->logging, "a_time", after - signal->hi_time);
	scheduleLogging(&signal->logging);
#endif /* EFI_DEFAILED_LOGGING */

#if EFI_WAVE_CHART
	if (CONFIG(isDigitalChartEnabled)) {
		// this is a performance optimization - array index is cheaper then invoking a method with 'switch'
		const char *pinName = namedPinsArray[pin];

		addWaveChartEvent(pinName, WC_DOWN);
	}
#endif /* EFI_WAVE_ANALYZER */
}

int getRevolutionCounter(void);

/**
 *
 * @param	delay	the number of ticks before the output signal
 * 					immediate output if delay is zero
 * @param	dwell	the number of ticks of output duration
 *
 */
void scheduleOutput(OutputSignal *signal, float delayMs, float durationMs) {
#if EFI_GPIO
	if (durationMs < 0) {
		firmwareError("duration cannot be negative: %d", durationMs);
		return;
	}
	if (cisnan(durationMs)) {
		firmwareError("NaN in scheduleOutput", durationMs);
		return;
	}

	efiAssertVoid(signal!=NULL, "signal is NULL");
	int index = getRevolutionCounter() % 2;
	scheduling_s * sUp = &signal->signalTimerUp[index];
	scheduling_s * sDown = &signal->signalTimerDown[index];

	scheduleTask("out up", sUp, (int) MS2US(delayMs), (schfunc_t) &turnPinHigh, (void *) signal->io_pin);
	scheduleTask("out down", sDown, (int) MS2US(delayMs) + MS2US(durationMs), (schfunc_t) &turnPinLow, (void*) signal->io_pin);
#endif
}

io_pin_e getPinByName(const char *name) {
	if (startsWith(name, "spa")) {
		int index = atoi(name + 3);
		return (io_pin_e) ((int) SPARKOUT_1_OUTPUT - 1 + index);
	}
	return IO_INVALID;
}
