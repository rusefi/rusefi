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

#if EFI_PROD_CODE || EFI_SIMULATOR
#include "tunerstudio_configuration.h"
#endif

// See PidCic below
#define PID_AVG_BUF_SIZE_SHIFT 5
#define PID_AVG_BUF_SIZE (1<<PID_AVG_BUF_SIZE_SHIFT) // 32*sizeof(float)

#define NOT_TIME_BASED_PID 1

// minimal period 5m meaning maximum control frequency 200Hz
#define PID_MINIMAL_PERIOD_MS 5

#define GET_PERIOD_LIMITED(pid_s_ptr) maxI(PID_MINIMAL_PERIOD_MS, ((pid_s_ptr)->periodMs))

#define MS2SEC(x) (x * 0.001)

class Pid {

public:
	Pid();
	explicit Pid(pid_s *pid);
	void initPidClass(pid_s *pid);
	bool isSame(pid_s *pid) const;

	/**
	 * This version of the method takes dTime from pid_s
	 *
	 * @param Controller input / process output
	 * @returns Output from the PID controller / the input to the process
	 */
	float getOutput(float target, float input);
	virtual float getOutput(float target, float input, float dTime);
	// doesn't limit the result (used in incremental CIC PID, see below)
	float getUnclampedOutput(float target, float input, float dTime);
	void updateFactors(float pFactor, float iFactor, float dFactor);
	virtual void reset(void);
	float getP(void) const;
	float getI(void) const;
	float getD(void) const;
	float getOffset(void) const;
	float getIntegration(void) const;
	float getPrevError(void) const;
	void setErrorAmplification(float coef);
#if EFI_TUNER_STUDIO
	void postState(TunerStudioOutputChannels *tsOutputChannels);
	void postState(TunerStudioOutputChannels *tsOutputChannels, int pMult);
#endif /* EFI_TUNER_STUDIO */
	float minResult;
	float maxResult;
	float iTerm;
	float dTerm; // we are remembering this only for debugging purposes
	void showPidStatus(Logging *logging, const char*msg);
	void sleep();
	int resetCounter;
	// todo: move this to pid_s one day
	float iTermMin = -1000000.0;
	float iTermMax =  1000000.0;
private:
	pid_s *pid;

	float previousError;
	// these are only used for logging
	float target;
	float input;
	float output;
	float errorAmplificationCoef;

private:
	virtual void updateITerm(float value);
};


/**
 * A PID implementation with a modified cascaded integrator-comb (CIC) filtering.
 * Used for incremental auto-IAC control. See autoIdle() in idle_thread.cpp
 * See pid_cic.md.
 *
 * https://rusefi.com/forum/viewtopic.php?f=9&t=1315
 */
class PidCic : public Pid {

public:
	PidCic();
	explicit PidCic(pid_s *pid);

	void reset(void) override;
	using Pid::getOutput;
	float getOutput(float target, float input, float dTime) override;
	
private:
	// Circular running-average buffer for I-term, used by CIC-like filter
	float iTermBuf[PID_AVG_BUF_SIZE];
	// Needed by averaging (smoothing) of iTerm sums
	float iTermInvNum;
	// Total PID iterations (>240 days max. for 10ms update period)
	int totalItermCnt;

private:
	void updateITerm(float value) override;
};

#endif /* PID_H_ */
