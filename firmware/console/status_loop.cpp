/**
 * @file	status_loop.cpp
 * @brief Human-readable protocol status messages
 *
 * http://rusefi.com/forum/viewtopic.php?t=263 Dev console overview
 * http://rusefi.com/forum/viewtopic.php?t=210 Commands overview
 *
 *
 * @date Mar 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2018
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

#include "adc_inputs.h"
#if EFI_WAVE_ANALYZER || defined(__DOXYGEN__)
#include "wave_analyzer.h"
#endif /* EFI_WAVE_ANALYZER */

// see RUS_EFI_VERSION_TAG in console source code
#define RUS_EFI_VERSION_TAG "rusEfiVersion"

#include "trigger_central.h"
#include "engine_state.h"
#include "io_pins.h"
#include "efiGpio.h"
#include "mmc_card.h"
#include "console_io.h"
#include "malfunction_central.h"
#include "speed_density.h"

#include "advance_map.h"
#include "tunerstudio.h"
#include "fuel_math.h"
#include "main_trigger_callback.h"
#include "engine_math.h"
#include "spark_logic.h"
#include "idle_thread.h"
#include "engine_configuration.h"
#include "rfiutil.h"
#include "svnversion.h"
#include "engine.h"
#include "lcd_controller.h"
#include "settings.h"
#include "can_hw.h"

extern afr_Map3D_t afrMap;
extern bool main_loop_started;

#if EFI_PROD_CODE || defined(__DOXYGEN__)
// todo: move this logic to algo folder!
#include "rtc_helper.h"
#include "lcd_HD44780.h"
#include "rusefi.h"
#include "pin_repository.h"
#include "flash_main.h"
#include "max31855.h"
#include "vehicle_speed.h"
#include "SingleTimerExecutor.h"
#include "CJ125.h"
#endif /* EFI_PROD_CODE */

#if EFI_MAP_AVERAGING
#include "map_averaging.h"
#endif

#if EFI_FSIO || defined(__DOXYGEN__)
#include "fsio_impl.h"
#endif /* EFI_FSIO */

// this 'true' value is needed for simulator
static volatile bool fullLog = true;
int warningEnabled = true;
//int warningEnabled = FALSE;

#if EFI_TUNER_STUDIO || defined(__DOXYGEN__)
extern TunerStudioOutputChannels tsOutputChannels;
#endif

extern bool hasFirmwareErrorFlag;
extern tunerstudio_counters_s tsState;
#define FULL_LOGGING_KEY "fl"

static char LOGGING_BUFFER[1800] CCM_OPTIONAL;
static Logging logger("status loop", LOGGING_BUFFER, sizeof(LOGGING_BUFFER));

static void setWarningEnabled(int value) {
	warningEnabled = value;
}

#if EFI_FILE_LOGGING || defined(__DOXYGEN__)
// this one needs to be in main ram so that SD card SPI DMA works fine
static char FILE_LOGGER[1000] MAIN_RAM;
static Logging fileLogger("file logger", FILE_LOGGER, sizeof(FILE_LOGGER));
#endif /* EFI_FILE_LOGGING */

static int logFileLineIndex = 0;
#define TAB "\t"

static void reportSensorF(Logging *log, bool fileFormat, const char *caption, const char *units, float value,
		int precision) {
	if (!fileFormat) {
#if (EFI_PROD_CODE || EFI_SIMULATOR) || defined(__DOXYGEN__)
		debugFloat(log, caption, value, precision);
#endif /* EFI_PROD_CODE || EFI_SIMULATOR */
	} else {

#if EFI_FILE_LOGGING || defined(__DOXYGEN__)
		if (logFileLineIndex == 0) {
			append(log, caption);
			append(log, TAB);
		} else if (logFileLineIndex == 1) {
			append(log, units);
			append(log, TAB);
		} else {
			appendFloat(log, value, precision);
			append(log, TAB);
		}
#endif /* EFI_FILE_LOGGING */
	}
}

static void reportSensorI(Logging *log, bool fileFormat, const char *caption, const char *units, int value) {
	if (!fileFormat) {

#if (EFI_PROD_CODE || EFI_SIMULATOR) || defined(__DOXYGEN__)
		debugInt(log, caption, value);
#endif /* EFI_PROD_CODE || EFI_SIMULATOR */
	} else {
#if EFI_FILE_LOGGING || defined(__DOXYGEN__)
		if (logFileLineIndex == 0) {
			append(log, caption);
			append(log, TAB);
		} else if (logFileLineIndex == 1) {
			append(log, units);
			append(log, TAB);
		} else {
			appendPrintf(log, "%d%s", value, TAB);
		}
#endif /* EFI_FILE_LOGGING */
	}
}

EXTERN_ENGINE
;

static char buf[6];

/**
 * This is useful if we are changing engine mode dynamically
 * For example http://rusefi.com/forum/viewtopic.php?f=5&t=1085
 */
static int packEngineMode(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	return (engineConfiguration->fuelAlgorithm << 4) +
			(engineConfiguration->injectionMode << 2) +
			engineConfiguration->ignitionMode;
}

