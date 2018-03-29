/**
 * @file pid.cpp
 *
 * https://en.wikipedia.org/wiki/Feedback
 * http://en.wikipedia.org/wiki/PID_controller
 *
 * @date Sep 16, 2014
 * @author Andrey Belomutskiy, (c) 2012-2018
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
	return this->pid->pFactor == pid->pFactor
			&& this->pid->iFactor == pid->iFactor
			&& this->pid->dFactor == pid->dFactor
			&& this->pid->offset == pid->offset
			&& this->pid->period == pid->period;
}

float Pid::getValue(float target, float input) {
	return getValue(target, input, 1);
}

float Pid::getRawValue(float target, float input, float dTime) {
	float error = (target - input) * errorAmplificationCoef;
	prevTarget = target;
	prevInput = input;

	float pTerm = pid->pFactor * error;
	updateITerm(pid->iFactor * dTime * error);
	dTerm = pid->dFactor / dTime * (error - prevError);

	prevError = error;

	return pTerm + iTerm + dTerm + pid->offset;
}

float Pid::getValue(float target, float input, float dTime) {
	float result = getRawValue(target, input, dTime);

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
	errorAmplificationCoef = 1.0f;
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

void Pid::setErrorAmplification(float coef) {
	errorAmplificationCoef = coef;
}

#if EFI_PROD_CODE || EFI_SIMULATOR
void Pid::postState(TunerStudioOutputChannels *tsOutputChannels) {
	postState(tsOutputChannels, 1);
}

/**
 * see https://rusefi.com/wiki/index.php?title=Manual:Debug_fields
 */
void Pid::postState(TunerStudioOutputChannels *tsOutputChannels, int pMult) {
	tsOutputChannels->debugFloatField1 = prevResult;
	tsOutputChannels->debugFloatField2 = iTerm;
	tsOutputChannels->debugFloatField3 = getPrevError();
	tsOutputChannels->debugFloatField4 = getI();
	tsOutputChannels->debugFloatField5 = getD();
//	tsOutputChannels->debugFloatField6 = pid->minValue;
	tsOutputChannels->debugFloatField7 = pid->maxValue;
	tsOutputChannels->debugIntField1 = getP() * pMult;
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

	scheduleMsg(logging, "%s status: value=%.2f input=%.2f/target=%.2f iTerm=%.5f dTerm=%.5f",
			msg,
			prevResult,
			prevInput,
			prevTarget,
			iTerm, dTerm);

}

void Pid::updateITerm(float value) {
	iTerm += value;
	/**
	 * If we have exceeded the ability of the controlled device to hit target, the I factor will keep accumulating and approach infinity.
	 * Here we limit the I-term #353
	 */
	if (iTerm > pid->maxValue * 100)
		iTerm = pid->maxValue * 100;

	// this is kind of a hack. a proper fix would be having separate additional settings 'maxIValue' and 'minIValye'
	if (iTerm < -pid->maxValue * 100)
		iTerm = -pid->maxValue * 100;
}


PidCic::PidCic() {
	// call our derived reset()
	reset();
}

PidCic::PidCic(pid_s *pid) : Pid(pid) {
	// call our derived reset()
	reset();
}

void PidCic::reset(void) {
	Pid::reset();

	totalItermCnt = 0;
	for (int i = 0; i < PID_AVG_BUF_SIZE; i++)
		iTermBuf[i] = 0;
	iTermInvNum = 1.0f / (float)PID_AVG_BUF_SIZE;
}

float PidCic::getValue(float target, float input, float dTime) {
	return getRawValue(target, input, dTime);
}

void PidCic::updateITerm(float value) {
	// use a variation of cascaded integrator-comb (CIC) filtering to get non-overflow iTerm
	totalItermCnt++;
	int localBufPos = (totalItermCnt >> PID_AVG_BUF_SIZE_SHIFT) % PID_AVG_BUF_SIZE;
	int localPrevBufPos = ((totalItermCnt - 1) >> PID_AVG_BUF_SIZE_SHIFT) % PID_AVG_BUF_SIZE;
	
	// reset old buffer cell
	if (localPrevBufPos != localBufPos)
		iTermBuf[localBufPos] = 0;
	// integrator stage
	iTermBuf[localBufPos] += value;
	
	// return moving average of all sums, to smoothen the result
	float iTermSum = 0;
	for (int i = 0; i < PID_AVG_BUF_SIZE; i++) {
		iTermSum += iTermBuf[i];
	}
	iTerm = iTermSum * iTermInvNum;
}
