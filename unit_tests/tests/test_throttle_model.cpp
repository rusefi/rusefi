#include "pch.h"
#include "throttle_model.h"

// From CFD modeled 70mm throttle
static const float throttle70mmFlowBins[] =   {        2,     5,      10,    20,    30,    40,    60,   80,   85,    90,    91 };
static const float throttle70mmFlowValues[] = { 0.000095, 0.002,  0.0107, 0.045, 0.103, 0.185, 0.438, 0.74, 0.77, 0.775, 0.775 };

class MockThrottleModel : public ThrottleModelBase {
public:
	float effectiveArea(float tps) const override {
		return interpolate2d(tps, throttle70mmFlowBins, throttle70mmFlowValues);
	}

	MOCK_METHOD(float, maxEngineFlow, (float map), (const, override));
};

TEST(ThrottleModel, PartThrottle) {
	MockThrottleModel model;

	EXPECT_CALL(model, maxEngineFlow(::testing::_)).WillRepeatedly([](float map) { return map / 100 * 0.5f; });

	// Vary throttle at constant PR/temp
	// 100kPa inlet
	// 45 kPa MAP
	// 0C IAT
	EXPECT_NEAR(0.01056, model.estimateThrottleFlow(100, 10, 45, 0), 1e-4);
	EXPECT_NEAR(0.04441, model.estimateThrottleFlow(100, 20, 45, 0), 1e-4);
	EXPECT_NEAR(0.10165, model.estimateThrottleFlow(100, 30, 45, 0), 1e-4);
	EXPECT_NEAR(0.18258, model.estimateThrottleFlow(100, 40, 45, 0), 1e-4);

	// Vary inlet pressure
	EXPECT_NEAR(1.0 * 0.04441, model.estimateThrottleFlow(100, 20, 45, 0), 1e-4);
	EXPECT_NEAR(1.5 * 0.04441, model.estimateThrottleFlow(150, 20, 45, 0), 1e-4);
}

// TEST(ThrottleModel, WideOpen) {
// 	MockThrottleModel model;

// 	// Engine can only flow 0.5kg/s at 100kPa
// 	EXPECT_CALL(model, maxEngineFlow(::testing::_)).WillRepeatedly([](float map) { return map / 100 * 0.5f; });

// 	EXPECT_NEAR(0.5f, model.estimateThrottleFlow(95, 90, 98, 0), 1e-4);
// }
