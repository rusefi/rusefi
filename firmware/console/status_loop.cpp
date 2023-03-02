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

#if EFI_LOGIC_ANALYZER
#include "logic_analyzer.h"
#endif /* EFI_LOGIC_ANALYZER */

#include "trigger_central.h"
#include "sensor_reader.h"
#include "mmc_card.h"
#include "console_io.h"
#include "malfunction_central.h"
#include "speed_density.h"

#include "advance_map.h"
#include "tunerstudio.h"
#include "fuel_math.h"
#include "main_trigger_callback.h"
#include "spark_logic.h"
#include "idle_thread.h"
#include "gitversion.h"
#include "can_hw.h"
#include "periodic_thread_controller.h"
#include "binary_logging.h"
#include "buffered_writer.h"
#include "dynoview.h"
#include "frequency_sensor.h"
#include "digital_input_exti.h"
#include "dc_motors.h"

extern bool main_loop_started;

#if EFI_PROD_CODE
// todo: move this logic to algo folder!
#include "rtc_helper.h"
#include "rusefi.h"
#include "pin_repository.h"
#include "max31855.h"
#include "single_timer_executor.h"
#include "periodic_task.h"
#endif /* EFI_PROD_CODE */

#if EFI_INTERNAL_FLASH
#include "flash_main.h"
#endif

#if EFI_MAP_AVERAGING
#include "map_averaging.h"
#endif

#if (BOARD_TLE8888_COUNT > 0)
#include "tle8888.h"
#endif /* BOARD_TLE8888_COUNT */

#if EFI_ENGINE_SNIFFER
#include "engine_sniffer.h"
extern WaveChart waveChart;
#endif /* EFI_ENGINE_SNIFFER */

#include "sensor_chart.h"

#ifndef LED_WARNING_BRAIN_PIN_MODE
#define LED_WARNING_BRAIN_PIN_MODE	OM_DEFAULT
#endif
#ifndef LED_RUNING_BRAIN_PIN_MODE
#define LED_RUNING_BRAIN_PIN_MODE	OM_DEFAULT
#endif
#ifndef LED_COMMUNICATION_BRAIN_PIN_MODE
#define LED_COMMUNICATION_BRAIN_PIN_MODE	OM_DEFAULT
#endif

int warningEnabled = true;

extern int maxTriggerReentrant;
extern uint32_t maxLockedDuration;

static void setWarningEnabled(int value) {
	warningEnabled = value;
}

/**
 * This is useful if we are changing engine mode dynamically
 * For example http://rusefi.com/forum/viewtopic.php?f=5&t=1085
 */
static int packEngineMode() {
	return (engineConfiguration->fuelAlgorithm << 4) +
			(engineConfiguration->injectionMode << 2) +
			engineConfiguration->ignitionMode;
}

static int prevCkpEventCounter = -1;

/**
 * Time when the firmware version was last reported
 * TODO: implement a request/response instead of just constantly sending this out
 */
static Timer printVersionTimer;

static void printRusefiVersion(const char *engineTypeName, const char *firmwareBuildId) {
	// VersionChecker in rusEFI console is parsing these version string, please follow the expected format
	efiPrintfProto(PROTOCOL_VERSION_TAG, "%d@%s %s %s %d",
			getRusEfiVersion(), GIT_HASH_SHORT,
			firmwareBuildId,
			engineTypeName,
			getTimeNowS());
}

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
	printOutPin(PROTOCOL_TACH_NAME, engineConfiguration->tachOutputPin);
#if EFI_LOGIC_ANALYZER
	printOutPin(PROTOCOL_WA_CHANNEL_1, engineConfiguration->logicAnalyzerPins[0]);
	printOutPin(PROTOCOL_WA_CHANNEL_2, engineConfiguration->logicAnalyzerPins[1]);
