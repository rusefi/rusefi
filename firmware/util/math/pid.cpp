/**
 * @file pid.cpp
 *
 * https://en.wikipedia.org/wiki/Feedback
 * http://en.wikipedia.org/wiki/PID_controller
 *
 * @date Sep 16, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "os_access.h"
#include "pid.h"
#include "math.h"

Pid::Pid() {
	initPidClass(nullptr);
}

Pid::Pid(pid_s *parameters) {
	initPidClass(parameters);
}

void Pid::initPidClass(pid_s *parameters) {
	this->parameters = parameters;
	resetCounter = 0;

	reset();
}

bool Pid::isSame(const pid_s *parameters) const {
	if (!this->parameters) {
		// this 'null' could happen on first execution during initialization
		return false;
	}
	efiAssert(OBD_PCM_Processor_Fault, parameters != NULL, "PID::isSame NULL", false);
	return this->parameters->pFactor == parameters->pFactor
			&& this->parameters->iFactor == parameters->iFactor
			&& this->parameters->dFactor == parameters->dFactor
			&& this->parameters->offset == parameters->offset
			&& this->parameters->periodMs == parameters->periodMs;
}

/**
 * @param Controller input / process output
 * @returns Output from the PID controller / the input to the process
 */
float Pid::getOutput(float target, float input) {
	float dTime = MS2SEC(GET_PERIOD_LIMITED(parameters));
	return getOutput(target, input, dTime);
}

float Pid::getUnclampedOutput(float target, float input, float dTime) {
	float error = (target - input) * errorAmplificationCoef;
	this->target = target;
	this->input = input;
	if (dTime == 0) {
		firmwareError(OBD_PCM_Processor_Fault, "zero dTime");
	}

	float pTerm = parameters->pFactor * error;
	updateITerm(parameters->iFactor * dTime * error);
	dTerm = parameters->dFactor / dTime * (error - previousError);

	previousError = error;

	return pTerm + iTerm + dTerm + getOffset();
}

/**
 * @param dTime seconds probably? :)
 */
float Pid::getOutput(float target, float input, float dTime) {
	float output = getUnclampedOutput(target, input, dTime);

	if (output > parameters->maxValue) {
		output = parameters->maxValue;
	} else if (output < getMinValue()) {
		output = getMinValue();
	}
	this->output = output;
	return output;
}

void Pid::updateFactors(float pFactor, float iFactor, float dFactor) {
	parameters->pFactor = pFactor;
	parameters->iFactor = iFactor;
	parameters->dFactor = dFactor;
	reset();
}

void Pid::reset(void) {
	dTerm = iTerm = 0;
	output = input = target = previousError = 0;
	errorAmplificationCoef = 1.0f;
	resetCounter++;
}

float Pid::getP(void) const {
	return parameters->pFactor;
}

float Pid::getI(void) const {
	return parameters->iFactor;
}

float Pid::getPrevError(void) const {
	return previousError;
}

float Pid::getIntegration(void) const {
	return iTerm;
}

float Pid::getD(void) const {
	return parameters->dFactor;
}

float Pid::getOffset(void) const {
	return parameters->offset;
}

float Pid::getMinValue(void) const {
	return parameters->minValue;
}

void Pid::setErrorAmplification(float coef) {
	errorAmplificationCoef = coef;
}

#if EFI_TUNER_STUDIO
void Pid::postState(TunerStudioOutputChannels *tsOutputChannels) const {
	postState(tsOutputChannels, 1);
}

/**
 * see https://rusefi.com/wiki/index.php?title=Manual:Debug_fields
 */
void Pid::postState(TunerStudioOutputChannels *tsOutputChannels, int pMult) const {
	tsOutputChannels->debugFloatField1 = output;
	tsOutputChannels->debugFloatField2 = iTerm;
	tsOutputChannels->debugFloatField3 = getPrevError();
	tsOutputChannels->debugFloatField4 = getI();
	tsOutputChannels->debugFloatField5 = getD();
	tsOutputChannels->debugFloatField6 = dTerm;
//	tsOutputChannels->debugFloatField6 = parameters->minValue;
	tsOutputChannels->debugFloatField7 = parameters->maxValue;
	tsOutputChannels->debugIntField1 = getP() * pMult;
	tsOutputChannels->debugIntField2 = getOffset();
	tsOutputChannels->debugIntField3 = resetCounter;
	tsOutputChannels->debugIntField4 = parameters->periodMs;
}
#endif /* EFI_TUNER_STUDIO */

