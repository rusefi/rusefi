/**
 * @file	electronic_throttle.h
 *
 * @date Dec 7, 2013
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#pragma once

// https://en.wikipedia.org/wiki/Nyquist%E2%80%93Shannon_sampling_theorem
#define DEFAULT_ETB_LOOP_FREQUENCY 200
#define DEFAULT_ETB_PWM_FREQUENCY 300

#include "engine.h"
#include "periodic_task.h"

#ifndef ETB_COUNT
#define ETB_COUNT 2
#endif /* ETB_COUNT */

class DcMotor;

class EtbController final : public PeriodicTimerController {
public:
	DECLARE_ENGINE_PTR;
	void init(DcMotor *motor);

	int getPeriodMs() override;
	void PeriodicTask() override;
	Pid etbPid;
	bool shouldResetPid = false;

private:
    DcMotor *m_motor;
};

void initElectronicThrottle(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void setDefaultEtbBiasCurve(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void setDefaultEtbParameters(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void setBoschVNH2SP30Curve(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void setEtbPFactor(float value);
void setEtbIFactor(float value);
void setEtbDFactor(float value);
void setEtbOffset(int value);
void setThrottleDutyCycle(percent_t level);
bool isETBRestartNeeded(void);
void stopETBPins(void);
void startETBPins(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void onConfigurationChangeElectronicThrottleCallback(engine_configuration_s *previousConfiguration);
void unregisterEtbPins();
