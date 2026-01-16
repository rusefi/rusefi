/**
 * @file	status_loop.cpp
 * @brief Human-readable protocol status messages
 *
 * http://rusefi.com/forum/viewtopic.php?t=263 rusEfi console overview
 * http://rusefi.com/forum/viewtopic.php?t=210 Commands overview
 *
 *
 * @date Mar 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
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

#include "pch.h"
#include "status_loop.h"
#include "electronic_throttle.h"

#if EFI_LOGIC_ANALYZER
#include "logic_analyzer.h"
#endif /* EFI_LOGIC_ANALYZER */

#include "trigger_central.h"
#include "sensor_reader.h"
#include "mmc_card.h"
#include "console_io.h"
#include "malfunction_central.h"
#include "speed_density.h"

#include "tunerstudio.h"
#include "tunerstudio_calibration_channel.h"
#include "fuel_math.h"
#include "main_trigger_callback.h"
#include "spark_logic.h"
#include "idle_thread.h"
#include "can_hw.h"
#include "periodic_thread_controller.h"
#include "cdm_ion_sense.h"
#include "binary_mlg_logging.h"
#include "buffered_writer.h"
#include "dynoview.h"
#include "frequency_sensor.h"
#include "digital_input_exti.h"
#include "dc_motors.h"

#if EFI_PROD_CODE
// todo: move this logic to algo folder!
#include "rtc_helper.h"
#include "rusefi.h"
#include "pin_repository.h"
#include "max3185x.h"
#include "single_timer_executor.h"
#include "periodic_task.h"
#endif /* EFI_PROD_CODE */

#if EFI_CONFIGURATION_STORAGE
#include "storage.h"
#include "flash_main.h"
#endif

#if EFI_MAP_AVERAGING && defined (MODULE_MAP_AVERAGING)
#include "map_averaging.h"
#endif

#if (BOARD_TLE8888_COUNT > 0)
#include "tle8888.h"
#endif /* BOARD_TLE8888_COUNT */

#if EFI_ENGINE_SNIFFER
#include "engine_sniffer.h"
extern WaveChart waveChart;
#endif /* EFI_ENGINE_SNIFFER */

extern int maxTriggerReentrant;
extern uint32_t maxLockedDuration;

/**
 * Time when the firmware version was last reported
 * TODO: implement a request/response instead of just constantly sending this out
 */
static Timer printVersionTimer;

// todo: unify with handleGetVersion?
static void printVersionForConsole(const char *engineTypeName, const char *firmwareBuildId) {
	// VersionChecker in rusEFI console is parsing these version string, please follow the expected format
	efiPrintfProto(PROTOCOL_VERSION_TAG, "%d@%u %s %s %ld",
			getRusEfiVersion(), /*do we have a working way to print 64 bit values?!*/(unsigned int)SIGNATURE_HASH,
			firmwareBuildId,
			engineTypeName,
			(uint32_t)getTimeNowS());
}

#if EFI_PROD_CODE
// Inform the console about the mapping between a pin's logical name (for example, injector 3)
// and the physical MCU pin backing that function (for example, PE3)
static void printOutPin(const char *pinName, brain_pin_e hwPin) {
	if (hwPin == Gpio::Unassigned || hwPin == Gpio::Invalid) {
		return;
	}
	const char *hwPinName;
	if (isBrainPinValid(hwPin)) {
		hwPinName = hwPortname(hwPin);
	} else {
		hwPinName = "smart";
	}

	efiPrintfProto(PROTOCOL_OUTPIN, "%s@%s", pinName, hwPinName);
}
#endif // EFI_PROD_CODE

// Print out the current mapping between logical and physical pins that
// the engine sniffer cares about, so we can display a physical pin
// in each engine sniffer row
static void printEngineSnifferPinMappings() {
#if EFI_PROD_CODE
	printOutPin(PROTOCOL_CRANK1, engineConfiguration->triggerInputPins[0]);
	printOutPin(PROTOCOL_CRANK2, engineConfiguration->triggerInputPins[1]);
	for (int i = 0;i<CAM_INPUTS_COUNT;i++) {
		extern const char *vvtNames[];
		printOutPin(vvtNames[i], engineConfiguration->camInputs[i]);
	}

	int cylCount = minI(engineConfiguration->cylindersCount, MAX_CYLINDER_COUNT);
	for (int i = 0; i < cylCount; i++) {
		printOutPin(enginePins.coils[i].getShortName(), engineConfiguration->ignitionPins[i]);
		printOutPin(enginePins.trailingCoils[i].getShortName(), engineConfiguration->trailingCoilPins[i]);
		printOutPin(enginePins.injectors[i].getShortName(), engineConfiguration->injectionPins[i]);
		printOutPin(enginePins.injectorsStage2[i].getShortName(), engineConfiguration->injectionPinsStage2[i]);
	}
#endif /* EFI_PROD_CODE */
}

