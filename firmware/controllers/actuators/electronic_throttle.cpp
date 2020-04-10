/**
 * @file	electronic_throttle.cpp
 * @brief	Electronic Throttle driver
 *
 * @see test test_etb.cpp
 *
 *
 * Limited user documentation at https://github.com/rusefi/rusefi_documentation/wiki/HOWTO_electronic_throttle_body
 *
 * todo: make this more universal if/when we get other hardware options
 *
 * Sep 2019 two-wire TLE9201 official driving around the block! https://www.youtube.com/watch?v=1vCeICQnbzI
 * May 2019 two-wire TLE7209 now behaves same as three-wire VNH2SP30 "eBay red board" on BOSCH 0280750009
 * Apr 2019 two-wire TLE7209 support added
 * Mar 2019 best results so far achieved with three-wire H-bridges like VNH2SP30 on BOSCH 0280750009
 * Jan 2019 actually driven around the block but still need some work.
 * Jan 2017 status:
 * Electronic throttle body with it's spring is definitely not linear - both P and I factors of PID are required to get any results
 *  PID implementation tested on a bench only
 *  it is believed that more than just PID would be needed, as is this is probably
 *  not usable on a real vehicle. Needs to be tested :)
 *
 * https://raw.githubusercontent.com/wiki/rusefi/rusefi_documentation/oem_docs/VAG/Bosch_0280750009_pinout.jpg
 *
 *  ETB is controlled according to pedal position input (pedal position sensor is a potentiometer)
 *    pedal 0% means pedal not pressed / idle
 *    pedal 100% means pedal all the way down
 *  (not TPS - not the one you can calibrate in TunerStudio)
 *
 *
 * See also pid.cpp
 *
 * Relevant console commands:
 *
 * ETB_BENCH_ENGINE
 * set engine_type 58
 *
 * enable verbose_etb
 * disable verbose_etb
 * ethinfo
 * set mock_pedal_position X
 *
 *
 * set debug_mode 17
 * for PID outputs
 *
 * set etb_p X
 * set etb_i X
 * set etb_d X
 * set etb_o X
 *
 * set_etb_duty X
 *
 * http://rusefi.com/forum/viewtopic.php?f=5&t=592
 *
 * @date Dec 7, 2013
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
 */

#include "global.h"

#if EFI_ELECTRONIC_THROTTLE_BODY

#include "electronic_throttle.h"
#include "tps.h"
#include "sensor.h"
#include "dc_motor.h"
#include "dc_motors.h"
#include "pid_auto_tune.h"

#if defined(HAS_OS_ACCESS)
#error "Unexpected OS ACCESS HERE"
#endif

#ifndef ETB_MAX_COUNT
#define ETB_MAX_COUNT 2
#endif /* ETB_MAX_COUNT */

static pid_s tuneWorkingPidSettings;
static Pid tuneWorkingPid(&tuneWorkingPidSettings);
static PID_AutoTune autoTune;

static LoggingWithStorage logger("ETB");
static pedal2tps_t pedal2tpsMap("Pedal2Tps", 1);

EXTERN_ENGINE;

static bool startupPositionError = false;

#define STARTUP_NEUTRAL_POSITION_ERROR_THRESHOLD 5

static SensorType indexToTpsSensor(size_t index) {
	switch(index) {
		case 0:  return SensorType::Tps1;
		default: return SensorType::Tps2;
	}
}

static percent_t directPwmValue = NAN;
static percent_t currentEtbDuty;

#define ETB_DUTY_LIMIT 0.9
// this macro clamps both positive and negative percentages from about -100% to 100%
#define ETB_PERCENT_TO_DUTY(X) (maxF(minF((X * 0.01), ETB_DUTY_LIMIT - 0.01), 0.01 - ETB_DUTY_LIMIT))

void EtbController::init(DcMotor *motor, int ownIndex, pid_s *pidParameters) {
	m_motor = motor;
	m_myIndex = ownIndex;
	m_pid.initPidClass(pidParameters);
}

void EtbController::reset() {
	m_shouldResetPid = true;
}

