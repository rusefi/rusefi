#include "pch.h"
#include "throttle_model.h"

// From CFD modeled 70mm throttle
static const float throttle70mmFlowBins[] =   {        2,     5,      10,    20,    30,    40,    60,   80,   85,    90,    91 };
static const float throttle70mmFlowValues[] = { 0.000095, 0.002,  0.0107, 0.045, 0.103, 0.185, 0.438, 0.74, 0.77, 0.775, 0.775 };

class MockThrottleModel : public ThrottleModelBase {
public:
	float effectiveArea(float tps) const override {
		// table is kg/s, function returns g/s
		return 1000 * interpolate2d(tps, throttle70mmFlowBins, throttle70mmFlowValues);
	}

	MOCK_METHOD(float, maxEngineFlow, (float map), (const, override));
};

TEST(ThrottleModel, PartThrottle) {
	MockThrottleModel model;

	EXPECT_CALL(model, maxEngineFlow(::testing::_)).WillRepeatedly([](float map) { return map / 100 * 500; });

	// Vary throttle at constant PR/temp
	// 100kPa inlet
	// 45 kPa MAP
	// 0C IAT
	EXPECT_NEAR( 10.56, model.estimateThrottleFlow(100, 10, 45, 0), 1e-1);
	EXPECT_NEAR( 44.41, model.estimateThrottleFlow(100, 20, 45, 0), 1e-1);
	EXPECT_NEAR(101.65, model.estimateThrottleFlow(100, 30, 45, 0), 1e-1);
	EXPECT_NEAR(182.58, model.estimateThrottleFlow(100, 40, 45, 0), 1e-1);

	// Vary inlet pressure
	EXPECT_NEAR(1.0 * 44.41, model.estimateThrottleFlow(100, 20, 45, 0), 1e-1);
	EXPECT_NEAR(1.5 * 44.41, model.estimateThrottleFlow(150, 20, 45, 0), 1e-1);
}

TEST(ThrottleModel, InverseFlowLowPressureRatio) {
	MockThrottleModel model;

	EXPECT_NEAR( 2.329, model.throttlePositionForFlow(  0.3, 0.3, 100, 0), 1e-2);
	EXPECT_NEAR( 3.446, model.throttlePositionForFlow(  1, 0.3, 100, 0), 1e-2);
	EXPECT_NEAR( 9.674, model.throttlePositionForFlow( 10, 0.3, 100, 0), 1e-2);
	EXPECT_NEAR(29.711, model.throttlePositionForFlow(100, 0.3, 100, 0), 1e-2);
	EXPECT_NEAR(41.395, model.throttlePositionForFlow(200, 0.3, 100, 0), 1e-2);
	EXPECT_NEAR(57.415, model.throttlePositionForFlow(400, 0.3, 100, 0), 1e-2);
	EXPECT_NEAR(71.255, model.throttlePositionForFlow(600, 0.3, 100, 0), 1e-2);
	EXPECT_NEAR(77.965, model.throttlePositionForFlow(700, 0.3, 100, 0), 1e-2);
	EXPECT_NEAR(83.323, model.throttlePositionForFlow(750, 0.3, 100, 0), 1e-2);
	EXPECT_NEAR(85.070, model.throttlePositionForFlow(760, 0.3, 100, 0), 1e-2);

	// Throttle is maxed out here!

	EXPECT_NEAR(100.00, model.throttlePositionForFlow(770, 0.3, 100, 0), 1e-2);
	EXPECT_NEAR(100.00, model.throttlePositionForFlow(800, 0.3, 100, 0), 1e-2);
	EXPECT_NEAR(100.00, model.throttlePositionForFlow(900, 0.3, 100, 0), 1e-2);
}

