

#include "engine_test_helper.h"

TEST(hardware, reinit) {
	WITH_ENGINE_TEST_HELPER(MIATA_NA6_MAP);

	ButtonDebounce::stopConfigurationList();
	ButtonDebounce::startConfigurationList();

	ButtonDebounce::stopConfigurationList();
	ButtonDebounce::startConfigurationList();
}
