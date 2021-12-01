/**
 * @file boards/skeleton/board_configuration.cpp
 *
 *
 * @brief Example configuration defaults for a rusEFI board
 *
 * @author Donald Becker November 2019
 * @author Hugo Becker November 2019
 *
 * This file is an example of board-specific firmware for rusEFI.
 * It contains the unique code need for the setup of a specific board.
 * 
 * This file must contain the configuration for the hard-wired aspects
 * of the board, for instance the pins used for a specific MCU functional
 * unit such as SPI.
 *
 * It may also contain preferences for the assignment of external connector
 * such as which analog input is used to measure coolant temperature, or
 * of if an analog input is connected to a throttle pedal.
 *
 * These initialization functions are called from
 * firmware/controllers/algo/engine_configuration.cpp
 *  void setBoardDefaultConfiguration(void);
 *  void setPinConfigurationOverrides(void);
 *  void setSerialConfigurationOverrides(void);
 *
 * Future: Clean up the distinction between these functions.
 */

#include "pch.h"
#include "fsio_impl.h"

// An example of how to configure complex features on the board.
// Generally these should be local (static) functions, one function per chip.

// This shows a SPI connected TLE8888.
static void setupTle8888() {
	// Enable the SPI channel and set up the SPI pins
	engineConfiguration->is_enabled_spi_3 = true;
	engineConfiguration->spi3mosiPin = GPIOB_5;
	engineConfiguration->spi3misoPin = GPIOB_4;
	engineConfiguration->spi3sckPin = GPIOB_3;

	// SPI chip select is often independent of the SPI pin limitations
	engineConfiguration->tle8888_cs = GPIOD_5;

	// Set SPI device
	engineConfiguration->tle8888spiDevice = SPI_DEVICE_3;
}

// A configuration for a Electronic Throttle Body (ETB) driver.
// This example uses the TLE9201 H-Bridge.
// The TLE9201 has three control pins:
//  DIR - sets direction of the motor
//  PWM - control (enable high, coast low), PWM capable
//  DIS - disables motor (enable low)
// Future: An example showing how to probe for an optionally connected
//   diagnostic interface on SPI
static void setupTle9201Etb() {
	// This chip has PWM/DIR, not dira/dirb
	engineConfiguration->etb_use_two_wires = false;
	// PWM and DIR pins
	engineConfiguration->etbIo[0].controlPin = GPIOC_7;
	engineConfiguration->etbIo[0].directionPin1 = GPIOA_8;
	engineConfiguration->etbIo[0].directionPin2 = GPIO_UNASSIGNED;
}

// Configure key sensors inputs.
//
// ToDo: Review count assumption with initialization of unused triggers/cams
// ToDo: Resolve angst over default input assignments.
static void setupDefaultSensorInputs() {
	// Engine rotation position sensors
	// Trigger is our primary timing signal, and usually comes from the crank.
	// trigger inputs up TRIGGER_SUPPORTED_CHANNELS (2)
	engineConfiguration->triggerInputPins[0] = GPIOC_6;
	engineConfiguration->triggerInputPins[1] = GPIO_UNASSIGNED;
	// A secondary Cam signal up to CAM_INPUTS_COUNT (4)
	engineConfiguration->camInputs[0] = GPIOA_5;

	// Throttle Body Position Sensors, second channel is a check/fail-safe
	// tps = "20 - AN volt 5"
	engineConfiguration->tps1_1AdcChannel = EFI_ADC_13;
	engineConfiguration->tps2_1AdcChannel = EFI_ADC_NONE;

	// Throttle pedal inputs
	// Idle/Up/Closed (no pressure on pedal) pin
	engineConfiguration->throttlePedalUpPin = GPIO_UNASSIGNED;
	// If the ETB has analog feedback we can use it for closed loop control.
	engineConfiguration->throttlePedalPositionAdcChannel = EFI_ADC_2;
  
	// Manifold Air Pressure sensor input
	// EFI_ADC_10: "27 - AN volt 1"
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_10;

	// Air Fuel Ratio (exhaust gas oxygen) sensor input
	// EFI_ADC_14: "32 - AN volt 6"
	engineConfiguration->afr.hwChannel = EFI_ADC_14;

	// Coolant Temp
	// clt = "18 - AN temp 1"
	engineConfiguration->clt.adcChannel = EFI_ADC_0;
	engineConfiguration->clt.config.bias_resistor = 2700;

	// Intake Air Temperature, IAT
	// iat = "23 - AN temp 2"
	engineConfiguration->iat.adcChannel = EFI_ADC_1;
	engineConfiguration->iat.config.bias_resistor = 2700;
}

void setPinConfigurationOverrides(void) {
}

