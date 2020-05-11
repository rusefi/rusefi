#include "engine_test_helper.h"
#include "fuel_math.h"

#include "gtest/gtest.h"

TEST(FuelMath, getStandardAirCharge) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	// Miata 1839cc 4cyl
	CONFIG(specs.displacement) = 1.839f;
	CONFIG(specs.cylindersCount) = 4;

	EXPECT_FLOAT_EQ(0.553539f, getStandardAirCharge(PASS_ENGINE_PARAMETER_SIGNATURE));

	// LS 5.3 liter v8
	CONFIG(specs.displacement) = 5.327f;
	CONFIG(specs.cylindersCount) = 8;

	EXPECT_FLOAT_EQ(0.80171353f, getStandardAirCharge(PASS_ENGINE_PARAMETER_SIGNATURE));

	// Chainsaw - single cylinder 32cc
	CONFIG(specs.displacement) = 0.032f;
	CONFIG(specs.cylindersCount) = 1;
	EXPECT_FLOAT_EQ(0.038528003, getStandardAirCharge(PASS_ENGINE_PARAMETER_SIGNATURE));

	// Leopard 1 47.666 liter v12
	CONFIG(specs.displacement) = 47.666f;
	CONFIG(specs.cylindersCount) = 12;

	EXPECT_FLOAT_EQ(4.782489f, getStandardAirCharge(PASS_ENGINE_PARAMETER_SIGNATURE));
}
