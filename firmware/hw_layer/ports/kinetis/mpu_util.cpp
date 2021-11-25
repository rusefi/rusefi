/**
 * @file	mpu_util.cpp
 *
 * @date Jul 27, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 * @author andreika <prometheus.pcb@gmail.com>
 */

#include "pch.h"

#if EFI_PROD_CODE

#include "mpu_util.h"
#include "flash_int.h"
#include "os_util.h"

void baseMCUInit(void) {
}

void _unhandled_exception(void) {
/*lint -restore*/

  chDbgPanic3("_unhandled_exception", __FILE__, __LINE__);
  while (true) {
  }
}

void DebugMonitorVector(void) {
	chDbgPanic3("DebugMonitorVector", __FILE__, __LINE__);
	while (TRUE)
		;
}

void UsageFaultVector(void) {
	chDbgPanic3("UsageFaultVector", __FILE__, __LINE__);
	while (TRUE)
		;
}

void BusFaultVector(void) {
	chDbgPanic3("BusFaultVector", __FILE__, __LINE__);
	while (TRUE) {
	}
}

void HardFaultVector(void) {
	while (TRUE) {
	}
}

#if HAL_USE_SPI || defined(__DOXYGEN__)
bool isSpiInitialized[5] = { false, false, false, false, false };

static int getSpiAf(SPIDriver *driver) {
#if STM32_SPI_USE_SPI1
	if (driver == &SPID1) {
		return EFI_SPI1_AF;
	}
#endif
#if STM32_SPI_USE_SPI2
	if (driver == &SPID2) {
		return EFI_SPI2_AF;
	}
#endif
#if STM32_SPI_USE_SPI3
	if (driver == &SPID3) {
		return EFI_SPI3_AF;
	}
#endif
	return -1;
}

brain_pin_e getMisoPin(spi_device_e device) {
	switch(device) {
	case SPI_DEVICE_1:
		return engineConfiguration->spi1misoPin;
	case SPI_DEVICE_2:
		return engineConfiguration->spi2misoPin;
	case SPI_DEVICE_3:
		return engineConfiguration->spi3misoPin;
	default:
		break;
	}
	return GPIO_UNASSIGNED;
}

brain_pin_e getMosiPin(spi_device_e device) {
	switch(device) {
	case SPI_DEVICE_1:
		return engineConfiguration->spi1mosiPin;
	case SPI_DEVICE_2:
		return engineConfiguration->spi2mosiPin;
	case SPI_DEVICE_3:
		return engineConfiguration->spi3mosiPin;
	default:
		break;
	}
	return GPIO_UNASSIGNED;
}

brain_pin_e getSckPin(spi_device_e device) {
	switch(device) {
	case SPI_DEVICE_1:
		return engineConfiguration->spi1sckPin;
	case SPI_DEVICE_2:
		return engineConfiguration->spi2sckPin;
	case SPI_DEVICE_3:
		return engineConfiguration->spi3sckPin;
	default:
		break;
	}
	return GPIO_UNASSIGNED;
}

void turnOnSpi(spi_device_e device) {
	if (isSpiInitialized[device])
		return; // already initialized
	isSpiInitialized[device] = true;
	if (device == SPI_DEVICE_1) {
// todo: introduce a nice structure with all fields for same SPI
#if STM32_SPI_USE_SPI1
//	scheduleMsg(&logging, "Turning on SPI1 pins");
		initSpiModule(&SPID1, getSckPin(device),
				getMisoPin(device),
				getMosiPin(device),
				engineConfiguration->spi1SckMode,
				engineConfiguration->spi1MosiMode,
				engineConfiguration->spi1MisoMode);
#endif /* STM32_SPI_USE_SPI1 */
	}
	if (device == SPI_DEVICE_2) {
#if STM32_SPI_USE_SPI2
//	scheduleMsg(&logging, "Turning on SPI2 pins");
		initSpiModule(&SPID2, getSckPin(device),
				getMisoPin(device),
				getMosiPin(device),
				engineConfiguration->spi2SckMode,
				engineConfiguration->spi2MosiMode,
				engineConfiguration->spi2MisoMode);
#endif /* STM32_SPI_USE_SPI2 */
	}
	if (device == SPI_DEVICE_3) {
#if STM32_SPI_USE_SPI3
//	scheduleMsg(&logging, "Turning on SPI3 pins");
		initSpiModule(&SPID3, getSckPin(device),
				getMisoPin(device),
				getMosiPin(device),
				engineConfiguration->spi3SckMode,
				engineConfiguration->spi3MosiMode,
				engineConfiguration->spi3MisoMode);
#endif /* STM32_SPI_USE_SPI3 */
	}
}

