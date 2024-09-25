#include "pch.h"

#include "closed_loop_fuel_cell.h"

#if EFI_ENGINE_CONTROL

constexpr float integrator_dt = FAST_CALLBACK_PERIOD_MS * 0.001f;

void ClosedLoopFuelCellBase::update(float lambdaDeadband, bool ignoreErrorMagnitude)
{
	// Compute how far off target we are
	float lambdaError = getLambdaError();

	// If we're within the deadband, make no adjustment.
	if (std::abs(lambdaError) < lambdaDeadband) {
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

float ClosedLoopFuelCellImpl::getLambdaError() const {
	auto lambda = Sensor::get(m_lambdaSensor);

	// Failed sensor -> no error
	if (!lambda) {
		return 0;
	}

	return lambda.Value - engine->fuelComputer.targetLambda;
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

	// Clamp to reasonable limits - 100ms to 100s
	float timeConstant = maxF(0.1f, minF(m_config->timeConstant, 100));

	return 1 / timeConstant;
}

#endif // EFI_ENGINE_CONTROL
