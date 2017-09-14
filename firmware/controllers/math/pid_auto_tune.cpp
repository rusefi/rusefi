/*
 * pid_auto_tune.cpp
 *
 *  Created on: Sep 13, 2017
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#include "pid_auto_tune.h"

PID_AutoTune::PID_AutoTune() {
	running = false;
	nLookBack = 50;
	oStep = 10;
	noiseBand = 0.5;
}

int PID_AutoTune::Runtime() {
	justevaled = false;
	if (peakCount > 9 && running) {
		running = false;
		FinishUp();
		return 1;
	}

	justevaled = true;
	if (!running) {
		//initialize working variables the first time around
		peakType = 0;
		peakCount = 0;
		justchanged = false;
		absMax = input;
		absMin = input;
		setpoint = input;
		running = true;
		outputStart = output;
		output = outputStart + oStep;

		for (int i = nLookBack - 1; i >= 0; i--) {
			// default values for history array
			lastInputs[i] = input;
		}

	} else {
		if (input > absMax)
			absMax = input;
		if (input < absMin)
			absMin = input;
	}

	//oscillate the output base on the input's relation to the setpoint

	if (input > setpoint + noiseBand)
		output = outputStart - oStep;
	else if (input < setpoint - noiseBand)
		output = outputStart + oStep;

	return 0;
}

