/*
 * pid_auto_tune.h
 *
 * http://brettbeauregard.com/blog/2012/01/arduino-pid-autotune-library/
 *
 *
 *  Created on: Sep 13, 2017
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef CONTROLLERS_MATH_PID_AUTO_TUNE_H_
#define CONTROLLERS_MATH_PID_AUTO_TUNE_H_

class PID_AutoTune {
public:
	PID_AutoTune();
	void reset();
	void FinishUp();
	int Runtime();
	bool isMax, isMin;
	/**
	 * sensor position
	 */
	float input;
	/**
	 * actuator duty cycle
	 */
	float output;
	/**
	 * trigger line
	 */
	float setpoint;
	float noiseBand;
	//int controlType = 1;
	bool running;
	unsigned int peak1, peak2, lastTime;
	//int sampleTime;
	int nLookBack;
	int peakType; // todo: convert to enum
	float lastInputs[101];
	float peaks[10];
	int peakCount;
	bool justchanged;
	bool justevaled;
	float absMax, absMin;
	float oStep;
	float outputStart;
	float Ku, Pu;
};

#endif /* CONTROLLERS_MATH_PID_AUTO_TUNE_H_ */
