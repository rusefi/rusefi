/**
 * @file	status_loop.cpp
 * @brief Human-readable protocol status messages
 *
 * http://rusefi.com/forum/viewtopic.php?t=263 Dev console overview
 * http://rusefi.com/forum/viewtopic.php?t=210 Commands overview
 *
 *
 * @date Mar 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
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
#include "rusefi_outputs.h"

extern fuel_Map3D_t veMap;
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
#endif

static bool subscription[(int) RO_LAST_ELEMENT];

// this 'true' value is needed for simulator
static volatile bool fullLog = true;
int warningEnabled = true;
//int warningEnabled = FALSE;

#if EFI_TUNER_STUDIO || defined(__DOXYGEN__)
extern TunerStudioOutputChannels tsOutputChannels;
#endif

extern bool hasFirmwareErrorFlag;
#define FULL_LOGGING_KEY "fl"

static char LOGGING_BUFFER[1400] CCM_OPTIONAL;
static Logging logger("status loop", LOGGING_BUFFER, sizeof(LOGGING_BUFFER));

static void setWarningEnabled(int value) {
	warningEnabled = value;
}

#if EFI_FILE_LOGGING || defined(__DOXYGEN__)
static char FILE_LOGGER[1000] CCM_OPTIONAL;
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
static int packEngineMode(DECLARE_ENGINE_PARAMETER_F) {
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

//	reportSensorF(log, fileFormat, "TRG_0_DUTY", "%", getTriggerDutyCycle(0), 2);
//	reportSensorF(log, fileFormat, "TRG_1_DUTY", "%", getTriggerDutyCycle(1), 2);
#endif

#if EFI_PROD_CODE || defined(__DOXYGEN__)
	reportSensorF(log, fileFormat, "int_temp", "C", getMCUInternalTemperature(), 2); // log column #3
#endif

	reportSensorI(log, fileFormat, "mode", "v", packEngineMode(PASS_ENGINE_PARAMETER_F)); // log column #3

	if (hasCltSensor()) {
		reportSensorF(log, fileFormat, "CLT", "C", getCoolantTemperature(PASS_ENGINE_PARAMETER_F), 2); // log column #4
	}
	if (hasTpsSensor()) {
		reportSensorF(log, fileFormat, "TPS", "%", getTPS(PASS_ENGINE_PARAMETER_F), 2); // log column #5
	}

	if (hasVBatt(PASS_ENGINE_PARAMETER_F)) {
		reportSensorF(log, fileFormat, "vbatt", "V", getVBatt(PASS_ENGINE_PARAMETER_F), 2); // log column #6
	}

	if (hasIatSensor()) {
		reportSensorF(log, fileFormat, "IAT", "C", getIntakeAirTemperature(PASS_ENGINE_PARAMETER_F), 2); // log column #7
	}

	if (hasMafSensor()) {
		reportSensorF(log, fileFormat, "maf", "V", getMaf(PASS_ENGINE_PARAMETER_F), 2);
		reportSensorF(log, fileFormat, "mafr", "kg/hr", getRealMaf(PASS_ENGINE_PARAMETER_F), 2);
	}
#if EFI_ANALOG_SENSORS || defined(__DOXYGEN__)
	if (hasMapSensor(PASS_ENGINE_PARAMETER_F)) {
		reportSensorF(log, fileFormat, "MAP", "kPa", getMap(), 2);
//		reportSensorF(log, fileFormat, "map_r", "V", getRawMap(), 2);
	}
#endif /* EFI_ANALOG_SENSORS */
#if EFI_ANALOG_SENSORS || defined(__DOXYGEN__)
	if (hasBaroSensor()) {
		reportSensorF(log, fileFormat, "baro", "kPa", getBaroPressure(), 2);
	}
#endif /* EFI_ANALOG_SENSORS */

	if (hasAfrSensor(PASS_ENGINE_PARAMETER_F)) {
		reportSensorF(log, fileFormat, "afr", "AFR", getAfr(PASS_ENGINE_PARAMETER_F), 2);
	}