TEST(ThrottleModel, InverseFlowHighPressureRatio) {
	MockThrottleModel model;

	EXPECT_NEAR( 2.626, model.throttlePositionForFlow(  0.3, 0.9, 100, 0), 1e-2);
	EXPECT_NEAR( 4.437, model.throttlePositionForFlow(  1, 0.9, 100, 0), 1e-2);
	EXPECT_NEAR( 5.738, model.throttlePositionForFlow(  2, 0.9, 100, 0), 1e-2);
	EXPECT_NEAR( 8.571, model.throttlePositionForFlow(  5, 0.9, 100, 0), 1e-2);
	EXPECT_NEAR(11.670, model.throttlePositionForFlow( 10, 0.9, 100, 0), 1e-2);
	EXPECT_NEAR(37.473, model.throttlePositionForFlow(100, 0.9, 100, 0), 1e-2);
	EXPECT_NEAR(51.348, model.throttlePositionForFlow(200, 0.9, 100, 0), 1e-2);
	EXPECT_NEAR(74.510, model.throttlePositionForFlow(400, 0.9, 100, 0), 1e-2);
	EXPECT_NEAR(87.093, model.throttlePositionForFlow(470, 0.9, 100, 0), 1e-2);
	EXPECT_NEAR(88.737, model.throttlePositionForFlow(471, 0.9, 100, 0), 1e-2);

	// Throttle is maxed out here!

	EXPECT_NEAR(100.00, model.throttlePositionForFlow(473, 0.9, 100, 0), 1e-2);
	EXPECT_NEAR(100.00, model.throttlePositionForFlow(500, 0.9, 100, 0), 1e-2);
	EXPECT_NEAR(100.00, model.throttlePositionForFlow(600, 0.9, 100, 0), 1e-2);
	EXPECT_NEAR(100.00, model.throttlePositionForFlow(700, 0.9, 100, 0), 1e-2);
}

TEST(ThrottleModel, SmallEngineBigThrottle) {
	MockThrottleModel model;

	// Engine can only flow 200g/s at 100kPa, much less than wide open throttle
	EXPECT_CALL(model, maxEngineFlow(::testing::_)).WillRepeatedly([](float map) { return map / 100 * 200; });

	// v   part throttle model   v
	EXPECT_NEAR( 81.9, model.estimateThrottleFlow(100,  40, 95, 0), 1e-1);
	EXPECT_NEAR(138.0, model.estimateThrottleFlow(100,  50, 95, 0), 1e-1);
	// ^   part throttle model   ^

	// this engine does 0.95PR at 59% throttle, as the throttle is very big for the engine

	// v   blend from part throttle to WOT   v
	EXPECT_NEAR(190.2, model.estimateThrottleFlow(100,  60, 95, 0), 1e-1);
	EXPECT_NEAR(192.6, model.estimateThrottleFlow(100,  70, 95, 0), 1e-1);
	EXPECT_NEAR(195.1, model.estimateThrottleFlow(100,  80, 95, 0), 1e-1);
	EXPECT_NEAR(197.5, model.estimateThrottleFlow(100,  90, 95, 0), 1e-1);
	// ^   blend from part throttle to WOT   ^

	// v   WOT   v
	EXPECT_NEAR(200.0, model.estimateThrottleFlow(100, 100, 95, 0), 1e-3);
}

TEST(ThrottleModel, BigEngineSmallThrottle) {
	MockThrottleModel model;

	// Big engine can flow 2000g/s, much more than wide open throttle will support
	EXPECT_CALL(model, maxEngineFlow(::testing::_)).WillRepeatedly([](float map) { return map / 100 * 2000; });

	// v   part throttle model   v
	EXPECT_NEAR( 82.0, model.estimateThrottleFlow(100,  40, 95, 0), 1e-1);
	EXPECT_NEAR(138.0, model.estimateThrottleFlow(100,  50, 95, 0), 1e-1);
	EXPECT_NEAR(194.1, model.estimateThrottleFlow(100,  60, 95, 0), 1e-1);
	EXPECT_NEAR(261.0, model.estimateThrottleFlow(100,  70, 95, 0), 1e-1);
	EXPECT_NEAR(327.9, model.estimateThrottleFlow(100,  80, 95, 0), 1e-1);
	EXPECT_NEAR(343.4, model.estimateThrottleFlow(100,  90, 95, 0), 1e-1);
	EXPECT_NEAR(343.4, model.estimateThrottleFlow(100, 100, 95, 0), 1e-1);
	// ^   part throttle model   ^
}
