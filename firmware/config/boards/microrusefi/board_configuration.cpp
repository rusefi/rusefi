/**
 * @file boards/microrusefi/board_configuration.cpp
 *
 *
 * @brief Configuration defaults for the microRusefi board
 *
 * See https://github.com/rusefi/rusefi_documentation/wiki/Hardware_microRusEfi_wiring
 *
 * @author Matthew Kennedy, (c) 2019
 */

#include "global.h"
#include "engine.h"
#include "engine_math.h"
#include "allsensors.h"
#include "fsio_impl.h"
#include "engine_configuration.h"

EXTERN_ENGINE;

static void setInjectorPins() {
	boardConfiguration->injectionPins[0] = GPIOE_14;
	boardConfiguration->injectionPins[1] = GPIOE_13;
	boardConfiguration->injectionPins[2] = GPIOE_12;
	boardConfiguration->injectionPins[3] = GPIOE_11;

	// Disable remainder
	for (int i = 4; i < INJECTION_PIN_COUNT;i++) {
		boardConfiguration->injectionPins[i] = GPIO_UNASSIGNED;
	}

	boardConfiguration->injectionPinMode = OM_DEFAULT;
}

static void setIgnitionPins() {
	// todo: I wonder if these are not right in light of the network rename and the +12 VP issue?
	boardConfiguration->ignitionPins[0] = GPIOD_4;
	boardConfiguration->ignitionPins[1] = GPIOD_3;
	boardConfiguration->ignitionPins[2] = GPIOD_2;
	boardConfiguration->ignitionPins[3] = GPIOD_1;

	// disable remainder
	for (int i = 4; i < IGNITION_PIN_COUNT; i++) {
		boardConfiguration->ignitionPins[i] = GPIO_UNASSIGNED;
	}

	boardConfiguration->ignitionPinMode = OM_DEFAULT;
}

// The MicroRusEFI board has the main LEDs on pins E1..E4
// PE1 Yellow
// PE2 Blue
// PE3 Red
// PE4 Green
static void setLedPins() {

	//engineConfiguration->FatalErrorPin = GPIOE_3;		// red
	engineConfiguration->communicationLedPin = GPIOE_2;	// blue
	engineConfiguration->runningLedPin = GPIOE_4;		// green
	boardConfiguration->triggerErrorPin = GPIOE_1;		// orange
}

// Set the coeffficients from sensed voltage to ADC count.
// For most uses these coefficients are not used.
// The ADC Vref is the buffered and divided 5V sensor supply, using
// an input structure that matches the sensors.
// Thus ratiometric sensors will use the full ADC range, inheriently
// corrected for temperature, voltage and component variations.
//
// MRE versions through 0.3 used a 1K / 1.5K (high / low side) divider,
// version 0.4 and later use a 6.8K / 10K resistor divider.
//  1.00K / 1.50K = 0.600   3.00V from a 5.00V supply
//  6.8K / 10.0K  = 0.595
// On all versions the 12V supply is measured by a 39K/10K divider,
// using the buffered/divided input structure.
static void setupVbatt() {
	// 1k high side/1.5k low side = 1.6667 ratio divider
	engineConfiguration->analogInputDividerCoefficient = 2.5f / 1.5f;

	// 39k high side/10k low side multiplied by above analogInputDividerCoefficient = 8.166666f
	engineConfiguration->vbattDividerCoeff = (49.0f / 10.0f) * engineConfiguration->analogInputDividerCoefficient;
	engineConfiguration->vbattAdcChannel = EFI_ADC_11;

	engineConfiguration->adcVcc = 3.29f;
}

// Configure the TLE8888 control interface.
// Future: Change to SPI1 to allow SPI3 to be mapped elsewhere.
static void setupTle8888() {
	// Enable SPI3
	boardConfiguration->is_enabled_spi_3 = true;

	// Wire up SPI3
	boardConfiguration->spi3mosiPin = GPIOB_5;
	boardConfiguration->spi3misoPin = GPIOB_4;
	boardConfiguration->spi3sckPin = GPIOB_3;

	// Chip select
	engineConfiguration->tle8888_cs = GPIOD_5;

	// SPI device
	engineConfiguration->tle8888spiDevice = SPI_DEVICE_3;
}

// Configure the Electronic Throttle Body (ETB) driver.
// On MRE the ETB is intended to be driven by the TLE9201 H-Bridge.
// 
// This chip has three control pins:
//  DIR - sets direction of the motor
//  PWM - control (enable high, coast low), PWM capable
//  DIS - disables motor (enable low)
// ToDo: Clarify assumptions about analog feedback from the ETB.
// Future: Probe for a connected diagnostic interface on SPI3
static void setupEtb() {
	// PWM pin
	boardConfiguration->etb1.controlPin1 = GPIOC_7;
	// DIR pin
	boardConfiguration->etb1.directionPin1 = GPIOA_8;

	// set_fsio_output_pin 7 PC8
#if EFI_FSIO
	// set_rpn_expression 8 "1"
	// disable ETB by default
	setFsio(7, GPIOC_8, "1" PASS_CONFIG_PARAMETER_SUFFIX);
	// enable ETB
	// set_rpn_expression 8 "0"
	//setFsio(7, GPIOC_8, "0" PASS_CONFIG_PARAMETER_SUFFIX);
#endif /* EFI_FSIO */


	// set_analog_input_pin pps PA7
	// EFI_ADC_7: "31 - AN volt 3" - PA7
	// engineConfiguration->throttlePedalPositionAdcChannel = EFI_ADC_7;

	// Unused
	boardConfiguration->etb1.directionPin2 = GPIO_UNASSIGNED;

	// we only have pwm/dir, no dira/dirb
	engineConfiguration->etb1_use_two_wires = false;

	engineConfiguration->etbFreq = 800;
}

