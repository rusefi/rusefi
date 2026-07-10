/*
 * @file spi.h
 *
 * @date May 27, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#if HAL_USE_SPI

#ifdef STM32H7XX

#define SPI_BaudRatePrescaler_2         (0x0UL << SPI_CFG1_MBR_Pos)
#define SPI_BaudRatePrescaler_4         (0x1UL << SPI_CFG1_MBR_Pos)
#define SPI_BaudRatePrescaler_8         (0x2UL << SPI_CFG1_MBR_Pos)
#define SPI_BaudRatePrescaler_16        (0x3UL << SPI_CFG1_MBR_Pos)
#define SPI_BaudRatePrescaler_32        (0x4UL << SPI_CFG1_MBR_Pos)
#define SPI_BaudRatePrescaler_64        (0x5UL << SPI_CFG1_MBR_Pos)
#define SPI_BaudRatePrescaler_128       (0x6UL << SPI_CFG1_MBR_Pos)
#define SPI_BaudRatePrescaler_256       (0x7UL << SPI_CFG1_MBR_Pos)

#else /* not STM32H7XX */

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

#endif

SPIDriver * getSpiDevice(spi_device_e spiDevice);
void turnOnSpi(spi_device_e device);
void lockSpi(spi_device_e device);
void unlockSpi(spi_device_e device);
brain_pin_e getMisoPin(spi_device_e device);
brain_pin_e getMosiPin(spi_device_e device);
brain_pin_e getSckPin(spi_device_e device);

void initSpiModules();
void stopSpiModules();

void printSpiConfig(const char *msg, spi_device_e device);

#endif /* HAL_USE_SPI */
