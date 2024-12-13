/*
 * sent.h
 *
 * SENT protocol decoder header
 *
 * @date Oct 01, 2022
 * @author Andrey Gusakov <dron0gus@gmail.com>, (c) 2022
 */

#pragma once

#if EFI_SENT_SUPPORT

#include "sent_decoder.h"

/* SENT decoder init */
void initSent();

/* decoder feed hook */
void SENT_ISR_Handler(uint8_t channels, uint16_t clocks, uint8_t flags);

/* Stop/Start for config update */
void startSent();
void stopSent();

float getSentValue(SentInput input);
int getSentValues(SentInput input, uint16_t *sig0, uint16_t *sig1);

#endif /* EFI_SENT_SUPPORT */
