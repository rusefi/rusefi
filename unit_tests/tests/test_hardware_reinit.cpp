

#include "engine_test_helper.h"

TEST(hardware, reinit) {
	WITH_ENGINE_TEST_HELPER(MIATA_NA6_MAP);

	ButtonDebounce::stopConfigurationList();
	ButtonDebounce::startConfigurationList();

	ButtonDebounce::stopConfigurationList();
	ButtonDebounce::startConfigurationList();


	resetConfigurationExt(nullptr, nullptr, ROVER_V8 PASS_ENGINE_PARAMETER_SUFFIX);
	resetConfigurationExt(nullptr, nullptr, MIATA_NA6_MAP PASS_ENGINE_PARAMETER_SUFFIX);
}
