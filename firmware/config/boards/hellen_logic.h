// file hellen_logic.h

#pragma once

#include "../../board_id/boards_id.h"

void hellenWbo();

void setHellenMegaEnPin(bool enableBoardOnStartUp = true);
void setHellenEnPin(Gpio pin, bool enableBoardOnStartUp = true);
void setHellen64MegaEnPin();
bool isBoardWithPowerManagement();
bool getHellenBoardEnabled();
void hellenEnableEn(const char *msg = "");
void hellenDisableEn(const char *msg = "");
void hellenDisableEnSilently(); // this version is called by fatal error handler meaning no OS access

void hellenBoardStandBy();
void hellenMegaSdWithAccelerometer();
void hellenMegaModule();
void hellenMegaAccelerometerPreInitCS2Pin();
void configureHellenCanTerminator();

void setHellenCan();
void setHellenCan2();
void setHellen64Can();

void setHellenAnalogDividers();
void setHellenVbatt();

int detectHellenBoardId();
void detectHellenBoardType();

int boardGetAnalogDiagnostic();

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
static void enableHellenSpi1() {
	engineConfiguration->spi1mosiPin = Gpio::H_SPI1_MOSI;
	engineConfiguration->spi1misoPin = Gpio::H_SPI1_MISO;
	engineConfiguration->spi1sckPin = Gpio::H_SPI1_SCK;
	engineConfiguration->is_enabled_spi_1 = true;
}

static void setHellenSdCardSpi1Hardware() {
  engineConfiguration->sdCardCsPin = Gpio::H_SPI1_CS1;
  engineConfiguration->sdCardSpiDevice = SPI_DEVICE_1;
	enableHellenSpi1();
}

static void enableHellenSpi3() {
	engineConfiguration->spi3mosiPin = H_SPI3_MOSI;
	engineConfiguration->spi3misoPin = H_SPI3_MISO;
	engineConfiguration->spi3sckPin = H_SPI3_SCK;
	engineConfiguration->is_enabled_spi_3 = true;
}

static void setHellenSdCardSpi3NoCS() {
    engineConfiguration->isSdCardEnabled = true;
	engineConfiguration->sdCardSpiDevice = SPI_DEVICE_3;
	enableHellenSpi3();
}

static void setAccelerometerSpi() {
	/* accelerometer SPI is shared with SD card SPI on mm144 */
	engineConfiguration->accelerometerSpiDevice = SPI_DEVICE_1;
	engineConfiguration->accelerometerCsPin = Gpio::H_SPI1_CS2;
}

static void setHellen64SdCardSpi() {
    engineConfiguration->isSdCardEnabled = true;
	engineConfiguration->sdCardSpiDevice = SPI_DEVICE_3;
	engineConfiguration->spi3mosiPin = Gpio::MM64_SPI3_MOSI;
	engineConfiguration->spi3misoPin = Gpio::MM64_SPI3_MISO;
	engineConfiguration->spi3sckPin = Gpio::MM64_SPI3_SCK;
	engineConfiguration->is_enabled_spi_3 = true;
	engineConfiguration->sdCardCsPin = Gpio::MM64_SPI3_CS;
}

static void enableHellenSpi2() {
	engineConfiguration->is_enabled_spi_2 = true;
	engineConfiguration->spi2mosiPin = H_SPI2_MOSI;
	engineConfiguration->spi2misoPin = H_SPI2_MISO;
	engineConfiguration->spi2sckPin = H_SPI2_SCK;
}

inline void setHellenSdCardSpi2() {
  enableHellenSpi2();
	engineConfiguration->sdCardSpiDevice = SPI_DEVICE_2;
	engineConfiguration->sdCardCsPin = H_SPI2_CS;
}

inline void setHellenSdCardSpi3() {
    setHellenSdCardSpi3NoCS();
	engineConfiguration->sdCardCsPin = H_SPI3_CS;
}

// *pullups* files CLT R211 IAT R213
#define HELLEN_DEFAULT_AT_PULLUP 4700

inline void setDefaultHellenAtPullUps(float pullup = HELLEN_DEFAULT_AT_PULLUP) {
	engineConfiguration->clt.config.bias_resistor = pullup;
	engineConfiguration->iat.config.bias_resistor = pullup;
	engineConfiguration->auxTempSensor1.config.bias_resistor = pullup;
	engineConfiguration->auxTempSensor2.config.bias_resistor = pullup;
}

inline void setHellenMMbaro() {
	engineConfiguration->lps25BaroSensorScl = Gpio::B10;
	engineConfiguration->lps25BaroSensorSda = Gpio::B11;
}
#pragma GCC diagnostic pop