void EtbController::onConfigurationChange(pid_s* previousConfiguration) {
	if (m_motor && m_pid.isSame(previousConfiguration)) {
		m_shouldResetPid = true;
	}
}

void EtbController::showStatus(Logging* logger) {
	m_pid.showPidStatus(logger, "ETB");
}

int EtbController::getPeriodMs() {
	return GET_PERIOD_LIMITED(&engineConfiguration->etb);
}

void EtbController::PeriodicTask() {
#if EFI_TUNER_STUDIO
	// Only debug throttle #0
	if (m_myIndex == 0) {
		// set debug_mode 17
		if (engineConfiguration->debugMode == DBG_ELECTRONIC_THROTTLE_PID) {
			m_pid.postState(&tsOutputChannels);
			tsOutputChannels.debugIntField5 = engine->engineState.etbFeedForward;
		} else if (engineConfiguration->debugMode == DBG_ELECTRONIC_THROTTLE_EXTRA) {
			// set debug_mode 29
			tsOutputChannels.debugFloatField1 = directPwmValue;
		}
	}
#endif /* EFI_TUNER_STUDIO */

	if (!m_motor) {
		return;
	}

	if (startupPositionError) {
		m_motor->set(0);
		return;
	}

	if (m_shouldResetPid) {
		m_pid.reset();
		m_shouldResetPid = false;
	}

	if (!cisnan(directPwmValue)) {
		m_motor->set(directPwmValue);
		return;
	}

	if (engineConfiguration->pauseEtbControl) {
		m_motor->set(0);
		return;
	}

	auto pedalPosition = Sensor::get(SensorType::AcceleratorPedal);

	if (!pedalPosition.Valid) {
		m_motor->set(0);
		return;
	}

	SensorResult actualThrottlePosition = Sensor::get(indexToTpsSensor(m_myIndex));

	if (!actualThrottlePosition.Valid) {
		m_motor->set(0);
		return;
	}

	float rpm = GET_RPM();
	engine->engineState.targetFromTable = pedal2tpsMap.getValue(rpm / RPM_1_BYTE_PACKING_MULT, pedalPosition.Value);
	percent_t etbIdleAddition = CONFIG(useETBforIdleControl) ? engine->engineState.idle.etbIdleAddition : 0;
	percent_t targetPosition = engine->engineState.targetFromTable + etbIdleAddition;

	if (engineConfiguration->debugMode == DBG_ETB_LOGIC) {
#if EFI_TUNER_STUDIO
		tsOutputChannels.debugFloatField1 = engine->engineState.targetFromTable;
		tsOutputChannels.debugFloatField2 = engine->engineState.idle.etbIdleAddition;
#endif /* EFI_TUNER_STUDIO */
	}

	if (cisnan(targetPosition)) {
		// this could happen while changing settings
		warning(CUSTOM_ERR_ETB_TARGET, "target");
		return;
	}
	engine->engineState.etbFeedForward = interpolate2d("etbb", targetPosition, engineConfiguration->etbBiasBins, engineConfiguration->etbBiasValues);

	m_pid.iTermMin = engineConfiguration->etb_iTermMin;
	m_pid.iTermMax = engineConfiguration->etb_iTermMax;

	float closedLoop;

	if (engine->etbAutoTune) {
		bool isPositive = actualThrottlePosition.Value > targetPosition;

		float autotuneAmplitude = 15;

		// Bang-bang control the output to induce oscillation
		closedLoop = autotuneAmplitude * isPositive ? -1 : 1;

		// End of cycle - record & reset
		if (!isPositive && m_lastIsPositive) {
			efitick_t now = getTimeNowNt();

			// Determine period
			efitick_t cycleTime = now - m_cycleStartTime;
			m_cycleStartTime = now;

			// Determine amplitude
			float a = m_maxCycleTps - m_minCycleTps;

			// Reset bounds
			m_minCycleTps = 100;
			m_maxCycleTps = 0;


			// Math is for Åström–Hägglund (relay) auto tuning
			// https://warwick.ac.uk/fac/cross_fac/iatl/reinvention/archive/volume5issue2/hornsey

			// Publish to TS state
			if (engineConfiguration->debugMode == DBG_ETB_AUTOTUNE) {
				// a - amplitude of output
				tsOutputChannels.debugFloatField1 = a;
				float b = 2 * autotuneAmplitude;
				// b - amplitude of input
				tsOutputChannels.debugFloatField2 = b;
				// Tu - oscillation period
				float tu = NT2US((float)cycleTime) / 1e6;
				tsOutputChannels.debugFloatField3 = tu;

				// Ku
				float ku = 4 * b / (3.14159f * a);
				tsOutputChannels.debugFloatField4 = ku;
				// Kp
				tsOutputChannels.debugFloatField5 = 0.35f * ku;
				// Ki
				tsOutputChannels.debugFloatField6 = 0.25f * ku / tu;
				// Kd
				tsOutputChannels.debugFloatField7 = 0.08f * ku * tu;
			}
		}

		m_lastIsPositive = isPositive;

		// Find the min/max of each cycle
		if (actualThrottlePosition.Value < m_minCycleTps) {
			m_minCycleTps = actualThrottlePosition.Value;
		}

		if (actualThrottlePosition.Value > m_maxCycleTps) {
			m_maxCycleTps = actualThrottlePosition.Value;
		}
	} else {
		// Normal case - use PID to compute closed loop part
		closedLoop = m_pid.getOutput(targetPosition, actualThrottlePosition.Value);
	}

	currentEtbDuty = engine->engineState.etbFeedForward + closedLoop;

	m_motor->set(ETB_PERCENT_TO_DUTY(currentEtbDuty));

	if (engineConfiguration->isVerboseETB) {
		m_pid.showPidStatus(&logger, "ETB");
	}

	DISPLAY_STATE(Engine)
	DISPLAY_TEXT(Electronic_Throttle);
	DISPLAY_SENSOR(TPS)
	DISPLAY_TEXT(eol);

	DISPLAY_TEXT(Pedal);
	DISPLAY_SENSOR(PPS);
	DISPLAY(DISPLAY_CONFIG(throttlePedalPositionAdcChannel));
	DISPLAY_TEXT(eol);

	DISPLAY_TEXT(Feed_forward);
	DISPLAY(DISPLAY_FIELD(etbFeedForward));
	DISPLAY_TEXT(eol);

	DISPLAY_STATE(ETB_pid)
	DISPLAY_TEXT(input);
	DISPLAY(DISPLAY_FIELD(input));
	DISPLAY_TEXT(Output);
	DISPLAY(DISPLAY_FIELD(output));
	DISPLAY_TEXT(iTerm);
	DISPLAY(DISPLAY_FIELD(iTerm));
	DISPLAY_TEXT(eol);
	DISPLAY(DISPLAY_FIELD(errorAmplificationCoef));
	DISPLAY(DISPLAY_FIELD(previousError));
	DISPLAY_TEXT(eol);

	DISPLAY_TEXT(Settings);
	DISPLAY(DISPLAY_CONFIG(ETB_PFACTOR));
	DISPLAY(DISPLAY_CONFIG(ETB_IFACTOR));
	DISPLAY(DISPLAY_CONFIG(ETB_DFACTOR));
	DISPLAY_TEXT(eol);
	DISPLAY(DISPLAY_CONFIG(ETB_OFFSET));
	DISPLAY(DISPLAY_CONFIG(ETB_PERIODMS));
	DISPLAY_TEXT(eol);
	DISPLAY(DISPLAY_CONFIG(ETB_MINVALUE));
	DISPLAY(DISPLAY_CONFIG(ETB_MAXVALUE));
/* DISPLAY_ELSE */
	DISPLAY_TEXT(No_Pedal_Sensor);
/* DISPLAY_ENDIF */

	// Only report the 0th throttle
	if (m_myIndex == 0) {
#if EFI_TUNER_STUDIO
		// 312
		tsOutputChannels.etbTarget = targetPosition;
		// 316
		tsOutputChannels.etb1DutyCycle = currentEtbDuty;
		// 320
		// Error is positive if the throttle needs to open further
		tsOutputChannels.etb1Error = targetPosition - actualThrottlePosition.Value;
#endif /* EFI_TUNER_STUDIO */
	}
}

