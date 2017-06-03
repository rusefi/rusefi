/**
 * @file pid.cpp
 *
 * https://en.wikipedia.org/wiki/Feedback
 * http://en.wikipedia.org/wiki/PID_controller
 *
 * @date Sep 16, 2014
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#include "pid.h"
#include "math.h"

Pid::Pid() {
    init(NULL);
}

Pid::Pid(pid_s *pid) {
	init(pid);
}

void Pid::init(pid_s *pid) {
	this->pid = pid;
	resetCounter = 0;

	reset();
}

bool Pid::isSame(pid_s *pid) {
	return this->pid->dFactor == pid->dFactor && this->pid->iFactor == pid->iFactor &&
			this->pid->offset == pid->offset && this->pid->pFactor == pid->pFactor;
}

float Pid::getValue(float target, float input) {
	return getValue(target, input, 1);
}

float Pid::getValue(float target, float input, float dTime) {
	float error = target - input;
	prevTarget = target;
	prevInput = input;

	float pTerm = pid->pFactor * error;
	iTerm += pid->iFactor * dTime * error;
	dTerm = pid->dFactor / dTime * (error - prevError);

	prevError = error;

	/**
	 * If we have exceeded the ability of the controlled device to hit target, the I factor will keep accumulating and approach infinity.
	 * Here we limit the I-term #353
	 */
	if (iTerm > pid->maxValue)
		iTerm = pid->maxValue;

	if (iTerm < pid->minValue)
		iTerm = pid->minValue;

	float result = pTerm + iTerm + dTerm + pid->offset;
	if (result > pid->maxValue) {
		result = pid->maxValue;
	} else if (result < pid->minValue) {
		result = pid->minValue;
	}
	prevResult = result;
	return result;
}

void Pid::updateFactors(float pFactor, float iFactor, float dFactor) {
	pid->pFactor = pFactor;
	pid->iFactor = iFactor;
	pid->dFactor = dFactor;
	reset();
}

void Pid::reset(void) {
	dTerm = iTerm = 0;
	prevResult = prevInput = prevTarget = prevError = 0;
	resetCounter++;
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
	tsOutputChannels->debugFloatField1 = prevResult;
	tsOutputChannels->debugFloatField2 = iTerm;
	tsOutputChannels->debugFloatField3 = getPrevError();
	tsOutputChannels->debugFloatField4 = getI();
	tsOutputChannels->debugFloatField5 = getD();
	tsOutputChannels->debugFloatField6 = pid->minValue;
	tsOutputChannels->debugFloatField7 = pid->maxValue;
	tsOutputChannels->debugIntField1 = getP();
	tsOutputChannels->debugIntField2 = getOffset();
	tsOutputChannels->debugIntField3 = resetCounter;
	tsOutputChannels->debugFloatField6 = dTerm;
}
#endif

void Pid::sleep() {
#if !EFI_UNIT_TEST || defined(__DOXYGEN__)
	int period = maxI(10, pid->period);
	chThdSleepMilliseconds(period);
#endif /* EFI_UNIT_TEST */
}

void Pid::showPidStatus(Logging *logging, const char*msg) {
	scheduleMsg(logging, "%s settings: offset=%d P=%.5f I=%.5f D=%.5f dT=%d",
			msg,
			pid->offset,
			pid->pFactor,
			pid->iFactor,
			pid->dFactor,
			pid->period);

	scheduleMsg(logging, "%s status: value=%f input=%f/target=%f iTerm=%.5f dTerm=%.5f",
			msg,
			prevResult,
			prevInput,
			prevTarget,
			iTerm, dTerm);

}
