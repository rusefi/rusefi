/*
 * @file mems.cpp
 *
 * stm32f4discovery has MEMS LIS302DL
 *
 * SPI1
 * LIS302DL_SPI_SCK PA5
 * LIS302DL_SPI_MISO PA6
 * LIS302DL_SPI_MOSI PA7
 * LIS302DL_SPI_CS_PIN PE3
 *
 * @date Aug 17, 2017
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#include "mems.h"
#include "lis302dl.h"
#include "hardware.h"

EXTERN_ENGINE;

void initMemsPins(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	engineConfiguration->LIS302DLCsPin = GPIOE_3;
	boardConfiguration->is_enabled_spi_1 = true;
}

#if EFI_MEMS || defined(__DOXYGEN__)

static SPIDriver *spip = &SPID1; // todo: make this configurable
static spi_device_e device = SPI_DEVICE_1;
static OutputPin memsCs;

void initMems(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	if (engineConfiguration->LIS302DLCsPin == GPIOA_0)
		return; // temporary code to handle old configurations
	if (engineConfiguration->LIS302DLCsPin == GPIO_UNASSIGNED)
		return; // not used

	turnOnSpi(device);

	memsCs.initPin("LIS302 CS", engineConfiguration->LIS302DLCsPin);


	/* LIS302DL initialization.*/
	lis302dlWriteRegister(spip, LIS302DL_CTRL_REG1, 0x43);
	lis302dlWriteRegister(spip, LIS302DL_CTRL_REG2, 0x00);
	lis302dlWriteRegister(spip, LIS302DL_CTRL_REG3, 0x00);

	int8_t x = (int8_t)lis302dlReadRegister(spip, LIS302DL_OUTX);
	int8_t y = (int8_t)lis302dlReadRegister(spip, LIS302DL_OUTY);

}

#endif /* EFI_MEMS */
