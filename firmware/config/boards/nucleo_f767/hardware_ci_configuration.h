#pragma once

// Shared with host tests so fixture calibration is checked without STM32 HAL.
inline void setNucleoHardwareCiConfiguration(engine_configuration_s& configuration) {
	// The Nucleo fixture uses a 3.3 V ADC reference, unlike the 3.0 V
	// Discovery engine default. Keep voltage scaling correct across presets.
	configuration.adcVcc = 3.3f;
	// MINIMAL_PINS has no MAP input, so the fast ADC would never start.
	// PC3 supports ADC2 (fast) and ADC1 (slow), and avoids Ethernet pins.
	configuration.map.sensor.hwChannel = EFI_ADC_13;
}