// real implementation (we mock for some unit tests)
EtbController etbControllers[ETB_COUNT];

static void showEthInfo(void) {
#if EFI_PROD_CODE
	if (engine->etbActualCount == 0) {
		scheduleMsg(&logger, "ETB DISABLED since no PPS");
	}

	scheduleMsg(&logger, "etbAutoTune=%d",
			engine->etbAutoTune);

	scheduleMsg(&logger, "TPS=%.2f", Sensor::get(SensorType::Tps1).value_or(0));


	scheduleMsg(&logger, "etbControlPin1=%s duty=%.2f freq=%d",
			hwPortname(CONFIG(etbIo[0].controlPin1)),
			currentEtbDuty,
			engineConfiguration->etbFreq);
	scheduleMsg(&logger, "dir1=%s", hwPortname(CONFIG(etbIo[0].directionPin1)));
	scheduleMsg(&logger, "dir2=%s", hwPortname(CONFIG(etbIo[0].directionPin2)));

	showDcMotorInfo(&logger);

#endif /* EFI_PROD_CODE */
}

static void etbPidReset(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	for (int i = 0 ; i < engine->etbActualCount; i++) {
		engine->etbControllers[i]->reset();
	}
}

#if !EFI_UNIT_TEST

/**
 * At the moment there are TWO ways to use this
 * set_etb_duty X
 * set etb X
 * manual duty cycle control without PID. Percent value from 0 to 100
 */
