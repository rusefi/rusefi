/*
 * @file    mmc_card.h
 *
 *
 * @date Dec 30, 2013
 * @author Kot_dnz
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#ifdef __cplusplus
#include "tunerstudio_io.h"

#define DOT_MLG ".mlg"

bool isLogFile(const char *fileName);
void initEarlyMmcCard();
void initMmcCard();
bool isSdCardAlive(void);

void readLogFileContent(char *buffer, short fileId, short offset, short length);

void handleTsR(TsChannelBase* tsChannel, char *input);
void handleTsW(TsChannelBase* tsChannel, char *input);

extern "C"
#endif
void onUsbConnectedNotifyMmcI(void);
