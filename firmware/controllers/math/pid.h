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

class Pid {

public:
	Pid();
	Pid(pid_s *pid);
	void init(pid_s *pid);
	bool isSame(pid_s *pid);

	float getValue(float target, float input);
	// todo: dTime should be taken from pid_s
	float getValue(float target, float input, float dTime);
	void updateFactors(float pFactor, float iFactor, float dFactor);
	void reset(void);
	float getP(void);
	float getI(void);
	float getD(void);
	float getOffset(void);
	float getIntegration(void);
	float getPrevError(void);
#if EFI_PROD_CODE || EFI_SIMULATOR
	void postState(TunerStudioOutputChannels *tsOutputChannels);
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
};

#endif /* PID_H_ */
