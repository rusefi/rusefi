/**
 * @file pid.h
 *
 * @date Sep 16, 2014
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef PID_H_
#define PID_H_

#include "global.h"
#include "engine_configuration_generated_structures.h"
#include "datalogging.h"

#if EFI_PROD_CODE || EFI_SIMULATOR
#include "tunerstudio_configuration.h"
#endif

// See PidCic below
#define PID_AVG_BUF_SIZE_SHIFT 5
#define PID_AVG_BUF_SIZE (1<<PID_AVG_BUF_SIZE_SHIFT) // 32*sizeof(float)

class Pid {

public:
	Pid();
	Pid(pid_s *pid);
	void init(pid_s *pid);
	bool isSame(pid_s *pid);

	float getValue(float target, float input);
	// todo: dTime should be taken from pid_s
	virtual float getValue(float target, float input, float dTime);
	// doesn't limit the result (used in incremental CIC PID, see below)
	float getRawValue(float target, float input, float dTime);
	void updateFactors(float pFactor, float iFactor, float dFactor);
	virtual void reset(void);
	float getP(void);
	float getI(void);
	float getD(void);
	float getOffset(void);
	float getIntegration(void);
	float getPrevError(void);
	void setErrorAmplification(float coef);
#if EFI_PROD_CODE || EFI_SIMULATOR
	void postState(TunerStudioOutputChannels *tsOutputChannels);
	void postState(TunerStudioOutputChannels *tsOutputChannels, int pMult);
#endif
	float minResult;
	float maxResult;
	float iTerm;
	float dTerm; // we are remembering this only for debugging purposes
	void showPidStatus(Logging *logging, const char*msg);
	void sleep();
	int resetCounter;
private:
	pid_s *pid;

	float prevError;
	// these are only used for logging
	float prevTarget;
	float prevInput;
	float prevResult;
	float errorAmplificationCoef;

private:
	virtual void updateITerm(float value);
};


/**
 * A PID impl. with a modified cascaded integrator-comb (CIC) filtering.
 * Used for incremental auto-IAC control. See autoIdle() in idle_thread.cpp
 *
 * https://rusefi.com/forum/viewtopic.php?f=9&t=1315
 */
class PidCic : public Pid {

public:
	PidCic();
	PidCic(pid_s *pid);

	virtual void reset(void);
	virtual float getValue(float target, float input, float dTime);
	
private:
	// Circular running-average buffer for I-term, used by CIC-like filter
	float iTermBuf[PID_AVG_BUF_SIZE];
	// Needed by averaging (smoothing) of iTerm sums
	float iTermInvNum;
	// Total PID iterations (>240 days max. for 10ms update period)
	int totalItermCnt;

private:
	virtual void updateITerm(float value);
};

#endif /* PID_H_ */
