/*
 * pid_auto_tune.cpp
 *
 * See https://github.com/br3ttb/Arduino-PID-AutoTune-Library/blob/master/PID_AutoTune_v0/PID_AutoTune_v0.cpp
 * See https://github.com/t0mpr1c3/Arduino-PID-AutoTune-Library/blob/master/PID_AutoTune_v0/PID_AutoTune_v0.cpp
 *
 *
 *  Created on: Sep 13, 2017
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "pid_auto_tune.h"
#include "efilib.h"

PID_AutoTune::PID_AutoTune() {
	running = false;
	nLookBack = 50;
	oStep = 10;
	noiseBand = 0.5;

}

//void PID_AutoTune::FinishUp() {
//	Ku = 4 * (2 * oStep) / ((absMax - absMin) * 3.14159);
//	Pu = (float) (currentPeakTime - prevPeakTime) / 1000.0; // converting ms to seconds
//
//}

bool PID_AutoTune::Runtime(Logging *logging) {

	if (peakCount > 9 && running) {
		running = false;
//		FinishUp();
		return 1;
	}

	if (!running) {
		//initialize working variables the first time around
		peakType = NOT_A_PEAK;
		peakCount = 0;

		absMax = input;
		absMin = input;
		setpoint = input;
		running = true;
		inputCount  = 0;
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
		inputCount ++;
	}

	//oscillate the output base on the input's relation to the setpoint

	float prevOutput = output;
	if (input > setpoint + noiseBand)
		output = outputStart - oStep;
	else if (input < setpoint - noiseBand)
		output = outputStart + oStep;

	if (output != prevOutput) {
		scheduleMsg(logging, "direction change %.2f", output);
	}


	bool isMax = true; // is current input max value for the known input history?
	bool isMin = true; // is current input min value for the known input history?
	//identify peaks
	for (int i = nLookBack - 1; i >= 0; i--) {
		float val = lastInputs[i];
		if (isMax)
			isMax = input >= val;
		if (isMin)
			isMin = input <= val;
		lastInputs[i + 1] = lastInputs[i];
	}
	lastInputs[0] = input;
	if (inputCount   < 9) { //we don't want to trust the maxes or mins until the inputs array has been filled
		return 0;
	}

	if (isMax || isMin) {
		scheduleMsg(logging, "min %d max %d %.2f peakType=%d", isMin, isMax, input, peakType);
	}


	bool directionJustChanged = false;

	if (isMax) {
		if (peakType == NOT_A_PEAK )
			peakType = MAXIMUM ;
		if (peakType == MINIMUM ) {
			peakType = MAXIMUM ;
			directionJustChanged = true;
		}

//		peaks[peakCount] = input; we are not using max peak values

	} else if (isMin) {
		if (peakType == NOT_A_PEAK )
			peakType = MINIMUM ;
		if (peakType == MAXIMUM ) {
			peakType = MINIMUM;
			lastPeakTime[1] = lastPeakTime[0];
			lastPeakTime[0] = currentTimeMillis();
			directionJustChanged = true;

			if (peakCount < 10) {
				peakCount++;
				lastPeaks[peakCount] = input;
			} else {
				// todo: reset peak counter maybe?
			}

		}

	}

	if (directionJustChanged && peakCount > 2) { //we've transitioned.  check if we can autotune based on the last peaks
		float avgSeparation = (absF(lastPeaks[peakCount - 0] - lastPeaks[peakCount - 1])
				+ absF(lastPeaks[peakCount - 1] - lastPeaks[peakCount - 2])) / 2;
		if (avgSeparation < 0.05 * (absMax - absMin)) {
			//FinishUp();
			running = false;
			return 1;

		}
	}

	return 0;
}

