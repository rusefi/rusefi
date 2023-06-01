/*
 * @file vvt.cpp
 *
 * @date Jun 26, 2016
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "local_version_holder.h"
#include "vvt.h"

#define NO_PIN_PERIOD 500

#if defined(HAS_OS_ACCESS)
#error "Unexpected OS ACCESS HERE"
#endif /* HAS_OS_ACCESS */

using vvt_map_t = Map3D<SCRIPT_TABLE_8, SCRIPT_TABLE_8, int8_t, uint16_t, uint16_t>;

static vvt_map_t vvtTable1;
static vvt_map_t vvtTable2;

void VvtController::init(int index, int bankIndex, int camIndex, const ValueProvider3D* targetMap) {
	this->index = index;
	m_bank = bankIndex;
	m_cam = camIndex;

	// Use the same settings for the Nth cam in every bank (ie, all exhaust cams use the same PID)
	m_pid.initPidClass(&engineConfiguration->auxPid[camIndex]);

	m_targetMap = targetMap;
}

int VvtController::getPeriodMs() {
	return isBrainPinValid(engineConfiguration->vvtPins[index]) ?
		GET_PERIOD_LIMITED(&engineConfiguration->auxPid[index]) : NO_PIN_PERIOD;
}

void VvtController::PeriodicTask() {
	if (engine->auxParametersVersion.isOld(engine->getGlobalConfigurationVersion())) {
		m_pid.reset();
	}

	update();
}

expected<angle_t> VvtController::observePlant() const {
#if EFI_SHAFT_POSITION_INPUT
	return engine->triggerCentral.getVVTPosition(m_bank, m_cam);
#else
	return unexpected;
#endif // EFI_SHAFT_POSITION_INPUT
}

expected<angle_t> VvtController::getSetpoint() {
	int rpm = Sensor::getOrZero(SensorType::Rpm);
	float load = getFuelingLoad();
	float target = m_targetMap->getValue(rpm, load);

#if EFI_TUNER_STUDIO
	engine->outputChannels.vvtTargets[index] = target;
#endif

	return target;
}

expected<percent_t> VvtController::getOpenLoop(angle_t target) {
	// TODO: could we do VVT open loop?
	UNUSED(target);
	return 0;
}

static bool shouldInvertVvt(int camIndex) {
	// grumble grumble, can't do an array of bits in c++
	switch (camIndex) {
		case 0: return engineConfiguration->invertVvtControlIntake;
		case 1: return engineConfiguration->invertVvtControlExhaust;
	}

	return false;
}

expected<percent_t> VvtController::getClosedLoop(angle_t target, angle_t observation) {
	// User labels say "advance" and "retard"
	// "advance" means that additional solenoid duty makes indicated VVT position more positive
	// "retard" means that additional solenoid duty makes indicated VVT position more negative
	bool isInverted = shouldInvertVvt(m_cam);
	m_pid.setErrorAmplification(isInverted ? -1.0f : 1.0f);
	
	float retVal = m_pid.getOutput(target, observation);

#if EFI_TUNER_STUDIO
	m_pid.postState(engine->outputChannels.vvtStatus[index]);
#endif /* EFI_TUNER_STUDIO */

	return retVal;
}

void VvtController::setOutput(expected<percent_t> outputValue) {
	float rpm = Sensor::getOrZero(SensorType::Rpm);
#if EFI_SHAFT_POSITION_INPUT
	bool enabled = rpm > engineConfiguration->vvtControlMinRpm
			&& engine->rpmCalculator.getSecondsSinceEngineStart(getTimeNowNt()) > engineConfiguration->vvtActivationDelayMs / MS_PER_SECOND
			 ;

	if (outputValue && enabled) {
		m_pwm.setSimplePwmDutyCycle(PERCENT_TO_DUTY(outputValue.Value));
	} else {
		m_pwm.setSimplePwmDutyCycle(0);

		// we need to avoid accumulating iTerm while engine is not running
		m_pid.reset();
	}
#endif // EFI_SHAFT_POSITION_INPUT
}

#if EFI_AUX_PID

static const char *vvtOutputNames[CAM_INPUTS_COUNT] = {
"Vvt Output#1",
#if CAM_INPUTS_COUNT > 1
"Vvt Output#2",
#endif
#if CAM_INPUTS_COUNT > 2
"Vvt Output#3",
#endif
#if CAM_INPUTS_COUNT > 3
"Vvt Output#4",
#endif
 };


static VvtController instances[CAM_INPUTS_COUNT];

static void turnVvtPidOn(int index) {
	if (!isBrainPinValid(engineConfiguration->vvtPins[index])) {
		return;
	}

	startSimplePwmExt(&instances[index].m_pwm, vvtOutputNames[index],
			&engine->executor,
			engineConfiguration->vvtPins[index],
			&instances[index].m_pin,
			// todo: do we need two separate frequencies?
			engineConfiguration->vvtOutputFrequency[0], 0.1);
}

void startVvtControlPins() {
	for (int i = 0;i <CAM_INPUTS_COUNT;i++) {
		turnVvtPidOn(i);
	}
}

void stopVvtControlPins() {
	for (int i = 0;i < CAM_INPUTS_COUNT;i++) {
		instances[i].m_pin.deInit();
	}
}

void initVvtActuators() {
	if (engineConfiguration->vvtControlMinRpm < engineConfiguration->cranking.rpm) {
		engineConfiguration->vvtControlMinRpm = engineConfiguration->cranking.rpm;
	}

	vvtTable1.init(config->vvtTable1, config->vvtTable1LoadBins,
			config->vvtTable1RpmBins);
	vvtTable2.init(config->vvtTable2, config->vvtTable2LoadBins,
			config->vvtTable2RpmBins);

	for (int i = 0;i < CAM_INPUTS_COUNT;i++) {

		int camIndex = i % CAMS_PER_BANK;
		int bankIndex = i / CAMS_PER_BANK;
		auto targetMap = camIndex == 0 ? &vvtTable1 : &vvtTable2;
		instances[i].init(i, bankIndex, camIndex, targetMap);
	}

	startVvtControlPins();

	for (int i = 0;i < CAM_INPUTS_COUNT;i++) {
		instances[i].start();
	}
}

#endif