static void printSensors(Logging *log, bool fileFormat) {
	// current time, in milliseconds
	int nowMs = currentTimeMillis();
	float sec = ((float) nowMs) / 1000;
	reportSensorF(log, fileFormat, "time", "", sec, 3); // log column 1

	int rpm = 0;
#if EFI_SHAFT_POSITION_INPUT || defined(__DOXYGEN__)
	rpm = getRpmE(engine);
	reportSensorI(log, fileFormat, "rpm", "RPM", rpm); // log column 2
#endif
	// why do we still send data into console in text mode?

	if (hasCltSensor()) {
		reportSensorF(log, fileFormat, "CLT", "C", getCoolantTemperature(PASS_ENGINE_PARAMETER_SIGNATURE), 2); // log column #4
	}
	if (hasTpsSensor()) {
		reportSensorF(log, fileFormat, "TPS", "%", getTPS(PASS_ENGINE_PARAMETER_SIGNATURE), 2); // log column #5
	}

	if (hasIatSensor()) {
		reportSensorF(log, fileFormat, "IAT", "C", getIntakeAirTemperature(PASS_ENGINE_PARAMETER_SIGNATURE), 2); // log column #7
	}

	if (hasVBatt(PASS_ENGINE_PARAMETER_SIGNATURE)) {
		reportSensorF(log, fileFormat, GAUGE_NAME_VBAT, "V", getVBatt(PASS_ENGINE_PARAMETER_SIGNATURE), 2); // log column #6
	}
#if EFI_ANALOG_SENSORS || defined(__DOXYGEN__)
	if (hasMapSensor(PASS_ENGINE_PARAMETER_SIGNATURE)) {
		reportSensorF(log, fileFormat, "MAP", "kPa", getMap(), 2);
//		reportSensorF(log, fileFormat, "map_r", "V", getRawMap(), 2);
	}
#endif /* EFI_ANALOG_SENSORS */
#if EFI_ANALOG_SENSORS || defined(__DOXYGEN__)
	if (hasBaroSensor()) {
		reportSensorF(log, fileFormat, "baro", "kPa", getBaroPressure(), 2);
	}
#endif /* EFI_ANALOG_SENSORS */


	if (!fileFormat) {
		return;
	}

	if (hasAfrSensor(PASS_ENGINE_PARAMETER_SIGNATURE)) {
		reportSensorF(log, fileFormat, GAUGE_NAME_AFR, "AFR", getAfr(PASS_ENGINE_PARAMETER_SIGNATURE), 2);
	}

	// below are the more advanced data points which only go into log file


#if EFI_PROD_CODE || defined(__DOXYGEN__)
	reportSensorF(log, fileFormat, GAUGE_NAME_CPU_TEMP, "C", getMCUInternalTemperature(), 2); // log column #3
#endif

	reportSensorI(log, fileFormat, "mode", "v", packEngineMode(PASS_ENGINE_PARAMETER_SIGNATURE)); // log column #3

	reportSensorF(log, fileFormat, GAUGE_NAME_ACCEL_X, "G", engine->sensors.accelerometer.x, 3);
	reportSensorF(log, fileFormat, GAUGE_NAME_ACCEL_Y, "G", engine->sensors.accelerometer.y, 3);

	if (hasMafSensor()) {
		reportSensorF(log, fileFormat, "maf", "V", getMaf(PASS_ENGINE_PARAMETER_SIGNATURE), 2);
		reportSensorF(log, fileFormat, "mafr", "kg/hr", getRealMaf(PASS_ENGINE_PARAMETER_SIGNATURE), 2);
	}

#if EFI_IDLE_CONTROL || defined(__DOXYGEN__)
	reportSensorF(log, fileFormat, GAUGE_NAME_IAC, "%", getIdlePosition(), 2);
#endif /* EFI_IDLE_CONTROL */

#if EFI_ANALOG_SENSORS || defined(__DOXYGEN__)
	reportSensorF(log, fileFormat, GAUGE_NAME_TARGET_AFR, "AFR", engine->engineState.targetAFR, 2);
#endif /* EFI_ANALOG_SENSORS */


#define DEBUG_F_PRECISION 6

#if EFI_TUNER_STUDIO || defined(__DOXYGEN__)
		reportSensorF(log, fileFormat, GAUGE_NAME_DEBUG_F1, "v", tsOutputChannels.debugFloatField1, DEBUG_F_PRECISION);
		reportSensorF(log, fileFormat, GAUGE_NAME_DEBUG_F1, "v", tsOutputChannels.debugFloatField2, DEBUG_F_PRECISION);
		reportSensorF(log, fileFormat, GAUGE_NAME_DEBUG_F1, "v", tsOutputChannels.debugFloatField3, DEBUG_F_PRECISION);
		reportSensorF(log, fileFormat, GAUGE_NAME_DEBUG_F1, "v", tsOutputChannels.debugFloatField4, DEBUG_F_PRECISION);
		reportSensorF(log, fileFormat, GAUGE_NAME_DEBUG_F1, "v", tsOutputChannels.debugFloatField5, DEBUG_F_PRECISION);
		reportSensorF(log, fileFormat, GAUGE_NAME_DEBUG_F1, "v", tsOutputChannels.debugFloatField6, DEBUG_F_PRECISION);
		reportSensorF(log, fileFormat, GAUGE_NAME_DEBUG_F1, "v", tsOutputChannels.debugFloatField7, DEBUG_F_PRECISION);

		reportSensorI(log, fileFormat, GAUGE_NAME_DEBUG_I1, "v", tsOutputChannels.debugIntField1);
		reportSensorI(log, fileFormat, GAUGE_NAME_DEBUG_I2, "v", tsOutputChannels.debugIntField2);
		reportSensorI(log, fileFormat, GAUGE_NAME_DEBUG_I3, "v", tsOutputChannels.debugIntField3);
#endif /* EFI_TUNER_STUDIO */

		reportSensorF(log, fileFormat, GAUGE_NAME_TCHARGE, "K", engine->engineState.tChargeK, 2); // log column #8
		if (hasMapSensor(PASS_ENGINE_PARAMETER_SIGNATURE)) {
			reportSensorF(log, fileFormat, GAUGE_NAME_FUEL_VR, "%", engine->engineState.currentVE * PERCENT_MULT, 2);
		}
		reportSensorF(log, fileFormat, GAUGE_NAME_VVT, "deg", engine->triggerCentral.vvtPosition, 1);

	float engineLoad = getEngineLoadT(PASS_ENGINE_PARAMETER_SIGNATURE);
	reportSensorF(log, fileFormat, GAUGE_NAME_ENGINE_LOAD, "x", engineLoad, 2);


	reportSensorF(log, fileFormat, GAUGE_COIL_DWELL_TIME, "ms", ENGINE(engineState.sparkDwell), 2);
	reportSensorF(log, fileFormat, GAUGE_NAME_TIMING_ADVANCE, "deg", engine->engineState.timingAdvance, 2);


		floatms_t fuelBase = getBaseFuel(rpm PASS_ENGINE_PARAMETER_SUFFIX);
		reportSensorF(log, fileFormat, GAUGE_NAME_FUEL_BASE, "ms", fuelBase, 2);
		reportSensorF(log, fileFormat, GAUGE_NAME_FUEL_LAST_INJECTION, "ms", ENGINE(actualLastInjection), 2);
		reportSensorF(log, fileFormat, GAUGE_NAME_INJECTOR_LAG, "ms", engine->engineState.injectorLag, 2);
		reportSensorF(log, fileFormat, GAUGE_NAME_FUEL_RUNNING, "ms", ENGINE(engineState.runningFuel), 2);
		reportSensorF(log, fileFormat, GAUGE_NAME_FUEL_PID_CORR, "ms", ENGINE(engineState.fuelPidCorrection), 2);

		reportSensorF(log, fileFormat, "f: wall amt", "v", ENGINE(wallFuel).getWallFuel(0), 2);
		reportSensorF(log, fileFormat, "f: wall crr", "v", ENGINE(wallFuelCorrection), 2);

		reportSensorI(log, fileFormat, GAUGE_NAME_VERSION, "#", getRusEfiVersion());

#if EFI_VEHICLE_SPEED || defined(__DOXYGEN__)
	if (hasVehicleSpeedSensor()) {
		float vehicleSpeed = getVehicleSpeed();
		reportSensorF(log, fileFormat, GAUGE_NAME_VVS, "kph", vehicleSpeed, 2);
		float sp2rpm = rpm == 0 ? 0 : vehicleSpeed / rpm;
		reportSensorF(log, fileFormat, "sp2rpm", "x", sp2rpm, 2);
	}
#endif /* EFI_PROD_CODE */


	reportSensorF(log, fileFormat, GAUGE_NAME_KNOCK_COUNTER, "count", engine->knockCount, 0);
	reportSensorF(log, fileFormat, GAUGE_NAME_KNOCK_LEVEL, "v", engine->knockVolts, 2);

	//	reportSensorF(log, fileFormat, "vref", "V", getVRef(engineConfiguration), 2);


		reportSensorF(log, fileFormat, "f: tps delta", "v", engine->tpsAccelEnrichment.getMaxDelta(), 2);
		reportSensorF(log, fileFormat, GAUGE_NAME_FUEL_TPS_EXTRA, "ms", engine->engineState.tpsAccelEnrich, 2);

		reportSensorF(log, fileFormat, "f: el delta", "v", engine->engineLoadAccelEnrichment.getMaxDelta(), 2);
		if (hasMapSensor(PASS_ENGINE_PARAMETER_SIGNATURE)) {
			reportSensorF(log, fileFormat, "f: el fuel", "v", engine->engineLoadAccelEnrichment.getEngineLoadEnrichment(PASS_ENGINE_PARAMETER_SIGNATURE) * 100 / getMap(), 2);
		}

		reportSensorF(log, fileFormat, GAUGE_NAME_FUEL_INJ_DUTY, "%", getInjectorDutyCycle(rpm PASS_ENGINE_PARAMETER_SUFFIX), 2);
		reportSensorF(log, fileFormat, GAUGE_NAME_DWELL_DUTY, "%", getCoilDutyCycle(rpm PASS_ENGINE_PARAMETER_SUFFIX), 2);



//	debugFloat(&logger, "tch", getTCharge1(tps), 2);

	for (int i = 0;i<FSIO_ANALOG_INPUT_COUNT;i++) {
		if (engineConfiguration->fsioAdc[i] != EFI_ADC_NONE) {
			strcpy(buf, "adcX");
			buf[3] = '0' + i;
			reportSensorF(log, fileFormat, buf, "", getVoltage("fsio", engineConfiguration->fsioAdc[i]), 2);
		}
	}

		reportSensorI(log, fileFormat, GAUGE_NAME_WARNING_COUNTER, "count", engine->engineState.warningCounter);
		reportSensorI(log, fileFormat, GAUGE_NAME_WARNING_LAST, "code", engine->engineState.lastErrorCode);

		reportSensorI(log, fileFormat, INDICATOR_NAME_CLUTCH_UP, "bool", engine->clutchUpState);
		reportSensorI(log, fileFormat, INDICATOR_NAME_CLUTCH_DOWN, "bool", engine->clutchDownState);
		reportSensorI(log, fileFormat, INDICATOR_NAME_BRAKE_DOWN, "bool", engine->brakePedalState);

}


