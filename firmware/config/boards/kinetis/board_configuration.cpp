/**
 * @file boards/kinetis/board_configuration.h
 *
 * @brief In this file we can override engine_configuration.cpp.
 *
 * @date Mar 22, 2019
 * @author andreika, (c) 2019
 */

#include "pch.h"
#include "board_overrides.h"

#if 0
char __debugBuffer[80];
int __debugEnabled = 0;
#endif

Gpio getWarningLedPin() {
	// this board has no warning led
	return Gpio::Unassigned;
}

Gpio getRunningLedPin() {
	return Gpio::Unassigned;
}

Gpio getCommsLedPin() {
	// this board has no comms led
	return Gpio::Unassigned;
}

static void setSerialConfigurationOverrides() {
	engineConfiguration->binarySerialTxPin = Gpio::C7;
	engineConfiguration->binarySerialRxPin = Gpio::C6;
//	engineConfiguration->consoleSerialTxPin = Gpio::A10;
//	engineConfiguration->consoleSerialRxPin = Gpio::A11;
	engineConfiguration->tunerStudioSerialSpeed = SERIAL_SPEED;
	engineConfiguration->uartConsoleSerialSpeed = SERIAL_SPEED;
}

static void kinetis_boardConfigOverrides() {
	engineConfiguration->useNoiselessTriggerDecoder = true;

	setAlgorithm(engine_load_mode_e::LM_SPEED_DENSITY);

	engineConfiguration->cylindersCount = 4;
	engineConfiguration->firingOrder = FO_1_3_4_2;

	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	engineConfiguration->injectionMode = IM_SIMULTANEOUS;

	engineConfiguration->globalTriggerAngleOffset = 114;	// the end of 19th tooth?

	engineConfiguration->displacement = 1.645;
	engineConfiguration->injector.flow = 200;

	setTable(config->crankingCycleBaseFuel, 25);		// ???
	engineConfiguration->cranking.rpm = 600;

	engineConfiguration->map.sensor.type = MT_MPX4250A;

	engineConfiguration->idleStepperReactionTime = 10;
	engineConfiguration->stepperDirectionPinMode = OM_INVERTED;

	engineConfiguration->useLinearCltSensor = true;
	// todo:
	engineConfiguration->clt.config.resistance_1 = 0;
	engineConfiguration->clt.config.tempC_1 = -40.0f;
	engineConfiguration->clt.config.resistance_2 = 5.0f;
	engineConfiguration->clt.config.tempC_2 = 120.0f,
	engineConfiguration->clt.config.bias_resistor = 3300;

	engineConfiguration->tpsMin = convertVoltageTo10bitADC(0.250);
	engineConfiguration->tpsMax = convertVoltageTo10bitADC(4.538);
	engineConfiguration->tpsErrorDetectionTooLow = -10; // -10% open
	engineConfiguration->tpsErrorDetectionTooHigh = 110; // 110% open

	engineConfiguration->mapMinBufferLength = 4;

	engineConfiguration->clt.adcChannel = EFI_ADC_14;

	engineConfiguration->triggerInputPins[0] = Gpio::E7;
	engineConfiguration->triggerInputPins[1] = Gpio::Unassigned;

	engineConfiguration->tle6240spiDevice = SPI_DEVICE_1;
	engineConfiguration->tle6240_cs = Gpio::B0;

	// todo:
	int i;
	for (i = 0; i < MAX_CYLINDER_COUNT; i++)
		engineConfiguration->injectionPins[i] = Gpio::Unassigned;
	for (i = 0; i < MAX_CYLINDER_COUNT; i++)
		engineConfiguration->ignitionPins[i] = Gpio::Unassigned;

	engineConfiguration->adcVcc = 5.0f;
	engineConfiguration->analogInputDividerCoefficient = 1;

	//engineConfiguration->isFastAdcEnabled = false;

	// we call it here because setDefaultBoardConfiguration() is not called for DEFAULT_ENGINE_TYPE=MINIMAL_PINS
	setSerialConfigurationOverrides();
}

void setAdcChannelOverrides() {
	// on Kinetis, ADC_FAST & SLOW are not really "fast" or "slow",
	// they are just different ADC numbers with different sets of channels
	removeChannel("VBatt", engineConfiguration->vbattAdcChannel);
	addFastAdcChannel("VBatt", engineConfiguration->vbattAdcChannel);

	removeChannel("TPS", engineConfiguration->tps1_1AdcChannel);
	addFastAdcChannel("TPS", engineConfiguration->tps1_1AdcChannel);
}

#include <setjmp.h>

void longjmp(jmp_buf /*env*/, int /*status*/) {
	// noop, but noreturn
	while (1) { }
}

int setjmp(jmp_buf /*env*/) {
	// Fake return 0, not implemented
	return 0;
}

void setup_custom_board_overrides() {
	custom_board_ConfigOverrides = kinetis_boardConfigOverrides;
}
