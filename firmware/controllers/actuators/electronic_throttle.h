/**
 * @file	electronic_throttle.h
 *
 * @date Dec 7, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef ELECTRONIC_THROTTLE_H_
#define ELECTRONIC_THROTTLE_H_

// https://en.wikipedia.org/wiki/Nyquist%E2%80%93Shannon_sampling_theorem
#define DEFAULT_ETB_LOOP_FREQUENCY 200
#define DEFAULT_ETB_PWM_FREQUENCY 300

#include "engine.h"
void initElectronicThrottle(void);
void setDefaultEtbBiasCurve(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void setDefaultEtbParameters(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void setEtbPFactor(float value);
void setEtbIFactor(float value);
void setEtbDFactor(float value);
void setEtbOffset(int value);
void setThrottleDutyCycle(float level);
bool isETBRestartNeeded(void);
void stopETBPins(void);
void startETBPins(void);
void onConfigurationChangeElectronicThrottleCallback(engine_configuration_s *previousConfiguration);
void unregisterEtbPins();

#endif /* ELECTRONIC_THROTTLE_H_ */
