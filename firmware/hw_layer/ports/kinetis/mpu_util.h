/**
 * @file	mpu_util.h
 *
 * @date Jul 27, 2014
 * @author Andrey Belomutskiy, (c) 2012-2017
 * @author andreika <prometheus.pcb@gmail.com>
 */

#ifndef MPU_UTIL_H_
#define MPU_UTIL_H_

// we are lucky - all CAN pins use the same AF
#define EFI_CAN_RX_AF 9
#define EFI_CAN_TX_AF 9

// burnout or 'Burn Out'
typedef enum {
	BOR_Level_None = 0,
	BOR_Level_1 = 1,
	BOR_Level_2 = 2,
	BOR_Level_3 = 3
} BOR_Level_t;

typedef enum {
	BOR_Result_Ok = 0x00,
	BOR_Result_Error
} BOR_Result_t;

BOR_Level_t BOR_Get(void);
BOR_Result_t BOR_Set(BOR_Level_t BORValue);

#ifndef ADC_TwoSamplingDelay_5Cycles
#define ADC_TwoSamplingDelay_5Cycles ((uint32_t)0x00000000)
#endif

#ifndef ADC_TwoSamplingDelay_20Cycles
#define ADC_TwoSamplingDelay_20Cycles ((uint32_t)0x00000F00)
#endif

#ifndef ADC_CR2_SWSTART
#define ADC_CR2_SWSTART ((uint32_t)0x40000000)
#endif

#define SPI_CR1_16BIT_MODE SPI_CR1_DFF
#define SPI_CR2_16BIT_MODE 0

// TODO
#define SPI_CR1_24BIT_MODE 0
#define SPI_CR2_24BIT_MODE 0

void baseHardwareInit(void);
void turnOnSpi(spi_device_e device);

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

// these need to be declared C style for the linker magic to work

void DebugMonitorVector(void);
void UsageFaultVector(void);
void BusFaultVector(void);
void HardFaultVector(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#if HAL_USE_SPI
void initSpiModule(SPIDriver *driver, brain_pin_e sck, brain_pin_e miso,
		brain_pin_e mosi,
		int sckMode,
		int mosiMode,
		int misoMode);
/**
 * @see getSpiDevice
 */
void initSpiCs(SPIConfig *spiConfig, brain_pin_e csPin);
#endif /* HAL_USE_SPI */

bool isValidCanTxPin(brain_pin_e pin);
bool isValidCanRxPin(brain_pin_e pin);
#if HAL_USE_CAN
CANDriver * detectCanDevice(brain_pin_e pinRx, brain_pin_e pinTx);
#endif /* HAL_USE_CAN */

#endif /* MPU_UTIL_H_ */
