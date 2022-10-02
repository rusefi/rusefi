/*
 * sent.h
 *
 * SENT protocol decoder header
 *
 * @date Oct 01, 2022
 * @author Andrey Gusakov <dron0gus@gmail.com>, (c) 2022
 */

#pragma once

#ifndef SENT_CHANNELS_NUM
#define SENT_CHANNELS_NUM		4 // Number of sent channels
#endif

#define SENT_OFFSET_INTERVAL	12
#define SENT_SYNC_INTERVAL		(56 - SENT_OFFSET_INTERVAL) // 56 ticks - 12

#define SENT_MIN_INTERVAL		12
#define SENT_MAX_INTERVAL		15

#define SENT_CRC_SEED 0x05

#define SENT_MSG_DATA_SIZE      6
/* Status + two 12-bit signals + CRC */
#define SENT_MSG_PAYLOAD_SIZE   (1 + SENT_MSG_DATA_SIZE + 1)  // Size of payload

/* collect statistic */
#define SENT_STATISTIC_COUNTERS	1

/* Move to high-level decoder */
#define SENT_THROTTLE_OPEN_VAL	435     // Sensor position of fully open throttle
#define SENT_THROTTLE_CLOSE_VAL	3665    // Sensor position of fully closed throttle

/* SENT decoder init */
void initSent();

/* ISR hook */
void SENT_ISR_Handler(uint8_t ch, uint16_t val_res);

/* Stop/Start for config update */
void startSent(void);
void stopSent(void);

uint16_t SENT_GetData(uint8_t ch);

/* Stat counters */
uint32_t SENT_GetShortIntervalErrCnt(void);
uint32_t SENT_GetLongIntervalErrCnt(void);
uint32_t SENT_GetCrcErrCnt(void);
uint32_t SENT_GetSyncErrCnt(void);
uint32_t SENT_GetSyncCnt(void);
uint32_t SENT_GetFrameCnt(uint32_t n);
uint32_t SENT_GetErrPercent(void);
uint32_t SENT_GetTickTimeNs(void);

/* Debug */
void SENT_GetRawNibbles(uint8_t * buf);

uint16_t SENT_GetOpenThrottleVal(void);
uint16_t SENT_GetClosedThrottleVal(void);
void SENT_SetRawDataProp(void);
void SENT_ResetRawDataProp(void);
uint8_t SENT_GetThrottleValPrec(void);

/* Fast channel */
uint32_t SENT_GetSig0(uint32_t n);
uint32_t SENT_GetSig1(uint32_t n);
uint8_t SENT_GetStat(uint32_t n);

/* Slow Channel */
uint16_t SENT_GetSlowMessagesFlags(uint32_t n);
uint16_t SENT_GetSlowMessage(uint32_t n, uint32_t i);
uint16_t SENT_GetSlowMessageID(uint32_t n, uint32_t i);