// Future: configure USART3 for LIN bus and UART4 for console
void setSerialConfigurationOverrides(void) {
	engineConfiguration->useSerialPort = false;
	engineConfiguration->binarySerialTxPin = GPIO_UNASSIGNED;
	engineConfiguration->binarySerialRxPin = GPIO_UNASSIGNED;
//	engineConfiguration->consoleSerialTxPin = GPIO_UNASSIGNED;
//	engineConfiguration->consoleSerialRxPin = GPIO_UNASSIGNED;
}


/**
 * @brief   Board-specific configuration overrides.
 *
 * See also setDefaultEngineConfiguration
 *
 * @todo    Add any board-specific code
 */
void setBoardDefaultConfiguration(void) {

	// Set indicator LED pins.
	// This is often redundant with efifeatures.h or the run-time config
	engineConfiguration->triggerErrorPin = GPIOE_1;
	engineConfiguration->communicationLedPin = GPIOE_2;
	engineConfiguration->runningLedPin = GPIOE_4;
	engineConfiguration->warningLedPin = GPIOE_5;
	engineConfiguration->errorLedPin = GPIOE_7;

	// Set injector pins and the pin output mode
	engineConfiguration->injectionPinMode = OM_DEFAULT;
	engineConfiguration->injectionPins[0] = GPIOE_14;
	engineConfiguration->injectionPins[1] = GPIOE_13;
	engineConfiguration->injectionPins[2] = GPIOE_12;
	engineConfiguration->injectionPins[3] = GPIOE_11;
	// Disable the remainder only when they may never be assigned
	for (int i = 4; i < MAX_CYLINDER_COUNT;i++) {
		engineConfiguration->injectionPins[i] = GPIO_UNASSIGNED;
	}

	// Do the same for ignition outputs
	engineConfiguration->ignitionPinMode = OM_DEFAULT;
	engineConfiguration->ignitionPins[0] = GPIOD_4;
	engineConfiguration->ignitionPins[1] = GPIOD_3;
	engineConfiguration->ignitionPins[2] = GPIOD_2;
	engineConfiguration->ignitionPins[3] = GPIOD_1;
	// Disable remainder
	for (int i = 4; i < MAX_CYLINDER_COUNT; i++) {
		engineConfiguration->ignitionPins[i] = GPIO_UNASSIGNED;
	}

	// Board-specific scaling values to convert ADC fraction to Volts.
	// It is good practice to make the math explicit, but still simple.
	// The results should be compile time constants

	// The ADC reference voltage
	engineConfiguration->adcVcc = 3.30f;

	// This is a board with 6.8 Kohm and 10 Kohm resistor dividers
	engineConfiguration->analogInputDividerCoefficient = (10.0+6.8) / 10.0f;

	// Vbatt is the voltage of the 12V battery.
	// Here the hardware has a 39 Kohm high side/10 Kohm low side divider,
	// with the second divider also applied.
	engineConfiguration->vbattDividerCoeff =
	  (49.0f / 10.0f) * engineConfiguration->analogInputDividerCoefficient;
	engineConfiguration->vbattAdcChannel = EFI_ADC_11;

	// Configure any special on-board chips
	setupTle8888();
	setupEtb();

	// The MRE uses the TLE8888 fixed-function main relay control pin.
	// This firmware is not involved with main relay control, although
	// the pin inputs can be over-ridden through the TLE8888 Cmd0 register.
	// ToDo: consider EFI_MAIN_RELAY_CONTROL to FALSE for MRE configuration

	// Configure the TLE8888 half bridges (pushpull, lowside, or high-low)
	// TLE8888_IN11 -> TLE8888_OUT21
	// TLE8888_PIN_21: "35 - GP Out 1"
	engineConfiguration->fuelPumpPin = TLE8888_PIN_21;


	// TLE8888 high current low side: VVT2 IN9 / OUT5
	// TLE8888_PIN_4: "3 - Lowside 2"
	engineConfiguration->idle.solenoidPin = TLE8888_PIN_5;


	// TLE8888_PIN_22: "34 - GP Out 2"
	engineConfiguration->fanPin = TLE8888_PIN_22;

	// The "required" hardware is done - set some reasonable input defaults
	setupDefaultSensorInputs();

	// Some sensible defaults for other options
	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);
	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL_60_2;
	engineConfiguration->useOnlyRisingEdgeForTrigger = true;
	setAlgorithm(LM_SPEED_DENSITY);

	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->specs.firingOrder = FO_1_3_4_2;

	// Ign is IM_ONE_COIL, IM_TWO_COILS, IM_INDIVIDUAL_COILS, IM_WASTED_SPARK
	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;
	// Inj mode: IM_SIMULTANEOUS, IM_SEQUENTIAL, IM_BATCH, IM_SINGLE_POINT
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	engineConfiguration->injectionMode = IM_SIMULTANEOUS;
}

/*
 * Local variables:
 *  c-basic-indent: 4
 *  tab-width: 4
 * End:
 */