void Pid::sleep() {
#if !EFI_UNIT_TEST
	int periodMs = maxI(10, parameters->periodMs);
	chThdSleepMilliseconds(periodMs);
#endif /* EFI_UNIT_TEST */
}

void Pid::showPidStatus(const char*msg) const {
	efiPrintf("%s settings: offset=%f P=%.5f I=%.5f D=%.5f period=%dms",
			msg,
			getOffset(),
			parameters->pFactor,
			parameters->iFactor,
			parameters->dFactor,
			parameters->periodMs);

	efiPrintf("%s status: value=%.2f input=%.2f/target=%.2f iTerm=%.5f dTerm=%.5f",
			msg,
			output,
			input,
			target,
			iTerm, dTerm);

}

void Pid::updateITerm(float value) {
	iTerm += value;
	/**
	 * If we have exceeded the ability of the controlled device to hit target, the I factor will keep accumulating and approach infinity.
	 * Here we limit the I-term #353
	 */
	if (iTerm > parameters->maxValue * 100) {
		iTerm = parameters->maxValue * 100;
	}
	if (iTerm > iTermMax) {
		iTerm = iTermMax;
	}

	// this is kind of a hack. a proper fix would be having separate additional settings 'maxIValue' and 'minIValye'
	if (iTerm < -parameters->maxValue * 100)
		iTerm = -parameters->maxValue * 100;
	if (iTerm < iTermMin) {
		iTerm = iTermMin;
	}
}


PidCic::PidCic() {
	// call our derived reset()
	reset();
}

PidCic::PidCic(pid_s *parameters) : Pid(parameters) {
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

float PidCic::getOutput(float target, float input, float dTime) {
	return getUnclampedOutput(target, input, dTime);
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

PidIndustrial::PidIndustrial() : Pid() {
}

PidIndustrial::PidIndustrial(pid_s *parameters) : Pid(parameters) {
}

float PidIndustrial::getOutput(float target, float input, float dTime) {
	float ad, bd;
	float error = (target - input) * errorAmplificationCoef;
	float pTerm = parameters->pFactor * error;

	// calculate dTerm coefficients
	if (fabsf(derivativeFilterLoss) > DBL_EPSILON) {
		// restore Td in the Standard form from the Parallel form: Td = Kd / Kc
		float Td = parameters->dFactor / parameters->pFactor;
		// calculate the backward differences approximation of the derivative term
		ad = Td / (Td + dTime / derivativeFilterLoss);
		bd = parameters->pFactor * ad / derivativeFilterLoss;
	} else {
		// According to the Theory of limits, if p.derivativeFilterLoss -> 0, then 
		//   lim(ad) = 0; lim(bd) = p.pFactor * Td / dTime = p.dFactor / dTime
		//   i.e. dTerm becomes equal to Pid's
		ad = 0.0f;
		bd = parameters->dFactor / dTime;
	}
	
	// (error - previousError) = (target-input) - (target-prevousInput) = -(input - prevousInput)
	dTerm = dTerm * ad + (error - previousError) * bd;

	updateITerm(parameters->iFactor * dTime * error);

	// calculate output and apply the limits
	float output = pTerm + iTerm + dTerm + getOffset();
	float limitedOutput = limitOutput(output);

	// apply the integrator anti-windup on top of the "normal" iTerm change above
	// If p.antiwindupFreq = 0, then iTerm is equal to PidParallelController's
	iTerm += dTime * antiwindupFreq * (limitedOutput - output);
	
	// update the state
	previousError = error;
	
	return limitedOutput;
}

float PidIndustrial::limitOutput(float v) const {
	if (v < getMinValue())
		v = getMinValue();
	if (v > parameters->maxValue)
		v = parameters->maxValue;
	return v;
}
