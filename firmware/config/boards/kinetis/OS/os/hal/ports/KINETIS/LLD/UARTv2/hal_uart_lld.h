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
 * @file    hal_uart_lld.h
 * @brief   KINETIS UART subsystem low level driver header.
 * @author  andreika <prometheus.pcb@gmail.com>
 *
 * @addtogroup UART
 * @{
 */

#ifndef HAL_UART_LLD_H
#define HAL_UART_LLD_H

#if (HAL_USE_UART == TRUE) || defined(__DOXYGEN__)

#include "fsl_common.h"
#include "fsl_lpuart_edma.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Kinetis configuration options
 * @{
 */
/**
 * @brief   UART driver enable switch.
 * @details If set to @p TRUE the support for UART1 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(KINETIS_UART_USE_UART1) || defined(__DOXYGEN__)
#define KINETIS_UART_USE_UART1             FALSE
#endif

#if !defined(KINETIS_UART_USE_UART2) || defined(__DOXYGEN__)
#define KINETIS_UART_USE_UART2             FALSE
#endif

#if !defined(KINETIS_UART_USE_UART3) || defined(__DOXYGEN__)
#define KINETIS_UART_USE_UART3             FALSE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#define UART_USE_BLOCKING_SEND TRUE
//#define UART_USE_RING_BUFFER TRUE
//#define KINETIS_UART_RX_RING_BUFFER_SIZE 64

// STM32-compatible flags
#define USART_CR1_PS 			(1U << 9)   /*!< 0x00000200 */
#define USART_CR1_PCE 			(1U << 10)  /*!< 0x00000400 */
#define USART_CR2_STOP1_BITS    (0 << 12)   /**< @brief CR2 1 stop bit value.*/
#define USART_CR2_STOP2_BITS    (2 << 12)   /**< @brief CR2 2 stop bit value.*/


/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   UART driver condition flags type.
 */
typedef uint32_t uartflags_t;

/**
 * @brief   Structure representing an UART driver.
 */
typedef struct UARTDriver UARTDriver;

/**
 * @brief   Generic UART notification callback type.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 */
typedef void (*uartcb_t)(UARTDriver *uartp);

/**
 * @brief   Character received UART notification callback type.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 * @param[in] c         received character
 */
typedef void (*uartccb_t)(UARTDriver *uartp, uint16_t c);

/**
 * @brief   Receive error UART notification callback type.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 * @param[in] e         receive error mask
 */
typedef void (*uartecb_t)(UARTDriver *uartp, uartflags_t e);

/**
 * @brief   Receive Half-transfer UART notification callback type.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 * @param[in] full      flag set to 1 for the second half, and 0 for the first half
 */
typedef void (*uarthcb_t)(UARTDriver *uartp, uartflags_t full);

/**
 * @brief   Driver configuration structure.
 * @note    Implementations may extend this structure to contain more,
 *          architecture dependent, fields.
 */
typedef struct {
  /**
   * @brief End of transmission buffer callback.
   */
  uartcb_t                  txend1_cb;
  /**
   * @brief Physical end of transmission callback.
   */
  uartcb_t                  txend2_cb;
  /**
   * @brief Receive buffer filled callback.
   */
  uartcb_t                  rxend_cb;
  /**
   * @brief Character received while out if the @p UART_RECEIVE state.
   */
  uartccb_t                 rxchar_cb;
  /**
   * @brief Receive error callback.
   */
  uartecb_t                 rxerr_cb;
  /**
   * @brief   Receiver timeout (idle) callback.
   */
  uartcb_t                  timeout_cb;
  /* End of the mandatory fields.*/
  /**
   * @brief Bit rate.
   */
  uint32_t                  speed;
  /**
   * @brief Initialization value for the CR1 register.
   */
  uint16_t                  cr1;
  /**
   * @brief Initialization value for the CR2 register.
   */
  uint16_t                  cr2;
  /**
   * @brief Initialization value for the CR3 register.
   */
  uint16_t                  cr3;
  /**
   * @brief Half-transfer receive buffer callback.
   */
  uarthcb_t                 rxhalf_cb;
} UARTConfig;

/**
 * @brief   Structure representing an UART driver.
 * @note    Implementations may extend this structure to contain more,
 *          architecture dependent, fields.
 */
struct UARTDriver {
  /**
   * @brief Driver state.
   */
  uartstate_t               state;
  /**
   * @brief Transmitter state.
   */
  uarttxstate_t             txstate;
  /**
   * @brief Receiver state.
   */
  uartrxstate_t             rxstate;
  /**
   * @brief Current configuration data.
   */
  const UARTConfig          *config;
#if (UART_USE_WAIT == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   Synchronization flag for transmit operations.
   */
  bool                      early;
  /**
   * @brief   Waiting thread on RX.
   */
  thread_reference_t        threadrx;
  /**
   * @brief   Waiting thread on TX.
   */
  thread_reference_t        threadtx;
#endif /* UART_USE_WAIT */
#if (UART_USE_MUTUAL_EXCLUSION == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   Mutex protecting the peripheral.
   */
  mutex_t                   mutex;
#endif /* UART_USE_MUTUAL_EXCLUSION */
#if defined(UART_DRIVER_EXT_FIELDS)
  UART_DRIVER_EXT_FIELDS
#endif
  /* End of the mandatory fields.*/
  /**
   * @brief Pointer to the USART registers block.
   */
  LPUART_Type             *lpuart;

  /**
   * @brief LPUART common eDMA channel handle.
   */
  lpuart_edma_handle_t dmaHandle;
  /**
   * @brief eDMA transfer handles.
   */
  edma_handle_t lpuartRxEdmaHandle, lpuartTxEdmaHandle;
  
  /**
   * @brief Used to trigger the IRQ from our software handler.
   * See uart_lld_callback() and UART_USE_RING_BUFFER
   */
  int pendingRxIrq;
  
  /**
   * @brief Used for the RingBuffer mode or to handle UART errors in EDMA mode.
   */
  lpuart_handle_t rxHandle;

#ifdef UART_USE_RING_BUFFER
  uint8_t rxRingBuffer[KINETIS_UART_RX_RING_BUFFER_SIZE];
#endif /* UART_USE_RING_BUFFER */
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if (KINETIS_UART_USE_UART1 == TRUE) && !defined(__DOXYGEN__)
extern UARTDriver UARTD1;
#endif
#if (KINETIS_UART_USE_UART2 == TRUE) && !defined(__DOXYGEN__)
extern UARTDriver UARTD2;
#endif
#if (KINETIS_UART_USE_UART3 == TRUE) && !defined(__DOXYGEN__)
extern UARTDriver UARTD3;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void uart_lld_init(void);
  void uart_lld_start(UARTDriver *uartp);
  void uart_lld_stop(UARTDriver *uartp);
  void uart_lld_start_send(UARTDriver *uartp, size_t n, const void *txbuf);
  size_t uart_lld_stop_send(UARTDriver *uartp);
  void uart_lld_start_receive(UARTDriver *uartp, size_t n, void *rxbuf);
  size_t uart_lld_stop_receive(UARTDriver *uartp);
  void uart_lld_blocking_send(UARTDriver *uartp, size_t n, const void *txbuf);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_UART == TRUE */

#endif /* HAL_UART_LLD_H */

/** @} */
