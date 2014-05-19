/**
 * @file	status_loop.c
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

#define PRINT_FIRMWARE_ONCE TRUE

static bool_t firmwareErrorReported = FALSE;

#include "advance_map.h"
#if EFI_TUNER_STUDIO
#include "tunerstudio.h"
#endif /* EFI_TUNER_STUDIO */

#include "wave_math.h"

#include "fuel_math.h"
#include "main_trigger_callback.h"
#include "engine_math.h"
#include "idle_thread.h"
#include "engine_configuration.h"
#include "rfiutil.h"
#include "svnversion.h"

#if EFI_PROD_CODE
// todo: move this logic to algo folder!
#include "rtc_helper.h"
#include "lcd_HD44780.h"
#include "rusefi.h"
#endif

// this 'true' value is needed for simulator
static volatile int fullLog = TRUE;
int warningEnabled = TRUE;
//int warningEnabled = FALSE;

extern engine_configuration_s * engineConfiguration;
extern engine_configuration2_s * engineConfiguration2;
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

static void reportSensorI(char *caption, int value) {
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

void finishStatusLine(void) {
	printLine(&logger);
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

	if (engineConfiguration2->hasMapSensor) {
		reportSensorF(getCaption(LP_MAP), getMap(), 2);
		reportSensorF("map_r", getRawMap(), 2);
	}
	reportSensorF("baro", getBaroPressure(), 2);

	reportSensorF("afr", getAfr(), 2);
	reportSensorF("vref", getVRef(), 2);
	reportSensorF("vbatt", getVBatt(), 2);

	reportSensorF(getCaption(LP_THROTTLE), getTPS(), 2);

	if (engineConfiguration2->hasCltSensor) {
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
	printSensors();

	int rpm = getRpm();
	debugInt(&logger, "ckp_c", currentCkpEventCounter);
	debugInt(&logger, "fuel_lag", getRevolutionCounter());

//	debugInt(&logger, "idl", getIdleSwitch());

//	debugFloat(&logger, "table_spark", getAdvance(rpm, getMaf()), 2);

	float engineLoad = getEngineLoad();
	debugFloat(&logger, "fuel_base", getBaseFuel(rpm, engineLoad), 2);
//	debugFloat(&logger, "fuel_iat", getIatCorrection(getIntakeAirTemperature()), 2);
//	debugFloat(&logger, "fuel_clt", getCltCorrection(getCoolantTemperature()), 2);
//	debugFloat(&logger, "fuel_lag", getInjectorLag(getVBatt()), 2);
	debugFloat(&logger, "fuel", getRunningFuel(rpm, engineLoad), 2);

	debugFloat(&logger, "timing", getAdvance(rpm, engineLoad), 2);

//		float map = getMap();
//		float fuel = getDefaultFuel(rpm, map);
//		debugFloat(&logger, "d_fuel", fuel, 2);

}

#define INITIAL_FULL_LOG TRUE
//#define INITIAL_FULL_LOG FALSE

static char LOGGING_BUFFER[500];

#if EFI_PROD_CODE

volatile int needToReportStatus = FALSE;
static int prevCkpEventCounter = -1;

static Logging logger2;

static void printStatus(void) {
	needToReportStatus = TRUE;
}

//float getTCharge1(float tps) {
//	float cltK = tempCtoKelvin(getCoolantTemperature());
//	float iatK = tempCtoKelvin(getIntakeAirTemperature());
//	return getTCharge(getCurrentRpm(), tps, cltK, iatK);
//}

#if EFI_CUSTOM_PANIC_METHOD
extern char *dbg_panic_file;
extern int dbg_panic_line;
#endif

bool_t hasFatalError(void) {
	return dbg_panic_msg != NULL;
}

static void checkIfShouldHalt(void) {
#if CH_DBG_ENABLED
	if (hasFatalError()) {
		setOutputPinValue(LED_ERROR, 1);
#if EFI_CUSTOM_PANIC_METHOD
		print("my FATAL [%s] at %s:%d\r\n", dbg_panic_msg, dbg_panic_file, dbg_panic_line);
#else
		print("my FATAL [%s] at %s:%d\r\n", dbg_panic_msg);
#endif
		chThdSleepSeconds(1);
		// todo: figure out how we halt exactly
		while (TRUE) {
		}
		chSysHalt();
	}
#endif
}

/**
 * Time when the firmware version was reported last time, in seconds
 * TODO: implement a request/response instead of just constantly sending this out
 */
static systime_t timeOfPreviousPrintVersion = (systime_t) -1;

static void printVersion(systime_t nowSeconds) {
	if (overflowDiff(nowSeconds, timeOfPreviousPrintVersion) < 4)
		return;
	timeOfPreviousPrintVersion = nowSeconds;
	appendPrintf(&logger, "rusEfiVersion%s%d@%d %s%s", DELIMETER, getRusEfiVersion(), SVN_VERSION,
			getConfigurationName(engineConfiguration),
			DELIMETER);
}

static systime_t timeOfPreviousReport = (systime_t) -1;

extern char errorMessageBuffer[200];

/**
 * @brief Sends all pending data to dev console
 */
void updateDevConsoleState(void) {
	if (!isConsoleReady())
		return;
	checkIfShouldHalt();
	printPending();

	if (hasFirmwareError()) {
		if (!firmwareErrorReported || !PRINT_FIRMWARE_ONCE)
			printMsg(&logger, "firmware error: %s", errorMessageBuffer);
		firmwareErrorReported = TRUE;
		warningEnabled = FALSE;
		return;
	}

	pokeAdcInputs();

	if (!fullLog)
		return;

	systime_t nowSeconds = getTimeNowSeconds();
	printVersion(nowSeconds);

	int currentCkpEventCounter = getCrankEventCounter();
	if (prevCkpEventCounter == currentCkpEventCounter && timeOfPreviousReport == nowSeconds)
		return;

	timeOfPreviousReport = nowSeconds;

	prevCkpEventCounter = currentCkpEventCounter;

	printState(currentCkpEventCounter);

#if EFI_WAVE_ANALYZER
//	printWave(&logger);
#endif

	finishStatusLine();
}

/*
 * command example:
 * sfm 3500 400
 * that would be 'show fuel for rpm 3500 maf 4.0'
 */

static void showFuelMap(int rpm, int key100) {
	float engineLoad = key100 / 100.0;

	float baseFuel = getBaseFuel(rpm, engineLoad);

	float iatCorrection = getIatCorrection(getIntakeAirTemperature());
	float cltCorrection = getCltCorrection(getCoolantTemperature());
	float injectorLag = getInjectorLag(getVBatt());
	print("baseFuel=%f\r\n", baseFuel);

	print("iatCorrection=%f cltCorrection=%f injectorLag=%d\r\n", iatCorrection, cltCorrection,
			(int) (100 * injectorLag));

	float value = getRunningFuel(rpm, engineLoad);

	print("fuel map rpm=%d, key=%f: %d\r\n", rpm, engineLoad, (int) (100 * value));

	scheduleMsg(&logger2, "fuel map value = %f", value);
}

static char buffer[10];
static char dateBuffer[30];

void updateHD44780lcd(void) {

	lcd_HD44780_set_position(0, 9);
	lcd_HD44780_print_char('R');
	lcd_HD44780_set_position(0, 10);

	char * ptr = itoa10((uint8_t*) buffer, getRpm());
	ptr[0] = 0;
	int len = ptr - buffer;
	for (int i = 0; i < 6 - len; i++)
		lcd_HD44780_print_char(' ');

	lcd_HD44780_print_string(buffer);

	lcd_HD44780_set_position(2, 0);
	lcd_HD44780_print_char('C');

	ftoa(buffer, getCoolantTemperature(), 100);
	lcd_HD44780_print_string(buffer);

#if EFI_PROD_CODE
	dateToString(dateBuffer);
	lcd_HD44780_set_position(1, 0);
	lcd_HD44780_print_string(dateBuffer);
#endif /* EFI_PROD_CODE */
}
#endif /* EFI_PROD_CODE */

static WORKING_AREA(lcdThreadStack, UTILITY_THREAD_STACK_SIZE);

static void lcdThread(void *arg) {
	chRegSetThreadName("lcd");
	while (true) {
#if EFI_HD44780_LCD
		updateHD44780lcd();
#endif
		chThdSleepMilliseconds(50);
	}
}

static WORKING_AREA(tsThreadStack, UTILITY_THREAD_STACK_SIZE);

static void tsStatusThread(void *arg) {
	chRegSetThreadName("tuner s");
	while (true) {
#if EFI_TUNER_STUDIO
		// sensor state for EFI Analytics Tuner Studio
		updateTunerStudioState();
#endif /* EFI_TUNER_STUDIO */
		chThdSleepMilliseconds(50);
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

	addConsoleActionII("sfm", showFuelMap);

	addConsoleAction("status", printStatus);
#endif /* EFI_PROD_CODE */

#if EFI_FILE_LOGGING
	initLogging(&fileLogger, "file logger");
#endif /* EFI_FILE_LOGGING */

}

void startStatusThreads(void) {
	// todo: refactoring needed, this file should probably be split into pieces
	chThdCreateStatic(lcdThreadStack, sizeof(lcdThreadStack), NORMALPRIO, (tfunc_t) lcdThread, NULL);
	chThdCreateStatic(tsThreadStack, sizeof(tsThreadStack), NORMALPRIO, (tfunc_t) tsStatusThread, NULL);
}

void setFullLog(int value) {
	print("Setting full logging: %s\r\n", boolean2string(value));
	printMsg(&logger, "%s%d", FULL_LOGGING_KEY, value);
	fullLog = value;
}

int getFullLog(void) {
	return fullLog;
}