void setThrottleDutyCycle(percent_t level) {
	scheduleMsg(&logger, "setting ETB duty=%f%%", level);
	if (cisnan(level)) {
		directPwmValue = NAN;
		return;
	}

	float dc = ETB_PERCENT_TO_DUTY(level);
	directPwmValue = dc;
	for (int i = 0 ; i < engine->etbActualCount; i++) {
		setDcMotorDuty(i, dc);
	}
	scheduleMsg(&logger, "duty ETB duty=%f", dc);
}

static void setEtbFrequency(int frequency) {
	engineConfiguration->etbFreq = frequency;

	for (int i = 0 ; i < engine->etbActualCount; i++) {
		setDcMotorFrequency(i, frequency);
	}
}

static void etbReset() {
	scheduleMsg(&logger, "etbReset");
	
	for (int i = 0 ; i < engine->etbActualCount; i++) {
		setDcMotorDuty(i, 0);
	}

	etbPidReset();
}
#endif /* EFI_PROD_CODE */

#if !EFI_UNIT_TEST
/**
 * set etb_p X
 */
void setEtbPFactor(float value) {
	engineConfiguration->etb.pFactor = value;
	etbPidReset();
	showEthInfo();
}

/**
 * set etb_i X
 */
void setEtbIFactor(float value) {
	engineConfiguration->etb.iFactor = value;
	etbPidReset();
	showEthInfo();
}

/**
 * set etb_d X
 */
void setEtbDFactor(float value) {
	engineConfiguration->etb.dFactor = value;
	etbPidReset();
	showEthInfo();
}

/**
 * set etb_o X
 */
void setEtbOffset(int value) {
	engineConfiguration->etb.offset = value;
	etbPidReset();
	showEthInfo();
}

#endif /* EFI_UNIT_TEST */

void setBoschVNH2SP30Curve(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	engineConfiguration->etbBiasBins[0] = 0;
	engineConfiguration->etbBiasBins[1] = 1;
	engineConfiguration->etbBiasBins[2] = 5;
	/**
	 * This specific throttle has default position of about 7% open
	 */
	engineConfiguration->etbBiasBins[3] = 7;
	engineConfiguration->etbBiasBins[4] = 14;
	engineConfiguration->etbBiasBins[5] = 65;
	engineConfiguration->etbBiasBins[6] = 66;
	engineConfiguration->etbBiasBins[7] = 100;

	/**
	 * Some negative bias for below-default position
	 */
	engineConfiguration->etbBiasValues[0] = -15;
	engineConfiguration->etbBiasValues[1] = -15;
	engineConfiguration->etbBiasValues[2] = -10;
	/**
	 * Zero bias for index which corresponds to default throttle position, when no current is applied
	 * This specific throttle has default position of about 7% open
	 */
	engineConfiguration->etbBiasValues[3] = 0;
	engineConfiguration->etbBiasValues[4] = 19;
	engineConfiguration->etbBiasValues[5] = 20;
	engineConfiguration->etbBiasValues[6] = 26;
	engineConfiguration->etbBiasValues[7] = 28;
}