void writeLogLine(void) {
#if EFI_FILE_LOGGING || defined(__DOXYGEN__)
	if (!main_loop_started)
		return;
	resetLogging(&fileLogger);
	printSensors(&fileLogger, true);

	if (isSdCardAlive()) {
		appendPrintf(&fileLogger, "\r\n");
		appendToLog(fileLogger.buffer);
		logFileLineIndex++;
	}
#endif /* EFI_FILE_LOGGING */
}

#define INITIAL_FULL_LOG TRUE
//#define INITIAL_FULL_LOG FALSE

volatile int needToReportStatus = FALSE;
static int prevCkpEventCounter = -1;

static LoggingWithStorage logger2("main event handler");

static void printStatus(void) {
	needToReportStatus = TRUE;
}

/**
 * Time when the firmware version was reported last time, in seconds
 * TODO: implement a request/response instead of just constantly sending this out
 */
static systime_t timeOfPreviousPrintVersion = (systime_t) -1;

#if EFI_PROD_CODE || defined(__DOXYGEN__)
static void printOutPin(const char *pinName, brain_pin_e hwPin) {
	if (hwPin != GPIO_UNASSIGNED) {
		appendPrintf(&logger, "outpin%s%s@%s%s", DELIMETER, pinName, hwPortname(hwPin), DELIMETER);
	}
}
#endif /* EFI_PROD_CODE */