void printOverallStatus() {
#if EFI_ENGINE_SNIFFER
	waveChart.publishIfFull();
#endif /* EFI_ENGINE_SNIFFER */


	/**
	 * we report the version every second - this way the console does not need to
	 * request it and we will display it pretty soon
	 */
	if (printVersionTimer.hasElapsedSec(1)) {
		// we're sending, reset the timer
		printVersionTimer.reset();

		// Output the firmware version, board type, git hash, uptime in seconds, etc for rusEFI console
		printVersionForConsole(getEngine_type_e(engineConfiguration->engineType), FIRMWARE_ID);

		// Output the current engine sniffer pin mappings
		printEngineSnifferPinMappings();
	}
}

#if !defined(LOGIC_ANALYZER_BUFFER_SIZE)
// TODO: how small can this be?
#define LOGIC_ANALYZER_BUFFER_SIZE 1000
#endif /* LOGIC_ANALYZER_BUFFER_SIZE */

#if EFI_LOGIC_ANALYZER
static char logicAnalyzerBuffer[LOGIC_ANALYZER_BUFFER_SIZE];
static Logging logicAnalyzerLogger("logic analyzer", logicAnalyzerBuffer, sizeof(logicAnalyzerBuffer));
#endif // EFI_LOGIC_ANALYZER

/**
 * @brief Sends all pending data to rusEfi console
 *
 * This method is periodically invoked by the main loop
 * todo: is this mostly dead code?
 */
void updateDevConsoleState() {
#if EFI_PROD_CODE
	// todo: unify with simulator!
	if (hasFirmwareError()) {
		efiPrintf("%s error: %s", CRITICAL_PREFIX, getCriticalErrorMessage());
		return;
	}
#endif /* EFI_PROD_CODE */

#if EFI_RTC
  engine->outputChannels.rtcUnixEpochTime = getEpochTime();
#endif

#if HAL_USE_ADC
	printFullAdcReportIfNeeded();
#endif /* HAL_USE_ADC */

#if EFI_ENGINE_CONTROL && EFI_SHAFT_POSITION_INPUT
	int currentCkpEventCounter = engine->triggerCentral.triggerState.getTotalEventCounter();
	systime_t nowSeconds = getTimeNowS();
static int prevCkpEventCounter = -1;
static systime_t timeOfPreviousReport = (systime_t) -1;
	if (prevCkpEventCounter == currentCkpEventCounter && timeOfPreviousReport == nowSeconds) {
		return;
	}
	timeOfPreviousReport = nowSeconds;

	prevCkpEventCounter = currentCkpEventCounter;
#else
	chThdSleepMilliseconds(200);
#endif

#if EFI_LOGIC_ANALYZER
	printWave(&logicAnalyzerLogger);
	scheduleLogging(&logicAnalyzerLogger);
#endif /* EFI_LOGIC_ANALYZER */
}

#if EFI_PROD_CODE
static OutputPin* leds[] = { &enginePins.warningLedPin, &enginePins.runningLedPin,
		&enginePins.errorLedPin, &enginePins.communicationLedPin, &enginePins.checkEnginePin };
#endif // EFI_PROD_CODE

void initWarningRunningPins() {
#if EFI_PROD_CODE
    // open question if we need warningLedPin and runningLedPin at all!
	enginePins.warningLedPin.initPin("led: warning status", getWarningLedPin(), LED_PIN_MODE, true);
	enginePins.runningLedPin.initPin("led: running status", getRunningLedPin(), LED_PIN_MODE, true);
#endif /* EFI_PROD_CODE */
}

#if EFI_PROD_CODE
static void initStatusLeds() {
	enginePins.communicationLedPin.initPin("led: comm status", getCommsLedPin(), LED_PIN_MODE, true);
	// checkEnginePin is already initialized by the time we get here
}

static bool isTriggerErrorNow() {
#if EFI_ENGINE_CONTROL && EFI_SHAFT_POSITION_INPUT
	bool justHadError = engine->triggerCentral.triggerState.someSortOfTriggerError();
	return justHadError || engine->triggerCentral.isTriggerDecoderError();
#else
	return false;
#endif /* EFI_ENGINE_CONTROL && EFI_SHAFT_POSITION_INPUT */
}

