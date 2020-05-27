/**
 * @file	serial_hw.h
 * @brief	SERIAL bus low level code
 *
 * @date Apr 17, 2020
 * @author Konstantin Smola, (c) 2020
 */

#pragma once

#include "efifeatures.h"
#if EFI_TUNER_STUDIO
#include "tunerstudio_outputs.h"
#endif /* EFI_TUNER_STUDIO */

void initAuxSerial(void);
#if EFI_AUX_SERIAL
void stopAuxSerialPins(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void startAuxSerialPins(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void enableAuxSerial(DECLARE_ENGINE_PARAMETER_SIGNATURE);
#endif //EFI_AUX_SERIAL