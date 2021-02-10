#include "adc_inputs.h"

/**
 * @brief   Board-specific configuration code overrides.
 * @todo    Add your board-specific code, if any.
 */
void setBoardConfigurationOverrides(void) {
}

/**
 * @brief   Board-specific pin configuration code overrides. Needed by bootloader code.
 * @todo    Add your board-specific code, if any.
 */
void setPinConfigurationOverrides(void) {
}

/**
 * @brief   Board-specific Serial configuration code overrides. Needed by bootloader code.
 * @todo    Add your board-specific code, if any.
 */
void setSerialConfigurationOverrides(void) {
}

/**
 * @brief   Board-specific SD card configuration code overrides. Needed by bootloader code.
 * @todo    Add your board-specific code, if any.
 */
void setSdCardConfigurationOverrides(void) {
}

/**
 * @brief   Board-specific ADC channel configuration code overrides.
 * @todo    Add your board-specific code, if any.
 */
void setAdcChannelOverrides(void) {
}

// TODO: fix analog inputs so we don't need these stubs
float getVoltage(const char*, adc_channel_e) {
	return 0;
}

float getVoltageDivided(const char*, adc_channel_e) {
	return 0;
}