extern bool consoleByteArrived;

class CommunicationBlinkingTask : public PeriodicTimerController {

	int getPeriodMs() override {
		return counter % 2 == 0 ? onTimeMs : offTimeMs;
	}

	void setAllLeds(int value) {
		// make sure we do not turn the critical LED off if already have
		// critical error by now
		for (size_t i = 0; !hasFirmwareError() && i < efi::size(leds); i++) {
			leds[i]->setValue(value, /*force*/true);
		}
	}

	void PeriodicTask() override {
		counter++;

		if (counter == 1) {
			// first invocation of BlinkingTask
			setAllLeds(1);
		} else if (counter == 2) {
			// second invocation of BlinkingTask
			setAllLeds(0);
		} else if (counter % 2 == 0) {
			enginePins.communicationLedPin.setValue(0, /*force*/true);

#if EFI_FILE_LOGGING
  extern bool needErrorReportFile;
#else
#define needErrorReportFile false
#endif // EFI_FILE_LOGGING
      // todo: properly encapsulate warning LED logic!
      if (!needErrorReportFile) {
			  enginePins.warningLedPin.setValue(0);
			}
		} else {
#define BLINKING_PERIOD_MS 33

			if (hasFirmwareError()) {
				// special behavior in case of critical error - not equal on/off time
				// this special behavior helps to notice that something is not right, also
				// differentiates software firmware error from critical interrupt error with CPU halt.
				offTimeMs = 50;
				onTimeMs = 450;
			} else if (consoleByteArrived) {
				offTimeMs = 100;
				onTimeMs = 33;
#if EFI_CONFIGURATION_STORAGE
			} else if (getNeedToWriteConfiguration()) {
				offTimeMs = onTimeMs = 500;
#endif /* EFI_CONFIGURATION_STORAGE */
			} else {
				onTimeMs =
#if EFI_USB_SERIAL
				is_usb_serial_ready() ? 3 * BLINKING_PERIOD_MS :
#endif // EFI_USB_SERIAL
				BLINKING_PERIOD_MS;
				offTimeMs = 0.6 * onTimeMs;
			}

			enginePins.communicationLedPin.setValue(1, /*force*/true);

	#if EFI_ENGINE_CONTROL
			if (isTriggerErrorNow()) {
				// todo: at the moment warning codes do not affect warning LED?!
				enginePins.warningLedPin.setValue(1);
			}
	#endif /* EFI_ENGINE_CONTROL */
		}
	}

private:
	int counter = 0;
	int onTimeMs = 100;
	int offTimeMs = 100;
};

static CommunicationBlinkingTask communicationsBlinkingTask;

#endif /* EFI_PROD_CODE */


#if EFI_TUNER_STUDIO

/**
 * This is useful if we are changing engine mode dynamically
 * For example http://rusefi.com/forum/viewtopic.php?f=5&t=1085
 */
static int packEngineMode() {
	return (Enum2Underlying(engineConfiguration->fuelAlgorithm) << 4) +
			(engineConfiguration->injectionMode << 2) +
			engineConfiguration->ignitionMode;
}

static void updateTempSensors() {
	SensorResult clt = Sensor::get(SensorType::Clt);
	engine->outputChannels.coolant = clt.value_or(0);
	engine->outputChannels.isCltError = !clt.Valid;

	SensorResult iat = Sensor::get(SensorType::Iat);
	engine->outputChannels.intake = iat.value_or(0);
	engine->outputChannels.isIatError = !iat.Valid;

	SensorResult auxTemp1 = Sensor::get(SensorType::AuxTemp1);
	engine->outputChannels.auxTemp1 = auxTemp1.value_or(0);

	SensorResult auxTemp2 = Sensor::get(SensorType::AuxTemp2);
	engine->outputChannels.auxTemp2 = auxTemp2.value_or(0);

	SensorResult oilTemp = Sensor::get(SensorType::OilTemperature);
	engine->outputChannels.oilTemp = oilTemp.value_or(0);

    // see also updateFuelSensors()
	SensorResult fuelTemp = Sensor::get(SensorType::FuelTemperature);
	engine->outputChannels.fuelTemp = fuelTemp.value_or(0);

	SensorResult ambientTemp = Sensor::get(SensorType::AmbientTemperature);
	engine->outputChannels.ambientTemp = ambientTemp.value_or(0);

	SensorResult compressorDischargeTemp = Sensor::get(SensorType::CompressorDischargeTemperature);
	engine->outputChannels.compressorDischargeTemp = compressorDischargeTemp.value_or(0);
}

