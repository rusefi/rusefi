/**
 * @file	can_hw.h
 *
 * @date Dec 11, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "efifeatures.h"
#if EFI_TUNER_STUDIO
#include "tunerstudio_configuration.h"
#endif /* EFI_TUNER_STUDIO */

void initCan(void);
void commonTxInit(int eid);
void sendCanMessage(int size = 8);
void setCanType(int type);
void setTxBit(int offset, int index);

#if EFI_CAN_SUPPORT
void stopCanPins(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void startCanPins(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void enableFrankensoCan(DECLARE_ENGINE_PARAMETER_SIGNATURE);
#if EFI_TUNER_STUDIO
void postCanState(TunerStudioOutputChannels *tsOutputChannels);
#endif /* EFI_TUNER_STUDIO */
#endif /* EFI_CAN_SUPPORT */
