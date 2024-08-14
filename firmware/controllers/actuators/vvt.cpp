/*
 * @file vvt.cpp
 *
 * @date Jun 26, 2016
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "local_version_holder.h"
#include "vvt.h"
#include "bench_test.h"

#define NO_PIN_PERIOD 500

using vvt_map_t = Map3D<SCRIPT_TABLE_8, SCRIPT_TABLE_8, int8_t, uint16_t, uint16_t>;

static vvt_map_t vvtTable1{"vvt1"};
static vvt_map_t vvtTable2{"vvt2"};

VvtController::VvtController(int p_index)
	: index(p_index)
	, m_bank(BANK_BY_INDEX(p_index))
	, m_cam(CAM_BY_INDEX(p_index))
{
}

void VvtController::init(const ValueProvider3D* targetMap, IPwm* pwm) {
	// Use the same settings for the Nth cam in every bank (ie, all exhaust cams use the same PID)
	m_pid.initPidClass(&engineConfiguration->auxPid[m_cam]);

	m_targetMap = targetMap;
	m_pwm = pwm;
}

void VvtController::onFastCallback() {
	if (!m_pwm || !m_targetMap) {
		// not init yet
		return;
	}

	update();
}

void VvtController::onConfigurationChange(engine_configuration_s const * previousConfig) {
	if (!previousConfig || !m_pid.isSame(&previousConfig->auxPid[m_cam])) {
		m_pid.reset();
	}
}

expected<angle_t> VvtController::observePlant() {
#if EFI_SHAFT_POSITION_INPUT
	return engine->triggerCentral.getVVTPosition(m_bank, m_cam);
#else
	return unexpected;
#endif // EFI_SHAFT_POSITION_INPUT
}

expected<angle_t> VvtController::getSetpoint() {
	int rpm = Sensor::getOrZero(SensorType::Rpm);
	bool enabled = rpm > engineConfiguration->vvtControlMinRpm
			&& engine->rpmCalculator.getSecondsSinceEngineStart(getTimeNowNt()) > engineConfiguration->vvtActivationDelayMs / MS_PER_SECOND
			 ;
	if (!enabled) {
		return unexpected;
    }

	float load = getFuelingLoad();
	float target = m_targetMap->getValue(rpm, load);

#if EFI_TUNER_STUDIO
	engine->outputChannels.vvtTargets[index] = target;
#endif

	vvtTarget = target;

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
#if EFI_SHAFT_POSITION_INPUT
	vvtOutput = outputValue.value_or(0);

	if (outputValue) {
		m_pwm->setSimplePwmDutyCycle(PERCENT_TO_DUTY(outputValue.Value));
	} else {
		m_pwm->setSimplePwmDutyCycle(0);

		// we need to avoid accumulating iTerm while engine is not running
		m_pid.reset();
	}
#endif // EFI_SHAFT_POSITION_INPUT
}

#if EFI_VVT_PID

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

static OutputPin vvtPins[CAM_INPUTS_COUNT];
static SimplePwm vvtPwms[CAM_INPUTS_COUNT] = { "VVT1", "VVT2", "VVT3", "VVT4" };

OutputPin* getVvtOutputPin(int index) {
    return &vvtPins[index];
}

static void applyVvtPinState(int stateIndex, PwmConfig *state) /* pwm_gen_callback */ {
    OutputPin *output = state->outputPins[0];
    if (output == getOutputOnTheBenchTest()) {
        return;
    }
    state->applyPwmValue(output, stateIndex);
}

static void turnVvtPidOn(int index) {
	if (!isBrainPinValid(engineConfiguration->vvtPins[index])) {
		return;
	}

	startSimplePwmExt(&vvtPwms[index], vvtOutputNames[index],
			&engine->executor,
			engineConfiguration->vvtPins[index],
			getVvtOutputPin(index),
			engineConfiguration->vvtOutputFrequency, 0.1,
			applyVvtPinState);
}

void startVvtControlPins() {
	for (int i = 0;i <CAM_INPUTS_COUNT;i++) {
		turnVvtPidOn(i);
	}
}

void stopVvtControlPins() {
	for (int i = 0;i < CAM_INPUTS_COUNT;i++) {
		getVvtOutputPin(i)->deInit();
	}
}

void initVvtActuators() {
	if (engineConfiguration->vvtControlMinRpm < engineConfiguration->cranking.rpm) {
		engineConfiguration->vvtControlMinRpm = engineConfiguration->cranking.rpm;
	}

	vvtTable1.initTable(config->vvtTable1, config->vvtTable1RpmBins, config->vvtTable1LoadBins);
	vvtTable2.initTable(config->vvtTable2, config->vvtTable2RpmBins, config->vvtTable2LoadBins);


	engine->module<VvtController1>()->init(&vvtTable1, &vvtPwms[0]);
	engine->module<VvtController2>()->init(&vvtTable2, &vvtPwms[1]);
	engine->module<VvtController3>()->init(&vvtTable1, &vvtPwms[2]);
	engine->module<VvtController4>()->init(&vvtTable2, &vvtPwms[3]);

	startVvtControlPins();
}

#endif
