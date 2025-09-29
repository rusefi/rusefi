#include "pch.h"
#include "biquad.h"

TEST(util, biQuad) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE); // LOL engineConfiguration->verboseQuad
	Biquad b;
	float filtered = 0;

	// 0.01 means filter bandwidth of ~1hz with ~100hz sensor
	b.configureLowpass(1, 0.01f);

	for (size_t i = 0; i < 10000; i++) {
		filtered = b.filter(25.0);
	}

	// very close, but never actually reaches
	EXPECT_NEAR(filtered, 25.0, 0.1);
}
