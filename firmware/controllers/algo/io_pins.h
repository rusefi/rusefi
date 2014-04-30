/**
 * @file	io_pins.h
 *
 * @date Jan 24, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef STATUS_LEDS_H_
#define STATUS_LEDS_H_

#include "main.h"

#define GPIO_NULL NULL

typedef enum {
	LED_CRANKING, // Orange on-board LED
	LED_RUNNING, // Green on-board LED
	LED_ERROR, // Red on-board LED
	LED_COMMUNICATION_1, // Blue on-board LED
	LED_EXT_1, // external board LED
	LED_EXT_2, // external board LED
	LED_EXT_3, // external board LED
	LED_DEBUG,
	LED_EMULATOR,

	IDLE_VALVE,
	TRIGGER_EMULATOR_PRIMARY,
	TRIGGER_EMULATOR_SECONDARY,

	SPARKOUT_1_OUTPUT,
	SPARKOUT_2_OUTPUT,
	SPARKOUT_3_OUTPUT,
	SPARKOUT_4_OUTPUT,
	SPARKOUT_5_OUTPUT,
	SPARKOUT_6_OUTPUT,
	SPARKOUT_7_OUTPUT,
	SPARKOUT_8_OUTPUT,
	SPARKOUT_9_OUTPUT,
	SPARKOUT_10_OUTPUT,
	SPARKOUT_11_OUTPUT,
	SPARKOUT_12_OUTPUT,

	INJECTOR_1_OUTPUT,
	INJECTOR_2_OUTPUT,
	INJECTOR_3_OUTPUT,
	INJECTOR_4_OUTPUT,
	INJECTOR_5_OUTPUT,
	INJECTOR_6_OUTPUT,
	INJECTOR_7_OUTPUT,
	INJECTOR_8_OUTPUT,
	INJECTOR_9_OUTPUT,
	INJECTOR_10_OUTPUT,
	INJECTOR_11_OUTPUT,
	INJECTOR_12_OUTPUT,

	ELECTRONIC_THROTTLE_CONTROL_1,
	ELECTRONIC_THROTTLE_CONTROL_2,
	ELECTRONIC_THROTTLE_CONTROL_3,

	/* digit 1 */
	LED_HUGE_0, // B2
	LED_HUGE_1,
	LED_HUGE_2,
	LED_HUGE_3,
	LED_HUGE_4,
	LED_HUGE_5,
	LED_HUGE_6,
	/* digit 2 */
	LED_HUGE_7,
	LED_HUGE_8,
	LED_HUGE_9, // E15
	LED_HUGE_10,
	LED_HUGE_11,
	LED_HUGE_12,
	LED_HUGE_13,
	/* digit 3 */
	LED_HUGE_14,
	LED_HUGE_15,
	LED_HUGE_16,
	LED_HUGE_17,
	LED_HUGE_18,
	LED_HUGE_19,
	LED_HUGE_20,

	// malfunction LED indicator - CheckEngine
	LED_CHECK_ENGINE,
	
	FUEL_PUMP_RELAY,
	FAN_RELAY,

	SPI_CS_1,
	SPI_CS_2,
	SPI_CS_3,
	SPI_CS_4,
	SPI_CS_SD_MODULE,


	/**
	 * This output pin is used to turn alternator on or off
	 */
	ALTERNATOR_SWITCH,

} io_pin_e;

#define IO_PIN_COUNT 100

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

void initPrimaryPins(void);
void initOutputPins(void);
char *getPinName(io_pin_e io_pin);
void turnOutputPinOn(io_pin_e pin);
void turnOutputPinOff(io_pin_e pin);
void setOutputPinValue(io_pin_e pin, int logicValue);
int getElectricalValue(int logicalValue, pin_output_mode_e mode);
int getOutputPinValue(io_pin_e pin);
void setDefaultPinState(io_pin_e pin, pin_output_mode_e *defaultState);

void outputPinRegisterExt2(const char *msg, io_pin_e ioPin, brain_pin_e brainPin, pin_output_mode_e *outputMode);
#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* STATUS_LEDS_H_ */
