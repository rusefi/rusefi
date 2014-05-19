/**
 * @file    engine_controller.cpp
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
#include "efilib2.h"
#include "ec2.h"
#include "PwmTester.h"


#define _10_MILLISECONDS (10 * TICKS_IN_MS)

#if defined __GNUC__
persistent_config_container_s persistentState __attribute__((section(".ccm")));
#else
persistent_config_container_s persistentState;
#endif

engine_configuration_s *engineConfiguration = &persistentState.persistentConfiguration.engineConfiguration;
board_configuration_s *boardConfiguration = &persistentState.persistentConfiguration.boardConfiguration;

/**
 * CH_FREQUENCY is the number of system ticks in a second
 */
#define FUEL_PUMP_DELAY (4 * CH_FREQUENCY)

static VirtualTimer everyMsTimer;
static VirtualTimer fuelPumpTimer;

static Logging logger;

static engine_configuration2_s ec2;
engine_configuration2_s * engineConfiguration2 = &ec2;

static msg_t csThread(void) {
	chRegSetThreadName("status");
	while (TRUE) {
		int is_cranking = isCranking();
		int is_running = getRpm() > 0 && !is_cranking;
		if (is_running) {
			// blinking while running
			setOutputPinValue(LED_RUNNING, 0);
			chThdSleepMilliseconds(50);
			setOutputPinValue(LED_RUNNING, 1);
			chThdSleepMilliseconds(50);
		} else {
			// constant on while cranking and off if engine is stopped
			setOutputPinValue(LED_RUNNING, is_cranking);
			chThdSleepMilliseconds(100);
		}
	}
}

static void updateErrorCodes(void) {
	/**
	 * technically we can set error codes right inside the getMethods, but I a bit on a fence about it
	 */
	setError(isValidIntakeAirTemperature(getIntakeAirTemperature()),
			OBD_Intake_Air_Temperature_Circuit_Malfunction);
	setError(isValidCoolantTemperature(getCoolantTemperature()),
			OBD_Engine_Coolant_Temperature_Circuit_Malfunction);
}

static void fanRelayControl(void) {
	if (boardConfiguration->fanPin == GPIO_NONE)
		return;

	int isCurrentlyOn = getOutputPinValue(FAN_RELAY);
	int newValue;
	if (isCurrentlyOn) {
		// if the fan is already on, we keep it on till the 'fanOff' temperature
		newValue = getCoolantTemperature()
				> engineConfiguration->fanOffTemperature;
	} else {
		newValue = getCoolantTemperature()
				> engineConfiguration->fanOnTemperature;
	}

	if (isCurrentlyOn != newValue) {
		scheduleMsg(&logger, "FAN relay: %s", newValue ? "ON" : "OFF");
		setOutputPinValue(FAN_RELAY, newValue);
	}
}

Overflow64Counter halTime;

uint64_t getTimeNowUs(void) {
	// todo: synchronization? multi-threading?
	halTime.offer(hal_lld_get_counter_value());
	return halTime.get() / (CORE_CLOCK / 1000000);
}

uint64_t getHalTimer(void) {
	return halTime.get();
}

efitimems_t currentTimeMillis(void) {
	// todo: migrate to getTimeNowUs? or not?
	return chTimeNow() / TICKS_IN_MS;
}

int getTimeNowSeconds(void) {
	return chTimeNow() / CH_FREQUENCY;
}

static void onEveny10Milliseconds(void *arg) {
	/**
	 * We need to push current value into the 64 bit counter often enough so that we do not miss an overflow
	 */
	halTime.offer(hal_lld_get_counter_value());

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
		scheduleMsg(&logger, "fuelPump OFF at %s%d",
				hwPortname(boardConfiguration->fuelPumpPin));
	turnOutputPinOff(FUEL_PUMP_RELAY);
}

