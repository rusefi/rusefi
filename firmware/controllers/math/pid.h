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

class Pid {

public:
	Pid();
	Pid(pid_s *pid, float minResult, float maxResult);
	void init(pid_s *pid, float minResult, float maxResult);
	bool isSame(pid_s *pid);

	float getValue(float target, float input);
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
private:
	pid_s *pid;
	float minResult;
	float maxResult;

	float iTerm;
	float dTerm; // we are remembering this only for debugging purposes
	float prevError;
};

#endif /* PID_H_ */
