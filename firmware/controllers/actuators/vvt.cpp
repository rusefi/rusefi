/*
 * @file vvt.cpp
 *
 * @date Jun 26, 2016
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "local_version_holder.h"
#include "vvt.h"

#include "fsio_impl.h"

#define NO_PIN_PERIOD 500

#if defined(HAS_OS_ACCESS)
#error "Unexpected OS ACCESS HERE"
#endif /* HAS_OS_ACCESS */

static fsio8_Map3D_u8t vvtTable1;
static fsio8_Map3D_u8t vvtTable2;

void VvtController::init(int index, int bankIndex, int camIndex, const ValueProvider3D* targetMap) {
	this->index = index;
	m_bank = bankIndex;
	m_cam = camIndex;

	// Use the same settings for the Nth cam in every bank (ie, all exhaust cams use the same PID)
	m_pid.initPidClass(&CONFIG(auxPid[camIndex]));

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
	return engine->triggerCentral.getVVTPosition(m_bank, m_cam);
}

expected<angle_t> VvtController::getSetpoint() const {
	int rpm = GET_RPM();
	float load = getFuelingLoad();
	float target = m_targetMap->getValue(rpm, load);

#if EFI_TUNER_STUDIO
	tsOutputChannels.vvtTargets[index] = target;
#endif

	return target;
}

expected<percent_t> VvtController::getOpenLoop(angle_t target) const {
	// TODO: could we do VVT open loop?
	UNUSED(target);
	return 0;
}

expected<percent_t> VvtController::getClosedLoop(angle_t target, angle_t observation) {
	float retVal = m_pid.getOutput(target, observation);

	if (engineConfiguration->isVerboseAuxPid1) {
		efiPrintf("aux duty: %.2f/value=%.2f/p=%.2f/i=%.2f/d=%.2f int=%.2f", retVal, observation,
				m_pid.getP(), m_pid.getI(), m_pid.getD(), m_pid.getIntegration());
	}

#if EFI_TUNER_STUDIO
	static debug_mode_e debugModeByIndex[4] = {DBG_VVT_1_PID, DBG_VVT_2_PID, DBG_VVT_3_PID, DBG_VVT_4_PID};

	if (engineConfiguration->debugMode == debugModeByIndex[index]) {
		m_pid.postState(&tsOutputChannels);
		tsOutputChannels.debugIntField3 = (int)(10 * target);
	}
#endif /* EFI_TUNER_STUDIO */

	return retVal;
}

void VvtController::setOutput(expected<percent_t> outputValue) {
	float rpm = GET_RPM();

	// todo: make this configurable?
	bool enabledAtCurrentRpm = rpm > engineConfiguration->cranking.rpm;

	if (outputValue && enabledAtCurrentRpm) {
		m_pwm.setSimplePwmDutyCycle(PERCENT_TO_DUTY(outputValue.Value));
	} else {
		m_pwm.setSimplePwmDutyCycle(0);

		// we need to avoid accumulating iTerm while engine is not running
		m_pid.reset();
	}
}

#if EFI_AUX_PID

static VvtController instances[CAM_INPUTS_COUNT] CCM_OPTIONAL;

static void turnAuxPidOn(int index) {
	if (!isBrainPinValid(engineConfiguration->vvtPins[index])) {
		return;
	}

	startSimplePwmExt(&instances[index].m_pwm, "Aux PID",
			&engine->executor,
			engineConfiguration->vvtPins[index],
			&instances[index].m_pin,
			// todo: do we need two separate frequencies?
			engineConfiguration->vvtOutputFrequency[0], 0.1);
}

void startVvtControlPins() {
	for (int i = 0;i <CAM_INPUTS_COUNT;i++) {
		turnAuxPidOn(i);
	}
}

void stopVvtControlPins() {
	for (int i = 0;i < CAM_INPUTS_COUNT;i++) {
		instances[i].m_pin.deInit();
	}
}

void initAuxPid() {

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
		instances[i].Start();
	}
}

#endif
