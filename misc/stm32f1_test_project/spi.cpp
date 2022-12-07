
#include "can.h"
#include "hal.h"
#include "spi.h"
#include <cstdint>

#define SPI_CS_PORT GPIOA
#define SPI_CS_PIN 4

/* Low speed SPI configuration (281.250kHz, CPHA=0, CPOL=0, MSb first).*/
static SPIConfig ls_spicfg = {false, NULL, SPI_CS_PORT, SPI_CS_PIN,
                              SPI_CR1_BR_2 | SPI_CR1_BR_1,
                              0};

/*
 * SPI TX and RX buffers.
 */
static uint8_t txbuf[512];
static uint8_t rxbuf[512];

void InitSpi() {
  /*
   * SPI1 I/O pins setup.
   */
  palSetPadMode(GPIOA, 5, PAL_MODE_STM32_ALTERNATE_PUSHPULL);     /* SCK. */
  palSetPadMode(GPIOA, 6, PAL_MODE_STM32_ALTERNATE_PUSHPULL);     /* MISO.*/
  palSetPadMode(GPIOA, 7, PAL_MODE_STM32_ALTERNATE_PUSHPULL);     /* MOSI.*/
  palSetPadMode(SPI_CS_PORT, SPI_CS_PIN, PAL_MODE_OUTPUT_PUSHPULL);
  palSetPad(SPI_CS_PORT, SPI_CS_PIN);
}