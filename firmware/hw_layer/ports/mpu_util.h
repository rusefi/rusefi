#pragma once

#include "rusefi_enums.h"

#include "port_mpu_util.h"

// Base MCU
void baseMCUInit(void);
void jump_to_bootloader();

// CAN bus
#if HAL_USE_CAN
bool isValidCanTxPin(brain_pin_e pin);
bool isValidCanRxPin(brain_pin_e pin);
CANDriver * detectCanDevice(brain_pin_e pinRx, brain_pin_e pinTx);
#endif // HAL_USE_CAN

// SPI
#if HAL_USE_SPI
void initSpiModule(SPIDriver *driver, brain_pin_e sck, brain_pin_e miso,
		brain_pin_e mosi,
		int sckMode,
		int mosiMode,
		int misoMode);

void initSpiCs(SPIConfig *spiConfig, brain_pin_e csPin);
void turnOnSpi(spi_device_e device);
#endif // HAL_USE_SPI

// Brownout Reset
typedef enum {
	BOR_Result_Ok = 0x00,
	BOR_Result_Error
} BOR_Result_t;

BOR_Level_t BOR_Get(void);
BOR_Result_t BOR_Set(BOR_Level_t BORValue);

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

// these need to be declared with C linkage - they're called from C and asm files
void DebugMonitorVector(void);
void UsageFaultVector(void);
void BusFaultVector(void);
void HardFaultVector(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
