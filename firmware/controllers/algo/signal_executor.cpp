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

extern engine_pins_s enginePins;

static const char *sparkNames[IGNITION_PIN_COUNT] = { "c1", "c2", "c3", "c4", "c5", "c6", "c7", "c8",
		"c9", "cA", "cB", "cD"};

static const char *injectorNames[INJECTION_PIN_COUNT] = { "i1", "i2", "i3", "i4", "i5", "i6", "i7", "i8",
		"j9", "iA", "iB", "iC"};

void initEnginePinsNames(void) {
	// todo: make engine_pins_s a class and move this to constructor?
	for (int i = 0; i < IGNITION_PIN_COUNT;i++) {
		enginePins.coils[i].name = sparkNames[i];
	}
	for (int i = 0; i < INJECTION_PIN_COUNT;i++) {
		enginePins.injectors[i].name = injectorNames[i];
	}
}

void initSignalExecutor(void) {
	initSignalExecutorImpl();
	initEnginePinsNames();
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

// todo: make these macro? kind of a penny optimization if compiler is not smart to inline
void seTurnPinHigh(NamedOutputPin *output) {
#if FUEL_MATH_EXTREME_LOGGING || defined(__DOXYGEN__)
	const char * w = output->currentLogicValue == true ? "err" : "";
	scheduleMsg(&sharedLogger, "^ %spin=%s eventIndex %d %d", w, output->name,
			getRevolutionCounter(), getTimeNowUs());
#endif /* FUEL_MATH_EXTREME_LOGGING */

#if EFI_UNIT_TEST
//	if (output->currentLogicValue == 1)
//		firmwareError("Already high");
#endif

	turnPinHigh(output);
}

void seTurnPinLow(InjectorOutputPin *output) {
	if (output->cancelNextTurningInjectorOff) {
		/**
		 * in case of fuel schedule overlap between engine cycles,
		 * and if engine cycle is above say 75% for batch mode on 4 cylinders,
		 * we will get a secondary overlap between the special injection and a normal injection on the same injector.
		 * In such a case want to combine these two injection into one continues injection.
		 * Unneeded turn of injector on is handle while scheduling that second injection, but cancellation
		 * of special injection end has to be taken care of dynamically	
		 *
		 */
		output->cancelNextTurningInjectorOff = false;
		return;
	}
#if FUEL_MATH_EXTREME_LOGGING || defined(__DOXYGEN__)
	const char * w = output->currentLogicValue == false ? "err" : "";

	scheduleMsg(&sharedLogger, "- %spin=%s eventIndex %d %d", w, output->name,
			getRevolutionCounter(), getTimeNowUs());
#endif /* FUEL_MATH_EXTREME_LOGGING */

#if EFI_UNIT_TEST
	if (output->currentLogicValue == 0)
		firmwareError("Already low");
#endif

	turnPinLow(output);
}

void seScheduleByTime(const char *prefix, scheduling_s *scheduling, efitimeus_t time, schfunc_t callback, NamedOutputPin *param) {
#if FUEL_MATH_EXTREME_LOGGING || defined(__DOXYGEN__)
	scheduleMsg(&sharedLogger, "schX %s %x %d", prefix, scheduling,	time);
	scheduleMsg(&sharedLogger, "schX %s", param->name);
#endif /* FUEL_MATH_EXTREME_LOGGING */

#if EFI_UNIT_TEST || defined(__DOXYGEN__)
	printf("schB %s %d\r\n", param->name, time);
#endif /* EFI_UNIT_TEST */
	scheduleByTime(prefix, scheduling, time, callback, param);
}

/**
 *
 * @param	delay	the number of ticks before the output signal
 * 					immediate output if delay is zero
 * @param	dwell	the number of ticks of output duration
 *
 */
void scheduleOutput2(scheduling_s * sUp, scheduling_s * sDown, efitimeus_t nowUs, float delayUs, float durationUs, InjectorOutputPin *output) {
#if EFI_GPIO || defined(__DOXYGEN__)

#if EFI_UNIT_TEST || defined(__DOXYGEN__)
	printf("scheduling output %s\r\n", output->name);
#endif /* EFI_UNIT_TEST */

	efitimeus_t turnOnTime = nowUs + (int) delayUs;

	seScheduleByTime("out up", sUp, turnOnTime, (schfunc_t) &seTurnPinHigh, output);
	efitimeus_t turnOffTime = nowUs + (int) (delayUs + durationUs);

	seScheduleByTime("out down", sDown, turnOffTime, (schfunc_t) &seTurnPinLow, output);
#endif /* EFI_GPIO */
}

