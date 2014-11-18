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
#include "efiGpio.h"

#if EFI_WAVE_CHART
/**
 * Signal executors feed digital events right into WaveChart used by Sniffer tab of Dev Console
 */
#include "rpm_calculator.h"
#endif

#if EFI_WAVE_ANALYZER

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

void initOutputSignal(OutputSignal *signal, io_pin_e ioPin) {
	signal->io_pin = ioPin;
}

void turnPinHigh(io_pin_e pin) {
#if EFI_DEFAILED_LOGGING
//	signal->hi_time = hTimeNow();
#endif /* EFI_DEFAILED_LOGGING */
	// turn the output level ACTIVE
	// todo: this XOR should go inside the setOutputPinValue method
	setOutputPinValue(pin, TRUE);
	// sleep for the needed duration

#if EFI_PROD_CODE || EFI_SIMULATOR
//	if (pin == SPARKOUT_1_OUTPUT || pin == SPARKOUT_3_OUTPUT) {
//		time_t now = hTimeNow();
//		float an = getCrankshaftAngle(now);
//		scheduleMsg(&logger, "spark up%d %d", pin, now);
//		scheduleMsg(&logger, "spark angle %d %f", (int)an, an);
//	}
#endif

#if EFI_WAVE_CHART
	addWaveChartEvent(getPinName(pin), WC_UP);
#endif /* EFI_WAVE_ANALYZER */
}

void turnPinLow(io_pin_e pin) {
	// turn off the output
	// todo: this XOR should go inside the setOutputPinValue method
	setOutputPinValue(pin, false);

#if EFI_DEFAILED_LOGGING
	systime_t after = hTimeNow();
	debugInt(&signal->logging, "a_time", after - signal->hi_time);
	scheduleLogging(&signal->logging);
#endif /* EFI_DEFAILED_LOGGING */

#if EFI_WAVE_CHART
	addWaveChartEvent(getPinName(pin), WC_DOWN);
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

	scheduleTask("out up", sUp, (int)MS2US(delayMs), (schfunc_t) &turnPinHigh, (void *) signal->io_pin);
	scheduleTask("out down", sDown, (int)MS2US(delayMs + durationMs), (schfunc_t) &turnPinLow, (void*) signal->io_pin);
}

io_pin_e getPinByName(const char *name) {
	if(startsWith(name, "spa")) {
		int index = atoi(name + 3);
		return (io_pin_e)((int)SPARKOUT_1_OUTPUT - 1 + index);
	}
	return IO_INVALID;
}

const char *getPinName(io_pin_e io_pin) {
	switch (io_pin) {
	// todo: refactor this hell - introduce arrays & checks?
	case SPARKOUT_1_OUTPUT:
		return "spa1";
	case SPARKOUT_2_OUTPUT:
		return "spa2";
	case SPARKOUT_3_OUTPUT:
		return "spa3";
	case SPARKOUT_4_OUTPUT:
		return "spa4";
	case SPARKOUT_5_OUTPUT:
		return "spa5";
	case SPARKOUT_6_OUTPUT:
		return "spa6";
	case SPARKOUT_7_OUTPUT:
		return "spa7";
	case SPARKOUT_8_OUTPUT:
		return "spa8";
	case SPARKOUT_9_OUTPUT:
		return "spa9";
	case SPARKOUT_10_OUTPUT:
		return "spa10";
	case SPARKOUT_11_OUTPUT:
		return "spa11";
	case SPARKOUT_12_OUTPUT:
		return "spa12";

	case INJECTOR_1_OUTPUT:
		return "inj1";
	case INJECTOR_2_OUTPUT:
		return "inj2";
	case INJECTOR_3_OUTPUT:
		return "inj3";
	case INJECTOR_4_OUTPUT:
		return "inj4";
	case INJECTOR_5_OUTPUT:
		return "inj5";
	case INJECTOR_6_OUTPUT:
		return "inj6";
	case INJECTOR_7_OUTPUT:
		return "inj7";
	case INJECTOR_8_OUTPUT:
		return "inj8";
	case INJECTOR_9_OUTPUT:
		return "inj9";
	case INJECTOR_10_OUTPUT:
		return "inj10";
	case INJECTOR_11_OUTPUT:
		return "inj11";
	case INJECTOR_12_OUTPUT:
		return "inj12";

	case GPIO_0:
		return "gpio0";
	case GPIO_1:
		return "gpio1";
	case GPIO_2:
		return "gpio2";
	case GPIO_3:
		return "gpio3";
	case GPIO_4:
		return "gpio4";
	case GPIO_5:
		return "gpio5";
	case GPIO_6:
		return "gpio6";
	case GPIO_7:
		return "gpio7";
	case GPIO_8:
		return "gpio8";
	case GPIO_9:
		return "gpio9";
	case GPIO_10:
		return "gpio10";
	case GPIO_11:
		return "gpio11";
	case GPIO_12:
		return "gpio12";
	case GPIO_13:
		return "gpio13";
	case GPIO_14:
		return "gpio14";
	case GPIO_15:
		return "gpio15";
	default:
		return "Pin needs name";
	}
}
