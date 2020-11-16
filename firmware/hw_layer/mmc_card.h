/*
 * @file    mmc_card.h
 *
 *
 * @date Dec 30, 2013
 * @author Kot_dnz
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "tunerstudio_io.h"

#define DOT_MLG ".mlg"

bool isLogFile(const char *fileName);
void initMmcCard(void);
bool isSdCardAlive(void);

void readLogFileContent(char *buffer, short fileId, short offset, short length);

void handleTsR(ts_channel_s *tsChannel, char *input);
void handleTsW(ts_channel_s *tsChannel, char *input);

extern spi_device_e mmcSpiDevice;
#define LOCK_SD_SPI lockSpi(mmcSpiDevice)
#define UNLOCK_SD_SPI unlockSpi(mmcSpiDevice)
