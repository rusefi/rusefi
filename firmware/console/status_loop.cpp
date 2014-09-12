/**
 * @file	status_loop.cpp
 * @brief Human-readable protocol status messages
 *
 * http://rusefi.com/forum/viewtopic.php?t=263 Dev console overview
 * http://rusefi.com/forum/viewtopic.php?t=210 Commands overview
 *
 *
 * @date Mar 15, 2013
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
 *
 */

#include "main.h"
#include "status_loop.h"
#include "ec2.h"

#include "adc_inputs.h"
#if EFI_WAVE_ANALYZER
#include "wave_analyzer.h"
#endif

#include "trigger_central.h"
#include "engine_state.h"
#include "io_pins.h"
#include "mmc_card.h"
#include "console_io.h"
#include "malfunction_central.h"
#include "speed_density.h"

#include "advance_map.h"
#if EFI_TUNER_STUDIO
#include "tunerstudio.h"
#endif /* EFI_TUNER_STUDIO */

#include "fuel_math.h"
#include "main_trigger_callback.h"
#include "engine_math.h"
#include "idle_thread.h"
#include "engine_configuration.h"
#include "rfiutil.h"
#include "svnversion.h"
#include "engine.h"
#include "lcd_controller.h"
#include "fuel_math.h"

#if EFI_PROD_CODE
// todo: move this logic to algo folder!
#include "rtc_helper.h"
#include "lcd_HD44780.h"
#include "rusefi.h"
#include "pin_repository.h"
#include "flash_main.h"
#endif

extern Engine engine;

// this 'true' value is needed for simulator
static volatile bool fullLog = true;
int warningEnabled = TRUE;
//int warningEnabled = FALSE;

extern engine_configuration_s * engineConfiguration;
extern engine_configuration2_s * engineConfiguration2;
extern board_configuration_s *boardConfiguration;
#define FULL_LOGGING_KEY "fl"

#if EFI_PROD_CODE || EFI_SIMULATOR
static Logging logger;

static void setWarningEnabled(int value) {
	warningEnabled = value;
}

#endif /* EFI_PROD_CODE || EFI_SIMULATOR */

#if EFI_FILE_LOGGING
static Logging fileLogger;
#endif /* EFI_FILE_LOGGING */

static void reportSensorF(const char *caption, float value, int precision) {
#if EFI_PROD_CODE || EFI_SIMULATOR
	debugFloat(&logger, caption, value, precision);
#endif /* EFI_PROD_CODE || EFI_SIMULATOR */

#if EFI_FILE_LOGGING
	debugFloat(&fileLogger, caption, value, precision);
#endif /* EFI_FILE_LOGGING */
}

static void reportSensorI(const char *caption, int value) {
#if EFI_PROD_CODE || EFI_SIMULATOR
	debugInt(&logger, caption, value);
#endif /* EFI_PROD_CODE || EFI_SIMULATOR */
#if EFI_FILE_LOGGING
	debugInt(&fileLogger, caption, value);
#endif /* EFI_FILE_LOGGING */
}

static const char* boolean2string(int value) {
	return value ? "YES" : "NO";
}

void printSensors(void) {
#if EFI_FILE_LOGGING
	resetLogging(&fileLogger);
#endif /* EFI_FILE_LOGGING */

	// current time, in milliseconds
	int nowMs = currentTimeMillis();
	float sec = ((float) nowMs) / 1000;
	reportSensorF("time", sec, 3);

	reportSensorI("rpm", getRpm());
	reportSensorF("maf", getMaf(), 2);

	if (engineConfiguration->hasMapSensor) {
		reportSensorF(getCaption(LP_MAP), getMap(), 2);
		reportSensorF("map_r", getRawMap(), 2);
	}
	reportSensorF("baro", getBaroPressure(), 2);

	reportSensorF("afr", getAfr(), 2);
	reportSensorF("vref", getVRef(), 2);
	reportSensorF("vbatt", getVBatt(), 2);

	reportSensorF("TRG_0_DUTY", getTriggerDutyCycle(0), 2);
	reportSensorF("TRG_1_DUTY", getTriggerDutyCycle(1), 2);

	reportSensorF(getCaption(LP_THROTTLE), getTPS(), 2);

	if (engineConfiguration->hasCltSensor) {
		reportSensorF(getCaption(LP_ECT), getCoolantTemperature(), 2);
	}

	reportSensorF(getCaption(LP_IAT), getIntakeAirTemperature(), 2);

//	debugFloat(&logger, "tch", getTCharge1(tps), 2);

#if EFI_FILE_LOGGING
	appendPrintf(&fileLogger, "\r\n");
	appendToLog(fileLogger.buffer);
#endif /* EFI_FILE_LOGGING */
}

