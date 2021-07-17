#include "closed_loop_fuel_cell.h"
#include "engine_configuration_generated_structures.h"

constexpr float integrator_dt = FAST_CALLBACK_PERIOD_MS * 0.001f;

void ClosedLoopFuelCellBase::update(float lambdaDeadband, bool ignoreErrorMagnitude DECLARE_ENGINE_PARAMETER_SUFFIX)
{
	// Compute how far off target we are
	float lambdaError = getLambdaError(PASS_ENGINE_PARAMETER_SIGNATURE);

	// If we're within the deadband, make no adjustment.
	if (absF(lambdaError) < lambdaDeadband) {
		return;
	}

	// Fixed magnitude - runs in constant adjustment rate mode
	if (ignoreErrorMagnitude) {
		if (lambdaError > 0) {
			lambdaError = 0.1f;
		} else {
			lambdaError = -0.1f;
		}
	}

	// Integrate
	float adjust = getIntegratorGain() * lambdaError * integrator_dt
					+ m_adjustment;

	// Clamp to bounds
	float minAdjust = getMinAdjustment();
	float maxAdjust = getMaxAdjustment();

	if (adjust > maxAdjust) {
		adjust = maxAdjust;
	} else if (adjust < minAdjust) {
		adjust = minAdjust;
	}

	// Save state
	m_adjustment = adjust;
}

float ClosedLoopFuelCellBase::getAdjustment() const {
	return 1.0f + m_adjustment;
}

float ClosedLoopFuelCellImpl::getLambdaError(DECLARE_ENGINE_PARAMETER_SIGNATURE) const {
	auto lambda = Sensor::get(m_lambdaSensor);

	// Failed sensor -> no error
	if (!lambda) {
		return 0;
	}

	return lambda.Value - ENGINE(engineState.targetLambda);
}

#define MAX_ADJ (0.25f)

float ClosedLoopFuelCellImpl::getMaxAdjustment() const {
	if (!m_config) {
		// If no config, disallow adjustment.
		return 0;
	}

	float raw = m_config->maxAdd * 0.01f;
	// Don't allow maximum less than 0, or more than maximum adjustment
	return minF(MAX_ADJ, maxF(raw, 0));
}

float ClosedLoopFuelCellImpl::getMinAdjustment() const {
	if (!m_config) {
		// If no config, disallow adjustment.
		return 0;
	}

	float raw = m_config->maxRemove * 0.01f;
	// Don't allow minimum more than 0, or more than maximum adjustment
	return maxF(-MAX_ADJ, minF(raw, 0));
}

float ClosedLoopFuelCellImpl::getIntegratorGain() const {
	if (!m_config) {
		// If no config, disallow adjustment.
		return 0.0f;
	}

	float timeConstant = m_config->timeConstant * 0.1f;

	// Clamp to reasonable limits - 100ms to 100s
	timeConstant = maxF(0.1f, minF(timeConstant, 100));

	return 1 / timeConstant;
}
