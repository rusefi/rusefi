#include "pch.h"
#include "adc_math.h"
#include "../../firmware/config/boards/nucleo_f767/hardware_ci_configuration.h"

TEST(NucleoHardwareCi, HalfSupplyFixtureVoltage) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setNucleoHardwareCiConfiguration(*engineConfiguration);
	EXPECT_EQ(EFI_ADC_13, engineConfiguration->map.sensor.hwChannel);
	// Host engine defaults omit the production 2x divider.
	engineConfiguration->analogInputDividerCoefficient = 2;
	const auto scaledVoltage = [](int sample) {
		return adcRawValueToRawVoltage(sample) * getAnalogInputDividerCoefficient(EFI_ADC_13);
	};

	EXPECT_FLOAT_EQ(3.3f, engineConfiguration->adcVcc);
	EXPECT_NEAR(3.3f, scaledVoltage(2048), 0.002f);
	// These codes produced the 2.945/2.963 V CI failures with adcVcc=3.0.
	// The same samples satisfy the unchanged 10% hardware-test tolerance.
	EXPECT_NEAR(3.3f, scaledVoltage(2010), 3.3f * 0.1f);
	EXPECT_NEAR(3.3f, scaledVoltage(2022), 3.3f * 0.1f);
}

TEST(NucleoHardwareCi, EnginePresetsKeepFixtureCalibration) {
	// Hardware CI boots MINIMAL_PINS, then selects these presets in order.
	for (auto engineType : {engine_type_e::MINIMAL_PINS, engine_type_e::VW_ABA,
		engine_type_e::FRANKENSO_BMW_M73_F, engine_type_e::FRANKENSO_MIATA_NA6_MAP}) {
		SCOPED_TRACE(static_cast<int>(engineType));
		// resetConfigurationExt applies board overrides before engine presets.
		EngineTestHelper eth(engineType, [](engine_configuration_s* configuration) {
			setNucleoHardwareCiConfiguration(*configuration);
		});
		EXPECT_FLOAT_EQ(3.3f, engineConfiguration->adcVcc);
	}
}