#endif /* EFI_LOGIC_ANALYZER */

	int cylCount = minI(engineConfiguration->specs.cylindersCount, MAX_CYLINDER_COUNT);
	for (int i = 0; i < cylCount; i++) {
		printOutPin(enginePins.coils[i].getShortName(), engineConfiguration->ignitionPins[i]);
		printOutPin(enginePins.trailingCoils[i].getShortName(), engineConfiguration->trailingCoilPins[i]);
		printOutPin(enginePins.injectors[i].getShortName(), engineConfiguration->injectionPins[i]);
	}
	for (int i = 0; i < AUX_DIGITAL_VALVE_COUNT;i++) {
		printOutPin(enginePins.auxValve[i].getShortName(), engineConfiguration->auxValves[i]);
	}
#endif /* EFI_PROD_CODE */
}

void printOverallStatus() {
#if EFI_ENGINE_SNIFFER
	waveChart.publishIfFull();
#endif /* EFI_ENGINE_SNIFFER */

#if EFI_SENSOR_CHART
	publishSensorChartIfFull();
#endif // EFI_SENSOR_CHART

	/**
	 * we report the version every second - this way the console does not need to
	 * request it and we will display it pretty soon
	 */
	if (printVersionTimer.hasElapsedSec(1)) {
		// we're sending, reset the timer
		printVersionTimer.reset();

		// Output the firmware version, board type, git hash, uptime in seconds, etc
		printRusefiVersion(getEngine_type_e(engineConfiguration->engineType), FIRMWARE_ID);

		// Output the current engine sniffer pin mappings
		printEngineSnifferPinMappings();
	}
}

