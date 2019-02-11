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
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "accelerometer.h"
#include "hardware.h"

EXTERN_ENGINE;

#if EFI_MEMS || defined(__DOXYGEN__)
#include "mpu_util.h"
#include "lis302dl.h"
#include "PeriodicController.h"

static SPIDriver *driver;

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
//	engineConfiguration->LIS302DLCsPin = GPIOE_3; // we have a conflict with VVT output on Miata
// 	CONFIGB(is_enabled_spi_1) = true; // we have a conflict with PA5 input pin

	// stm32f4discovery defaults
	CONFIGB(spi1mosiPin) = GPIOA_7;
	CONFIGB(spi1misoPin) = GPIOA_6;
	CONFIGB(spi1sckPin) = GPIOA_5;
}


#if EFI_MEMS || defined(__DOXYGEN__)

static THD_WORKING_AREA(ivThreadStack, UTILITY_THREAD_STACK_SIZE);

class AccelController : public PeriodicController<UTILITY_THREAD_STACK_SIZE> {
public:
	AccelController() : PeriodicController("Acc SPI") { }
private:
	void PeriodicTask(efitime_t nowNt) override	{
		// has to be a thread since we want to use blocking method - blocking method only available in threads, not in interrupt handler
		// todo: migrate to async SPI API?
		engine->sensors.accelerometer.x = (int8_t)lis302dlReadRegister(driver, LIS302DL_OUTX);
		engine->sensors.accelerometer.y = (int8_t)lis302dlReadRegister(driver, LIS302DL_OUTY);
		chThdSleepMilliseconds(20);
	}
};

static BenchController instance;

void initAccelerometer(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	if (engineConfiguration->LIS302DLCsPin == GPIO_UNASSIGNED)
		return; // not used

	if (!CONFIGB(is_enabled_spi_1))
		return; // temporary
#if HAL_USE_SPI || defined(__DOXYGEN__)
	driver = getSpiDevice(engineConfiguration->accelerometerSpiDevice);

	turnOnSpi(engineConfiguration->accelerometerSpiDevice);
	spiStart(driver, &accelerometerCfg);
	initSpiCs((SPIConfig *)driver->config, engineConfiguration->LIS302DLCsPin);

//	memsCs.initPin("LIS302 CS", engineConfiguration->LIS302DLCsPin);
//	memsCfg.ssport = getHwPort("mmc", CONFIGB(sdCardCsPin));
//	memsCfg.sspad = getHwPin("mmc", CONFIGB(sdCardCsPin));


	/* LIS302DL initialization.*/
	lis302dlWriteRegister(driver, LIS302DL_CTRL_REG1, 0x47); // enable device, enable XYZ
	lis302dlWriteRegister(driver, LIS302DL_CTRL_REG2, 0x00); // 4 wire mode
	lis302dlWriteRegister(driver, LIS302DL_CTRL_REG3, 0x00);

	chThdCreateStatic(ivThreadStack, sizeof(ivThreadStack), NORMALPRIO, (tfunc_t)(void*) ivThread, NULL);
#endif /* HAL_USE_SPI */
}

#endif /* EFI_MEMS */


float getLongitudinalAcceleration(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	return engine->sensors.accelerometer.x;
}

float getTransverseAcceleration(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	return engine->sensors.accelerometer.y;
}
