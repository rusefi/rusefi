/**
 * @file pid.h
 *
 * @date Sep 16, 2014
 * @author Andrey Belomutskiy, (c) 2012-2016
 */

#ifndef PID_H_
#define PID_H_

#include "engine_configuration_generated_structures.h"

class Pid {

public:
	Pid(pid_s *pid, float minResult, float maxResult);
	float getValue(float target, float input, float dTime);
	void updateFactors(float pFactor, float iFactor, float dFactor);
	void reset(void);
	float getP(void);
	float getI(void);
	float getIntegration(void);
	float getD(void);
private:
	pid_s *pid;
	float minResult;
	float maxResult;

	float iTerm;
	float prevError;
};

#endif /* PID_H_ */
