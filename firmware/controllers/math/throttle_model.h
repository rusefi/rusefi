#pragma once

struct ThrottleModelBase {
public:
	float estimateThrottleFlow(float tip, float tps, float map, float iat);
	expected<float> estimateThrottleFlow(float map, float tps);

	float partThrottleFlow(float tps, float flowCorrection) const;
	float partThrottleFlow(float tps, float pressureRatio, float p_up, float iat) const;

protected:
	// Given some TPS, what is the normalized choked flow in kg/s?
	virtual float effectiveArea(float tps) const = 0;

	// Given some MAP, what is the most the engine can pull through a wide open throttle, in kg/s?
	virtual float maxEngineFlow(float map) const = 0;

private:
	float inversePartThrottleFlow(float maxEngineFlow, float flow, float pressureRatio, float p_up, float iat) const;
};

struct ThrottleModel : public ThrottleModelBase {
	float effectiveArea(float tps) const override;
	float maxEngineFlow(float map) const override;
};