void initSpiModule(SPIDriver *driver, brain_pin_e sck, brain_pin_e miso,
		brain_pin_e mosi,
		int sckMode,
		int mosiMode,
		int misoMode) {

	/**
	 * See https://github.com/rusefi/rusefi/pull/664/
	 *
	 * Info on the silicon defect can be found in this document, section 2.5.2:
	 * https://www.st.com/content/ccc/resource/technical/document/errata_sheet/0a/98/58/84/86/b6/47/a2/DM00037591.pdf/files/DM00037591.pdf/jcr:content/translations/en.DM00037591.pdf
	 */
	efiSetPadMode("SPI clock", sck,	PAL_MODE_ALTERNATE(getSpiAf(driver)) | sckMode | PAL_STM32_OSPEED_HIGHEST);

	efiSetPadMode("SPI master out", mosi, PAL_MODE_ALTERNATE(getSpiAf(driver)) | mosiMode | PAL_STM32_OSPEED_HIGHEST);
	efiSetPadMode("SPI master in ", miso, PAL_MODE_ALTERNATE(getSpiAf(driver)) | misoMode | PAL_STM32_OSPEED_HIGHEST);
}

void initSpiCs(SPIConfig *spiConfig, brain_pin_e csPin) {
	spiConfig->end_cb = NULL;
	ioportid_t port = getHwPort("spi", csPin);
	ioportmask_t pin = getHwPin("spi", csPin);
	spiConfig->ssport = port;
	spiConfig->sspad = pin;
	// CS is controlled inside 'hal_spi_lld' driver using both software and hardware methods.
	//efiSetPadMode("chip select", csPin, PAL_MODE_OUTPUT_OPENDRAIN);
}

#endif /* HAL_USE_SPI */

BOR_Level_t BOR_Get(void) {
	return BOR_Level_None;
}

BOR_Result_t BOR_Set(BOR_Level_t BORValue) {
	return BOR_Result_Ok;
}

#if EFI_CAN_SUPPORT || defined(__DOXYGEN__)

static bool isValidCan1RxPin(brain_pin_e pin) {
	return pin == GPIOA_11 || pin == GPIOB_8 || pin == GPIOD_0;
}

static bool isValidCan1TxPin(brain_pin_e pin) {
	return pin == GPIOA_12 || pin == GPIOB_9 || pin == GPIOD_1;
}

static bool isValidCan2RxPin(brain_pin_e pin) {
	return pin == GPIOB_5 || pin == GPIOB_12;
}

static bool isValidCan2TxPin(brain_pin_e pin) {
	return pin == GPIOB_6 || pin == GPIOB_13;
}

bool isValidCanTxPin(brain_pin_e pin) {
   return isValidCan1TxPin(pin) || isValidCan2TxPin(pin);
}

bool isValidCanRxPin(brain_pin_e pin) {
   return isValidCan1RxPin(pin) || isValidCan2RxPin(pin);
}

CANDriver * detectCanDevice(brain_pin_e pinRx, brain_pin_e pinTx) {
   if (isValidCan1RxPin(pinRx) && isValidCan1TxPin(pinTx))
      return &CAND1;
   if (isValidCan2RxPin(pinRx) && isValidCan2TxPin(pinTx))
      return &CAND2;
   return NULL;
}

#endif /* EFI_CAN_SUPPORT */

bool allowFlashWhileRunning() {
	return false;
}

size_t flashSectorSize(flashsector_t sector) {
	// sectors 0..11 are the 1st memory bank (1Mb), and 12..23 are the 2nd (the same structure).
	if (sector <= 3 || (sector >= 12 && sector <= 15))
		return 16 * 1024;
	else if (sector == 4 || sector == 16)
		return 64 * 1024;
	else if ((sector >= 5 && sector <= 11) || (sector >= 17 && sector <= 23))
		return 128 * 1024;
	return 0;
}


/**
 * Flex Non Volatile Memory is faster than flash
 * It also has smaller pages so it takes less time to erase
 *
 * There is no remote access to FlexNVM meaning that we cannot erase settings externally
 */

uintptr_t getFlashAddrFirstCopy() {
	return 0x10000000;
}

uintptr_t getFlashAddrSecondCopy() {
	return 0x10008000;
}

/*static*/ hardware_pwm* hardware_pwm::tryInitPin(const char*, brain_pin_e, float, float) {
	// TODO: implement me!
	return nullptr;
}

void portInitAdc() {
	// Init slow ADC
	adcStart(&ADCD1, NULL);

	// Init fast ADC (MAP sensor)
	adcStart(&ADCD2, NULL);
}

float getMcuTemperature() {
	// TODO: implement me!
	return 0;
}

bool readSlowAnalogInputs(adcsample_t* convertedSamples) {
	// TODO: implement me!
	return true;
}

static constexpr FastAdcToken invalidToken = (FastAdcToken)(-1);

FastAdcToken enableFastAdcChannel(const char*, adc_channel_e channel) {
	if (!isAdcChannelValid(channel)) {
		return invalidToken;
	}

	// TODO: implement me!
	return invalidToken;
}

adcsample_t getFastAdc(FastAdcToken token) {
	if (token == invalidToken) {
		return 0;
	}

	// TODO: implement me!
	return 0;
}

#endif /* EFI_PROD_CODE */
