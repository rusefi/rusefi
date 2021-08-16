/*
 * hal_flash_device.h
 *
 * QSPI NOR flash driver with JEDEC SFDP for ChibiOS
 * Tested and developed with Microchip SST26F064A
 *
 * @date Aug 14, 2021
 * @author Andrey Gusakov, (c) 2021
 */

/**
 * @file    hal_flash_device.h
 * @brief   Jedec JESD216 SFDP
 *
 * @addtogroup JEDEC_SFDP
 * @{
 */

#ifndef HAL_FLASH_DEVICE_H
#define HAL_FLASH_DEVICE_H

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    Device capabilities
 * @{
 */
#define SNOR_DEVICE_SUPPORTS_XIP            TRUE
/** @} */

/**
 * @name    Device identification
 * @{
 */

/** @} */

/**
 * @name    Command codes
 * @{
 */

#define JEDEC_CMD_READ_DISCOVERY_PARAMETER          0x5A

/* default/fallback commands */
#define JEDEC_CMD_WRITE_STATUS_REGISTER             0x01
#define JEDEC_CMD_PAGE_PROGRAM                      0x02
#define JEDEC_CMD_READ                              0x03
#define JEDEC_CMD_WRITE_DISABLE                     0x04
#define JEDEC_CMD_READ_STATUS_REGISTER              0x05
#define JEDEC_CMD_WRITE_ENABLE                      0x06
#define JEDEC_CMD_SUBSECTOR_ERASE                   0x20
#define JEDEC_CMD_READ_CONFIGURATION_REGISTER       0x35
#define JEDEC_CMD_BULK_ERASE                        0xC7
#define JEDEC_CMD_RESET_ENABLE                      0x66
#define JEDEC_CMD_GLOBAL_BLOCK_PROTECTION_UNLOCK    0x98
#define JEDEC_CMD_RESET_MEMORY                      0x99
/** @} */

/**
 * @name    Flags status register bits
 * @{
 */
#define JEDEC_FLAGS_STS_BUSY                        0x80U
/** @} */

/**
 * @name    Bus interface modes.
 * @{
 */
#define JEDEC_BUS_MODE_WSPI1L                       1U
#define JEDEC_BUS_MODE_WSPI2L                       2U
#define JEDEC_BUS_MODE_WSPI4L                       4U
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   Switch WSPI bus width on initialization.
 * @details A bus width initialization is performed by writing the
 *          Enhanced Volatile Configuration Register. If the flash
 *          device is configured using the Non Volatile Configuration
 *          Register then this option is not required.
 * @note    This option is only valid in WSPI bus mode.
 */
#if !defined(JEDEC_SWITCH_WIDTH) || defined(__DOXYGEN__)
#define JEDEC_SWITCH_WIDTH                  TRUE
#endif

/**
 * @brief   Device bus mode to be used.
 * #note    if @p JEDEC_SWITCH_WIDTH is @p FALSE then this is the bus mode
 *          that the device is expected to be using.
 * #note    if @p JEDEC_SWITCH_WIDTH is @p TRUE then this is the bus mode
 *          that the device will be switched in.
 * @note    This option is only valid in WSPI bus mode.
 */
#if !defined(JEDEC_BUS_MODE) || defined(__DOXYGEN__)
#define JEDEC_BUS_MODE                      JEDEC_BUS_MODE_WSPI4L
#endif

/**
 * @brief   Delays insertions.
 * @details If enabled this options inserts delays into the flash waiting
 *          routines releasing some extra CPU time for threads with lower
 *          priority, this may slow down the driver a bit however.
 */
#if !defined(JEDEC_NICE_WAITING) || defined(__DOXYGEN__)
#define JEDEC_NICE_WAITING                  TRUE
#endif

/**
 * @brief   Uses 4kB sub-sectors rather than 64kB sectors.
 */
#if !defined(JEDEC_USE_SUB_SECTORS) || defined(__DOXYGEN__)
#define JEDEC_USE_SUB_SECTORS               FALSE
#endif

/**
 * @brief   Number of dummy cycles for fast read (1..15).
 * @details This is the number of dummy cycles to be used for fast read
 *          operations.
 */
#if !defined(JEDEC_READ_DUMMY_CYCLES) || defined(__DOXYGEN__)
#define JEDEC_READ_DUMMY_CYCLES             8
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if (JEDEC_READ_DUMMY_CYCLES < 1) || (JEDEC_READ_DUMMY_CYCLES > 15)
#error "invalid JEDEC_READ_DUMMY_CYCLES value (1..15)"
#endif

/**
 * @brief   WSPI settings for command only.
 */
#define SNOR_WSPI_CFG_CMD               (WSPI_CFG_CMD_MODE_ONE_LINE       | \
                                         WSPI_CFG_ADDR_MODE_NONE          | \
                                         WSPI_CFG_ALT_MODE_NONE           | \
                                         WSPI_CFG_DATA_MODE_NONE          | \
                                         WSPI_CFG_CMD_SIZE_8              | \
                                         WSPI_CFG_ADDR_SIZE_24)
