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


// todo: this is to some extent a copy-paste of alternator_controller. maybe same loop
// for all PIDs?

static Logging *logger;

class AuxPidController : public PeriodicTimerController {
public:

	SimplePwm auxPidPwm;
	OutputPin auxOutputPin;

	void init(int index) {
		this->index = index;
		auxPid.initPidClass(&persistentState.persistentConfiguration.engineConfiguration.auxPid[index]);
		int camIndex = index % CAMS_PER_BANK;
		table = camIndex == 0 ? &vvtTable1 : &vvtTable2;
	}

	int getPeriodMs() override {
		return isBrainPinValid(engineConfiguration->auxPidPins[index]) ?
			GET_PERIOD_LIMITED(&engineConfiguration->auxPid[index]) : NO_PIN_PERIOD;
	}

	void PeriodicTask() override {
			if (engine->auxParametersVersion.isOld(engine->getGlobalConfigurationVersion())) {
				auxPid.reset();

			}

			float rpm = GET_RPM();

			// todo: make this configurable?
			bool enabledAtCurrentRpm = rpm > engineConfiguration->cranking.rpm;

			if (!enabledAtCurrentRpm) {
				// we need to avoid accumulating iTerm while engine is not running
				auxPid.reset();
				return;
			}


			float value = engine->triggerCentral.getVVTPosition();
			float targetValue = table->getValue(rpm, getFuelingLoad(PASS_ENGINE_PARAMETER_SIGNATURE));

			percent_t pwm = auxPid.getOutput(targetValue, value);
			if (engineConfiguration->isVerboseAuxPid1) {
				scheduleMsg(logger, "aux duty: %.2f/value=%.2f/p=%.2f/i=%.2f/d=%.2f int=%.2f", pwm, value,
						auxPid.getP(), auxPid.getI(), auxPid.getD(), auxPid.getIntegration());
			}


			if (engineConfiguration->debugMode == DBG_AUX_PID_1) {
#if EFI_TUNER_STUDIO
				auxPid.postState(&tsOutputChannels);
				tsOutputChannels.debugIntField3 = (int)(10 * targetValue);
#endif /* EFI_TUNER_STUDIO */
			}

			auxPidPwm.setSimplePwmDutyCycle(PERCENT_TO_DUTY(pwm));
		}
private:
	Pid auxPid;
	int index = 0;
	ValueProvider3D *table = nullptr;
};

static AuxPidController instances[CAM_INPUTS_COUNT];

static void turnAuxPidOn(int index) {
	if (!isBrainPinValid(engineConfiguration->auxPidPins[index])) {
		return;
	}

	startSimplePwmExt(&instances[index].auxPidPwm, "Aux PID",
			&engine->executor,
			engineConfiguration->auxPidPins[index],
			&instances[index].auxOutputPin,
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
		instances[i].auxOutputPin.deInit();
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