#if EFI_IDLE_CONTROL || defined(__DOXYGEN__)
	if (fileFormat) {
		reportSensorF(log, fileFormat, "idle", "%", getIdlePosition(), 2);
	}
#endif /* EFI_IDLE_CONTROL */

#if EFI_ANALOG_SENSORS || defined(__DOXYGEN__)
	reportSensorF(log, fileFormat, "target", "AFR", engine->engineState.targetAFR, 2);
#endif /* EFI_ANALOG_SENSORS */

	if (fileFormat) {

#if EFI_TUNER_STUDIO || defined(__DOXYGEN__)
		reportSensorF(log, fileFormat, "debugF1", "v", tsOutputChannels.debugFloatField1, 4);
		reportSensorF(log, fileFormat, "debugF2", "v", tsOutputChannels.debugFloatField2, 4);
		reportSensorF(log, fileFormat, "debugF3", "v", tsOutputChannels.debugFloatField3, 4);
		reportSensorF(log, fileFormat, "debugF4", "v", tsOutputChannels.debugFloatField4, 4);
		reportSensorF(log, fileFormat, "debugF5", "v", tsOutputChannels.debugFloatField5, 4);
		reportSensorF(log, fileFormat, "debugF6", "v", tsOutputChannels.debugFloatField6, 4);
		reportSensorF(log, fileFormat, "debugF7", "v", tsOutputChannels.debugFloatField7, 4);

		reportSensorI(log, fileFormat, "debugInt1", "v", tsOutputChannels.debugIntField1);
		reportSensorI(log, fileFormat, "debugInt2", "v", tsOutputChannels.debugIntField2);
		reportSensorI(log, fileFormat, "debugInt3", "v", tsOutputChannels.debugIntField3);
#endif /* EFI_TUNER_STUDIO */

		reportSensorF(log, fileFormat, "tCharge", "K", engine->engineState.tChargeK, 2); // log column #8
		if (hasMapSensor(PASS_ENGINE_PARAMETER_F)) {
			reportSensorF(log, fileFormat, "curVE", "%", veMap.getValue(rpm, getMap()), 2);
		}
		reportSensorF(log, fileFormat, "VVT", "deg", engine->triggerCentral.vvtPosition, 1);
	}

	float engineLoad = getEngineLoadT(PASS_ENGINE_PARAMETER_F);
	reportSensorF(log, fileFormat, "ENGINE_LOAD", "x", engineLoad, 2);


	reportSensorF(log, fileFormat, "dwell", "ms", ENGINE(engineState.sparkDwell), 2);
	if (fileFormat) {
		reportSensorF(log, fileFormat, "timing", "deg", engine->engineState.timingAdvance, 2);

	}

	if (fileFormat) {
		floatms_t fuelBase = getBaseFuel(rpm PASS_ENGINE_PARAMETER);
		reportSensorF(log, fileFormat, "f: base", "ms", fuelBase, 2);
		reportSensorF(log, fileFormat, "f: actual", "ms", ENGINE(actualLastInjection), 2);
		reportSensorF(log, fileFormat, "f: lag", "ms", engine->engineState.injectorLag, 2);
		reportSensorF(log, fileFormat, "f: running", "ms", ENGINE(engineState.runningFuel), 2);
		reportSensorF(log, fileFormat, "f: pid", "ms", ENGINE(engineState.fuelPidCorrection), 2);

		reportSensorF(log, fileFormat, "f: wall amt", "v", ENGINE(wallFuel).getWallFuel(0), 2);
		reportSensorF(log, fileFormat, "f: wall crr", "v", ENGINE(wallFuelCorrection), 2);

		reportSensorI(log, fileFormat, "version", "#", getRusEfiVersion());
	}

