#include "pch.h"

TEST(hardware, reinit) {
	EngineTestHelper eth(engine_type_e::FRANKENSO_MIATA_NA6_MAP);

	ButtonDebounce::stopConfigurationList();
	ButtonDebounce::startConfigurationList();

	ButtonDebounce::stopConfigurationList();
	ButtonDebounce::startConfigurationList();


	resetConfigurationExt(nullptr, engine_type_e::DODGE_NEON_1995);
	resetConfigurationExt(nullptr, engine_type_e::FRANKENSO_MIATA_NA6_MAP);
}