void updateUnfilteredRawPedal();

static void updateThrottles() {
	SensorResult tps1 = Sensor::get(SensorType::Tps1);
	engine->outputChannels.TPSValue = tps1.value_or(0);
	engine->outputChannels.isTpsError = Sensor::hasSensor(SensorType::Tps1) ? !tps1.Valid : false;
	engine->outputChannels.tpsADC = convertVoltageTo10bitADC(Sensor::getRaw(SensorType::Tps1Primary));

	SensorResult tps2 = Sensor::get(SensorType::Tps2);
	engine->outputChannels.TPS2Value = tps2.value_or(0);
	// If we don't have a TPS2 at all, don't turn on the failure light
	engine->outputChannels.isTps2Error = Sensor::hasSensor(SensorType::Tps2Primary) ? !tps2.Valid : false;

	SensorResult pedal = Sensor::get(SensorType::AcceleratorPedal);
	engine->outputChannels.throttlePedalPosition = pedal.value_or(0);
	// Only report fail if you have one (many people don't)
	engine->outputChannels.isPedalError = Sensor::hasSensor(SensorType::AcceleratorPedal) ? !pedal.Valid : false;

	// TPS 1 pri/sec split
	engine->outputChannels.tps1Split = Sensor::getOrZero(SensorType::Tps1Primary) - Sensor::getOrZero(SensorType::Tps1Secondary);
	// TPS 2 pri/sec split
	engine->outputChannels.tps2Split = Sensor::getOrZero(SensorType::Tps2Primary) - Sensor::getOrZero(SensorType::Tps2Secondary);
	// TPS1 - TPS2 split
	engine->outputChannels.tps12Split = Sensor::getOrZero(SensorType::Tps1) - Sensor::getOrZero(SensorType::Tps2);
	// Pedal pri/sec split
	engine->outputChannels.accPedalSplit = Sensor::getOrZero(SensorType::AcceleratorPedalPrimary) - Sensor::getOrZero(SensorType::AcceleratorPedalSecondary);

	engine->outputChannels.accPedalUnfiltered = Sensor::getOrZero(SensorType::AcceleratorPedalUnfiltered);
	updateUnfilteredRawPedal();
}

static void updateLambda() {
	float lambdaValue = Sensor::getOrZero(SensorType::Lambda1);
	engine->outputChannels.lambdaValue = lambdaValue;
	engine->outputChannels.AFRValue = lambdaValue * engine->fuelComputer.stoichiometricRatio;
	// TODO: this can be calculated on PC side!
	engine->outputChannels.afrGasolineScale = lambdaValue * STOICH_RATIO;
	engine->outputChannels.SmoothedAFRValue = Sensor::getOrZero(SensorType::SmoothedLambda1);

	float lambda2Value = Sensor::getOrZero(SensorType::Lambda2);
	engine->outputChannels.lambdaValue2 = lambda2Value;
	engine->outputChannels.AFRValue2 = lambda2Value * engine->fuelComputer.stoichiometricRatio;
	// TODO: this can be calculated on PC side!
	engine->outputChannels.afr2GasolineScale = lambda2Value * STOICH_RATIO;
	engine->outputChannels.SmoothedAFRValue2 = Sensor::getOrZero(SensorType::SmoothedLambda2);
}

static void updateFuelSensors() {
	// Low pressure is directly in kpa
	engine->outputChannels.lowFuelPressure = Sensor::getOrZero(SensorType::FuelPressureLow);
	// High pressure is in bar, aka 100 kpa
	engine->outputChannels.highFuelPressure = KPA2BAR(Sensor::getOrZero(SensorType::FuelPressureHigh));

	engine->outputChannels.flexPercent = Sensor::getOrZero(SensorType::FuelEthanolPercent);

	engine->outputChannels.fuelTankLevel = Sensor::getOrZero(SensorType::FuelLevel);
}

static void updateEgtSensors() {
	engine->outputChannels.egt[0] = Sensor::getOrZero(SensorType::EGT1);
	engine->outputChannels.egt[1] = Sensor::getOrZero(SensorType::EGT2);
	engine->outputChannels.egt[2] = Sensor::getOrZero(SensorType::EGT3);
	engine->outputChannels.egt[3] = Sensor::getOrZero(SensorType::EGT4);
	engine->outputChannels.egt[4] = Sensor::getOrZero(SensorType::EGT5);
	engine->outputChannels.egt[5] = Sensor::getOrZero(SensorType::EGT6);
	engine->outputChannels.egt[6] = Sensor::getOrZero(SensorType::EGT7);
	engine->outputChannels.egt[7] = Sensor::getOrZero(SensorType::EGT8);
}

