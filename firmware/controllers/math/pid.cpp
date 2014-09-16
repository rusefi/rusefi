/**
 * @file pid.cpp
 *
 * http://en.wikipedia.org/wiki/PID_controller
 *
 * @date Sep 16, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "pid.h"

Pid::Pid(float pFactor, float iFactor, float dFactor) {
	this->pFactor = pFactor;
	this->iFactor = iFactor;
	this->dFactor = dFactor;

	integration = 0;
	prevError = 0;
}

float Pid::getValue(float target, float input, float dTime) {
	float error = target - input;

	float pTerm = pFactor * error;
	integration += iFactor * dTime * error;
	float dTerm = dFactor / dTime * (error - prevError);

	prevError = error;
	return pTerm + integration + dTerm;
}

