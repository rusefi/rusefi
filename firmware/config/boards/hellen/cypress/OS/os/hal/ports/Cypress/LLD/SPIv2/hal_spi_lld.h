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
 * @file    SPIv1/hal_spi_lld.h
 * @brief   CYPRESS SPI subsystem low level driver header.
 * @author  andreika <prometheus.pcb@gmail.com>
 *
 * @addtogroup SPI
 * @{
 */

#ifndef HAL_SPI_LLD_H_
#define HAL_SPI_LLD_H_

#if HAL_USE_SPI || defined(__DOXYGEN__)

#include "pdl_header.h"
//#include "mfs/mfs.h"
//#include "mfs/mfs_hl.h"

#define SPI_TX_BUFFSIZE          (64)
#define SPI_RX_BUFFSIZE          (64)
#define SPI_RX_BUFF_FILL_LVL     (1)



/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Circular mode support flag.
 */
#define SPI_SUPPORTS_CIRCULAR           TRUE

#define SPI_CIRCULAR_FLAG				0x010
#define SPI_SOFTWARE_CS_FLAG			0x100

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   SPI0 driver enable switch.
 * @details If set to @p TRUE the support for SPI0 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(CYPRESS_SPI_USE_SPI0) || defined(__DOXYGEN__)
#define CYPRESS_SPI_USE_SPI0                  FALSE
#endif

/**
 * @brief   SPI1 driver enable switch.
 * @details If set to @p TRUE the support for SPI0 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(CYPRESS_SPI_USE_SPI1) || defined(__DOXYGEN__)
#define CYPRESS_SPI_USE_SPI1                  FALSE
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if !(CYPRESS_SPI_USE_SPI0 || CYPRESS_SPI_USE_SPI1)
#error "SPI driver activated but no SPI peripheral assigned"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   SPI notification callback type.
 *
 * @param[in] spip      pointer to the @p SPIDriver object triggering the
 *                      callback
 */
typedef void (*spicallback_t)(SPIDriver *spip);

/**
 * @brief   Low level fields of the SPI configuration structure.
 */
#define spi_lld_config_fields                                               \
  /* The chip select port. */                                               \
  ioportid_t                ssport;                                         \
  /* The chip select pad number. */                                         \
  uint_fast8_t              sspad;                                          \
  /* SPI CR1 register initialization data.*/                                \
  uint16_t                  cr1;                                            \
  /* SPI CR2 register initialization data.*/                                \
  /* Actually noy used, keep for compatibility with stm32 */                \
  uint16_t                  cr2

/**
 * @brief   Low level fields of the SPI driver structure.
 */
#define spi_lld_driver_fields                                               \
  /* Pointer to the SPIx registers block. */                                \
	volatile stc_mfsn_csio_t *spi;                                            \
	/* PDL internal structures. */                                            \
	stc_mfs_csio_config_t     stcMfsCsioCfg;                                  \
	stc_csio_cs_t             stcCsioCsConfig;                                \
	stc_csio_irq_cb_t         stcCsioIrqCb;                                   \
	en_cs_pin_sel_t           stcCsioCsPin;                                   \
	/* Master or Slave. */                                                    \
	bool isMaster;                                                            \
	/* Transfer flags (including kLPSPI_MasterPcs*) for this SPI config. */   \
	int32_t flags;                                                            \
	/* Transfer buffers. */                                                   \
	uint8_t *csioTxBuf;                                                       \
	uint8_t *csioRxBuf;                                                       \
	uint32_t csioTxBufSize, csioRxBufSize;                                    \
	uint32_t csioReceiveCnt, csioSendCnt;                                     \
	volatile uint16_t u16RxBufFillCnt

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if CYPRESS_SPI_USE_SPI0 && !defined(__DOXYGEN__)
extern SPIDriver SPID1;
#endif

#if CYPRESS_SPI_USE_SPI1 && !defined(__DOXYGEN__)
extern SPIDriver SPID2;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void spi_lld_init(void);
  void spi_lld_start(SPIDriver *spip);
  void spi_lld_stop(SPIDriver *spip);
  void spi_lld_select(SPIDriver *spip);
  void spi_lld_unselect(SPIDriver *spip);
  void spi_lld_ignore(SPIDriver *spip, size_t n);
  void spi_lld_exchange(SPIDriver *spip, size_t n,
                        const void *txbuf, void *rxbuf);
  void spi_lld_send(SPIDriver *spip, size_t n, const void *txbuf);
  void spi_lld_receive(SPIDriver *spip, size_t n, void *rxbuf);
#if (SPI_SUPPORTS_CIRCULAR == TRUE) || defined(__DOXYGEN__)
  void spi_lld_abort(SPIDriver *spip);
#endif
  uint16_t spi_lld_polled_exchange(SPIDriver *spip, uint16_t frame);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_SPI */

#endif /* HAL_SPI_LLD_H_ */

/** @} */