void printOverallStatus(systime_t nowSeconds) {
	/**
	 * we report the version every 4 seconds - this way the console does not need to
	 * request it and we will display it pretty soon
	 */
	if (overflowDiff(nowSeconds, timeOfPreviousPrintVersion) < 4) {
		return;
	}
	timeOfPreviousPrintVersion = nowSeconds;
	appendPrintf(&logger, "%s%s%d@%s %s %d%s", RUS_EFI_VERSION_TAG, DELIMETER,
			getRusEfiVersion(), VCS_VERSION,
			getConfigurationName(engineConfiguration->engineType),
			getTimeNowSeconds(),
			DELIMETER);
#if EFI_PROD_CODE || defined(__DOXYGEN__)
	printOutPin(CRANK1, boardConfiguration->triggerInputPins[0]);
	printOutPin(CRANK2, boardConfiguration->triggerInputPins[1]);
	printOutPin(VVT_NAME, engineConfiguration->camInput);
	printOutPin(HIP_NAME, boardConfiguration->hip9011IntHoldPin);
	printOutPin(TACH_NAME, boardConfiguration->tachOutputPin);
	printOutPin(DIZZY_NAME, engineConfiguration->dizzySparkOutputPin);
#if EFI_WAVE_ANALYZER || defined(__DOXYGEN__)
	printOutPin(WA_CHANNEL_1, boardConfiguration->logicAnalyzerPins[0]);
	printOutPin(WA_CHANNEL_2, boardConfiguration->logicAnalyzerPins[1]);
#endif /* EFI_WAVE_ANALYZER */

	for (int i = 0; i < engineConfiguration->specs.cylindersCount; i++) {
		printOutPin(enginePins.coils[i].name, boardConfiguration->ignitionPins[i]);

		printOutPin(enginePins.injectors[i].name, boardConfiguration->injectionPins[i]);
	}
	for (int i = 0; i < AUX_DIGITAL_VALVE_COUNT;i++) {
		printOutPin(enginePins.auxValve[i].name, engineConfiguration->auxValves[i]);
	}

#endif /* EFI_PROD_CODE */
	scheduleLogging(&logger);
}

static systime_t timeOfPreviousReport = (systime_t) -1;

extern bool consoleInBinaryMode;

/**
 * @brief Sends all pending data to dev console
 *
 * This method is periodically invoked by the main loop
 */
void updateDevConsoleState(void) {
	// todo: make SWO work
//	char *msg = "hello\r\n";
//	for(int i=0;i<strlen(msg);i++) {
//		ITM_SendChar(msg[i]);
//	}



	if (!isCommandLineConsoleReady()) {
		return;
	}
// looks like this is not needed anymore
//	checkIfShouldHalt();
	if (!consoleInBinaryMode) {
		printPending();
	}

	/**
	 * this should go before the firmware error so that console can detect connection
	 */
	printSensors(&logger, false);

#if EFI_PROD_CODE || defined(__DOXYGEN__)
	// todo: unify with simulator!
	if (hasFirmwareError()) {
		scheduleMsg(&logger, "FATAL error: %s", getFirmwareError());
		warningEnabled = false;
		scheduleLogging(&logger);
		return;
	}
#endif

#if (EFI_PROD_CODE && HAL_USE_ADC) || defined(__DOXYGEN__)
	printFullAdcReportIfNeeded(&logger);
#endif

	if (!fullLog) {
		return;
	}

	systime_t nowSeconds = getTimeNowSeconds();

#if EFI_ENGINE_CONTROL || defined(__DOXYGEN__)
	int currentCkpEventCounter = getCrankEventCounter();
	if (prevCkpEventCounter == currentCkpEventCounter && timeOfPreviousReport == nowSeconds) {
		return;
	}
	timeOfPreviousReport = nowSeconds;

	prevCkpEventCounter = currentCkpEventCounter;
#else
	chThdSleepMilliseconds(200);
#endif

#if EFI_WAVE_ANALYZER
	printWave(&logger);
#endif

	scheduleLogging(&logger);
}

/*
 * command example:
 * sfm 3500 400
 * that would be 'show fuel for rpm 3500 maf 4.0'
 */

static void showFuelInfo2(float rpm, float engineLoad) {

	float baseFuelMs = getBaseTableFuel((int) rpm, engineLoad);

	float magicAir = getCylinderAirMass(engineConfiguration, 1, 100, convertCelsiusToKelvin(20));

	scheduleMsg(&logger, "SD magic fuel %.2f", sdMath(engineConfiguration, magicAir, 14.7));
	scheduleMsg(&logger, "inj flow %.2fcc/min displacement %.2fL", engineConfiguration->injector.flow,
			engineConfiguration->specs.displacement);

	scheduleMsg(&logger2, "algo=%s/pump=%s", getEngine_load_mode_e(engineConfiguration->fuelAlgorithm),
			boolToString(enginePins.fuelPumpRelay.getLogicValue()));

	scheduleMsg(&logger2, "injection phase=%.2f/global fuel correction=%.2f", getinjectionOffset(rpm), engineConfiguration->globalFuelCorrection);

	scheduleMsg(&logger2, "baro correction=%.2f", engine->engineState.baroCorrection);

#if EFI_ENGINE_CONTROL || defined(__DOXYGEN__)
	scheduleMsg(&logger, "base cranking fuel %.2f", engineConfiguration->cranking.baseFuel);
	scheduleMsg(&logger2, "cranking fuel: %.2f", getCrankingFuel(PASS_ENGINE_PARAMETER_SIGNATURE));

	if (!engine->rpmCalculator.isStopped(PASS_ENGINE_PARAMETER_SIGNATURE)) {
		float iatCorrection = engine->engineState.iatFuelCorrection;
		float cltCorrection = engine->engineState.cltFuelCorrection;
		floatms_t injectorLag = engine->engineState.injectorLag;
		scheduleMsg(&logger2, "rpm=%.2f engineLoad=%.2f", rpm, engineLoad);
		scheduleMsg(&logger2, "baseFuel=%.2f", baseFuelMs);

		scheduleMsg(&logger2, "iatCorrection=%.2f cltCorrection=%.2f injectorLag=%.2f", iatCorrection, cltCorrection,
				injectorLag);

		float value = getRunningFuel(baseFuelMs PASS_ENGINE_PARAMETER_SUFFIX);
		scheduleMsg(&logger2, "injection pulse width: %.2f", value);
	}
#endif
}

