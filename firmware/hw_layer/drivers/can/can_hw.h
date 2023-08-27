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

const CANConfig* findCanConfig(can_baudrate_e rate);

void stopCanPins();
void startCanPins();
bool getIsCanEnabled(void);

#if EFI_TUNER_STUDIO
void postCanState();
#endif /* EFI_TUNER_STUDIO */

#endif /* EFI_CAN_SUPPORT */
