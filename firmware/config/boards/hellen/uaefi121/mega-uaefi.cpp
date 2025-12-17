#include "pch.h"
#include "defaults.h"
#include "hellen_meta.h"
#include "mega-uaefi.h"

// this is also used by mega121 module units like https://github.com/rusefi/fw-uaefi-Honda-OBD1
void setMegaUaefiBoardConfigOverrides() {
	setHellenMegaEnPin();
	setHellenVbatt();

	engineConfiguration->vrThreshold[0].pin = Gpio::MM100_OUT_PWM6;

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

void setMegaUaefiBoardDefaultConfiguration() {
	// GM stepper: DC2 positive#4/negative#5 to pins A/B
  // DC1 positive#1/negative#2 to pin D/C
  setupTLE9201IncludingStepper(/*PWM controlPin*/Gpio::MM100_OUT_PWM3, Gpio::MM100_OUT_PWM4, Gpio::MM100_SPI2_MISO);
  setupTLE9201IncludingStepper(/*PWM controlPin*/Gpio::MM100_OUT_PWM5, Gpio::MM100_SPI2_MOSI, Gpio::MM100_USB1ID, 1);

  setHellenMMbaro();
}
