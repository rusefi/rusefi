/**
 * @file	signal_executor.cpp
 *
 * todo: we should split this file into two:
 * one for pure scheduling and another one for signal output which would
 * use the scheduling
 *
 * @date Dec 4, 2013
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

extern NamedOutputPin outputs[IO_PIN_COUNT];

static const char *namedPinsArray[NAMED_PIN_COUNT] = { "spa1", "spa2", "spa3", "spa4", "spa5", "spa6", "spa7", "spa8",
		"spa9", "spa10", "spa11", "spa12", "inj1", "inj2", "inj3", "inj4", "inj5", "inj6", "inj7", "inj8", "inj9",
		"inj10", "inj11", "inj12", };

static const char *getPinName(io_pin_e io_pin) {
	switch (io_pin) {
	// todo: refactor this hell - introduce arrays & checks?
	case SPARKOUT_1_OUTPUT:
	case SPARKOUT_2_OUTPUT:
	case SPARKOUT_3_OUTPUT:
	case SPARKOUT_4_OUTPUT:
	case SPARKOUT_5_OUTPUT:
	case SPARKOUT_6_OUTPUT:
	case SPARKOUT_7_OUTPUT:
	case SPARKOUT_8_OUTPUT:
	case SPARKOUT_9_OUTPUT:
	case SPARKOUT_10_OUTPUT:
	case SPARKOUT_11_OUTPUT:
	case SPARKOUT_12_OUTPUT:
	case INJECTOR_1_OUTPUT:
	case INJECTOR_2_OUTPUT:
	case INJECTOR_3_OUTPUT:
	case INJECTOR_4_OUTPUT:
	case INJECTOR_5_OUTPUT:
	case INJECTOR_6_OUTPUT:
	case INJECTOR_7_OUTPUT:
	case INJECTOR_8_OUTPUT:
	case INJECTOR_9_OUTPUT:
	case INJECTOR_10_OUTPUT:
	case INJECTOR_11_OUTPUT:
	case INJECTOR_12_OUTPUT:
		return namedPinsArray[io_pin];
	default:
		return "Pin needs name";
	}
}

void initSignalExecutor(void) {
	initLogging(&logger, "s exec");
	initSignalExecutorImpl();

	for (int i = 0; i < IO_PIN_COUNT;i++)
		outputs[i].name = getPinName((io_pin_e)i);

}

//uint32_t dbgStart;
//uint32_t dbgDurr;

void turnPinHigh(NamedOutputPin *output) {
	efiAssertVoid(output!=NULL, "NULL @ turnPinHigh");
#if EFI_DEFAILED_LOGGING
//	signal->hi_time = hTimeNow();
#endif /* EFI_DEFAILED_LOGGING */

#if EFI_GPIO
	// turn the output level ACTIVE
	// todo: this XOR should go inside the setOutputPinValue method
	doSetOutputPinValue2(output, true);
	// sleep for the needed duration
#endif
#if EFI_WAVE_CHART
	// explicit check here is a performance optimization to speed up no-chart mode
	if (CONFIG(isDigitalChartEnabled)) {
		// this is a performance optimization - array index is cheaper then invoking a method with 'switch'
		const char *pinName = output->name;
//	dbgDurr = hal_lld_get_counter_value() - dbgStart;

		addWaveChartEvent(pinName, WC_UP);
	}
#endif /* EFI_WAVE_ANALYZER */
//	dbgDurr = hal_lld_get_counter_value() - dbgStart;
}

void turnPinLow(NamedOutputPin *output) {
	efiAssertVoid(output!=NULL, "NULL turnPinLow");
#if EFI_GPIO
	// turn off the output
	doSetOutputPinValue2(output, false);
#endif

#if EFI_DEFAILED_LOGGING
	systime_t after = hTimeNow();
	debugInt(&signal->logging, "a_time", after - signal->hi_time);
	scheduleLogging(&signal->logging);
#endif /* EFI_DEFAILED_LOGGING */

#if EFI_WAVE_CHART
	if (CONFIG(isDigitalChartEnabled)) {
		// this is a performance optimization - array index is cheaper then invoking a method with 'switch'
		const char *pinName = output->name;

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

	scheduleTask("out up", sUp, (int) MS2US(delayMs), (schfunc_t) &turnPinHigh, signal->output);
	scheduleTask("out down", sDown, (int) MS2US(delayMs) + MS2US(durationMs), (schfunc_t) &turnPinLow, signal->output);
#endif
}

io_pin_e getPinByName(const char *name) {
	if (startsWith(name, "spa")) {
		int index = atoi(name + 3);
		return (io_pin_e) ((int) SPARKOUT_1_OUTPUT - 1 + index);
	}
	return IO_INVALID;
}