void printState(int currentCkpEventCounter) {
#if EFI_SHAFT_POSITION_INPUT
	printSensors();

	int rpm = getRpm();
	debugInt(&logger, "ckp_c", currentCkpEventCounter);

//	debugInt(&logger, "idl", getIdleSwitch());

//	debugFloat(&logger, "table_spark", getAdvance(rpm, getMaf()), 2);

	float engineLoad = getEngineLoad();
	float baseFuel = getBaseFuel(&engine, rpm);
	debugFloat(&logger, "fuel_base", baseFuel, 2);
//	debugFloat(&logger, "fuel_iat", getIatCorrection(getIntakeAirTemperature()), 2);
//	debugFloat(&logger, "fuel_clt", getCltCorrection(getCoolantTemperature()), 2);
	debugFloat(&logger, "fuel_lag", getInjectorLag(getVBatt()), 2);
	debugFloat(&logger, "fuel", getRunningFuel(baseFuel, &engine, rpm), 2);

	debugFloat(&logger, "timing", getAdvance(rpm, engineLoad), 2);

//		float map = getMap();
//		float fuel = getDefaultFuel(rpm, map);
//		debugFloat(&logger, "d_fuel", fuel, 2);

#endif /* EFI_SHAFT_POSITION_INPUT */
}

#define INITIAL_FULL_LOG TRUE
//#define INITIAL_FULL_LOG FALSE

static char LOGGING_BUFFER[700];

volatile int needToReportStatus = FALSE;
static int prevCkpEventCounter = -1;

static Logging logger2;

static void printStatus(void) {
	needToReportStatus = TRUE;
}

/**
 * Time when the firmware version was reported last time, in seconds
 * TODO: implement a request/response instead of just constantly sending this out
 */
static systime_t timeOfPreviousPrintVersion = (systime_t) -1;

#if EFI_PROD_CODE
static void printOutPin(const char *pinName, brain_pin_e hwPin) {
	appendPrintf(&logger, "outpin%s%s@%s%s", DELIMETER, pinName, hwPortname(hwPin), DELIMETER);
}
#endif /* EFI_PROD_CODE */

static void printInfo(systime_t nowSeconds) {
	/**
	 * we report the version every 4 seconds - this way the console does not need to
	 * request it and we will display it pretty soon
	 */
	if (overflowDiff(nowSeconds, timeOfPreviousPrintVersion) < 4) {
		return;
	}
	timeOfPreviousPrintVersion = nowSeconds;
	appendPrintf(&logger, "rusEfiVersion%s%d@%s %s%s", DELIMETER, getRusEfiVersion(), VCS_VERSION,
			getConfigurationName(engineConfiguration->engineType),
			DELIMETER);
#if EFI_PROD_CODE
	printOutPin(WC_CRANK1, boardConfiguration->triggerInputPins[0]);
	printOutPin(WC_CRANK2, boardConfiguration->triggerInputPins[1]);
	printOutPin(WA_CHANNEL_1, boardConfiguration->logicAnalyzerPins[0]);
	printOutPin(WA_CHANNEL_2, boardConfiguration->logicAnalyzerPins[1]);

	for (int i = 0; i < engineConfiguration->cylindersCount; i++) {
		// todo: extract method?
		io_pin_e pin = (io_pin_e) ((int) SPARKOUT_1_OUTPUT + i);

		printOutPin(getPinName(pin), boardConfiguration->ignitionPins[i]);

		pin = (io_pin_e) ((int) INJECTOR_1_OUTPUT + i);
		printOutPin(getPinName(pin), boardConfiguration->injectionPins[i]);
	}
#endif

}

