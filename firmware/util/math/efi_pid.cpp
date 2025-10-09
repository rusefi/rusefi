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

#include "efi_pid.h"
#include "math.h"

Pid::Pid() {
	initPidClass(nullptr);
}

Pid::Pid(pid_s *p_parameters) {
	initPidClass(p_parameters);
}

void Pid::initPidClass(pid_s *p_parameters) {
	parameters = p_parameters;
	resetCounter = 0;

	Pid::reset();
}

static bool isClose(float a, float b) {
  bool result = (a == b);
  if (!result) {
      efiPrintf("PID: not same %f %f", a, b);
  }
  return result;
}

bool Pid::isSame(const pid_s *p_parameters) const {
	if (!parameters) {
		// this 'null' could happen on first execution during initialization
		return false;
	}
	efiAssert(ObdCode::OBD_PCM_Processor_Fault, p_parameters != nullptr, "PID::isSame nullptr", false);
	return isClose(parameters->pFactor, p_parameters->pFactor)
			&& isClose(parameters->iFactor, p_parameters->iFactor)
			&& isClose(parameters->dFactor, p_parameters->dFactor)
			&& isClose(parameters->offset, p_parameters->offset)
			&& isClose(parameters->periodMs, p_parameters->periodMs);
}

/**
 * @param Controller input / process output
 * @returns Output from the PID controller / the input to the process
 */
float Pid::getOutput(float p_target, float p_input) {
  efiAssert(ObdCode::OBD_PCM_Processor_Fault, parameters != nullptr, "PID::getOutput nullptr", 0);
	float dTime = MS2SEC(GET_PERIOD_LIMITED(parameters));
	return getOutput(p_target, p_input, dTime);
}

float Pid::getUnclampedOutput(float p_target, float p_input, float dTime) {
	target = p_target;
	input = p_input;
	float error = (target - input) * errorAmplificationCoef;

	float pTerm = parameters->pFactor * error;
	updateITerm(parameters->iFactor * dTime * error);
	dTerm = parameters->dFactor / dTime * (error - previousError);

	previousError = error;

	if (dTime <=0) {
		warning(ObdCode::CUSTOM_PID_DTERM, "PID: unexpected dTime");
		return pTerm + getOffset();
	}

	return pTerm + iTerm + dTerm + getOffset();
}

/**
 * @param dTime seconds probably? :)
 */
float Pid::getOutput(float p_target, float p_input, float dTime) {
	float l_output = getUnclampedOutput(p_target, p_input, dTime);

	if (l_output > parameters->maxValue) {
		l_output = parameters->maxValue;
	} else if (l_output < getMinValue()) {
		l_output = getMinValue();
	}
	output = l_output;
	return output;
}

void Pid::updateFactors(float pFactor, float iFactor, float dFactor) {
	parameters->pFactor = pFactor;
	parameters->iFactor = iFactor;
	parameters->dFactor = dFactor;
	reset();
}

void Pid::reset() {
	dTerm = iTerm = 0;
	output = input = target = previousError = 0;
	errorAmplificationCoef = 1.0f;
	resetCounter++;
}

float Pid::getP() const {
	return parameters->pFactor;
}

float Pid::getI() const {
	return parameters->iFactor;
}

float Pid::getPrevError() const {
	return previousError;
}

float Pid::getIntegration() const {
	return iTerm;
}

float Pid::getD() const {
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

void Pid::postState(pid_status_s& pidStatus) const {
	pidStatus.output = output;
	pidStatus.error = previousError;
	pidStatus.pTerm = parameters == nullptr ? 0 : parameters->pFactor * previousError;
	pidStatus.iTerm = iTerm;
	pidStatus.dTerm = dTerm;
	pidStatus.resetCounter = resetCounter;
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
	PidCic::reset();
}

PidCic::PidCic(pid_s *p_parameters) : Pid(p_parameters) {
	// call our derived reset()
	PidCic::reset();
}

void PidCic::reset(void) {
	Pid::reset();

	totalItermCnt = 0;
	for (int i = 0; i < PID_AVG_BUF_SIZE; i++)
		iTermBuf[i] = 0;
	iTermInvNum = 1.0f / (float)PID_AVG_BUF_SIZE;
}

float PidCic::getOutput(float p_target, float p_input, float dTime) {
	return getUnclampedOutput(p_target, p_input, dTime);
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

PidIndustrial::PidIndustrial(pid_s *p_parameters) : Pid(p_parameters) {
}

float PidIndustrial::getOutput(float p_target, float p_input, float dTime) {
	float ad, bd;
	float error = (p_target - p_input) * errorAmplificationCoef;
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
	float l_output = pTerm + iTerm + dTerm + getOffset();
	float limitedOutput = limitOutput(l_output);

	// apply the integrator anti-windup on top of the "normal" iTerm change above
	// If p.antiwindupFreq = 0, then iTerm is equal to PidParallelController's
	iTerm += dTime * antiwindupFreq * (limitedOutput - l_output);

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