void setDefaultEtbParameters(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	CONFIG(etbIdleThrottleRange) = 5;

	setLinearCurve(config->pedalToTpsPedalBins, /*from*/0, /*to*/100, 1);
	setLinearCurve(config->pedalToTpsRpmBins, /*from*/0, /*to*/8000 / RPM_1_BYTE_PACKING_MULT, 1);

	for (int pedalIndex = 0;pedalIndex<PEDAL_TO_TPS_SIZE;pedalIndex++) {
		for (int rpmIndex = 0;rpmIndex<PEDAL_TO_TPS_SIZE;rpmIndex++) {
			config->pedalToTpsTable[pedalIndex][rpmIndex] = config->pedalToTpsPedalBins[pedalIndex];
		}
	}


	engineConfiguration->throttlePedalUpVoltage = 0; // that's voltage, not ADC like with TPS
	engineConfiguration->throttlePedalWOTVoltage = 6; // that's voltage, not ADC like with TPS

	engineConfiguration->etb.pFactor = 1;
	engineConfiguration->etb.iFactor = 0.05;
	engineConfiguration->etb.dFactor = 0.0;
	engineConfiguration->etb.periodMs = (1000 / DEFAULT_ETB_LOOP_FREQUENCY);
	engineConfiguration->etbFreq = DEFAULT_ETB_PWM_FREQUENCY;
	engineConfiguration->etb_iTermMin = -300;
	engineConfiguration->etb_iTermMax = 300;

	// values are above 100% since we have feedforward part of the total summation
	engineConfiguration->etb.minValue = -200;
	engineConfiguration->etb.maxValue = 200;
}

void onConfigurationChangeElectronicThrottleCallback(engine_configuration_s *previousConfiguration) {
	for (int i = 0; i < ETB_COUNT; i++) {
		etbControllers[i].onConfigurationChange(&previousConfiguration->etb);
	}
}

#if EFI_PROD_CODE && 0
static void setTempOutput(float value) {
	autoTune.output = value;
}

/**
 * set_etbat_step X
 */
static void setAutoStep(float value) {
	autoTune.reset();
	autoTune.SetOutputStep(value);
}

static void setAutoPeriod(int period) {
	tuneWorkingPidSettings.periodMs = period;
	autoTune.reset();
}

static void setAutoOffset(int offset) {
	tuneWorkingPidSettings.offset = offset;
	autoTune.reset();
}
#endif /* EFI_PROD_CODE */

void setDefaultEtbBiasCurve(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	engineConfiguration->etbBiasBins[0] = 0;
	engineConfiguration->etbBiasBins[1] = 1;
	engineConfiguration->etbBiasBins[2] = 2;
	/**
	 * This specific throttle has default position of about 4% open
	 */
	engineConfiguration->etbBiasBins[3] = 4;
	engineConfiguration->etbBiasBins[4] = 7;
	engineConfiguration->etbBiasBins[5] = 98;
	engineConfiguration->etbBiasBins[6] = 99;
	engineConfiguration->etbBiasBins[7] = 100;

	/**
	 * Some negative bias for below-default position
	 */
	engineConfiguration->etbBiasValues[0] = -20;
	engineConfiguration->etbBiasValues[1] = -18;
	engineConfiguration->etbBiasValues[2] = -17;
	/**
	 * Zero bias for index which corresponds to default throttle position, when no current is applied
	 * This specific throttle has default position of about 4% open
	 */
	engineConfiguration->etbBiasValues[3] = 0;
	engineConfiguration->etbBiasValues[4] = 20;
	engineConfiguration->etbBiasValues[5] = 21;
	engineConfiguration->etbBiasValues[6] = 22;
	engineConfiguration->etbBiasValues[7] = 25;
}

void unregisterEtbPins() {
	// todo: we probably need an implementation here?!
}