#if EFI_ENGINE_CONTROL || defined(__DOXYGEN__)
static void showFuelInfo(void) {
	showFuelInfo2((float) getRpmE(engine), getEngineLoadT(PASS_ENGINE_PARAMETER_SIGNATURE));
}
#endif

static THD_WORKING_AREA(lcdThreadStack, UTILITY_THREAD_STACK_SIZE);

/**
 * blinking thread to show that we are alive
 * that's a trivial task - a smaller stack should work
 */
static THD_WORKING_AREA(blinkingStack, 128);

static OutputPin *leds[] = { &enginePins.warningPin, &enginePins.runningPin, &enginePins.checkEnginePin,
		&enginePins.errorLedPin, &enginePins.communicationPin, &enginePins.checkEnginePin };

static void initStatusLeds(void) {
	enginePins.communicationPin.initPin("led: comm status", engineConfiguration->communicationPin);
	// we initialize this here so that we can blink it on start-up
	enginePins.checkEnginePin.initPin("MalfunctionIndicator", boardConfiguration->malfunctionIndicatorPin, &boardConfiguration->malfunctionIndicatorPinMode);


#if EFI_WARNING_LED || defined(__DOXYGEN__)
	enginePins.warningPin.initPin("led: warning status", LED_WARNING_BRAIN_PIN);
	enginePins.runningPin.initPin("led: running status", engineConfiguration->runningPin);
#endif /* EFI_WARNING_LED */
}

/**
 * This method would blink all the LEDs just to test them
 */
static void initialLedsBlink(void) {
	if (hasFirmwareError()) {
		// make sure we do not turn the fatal LED off if already have
		// fatal error by now
		return;
	}
	int size = sizeof(leds) / sizeof(leds[0]);
	for (int i = 0; i < size && !hasFirmwareError(); i++)
		leds[i]->setValue(1);

	chThdSleepMilliseconds(100);

	// re-checking in case the error has happened while we were sleeping
	for (int i = 0; i < size && !hasFirmwareError(); i++)
		leds[i]->setValue(0);
}

int blinkingPeriod = 33;

/**
 * this is useful to test connectivity
 */
static void setBlinkingPeriod(int value) {
	if (value > 0)
		blinkingPeriod = value;
}

#if EFI_PROD_CODE || defined(__DOXYGEN__)

extern efitick_t lastDecodingErrorTime;

static bool isTriggerErrorNow() {
	bool justHadError = (getTimeNowNt() - lastDecodingErrorTime) < US2NT(2 * 1000 * 3 * blinkingPeriod);
	return justHadError || isTriggerDecoderError();
}

extern bool consoleByteArrived;

/**
 * this thread has a lower-then-usual stack size so we cannot afford *print* methods here
 */
static void blinkingThread(void *arg) {
	(void) arg;
	chRegSetThreadName("communication blinking");

	initialLedsBlink();

	while (true) {
		int delayMs = is_usb_serial_ready() ? 3 * blinkingPeriod : blinkingPeriod;

#if EFI_INTERNAL_FLASH || defined(__DOXYGEN__)
		if (getNeedToWriteConfiguration()) {
			delayMs = 2 * delayMs;
		}
#endif

		if (!hasFirmwareError() && !hasFirmwareErrorFlag) {
			enginePins.communicationPin.setValue(0);
		}
		enginePins.warningPin.setValue(0);
		chThdSleepMilliseconds(delayMs);

		enginePins.communicationPin.setValue(1);
#if EFI_ENGINE_CONTROL || defined(__DOXYGEN__)
		if (isTriggerErrorNow() || isIgnitionTimingError() || consoleByteArrived) {
			consoleByteArrived = false;
			enginePins.warningPin.setValue(1);
		}
#endif
		chThdSleepMilliseconds(delayMs);

	}
}

#endif /* EFI_PROD_CODE */

static void lcdThread(void *arg) {
	(void)arg;
	chRegSetThreadName("lcd");
	while (true) {
		if (engineConfiguration->bc.useLcdScreen) {
#if EFI_HD44780_LCD
			updateHD44780lcd();
#endif
		}
		chThdSleepMilliseconds(engineConfiguration->bc.lcdThreadPeriod);
	}
}

#if EFI_HIP_9011 || defined(__DOXYGEN__)
extern int correctResponsesCount;
extern int invalidResponsesCount;
#endif /* EFI_HIP_9011 */

#if EFI_TUNER_STUDIO || defined(__DOXYGEN__)

