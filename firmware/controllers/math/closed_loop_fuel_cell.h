#pragma once

#include "sensor.h"

class ClosedLoopFuelCellBase {
public:
	// Update the cell's internal state - adjusting fuel up/down as appropriate
	void update(float lambdaDeadband, bool ignoreErrorMagnitude);

	// Get the current adjustment amount, without altering internal state.
	float getAdjustment() const;

protected:
	// Helpers - virtual for mocking
	virtual float getLambdaError() const = 0;
	virtual float getMaxAdjustment() const = 0;
	virtual float getMinAdjustment() const = 0;
	virtual float getIntegratorGain() const = 0;

private:
	// Current fueling adjustment.
	// 0 = no adjustment.
	// 0.1 = add 10% fuel.
	float m_adjustment = 0;
};

struct stft_cell_cfg_s;

class ClosedLoopFuelCellImpl final : public ClosedLoopFuelCellBase {
public:
	void configure(const stft_cell_cfg_s* configuration, SensorType lambdaSensor) {
		m_config = configuration;
		m_lambdaSensor = lambdaSensor;
	}

private:
	const stft_cell_cfg_s *m_config = nullptr;
	SensorType m_lambdaSensor = SensorType::Invalid;

protected:
	float getLambdaError() const override;
	float getMaxAdjustment() const override;
	float getMinAdjustment() const override;
	float getIntegratorGain() const override;
};