void doInitElectronicThrottle(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	efiAssertVoid(OBD_PCM_Processor_Fault, engine->etbControllers != NULL, "etbControllers NULL");
#if EFI_PROD_CODE
	addConsoleAction("ethinfo", showEthInfo);
	addConsoleAction("etbreset", etbReset);
	addConsoleActionI("etb_freq", setEtbFrequency);
#endif /* EFI_PROD_CODE */

	// If you don't have a pedal, we have no business here.
	if (!Sensor::hasSensor(SensorType::AcceleratorPedal)) {
		return;
	}

	engine->etbActualCount = Sensor::hasSensor(SensorType::Tps2) ? 2 : 1;

	for (int i = 0 ; i < engine->etbActualCount; i++) {
		auto motor = initDcMotor(i PASS_ENGINE_PARAMETER_SUFFIX);

		// If this motor is actually set up, init the etb
		if (motor)
		{
			engine->etbControllers[i]->init(motor, i, &engineConfiguration->etb);
			INJECT_ENGINE_REFERENCE(engine->etbControllers[i]);
		}
	}

	pedal2tpsMap.init(config->pedalToTpsTable, config->pedalToTpsPedalBins, config->pedalToTpsRpmBins);

#if 0 && ! EFI_UNIT_TEST
	percent_t startupThrottlePosition = getTPS(PASS_ENGINE_PARAMETER_SIGNATURE);
	if (absF(startupThrottlePosition - engineConfiguration->etbNeutralPosition) > STARTUP_NEUTRAL_POSITION_ERROR_THRESHOLD) {
		/**
		 * Unexpected electronic throttle start-up position is worth a critical error
		 */
		firmwareError(OBD_Throttle_Actuator_Control_Range_Performance_Bank_1, "startup ETB position %.2f not %d",
				startupThrottlePosition,
				engineConfiguration->etbNeutralPosition);
		startupPositionError = true;
	}
#endif /* EFI_UNIT_TEST */

#if EFI_PROD_CODE
	if (engineConfiguration->etbCalibrationOnStart) {

		for (int i = 0 ; i < engine->etbActualCount; i++) {
			setDcMotorDuty(i, 70);
			chThdSleep(600);
			// todo: grab with proper index
			grabTPSIsWideOpen();
			setDcMotorDuty(i, -70);
			chThdSleep(600);
			// todo: grab with proper index
			grabTPSIsClosed();
		}
	}

	// manual duty cycle control without PID. Percent value from 0 to 100
	addConsoleActionNANF(CMD_ETB_DUTY, setThrottleDutyCycle);
#endif /* EFI_PROD_CODE */

#if EFI_PROD_CODE && 0
	tuneWorkingPidSettings.pFactor = 1;
	tuneWorkingPidSettings.iFactor = 0;
	tuneWorkingPidSettings.dFactor = 0;
//	tuneWorkingPidSettings.offset = 10; // todo: not hard-coded value
	//todo tuneWorkingPidSettings.periodMs = 10;
	tuneWorkingPidSettings.minValue = 0;
	tuneWorkingPidSettings.maxValue = 100;
	tuneWorkingPidSettings.periodMs = 100;

	// this is useful once you do "enable etb_auto"
	addConsoleActionF("set_etbat_output", setTempOutput);
	addConsoleActionF("set_etbat_step", setAutoStep);
	addConsoleActionI("set_etbat_period", setAutoPeriod);
	addConsoleActionI("set_etbat_offset", setAutoOffset);
#endif /* EFI_PROD_CODE */

	etbPidReset(PASS_ENGINE_PARAMETER_SIGNATURE);

	for (int i = 0 ; i < engine->etbActualCount; i++) {
		engine->etbControllers[i]->Start();
	}
}

void initElectronicThrottle(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	if (hasFirmwareError()) {
		return;
	}

	for (int i = 0; i < ETB_COUNT; i++) {
		engine->etbControllers[i] = &etbControllers[i];
	}

	doInitElectronicThrottle(PASS_ENGINE_PARAMETER_SIGNATURE);
}

#endif /* EFI_ELECTRONIC_THROTTLE_BODY */
