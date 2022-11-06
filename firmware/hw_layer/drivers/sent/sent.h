/*
 * sent.h
 *
 * SENT protocol decoder header
 *
 * @date Oct 01, 2022
 * @author Andrey Gusakov <dron0gus@gmail.com>, (c) 2022
 */

#pragma once

/* SENT decoder init */
void initSent();

/* ISR hook */
void SENT_ISR_Handler(uint8_t ch, uint16_t val_res);

/* Stop/Start for config update */
void startSent(void);
void stopSent(void);

float getSentValue(size_t index);
int getSentValues(size_t index, uint16_t *sig0, uint16_t *sig1);
