// this section was generated automatically by rusEFI tool config_definition-all.jar based on gen_config.sh integration/config_page_4.txt
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of page4_s
struct page4_s {
	/**
	 * units: %
	 * offset 0
	 */
	scaled_channel<uint16_t, 10, 1> secondVeTable[VE_LOAD_COUNT][VE_RPM_COUNT] = {};
	/**
	 * units: {bitStringValue(veLoadUnitLabels, veLoadUnitIdxPcv)}
	 * offset 512
	 */
	uint16_t secondVeLoadBins[VE_LOAD_COUNT] = {};
	/**
	 * units: RPM
	 * offset 544
	 */
	uint16_t secondVeRpmBins[VE_RPM_COUNT] = {};
	/**
	 * Pin that activates the second VE table (hard switch, overrides blend)
	 * offset 576
	 */
	switch_input_pin_e secondVeTableInput;
	/**
	 * offset 578
	 */
	pin_input_mode_e secondVeTableInputMode;
	/**
	 * Input that controls the blend ratio between primary and second VE table
	 * offset 579
	 */
	gppwm_channel_e secondVeBlendParameter;
	/**
	 * offset 580
	 */
	scaled_channel<int16_t, 10, 1> secondVeBlendBins[BLEND_FACTOR_SIZE] = {};
	/**
	 * units: %
	 * offset 596
	 */
	scaled_channel<uint8_t, 2, 1> secondVeBlendValues[BLEND_FACTOR_SIZE] = {};
	/**
	 * units: deg
	 * offset 604
	 */
	scaled_channel<int16_t, 10, 1> secondIgnitionTable[IGN_LOAD_COUNT][IGN_RPM_COUNT] = {};
	/**
	 * units: {bitStringValue(ignLoadUnitLabels, ignLoadUnitIdxPcv)}
	 * offset 1116
	 */
	uint16_t secondIgnitionLoadBins[IGN_LOAD_COUNT] = {};
	/**
	 * units: RPM
	 * offset 1148
	 */
	uint16_t secondIgnitionRpmBins[IGN_RPM_COUNT] = {};
	/**
	 * Pin that activates the second ignition table (hard switch, overrides blend)
	 * offset 1180
	 */
	switch_input_pin_e secondIgnitionTableInput;
	/**
	 * offset 1182
	 */
	pin_input_mode_e secondIgnitionTableInputMode;
	/**
	 * Input that controls the blend ratio between primary and second ignition table
	 * offset 1183
	 */
	gppwm_channel_e secondIgnitionBlendParameter;
	/**
	 * offset 1184
	 */
	scaled_channel<int16_t, 10, 1> secondIgnitionBlendBins[BLEND_FACTOR_SIZE] = {};
	/**
	 * units: %
	 * offset 1200
	 */
	scaled_channel<uint8_t, 2, 1> secondIgnitionBlendValues[BLEND_FACTOR_SIZE] = {};
	/**
	 * units: Nm
	 * offset 1208
	 */
	scaled_channel<uint8_t, 1, 10> torqueTable[TORQUE_CURVE_SIZE][TORQUE_CURVE_RPM_SIZE] = {};
	/**
	 * units: RPM
	 * offset 1244
	 */
	uint16_t torqueRpmBins[TORQUE_CURVE_RPM_SIZE] = {};
	/**
	 * units: Load
	 * offset 1256
	 */
	uint16_t torqueLoadBins[TORQUE_CURVE_SIZE] = {};
};
static_assert(sizeof(page4_s) == 1268);

// end
// this section was generated automatically by rusEFI tool config_definition-all.jar based on gen_config.sh integration/config_page_4.txt