// Configure key sensors inputs.
//
// The Trigger is our primary timing signal, usually from the crank.
// MRE default to the VR output from the TLE8888.
// A secondary Cam signal is assumed to be a Hall sensor or pre-conditioned
// logic-level or open-collector signal.
// ToDo: Review count assumption with initialization of unused triggers/cams
// ToDo: Resolve angst over default input assignments.
static void setupDefaultSensorInputs() {
	// trigger inputs
	// tle8888 VR conditioner
	boardConfiguration->triggerInputPins[0] = GPIOC_6;
	boardConfiguration->triggerInputPins[1] = GPIO_UNASSIGNED;
	// Direct hall-only cam input
	engineConfiguration->camInputs[0] = GPIOA_5;

	// Open question if it's great to have TPS in default TPS
	// The down-side is for vehicles without TPS or for first start
	// without TPS one would have to turn it off to avoid cranking
	// corrections based on wrong TPS data
	// tps = "20 - AN volt 5"
	engineConfiguration->tps1_1AdcChannel = EFI_ADC_13;
	engineConfiguration->tps2_1AdcChannel = EFI_ADC_NONE;

	// EFI_ADC_10: "27 - AN volt 1"
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_10;

	// EFI_ADC_14: "32 - AN volt 6"
	engineConfiguration->afr.hwChannel = EFI_ADC_14;

	// clt = "18 - AN temp 1"
	engineConfiguration->clt.adcChannel = EFI_ADC_0;
	engineConfiguration->clt.config.bias_resistor = 2700;

	// iat = "23 - AN temp 2"
	engineConfiguration->iat.adcChannel = EFI_ADC_1;
	engineConfiguration->iat.config.bias_resistor = 2700;
}

void setPinConfigurationOverrides(void) {
}

// Future: configure USART3 for LIN bus and UART4 for console
void setSerialConfigurationOverrides(void) {
	boardConfiguration->useSerialPort = false;
	engineConfiguration->binarySerialTxPin = GPIO_UNASSIGNED;
	engineConfiguration->binarySerialRxPin = GPIO_UNASSIGNED;
	engineConfiguration->consoleSerialTxPin = GPIO_UNASSIGNED;
	engineConfiguration->consoleSerialRxPin = GPIO_UNASSIGNED;
}


/**
 * @brief   Board-specific configuration code overrides.
 *
 * See also setDefaultEngineConfiguration
 *
 * @todo    Add your board-specific code, if any.
 */
void setBoardConfigurationOverrides(void) {
	setInjectorPins();
	setIgnitionPins();
	setLedPins();
	setupVbatt();
	setupTle8888();
	setupEtb();

	// The MRE uses the TLE8888 fixed-function main relay control pin.
	// This firmware is not involved with main relay control, although
	// the pin inputs can be over-ridden through the TLE8888 Cmd0 register.
	// ToDo: Review if EFI_MAIN_RELAY_CONTROL should be TRUE or FALSE
	// for this type of partial control.

	// Configure the TLE8888 outputs, some are controlled by pin mappings,
	// others through SPI registers.

	// TLE8888_IN11 -> TLE8888_OUT21
	// GPIOE_8: "35 - GP Out 1"
	boardConfiguration->fuelPumpPin = GPIOE_8;

	// TLE8888_IN9 -> OUT5 high current low side: VVT2
	// GPIOE_10: "3 - Lowside 2"
	boardConfiguration->idle.solenoidPin = GPIOE_10;

	// This is a SPI controlled output
	// TLE8888_PIN_22: "34 - GP Out 2"
	boardConfiguration->fanPin = TLE8888_PIN_22;

	// The "required" hardware is done - set some reasonable defaults
	setupDefaultSensorInputs();

	// Some sensible defaults for other options
	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);
	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL_60_2;
	engineConfiguration->useOnlyRisingEdgeForTrigger = true;
	setAlgorithm(LM_SPEED_DENSITY PASS_CONFIG_PARAMETER_SUFFIX);

	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->specs.firingOrder = FO_1_3_4_2;

	// Ign is IM_{ONE_COIL,TWO_COILS,INDIVIDUAL_COILS,WASTED_SPARK}
	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;
	// Inj mode: IM_SIMULTANEOUS, IM_SEQUENTIAL, IM_BATCH, IM_SINGLE_POINT
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	engineConfiguration->injectionMode = IM_SIMULTANEOUS;
}

void setAdcChannelOverrides(void) {
}

/**
 * @brief   Board-specific SD card configuration code overrides. Needed by bootloader code.
 * @todo    Add your board-specific code, if any.
 */
void setSdCardConfigurationOverrides(void) {
}
