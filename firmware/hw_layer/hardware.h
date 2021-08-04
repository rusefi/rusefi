/*
 * @file hardware.h
 *
 * @date May 27, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

void startHardware(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void stopHardware(DECLARE_ENGINE_PARAMETER_SIGNATURE);

#if HAL_USE_SPI

// Peripherial Clock 42MHz SPI2 SPI3
// Peripherial Clock 84MHz SPI1                                SPI1        SPI2/3
//  42 MHz      21 MHZ
#define SPI_BaudRatePrescaler_2         ((uint16_t)0x0000)
//  21 MHz      10.5 MHz
#define SPI_BaudRatePrescaler_4         ((uint16_t)0x0008)
//  10.5 MHz    5.25 MHz
#define SPI_BaudRatePrescaler_8         ((uint16_t)0x0010)
//  5.25 MHz    2.626 MHz
#define SPI_BaudRatePrescaler_16        ((uint16_t)0x0018)
//  2.626 MHz   1.3125 MHz
#define SPI_BaudRatePrescaler_32        ((uint16_t)0x0020)
//  1.3125 MHz  656.25 KHz
#define SPI_BaudRatePrescaler_64        ((uint16_t)0x0028)
//  656.25 KHz  328.125 KHz
#define SPI_BaudRatePrescaler_128       ((uint16_t)0x0030)
//  328.125 KHz 164.06 KHz
#define SPI_BaudRatePrescaler_256       ((uint16_t)0x0038)

int getSpiPrescaler(spi_speed_e speed, spi_device_e device);

EXTERNC SPIDriver * getSpiDevice(spi_device_e spiDevice);
void turnOnSpi(spi_device_e device);
void lockSpi(spi_device_e device);
void unlockSpi(spi_device_e device);
brain_pin_e getMisoPin(spi_device_e device);
brain_pin_e getMosiPin(spi_device_e device);
brain_pin_e getSckPin(spi_device_e device);

#endif /* HAL_USE_SPI */

#ifdef __cplusplus

void applyNewHardwareSettings(DECLARE_ENGINE_PARAMETER_SIGNATURE);

// Initialize hardware that doesn't require configuration to be loaded
void initHardwareNoConfig(DECLARE_ENGINE_PARAMETER_SIGNATURE);

// Initialize hardware with configuration loaded
void initHardware(DECLARE_ENGINE_PARAMETER_SIGNATURE);

#if EFI_PROD_CODE
#include "debounce.h"

#endif /* EFI_PROD_CODE */

void showBor(void);
void setBor(int borValue);

class ButtonDebounce;

#endif /* __cplusplus */
