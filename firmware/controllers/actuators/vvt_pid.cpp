/*
 * @file vvt_pid.cpp
 *
 * This class is a copy-paste of alternator_controller.cpp TODO: do something about it? extract more common logic?
 *
 * @date Jun 26, 2016
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "local_version_holder.h"
#include "allsensors.h"
#include "vvt_pid.h"

#if EFI_AUX_PID
#include "pwm_generator_logic.h"
#include "tunerstudio_outputs.h"
#include "fsio_impl.h"
#include "engine_math.h"
#include "pin_repository.h"
#include "periodic_task.h"

#define NO_PIN_PERIOD 500

#if defined(HAS_OS_ACCESS)
#error "Unexpected OS ACCESS HERE"
#endif /* HAS_OS_ACCESS */

EXTERN_ENGINE;

static fsio8_Map3D_u8t vvtTable1("vvt#1");
static fsio8_Map3D_u8t vvtTable2("vvt#2");

static Logging *logger;

void VvtController::init(int index) {
	this->index = index;
	m_pid.initPidClass(&persistentState.persistentConfiguration.engineConfiguration.auxPid[index]);
	int camIndex = index % CAMS_PER_BANK;
	table = camIndex == 0 ? &vvtTable1 : &vvtTable2;
}

int VvtController::getPeriodMs() override {
	return isBrainPinValid(engineConfiguration->auxPidPins[index]) ?
		GET_PERIOD_LIMITED(&engineConfiguration->auxPid[index]) : NO_PIN_PERIOD;
}

void VvtController::PeriodicTask() override {
	if (engine->auxParametersVersion.isOld(engine->getGlobalConfigurationVersion())) {
		auxPid.reset();
	}

	update();
}

expected<angle_t> VvtController::observePlant() const {
	return engine->triggerCentral.getVVTPosition();
}

expected<angle_t> VvtController::getSetpoint() const {
	int rpm = GET_RPM();
	float load = getFuelingLoad(PASS_ENGINE_PARAMETER_SIGNATURE);
	return m_targetMap->getValue(rpm, load);
}

expected<percent_t> VvtController::getOpenLoop(angle_t target) const {
	// TODO: could we do VVT open loop?
	return 0;
}

expected<percent_t> VvtController::getClosedLoop(angle_t setpoint, angle_t observation) {
	float retVal = m_pid.getOutput(setpoint, observation);

	if (engineConfiguration->isVerboseAuxPid1) {
		scheduleMsg(logger, "aux duty: %.2f/value=%.2f/p=%.2f/i=%.2f/d=%.2f int=%.2f", retVal, observation,
				auxPid.getP(), auxPid.getI(), auxPid.getD(), auxPid.getIntegration());
	}

#if EFI_TUNER_STUDIO
	if (engineConfiguration->debugMode == DBG_AUX_PID_1) {
		m_pid.postState(&tsOutputChannels);
		tsOutputChannels.debugIntField3 = (int)(10 * setpoint);
	}
#endif /* EFI_TUNER_STUDIO */

	return retVal;
}

void VvtController::setOutput(expected<percent_t> outputValue) {
	float rpm = GET_RPM();

	// todo: make this configurable?
	bool enabledAtCurrentRpm = rpm > engineConfiguration->cranking.rpm;

	if (enabledAtCurrentRpm) {
		m_pid->setSimplePwmDutyCycle(PERCENT_TO_DUTY(pwm));
	} else {
		m_pid->setSimplePwmDutyCycle(0);

		// we need to avoid accumulating iTerm while engine is not running
		m_pid.reset();
	}
}

static VvtController instances[CAM_INPUTS_COUNT];

static void turnAuxPidOn(int index) {
	if (!isBrainPinValid(engineConfiguration->auxPidPins[index])) {
		return;
	}

	startSimplePwmExt(&instances[index].m_pwm, "Aux PID",
			&engine->executor,
			engineConfiguration->auxPidPins[index],
			&instances[index].m_pin,
			// todo: do we need two separate frequencies?
			engineConfiguration->auxPidFrequency[0], 0.1);
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

void initAuxPid(Logging *sharedLogger) {

	vvtTable1.init(config->vvtTable1, config->vvtTable1LoadBins,
			config->vvtTable1RpmBins);
	vvtTable2.init(config->vvtTable2, config->vvtTable2LoadBins,
			config->vvtTable2RpmBins);


	logger = sharedLogger;

	for (int i = 0;i < CAM_INPUTS_COUNT;i++) {
		instances[i].init(i);
	}

	startVvtControlPins();

	for (int i = 0;i < CAM_INPUTS_COUNT;i++) {
		instances[i].Start();
	}
}

#endif
