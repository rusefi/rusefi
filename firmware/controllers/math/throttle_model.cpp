#include "pch.h"

#include "throttle_model.h"

#include "fuel_math.h"

static const float pressureRatioCorrectionBins[] =   { 0.53125, 0.546875, 0.5625, 0.578125, 0.59375, 0.609375, 0.625, 0.640625, 0.65625, 0.671875, 0.6875, 0.703125, 0.71875, 0.734375, 0.750, 0.765625, 0.78125, 0.796875, 0.8125, 0.828125, 0.84375, 0.859375, 0.875, 0.890625, 0.90625, 0.921875, 0.9375, 0.953125 };
static const float pressureRatioCorrectionValues[] = {       1,   0.9993, 0.998,  0.995,    0.991,   0.986,    0.979, 0.972,    0.963,   0.953,    0.942,  0.930,    0.916,   0.901,    0.884, 0.866,    0.845,   0.824,    0.800,  0.774,    0.745,   0.714,    0.679, 0.642,    0.600,   0.553,    0.449,  0.449    };
static float pressureRatioFlowCorrection(float pr) {
	if (pr < 0.531) {
		return 1.0;
	}

	if (pr > 0.95) {
		return 0.449f;
	}

	// float x = pr;
	// float x2 = x * x;
	// float x3 = x2 * x;
	// return -6.9786 * x3 + 11.597 * x2 - 6.7227 * x + 2.3509;

	return interpolate2d(pr, pressureRatioCorrectionBins, pressureRatioCorrectionValues);
}

static float flowCorrections(float pressureRatio, float p_up, float iat) {
	// PR correction
	float prCorrectionFactor = pressureRatioFlowCorrection(pressureRatio);

	// Inlet density correction
	float tempCorrection = sqrt(273 / (iat + 273));
	float pressureCorrection = p_up / 101.325;
	float densityCorrection = tempCorrection * pressureCorrection;

	return prCorrectionFactor * densityCorrection;
}

float ThrottleModelBase::partThrottleFlow(float tps, float flowCorrection) const {
	return effectiveArea(tps) * flowCorrection;
}

float ThrottleModelBase::partThrottleFlow(float tps, float pressureRatio, float p_up, float iat) const {
	return partThrottleFlow(tps, flowCorrections(pressureRatio, p_up, iat));
}

class InverseFlowSolver : public NewtonsMethodSolver {
public:
	InverseFlowSolver(const ThrottleModelBase* model, float target, float pressureRatio, float p_up, float iat)
		: m_model(*model)
		, m_flowCorrection(flowCorrections(pressureRatio, p_up, iat))
		, m_target(target)
	{
	}

private:
	const ThrottleModelBase& m_model;
	const float m_flowCorrection;
	const float m_target;

	float fx(float x) override {
		// Return 0 when the estimate equals the target, positive when estimate too large
		return m_model.partThrottleFlow(x, m_flowCorrection) - m_target;
	}

	float dfx(float x) override {
		// The marginal flow per angle (dFlow/dTPS) is not trivially differentiable,
		// but it is continuous, so we can use a finite difference approximation over some
		// "small" step size (0.1 degree ~= 0 for throttle purposes)
		// Too small a step may provoke numerical instability.
		return (fx(x + 0.1) - fx(x - 0.1)) / 0.2;
	}
};

// Find the throttle position that gives the specified flow
float ThrottleModelBase::throttlePositionForFlow(float flow, float pressureRatio, float p_up, float iat) const {
	// What does the bare throttle flow at wide open?
	float wideOpenFlow = partThrottleFlow(100, pressureRatio, p_up, iat);

	// If the target flow is more than the throttle can flow, return 100% since the throttle
	// can't open any further
	// If we don't do this, trying to solve using the solver may diverge
	if (flow > wideOpenFlow) {
		return 100;
	}

	InverseFlowSolver solver(this, flow, pressureRatio, p_up, iat);
	return solver.solve(50, 0.1).value_or(0);
}

float ThrottleModelBase::estimateThrottleFlow(float tip, float tps, float map, float iat) {
	// How much flow would the engine pull at 0.95 PR?
	// The throttle won't flow much more than this in any scenario, even if the throttle could move more flow.
	constexpr float crossoverPr = 0.95f;
	float p95Flow = maxEngineFlow(tip * crossoverPr);

	// What throttle position gives us that flow at 0.95 PR?
	float throttleAngle95Pr = throttlePositionForFlow(p95Flow, crossoverPr, tip, iat);
	throttleModelCrossoverAngle = throttleAngle95Pr;

	bool useWotModel = tps > throttleAngle95Pr;
	throttleUseWotModel = useWotModel;

	if (useWotModel) {
		// Maximum flow if the throttle was removed
		float maximumPossibleFlow = maxEngineFlow(tip);

		// Linearly interpolate between the P95 point (throttle flow limited) and wide open (engine flow limited)
		return interpolateClamped(throttleAngle95Pr, p95Flow, 100, maximumPossibleFlow, tps);
	} else {
		float pressureRatio = map / tip;

		return partThrottleFlow(tps, pressureRatio, tip, iat);
	}
}

expected<float> ThrottleModelBase::estimateThrottleFlow(float map, float tps) {
	// Inputs
	auto iat = Sensor::get(SensorType::Iat);

	// Use TIP sensor
	// or use Baro sensor if no TIP
	// or use 101.325kPa (std atmosphere) if no Baro
	// TODO: have a real TIP sensor
	auto tip = 	Sensor::hasSensor(SensorType::ThrottleInletPressure) ? Sensor::get(SensorType::ThrottleInletPressure) :
				Sensor::hasSensor(SensorType::BarometricPressure) ? Sensor::get(SensorType::BarometricPressure) :
				SensorResult(101.325f);

	if (!tip || !iat) {
		return unexpected;
	}

	return estimateThrottleFlow(tip.Value, tps, map, iat.Value);
}

void ThrottleModelBase::onSlowCallback() {
	throttleEstimatedFlow = estimateThrottleFlow(Sensor::getOrZero(SensorType::Map), Sensor::getOrZero(SensorType::Tps1)).value_or(0);
}

float ThrottleModel::effectiveArea(float tps) const {
	return interpolate2d(tps, config->throttleEstimateEffectiveAreaBins, config->throttleEstimateEffectiveAreaValues);
}

float ThrottleModel::maxEngineFlow(float map) const {
	return getMaxAirflowAtMap(map);
}
