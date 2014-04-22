/**
 * @file    engine_controller.c
 * @brief   Controllers package entry point code
 *
 *
 *
 * @date Feb 7, 2013
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
#include "trigger_central.h"
#include "engine_controller.h"
#include "idle_thread.h"
#include "rpm_calculator.h"
#include "signal_executor.h"
#include "main_trigger_callback.h"
#include "map_multiplier_thread.h"
#include "io_pins.h"
#include "tunerstudio.h"
#include "injector_central.h"
#include "ignition_central.h"
#include "rfiutil.h"
#include "engine_configuration.h"
#include "engine_math.h"
#include "wave_analyzer.h"
#include "allsensors.h"
#include "analog_chart.h"
#include "electronic_throttle.h"
#include "malfunction_indicator.h"
#include "map_averaging.h"
#include "malfunction_central.h"
#include "pin_repository.h"
#include "pwm_generator.h"
#include "adc_inputs.h"
#include "algo.h"

#define _10_MILLISECONDS (10 * TICKS_IN_MS)

extern engine_configuration_s *engineConfiguration;

/**
 * CH_FREQUENCY is the number of system ticks in a second
 */
#define FUEL_PUMP_DELAY (4 * CH_FREQUENCY)

static VirtualTimer everyMsTimer;
static VirtualTimer fuelPumpTimer;

static Logging logger;

static engine_configuration2_s ec2;
engine_configuration2_s * engineConfiguration2 = &ec2;

static void updateStatusLeds(void) {
	int is_cranking = isCranking();
	setOutputPinValue(LED_RUNNING, getRpm() > 0 && !is_cranking);
	setOutputPinValue(LED_CRANKING, is_cranking);
}

static void updateErrorCodes(void) {
	/**
	 * technically we can set error codes right inside the getMethods, but I a bit on a fence about it
	 */
	setError(isValidIntakeAirTemperature(getIntakeAirTemperature()), OBD_Intake_Air_Temperature_Circuit_Malfunction);
	setError(isValidCoolantTemperature(getCoolantTemperature()), OBD_Engine_Coolant_Temperature_Circuit_Malfunction);
}

static void fanRelayControl(void) {

	int isCurrentlyOn = getOutputPinValue(FAN_RELAY);
	int newValue;
	if (isCurrentlyOn) {
		// if the fan is already on, we keep it on till the 'fanOff' temperature
		newValue = getCoolantTemperature() > engineConfiguration->fanOffTemperature;
	} else {
		newValue = getCoolantTemperature() > engineConfiguration->fanOnTemperature;
	}

	if (isCurrentlyOn != newValue) {
		scheduleMsg(&logger, "FAN relay: %s", newValue ? "ON" : "OFF");
		setOutputPinValue(FAN_RELAY, newValue);
	}
}

static void onEveny10Milliseconds(void *arg) {
	updateStatusLeds();

	updateErrorCodes();

	fanRelayControl();

	// schedule next invocation
	chVTSetAny(&everyMsTimer, _10_MILLISECONDS, &onEveny10Milliseconds, 0);
}

static void initPeriodicEvents(void) {
	// schedule first invocation
	chVTSetAny(&everyMsTimer, _10_MILLISECONDS, &onEveny10Milliseconds, 0);
}

static void fuelPumpOff(void *arg) {
	if (getOutputPinValue(FUEL_PUMP_RELAY))
		scheduleMsg(&logger, "fuelPump OFF at %s%d", portname(FUEL_PUMP_PORT ), FUEL_PUMP_PIN);
	turnOutputPinOff(FUEL_PUMP_RELAY);
}

static void fuelPumpOn(ShaftEvents signal, int index) {
	if (index != 0)
		return; // let's not abuse the timer - one time per revolution would be enough
	if (!getOutputPinValue(FUEL_PUMP_RELAY))
		scheduleMsg(&logger, "fuelPump ON at %s%d", portname(FUEL_PUMP_PORT ), FUEL_PUMP_PIN);
	turnOutputPinOn(FUEL_PUMP_RELAY);
	/**
	 * the idea of this implementation is that we turn the pump when the ECU turns on or
	 * if the shafts are spinning and then we are constantly postponing the time when we
	 * will turn it off. Only if the shafts stop the turn off would actually happen.
	 */
	chVTSetAny(&fuelPumpTimer, FUEL_PUMP_DELAY, &fuelPumpOff, 0);
}

static void initFuelPump(void) {
	registerShaftPositionListener(&fuelPumpOn, "fuel pump");
	fuelPumpOn(SHAFT_PRIMARY_UP, 0);
}


char * getPinNameByAdcChannel(int hwChannel, uint8_t *buffer) {
	strcpy(buffer, portname(getAdcChannelPort(hwChannel)));
	itoa10(&buffer[2], getAdcChannelPin(hwChannel));
	return buffer;
}

static uint8_t pinNameBuffer[16];

static void printAnalogChannelInfoExt(char *name, int hwChannel, float voltage) {
	scheduleMsg(&logger, "%s ADC%d %s value=%fv", name, hwChannel, getPinNameByAdcChannel(hwChannel, pinNameBuffer),
			voltage);
}

static void printAnalogChannelInfo(char *name, int hwChannel) {
	printAnalogChannelInfoExt(name, hwChannel, getVoltageDivided(hwChannel));
}

static void printAnalogInfo(void) {
	printAnalogChannelInfo("TPS", engineConfiguration->tpsAdcChannel);
	printAnalogChannelInfo("CLT", engineConfiguration->cltAdcChannel);
	printAnalogChannelInfo("IAT", engineConfiguration->iatAdcChannel);
	printAnalogChannelInfo("MAF", engineConfiguration->mafAdcChannel);
	printAnalogChannelInfo("AFR", engineConfiguration->afrSensor.afrAdcChannel);
	printAnalogChannelInfoExt("Vbatt", engineConfiguration->vBattAdcChannel, getVBatt());
}

void initEngineContoller(void) {
	initLogging(&logger, "Engine Controller");

	initSensors();

	initPwmGenerator();

	initAnalogChart();

	initAlgo();

#ifdef EFI_WAVE_ANALYZER
	initWaveAnalyzer();
#endif

	/**
	 * there is an implicit dependency on the fact that 'tachometer' listener is the 1st listener - this case
	 * other listeners can access current RPM value
	 */
	initRpmCalculator();


#if EFI_TUNER_STUDIO
	startTunerStudioConnectivity();
#endif

// multiple issues with this	initMapAdjusterThread();
	initPeriodicEvents();

#if EFI_SIGNAL_EXECUTOR_SINGLE_TIMER
	initOutputScheduler();
#endif
	initInjectorCentral();
	initIgnitionCentral();
	initMalfunctionCentral();

#if EFI_ELECTRONIC_THROTTLE_BODY
	initElectronicThrottle();
#endif /* EFI_ELECTRONIC_THROTTLE_BODY */
	initMalfunctionIndicator();
	initMapAveraging();

	/**
	 * This method initialized the main listener which actually runs injectors & ignition
	 */
	initMainEventListener();

#if EFI_IDLE_CONTROL
	startIdleThread();
#endif

	initFuelPump();

	addConsoleAction("analoginfo", printAnalogInfo);
}