static void fuelPumpOn(ShaftEvents signal, int index) {
	if (index != 0)
		return; // let's not abuse the timer - one time per revolution would be enough
	// todo: the check about GPIO_NONE should be somewhere else!
	if (!getOutputPinValue(FUEL_PUMP_RELAY)
			&& boardConfiguration->fuelPumpPin != GPIO_NONE)
		scheduleMsg(&logger, "fuelPump ON at %s",
				hwPortname(boardConfiguration->fuelPumpPin));
	turnOutputPinOn(FUEL_PUMP_RELAY);
	/**
	 * the idea of this implementation is that we turn the pump when the ECU turns on or
	 * if the shafts are spinning and then we are constantly postponing the time when we
	 * will turn it off. Only if the shafts stop the turn off would actually happen.
	 */
	chVTSetAny(&fuelPumpTimer, FUEL_PUMP_DELAY, &fuelPumpOff, 0);
}

static void initFuelPump(void) {
	addTriggerEventListener(&fuelPumpOn, "fuel pump");
	fuelPumpOn(SHAFT_PRIMARY_UP, 0);
}

char * getPinNameByAdcChannel(int hwChannel, uint8_t *buffer) {
	strcpy((char*) buffer, portname(getAdcChannelPort(hwChannel)));
	itoa10(&buffer[2], getAdcChannelPin(hwChannel));
	return (char*) buffer;
}

static uint8_t pinNameBuffer[16];

static void printAnalogChannelInfoExt(const char *name, int hwChannel,
		float voltage) {
	scheduleMsg(&logger, "%s ADC%d %s value=%fv", name, hwChannel,
			getPinNameByAdcChannel(hwChannel, pinNameBuffer), voltage);
}

static void printAnalogChannelInfo(const char *name, int hwChannel) {
	printAnalogChannelInfoExt(name, hwChannel, getVoltageDivided(hwChannel));
}

static void printAnalogInfo(void) {
	printAnalogChannelInfo("TPS", engineConfiguration->tpsAdcChannel);
	printAnalogChannelInfo("CLT", engineConfiguration->cltAdcChannel);
	printAnalogChannelInfo("IAT", engineConfiguration->iatAdcChannel);
	printAnalogChannelInfo("MAF", engineConfiguration->mafAdcChannel);
	printAnalogChannelInfo("AFR", engineConfiguration->afrSensor.afrAdcChannel);
	printAnalogChannelInfo("MAP", engineConfiguration->map.sensor.hwChannel);
	printAnalogChannelInfo("BARO", engineConfiguration->baroSensor.hwChannel);
	printAnalogChannelInfoExt("Vbatt", engineConfiguration->vBattAdcChannel,
			getVBatt());
}

static WORKING_AREA(csThreadStack, UTILITY_THREAD_STACK_SIZE);// declare thread stack

void initEngineContoller(void) {
	if (hasFirmwareError())
		return;
	initLogging(&logger, "Engine Controller");

	initSensors();

	initPwmGenerator();

#if EFI_ANALOG_CHART
	initAnalogChart();
#endif /* EFI_ANALOG_CHART */

	initAlgo();

#if EFI_WAVE_ANALYZER
	initWaveAnalyzer();
#endif /* EFI_WAVE_ANALYZER */

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

	chThdCreateStatic(csThreadStack, sizeof(csThreadStack), LOWPRIO,
			(tfunc_t) csThread, NULL);

	initInjectorCentral();
	initPwmTester();
	initIgnitionCentral();
	initMalfunctionCentral();

#if EFI_ELECTRONIC_THROTTLE_BODY
	initElectronicThrottle();
#endif /* EFI_ELECTRONIC_THROTTLE_BODY */

#if EFI_MALFUNCTION_INDICATOR
	initMalfunctionIndicator();
#endif /* EFI_MALFUNCTION_INDICATOR */

#if EFI_MAP_AVERAGING
	initMapAveraging();
#endif /* EFI_MAP_AVERAGING */

#if EFI_ENGINE_CONTROL
	/**
	 * This method initialized the main listener which actually runs injectors & ignition
	 */
	initMainEventListener();
#endif /* EFI_ENGINE_CONTROL */

#if EFI_IDLE_CONTROL
	startIdleThread();
#else
	scheduleMsg(&logger, "no idle control");
#endif

#if EFI_FUEL_PUMP
	initFuelPump();
#endif


	addConsoleAction("analoginfo", printAnalogInfo);
}