static void updateVvtSensors() {
#if EFI_SHAFT_POSITION_INPUT
	// 248
	engine->outputChannels.vvtPositionB1I = engine->triggerCentral.getVVTPosition(/*bankIndex*/0, /*camIndex*/0);
	engine->outputChannels.vvtPositionB1E = engine->triggerCentral.getVVTPosition(/*bankIndex*/0, /*camIndex*/1);
	engine->outputChannels.vvtPositionB2I = engine->triggerCentral.getVVTPosition(/*bankIndex*/1, /*camIndex*/0);
	engine->outputChannels.vvtPositionB2E = engine->triggerCentral.getVVTPosition(/*bankIndex*/1, /*camIndex*/1);
#endif
}

static void updateVehicleSpeed() {
#if EFI_VEHICLE_SPEED
	engine->outputChannels.vehicleSpeedKph = Sensor::getOrZero(SensorType::VehicleSpeed);
	engine->outputChannels.wheelSlipRatio = Sensor::getOrZero(SensorType::WheelSlipRatio);
#ifdef MODULE_GEAR_DETECTOR
	engine->outputChannels.speedToRpmRatio = engine->module<GearDetector>()->getGearboxRatio();
	engine->outputChannels.detectedGear = Sensor::getOrZero(SensorType::DetectedGear);
#endif
#endif /* EFI_VEHICLE_SPEED */
}

static SensorType luaGaugeTypes[] = {
  SensorType::LuaGauge1,
  SensorType::LuaGauge2,
  SensorType::LuaGauge3,
  SensorType::LuaGauge4,
  SensorType::LuaGauge5,
  SensorType::LuaGauge6,
  SensorType::LuaGauge7,
  SensorType::LuaGauge8
  };

static void updateRawSensors() {
	engine->outputChannels.rawTps1Primary = Sensor::getRaw(SensorType::Tps1Primary);
	engine->outputChannels.rawTps1Secondary = Sensor::getRaw(SensorType::Tps1Secondary);
	engine->outputChannels.rawTps2Primary = Sensor::getRaw(SensorType::Tps2Primary);
	engine->outputChannels.rawTps2Secondary = Sensor::getRaw(SensorType::Tps2Secondary);
	engine->outputChannels.rawPpsPrimary = Sensor::getRaw(SensorType::AcceleratorPedalPrimary);
	engine->outputChannels.rawPpsSecondary = Sensor::getRaw(SensorType::AcceleratorPedalSecondary);
	engine->outputChannels.rawBattery = Sensor::getRaw(SensorType::BatteryVoltage);
	engine->outputChannels.rawClt = Sensor::getRaw(SensorType::Clt);
	engine->outputChannels.rawIat = Sensor::getRaw(SensorType::Iat);
	engine->outputChannels.rawAuxTemp1 = Sensor::getRaw(SensorType::AuxTemp1);
	engine->outputChannels.rawAuxTemp2 = Sensor::getRaw(SensorType::AuxTemp2);
	engine->outputChannels.rawAmbientTemp = Sensor::getRaw(SensorType::AmbientTemperature);
	engine->outputChannels.rawOilPressure = Sensor::getRaw(SensorType::OilPressure);
	engine->outputChannels.rawFuelLevel = Sensor::getRaw(SensorType::FuelLevel);
  engine->outputChannels.rawAcPressure = Sensor::getRaw(SensorType::AcPressure);
	engine->outputChannels.rawLowFuelPressure = Sensor::getRaw(SensorType::FuelPressureLow);
	engine->outputChannels.rawHighFuelPressure = Sensor::getRaw(SensorType::FuelPressureHigh);
	engine->outputChannels.rawMaf = Sensor::getRaw(SensorType::Maf);
	engine->outputChannels.rawMaf2 = Sensor::getRaw(SensorType::Maf2);
	engine->outputChannels.rawMap = Sensor::getRaw(SensorType::MapSlow);
	engine->outputChannels.rawWastegatePosition = Sensor::getRaw(SensorType::WastegatePosition);
	engine->outputChannels.rawIdlePositionSensor = Sensor::getRaw(SensorType::IdlePosition);
	engine->outputChannels.rawAuxAnalog1 = Sensor::getRaw(SensorType::AuxAnalog1);
	engine->outputChannels.rawAuxAnalog2 = Sensor::getRaw(SensorType::AuxAnalog2);
	engine->outputChannels.rawAuxAnalog3 = Sensor::getRaw(SensorType::AuxAnalog3);
	engine->outputChannels.rawAuxAnalog4 = Sensor::getRaw(SensorType::AuxAnalog4);

  for (size_t i = 0;i<LUA_GAUGE_COUNT;i++) {
	  engine->outputChannels.luaGauges[i] = Sensor::getOrZero(luaGaugeTypes[i]);
  }

	for (int i = 0; i < LUA_ANALOG_INPUT_COUNT; i++) {
		adc_channel_e channel = engineConfiguration->auxAnalogInputs[i];
		if (isAdcChannelValid(channel)) {
			engine->outputChannels.rawAnalogInput[i] = adcGetScaledVoltage("raw aux", channel).value_or(0);
		}
	}

	// TODO: transition AFR to new sensor model
	engine->outputChannels.rawAfr = (engineConfiguration->afr.hwChannel == EFI_ADC_NONE) ? 0 : adcGetScaledVoltage("ego", engineConfiguration->afr.hwChannel).value_or(0);
}
static void updatePressures() {
	engine->outputChannels.baroPressure = Sensor::getOrZero(SensorType::BarometricPressure);
  // instantMAPValue is injected in a very different way
  float mapValue = Sensor::getOrZero(SensorType::Map);
	engine->outputChannels.MAPValue = mapValue;

	engine->outputChannels.mapFast = Sensor::getOrZero(SensorType::MapFast);
	engine->outputChannels.oilPressure = Sensor::getOrZero(SensorType::OilPressure);
  engine->outputChannels.acPressure = Sensor::getOrZero(SensorType::AcPressure);

 	engine->outputChannels.compressorDischargePressure = Sensor::getOrZero(SensorType::CompressorDischargePressure);
 	engine->outputChannels.throttleInletPressure = Sensor::getOrZero(SensorType::ThrottleInletPressure);
 	engine->outputChannels.throttlePressureRatio = getThrottlePressureRatio(mapValue);

	engine->outputChannels.auxLinear1 = Sensor::getOrZero(SensorType::AuxLinear1);
	engine->outputChannels.auxLinear2 = Sensor::getOrZero(SensorType::AuxLinear2);
	engine->outputChannels.auxLinear3 = Sensor::getOrZero(SensorType::AuxLinear3);
	engine->outputChannels.auxLinear4 = Sensor::getOrZero(SensorType::AuxLinear4);
}

