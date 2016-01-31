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
#endif

#include "efiGpio.h"

extern engine_pins_s enginePins;

static const char *sparkNames[IGNITION_PIN_COUNT] = { "c1", "c2", "c3", "c4", "c5", "c6", "c7", "c8",
		"c9", "cA", "cB", "cD"};

static const char *injectorNames[INJECTION_PIN_COUNT] = { "i1", "i2", "i3", "i4", "i5", "i6", "i7", "i8",
		"j9", "iA", "iB", "iC"};

void initSignalExecutor(void) {
	initSignalExecutorImpl();
	for (int i = 0; i < IGNITION_PIN_COUNT;i++) {
		enginePins.coils[i].name = sparkNames[i];
	}
	for (int i = 0; i < INJECTION_PIN_COUNT;i++) {
		enginePins.injectors[i].name = injectorNames[i];
	}
}

void turnPinHigh(NamedOutputPin *output) {
	efiAssertVoid(output!=NULL, "NULL @ turnPinHigh");
#if EFI_DEFAILED_LOGGING || defined(__DOXYGEN__)
//	signal->hi_time = hTimeNow();
#endif /* EFI_DEFAILED_LOGGING */

#if EFI_GPIO || defined(__DOXYGEN__)
	// turn the output level ACTIVE
	// todo: this XOR should go inside the setOutputPinValue method
	doSetOutputPinValue2(output, true);
	// sleep for the needed duration
#endif
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
#if EFI_GPIO || defined(__DOXYGEN__)
	// turn off the output
	doSetOutputPinValue2(output, false);
#endif /* EFI_GPIO */

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

int getRevolutionCounter(void);

/**
 *
 * @param	delay	the number of ticks before the output signal
 * 					immediate output if delay is zero
 * @param	dwell	the number of ticks of output duration
 *
 */
void scheduleOutput(OutputSignal *signal, efitimeus_t nowUs, float delayUs, float durationUs, NamedOutputPin *output) {
#if EFI_GPIO || defined(__DOXYGEN__)
	if (durationUs < 0) {
		warning(OBD_PCM_Processor_Fault, "duration cannot be negative: %d", durationUs);
		return;
	}
	if (cisnan(durationUs)) {
		warning(OBD_PCM_Processor_Fault, "NaN in scheduleOutput", durationUs);
		return;
	}

	efiAssertVoid(signal!=NULL, "signal is NULL");
	int index = getRevolutionCounter() % 2;
	scheduling_s * sUp = &signal->signalTimerUp[index];
	scheduling_s * sDown = &signal->signalTimerDown[index];
#if EFI_UNIT_TEST || defined(__DOXYGEN__)
	printf("scheduling output %s\r\n", output->name);
#endif

	scheduleByTime("out up", sUp, nowUs + (int) delayUs, (schfunc_t) &turnPinHigh, output);
	scheduleByTime("out down", sDown, nowUs + (int) (delayUs + durationUs), (schfunc_t) &turnPinLow, output);
#endif
}
