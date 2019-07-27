/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

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
 * @file    DMAv1/kinetis_dma.h
 * @brief   DMA helper driver header.
 * @author  andreika <prometheus.pcb@gmail.com>
 * @note    This driver uses the new naming convention used for the KINETIS
 *          so the "DMA channels" are referred as "DMA streams".
 *
 * @addtogroup KINETIS_DMA
 * @{
 */

#ifndef KINETIS_DMA_H
#define KINETIS_DMA_H

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

#define KINETIS_DMA_STREAM_UART0_RX 1
#define KINETIS_DMA_STREAM_UART0_TX 2

#define KINETIS_DMA_STREAM_UART1_RX 3
#define KINETIS_DMA_STREAM_UART1_TX 4

#define KINETIS_DMA_STREAM_SPI_RX 5
#define KINETIS_DMA_STREAM_SPI_TX 6


#ifdef __cplusplus
extern "C" {
#endif
  void dmaInit(void);
#ifdef __cplusplus
}
#endif

#endif /* KINETIS_DMA_H */

/** @} */
