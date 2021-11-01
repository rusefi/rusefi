/**
 * @file pid.h
 *
 * @date Sep 16, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "pid_state_generated.h"

// See PidCic below
#define PID_AVG_BUF_SIZE_SHIFT 5
#define PID_AVG_BUF_SIZE (1<<PID_AVG_BUF_SIZE_SHIFT) // 32*sizeof(float)

#define NOT_TIME_BASED_PID 1

// minimal period 5m meaning maximum control frequency 200Hz
#define PID_MINIMAL_PERIOD_MS 5

#define GET_PERIOD_LIMITED(pid_s_ptr) maxI(PID_MINIMAL_PERIOD_MS, ((pid_s_ptr)->periodMs))

#define MS2SEC(x) (x * 0.001)

struct pid_s;

/**
 * default basic implementation also known as PidParallelController
 */
class Pid : public pid_state_s {

public:
	DECLARE_ENGINE_PTR;

	Pid();
	explicit Pid(pid_s *parameters);
	void initPidClass(pid_s *parameters);
	bool isSame(const pid_s *parameters) const;

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
	float getOffset() const;
	float getMinValue() const;
	float getIntegration(void) const;
	float getPrevError(void) const;
	void setErrorAmplification(float coef);
#if EFI_TUNER_STUDIO
	void postState(TunerStudioOutputChannels *tsOutputChannels) const;
	void postState(TunerStudioOutputChannels *tsOutputChannels, int pMult) const;
#endif /* EFI_TUNER_STUDIO */
	void showPidStatus(const char* msg) const;
	void sleep();
	int resetCounter;
	// todo: move this to pid_s one day
	float iTermMin = -1000000.0;
	float iTermMax =  1000000.0;
protected:
	pid_s *parameters;
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

/**
 * A PID with derivative filtering (backward differences) and integrator anti-windup.
 * See: Wittenmark B., Astrom K., Arzen K. IFAC Professional Brief. Computer Control: An Overview. 
 * Two additional parameters used: derivativeFilterLoss and antiwindupFreq
 * (If both are 0, then this controller is identical to PidParallelController)
 *
 * TODO: should PidIndustrial replace all usages of Pid/PidParallelController?
 */
class PidIndustrial : public Pid {
public:
	PidIndustrial();
	explicit PidIndustrial(pid_s *pid);

	using Pid::getOutput;
	float getOutput(float target, float input, float dTime) override;

public:
	// todo: move this to pid_s one day
	float antiwindupFreq = 0.0f;			// = 1/ResetTime
	float derivativeFilterLoss = 0.0f;	// = 1/Gain

private:
	float limitOutput(float v) const;
};


// todo: composition instead of inheritance? :(
class PidWithParameters : public Pid {
public:
	pid_s parametersStorage;

	PidWithParameters() {
		initPidClass(&parametersStorage);
	}
};