#if EFI_VEHICLE_SPEED || defined(__DOXYGEN__)
	if (hasVehicleSpeedSensor()) {
		float vehicleSpeed = getVehicleSpeed();
		reportSensorF(log, fileFormat, "vss", "kph", vehicleSpeed, 2);
		float sp2rpm = rpm == 0 ? 0 : vehicleSpeed / rpm;
		reportSensorF(log, fileFormat, "sp2rpm", "x", sp2rpm, 2);
	}
#endif /* EFI_PROD_CODE */

	reportSensorF(log, fileFormat, "knck_c", "count", engine->knockCount, 0);
	reportSensorF(log, fileFormat, "knck_v", "v", engine->knockVolts, 2);

//	reportSensorF(log, fileFormat, "vref", "V", getVRef(engineConfiguration), 2);

	if (fileFormat) {
		reportSensorF(log, fileFormat, "f: tps delta", "v", engine->tpsAccelEnrichment.getMaxDelta(), 2);
		reportSensorF(log, fileFormat, "f: tps fuel", "ms", engine->engineState.tpsAccelEnrich, 2);

		reportSensorF(log, fileFormat, "f: el delta", "v", engine->engineLoadAccelEnrichment.getMaxDelta(), 2);
		if (hasMapSensor(PASS_ENGINE_PARAMETER_F)) {
			reportSensorF(log, fileFormat, "f: el fuel", "v", engine->engineLoadAccelEnrichment.getEngineLoadEnrichment(PASS_ENGINE_PARAMETER_F) * 100 / getMap(), 2);
		}

		reportSensorF(log, fileFormat, "f: duty", "%", getInjectorDutyCycle(rpm PASS_ENGINE_PARAMETER), 2);
		reportSensorF(log, fileFormat, "dwell: duty", "%", getCoilDutyCycle(rpm PASS_ENGINE_PARAMETER), 2);
	}


//	debugFloat(&logger, "tch", getTCharge1(tps), 2);

	for (int i = 0;i<FSIO_ADC_COUNT;i++) {
		if (engineConfiguration->fsioAdc[i] != EFI_ADC_NONE) {
			strcpy(buf, "adcX");
			buf[3] = '0' + i;
			reportSensorF(log, fileFormat, buf, "", getVoltage("fsio", engineConfiguration->fsioAdc[i]), 2);
		}
	}

	reportSensorI(log, fileFormat, "warn", "count", engine->engineState.warningCounter);
	reportSensorI(log, fileFormat, "error", "code", engine->engineState.lastErrorCode);

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

