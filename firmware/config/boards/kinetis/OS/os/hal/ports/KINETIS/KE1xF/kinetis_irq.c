/*
    ChibiOS - Copyright (C) 2014-2015 Fabio Utzig

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    kinetis_irq.c
 * @brief   IRQ handler with mappings to Kinetis FSL level drivers.
 * @author  andreika <prometheus.pcb@gmail.com>
 *
 */

#include "osal.h"
#include "hal.h"

#define DECLARE_DMA_HANDLER(n) extern void DMA##n##_DriverIRQHandler(void); \
	OSAL_IRQ_HANDLER(KINETIS_DMA##n##_IRQ_VECTOR) { \
		OSAL_IRQ_PROLOGUE(); \
		DMA##n##_DriverIRQHandler(); \
		OSAL_IRQ_EPILOGUE(); \
	}

DECLARE_DMA_HANDLER(0);
DECLARE_DMA_HANDLER(1);
DECLARE_DMA_HANDLER(2);
DECLARE_DMA_HANDLER(3);
DECLARE_DMA_HANDLER(4);
DECLARE_DMA_HANDLER(5);
DECLARE_DMA_HANDLER(6);
DECLARE_DMA_HANDLER(7);
DECLARE_DMA_HANDLER(8);
DECLARE_DMA_HANDLER(9);
DECLARE_DMA_HANDLER(10);
DECLARE_DMA_HANDLER(11);
DECLARE_DMA_HANDLER(12);
DECLARE_DMA_HANDLER(13);
DECLARE_DMA_HANDLER(14);
DECLARE_DMA_HANDLER(15);

#ifdef KINETIS_HAS_DMA_ERROR_IRQ
extern void DMA_Error_DriverIRQHandler(void);
OSAL_IRQ_HANDLER(KINETIS_DMA_ERROR_IRQ_VECTOR) {
	OSAL_IRQ_PROLOGUE();
	DMA_Error_DriverIRQHandler();
	OSAL_IRQ_EPILOGUE();
}
#endif

#define DECLARE_UART_HANDLER(n,RT) extern void LPUART##n##_##RT##_DriverIRQHandler(void); \
	OSAL_IRQ_HANDLER(KINETIS_UART##n##_##RT##_IRQ_VECTOR) { \
		OSAL_IRQ_PROLOGUE(); \
		LPUART##n##_##RT##_DriverIRQHandler(); \
		OSAL_IRQ_EPILOGUE(); \
	}


#ifdef KINETIS_HAS_UART0
DECLARE_UART_HANDLER(0,RX);
DECLARE_UART_HANDLER(0,TX);
#endif

#ifdef KINETIS_HAS_UART1
DECLARE_UART_HANDLER(1,RX);
DECLARE_UART_HANDLER(1,TX);
#endif

#ifdef KINETIS_HAS_UART2
DECLARE_UART_HANDLER(2,RX);
DECLARE_UART_HANDLER(2,TX);
#endif

#define DECLARE_SPI_HANDLER(n) extern void LPSPI##n##_DriverIRQHandler(void); \
	OSAL_IRQ_HANDLER(KINETIS_SPI##n##_IRQ_VECTOR) { \
		OSAL_IRQ_PROLOGUE(); \
		LPSPI##n##_DriverIRQHandler(); \
		OSAL_IRQ_EPILOGUE(); \
	}

#ifdef KINETIS_HAS_SPI0
DECLARE_SPI_HANDLER(0);
#endif

#ifdef KINETIS_HAS_SPI1
DECLARE_SPI_HANDLER(1);
#endif

#define DECLARE_GPIO_HANDLER(n) extern void _pal_lld_irq_handler(PORT_Type *base); \
	OSAL_IRQ_HANDLER(KINETIS_PORT##n##_IRQ_VECTOR) { \
		OSAL_IRQ_PROLOGUE(); \
		_pal_lld_irq_handler(PORT##n); \
		OSAL_IRQ_EPILOGUE(); \
	}

#ifdef KINETIS_HAS_GPIOA
DECLARE_GPIO_HANDLER(A);
#endif

#ifdef KINETIS_HAS_GPIOB
DECLARE_GPIO_HANDLER(B);
#endif

#ifdef KINETIS_HAS_GPIOC
DECLARE_GPIO_HANDLER(C);
#endif

#ifdef KINETIS_HAS_GPIOD
DECLARE_GPIO_HANDLER(D);
#endif

#ifdef KINETIS_HAS_GPIOE
DECLARE_GPIO_HANDLER(E);
#endif
