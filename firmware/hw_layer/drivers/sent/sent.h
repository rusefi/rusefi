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

/* SENT decoder init */
void initSent();

/* ISR hook */
void SENT_ISR_Handler(uint8_t ch, uint16_t val_res);

/* Stop/Start for config update */
void startSent();
void stopSent();

float getSentValue(SentInput input);
int getSentValues(SentInput input, uint16_t *sig0, uint16_t *sig1);

#endif /* EFI_SENT_SUPPORT */
