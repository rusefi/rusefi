/*
 * hal_flash_device.h
 *
 * QSPI NOR flash driver with JEDEC SFDP for ChibiOS
 * Tested and developed with Microchip SST26F064A
 *
 * @date Aug 14, 2021
 * @author Andrey Gusakov, (c) 2021
 *
 * Based on ChibiOS drivers: Macronix MX25 and Micron N25Q
 * ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio
 *
 */

/**
 * @file    hal_flash_device.c
 * @brief   Jedec JESD216 SFDP code.
 *
 * @addtogroup JEDEC_SFDP
 * @{
 */

#include <string.h>

#include "hal.h"
#include "hal_serial_nor.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   Flash descriptor.
 */
flash_descriptor_t snor_descriptor = {
  .attributes       = FLASH_ATTR_ERASED_IS_ONE | FLASH_ATTR_REWRITABLE |
                      FLASH_ATTR_SUSPEND_ERASE_CAPABLE,
  .page_size        = 256U,
  .sectors_count    = 0U,           /* It is overwritten.*/
  .sectors          = NULL,
  .sectors_size     = 0U,           /* It is overwritten.*/
  .address          = 0U,
  .size             = 0U            /* It is overwritten.*/
};

/* NOT TESTED YET */
#if (SNOR_BUS_DRIVER == SNOR_BUS_DRIVER_WSPI) || defined(__DOXYGEN__)
#if (WSPI_SUPPORTS_MEMMAP == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Fast read command for memory mapped mode.
 */
const wspi_command_t snor_memmap_read = {
  .cmd              = JEDEC_CMD_READ,
  .addr             = 0,
  .dummy            = 0, //JEDEC_READ_DUMMY_CYCLES,
  .cfg              = WSPI_CFG_ADDR_SIZE_24 |
#if JEDEC_BUS_MODE == JEDEC_BUS_MODE_WSPI1L
                      WSPI_CFG_CMD_MODE_ONE_LINE |
                      WSPI_CFG_ADDR_MODE_ONE_LINE |
                      WSPI_CFG_DATA_MODE_ONE_LINE |
#elif JEDEC_BUS_MODE == JEDEC_BUS_MODE_WSPI2L
                      WSPI_CFG_CMD_MODE_TWO_LINES |
                      WSPI_CFG_ADDR_MODE_TWO_LINES |
                      WSPI_CFG_DATA_MODE_TWO_LINES |
#else
                      WSPI_CFG_CMD_MODE_FOUR_LINES |
                      WSPI_CFG_ADDR_MODE_FOUR_LINES |
                      WSPI_CFG_DATA_MODE_FOUR_LINES |
#endif
                      WSPI_CFG_ALT_MODE_NONE
};
#endif
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/* Hack to keep ChibiOS sources untouched and use on MCU with data cache:
 * read/write through temp buffer in non-cached ram. */
#define NO_CACHE __attribute__((section(".ram2")))
static NO_CACHE uint8_t tmpbuf[256] __attribute__((aligned (32)));

/* Buffer for SFDP parsing */
static uint32_t sfdpbuf[64 / 4];

/* JEDEC read command.*/
static wspi_command_t jedec_cmd_read;

/* JEDEC erase command.*/
static wspi_command_t jedec_cmd_erase;

/* JEDEC page program command.*/
static wspi_command_t jedec_cmd_program;

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static flash_error_t jedec_poll_status(SNORDriver *devp) {
  uint8_t sts;

  do {
#if JEDEC_NICE_WAITING == TRUE
    osalThreadSleepMilliseconds(1);
#endif
    /* Read status command.*/
    bus_cmd_receive(devp->config->busp, JEDEC_CMD_READ_STATUS_REGISTER,
                    1, tmpbuf);
    sts = tmpbuf[0];
  } while ((sts & JEDEC_FLAGS_STS_BUSY) != 0U);

  return FLASH_NO_ERROR;
}

