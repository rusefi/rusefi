/**
 * @file closed_loop_controller.h
 */

#pragma once

#include "expected.h"

template <typename TInput, typename TOutput>
class ClosedLoopController {
public:
	void update() {
		expected<TOutput> outputValue = getOutput();
		setOutput(outputValue);
	}

private:
	expected<TOutput> getOutput() {
		expected<TInput> setpoint = getSetpoint();
		// If we don't know the setpoint, return failure.
		if (!setpoint) {
			return unexpected;
		}

		expected<TInput> observation = observePlant();
		// If we couldn't observe the plant, return failure.
		if (!observation) {
			return unexpected;
		}

		expected<TOutput> openLoopResult = getOpenLoop(setpoint.Value);
		// If we couldn't compute open loop, return failure.
		if (!openLoopResult) {
			return unexpected;
		}

		expected<TOutput> closedLoopResult = getClosedLoop(setpoint.Value, observation.Value);
		// If we couldn't compute closed loop, return failure.
		if (!closedLoopResult) {
			return unexpected;
		}

		return openLoopResult.Value + closedLoopResult.Value;
	}

	// Get the setpoint: where should the controller put the plant?
	virtual expected<TInput> getSetpoint() const = 0;

	// Get the current observation: what is the current state of the world?
	virtual expected<TInput> observePlant() const = 0;

	// Get the open-loop output: output state based on only the setpoint
	virtual expected<TOutput> getOpenLoop(TInput setpoint) const = 0;

	// Get the closed-loop output: output state based on setpoint and observation
	virtual expected<TOutput> getClosedLoop(TInput setpoint, TInput observation) = 0;

	// Set the output: Drive whatever output system will perturb the plant in the real world
	virtual void setOutput(expected<TOutput> outputValue) = 0;
};