static systime_t timeOfPreviousReport = (systime_t) -1;

extern char errorMessageBuffer[200];

/**
 * @brief Sends all pending data to dev console
 */
void updateDevConsoleState(void) {
	if (!isConsoleReady()) {
		return;
	}
// looks like this is not needed anymore
//	checkIfShouldHalt();
	printPending();

#if EFI_PROD_CODE
	// todo: unify with simulator!
	if (hasFirmwareError()) {
		printMsg(&logger, "firmware error: %s", errorMessageBuffer);
		warningEnabled = FALSE;
		chThdSleepMilliseconds(200);
		return;
	}
#endif

#if EFI_PROD_CODE
	pokeAdcInputs();
#endif

	if (!fullLog) {
		return;
	}

	systime_t nowSeconds = getTimeNowSeconds();
	printInfo(nowSeconds);

	int currentCkpEventCounter = getCrankEventCounter();
	if (prevCkpEventCounter == currentCkpEventCounter && timeOfPreviousReport == nowSeconds) {
		return;
	}

	timeOfPreviousReport = nowSeconds;

	prevCkpEventCounter = currentCkpEventCounter;

	printState(currentCkpEventCounter);

#if EFI_WAVE_ANALYZER
	printWave(&logger);
#endif

	printLine(&logger);
}

#if EFI_PROD_CODE

/*
 * command example:
 * sfm 3500 400
 * that would be 'show fuel for rpm 3500 maf 4.0'
 */

static void showFuelInfo2(float rpm, float engineLoad) {
	float baseFuel = getBaseTableFuel((int) rpm, engineLoad);

	scheduleMsg(&logger2, "algo=%s/pump=%s", algorithmToString(engineConfiguration->algorithm), boolToString(getOutputPinValue(FUEL_PUMP_RELAY)));

	scheduleMsg(&logger2, "cranking fuel: %f", getCrankingFuel());

	if (engine.rpmCalculator->isRunning()) {
		float iatCorrection = getIatCorrection(getIntakeAirTemperature());
		float cltCorrection = getCltCorrection(getCoolantTemperature());
		float injectorLag = getInjectorLag(getVBatt());
		scheduleMsg(&logger2, "rpm=%f engineLoad=%f", rpm, engineLoad);
		scheduleMsg(&logger2, "baseFuel=%f", baseFuel);

		scheduleMsg(&logger2, "iatCorrection=%f cltCorrection=%f injectorLag=%f", iatCorrection, cltCorrection,
				injectorLag);

		float value = getRunningFuel(baseFuel, &engine, (int) rpm);
		scheduleMsg(&logger2, "injection pulse width: %f", value);
	}
}

static void showFuelInfo(void) {
	showFuelInfo2((float) getRpm(), getEngineLoad());
}

#endif /* EFI_PROD_CODE */

static THD_WORKING_AREA(lcdThreadStack, UTILITY_THREAD_STACK_SIZE);

static void lcdThread(void) {
	chRegSetThreadName("lcd");
	while (true) {
#if EFI_HD44780_LCD
		updateHD44780lcd();
#endif
		chThdSleepMilliseconds(boardConfiguration->lcdThreadPeriod);
	}
}

static THD_WORKING_AREA(tsThreadStack, UTILITY_THREAD_STACK_SIZE);

#if EFI_TUNER_STUDIO