#if (SNOR_BUS_DRIVER == SNOR_BUS_DRIVER_WSPI) || defined(__DOXYGEN__)
static void jedec_reset_memory(SNORDriver *devp) {

  /* 1x JEDEC_CMD_RESET_ENABLE command.*/
  static const wspi_command_t cmd_reset_enable_1 = {
    .cmd              = JEDEC_CMD_RESET_ENABLE,
    .cfg              = WSPI_CFG_CMD_MODE_ONE_LINE,
    .addr             = 0,
    .alt              = 0,
    .dummy            = 0
  };

  /* 1x JEDEC_CMD_RESET_MEMORY command.*/
  static const wspi_command_t cmd_reset_memory_1 = {
    .cmd              = JEDEC_CMD_RESET_MEMORY,
    .cfg              = WSPI_CFG_CMD_MODE_ONE_LINE,
    .addr             = 0,
    .alt              = 0,
    .dummy            = 0
  };

  /* If the device is in one bit mode then the following commands are
     rejected because shorter than 8 bits. If the device is in multiple
     bits mode then the commands are accepted and the device is reset to
     one bit mode.*/
#if JEDEC_BUS_MODE == JEDEC_BUS_MODE_WSPI4L
  /* 4x JEDEC_CMD_RESET_ENABLE command.*/
  static const wspi_command_t cmd_reset_enable_4 = {
    .cmd              = JEDEC_CMD_RESET_ENABLE,
    .cfg              = WSPI_CFG_CMD_MODE_FOUR_LINES,
    .addr             = 0,
    .alt              = 0,
    .dummy            = 0
  };

  /* 4x JEDEC_CMD_RESET_MEMORY command.*/
  static const wspi_command_t cmd_reset_memory_4 = {
    .cmd              = JEDEC_CMD_RESET_MEMORY,
    .cfg              = WSPI_CFG_CMD_MODE_FOUR_LINES,
    .addr             = 0,
    .alt              = 0,
    .dummy            = 0
  };

  wspiCommand(devp->config->busp, &cmd_reset_enable_4);
  wspiCommand(devp->config->busp, &cmd_reset_memory_4);
#elif JEDEC_BUS_MODE == JEDEC_BUS_MODE_WSPI4L
  /* 2x JEDEC_CMD_RESET_ENABLE command.*/
  static const wspi_command_t cmd_reset_enable_2 = {
    .cmd              = JEDEC_CMD_RESET_ENABLE,
    .cfg              = WSPI_CFG_CMD_MODE_TWO_LINES,
    .addr             = 0,
    .alt              = 0,
    .dummy            = 0
  };

  /* 2x JEDEC_CMD_RESET_MEMORY command.*/
  static const wspi_command_t cmd_reset_memory_2 = {
    .cmd              = JEDEC_CMD_RESET_MEMORY,
    .cfg              = WSPI_CFG_CMD_MODE_TWO_LINES,
    .addr             = 0,
    .alt              = 0,
    .dummy            = 0
  };

  wspiCommand(devp->config->busp, &cmd_reset_enable_2);
  wspiCommand(devp->config->busp, &cmd_reset_memory_2);
#endif

  /* Now the device should be in one bit mode for sure and we perform a
     device reset.*/
  wspiCommand(devp->config->busp, &cmd_reset_enable_1);
  wspiCommand(devp->config->busp, &cmd_reset_memory_1);
}
#endif /* SNOR_BUS_DRIVER == SNOR_BUS_DRIVER_WSPI */

static void jedec_set_config(SNORDriver *devp, uint8_t val)
{
  tmpbuf[0] = 0;
  tmpbuf[1] = val;

  bus_cmd_send(devp->config->busp, JEDEC_CMD_WRITE_STATUS_REGISTER, 2, tmpbuf);
}

static uint8_t jedec_get_config(SNORDriver *devp) {
  /* Read status command.*/
  bus_cmd_receive(devp->config->busp, JEDEC_CMD_READ_CONFIGURATION_REGISTER,
                  1, tmpbuf);
  return tmpbuf[0];
}

static void jedec_write_enable(SNORDriver *devp, int enable) {
    /* Enabling write operation.*/
    bus_cmd(devp->config->busp, enable ? JEDEC_CMD_WRITE_ENABLE : JEDEC_CMD_WRITE_DISABLE);
}