static systime_t timeOfPreviousReport = (systime_t) -1;

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
void updateDevConsoleState(void) {
	// todo: make SWO work
//	char *msg = "hello\r\n";
//	for(int i=0;i<strlen(msg);i++) {
//		ITM_SendChar(msg[i]);
//	}

#if EFI_PROD_CODE
	// todo: unify with simulator!
	if (hasFirmwareError()) {
		efiPrintf("%s error: %s", CRITICAL_PREFIX, getCriticalErrorMessage());
		warningEnabled = false;
		return;
	}
#endif /* EFI_PROD_CODE */

#if HAL_USE_ADC
	printFullAdcReportIfNeeded();
#endif /* HAL_USE_ADC */

	systime_t nowSeconds = getTimeNowS();

#if EFI_ENGINE_CONTROL && EFI_SHAFT_POSITION_INPUT
	int currentCkpEventCounter = engine->triggerCentral.triggerState.getTotalEventCounter();
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

static OutputPin *leds[] = { &enginePins.warningLedPin, &enginePins.runningLedPin,
		&enginePins.errorLedPin, &enginePins.communicationLedPin, &enginePins.checkEnginePin };

static void initStatusLeds() {
	enginePins.communicationLedPin.initPin("led: comm status", engineConfiguration->communicationLedPin, LED_COMMUNICATION_BRAIN_PIN_MODE, true);
	// checkEnginePin is already initialized by the time we get here

	enginePins.warningLedPin.initPin("led: warning status", engineConfiguration->warningLedPin, LED_WARNING_BRAIN_PIN_MODE, true);
	enginePins.runningLedPin.initPin("led: running status", engineConfiguration->runningLedPin, LED_RUNING_BRAIN_PIN_MODE, true);
}

#if EFI_PROD_CODE

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
		for (uint32_t i = 0; !hasFirmwareError() && i < sizeof(leds) / sizeof(leds[0]); i++) {
			leds[i]->setValue(value);
		}
	}

	void PeriodicTask() override {
		counter++;

		bool lowVBatt = Sensor::getOrZero(SensorType::BatteryVoltage) < LOW_VBATT;

		if (counter == 1) {
			// first invocation of BlinkingTask
			setAllLeds(1);
		} else if (counter == 2) {
			// second invocation of BlinkingTask
			setAllLeds(0);
		} else if (counter % 2 == 0) {
			enginePins.communicationLedPin.setValue(0);

			if (!lowVBatt) {
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
#if EFI_INTERNAL_FLASH
			} else if (getNeedToWriteConfiguration()) {
				offTimeMs = onTimeMs = 500;
#endif // EFI_INTERNAL_FLASH
			} else {
				onTimeMs =
#if EFI_USB_SERIAL
				is_usb_serial_ready() ? 3 * BLINKING_PERIOD_MS :
#endif // EFI_USB_SERIAL
				BLINKING_PERIOD_MS;
				offTimeMs = 0.6 * onTimeMs;
			}

			enginePins.communicationLedPin.setValue(1);

	#if EFI_ENGINE_CONTROL
			if (lowVBatt || isTriggerErrorNow() || isIgnitionTimingError()) {
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
}

static void updateThrottles() {
	SensorResult tps1 = Sensor::get(SensorType::Tps1);
	engine->outputChannels.TPSValue = tps1.value_or(0);
	engine->outputChannels.isTpsError = !tps1.Valid;
	engine->outputChannels.tpsADC = convertVoltageTo10bitADC(Sensor::getRaw(SensorType::Tps1Primary));

	SensorResult tps2 = Sensor::get(SensorType::Tps2);
	engine->outputChannels.TPS2Value = tps2.value_or(0);
	// If we don't have a TPS2 at all, don't turn on the failure light
	engine->outputChannels.isTps2Error = isTps2Error();

	SensorResult pedal = Sensor::get(SensorType::AcceleratorPedal);
	engine->outputChannels.throttlePedalPosition = pedal.value_or(0);
	// Only report fail if you have one (many people don't)
	engine->outputChannels.isPedalError = isPedalError();

	// TPS 1 pri/sec split
	engine->outputChannels.tps1Split = Sensor::getOrZero(SensorType::Tps1Primary) - Sensor::getOrZero(SensorType::Tps1Secondary);
	// TPS 2 pri/sec split
	engine->outputChannels.tps2Split = Sensor::getOrZero(SensorType::Tps2Primary) - Sensor::getOrZero(SensorType::Tps2Secondary);
	// TPS1 - TPS2 split
	engine->outputChannels.tps12Split = Sensor::getOrZero(SensorType::Tps1) - Sensor::getOrZero(SensorType::Tps2);
	// Pedal pri/sec split
	engine->outputChannels.accPedalSplit = Sensor::getOrZero(SensorType::AcceleratorPedalPrimary) - Sensor::getOrZero(SensorType::AcceleratorPedalSecondary);
}

static void updateLambda() {
	float lambdaValue = Sensor::getOrZero(SensorType::Lambda1);
	engine->outputChannels.lambdaValue = lambdaValue;
	engine->outputChannels.AFRValue = lambdaValue * engine->fuelComputer.stoichiometricRatio;

	float lambda2Value = Sensor::getOrZero(SensorType::Lambda2);
	engine->outputChannels.lambdaValue2 = lambda2Value;
	engine->outputChannels.AFRValue2 = lambda2Value * engine->fuelComputer.stoichiometricRatio;
}

static void updateFuelSensors() {
	// Low pressure is directly in kpa
	engine->outputChannels.lowFuelPressure = Sensor::getOrZero(SensorType::FuelPressureLow);
	// High pressure is in bar, aka 100 kpa
	engine->outputChannels.highFuelPressure = KPA2BAR(Sensor::getOrZero(SensorType::FuelPressureHigh));

	engine->outputChannels.flexPercent = Sensor::getOrZero(SensorType::FuelEthanolPercent);

	engine->outputChannels.fuelTankLevel = Sensor::getOrZero(SensorType::FuelLevel);
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
	engine->outputChannels.speedToRpmRatio = engine->module<GearDetector>()->getGearboxRatio();
	engine->outputChannels.detectedGear = engine->module<GearDetector>()->getCurrentGear();
#endif /* EFI_VEHICLE_SPEED */
}

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
	engine->outputChannels.rawOilPressure = Sensor::getRaw(SensorType::OilPressure);
	engine->outputChannels.rawLowFuelPressure = Sensor::getRaw(SensorType::FuelPressureLow);
	engine->outputChannels.rawHighFuelPressure = Sensor::getRaw(SensorType::FuelPressureHigh);
	engine->outputChannels.rawMaf = Sensor::getRaw(SensorType::Maf);
	engine->outputChannels.rawMaf2 = Sensor::getRaw(SensorType::Maf2);
	engine->outputChannels.rawMap = Sensor::getRaw(SensorType::MapSlow);
	engine->outputChannels.rawWastegatePosition = Sensor::getRaw(SensorType::WastegatePosition);

	engine->outputChannels.luaGauges[0] = Sensor::getOrZero(SensorType::LuaGauge1);
	engine->outputChannels.luaGauges[1] = Sensor::getOrZero(SensorType::LuaGauge2);

	for (int i = 0; i < LUA_ANALOG_INPUT_COUNT; i++) {
		adc_channel_e ch = engineConfiguration->auxAnalogInputs[i];
		if (isAdcChannelValid(ch)) {
			engine->outputChannels.rawAnalogInput[i] = getVoltageDivided("raw aux", ch);
		}
	}

	// TODO: transition AFR to new sensor model
	engine->outputChannels.rawAfr = (engineConfiguration->afr.hwChannel == EFI_ADC_NONE) ? 0 : getVoltageDivided("ego", engineConfiguration->afr.hwChannel);
}
static void updatePressures() {
	engine->outputChannels.baroPressure = Sensor::getOrZero(SensorType::BarometricPressure);
	engine->outputChannels.MAPValue = Sensor::getOrZero(SensorType::Map);
	engine->outputChannels.oilPressure = Sensor::getOrZero(SensorType::OilPressure);

	engine->outputChannels.auxLinear1 = Sensor::getOrZero(SensorType::AuxLinear1);
	engine->outputChannels.auxLinear2 = Sensor::getOrZero(SensorType::AuxLinear2);
}

static void updateMiscSensors() {
	engine->outputChannels.VBatt = Sensor::getOrZero(SensorType::BatteryVoltage);

	engine->outputChannels.wastegatePositionSensor = Sensor::getOrZero(SensorType::WastegatePosition);

	engine->outputChannels.ISSValue = Sensor::getOrZero(SensorType::InputShaftSpeed);
	engine->outputChannels.auxSpeed1 = Sensor::getOrZero(SensorType::AuxSpeed1);
	engine->outputChannels.auxSpeed2 = Sensor::getOrZero(SensorType::AuxSpeed2);

#if	HAL_USE_ADC
	engine->outputChannels.internalMcuTemperature = getMCUInternalTemperature();
#endif /* HAL_USE_ADC */

	// tCharge depends on the previous state, so we should use the stored value.
	engine->outputChannels.tCharge = engine->engineState.sd.tCharge;
}

static void updateSensors() {
	updateTempSensors();
	updateThrottles();
	updateRawSensors();
	updateLambda();
	updateFuelSensors();
	updateVvtSensors();
	updateVehicleSpeed();
	updatePressures();
	updateMiscSensors();
}

static void updateFuelCorrections() {
	engine->outputChannels.fuelPidCorrection[0] = 100.0f * (engine->stftCorrection[0] - 1.0f);
	engine->outputChannels.fuelPidCorrection[1] = 100.0f * (engine->stftCorrection[1] - 1.0f);

// get rid of this, have gauges use injector model info directly
	engine->outputChannels.injectorLagMs = engine->module<InjectorModel>()->getDeadtime();
}

static void updateFuelResults() {
	// todo: kill outputChannel while taking care of gauge name and scale!
	engine->outputChannels.chargeAirMass = engine->fuelComputer.sdAirMassInOneCylinder;

	engine->outputChannels.baseFuel = engine->engineState.baseFuel * 1000;	// Convert grams to mg
	engine->outputChannels.fuelRunning = engine->fuelComputer.running.fuel;

	engine->outputChannels.fuelFlowRate = engine->engineState.fuelConsumption.getConsumptionGramPerSecond();
	engine->outputChannels.totalFuelConsumption = engine->engineState.fuelConsumption.getConsumedGrams();
}

static void updateFuelInfo() {
	updateFuelCorrections();
	updateFuelResults();

	const auto& wallFuel = engine->injectionEvents.elements[0].wallFuel;
	engine->outputChannels.wallFuelAmount = wallFuel.getWallFuel() * 1000;			// Convert grams to mg
	engine->outputChannels.wallFuelCorrectionValue = wallFuel.wallFuelCorrection * 1000;	// Convert grams to mg

	engine->outputChannels.injectionOffset = engine->engineState.injectionOffset;

	engine->outputChannels.veValue = engine->engineState.currentVe;

	engine->outputChannels.crankingFuelMs = engine->engineState.crankingFuel.fuel;
}

static void updateIgnition(int rpm) {
	float timing = engine->engineState.timingAdvance[0];
	// that's weird logic. also seems broken for two stroke?
	engine->outputChannels.ignitionAdvance = timing > FOUR_STROKE_CYCLE_DURATION / 2 ? timing - FOUR_STROKE_CYCLE_DURATION : timing;

	engine->outputChannels.coilDutyCycle = getCoilDutyCycle(rpm);

	engine->outputChannels.knockCount = engine->module<KnockController>()->getKnockCount();
	engine->outputChannels.knockRetard = engine->module<KnockController>()->getKnockRetard();
}

static void updateFlags() {
#if EFI_USB_SERIAL
	engine->outputChannels.isUsbConnected =	is_usb_serial_ready();
#endif // EFI_USB_SERIAL

	engine->outputChannels.isMainRelayOn = enginePins.mainRelay.getLogicValue();
	engine->outputChannels.isFanOn = enginePins.fanRelay.getLogicValue();
	engine->outputChannels.isFan2On = enginePins.fanRelay2.getLogicValue();
	engine->outputChannels.isO2HeaterOn = enginePins.o2heater.getLogicValue();
	// todo: eliminate state copy logic by giving DfcoController it's owm xxx.txt and leveraging LiveData
	engine->outputChannels.dfcoActive = engine->module<DfcoController>()->cutFuel();

#if EFI_LAUNCH_CONTROL
	engine->outputChannels.launchTriggered = engine->launchController.isLaunchCondition;
#endif

#if EFI_PROD_CODE
	engine->outputChannels.isTriggerError = isTriggerErrorNow();
#endif // EFI_PROD_CODE

#if EFI_INTERNAL_FLASH
	engine->outputChannels.needBurn = getNeedToWriteConfiguration();
#endif /* EFI_INTERNAL_FLASH */
}

// weird thing: one of the reasons for this to be a separate method is stack usage reduction in non-optimized build
// see https://github.com/rusefi/rusefi/issues/3302 and linked tickets
static void updateTpsDebug() {
	// TPS 1 pri/sec ratio - useful for ford ETB that has partial-range second channel
	engine->outputChannels.debugFloatField5 = 100 * Sensor::getOrZero(SensorType::Tps1Primary) / Sensor::getOrZero(SensorType::Tps1Secondary);
}

// sensor state for EFI Analytics Tuner Studio
// todo: the 'let's copy internal state for external consumers' approach is DEPRECATED
// As of 2022 it's preferred to leverage LiveData where all state is exposed
void updateTunerStudioState() {
	TunerStudioOutputChannels *tsOutputChannels = &engine->outputChannels;
#if EFI_SHAFT_POSITION_INPUT
	int rpm = Sensor::get(SensorType::Rpm).value_or(0);
#else /* EFI_SHAFT_POSITION_INPUT */
	int rpm = 0;
#endif /* EFI_SHAFT_POSITION_INPUT */

#if EFI_PROD_CODE
	executorStatistics();
#endif /* EFI_PROD_CODE */

	// header
	tsOutputChannels->tsConfigVersion = TS_FILE_VERSION;
	static_assert(offsetof (TunerStudioOutputChannels, tsConfigVersion) == TS_FILE_VERSION_OFFSET);

DcHardware *getdcHardware();

    DcHardware *dc = getdcHardware();
    engine->dc_motors.dcOutput0 = dc->dcMotor.get();
    engine->dc_motors.isEnabled0_int = dc->msg() == nullptr;

#if EFI_SHAFT_POSITION_INPUT

	// offset 0
	tsOutputChannels->RPMValue = rpm;
	auto instantRpm = engine->triggerCentral.instantRpm.getInstantRpm();
	tsOutputChannels->instantRpm = instantRpm;

	updateSensors();
	updateFuelInfo();
	updateIgnition(rpm);
	updateFlags();

	// 104
	tsOutputChannels->rpmAcceleration = engine->rpmCalculator.getRpmAcceleration();

	// Output both the estimated air flow, and measured air flow (if available)
	tsOutputChannels->mafMeasured = Sensor::getOrZero(SensorType::Maf);
	tsOutputChannels->mafMeasured2 = Sensor::getOrZero(SensorType::Maf2);
	tsOutputChannels->mafEstimate = engine->engineState.airflowEstimate;

	// offset 116
	// TPS acceleration
	tsOutputChannels->deltaTpsValue = engine->tpsAccelEnrichment.getMaxDelta();

	tsOutputChannels->totalTriggerErrorCounter = engine->triggerCentral.triggerState.totalTriggerErrorCounter;

	tsOutputChannels->orderingErrorCounter = engine->triggerCentral.triggerState.orderingErrorCounter;
#endif // EFI_SHAFT_POSITION_INPUT


	// 68
	// 140
#if EFI_ENGINE_CONTROL
	tsOutputChannels->injectorDutyCycle = getInjectorDutyCycle(rpm);
#endif

	// 224
	efitimesec_t timeSeconds = getTimeNowS();
	tsOutputChannels->seconds = timeSeconds;

	// 252
	tsOutputChannels->engineMode = packEngineMode();
	// 120
	tsOutputChannels->firmwareVersion = getRusEfiVersion();

	// 276
	tsOutputChannels->accelerationX = engine->sensors.accelerometer.x;
	// 278
	tsOutputChannels->accelerationY = engine->sensors.accelerometer.y;
	tsOutputChannels->accelerationZ = engine->sensors.accelerometer.z;
	tsOutputChannels->accelerationRoll = engine->sensors.accelerometer.roll;
	tsOutputChannels->accelerationYaw = engine->sensors.accelerometer.yaw;

#if EFI_DYNO_VIEW
	tsOutputChannels->VssAcceleration = getDynoviewAcceleration();
#endif

	tsOutputChannels->turboSpeed = Sensor::getOrZero(SensorType::TurbochargerSpeed);
	extern FrequencySensor inputShaftSpeedSensor;
	tsOutputChannels->issEdgeCounter = inputShaftSpeedSensor.eventCounter;
	extern FrequencySensor vehicleSpeedSensor;
	tsOutputChannels->vssEdgeCounter = vehicleSpeedSensor.eventCounter;

	tsOutputChannels->hasCriticalError = hasFirmwareError();

	tsOutputChannels->isWarnNow = engine->engineState.warnings.isWarningNow();

	tsOutputChannels->tpsAccelFuel = engine->engineState.tpsAccelEnrich;

	tsOutputChannels->checkEngine = hasErrorCodes();

#if EFI_MAX_31855
	for (int i = 0; i < EGT_CHANNEL_COUNT; i++)
		tsOutputChannels->egt[i] = getMax31855EgtValue(i);
#endif /* EFI_MAX_31855 */

#if EFI_IDLE_CONTROL
	tsOutputChannels->idleAirValvePosition = getIdlePosition();
#endif

	tsOutputChannels->warningCounter = engine->engineState.warnings.warningCounter;
	tsOutputChannels->lastErrorCode = engine->engineState.warnings.lastErrorCode;
	for (int i = 0; i < 8;i++) {
		tsOutputChannels->recentErrorCode[i] = engine->engineState.warnings.recentWarnings.get(i).Code;
	}

	tsOutputChannels->starterState = enginePins.starterControl.getLogicValue();
	tsOutputChannels->starterRelayDisable = enginePins.starterRelayDisable.getLogicValue();

	tsOutputChannels->mapFast = Sensor::getOrZero(SensorType::MapFast);


	tsOutputChannels->revolutionCounterSinceStart = engine->rpmCalculator.getRevolutionCounterSinceStart();
#if EFI_CAN_SUPPORT
	postCanState();
#endif /* EFI_CAN_SUPPORT */

#if EFI_CLOCK_LOCKS
	tsOutputChannels->maxLockedDuration = NT2US(maxLockedDuration);
#endif /* EFI_CLOCK_LOCKS */

#if EFI_SHAFT_POSITION_INPUT
	tsOutputChannels->maxTriggerReentrant = maxTriggerReentrant;
	tsOutputChannels->triggerPrimaryFall = engine->triggerCentral.getHwEventCounter((int)SHAFT_PRIMARY_FALLING);
	tsOutputChannels->triggerPrimaryRise = engine->triggerCentral.getHwEventCounter((int)SHAFT_PRIMARY_RISING);

	tsOutputChannels->triggerSecondaryFall = engine->triggerCentral.getHwEventCounter((int)SHAFT_SECONDARY_FALLING);
	tsOutputChannels->triggerSecondaryRise = engine->triggerCentral.getHwEventCounter((int)SHAFT_SECONDARY_RISING);

	tsOutputChannels->triggerVvtRise = engine->triggerCentral.vvtEventRiseCounter[0];
	tsOutputChannels->triggerVvtFall = engine->triggerCentral.vvtEventFallCounter[0];
#endif // EFI_SHAFT_POSITION_INPUT

#if HAL_USE_PAL && EFI_PROD_CODE
	tsOutputChannels->extiOverflowCount = getExtiOverflowCounter();
#endif

	switch (engineConfiguration->debugMode)	{
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
	case DBG_TRIGGER_COUNTERS:

#if EFI_SHAFT_POSITION_INPUT
		tsOutputChannels->debugIntField4 = engine->triggerCentral.triggerState.currentCycle.eventCount[0];
		tsOutputChannels->debugIntField5 = engine->triggerCentral.triggerState.currentCycle.eventCount[1];
#endif // EFI_SHAFT_POSITION_INPUT

		break;
#if EFI_MAP_AVERAGING
	case DBG_MAP:
		postMapState(tsOutputChannels);
		break;
#endif /* EFI_MAP_AVERAGING */
	case DBG_ANALOG_INPUTS:
		tsOutputChannels->debugFloatField4 = isAdcChannelValid(engineConfiguration->map.sensor.hwChannel) ? getVoltageDivided("map", engineConfiguration->map.sensor.hwChannel) : 0.0f;
		tsOutputChannels->debugFloatField7 = isAdcChannelValid(engineConfiguration->afr.hwChannel) ? getVoltageDivided("ego", engineConfiguration->afr.hwChannel) : 0.0f;
		break;
	case DBG_ANALOG_INPUTS2:
		updateTpsDebug();
		break;
	case DBG_INSTANT_RPM:
		{
#if EFI_SHAFT_POSITION_INPUT
			tsOutputChannels->debugFloatField2 = instantRpm / Sensor::getOrZero(SensorType::Rpm);
#endif // EFI_SHAFT_POSITION_INPUT

		}
		break;
	case DBG_TLE8888:
#if (BOARD_TLE8888_COUNT > 0)
		tle8888PostState();
#endif /* BOARD_TLE8888_COUNT */
		break;
	case DBG_LOGIC_ANALYZER: 
#if EFI_LOGIC_ANALYZER	
		reportLogicAnalyzerToTS();
#endif /* EFI_LOGIC_ANALYZER */		
		break;
	default:
		;
	}
}

#endif /* EFI_TUNER_STUDIO */

void initStatusLoop(void) {
	addConsoleActionI("warn", setWarningEnabled);
}

void startStatusThreads(void) {
	// todo: refactoring needed, this file should probably be split into pieces
#if EFI_PROD_CODE
	initStatusLeds();
	communicationsBlinkingTask.start();
#endif /* EFI_PROD_CODE */
}
