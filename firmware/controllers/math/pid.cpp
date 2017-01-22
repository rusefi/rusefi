/**
 * @file pid.cpp
 *
 * http://en.wikipedia.org/wiki/PID_controller
 *
 * @date Sep 16, 2014
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#include "pid.h"
#include "math.h"

Pid::Pid() {
    init(NULL, NAN, NAN);
}

Pid::Pid(pid_s *pid, float minResult, float maxResult) {
	init(pid, minResult, maxResult);
}

void Pid::init(pid_s *pid, float minResult, float maxResult) {
	this->pid = pid;
	this->minResult = minResult;
	this->maxResult = maxResult;

	dTerm = iTerm = 0;
	prevError = 0;
}

bool Pid::isSame(pid_s *pid) {
	return this->pid->dFactor == pid->dFactor && this->pid->iFactor == pid->iFactor  &&
			this->pid->offset == pid->offset && this->pid->pFactor == pid->pFactor;
}

float Pid::getValue(float target, float input) {
	return getValue(target, input, 1);
}

float Pid::getValue(float target, float input, float dTime) {
	float error = target - input;

	float pTerm = pid->pFactor * error;
	iTerm += pid->iFactor * dTime * error;
	dTerm = pid->dFactor / dTime * (error - prevError);

	prevError = error;

	float result = pTerm + iTerm + dTerm + pid->offset;
	if (result > maxResult) {
//		iTerm -= result - maxResult;
		result = maxResult;
	} else if (result < minResult) {
//		iTerm += minResult - result;
		result = minResult;
	}
	return result;
}

void Pid::updateFactors(float pFactor, float iFactor, float dFactor) {
	pid->pFactor = pFactor;
	pid->iFactor = iFactor;
	pid->dFactor = dFactor;
	reset();
}

void Pid::reset(void) {
	iTerm = 0;
	prevError = 0;
}

float Pid::getP(void) {
	return pid->pFactor;
}

float Pid::getI(void) {
	return pid->iFactor;
}

float Pid::getPrevError(void) {
	return prevError;
}

float Pid::getIntegration(void) {
	return iTerm;
}

float Pid::getD(void) {
	return pid->dFactor;
}

float Pid::getOffset(void) {
	return pid->offset;
}

#if EFI_PROD_CODE || EFI_SIMULATOR
void Pid::postState(TunerStudioOutputChannels *tsOutputChannels) {
	tsOutputChannels->debugFloatField2 = iTerm;
	tsOutputChannels->debugFloatField3 = getPrevError();
	tsOutputChannels->debugFloatField4 = getI();
	tsOutputChannels->debugFloatField5 = getD();
	tsOutputChannels->debugIntField1 = getP();
	tsOutputChannels->debugIntField2 = getOffset();
	tsOutputChannels->debugFloatField6 = dTerm;
}
#endif
