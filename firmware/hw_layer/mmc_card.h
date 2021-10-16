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

void initEarlyMmcCard();
void initMmcCard();
bool isSdCardAlive(void);

extern "C"
#endif
void onUsbConnectedNotifyMmcI(void);