void updateTunerStudioState(TunerStudioOutputChannels *tsOutputChannels DECLARE_ENGINE_PARAMETER_SUFFIX) {
#if EFI_SHAFT_POSITION_INPUT || defined(__DOXYGEN__)
	int rpm = getRpmE(engine);
#else
	int rpm = 0;
#endif

#if EFI_PROD_CODE || defined(__DOXYGEN__)
	executorStatistics();
#endif

	float tps = getTPS(PASS_ENGINE_PARAMETER_SIGNATURE);
	float coolant = getCoolantTemperature(PASS_ENGINE_PARAMETER_SIGNATURE);
	float intake = getIntakeAirTemperature(PASS_ENGINE_PARAMETER_SIGNATURE);

	float engineLoad = getEngineLoadT(PASS_ENGINE_PARAMETER_SIGNATURE);

	// header
	tsOutputChannels->tsConfigVersion = TS_FILE_VERSION;

	// engine state
	tsOutputChannels->rpm = rpm;
	tsOutputChannels->coolantTemperature = coolant;
	tsOutputChannels->intakeAirTemperature = intake;
	tsOutputChannels->throttlePositon = tps;
	tsOutputChannels->massAirFlowVoltage = hasMafSensor() ? getMaf(PASS_ENGINE_PARAMETER_SIGNATURE) : 0;
    tsOutputChannels->massAirFlow = hasMafSensor() ? getRealMaf(PASS_ENGINE_PARAMETER_SIGNATURE) : 0;
    tsOutputChannels->oilPressure = engine->sensors.oilPressure;

    tsOutputChannels->accelerationX = engine->sensors.accelerometer.x;
    tsOutputChannels->accelerationY = engine->sensors.accelerometer.y;

	if (hasMapSensor(PASS_ENGINE_PARAMETER_SIGNATURE)) {
		float mapValue = getMap();
		tsOutputChannels->veValue = engine->engineState.currentVE * PERCENT_MULT;
		// todo: bug here? target afr could work based on real MAF?
		tsOutputChannels->currentTargetAfr = afrMap.getValue(rpm, mapValue);
		tsOutputChannels->manifoldAirPressure = mapValue;
	}
	if (hasAfrSensor(PASS_ENGINE_PARAMETER_SIGNATURE)) {
		tsOutputChannels->airFuelRatio = getAfr(PASS_ENGINE_PARAMETER_SIGNATURE);
	}
	if (hasVBatt(PASS_ENGINE_PARAMETER_SIGNATURE)) {
		tsOutputChannels->vBatt = getVBatt(PASS_ENGINE_PARAMETER_SIGNATURE);
	}
	tsOutputChannels->tpsADC = getTPS12bitAdc(PASS_ENGINE_PARAMETER_SIGNATURE) / TPS_TS_CONVERSION;
#if EFI_ANALOG_SENSORS || defined(__DOXYGEN__)
	tsOutputChannels->baroPressure = hasBaroSensor() ? getBaroPressure() : 0;
#endif /* EFI_ANALOG_SENSORS */
	tsOutputChannels->engineLoad = engineLoad;

	tsOutputChannels->triggerErrorsCounter = engine->triggerCentral.triggerState.totalTriggerErrorCounter;
	tsOutputChannels->baroCorrection = engine->engineState.baroCorrection;
	tsOutputChannels->pedalPosition = hasPedalPositionSensor(PASS_ENGINE_PARAMETER_SIGNATURE) ? getPedalPosition(PASS_ENGINE_PARAMETER_SIGNATURE) : 0;
	tsOutputChannels->knockCount = engine->knockCount;
	tsOutputChannels->knockLevel = engine->knockVolts;
	tsOutputChannels->fuelTankGauge = engine->sensors.fuelTankGauge;

	tsOutputChannels->hasFatalError = hasFirmwareError();
	tsOutputChannels->totalTriggerErrorCounter = engine->triggerCentral.triggerState.totalTriggerErrorCounter;

	tsOutputChannels->injectorDutyCycle = getInjectorDutyCycle(rpm PASS_ENGINE_PARAMETER_SUFFIX);
	tsOutputChannels->fuelRunning = ENGINE(engineState.runningFuel);
	tsOutputChannels->fuelPidCorrection = ENGINE(engineState.fuelPidCorrection);
	tsOutputChannels->injectorLagMs = ENGINE(engineState.injectorLag);
	tsOutputChannels->fuelBase = engine->engineState.baseFuel;
	tsOutputChannels->actualLastInjection = ENGINE(actualLastInjection);

	tsOutputChannels->coilDutyCycle = getCoilDutyCycle(rpm PASS_ENGINE_PARAMETER_SUFFIX);

	efitimesec_t now = getTimeNowSeconds();
	tsOutputChannels->timeSeconds = now;
	tsOutputChannels->firmwareVersion = getRusEfiVersion();

	tsOutputChannels->isWarnNow = isWarningNow(now, true);
	tsOutputChannels->isCltBroken = engine->isCltBroken;

	switch (engineConfiguration->debugMode)	{
	case DBG_STATUS:
		tsOutputChannels->debugFloatField1 = getTimeNowSeconds();
		tsOutputChannels->debugIntField1 = atoi(VCS_VERSION);
		break;
	case DBG_TPS_ACCEL:
		tsOutputChannels->debugIntField1 = engine->tpsAccelEnrichment.cb.getSize();
		break;
	case DBG_SR5_PROTOCOL: {
		const int _10_6 = 100000;
		tsOutputChannels->debugIntField1 = tsState.textCommandCounter * _10_6 +  tsState.totalCounter;
		tsOutputChannels->debugIntField2 = tsState.outputChannelsCommandCounter * _10_6 + tsState.writeValueCommandCounter;
		tsOutputChannels->debugIntField3 = tsState.readPageCommandsCounter * _10_6 + tsState.burnCommandCounter;
		break;
		}
	case DBG_AUX_VALVES:
		tsOutputChannels->debugFloatField1 = engine->engineState.auxValveStart;
		tsOutputChannels->debugFloatField2 = engine->engineState.auxValveEnd;
		break;
	case DBG_TRIGGER_INPUT:
		tsOutputChannels->debugIntField1 = engine->triggerCentral.getHwEventCounter((int)SHAFT_PRIMARY_FALLING);
		tsOutputChannels->debugIntField2 = engine->triggerCentral.getHwEventCounter((int)SHAFT_SECONDARY_FALLING);
		tsOutputChannels->debugIntField3 = engine->triggerCentral.getHwEventCounter((int)SHAFT_3RD_FALLING);

		tsOutputChannels->debugFloatField1 = engine->triggerCentral.getHwEventCounter((int)SHAFT_PRIMARY_RISING);
		tsOutputChannels->debugFloatField2 = engine->triggerCentral.getHwEventCounter((int)SHAFT_SECONDARY_RISING);
		tsOutputChannels->debugFloatField3 = engine->triggerCentral.getHwEventCounter((int)SHAFT_3RD_RISING);
		tsOutputChannels->debugFloatField4 = engine->rpmCalculator.getRpmAcceleration();
		break;
	case DBG_FSIO_ADC:
		// todo: implement a proper loop
		if (engineConfiguration->fsioAdc[0] != EFI_ADC_NONE) {
			strcpy(buf, "adcX");
			tsOutputChannels->debugFloatField1 = getVoltage("fsio", engineConfiguration->fsioAdc[0]);
		}
		break;
	case DBG_FSIO_EXPRESSION:
#if EFI_PROD_CODE && EFI_FSIO || defined(__DOXYGEN__)
		tsOutputChannels->debugFloatField1 = getFsioOutputValue(0 PASS_ENGINE_PARAMETER_SUFFIX);
		tsOutputChannels->debugFloatField2 = getFsioOutputValue(1 PASS_ENGINE_PARAMETER_SUFFIX);
		tsOutputChannels->debugFloatField3 = getFsioOutputValue(2 PASS_ENGINE_PARAMETER_SUFFIX);
		tsOutputChannels->debugFloatField4 = getFsioOutputValue(3 PASS_ENGINE_PARAMETER_SUFFIX);
		tsOutputChannels->debugFloatField5 = getFsioOutputValue(4 PASS_ENGINE_PARAMETER_SUFFIX);
		tsOutputChannels->debugFloatField6 = getFsioOutputValue(5 PASS_ENGINE_PARAMETER_SUFFIX);
		tsOutputChannels->debugFloatField7 = getFsioOutputValue(6 PASS_ENGINE_PARAMETER_SUFFIX);
#endif /* EFI_FSIO */
		break;
	case DBG_VEHICLE_SPEED_SENSOR:
		tsOutputChannels->debugIntField1 = engine->engineState.vssEventCounter;
		break;
	case DBG_SD_CARD:
		tsOutputChannels->debugIntField1 = engine->engineState.totalLoggedBytes;
		break;
	case DBG_CRANKING_DETAILS:
		tsOutputChannels->debugIntField1 = engine->rpmCalculator.getRevolutionCounterSinceStart();
		break;
#if EFI_HIP_9011 || defined(__DOXYGEN__)
	case DBG_KNOCK:
		tsOutputChannels->debugIntField1 = correctResponsesCount;
		tsOutputChannels->debugIntField2 = invalidResponsesCount;
		break;
#endif /* EFI_HIP_9011 */
#if EFI_CJ125 || defined(__DOXYGEN__)
	case DBG_CJ125:
		cjPostState(tsOutputChannels);
		break;
#endif /* EFI_CJ125 */
#if EFI_MAP_AVERAGING
		case DBG_MAP:
		postMapState(tsOutputChannels);
		break;
#endif /* EFI_MAP_AVERAGING */
#if EFI_CAN_SUPPORT || defined(__DOXYGEN__)
	case DBG_CAN:
		postCanState(tsOutputChannels);
		break;
#endif /* EFI_CAN_SUPPORT */
	case DBG_ADC:
		tsOutputChannels->debugFloatField1 = (engineConfiguration->vbattAdcChannel != EFI_ADC_NONE) ? getVoltageDivided("vbatt", engineConfiguration->vbattAdcChannel) : 0.0f;
		tsOutputChannels->debugFloatField2 = (engineConfiguration->tpsAdcChannel != EFI_ADC_NONE) ? getVoltageDivided("tps", engineConfiguration->tpsAdcChannel) : 0.0f;
		tsOutputChannels->debugFloatField3 = (engineConfiguration->mafAdcChannel != EFI_ADC_NONE) ? getVoltageDivided("maf", engineConfiguration->mafAdcChannel) : 0.0f;
		tsOutputChannels->debugFloatField4 = (engineConfiguration->map.sensor.hwChannel != EFI_ADC_NONE) ? getVoltageDivided("map", engineConfiguration->map.sensor.hwChannel) : 0.0f;
		tsOutputChannels->debugFloatField5 = (engineConfiguration->clt.adcChannel != EFI_ADC_NONE) ? getVoltageDivided("clt", engineConfiguration->clt.adcChannel) : 0.0f;
		tsOutputChannels->debugFloatField6 = (engineConfiguration->iat.adcChannel != EFI_ADC_NONE) ? getVoltageDivided("iat", engineConfiguration->iat.adcChannel) : 0.0f;
		tsOutputChannels->debugFloatField7 = (engineConfiguration->afr.hwChannel != EFI_ADC_NONE) ? getVoltageDivided("ego", engineConfiguration->afr.hwChannel) : 0.0f;
		break;
	case DBG_INSTANT_RPM:
		{
			float instantRpm = engine->triggerCentral.triggerState.instantRpm;
			tsOutputChannels->debugFloatField1 = instantRpm;
			tsOutputChannels->debugFloatField2 = instantRpm / engine->rpmCalculator.rpmValue;
		}
		break;
	default:
		;
	}

	tsOutputChannels->wallFuelAmount = ENGINE(wallFuel).getWallFuel(0);
	tsOutputChannels->wallFuelCorrection = ENGINE(wallFuelCorrection);
	// TPS acceleration
	tsOutputChannels->deltaTps = engine->tpsAccelEnrichment.getMaxDelta();
	tsOutputChannels->tpsAccelFuel = engine->engineState.tpsAccelEnrich;
	// engine load acceleration
	if (hasMapSensor(PASS_ENGINE_PARAMETER_SIGNATURE)) {
		tsOutputChannels->engineLoadAccelExtra = engine->engineLoadAccelEnrichment.getEngineLoadEnrichment(PASS_ENGINE_PARAMETER_SIGNATURE) * 100 / getMap();
	}
	tsOutputChannels->engineLoadDelta = engine->engineLoadAccelEnrichment.getMaxDelta();


	tsOutputChannels->iatCorrection = ENGINE(engineState.iatFuelCorrection);
	tsOutputChannels->cltCorrection = ENGINE(engineState.cltFuelCorrection);

	tsOutputChannels->checkEngine = hasErrorCodes();

	tsOutputChannels->vvtPosition = engine->triggerCentral.vvtPosition;

	tsOutputChannels->engineMode = packEngineMode(PASS_ENGINE_PARAMETER_SIGNATURE);

#if EFI_PROD_CODE || defined(__DOXYGEN__)
	tsOutputChannels->internalMcuTemperature = getMCUInternalTemperature();
	tsOutputChannels->idlePosition = getIdlePosition();
	tsOutputChannels->isTriggerError = isTriggerErrorNow();

#if EFI_MAX_31855 || defined(__DOXYGEN__)
	for (int i = 0; i < EGT_CHANNEL_COUNT; i++)
		tsOutputChannels->egtValues.values[i] = getEgtValue(i);
#endif /* EFI_MAX_31855 */

#if EFI_INTERNAL_FLASH || defined(__DOXYGEN__)
	tsOutputChannels->needBurn = getNeedToWriteConfiguration();
#endif /* EFI_INTERNAL_FLASH */

#if EFI_FILE_LOGGING || defined(__DOXYGEN__)
	tsOutputChannels->hasSdCard = isSdCardAlive();
#endif /* EFI_FILE_LOGGING */

	tsOutputChannels->isFuelPumpOn = enginePins.fuelPumpRelay.getLogicValue();
	tsOutputChannels->isFanOn = enginePins.fanRelay.getLogicValue();
	tsOutputChannels->isO2HeaterOn = enginePins.o2heater.getLogicValue();
	tsOutputChannels->isIgnitionEnabled = engineConfiguration->isIgnitionEnabled;
	tsOutputChannels->isInjectionEnabled = engineConfiguration->isInjectionEnabled;
	tsOutputChannels->isCylinderCleanupEnabled = engineConfiguration->isCylinderCleanupEnabled;
	tsOutputChannels->isCylinderCleanupActivated = engine->isCylinderCleanupMode;
	tsOutputChannels->secondTriggerChannelEnabled = engineConfiguration->secondTriggerChannelEnabled;
#if EFI_VEHICLE_SPEED || defined(__DOXYGEN__)
	float vehicleSpeed = getVehicleSpeed();
	tsOutputChannels->vehicleSpeedKph = vehicleSpeed;
	tsOutputChannels->speedToRpmRatio = vehicleSpeed / rpm;

#endif /* EFI_VEHICLE_SPEED */
	tsOutputChannels->isCltError = !isValidCoolantTemperature(getCoolantTemperature(PASS_ENGINE_PARAMETER_SIGNATURE));
	tsOutputChannels->isIatError = !isValidIntakeAirTemperature(getIntakeAirTemperature(PASS_ENGINE_PARAMETER_SIGNATURE));
#endif /* EFI_PROD_CODE */

	tsOutputChannels->fuelConsumptionPerHour = engine->engineState.fuelConsumption.perSecondConsumption;
	
	tsOutputChannels->warningCounter = engine->engineState.warningCounter;
	tsOutputChannels->lastErrorCode = engine->engineState.lastErrorCode;

	tsOutputChannels->knockNowIndicator = engine->knockCount > 0;
	tsOutputChannels->knockEverIndicator = engine->knockEver;

	tsOutputChannels->clutchUpState = engine->clutchUpState;
	tsOutputChannels->clutchDownState = engine->clutchDownState;
	tsOutputChannels->brakePedalState = engine->brakePedalState;

	tsOutputChannels->tCharge = getTCharge(rpm, tps, coolant, intake PASS_ENGINE_PARAMETER_SUFFIX);
	float timing = engine->engineState.timingAdvance;
	tsOutputChannels->ignitionAdvance = timing > 360 ? timing - 720 : timing;
	tsOutputChannels->sparkDwell = ENGINE(engineState.sparkDwell);
	tsOutputChannels->crankingFuelMs = engine->isCylinderCleanupMode ? 0 : getCrankingFuel(PASS_ENGINE_PARAMETER_SIGNATURE);
	tsOutputChannels->chargeAirMass = engine->engineState.airMass;
}

