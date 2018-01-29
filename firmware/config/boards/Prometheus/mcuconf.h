/**
 * @file boards/Prometheus/mcuconf.h
 *
 * @brief In this header we can override mcuconf.h.
 *
 * @date Apr 22, 2017
 * @author andreika, (c) 2017
 */

#include "../../stm32f4ems/mcuconf.h"

#ifndef _MCUCONF_PROMETHEUS_H_
#define _MCUCONF_PROMETHEUS_H_

//#define STM32_ADC_ADCPRE                    ADC_CCR_ADCPRE_DIV2 // ADC_CCR_ADCPRE_DIV4

/*
#undef STM32_PLLN_VALUE
#define STM32_PLLN_VALUE                    360		// 180 MHz

#undef STM32_PLLI2SR_VALUE
#define STM32_PLLI2SR_VALUE                 4

#undef STM32_PLLSAIP_VALUE
#define STM32_PLLSAIP_VALUE                 4
*/


#undef STM32_LSE_ENABLED
#define STM32_LSE_ENABLED FALSE

#undef STM32_RTCSEL
#define STM32_RTCSEL                        STM32_RTCSEL_HSEDIV // STM32_RTCSEL_LSI

#undef STM32_SERIAL_USE_USART1
#define STM32_SERIAL_USE_USART1             FALSE

#undef STM32_SERIAL_USE_USART2
#define STM32_SERIAL_USE_USART2             FALSE

#undef STM32_SERIAL_USE_USART3
#define STM32_SERIAL_USE_USART3             FALSE

#undef STM32_SERIAL_USE_UART4
#if TS_UART_DMA_MODE
#define STM32_SERIAL_USE_UART4              FALSE
#else
#define STM32_SERIAL_USE_UART4              TRUE
#endif

#undef STM32_UART_USE_USART3
#define STM32_UART_USE_USART3               FALSE

#undef STM32_UART_USE_UART4
#define STM32_UART_USE_UART4                TRUE

//#undef STM32_USB_USE_OTG1
//#define STM32_USB_USE_OTG1                  FALSE

#undef STM32_I2C_USE_I2C1
#define STM32_I2C_USE_I2C1                  FALSE

#undef STM32_SPI_USE_SPI2
#define STM32_SPI_USE_SPI2                  FALSE


//#undef STM32_CAN_CAN1_IRQ_PRIORITY
//#define STM32_CAN_CAN1_IRQ_PRIORITY 4

#endif /* _MCUCONF_PROMETHEUS_H_ */
