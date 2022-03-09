/**
 * @file    crc.h
 *
 * @date Sep 20, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

uint8_t crc8(const uint8_t * buf, uint8_t len);
uint32_t crc32(const void *buf, uint32_t size);
uint32_t crc32inc(const void *buf, uint32_t crc, uint32_t size);

#ifdef __cplusplus
}
#endif /* __cplusplus */