static void updateMiscSensors() {
	engine->outputChannels.VBatt = Sensor::getOrZero(SensorType::BatteryVoltage);

	engine->outputChannels.idlePositionSensor = Sensor::getOrZero(SensorType::IdlePosition);

	engine->outputChannels.wastegatePositionSensor = Sensor::getOrZero(SensorType::WastegatePosition);

	engine->outputChannels.ISSValue = Sensor::getOrZero(SensorType::InputShaftSpeed);
	engine->outputChannels.auxSpeed1 = Sensor::getOrZero(SensorType::AuxSpeed1);
	engine->outputChannels.auxSpeed2 = Sensor::getOrZero(SensorType::AuxSpeed2);

#if	HAL_USE_ADC
	engine->outputChannels.internalMcuTemperature = getMCUInternalTemperature();
	engine->outputChannels.internalVref = getMCUVref();
	engine->outputChannels.internalVbat = getMcuVbatVoltage();
#endif /* HAL_USE_ADC */
}

static void updateSensors() {
	updateTempSensors();
	updateThrottles();
	updateRawSensors();
	updateLambda();
	updateFuelSensors();
	updateEgtSensors();
	updateVvtSensors();
	updateVehicleSpeed();
	updatePressures();
	updateMiscSensors();
}

static void updateFuelCorrections() {
	engine->outputChannels.Gego = 100.0f * engine->engineState.stftCorrection[0];
}

static void updateFuelResults() {
#if EFI_VEHICLE_SPEED && defined (MODULE_ODOMETER)
	engine->outputChannels.fuelFlowRate = engine->module<TripOdometer>()->getConsumptionGramPerSecond();
	engine->outputChannels.totalFuelConsumption = engine->module<TripOdometer>()->getConsumedGrams();
	engine->outputChannels.ignitionOnTime = engine->module<TripOdometer>()->getIgnitionOnTime();
	engine->outputChannels.engineRunTime = engine->module<TripOdometer>()->getEngineRunTime();

	// output channel in km
	engine->outputChannels.distanceTraveled = 0.001f * engine->module<TripOdometer>()->getDistanceMeters();
#endif // EFI_VEHICLE_SPEED MODULE_ODOMETER
}