void updateTunerStudioState(TunerStudioOutputChannels *tsOutputChannels) {
#if EFI_SHAFT_POSITION_INPUT
	int rpm = getRpm();
#else
	int rpm = 0;
#endif

	float tps = getTPS();
	float coolant = getCoolantTemperature();
	float intake = getIntakeAirTemperature();

	float engineLoad = getEngineLoad();
	float baseFuel = getBaseTableFuel((int) rpm, engineLoad);

	tsOutputChannels->rpm = rpm;
	tsOutputChannels->coolant_temperature = coolant;
	tsOutputChannels->intake_air_temperature = intake;
	tsOutputChannels->throttle_positon = tps;
	tsOutputChannels->mass_air_flow = getMaf();
	tsOutputChannels->air_fuel_ratio = getAfr();
	tsOutputChannels->v_batt = getVBatt();
	tsOutputChannels->tsConfigVersion = TS_FILE_VERSION;
	tsOutputChannels->tpsADC = getTPS10bitAdc();
	tsOutputChannels->atmospherePressure = getBaroPressure();
	tsOutputChannels->manifold_air_pressure = getMap();
	tsOutputChannels->checkEngine = hasErrorCodes();
#if EFI_PROD_CODE
	tsOutputChannels->needBurn = getNeedToWriteConfiguration();
	tsOutputChannels->hasSdCard = isSdCardAlive();
	tsOutputChannels->isFuelPumpOn = getOutputPinValue(FUEL_PUMP_RELAY);
	tsOutputChannels->isFanOn = getOutputPinValue(FAN_RELAY);
	tsOutputChannels->isO2HeaterOn = getOutputPinValue(O2_HEATER);
	tsOutputChannels->ignition_enabled = engineConfiguration->isIgnitionEnabled;
	tsOutputChannels->injection_enabled = engineConfiguration->isInjectionEnabled;
	tsOutputChannels->cylinder_cleanup_enabled = engineConfiguration->isCylinderCleanupEnabled;
	tsOutputChannels->secondTriggerChannelEnabled = engineConfiguration->secondTriggerChannelEnabled;

	tsOutputChannels->isCltError = !isValidCoolantTemperature(getCoolantTemperature());
	tsOutputChannels->isIatError = !isValidIntakeAirTemperature(getIntakeAirTemperature());
#endif
	tsOutputChannels->tCharge = getTCharge(rpm, tps, coolant, intake);
	tsOutputChannels->sparkDwell = getSparkDwellMs(rpm);
	tsOutputChannels->pulseWidth = getRunningFuel(baseFuel, &engine, rpm);
	tsOutputChannels->crankingFuel = getCrankingFuel();
}

extern TunerStudioOutputChannels tsOutputChannels;
#endif /* EFI_TUNER_STUDIO */

static void tsStatusThread(void *arg) {
	chRegSetThreadName("tuner s");
	while (true) {
#if EFI_TUNER_STUDIO
		// sensor state for EFI Analytics Tuner Studio
		updateTunerStudioState(&tsOutputChannels);
#endif /* EFI_TUNER_STUDIO */
		chThdSleepMilliseconds(boardConfiguration->tunerStudioThreadPeriod);
	}
}

void initStatusLoop(void) {
#if EFI_PROD_CODE || EFI_SIMULATOR
	initLoggingExt(&logger, "status loop", LOGGING_BUFFER, sizeof(LOGGING_BUFFER));
#endif /* EFI_PROD_CODE || EFI_SIMULATOR */

	setFullLog(INITIAL_FULL_LOG);
	addConsoleActionI(FULL_LOGGING_KEY, setFullLog);
	addConsoleActionI("warn", setWarningEnabled);

#if EFI_PROD_CODE
	initLogging(&logger2, "main event handler");

	addConsoleActionFF("fuelinfo2", showFuelInfo2);
	addConsoleAction("fuelinfo", showFuelInfo);

	addConsoleAction("status", printStatus);
#endif /* EFI_PROD_CODE */

#if EFI_FILE_LOGGING
	initLogging(&fileLogger, "file logger");
#endif /* EFI_FILE_LOGGING */

}

void startStatusThreads(void) {
	// todo: refactoring needed, this file should probably be split into pieces
	chThdCreateStatic(lcdThreadStack, sizeof(lcdThreadStack), NORMALPRIO, (tfunc_t) lcdThread, (void*) NULL);
	chThdCreateStatic(tsThreadStack, sizeof(tsThreadStack), NORMALPRIO, (tfunc_t) tsStatusThread, (void*) NULL);
}

void setFullLog(int value) {
	print("Setting full logging: %s\r\n", boolean2string(value));
	printMsg(&logger, "%s%d", FULL_LOGGING_KEY, value);
	fullLog = value;
}

bool getFullLog(void) {
	return fullLog;
}
