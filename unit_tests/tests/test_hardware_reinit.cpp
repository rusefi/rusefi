#include "pch.h"

TEST(hardware, reinit) {
	WITH_ENGINE_TEST_HELPER(FRANKENSO_MIATA_NA6_MAP);

	ButtonDebounce::stopConfigurationList();
	ButtonDebounce::startConfigurationList();

	ButtonDebounce::stopConfigurationList();
	ButtonDebounce::startConfigurationList();


	resetConfigurationExt(nullptr, DODGE_NEON_1995 PASS_ENGINE_PARAMETER_SUFFIX);
	resetConfigurationExt(nullptr, FRANKENSO_MIATA_NA6_MAP PASS_ENGINE_PARAMETER_SUFFIX);
}
