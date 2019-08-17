/*
 * boost_controller.cpp
 *
 *  Created on: 14. aug. 2019
 *      Author: Ola
 */


#include "global.h"
//#if EFI_BOOST_CONTROL
#include "engine.h"
#include "rpm_calculator.h"
#include "alternator_controller.h"
#include "voltage.h"
#include "pid.h"
#include "local_version_holder.h"
#include "periodic_task.h"
#include "engine_configuration.h"
#include "os_access.h"
#include "fsio_impl.h"
#include "settings.h"
#include "allsensors.h"
#include "rpm_calculator.h"
#include "efi_gpio.h"
#include "pwm_generator_logic.h"
#include "pwm_generator.h"
#include "pin_repository.h"
#include "tunerstudio_configuration.h"
#include "engine_math.h"






#if EFI_TUNER_STUDIO
extern TunerStudioOutputChannels tsOutputChannels;
#endif /* EFI_TUNER_STUDIO */
EXTERN_ENGINE
;
static Logging *logger;
static percent_t boostDuty;
static boost_table_t boostTable("Boost", 1);
static SimplePwm boostControl("boost");
static pid_s *bstPidS = &persistentState.persistentConfiguration.engineConfiguration.boostControl;
static Pid bstPid(bstPidS);
static bool shouldResetPid = false;
static void pidReset(void) {
    bstPid.reset();
}
class BoostController : public PeriodicTimerController {
    int getPeriodMs() override {
        return GET_PERIOD_LIMITED(&engineConfiguration->boostControl);
    }
    void PeriodicTask() override {
#if ! EFI_UNIT_TEST
        if (shouldResetPid) {
            pidReset();
            shouldResetPid = false;
        }
#endif
#if EFI_TUNER_STUDIO
            bstPid.postState(&tsOutputChannels);
#endif /* EFI_TUNER_STUDIO */


        bool boostShouldBeEnabledAtCurrentRpm = GET_RPM_VALUE > engineConfiguration->cranking.rpm;
        engine->isBoostControlEnabled = CONFIG(isBoostControlEnabled) && boostShouldBeEnabledAtCurrentRpm;
       if (!engine->isBoostControlEnabled) {
            // we need to avoid accumulating iTerm while engine is not running
            pidReset();
            return;
        }


        int rpm = GET_RPM();

        float boost = getMap(PASS_ENGINE_PARAMETER_SIGNATURE);
        float targetBoost = boostTable.getValue(rpm, getEngineLoadT(PASS_ENGINE_PARAMETER_SIGNATURE));
       float boostDuty = bstPid.getOutput(targetBoost, boost);
        boostControl.setSimplePwmDutyCycle(PERCENT_TO_DUTY(boostDuty));
    }
};
static BoostController instance;
void setBstPFactor(float p) {
    engineConfiguration->boostControl.pFactor = p;
    scheduleMsg(logger, "setBstPid: %.2f", p);
    pidReset();
//  showBstInfo();
}
static void applyBoostPinState(int stateIndex, PwmConfig *state) /* pwm_gen_callback */ {
 efiAssertVoid(CUSTOM_ERR_6643, stateIndex < PWM_PHASE_MAX_COUNT, "invalid stateIndex");
  efiAssertVoid(CUSTOM_IDLE_WAVE_CNT, state->multiWave.waveCount == 1, "invalid idle waveCount");
  OutputPin *output = state->outputPins[0];
  int value = state->multiWave.getChannelState(/*channelIndex*/0, stateIndex);
    /**
     * 'engine->isBoostControlEnabled' would be false is RPM is too low
     */
  if (!value || engine->isBoostControlEnabled)
     output->setValue(value);
}
void onConfigurationChangeBoostCallback(engine_configuration_s *previousConfiguration) {
    shouldResetPid = !bstPid.isSame(&previousConfiguration->boostControl);
}
void initBoostCtrl(Logging *sharedLogger) {
 logger = sharedLogger;
 // addConsoleAction("bstinfo", showBstInfo);
 if (CONFIGB(boostControlPin) == GPIO_UNASSIGNED)
     return;

 else {
     startSimplePwmExt(&boostControl,
             "Boost control",
             &engine->executor,
             CONFIGB(boostControlPin),
            &enginePins.boostPin,
              engineConfiguration->boostPwmFrequency, 0.1, (pwm_gen_callback*)applyBoostPinState);
  }
 instance.Start();
}
//#endif /* EFI_BOOST_CONTROL */
