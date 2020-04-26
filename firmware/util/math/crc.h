/**
 * @file    crc.h
 *
 * @date Sep 20, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "stdint.h"

typedef unsigned char crc_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

crc_t calc_crc(const crc_t message[], int nBytes);
uint32_t crc32(const void *buf, uint32_t size);
uint32_t crc32inc(const void *buf, uint32_t crc, uint32_t size);

#ifdef __cplusplus
}
#endif /* __cplusplus */