static void printState(void) {
#if EFI_SHAFT_POSITION_INPUT || defined(__DOXYGEN__)

	// todo: make SWO work
//	char *msg = "hello\r\n";
//	for(int i=0;i<strlen(msg);i++) {
//		ITM_SendChar(msg[i]);
//	}

	int rpm = getRpmE(engine);
	if (subscription[(int) RO_TOTAL_REVOLUTION_COUNTER])
		debugInt(&logger, "ckp_c", getCrankEventCounter());
	if (subscription[(int) RO_RUNNING_REVOLUTION_COUNTER])
		debugInt(&logger, "ckp_r", engine->triggerCentral.triggerState.runningRevolutionCounter);

	if (subscription[(int) RO_RUNNING_TRIGGER_ERROR])
		debugInt(&logger, "trg_r_errors", engine->triggerCentral.triggerState.runningTriggerErrorCounter);

	if (subscription[(int) RO_RUNNING_ORDERING_TRIGGER_ERROR])
		debugInt(&logger, "trg_r_order_errors", engine->triggerCentral.triggerState.runningOrderingErrorCounter);

	if (subscription[(int) RO_WAVE_CHART_CURRENT_SIZE])
		debugInt(&logger, "wave_chart_current", 0);

//	debugInt(&logger, "idl", getIdleSwitch());

//	debugFloat(&logger, "table_spark", getAdvance(rpm, getMaf()), 2);

//		float map = getMap();

#endif /* EFI_SHAFT_POSITION_INPUT */
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

static void printInfo(systime_t nowSeconds) {
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
#endif /* EFI_PROD_CODE */

}

static systime_t timeOfPreviousReport = (systime_t) -1;

extern fatal_msg_t errorMessageBuffer;

/**
 * @brief Sends all pending data to dev console
 */
void updateDevConsoleState(Engine *engine) {
	if (!isCommandLineConsoleReady()) {
		return;
	}
// looks like this is not needed anymore
//	checkIfShouldHalt();
	printPending();

	/**
	 * this should go before the firmware error so that console can detect connection
	 */
	printSensors(&logger, false);

#if EFI_PROD_CODE || defined(__DOXYGEN__)
	// todo: unify with simulator!
	if (hasFirmwareError()) {
		scheduleMsg(&logger, "FATAL error: %s", errorMessageBuffer);
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
	printInfo(nowSeconds);

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

	printState();

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

	float baseFuelMs = getBaseTableFuel(engineConfiguration, (int) rpm, engineLoad);

	float magicAir = getAirMass(engineConfiguration, 1, 100, convertCelsiusToKelvin(20));

	scheduleMsg(&logger, "SD magic fuel %f", sdMath(engineConfiguration, magicAir, 14.7));
	scheduleMsg(&logger, "inj flow %fcc/min displacement %fL", engineConfiguration->injector.flow,
			engineConfiguration->specs.displacement);

	scheduleMsg(&logger2, "algo=%s/pump=%s", getEngine_load_mode_e(engineConfiguration->fuelAlgorithm),
			boolToString(enginePins.fuelPumpRelay.getLogicValue()));

	scheduleMsg(&logger2, "injection phase=%f/global fuel correction=%f", getinjectionOffset(rpm), engineConfiguration->globalFuelCorrection);

	scheduleMsg(&logger2, "baro correction=%f", engine->engineState.baroCorrection);

#if EFI_ENGINE_CONTROL || defined(__DOXYGEN__)
	scheduleMsg(&logger, "base cranking fuel %f", engineConfiguration->cranking.baseFuel);
	scheduleMsg(&logger2, "cranking fuel: %f", getCrankingFuel(PASS_ENGINE_PARAMETER_F));

	if (engine->rpmCalculator.isRunning(PASS_ENGINE_PARAMETER_F)) {
		float iatCorrection = engine->engineState.iatFuelCorrection;
		float cltCorrection = engine->engineState.cltFuelCorrection;
		floatms_t injectorLag = engine->engineState.injectorLag;
		scheduleMsg(&logger2, "rpm=%f engineLoad=%f", rpm, engineLoad);
		scheduleMsg(&logger2, "baseFuel=%f", baseFuelMs);

		scheduleMsg(&logger2, "iatCorrection=%f cltCorrection=%f injectorLag=%f", iatCorrection, cltCorrection,
				injectorLag);

		float value = getRunningFuel(baseFuelMs, (int) rpm PASS_ENGINE_PARAMETER);
		scheduleMsg(&logger2, "injection pulse width: %f", value);
	}
#endif
}

#if EFI_ENGINE_CONTROL || defined(__DOXYGEN__)
static void showFuelInfo(void) {
	showFuelInfo2((float) getRpmE(engine), getEngineLoadT(PASS_ENGINE_PARAMETER_F));
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

extern pin_output_mode_e DEFAULT_OUTPUT;

static void initStatusLeds(void) {
#if EFI_PROD_CODE || defined(__DOXYGEN__)
	outputPinRegisterExt2("led: comm status", &enginePins.communicationPin,
			engineConfiguration->communicationPin, &DEFAULT_OUTPUT);
	// we initialize this here so that we can blink it on start-up
	outputPinRegisterExt2("MalfunctionIndicator", &enginePins.checkEnginePin, boardConfiguration->malfunctionIndicatorPin, &DEFAULT_OUTPUT);
#endif

#if EFI_WARNING_LED || defined(__DOXYGEN__)
	outputPinRegister("led: warning status", &enginePins.warningPin, LED_WARNING_PORT,
	LED_WARNING_PIN);
	outputPinRegisterExt2("led: running status", &enginePins.runningPin, engineConfiguration->runningPin,
			&DEFAULT_OUTPUT);
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
			updateHD44780lcd(engine);
#endif
		}
		chThdSleepMilliseconds(engineConfiguration->bc.lcdThreadPeriod);
	}
}

#if EFI_TUNER_STUDIO || defined(__DOXYGEN__)

void updateTunerStudioState(TunerStudioOutputChannels *tsOutputChannels DECLARE_ENGINE_PARAMETER_S) {
#if EFI_SHAFT_POSITION_INPUT || defined(__DOXYGEN__)
	int rpm = getRpmE(engine);
#else
	int rpm = 0;
#endif

	float tps = getTPS(PASS_ENGINE_PARAMETER_F);
	float coolant = getCoolantTemperature(PASS_ENGINE_PARAMETER_F);
	float intake = getIntakeAirTemperature(PASS_ENGINE_PARAMETER_F);

	float engineLoad = getEngineLoadT(PASS_ENGINE_PARAMETER_F);

	// header
	tsOutputChannels->tsConfigVersion = TS_FILE_VERSION;

	// engine state
	tsOutputChannels->rpm = rpm;
	tsOutputChannels->coolantTemperature = coolant;
	tsOutputChannels->intakeAirTemperature = intake;
	tsOutputChannels->throttlePositon = tps;
	tsOutputChannels->massAirFlowVoltage = hasMafSensor() ? getMaf(PASS_ENGINE_PARAMETER_F) : 0;
    tsOutputChannels->massAirFlow = hasMafSensor() ? getRealMaf(PASS_ENGINE_PARAMETER_F) : 0;
          
	if (hasMapSensor(PASS_ENGINE_PARAMETER_F)) {
		float mapValue = getMap();
		tsOutputChannels->veValue = veMap.getValue(rpm, mapValue);
		// todo: bug here? target afr could work based on real MAF?
		tsOutputChannels->currentTargetAfr = afrMap.getValue(rpm, mapValue);
		tsOutputChannels->manifoldAirPressure = mapValue;
	}
	tsOutputChannels->airFuelRatio = getAfr(PASS_ENGINE_PARAMETER_F);
	if (hasVBatt(PASS_ENGINE_PARAMETER_F)) {
		tsOutputChannels->vBatt = getVBatt(PASS_ENGINE_PARAMETER_F);
	}
	tsOutputChannels->tpsADC = getTPS12bitAdc(PASS_ENGINE_PARAMETER_F) / TPS_TS_CONVERSION;
#if EFI_ANALOG_SENSORS || defined(__DOXYGEN__)
	tsOutputChannels->baroPressure = hasBaroSensor() ? getBaroPressure() : 0;
#endif /* EFI_ANALOG_SENSORS */
	tsOutputChannels->engineLoad = engineLoad;
	tsOutputChannels->rpmAcceleration = engine->rpmCalculator.getRpmAcceleration();
	tsOutputChannels->triggerErrorsCounter = engine->triggerCentral.triggerState.totalTriggerErrorCounter;
	tsOutputChannels->baroCorrection = engine->engineState.baroCorrection;
	tsOutputChannels->pedalPosition = hasPedalPositionSensor(PASS_ENGINE_PARAMETER_F) ? getPedalPosition(PASS_ENGINE_PARAMETER_F) : 0;
	tsOutputChannels->knockCount = engine->knockCount;
	tsOutputChannels->knockLevel = engine->knockVolts;
	tsOutputChannels->fuelTankGauge = engine->sensors.fuelTankGauge;

	tsOutputChannels->hasFatalError = hasFirmwareError();
	tsOutputChannels->totalTriggerErrorCounter = engine->triggerCentral.triggerState.totalTriggerErrorCounter;

	tsOutputChannels->injectorDutyCycle = getInjectorDutyCycle(rpm PASS_ENGINE_PARAMETER);
	tsOutputChannels->fuelRunning = ENGINE(engineState.runningFuel);
	tsOutputChannels->fuelPidCorrection = ENGINE(engineState.fuelPidCorrection);
	tsOutputChannels->injectorLagMs = ENGINE(engineState.injectorLag);
	tsOutputChannels->fuelBase = engine->engineState.baseFuel;
	tsOutputChannels->actualLastInjection = ENGINE(actualLastInjection);

	tsOutputChannels->coilDutyCycle = getCoilDutyCycle(rpm PASS_ENGINE_PARAMETER);

	efitimesec_t now = getTimeNowSeconds();
	tsOutputChannels->timeSeconds = now;
	tsOutputChannels->firmwareVersion = getRusEfiVersion();

	tsOutputChannels->isWarnNow = isWarningNow(now, true);

	if (engineConfiguration->debugMode == DBG_TPS_ACCEL) {
		tsOutputChannels->debugIntField1 = engine->tpsAccelEnrichment.cb.getSize();
	}

	if (engineConfiguration->debugMode == DBG_TRIGGER_INPUT) {
		tsOutputChannels->debugIntField1 = engine->triggerCentral.getHwEventCounter((int)SHAFT_PRIMARY_FALLING);
		tsOutputChannels->debugIntField2 = engine->triggerCentral.getHwEventCounter((int)SHAFT_SECONDARY_FALLING);
		tsOutputChannels->debugIntField3 = engine->triggerCentral.getHwEventCounter((int)SHAFT_3RD_FALLING);

		tsOutputChannels->debugFloatField1 = engine->triggerCentral.getHwEventCounter((int)SHAFT_PRIMARY_RISING);
		tsOutputChannels->debugFloatField2 = engine->triggerCentral.getHwEventCounter((int)SHAFT_SECONDARY_RISING);
		tsOutputChannels->debugFloatField3 = engine->triggerCentral.getHwEventCounter((int)SHAFT_3RD_RISING);

	} else if (engineConfiguration->debugMode == FSIO_ADC) {
		// todo: implement a proper loop
		if (engineConfiguration->fsioAdc[0] != EFI_ADC_NONE) {
			strcpy(buf, "adcX");
			tsOutputChannels->debugFloatField1 = getVoltage("fsio", engineConfiguration->fsioAdc[0]);
		}
	} else if (engineConfiguration->debugMode == DBG_VEHICLE_SPEED_SENSOR) {
		tsOutputChannels->debugIntField1 = engine->engineState.vssCounter;
	} else if (engineConfiguration->debugMode == DBG_SD_CARD) {
		tsOutputChannels->debugIntField1 = engine->engineState.totalLoggedBytes;
	} else if (engineConfiguration->debugMode == DBG_CRANKING_DETAILS) {
		tsOutputChannels->debugIntField1 = engine->rpmCalculator.getRevolutionCounterSinceStart();
	}


	tsOutputChannels->wallFuelAmount = ENGINE(wallFuel).getWallFuel(0);
	tsOutputChannels->wallFuelCorrection = ENGINE(wallFuelCorrection);
	// TPS acceleration
	tsOutputChannels->deltaTps = engine->tpsAccelEnrichment.getMaxDelta();
	tsOutputChannels->tpsAccelFuel = engine->engineState.tpsAccelEnrich;
	// engine load acceleration
	if (hasMapSensor(PASS_ENGINE_PARAMETER_F)) {
		tsOutputChannels->engineLoadAccelExtra = engine->engineLoadAccelEnrichment.getEngineLoadEnrichment(PASS_ENGINE_PARAMETER_F) * 100 / getMap();
	}
	tsOutputChannels->engineLoadDelta = engine->engineLoadAccelEnrichment.getMaxDelta();


	tsOutputChannels->iatCorrection = ENGINE(engineState.iatFuelCorrection);
	tsOutputChannels->cltCorrection = ENGINE(engineState.cltFuelCorrection);

	tsOutputChannels->checkEngine = hasErrorCodes();

	tsOutputChannels->vvtPosition = engine->triggerCentral.vvtPosition;

	tsOutputChannels->engineMode = packEngineMode(PASS_ENGINE_PARAMETER_F);

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
#endif
#if EFI_FILE_LOGGING || defined(__DOXYGEN__)
	tsOutputChannels->hasSdCard = isSdCardAlive();
#endif
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
	tsOutputChannels->isCltError = !isValidCoolantTemperature(getCoolantTemperature(PASS_ENGINE_PARAMETER_F));
	tsOutputChannels->isIatError = !isValidIntakeAirTemperature(getIntakeAirTemperature(PASS_ENGINE_PARAMETER_F));
#endif /* EFI_PROD_CODE */

	tsOutputChannels->warningCounter = engine->engineState.warningCounter;
	tsOutputChannels->lastErrorCode = engine->engineState.lastErrorCode;

	tsOutputChannels->knockNowIndicator = engine->knockCount > 0;
	tsOutputChannels->knockEverIndicator = engine->knockEver;

	tsOutputChannels->clutchUpState = engine->clutchUpState;
	tsOutputChannels->clutchDownState = engine->clutchDownState;
	tsOutputChannels->tCharge = getTCharge(rpm, tps, coolant, intake PASS_ENGINE_PARAMETER);
	float timing = engine->engineState.timingAdvance;
	tsOutputChannels->ignitionAdvance = timing > 360 ? timing - 720 : timing;
	tsOutputChannels->sparkDwell = ENGINE(engineState.sparkDwell);
	tsOutputChannels->crankingFuelMs = getCrankingFuel(PASS_ENGINE_PARAMETER_F);
	tsOutputChannels->chargeAirMass = engine->engineState.airMass;
}

extern TunerStudioOutputChannels tsOutputChannels;

void prepareTunerStudioOutputs(void) {
	// sensor state for EFI Analytics Tuner Studio
	updateTunerStudioState(&tsOutputChannels PASS_ENGINE_PARAMETER);
}

#endif /* EFI_TUNER_STUDIO */

static void subscribe(int outputOrdinal) {
	subscription[outputOrdinal] = true;
}

static void unsubscribe(int outputOrdinal) {
	subscription[outputOrdinal] = false;
}

void initStatusLoop(Engine *engine) {
	setFullLog(INITIAL_FULL_LOG);
	addConsoleActionI(FULL_LOGGING_KEY, setFullLog);
	addConsoleActionI("warn", setWarningEnabled);

#if EFI_ENGINE_CONTROL || defined(__DOXYGEN__)
	addConsoleActionFF("fuelinfo2", (VoidFloatFloat) showFuelInfo2);
	addConsoleAction("fuelinfo", showFuelInfo);
#endif

#if EFI_PROD_CODE || defined(__DOXYGEN__)
	subscription[(int) RO_TRG1_DUTY] = true;
	subscription[(int) RO_TRG2_DUTY] = true;
	subscription[(int) RO_TRG3_DUTY] = false;
	subscription[(int) RO_TRG4_DUTY] = false;
	subscription[(int) RO_TOTAL_REVOLUTION_COUNTER] = true;
	subscription[(int) RO_RUNNING_REVOLUTION_COUNTER] = false;

	addConsoleActionI("subscribe", subscribe);
	addConsoleActionI("unsubscribe", unsubscribe);
	addConsoleActionI("set_led_blinking_period", setBlinkingPeriod);

	addConsoleAction("status", printStatus);
#endif /* EFI_PROD_CODE */
}

void startStatusThreads(Engine *engine) {
	// todo: refactoring needed, this file should probably be split into pieces
	chThdCreateStatic(lcdThreadStack, sizeof(lcdThreadStack), NORMALPRIO, (tfunc_t) lcdThread, engine);
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