static void snor_device_fill_cmd(wspi_command_t *cmd,
  uint32_t cfg, uint8_t opcode,
  uint8_t mode_clocks, uint8_t dummy_clocks)
{
  cmd->cmd = opcode;
  cmd->cfg = cfg;
  cmd->dummy = 0;
  cmd->alt = 0; /* ? */

  if (mode_clocks) {
    uint8_t mode_bytes = 0;

    /* Alt bytes mode - same as address */
    if ((cmd->cfg & WSPI_CFG_ADDR_MODE_MASK) == WSPI_CFG_ADDR_MODE_ONE_LINE) {
      cmd->cfg |= WSPI_CFG_ALT_MODE_ONE_LINE;
      mode_bytes = mode_clocks / 8;
    } else if ((cmd->cfg & WSPI_CFG_ADDR_MODE_MASK) == WSPI_CFG_ADDR_MODE_TWO_LINES) {
      cmd->cfg |= WSPI_CFG_ALT_MODE_TWO_LINES;
      mode_bytes = mode_clocks / 4;
    } else if ((cmd->cfg & WSPI_CFG_ADDR_MODE_MASK) == WSPI_CFG_ADDR_MODE_FOUR_LINES) {
      cmd->cfg |= WSPI_CFG_ALT_MODE_FOUR_LINES;
      mode_bytes = mode_clocks / 2;
    } /* else if ((cmd->cfg & WSPI_CFG_ADDR_MODE_MASK) == WSPI_CFG_ADDR_MODE_EIGHT_LINES){
      cmd->cfg |= WSPI_CFG_ALT_MODE_EIGHT_LINES;
      mode_bytes = mode_clocks / 1;
    } */

    if (mode_bytes == 1)
      cmd->cfg |= WSPI_CFG_ALT_SIZE_8;
    else if (mode_bytes == 2)
      cmd->cfg |= WSPI_CFG_ALT_SIZE_16;
    else if (mode_bytes == 3)
      cmd->cfg |= WSPI_CFG_ALT_SIZE_24;
    else if (mode_bytes == 4)
      cmd->cfg |= WSPI_CFG_ALT_SIZE_32;
    else
      osalDbgAssert(0, "Failed to calculate alternative bytes size");
  } else {
    cmd->cfg |= WSPI_CFG_ALT_MODE_NONE;
  }

  cmd->dummy = dummy_clocks;
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

void snor_device_init(SNORDriver *devp) {
  int i;
  uint8_t cfg;
  uint8_t parameter_headers_n;
  /* use as temp buffer, should be at least 64 bytes */
  uint8_t *buf = (uint8_t *)sfdpbuf;
  uint32_t *sfdp = sfdpbuf;
  /* offset in sfdp area */
  flash_offset_t offset = 0;
  const uint8_t sfdp_sign[4] = {0x53, 0x46, 0x44, 0x50}; /* "SFDP" */

#if SNOR_BUS_DRIVER == SNOR_BUS_DRIVER_WSPI
  /* Attempting a reset of the XIP mode, it could be in an unexpected state
     because a CPU reset does not reset the memory too.*/
  //snor_reset_xip(devp);

  /* Attempting a reset of the device, it could be in an unexpected state
     because a CPU reset does not reset the memory too.*/
  jedec_reset_memory(devp);
#endif /* SNOR_BUS_DRIVER == SNOR_BUS_DRIVER_WSPI */

  /* SST26VF specific: adjust configuration register */
  cfg = jedec_get_config(devp);
  /* disable WP and HOLD */
  cfg |= (1 << 1);
  /* WP# disable */
  cfg &= ~(1 << 7);
  jedec_set_config(devp, cfg);

  /* Global Block Protection Unlock */
  jedec_write_enable(devp, 1);
  bus_cmd(devp->config->busp, JEDEC_CMD_GLOBAL_BLOCK_PROTECTION_UNLOCK);

  /* Reading SFDP Header. */
  snor_device_read_sfdp(devp, offset, 8, buf);

  /* Checking if the device supports SFDP. */
  osalDbgAssert(memcmp(sfdp_sign, buf, 4) == 0,
                  "chip does not support SFDP");

  /* Find JEDEC Flash Parameter Header */
  parameter_headers_n = buf[6];
  for (i = 0; i < parameter_headers_n; i++) {
    int length;
    /* each header is 8 bytes lont + 8 bytes of SFDP header */
    offset = 8 + (i * 8);

    snor_device_read_sfdp(devp, offset, 8, buf);
    if (buf[0] != 0x00) {
      /* vendor-specific header - skip. */
      continue;
    }

    /* get Parameter Table Pointer */
    offset = buf[4] | (buf[5] << 8) | (buf[6] << 16);
    /* and length */
    length = buf[3] * 4; /* in DWORDs */

    if (length != 0x40)
      continue;

    snor_device_read_sfdp(devp, offset, length, buf);

    break;
  }

  osalDbgAssert(i != parameter_headers_n,
                "JEDEC SFDP parameters block not found");

  /* Setting up the device sizes.*/
  /* Chip density defined in bits */
  if (sfdp[1] & 0x80000000)
    /* more than 4 gigabits */
    snor_descriptor.size = (size_t)(1 << ((sfdp[1] & 0x7fffffff) - 3));
  else
    snor_descriptor.size = (size_t)((sfdp[1] + 1) >> 3);
  /* Use sector size 1, assume smalest */
  snor_descriptor.sectors_size = (size_t)1 << (sfdp[7] & 0xff);
  snor_descriptor.sectors_count = snor_descriptor.size /
                                  snor_descriptor.sectors_size;

  /* Fastest read command */
  /* TODO: add 4-4-4 and 2-2-2 support */
  if (sfdp[0] & (1 << 21)) {
    /* 1-4-4 */
    snor_device_fill_cmd(&jedec_cmd_read,
      WSPI_CFG_CMD_MODE_ONE_LINE | WSPI_CFG_CMD_SIZE_8 |
      WSPI_CFG_ADDR_MODE_FOUR_LINES |
      WSPI_CFG_DATA_MODE_FOUR_LINES,
      (sfdp[2] >> 8) & 0xff, (sfdp[2] >> 5) & 0x07, (sfdp[2] >> 0) & 0x1f);
  } else if (sfdp[0] & (1 << 22)) {
    /* 1-1-4 */
    snor_device_fill_cmd(&jedec_cmd_read,
      WSPI_CFG_CMD_MODE_ONE_LINE | WSPI_CFG_CMD_SIZE_8 |
      WSPI_CFG_ADDR_MODE_ONE_LINE |
      WSPI_CFG_DATA_MODE_FOUR_LINES,
      (sfdp[2] >> 24) & 0xff, (sfdp[2] >> 21) & 0x07, (sfdp[2] >> 16) & 0x1f);
  } else if (sfdp[0] & (1 << 20)) {
    /* 1-2-2 */
    snor_device_fill_cmd(&jedec_cmd_read,
      WSPI_CFG_CMD_MODE_ONE_LINE | WSPI_CFG_CMD_SIZE_8 |
      WSPI_CFG_ADDR_MODE_TWO_LINES |
      WSPI_CFG_DATA_MODE_TWO_LINES,
      (sfdp[3] >> 24) & 0xff, (sfdp[3] >> 21) & 0x07, (sfdp[3] >> 16) & 0x1f);
  } else if (sfdp[0] & (1 << 16)) {
    /* 1-1-2 */
    snor_device_fill_cmd(&jedec_cmd_read,
      WSPI_CFG_CMD_MODE_ONE_LINE | WSPI_CFG_CMD_SIZE_8 |
      WSPI_CFG_ADDR_MODE_ONE_LINE |
      WSPI_CFG_DATA_MODE_TWO_LINES,
      (sfdp[3] >> 8) & 0xff, (sfdp[3] >> 5) & 0x07, (sfdp[3] >> 0) & 0x1f);
  }
  if (1) {
    /* Fallback to 1-1-1 */
    snor_device_fill_cmd(&jedec_cmd_read,
      WSPI_CFG_CMD_MODE_ONE_LINE | WSPI_CFG_CMD_SIZE_8 |
      WSPI_CFG_ADDR_MODE_ONE_LINE |
      WSPI_CFG_DATA_MODE_ONE_LINE,
      JEDEC_CMD_FAST_READ, 0, 8);
  }

  /* TODO: get from SFDP */
  snor_device_fill_cmd(&jedec_cmd_erase,
      WSPI_CFG_CMD_MODE_ONE_LINE | WSPI_CFG_CMD_SIZE_8 |
      WSPI_CFG_ADDR_MODE_ONE_LINE,
      JEDEC_CMD_SUBSECTOR_ERASE, 0, 0);

  snor_device_fill_cmd(&jedec_cmd_program,
      WSPI_CFG_CMD_MODE_ONE_LINE | WSPI_CFG_CMD_SIZE_8 |
      WSPI_CFG_ADDR_MODE_ONE_LINE |
      WSPI_CFG_DATA_MODE_ONE_LINE,
      JEDEC_CMD_PAGE_PROGRAM, 0, 0);

  /* TODO: how to check addressing in SFDP? */
  if (1) {
    jedec_cmd_read.cfg |= WSPI_CFG_ADDR_SIZE_24;
    jedec_cmd_erase.cfg |= WSPI_CFG_ADDR_SIZE_24;
    jedec_cmd_program.cfg |= WSPI_CFG_ADDR_SIZE_24;
  } else {
    jedec_cmd_read.cfg |= WSPI_CFG_ADDR_SIZE_32;
    jedec_cmd_erase.cfg |= WSPI_CFG_ADDR_SIZE_32;
    jedec_cmd_program.cfg |= WSPI_CFG_ADDR_SIZE_32;
  }
}

flash_error_t snor_device_read(SNORDriver *devp, flash_offset_t offset,
                               size_t n, uint8_t *rp) {
  while (n) {
    size_t chunk = n < sizeof(tmpbuf) ? n : sizeof(tmpbuf);

    jedec_cmd_read.addr = offset;
    /* read through non-cached buffer */
    if (wspiReceive(devp->config->busp, &jedec_cmd_read, chunk, tmpbuf))
      return FLASH_ERROR_READ;

    memcpy(rp, tmpbuf, chunk);
    offset += chunk;
    rp     += chunk;
    n      -= chunk;
  }

  return FLASH_NO_ERROR;
}

flash_error_t snor_device_program(SNORDriver *devp, flash_offset_t offset,
                                  size_t n, const uint8_t *pp) {
  /* Data is programmed page by page.*/
  while (n > 0U) {
    flash_error_t err;

    /* Data size that can be written in a single program page operation.*/
    size_t chunk = (size_t)(((offset | (snor_descriptor.page_size - 1)) + 1U) - offset);
    if (chunk > n)
      chunk = n;

    /* send through non-cached buffer */
    memcpy(tmpbuf, pp, chunk);

    /* Enabling write operation.*/
    jedec_write_enable(devp, 1);

    /* Page program command.*/
    jedec_cmd_program.addr = offset;
    wspiSend(devp->config->busp, &jedec_cmd_program, chunk, tmpbuf);

    /* Wait for status and check errors.*/
    err = jedec_poll_status(devp);
    if (err != FLASH_NO_ERROR) {
      return err;
    }

    /* Next page.*/
    offset += chunk;
    pp     += chunk;
    n      -= chunk;
  }

  return FLASH_NO_ERROR;
}

flash_error_t snor_device_start_erase_all(SNORDriver *devp) {

  /* Enabling write operation.*/
  jedec_write_enable(devp, 1);

  /* Bulk erase command.*/
  bus_cmd(devp->config->busp, JEDEC_CMD_BULK_ERASE);

  return FLASH_NO_ERROR;
}

flash_error_t snor_device_start_erase_sector(SNORDriver *devp,
                                             flash_sector_t sector) {
  flash_offset_t offset = (flash_offset_t)(sector * snor_descriptor.sectors_size);

  /* Enabling write operation.*/
  jedec_write_enable(devp, 1);

  /* Sector erase command.*/
  jedec_cmd_erase.addr = offset;
  wspiCommand(devp->config->busp, &jedec_cmd_erase);

  return FLASH_NO_ERROR;
}

flash_error_t snor_device_verify_erase(SNORDriver *devp,
                                       flash_sector_t sector) {
  flash_offset_t offset;
  size_t n;

  /* Read command.*/
  offset = (flash_offset_t)(sector * snor_descriptor.sectors_size);
  n = snor_descriptor.sectors_size;
  while (n > 0U) {
    size_t i;
    size_t chunk = n < sizeof(tmpbuf) ? n : sizeof(tmpbuf);

    jedec_cmd_read.addr = offset;
    /* read through non-cached buffer */
    if (wspiReceive(devp->config->busp, &jedec_cmd_read, chunk, tmpbuf)) {
      return FLASH_ERROR_READ;
    }

    /* Checking for erased state of current buffer.*/
    for (i = 0; i < chunk; i++) {
      if (tmpbuf[i] != 0xFFU) {
        /* Ready state again.*/
        devp->state = FLASH_READY;

        return FLASH_ERROR_VERIFY;
      }
    }

    offset += chunk;
    n -= chunk;
  }

  return FLASH_NO_ERROR;
}

flash_error_t snor_device_query_erase(SNORDriver *devp, uint32_t *msec) {
  uint8_t sts;

  /* Read status command.*/
  bus_cmd_receive(devp->config->busp, JEDEC_CMD_READ_STATUS_REGISTER,
                  1, tmpbuf);
  sts = tmpbuf[0];

  /* Busy?.*/
  if ((sts & JEDEC_FLAGS_STS_BUSY) != 0U) {
    /* Recommended time before polling again, this is a simplified
       implementation.*/
    if (msec != NULL) {
      *msec = 1U;
    }

    return FLASH_BUSY_ERASING;
  }

  return FLASH_NO_ERROR;
}

flash_error_t snor_device_read_sfdp(SNORDriver *devp, flash_offset_t offset,
                                    size_t n, uint8_t *rp) {
  /* JEDEC SFDP read command.*/
  wspi_command_t jedec_cmd_read_sfdp = {
    .cmd              = JEDEC_CMD_READ_DISCOVERY_PARAMETER,
    .cfg              = WSPI_CFG_CMD_MODE_ONE_LINE |
                        WSPI_CFG_CMD_SIZE_8 |
                        WSPI_CFG_ADDR_MODE_ONE_LINE |
                        WSPI_CFG_ADDR_SIZE_24 |
                        WSPI_CFG_ALT_MODE_NONE |
                        WSPI_CFG_DATA_MODE_ONE_LINE,
    .addr             = 0,
    .alt              = 0,
    .dummy            = 8 /* cycles, not bytes! */
  };

  while (n) {
    size_t chunk = n < sizeof(tmpbuf) ? n : sizeof(tmpbuf);

    jedec_cmd_read_sfdp.addr = offset;
    /* read through non-cached buffer */
    if (wspiReceive(devp->config->busp, &jedec_cmd_read_sfdp, chunk, tmpbuf))
      return FLASH_ERROR_HW_FAILURE;

    memcpy(rp, tmpbuf, chunk);

    offset += chunk;
    rp     += chunk;
    n      -= chunk;
  }

  return FLASH_NO_ERROR;
}

#if (SNOR_BUS_DRIVER == SNOR_BUS_DRIVER_WSPI) || defined(__DOXYGEN__)
void snor_activate_xip(SNORDriver *devp) {
  (void)devp;
#if 0
  static const uint8_t flash_status_xip[1] = {
    (JEDEC_READ_DUMMY_CYCLES << 4U) | 0x07U
  };

  /* Activating XIP mode in the device.*/
  jedec_write_enable(devp, 1);
  bus_cmd_send(devp->config->busp, JEDEC_CMD_WRITE_V_CONF_REGISTER,
               1, flash_status_xip);
#endif
}

void snor_reset_xip(SNORDriver *devp) {
  (void)devp;
#if 0
  static const uint8_t flash_conf[1] = {
    (JEDEC_READ_DUMMY_CYCLES << 4U) | 0x0FU
  };
  wspi_command_t cmd;
  uint8_t buf[1];

  /* Resetting XIP mode by reading one byte without XIP confirmation bit.*/
  cmd.cmd   = 0U;
  cmd.alt   = 0xFFU;
  cmd.addr  = 0U;
  cmd.dummy = JEDEC_READ_DUMMY_CYCLES - 2U;
  cmd.cfg   = WSPI_CFG_CMD_MODE_NONE |
              WSPI_CFG_ADDR_SIZE_24 |
#if JEDEC_BUS_MODE == JEDEC_BUS_MODE_WSPI1L
              WSPI_CFG_ADDR_MODE_ONE_LINE |
              WSPI_CFG_DATA_MODE_ONE_LINE |
#elif JEDEC_BUS_MODE == JEDEC_BUS_MODE_WSPI2L
              WSPI_CFG_ADDR_MODE_TWO_LINES |
              WSPI_CFG_DATA_MODE_TWO_LINES |
#elif JEDEC_BUS_MODE == JEDEC_BUS_MODE_WSPI4L
              WSPI_CFG_ADDR_MODE_FOUR_LINES |
              WSPI_CFG_DATA_MODE_FOUR_LINES |
#else
              WSPI_CFG_ADDR_MODE_EIGHT_LINES |
              WSPI_CFG_DATA_MODE_EIGHT_LINES |
#endif
              WSPI_CFG_ALT_MODE_FOUR_LINES |  /* Always 4 lines, note.*/
              WSPI_CFG_ALT_SIZE_8;
  wspiReceive(devp->config->busp, &cmd, 1, buf);

  /* Enabling write operation.*/
  jedec_write_enable(devp, 1);
  /* Rewriting volatile configuration register.*/
  bus_cmd_send(devp->config->busp, JEDEC_CMD_WRITE_V_CONF_REGISTER,
               1, flash_conf);
#endif
}
#endif /* SNOR_BUS_DRIVER == SNOR_BUS_DRIVER_WSPI */

/** @} */