extern TunerStudioOutputChannels tsOutputChannels;

void prepareTunerStudioOutputs(void) {
	// sensor state for EFI Analytics Tuner Studio
	updateTunerStudioState(&tsOutputChannels PASS_ENGINE_PARAMETER_SUFFIX);
}

#endif /* EFI_TUNER_STUDIO */

void initStatusLoop(void) {
	setFullLog(INITIAL_FULL_LOG);
	addConsoleActionI(FULL_LOGGING_KEY, setFullLog);
	addConsoleActionI("warn", setWarningEnabled);

#if EFI_ENGINE_CONTROL || defined(__DOXYGEN__)
	addConsoleActionFF("fuelinfo2", (VoidFloatFloat) showFuelInfo2);
	addConsoleAction("fuelinfo", showFuelInfo);
#endif

#if EFI_PROD_CODE || defined(__DOXYGEN__)

	addConsoleActionI("set_led_blinking_period", setBlinkingPeriod);

	addConsoleAction("status", printStatus);
#endif /* EFI_PROD_CODE */
}

void startStatusThreads(void) {
	// todo: refactoring needed, this file should probably be split into pieces
	chThdCreateStatic(lcdThreadStack, sizeof(lcdThreadStack), NORMALPRIO, (tfunc_t) lcdThread, NULL);
#if EFI_PROD_CODE || defined(__DOXYGEN__)
	initStatusLeds();
	chThdCreateStatic(blinkingStack, sizeof(blinkingStack), NORMALPRIO, (tfunc_t) blinkingThread, NULL);
#endif /* EFI_PROD_CODE */
}

void setFullLog(int value) {
	print("Setting full logging: %s\r\n", boolToString(value));
	printMsg(&logger, "%s%d", FULL_LOGGING_KEY, value);
	fullLog = value;
}

bool getFullLog(void) {
	return fullLog;
}
