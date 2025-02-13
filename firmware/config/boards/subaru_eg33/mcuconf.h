/**
 * @file boards/subarue-eg33/mcuconf.h
 *
 * @brief In this header we can override mcuconf.h.
 *
 * @date Feb 06, 2021
 * @author Andrey Gusakov, 2021
 */
#ifndef _MCUCONF_SUBARUEG33_H_
#define _MCUCONF_SUBARUEG33_H_

#include "../../../hw_layer/ports/stm32/stm32f7/cfg/mcuconf.h"

#undef STM32_LSE_ENABLED
#define STM32_LSE_ENABLED				FALSE

#undef STM32_RTCSEL
#define STM32_RTCSEL					STM32_RTCSEL_HSEDIV

/* serials and uarts */ 
#undef STM32_SERIAL_USE_USART1
#undef STM32_UART_USE_USART1
#define STM32_SERIAL_USE_USART1			TRUE
#define STM32_UART_USE_USART1			FALSE

#undef STM32_SERIAL_USE_USART2
#undef STM32_UART_USE_USART2
#define STM32_SERIAL_USE_USART2			FALSE
#define STM32_UART_USE_USART2			FALSE

#undef STM32_SERIAL_USE_USART3
#undef STM32_UART_USE_USART3
#define STM32_SERIAL_USE_USART3			FALSE
#define STM32_UART_USE_USART3			FALSE

#undef STM32_UART_USE_USART3
#define STM32_UART_USE_USART3			FALSE

#undef STM32_UART_USE_UART4
#define STM32_UART_USE_UART4			FALSE

#undef STM32_USB_USE_OTG1
#define STM32_USB_USE_OTG1				TRUE

#undef STM32_USB_USE_USB1
#define STM32_USB_USE_USB1				TRUE

#undef STM32_I2C_USE_I2C1
#define STM32_I2C_USE_I2C1				FALSE

#undef STM32_SPI_USE_SPI2
#define STM32_SPI_USE_SPI2				TRUE

#undef STM32_SPI_USE_SPI4
#define STM32_SPI_USE_SPI4				TRUE

#undef STM32_SPI_USE_SPI5
#define STM32_SPI_USE_SPI5				TRUE

#undef STM32_ADC_USE_ADC3
#define STM32_ADC_USE_ADC3				TRUE

/* default STM32_DMA_STREAM_ID(2, 4) used by ADC1 */
#undef STM32_SPI_SPI5_TX_DMA_STREAM
#define STM32_SPI_SPI5_TX_DMA_STREAM	STM32_DMA_STREAM_ID(2, 6)

/* STM32_DMA_STREAM_ID(2, 0) is used by SPI4_RX */
#undef STM32_SDC_SDMMC2_DMA_STREAM
#define STM32_SDC_SDMMC2_DMA_STREAM     STM32_DMA_STREAM_ID(2, 5)

/* To remove futher possible conflict */
#undef STM32_SPI_SPI6_RX_DMA_STREAM

#undef STM32_CAN_USE_CAN2
#define STM32_CAN_USE_CAN2				FALSE

//#undef STM32_CAN_CAN1_IRQ_PRIORITY
//#define STM32_CAN_CAN1_IRQ_PRIORITY 4

/*
 * WSPI driver system settings.
 */
#define STM32_WSPI_USE_QUADSPI1         TRUE
#define STM32_WSPI_QUADSPI1_DMA_STREAM  STM32_DMA_STREAM_ID(2, 7)
#define JEDEC_BUS_MODE                  JEDEC_BUS_MODE_WSPI4L
/* QSPI is clocked from AHB clock, which is 216 MHz max
 * Maximum CLK rate for SST26VF is 104/80MHz for Fast Read and
 * Page program
 * 216 / 3 = 72 MHz */
#define STM32_WSPI_QUADSPI1_PRESCALER_VALUE 3

#endif /* _MCUCONF_SUBARUEG33_H_ */
