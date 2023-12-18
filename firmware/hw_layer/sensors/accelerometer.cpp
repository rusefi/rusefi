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
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "accelerometer.h"
#include "hardware.h"

#if EFI_ONBOARD_MEMS
#include "mpu_util.h"
#include "lis302dl.h"
#include "periodic_thread_controller.h"

#if (EFI_ONBOARD_MEMS_LIS2DW12 == TRUE)
#include "lis2dw12.h"
#endif
#if (EFI_ONBOARD_MEMS_LIS2DH12 == TRUE)
#include "lsm303agr.h"
#endif

/*
 * SPI1 configuration structure.
 * Speed 5.25MHz, CPHA=1, CPOL=1, 8bits frames, MSb transmitted first.
 */
static SPIConfig accelerometerSpiCfg = {
#if SPI_SUPPORTS_CIRCULAR == TRUE
	.circular = FALSE,
#endif
	.end_cb = NULL,
	/* HW dependent part.*/
	.ssport = NULL,
	.sspad = 0,
	.cr1 = SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_CPOL | SPI_CR1_CPHA |
		SPI_CR1_8BIT_MODE,
	.cr2 = SPI_CR2_8BIT_MODE
};


#if 0
	static OutputPin chipSelect;
#endif

#if (EFI_ONBOARD_MEMS_LIS2DW12 == TRUE)

static LIS2DW12Config lis2dw12cfg = {
#if LIS2DW12_USE_SPI
	.spip = NULL,
	.spicfg = &accelerometerSpiCfg,
#endif
#ifdef LIS2DW12_USE_I2C
	/* TODO: */
#endif
	.accsensitivity = NULL,
	.accbias = NULL,
	.accoutputdatarate = LIS2DW12_ACC_ODR_25HZ,
	.accoutputresolution = LIS2DW12_ACC_OR_HP,
	.acclowpowermode = LIS2DW12_ACC_LP_MODE2,
	.accbadwidthselect = LIS2DW12_ACC_BW_ODR4,
	.accfullscale = LIS2DW12_ACC_FS_4G,
};

/* LIS302DL Driver: This object represent an LIS302DL instance */
static LIS2DW12Driver LIS2DW12;

#endif //EFI_ONBOARD_MEMS_LIS2DW12 == TRUE

#if (EFI_ONBOARD_MEMS_LIS2DH12 == TRUE)

static LSM303AGRConfig lis2dh12cfg = {
#if LSM303AGR_USE_SPI
	.spip = NULL,
	.spicfg = &accelerometerSpiCfg,
#endif
	.accsensitivity = NULL,
	.accbias = NULL,
	.accfullscale = LSM303AGR_ACC_FS_4G,
	.accoutdatarate = LSM303AGR_ACC_ODR_50Hz,
#if LSM303AGR_USE_ADVANCED
	.accmode = LSM303AGR_ACC_MODE_HRES,
	.accblockdataupdate = LSM303AGR_ACC_BDU_CONT,
	.accendianess = LSM303AGR_ACC_END_LITTLE,
#endif
	/* LIS2DW12 is Accelerometer only, ignore following */
	.compsensitivity = NULL,
	.compbias = NULL,
	.compoutputdatarate = LSM303AGR_COMP_ODR_50HZ,
#if LSM303AGR_USE_ADVANCED
	.compmode = LSM303AGR_COMP_MODE_NORM,
	.complp = LSM303AGR_COMP_LPOW_EN
#endif
};

/* LSM303AGR Driver: This object represent an LIS2DH12 instance */
static LSM303AGRDriver LIS2DH12;

#endif //EFI_ONBOARD_MEMS_LIS2DH12 == TRUE

class AccelController : public PeriodicController<UTILITY_THREAD_STACK_SIZE> {
public:
	AccelController() : PeriodicController("Acc SPI") { }
private:
	void PeriodicTask(efitick_t nowNt) override	{
		float acccooked[3];

		#if (EFI_ONBOARD_MEMS_LIS2DW12 == TRUE)
			lis2dw12AccelerometerReadCooked(&LIS2DW12, acccooked);
		#endif
		#if (EFI_ONBOARD_MEMS_LIS2DH12 == TRUE)
			lsm303agrAccelerometerReadCooked(&LIS2DH12, acccooked);
		#endif

		if (engineConfiguration->useSpiImu) {
			/* milli-G to G */
			engine->sensors.accelerometer.lat  = acccooked[0] / 1000.0;
			engine->sensors.accelerometer.lon  = acccooked[1] / 1000.0;
			engine->sensors.accelerometer.vert = acccooked[2] / 1000.0;
		}
	}
};

static AccelController instance;

void initAccelerometer() {
	static SPIDriver *bus;
	msg_t ret = MSG_RESET;

#if HAL_USE_SPI
	if (!isBrainPinValid(engineConfiguration->accelerometerCsPin))
		return; // not used

	bus = getSpiDevice(engineConfiguration->accelerometerSpiDevice);
	if (bus == nullptr) {
		// error already reported
		return;
	}

	/* Commented until we have configureHellenMegaAccCS2Pin() */
	#if 0
		chipSelect.initPin("SPI Acc", engineConfiguration->accelerometerCsPin);
	#endif
	accelerometerSpiCfg.ssport = getHwPort("SPI Acc", engineConfiguration->accelerometerCsPin);
	accelerometerSpiCfg.sspad = getHwPin("SPI Acc", engineConfiguration->accelerometerCsPin);

#if (EFI_ONBOARD_MEMS_LIS2DW12 == TRUE)
	lis2dw12cfg.spip = bus;

	/* LIS302DL Object Initialization.*/
	lis2dw12ObjectInit(&LIS2DW12);

	/* Activates the LIS302DL driver.*/
	ret = lis2dw12Start(&LIS2DW12, &lis2dw12cfg);
#elif (EFI_ONBOARD_MEMS_LIS2DH12 == TRUE)
	lis2dh12cfg.spip = bus;

	/* LIS302DL Object Initialization.*/
	lsm303agrObjectInit(&LIS2DH12);

	/* Activates the LIS302DL driver.*/
	ret = lsm303agrStart(&LIS2DH12, &lis2dh12cfg);
#else
 fail("no MEMS type");
#endif /* EFI_ONBOARD_MEMS_LIS2DW12 == TRUE */

	/* TODO: add support for LIS302 on discovery board */

	if (ret == MSG_OK) {
		/* 50 Hz */
		instance.setPeriod(20 /*ms*/);
		instance.start();
		efiPrintf("accelerometer init OK");
	} else {
		efiPrintf("accelerometer init failed %d", ret);
	}
#endif /* HAL_USE_SPI */
}

#endif /* EFI_ONBOARD_MEMS */