static void updateFuelInfo() {
	updateFuelCorrections();
	updateFuelResults();
#if EFI_ENGINE_CONTROL
	const auto& wallFuel = engine->injectionEvents.elements[0].getWallFuel();
	engine->outputChannels.wallFuelAmount = wallFuel.getWallFuel() * 1000;			// Convert grams to mg
	engine->outputChannels.wallFuelCorrectionValue = wallFuel.wallFuelCorrection * 1000;	// Convert grams to mg

	engine->outputChannels.veValue = engine->engineState.currentVe;
#endif // EFI_ENGINE_CONTROL
}

static void updateIgnition(float rpm) {
#if EFI_ENGINE_CONTROL
	engine->outputChannels.coilDutyCycle = getCoilDutyCycle(rpm);
#endif // EFI_ENGINE_CONTROL
}

static void updateFlags() {
	engine->outputChannels.isO2HeaterOn = enginePins.o2heater.getLogicValue();
	// todo: eliminate state copy logic by giving DfcoController it's owm xxx.txt and leveraging LiveData
	engine->outputChannels.dfcoActive = engine->module<DfcoController>()->cutFuel();

#if EFI_LAUNCH_CONTROL
	engine->outputChannels.launchTriggered = engine->launchController.isLaunchCondition;
#endif

#if EFI_PROD_CODE
	engine->outputChannels.isTriggerError = isTriggerErrorNow();
#endif // EFI_PROD_CODE

#if EFI_CONFIGURATION_STORAGE
	engine->outputChannels.needBurn = getNeedToWriteConfiguration();
#endif /* EFI_CONFIGURATION_STORAGE */
#if EFI_FILE_LOGGING
	updateSdCardLiveFlags();
#endif
}

