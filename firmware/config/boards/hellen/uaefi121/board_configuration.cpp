/**
 * See https://rusefi.com/s/uaefi121
 *
 * @author Andrey Belomutskiy, (c) 2012-2023
 */

#include "pch.h"
#include "defaults.h"
#include "hellen_meta.h"
#include "hellen_leds_100.cpp"

static void setupDefaultSensorInputs() {
	engineConfiguration->tps1_1AdcChannel = MM100_IN_TPS_ANALOG;
	engineConfiguration->tps1_2AdcChannel = MM100_IN_AUX1_ANALOG;
	engineConfiguration->map.sensor.hwChannel = MM100_IN_MAP1_ANALOG;

	setPPSInputs(MM100_IN_PPS_ANALOG, MM100_IN_AUX2_ANALOG);

	engineConfiguration->clt.adcChannel = MM100_IN_CLT_ANALOG;
	engineConfiguration->iat.adcChannel = MM100_IN_IAT_ANALOG;
}

void setBoardConfigOverrides() {
	setHellenMegaEnPin();
	setHellenVbatt();

	hellenMegaAccelerometerPreInitCS2Pin();
  engineConfiguration->isSdCardEnabled = true;
  engineConfiguration->sdCardCsPin = Gpio::H_SPI1_CS1;
	engineConfiguration->sdCardSpiDevice = SPI_DEVICE_1;
	engineConfiguration->spi1mosiPin = Gpio::A7; // not the usual H_SPI1_MOSI since that's CAN2RX
	engineConfiguration->spi1misoPin = Gpio::H_SPI1_MISO;
	engineConfiguration->spi1sckPin = Gpio::H_SPI1_SCK;
	engineConfiguration->is_enabled_spi_1 = true;
	setDefaultHellenAtPullUps();

	engineConfiguration->canTxPin = Gpio::B13;
	engineConfiguration->canRxPin = Gpio::B5;


}

void setBoardDefaultConfiguration() {
		engineConfiguration->injectionPins[0] = Gpio::MM100_INJ1;
  	engineConfiguration->injectionPins[1] = Gpio::MM100_INJ2;
  	engineConfiguration->injectionPins[2] = Gpio::MM100_INJ3;
  	engineConfiguration->injectionPins[3] = Gpio::MM100_INJ4;
  	engineConfiguration->injectionPins[4] = Gpio::MM100_INJ5;
  	engineConfiguration->injectionPins[5] = Gpio::MM100_INJ6;

	engineConfiguration->ignitionPins[0] = Gpio::MM100_IGN1;
	engineConfiguration->ignitionPins[1] = Gpio::MM100_IGN2;
	engineConfiguration->ignitionPins[2] = Gpio::MM100_IGN3;
	engineConfiguration->ignitionPins[3] = Gpio::MM100_IGN4;
	engineConfiguration->ignitionPins[4] = Gpio::MM100_IGN5;
	engineConfiguration->ignitionPins[5] = Gpio::MM100_IGN6;

    setupDefaultSensorInputs();
}

int getBoardMetaDcOutputsCount() {
    // todo return 2;
    return 0;
}

static Gpio OUTPUTS[] = {
	Gpio::MM100_INJ1,
	Gpio::MM100_INJ2,
	Gpio::MM100_INJ3,
};

int getBoardMetaOutputsCount() {
    return efi::size(OUTPUTS);
}

Gpio* getBoardMetaOutputs() {
    return OUTPUTS;
}
