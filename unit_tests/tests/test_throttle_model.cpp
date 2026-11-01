#include "pch.h"
#include "throttle_model.h"

// From CFD modeled 70mm throttle
static const float throttle70mmFlowBins[] =   {        2,     5,      10,    20,    30,    40,    60,   80,   85,    90,    91 };
static const float throttle70mmFlowValues[] = { 0.000095f, 0.002f,  0.0107f, 0.045f, 0.103f, 0.185f, 0.438f, 0.74f, 0.77f, 0.775f, 0.775f };

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

	EXPECT_NEAR( 2.329f, model.throttlePositionForFlow(  0.3f, 0.3f, 100, 0), 1e-2);
	EXPECT_NEAR( 3.446f, model.throttlePositionForFlow(  1, 0.3f, 100, 0), 1e-2);
	EXPECT_NEAR( 9.674f, model.throttlePositionForFlow( 10, 0.3f, 100, 0), 1e-2);
	EXPECT_NEAR(29.711f, model.throttlePositionForFlow(100, 0.3f, 100, 0), 1e-2);
	EXPECT_NEAR(41.395f, model.throttlePositionForFlow(200, 0.3f, 100, 0), 1e-2);
	EXPECT_NEAR(57.415f, model.throttlePositionForFlow(400, 0.3f, 100, 0), 1e-2);
	EXPECT_NEAR(71.255f, model.throttlePositionForFlow(600, 0.3f, 100, 0), 1e-2);
	EXPECT_NEAR(77.965f, model.throttlePositionForFlow(700, 0.3f, 100, 0), 1e-2);
	EXPECT_NEAR(83.323f, model.throttlePositionForFlow(750, 0.3f, 100, 0), 1e-2);
	EXPECT_NEAR(85.070f, model.throttlePositionForFlow(760, 0.3f, 100, 0), 1e-2);

	// Throttle is maxed out here!

	EXPECT_NEAR(100.00f, model.throttlePositionForFlow(770, 0.3f, 100, 0), 1e-2);
	EXPECT_NEAR(100.00f, model.throttlePositionForFlow(800, 0.3f, 100, 0), 1e-2);
	EXPECT_NEAR(100.00f, model.throttlePositionForFlow(900, 0.3f, 100, 0), 1e-2);
}

TEST(ThrottleModel, InverseFlowHighPressureRatio) {
	MockThrottleModel model;

	EXPECT_NEAR( 2.626f, model.throttlePositionForFlow(  0.3f, 0.9f, 100, 0), 1e-2);
	EXPECT_NEAR( 4.437f, model.throttlePositionForFlow(  1, 0.9f, 100, 0), 1e-2);
	EXPECT_NEAR( 5.738f, model.throttlePositionForFlow(  2, 0.9f, 100, 0), 1e-2);
	EXPECT_NEAR( 8.571f, model.throttlePositionForFlow(  5, 0.9f, 100, 0), 1e-2);
	EXPECT_NEAR(11.670f, model.throttlePositionForFlow( 10, 0.9f, 100, 0), 1e-2);
	EXPECT_NEAR(37.473f, model.throttlePositionForFlow(100, 0.9f, 100, 0), 1e-2);
	EXPECT_NEAR(51.348f, model.throttlePositionForFlow(200, 0.9f, 100, 0), 1e-2);
	EXPECT_NEAR(74.510f, model.throttlePositionForFlow(400, 0.9f, 100, 0), 1e-2);
	EXPECT_NEAR(87.093f, model.throttlePositionForFlow(470, 0.9f, 100, 0), 1e-2);
	EXPECT_NEAR(88.737f, model.throttlePositionForFlow(471, 0.9f, 100, 0), 1e-2);

	// Throttle is maxed out here!

	EXPECT_NEAR(100.00f, model.throttlePositionForFlow(473, 0.9f, 100, 0), 1e-2);
	EXPECT_NEAR(100.00f, model.throttlePositionForFlow(500, 0.9f, 100, 0), 1e-2);
	EXPECT_NEAR(100.00f, model.throttlePositionForFlow(600, 0.9f, 100, 0), 1e-2);
	EXPECT_NEAR(100.00f, model.throttlePositionForFlow(700, 0.9f, 100, 0), 1e-2);
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
