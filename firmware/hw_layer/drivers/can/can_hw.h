/**
 * @file	can_hw.h
 *
 * @date Dec 11, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "efifeatures.h"

void initCan();
void setCanType(int type);
void setCanVss(int type);

#if EFI_CAN_SUPPORT

void stopCanPins();
void startCanPins();
bool getIsCanEnabled(void);
int setCanBaud(size_t index, can_baudrate_e rate);
int setCanBaud(size_t index, int baud);
// Runtime listen-only (silent mode, no ACK / no TX) override; switch back and forth at will.
// Overrides the can*ListenMode configuration until reboot.
int setCanListenMode(size_t index, bool listenOnlyEnabled);

#endif /* EFI_CAN_SUPPORT */
