/*
 * @file accelerometer.cpp
 *
 * stm32f4discovery has MEMS LIS302DL
 * www.st.com/resource/en/datasheet/lis302dl.pdf
 *
 * SPI1
 * LIS302DL_SPI_SCK PA5
 * LIS302DL_SPI_MISO PA6
 * LIS302DL_SPI_MOSI PA7
 * LIS302DL_SPI_CS_PIN PE3
 *
 *
 *
 * @date May 19, 2016
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#include "accelerometer.h"
#include "lis302dl.h"
#include "hardware.h"
#include "mpu_util.h"

EXTERN_ENGINE;

#if EFI_MEMS || defined(__DOXYGEN__)
static SPIDriver *driver = &SPID1; // todo: make this configurable
static spi_device_e device = SPI_DEVICE_1;

/*
 * SPI1 configuration structure.
 * Speed 5.25MHz, CPHA=1, CPOL=1, 8bits frames, MSb transmitted first.
 * The slave select line is the pin GPIOE_CS_SPI on the port GPIOE.
 */
static const SPIConfig accelerometerCfg = {
  NULL,
  /* HW dependent part.*/
  GPIOE,
  GPIOE_PIN3,
  SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_CPOL | SPI_CR1_CPHA
};
#endif /* EFI_MEMS */

void configureAccelerometerPins(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	engineConfiguration->LIS302DLCsPin = GPIOE_3;
// 	boardConfiguration->is_enabled_spi_1 = true; // we have a conflict with PA5 input pin

	// stm32f4discovery defaults
	boardConfiguration->spi1mosiPin = GPIOA_7;
	boardConfiguration->spi1misoPin = GPIOA_6;
	boardConfiguration->spi1sckPin = GPIOA_5;
}


#if EFI_MEMS || defined(__DOXYGEN__)

void accelerometerPeriodicCallback(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
//	int8_t x = (int8_t)lis302dlReadRegister(driver, LIS302DL_OUTX);
//	int8_t y = (int8_t)lis302dlReadRegister(driver, LIS302DL_OUTY);
}

void initMems(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	if (engineConfiguration->LIS302DLCsPin == GPIOA_0)
		return; // temporary code to handle old configurations
	if (engineConfiguration->LIS302DLCsPin == GPIO_UNASSIGNED)
		return; // not used

	// todo: driver = getSpiDevice(device);

	turnOnSpi(device);
	spiStart(driver, &accelerometerCfg);
	initSpiCs((SPIConfig *)driver->config, engineConfiguration->LIS302DLCsPin);

//	memsCs.initPin("LIS302 CS", engineConfiguration->LIS302DLCsPin);
//	memsCfg.ssport = getHwPort("mmc", boardConfiguration->sdCardCsPin);
//	memsCfg.sspad = getHwPin("mmc", boardConfiguration->sdCardCsPin);


	/* LIS302DL initialization.*/
	lis302dlWriteRegister(driver, LIS302DL_CTRL_REG1, 0x47); // enable device, enable XYZ
	lis302dlWriteRegister(driver, LIS302DL_CTRL_REG2, 0x00); // 4 wire mode
	lis302dlWriteRegister(driver, LIS302DL_CTRL_REG3, 0x00);


}

#endif /* EFI_MEMS */



void initAccelerometer(DECLARE_ENGINE_PARAMETER_SIGNATURE) {

}

float getLongitudinalAcceleration(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	return 0;
}

float getTransverseAcceleration(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	return 0;
}