// sensor state for EFI Analytics Tuner Studio
// todo: the 'let's copy internal state for external consumers' approach is DEPRECATED
// As of 2022 it's preferred to leverage LiveData where all state is exposed
// this method is invoked ONLY if we SD card log or have serial connection with some frontend app
void updateTunerStudioState() {
	TunerStudioOutputChannels *tsOutputChannels = &engine->outputChannels;
#if EFI_USB_SERIAL
  // pretty much SD card logs know if specifically USB serial is active
	engine->outputChannels.isUsbConnected =	is_usb_serial_ready();
#endif // EFI_USB_SERIAL

	float rpm = Sensor::get(SensorType::Rpm).value_or(0);


  static Timer blinkIndicatorsTimer;
  constexpr float blinkHalfPeriod = 0.3;
  bool isBlinkPhase = blinkIndicatorsTimer.hasElapsedSec(blinkHalfPeriod);
#if EFI_ELECTRONIC_THROTTLE_BODY
  blinkEtbErrorCodes(isBlinkPhase);
#endif // EFI_ELECTRONIC_THROTTLE_BODY
  if (isBlinkPhase) {
    engine->outputChannels.sparkCutReasonBlinker = 0;
    engine->outputChannels.fuelCutReasonBlinker = 0;

    if (blinkIndicatorsTimer.hasElapsedSec(2 * blinkHalfPeriod)) {
      blinkIndicatorsTimer.reset();
    }
  } else {
    engine->outputChannels.sparkCutReasonBlinker = engine->outputChannels.sparkCutReason;
    engine->outputChannels.fuelCutReasonBlinker = engine->outputChannels.fuelCutReason;
  }


#if EFI_PROD_CODE
	executorStatistics();
#endif /* EFI_PROD_CODE */

	DcHardware *dc = getPrimaryDCHardwareForLogging();
	engine->dc_motors.dcOutput0 = dc->dcMotor.get();
	engine->dc_motors.isEnabled0_int = dc->msg() == nullptr;

	tsOutputChannels->RPMValue = rpm;
#if EFI_SHAFT_POSITION_INPUT
	tsOutputChannels->instantRpm = engine->triggerCentral.instantRpm.getInstantRpm();
	tsOutputChannels->totalTriggerErrorCounter = engine->triggerCentral.triggerState.totalTriggerErrorCounter;
	tsOutputChannels->rpmAcceleration = engine->rpmCalculator.getRpmAcceleration();

	tsOutputChannels->orderingErrorCounter = engine->triggerCentral.triggerState.orderingErrorCounter;
#endif // EFI_SHAFT_POSITION_INPUT

	updateSensors();
	updateFuelInfo();
	updateIgnition(rpm);
	updateFlags();
	// update calibration channel, reset to None state after timeout
	tsCalibrationIsIdle();

	// Output both the estimated air flow, and measured air flow (if available)
	tsOutputChannels->mafMeasured = Sensor::getOrZero(SensorType::Maf);
	tsOutputChannels->mafMeasured2 = Sensor::getOrZero(SensorType::Maf2);
	tsOutputChannels->mafEstimate = engine->engineState.airflowEstimate;

#if EFI_ENGINE_CONTROL
	tsOutputChannels->injectorDutyCycle = minF(/*let's avoid scaled "uint8_t, 2" overflow*/127, getInjectorDutyCycle(rpm));
	tsOutputChannels->injectorDutyCycleStage2 = getInjectorDutyCycleStage2(rpm);
#endif

	tsOutputChannels->seconds = getTimeNowS();

	tsOutputChannels->engineMode = packEngineMode();
	tsOutputChannels->firmwareVersion = getRusEfiVersion();

	tsOutputChannels->accelerationLat = engine->sensors.accelerometer.lat;
	tsOutputChannels->accelerationLon = engine->sensors.accelerometer.lon;
	tsOutputChannels->accelerationVert = engine->sensors.accelerometer.vert;
	tsOutputChannels->gyroYaw = engine->sensors.accelerometer.yawRate;

#if EFI_DYNO_VIEW
	tsOutputChannels->hp = getDynoviewHP();
  tsOutputChannels->torque = getDynoviewTorque();
#else
	tsOutputChannels->hp = -1;
  tsOutputChannels->torque = -1;
#endif

	tsOutputChannels->turboSpeed = Sensor::getOrZero(SensorType::TurbochargerSpeed);
	extern FrequencySensor inputShaftSpeedSensor;
	tsOutputChannels->issEdgeCounter = inputShaftSpeedSensor.eventCounter;
	extern FrequencySensor vehicleSpeedSensor;
	tsOutputChannels->vssEdgeCounter = vehicleSpeedSensor.eventCounter;

	tsOutputChannels->hasCriticalError = hasFirmwareError() || hasConfigError() || engine->engineState.warnings.hasWarningMessage();
	tsOutputChannels->hasFaultReportFile = hasErrorReportFile();
	tsOutputChannels->triggerPageRefreshFlag = needToTriggerTsRefresh() || ltftNeedVeRefresh();

	tsOutputChannels->isWarnNow = engine->engineState.warnings.isWarningNow();

	tsOutputChannels->tpsAccelFuel = engine->engineState.tpsAccelEnrich;

	tsOutputChannels->checkEngine = hasErrorCodes();

	engine->engineState.warnings.refreshTs();

	tsOutputChannels->starterState = enginePins.starterControl.getLogicValue();
	tsOutputChannels->starterRelayDisable = enginePins.starterRelayDisable.getLogicValue();


	tsOutputChannels->revolutionCounterSinceStart = engine->rpmCalculator.getRevolutionCounterSinceStart();

#if EFI_CLOCK_LOCKS
	tsOutputChannels->maxLockedDuration = NT2US(maxLockedDuration);
#endif /* EFI_CLOCK_LOCKS */

#if EFI_SHAFT_POSITION_INPUT
	tsOutputChannels->maxTriggerReentrant = maxTriggerReentrant;
	tsOutputChannels->triggerPrimaryFall = engine->triggerCentral.getHwEventCounter((int)SHAFT_PRIMARY_FALLING);
	tsOutputChannels->triggerPrimaryRise = engine->triggerCentral.getHwEventCounter((int)SHAFT_PRIMARY_RISING);

	tsOutputChannels->triggerSecondaryFall = engine->triggerCentral.getHwEventCounter((int)SHAFT_SECONDARY_FALLING);
	tsOutputChannels->triggerSecondaryRise = engine->triggerCentral.getHwEventCounter((int)SHAFT_SECONDARY_RISING);

#endif // EFI_SHAFT_POSITION_INPUT

#if HAL_USE_PAL && EFI_PROD_CODE
	tsOutputChannels->extiOverflowCount = getExtiOverflowCounter();
#endif

	switch (engineConfiguration->debugMode)	{
	case DBG_LOGIC_ANALYZER:
#if EFI_LOGIC_ANALYZER
    // used by HW CI
		reportLogicAnalyzerToTS();
#endif /* EFI_LOGIC_ANALYZER */
		break;
	default:
		;
	}
}

#endif /* EFI_TUNER_STUDIO */

void startStatusThreads() {
	// todo: refactoring needed, this file should probably be split into pieces
#if EFI_PROD_CODE
	initStatusLeds();
	communicationsBlinkingTask.start();
#endif /* EFI_PROD_CODE */
}
