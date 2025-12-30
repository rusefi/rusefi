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

// see also SD_STATUS
typedef enum {
	SD_MODE_IDLE = 0,
	SD_MODE_ECU,
	SD_MODE_PC,
	SD_MODE_UNMOUNT,
	SD_MODE_FORMAT,
} SD_MODE;

void initEarlyMmcCard();
void initMmcCard();

void onUsbConnectedNotifyMmcI();

void updateSdCardLiveFlags();

struct USBDriver;
bool msd_request_hook_new(USBDriver *usbp);

void sdCardRequestMode(SD_MODE mode);
SD_MODE sdCardGetCurrentMode();
void sdCardRemoveReportFiles();