/**
 * @brief   WSPI settings for command and address.
 */
#define SNOR_WSPI_CFG_CMD_ADDR          (WSPI_CFG_CMD_MODE_ONE_LINE       | \
                                         WSPI_CFG_ADDR_MODE_ONE_LINE      | \
                                         WSPI_CFG_ALT_MODE_NONE           | \
                                         WSPI_CFG_DATA_MODE_NONE          | \
                                         WSPI_CFG_CMD_SIZE_8              | \
                                         WSPI_CFG_ADDR_SIZE_24)
/**
 * @brief   WSPI settings for command and data.
 */
#define SNOR_WSPI_CFG_CMD_DATA          (WSPI_CFG_CMD_MODE_ONE_LINE       | \
                                         WSPI_CFG_ADDR_MODE_NONE          | \
                                         WSPI_CFG_ALT_MODE_NONE           | \
                                         WSPI_CFG_DATA_MODE_ONE_LINE      | \
                                         WSPI_CFG_CMD_SIZE_8              | \
                                         WSPI_CFG_ADDR_SIZE_24)
/**
 * @brief   WSPI settings for command, address and data.
 */
#define SNOR_WSPI_CFG_CMD_ADDR_DATA     (WSPI_CFG_CMD_MODE_ONE_LINE       | \
                                         WSPI_CFG_ADDR_MODE_ONE_LINE      | \
                                         WSPI_CFG_ALT_MODE_NONE           | \
                                         WSPI_CFG_DATA_MODE_ONE_LINE      | \
                                         WSPI_CFG_CMD_SIZE_8              | \
                                         WSPI_CFG_ADDR_SIZE_24)

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if !defined(__DOXYGEN__)
extern flash_descriptor_t snor_descriptor;
#endif

#if (SNOR_BUS_DRIVER == SNOR_BUS_DRIVER_WSPI) && (WSPI_SUPPORTS_MEMMAP == TRUE)
extern const wspi_command_t snor_memmap_read;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void snor_device_init(SNORDriver *devp);
  flash_error_t snor_device_read(SNORDriver *devp, flash_offset_t offset,
                                 size_t n, uint8_t *rp);
  flash_error_t snor_device_program(SNORDriver *devp, flash_offset_t offset,
                                    size_t n, const uint8_t *pp);
  flash_error_t snor_device_start_erase_all(SNORDriver *devp);
  flash_error_t snor_device_start_erase_sector(SNORDriver *devp,
                                               flash_sector_t sector);
  flash_error_t snor_device_verify_erase(SNORDriver *devp,
                                         flash_sector_t sector);
  flash_error_t snor_device_query_erase(SNORDriver *devp, uint32_t *msec);
  flash_error_t snor_device_read_sfdp(SNORDriver *devp, flash_offset_t offset,
                                      size_t n, uint8_t *rp);
#if (SNOR_BUS_DRIVER == SNOR_BUS_DRIVER_WSPI) &&                            \
    (SNOR_DEVICE_SUPPORTS_XIP == TRUE)
  void snor_activate_xip(SNORDriver *devp);
  void snor_reset_xip(SNORDriver *devp);
#endif
#ifdef __cplusplus
}
#endif

#endif /* HAL_FLASH_DEVICE_H */

/** @} */

