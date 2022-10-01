/*
 * sent.cpp
 *
 *  Created on: 16 May 2022
 *      Author: alexv
 */

#include "ch.h"
#include "hal.h"

#include "sent.h"

struct sent_channel {
    SM_SENT_enum state;
    uint8_t nibbles[SENT_MSG_PAYLOAD_SIZE];
    /* Tick interval in CPU clocks - adjusted on SYNC */
    uint32_t tickClocks;

    /* slow channel stuff */
    struct {
        uint16_t data;
        uint8_t id;
    } scMsg[16];
    uint16_t scMsgFlags;
    uint32_t scShift2;   /* shift register for bit 2 from status nibble */
    uint32_t scShift3;   /* shift register for bit 3 from status nibble */
    bool sc16Bit;       /* C-flag */

#if SENT_STATISTIC_COUNTERS
    /* stats */
    uint32_t PulseCnt;
    uint32_t ShortIntervalErr;
    uint32_t LongIntervalErr;
    uint32_t SyncErr;
    uint32_t CrcErrCnt;
    uint32_t FrameCnt;
#endif // SENT_STATISTIC_COUNTERS
};

static struct sent_channel channels[SENT_CHANNELS_NUM];

/* Si7215 decoded data */
int32_t si7215_magnetic[SENT_CHANNELS_NUM];
int32_t si7215_counter[SENT_CHANNELS_NUM];

/* GM DI fuel pressure, temperature sensor decoded data */
int32_t gm_sig0[SENT_CHANNELS_NUM];
int32_t gm_sig1[SENT_CHANNELS_NUM];
int32_t gm_stat[SENT_CHANNELS_NUM];

#if SENT_DEV == SENT_GM_ETB

uint16_t sentOpenThrottleVal = 0;
uint16_t sentClosedThrottleVal = 0;

uint16_t sentOpenTempVal = 0;
uint16_t sentClosedTempVal = 0;

uint8_t sentRawData = 1;

uint8_t sent_crc4(uint8_t* pdata, uint16_t ndata);
uint8_t sent_crc4_gm(uint8_t* pdata, uint16_t ndata);

static int SENT_SlowChannelDecoder(struct sent_channel *ch);

//#define SENT_TICK (5 * 72) // 5uS @72MHz
#define SENT_TICK (27 * 72 / 10) // 2.7uS @72MHz

int SENT_Decoder(struct sent_channel *ch, uint16_t clocks)
{
    int ret = 0;

    #if SENT_STATISTIC_COUNTERS
        ch->PulseCnt++;
    #endif

    /* special case for out-of-sync state */
    if (ch->state == SM_SENT_INIT_STATE) {
        /* check is pulse looks like sync with allowed +/-20% deviation */
        int syncClocks = (SENT_SYNC_INTERVAL + SENT_OFFSET_INTERVAL) * SENT_TICK;

        if ((clocks >= (syncClocks * 80 / 100)) &&
            (clocks <= (syncClocks * 120 / 100))) {
            /* calculate tick time */
            ch->tickClocks = (clocks + 56 / 2) / (SENT_SYNC_INTERVAL + SENT_OFFSET_INTERVAL);
            /* next state */
            ch->state = SM_SENT_STATUS_STATE;
            /* done for this pulse */
            return 0;
        }
    }

    int interval = (clocks + ch->tickClocks / 2) / ch->tickClocks - SENT_OFFSET_INTERVAL;

    if (interval < 0) {
        #if SENT_STATISTIC_COUNTERS
            ch->ShortIntervalErr++;
        #endif //SENT_STATISTIC_COUNTERS
        ch->state = SM_SENT_INIT_STATE;
        return -1;
    }

    switch(ch->state)
    {
        case SM_SENT_INIT_STATE:
            /* handles above, should not get in here */
            break;

        case SM_SENT_SYNC_STATE:
            if (interval == SENT_SYNC_INTERVAL)
            {// sync interval - 56 ticks
                /* measured tick interval will be used until next sync pulse */
                ch->tickClocks = (clocks + 56 / 2) / (SENT_SYNC_INTERVAL + SENT_OFFSET_INTERVAL);
                ch->state = SM_SENT_STATUS_STATE;
            }
            else
            {
                #if SENT_STATISTIC_COUNTERS
                    // Increment sync interval err count
                    ch->SyncErr++;
                    if (interval > SENT_SYNC_INTERVAL)
                    {
                        ch->LongIntervalErr++;
                    }
                    else
                    {
                        ch->ShortIntervalErr++;
                    }
                #endif // SENT_STATISTIC_COUNTERS
                /* wait for next sync and recalibrate tickClocks */
                ch->state = SM_SENT_INIT_STATE;
            }
            break;

        case SM_SENT_STATUS_STATE:
        case SM_SENT_SIG1_DATA1_STATE:
        case SM_SENT_SIG1_DATA2_STATE:
        case SM_SENT_SIG1_DATA3_STATE:
        case SM_SENT_SIG2_DATA1_STATE:
        case SM_SENT_SIG2_DATA2_STATE:
        case SM_SENT_SIG2_DATA3_STATE:
        case SM_SENT_CRC_STATE:
            if(interval <= SENT_MAX_INTERVAL)
            {
                ch->nibbles[ch->state - SM_SENT_STATUS_STATE] = interval;

                if (ch->state != SM_SENT_CRC_STATE)
                {
                    /* TODO: refactor */
                    ch->state = (SM_SENT_enum)((int)ch->state + 1);
                }
                else
                {
                    #if SENT_STATISTIC_COUNTERS
                        ch->FrameCnt++;
                    #endif // SENT_STATISTIC_COUNTERS
                    /* CRC check */
                    if ((ch->nibbles[7] == sent_crc4(ch->nibbles, 7)) ||
                        (ch->nibbles[7] == sent_crc4_gm(ch->nibbles + 1, 6)))
                    {
                        // Full packet has been received
                        ret = 1;
                    }
                    else
                    {
                        #if SENT_STATISTIC_COUNTERS
                            ch->CrcErrCnt++;
                        #endif // SENT_STATISTIC_COUNTERS
                        ret = -1;
                    }
                    ch->state = SM_SENT_SYNC_STATE;
                }
            }
            else
            {
                #if SENT_STATISTIC_COUNTERS
                    ch->LongIntervalErr++;
                #endif

                ch->state = SM_SENT_INIT_STATE;
            }
            break;
    }

    if (ret > 0) {
        /* valid packet received, can process slow channels */
        SENT_SlowChannelDecoder(ch);
    } else if (ret < 0) {
        /* packet is incorrect, reset slow channel state machine */
        ch->scShift2 = 0;
        ch->scShift3 = 0;
    }

    return ret;
}

static int SENT_SlowChannelDecoder(struct sent_channel *ch)
{
    /* bit 2 and bit 3 from status nibble are used to transfer short messages */
    bool b2 = !!(ch->nibbles[0] & (1 << 2));
    bool b3 = !!(ch->nibbles[0] & (1 << 3));

    /* shift in new data */
    ch->scShift2 = (ch->scShift2 << 1) | b2;
    ch->scShift3 = (ch->scShift3 << 1) | b3;

    if (1) {
        /* Short Serial Message format */

        /* 0b1000.0000.0000.0000? */
        if ((ch->scShift3 & 0xffff) == 0x8000) {
            /* Done receiving */
            uint8_t id = (ch->scShift2 >> 12) & 0x0f;

            /* TODO: add CRC check */
            ch->scMsg[id].data = (ch->scShift2 >> 4) & 0xff;
            ch->scMsg[id].id = id;
            ch->scMsgFlags |= (1 << id);
        }
    }
    if (1) {
        /* Enhanced Serial Message format */

        /* 0b11.1111.0xxx.xx0x.xxx0 ? */
        if ((ch->scShift3 & 0x3f821) == 0x3f000) {
            uint8_t id;

            /* C: configuration bit is used to indicate 16 bit format */
            ch->sc16Bit = !!(ch->scShift3 & (1 << 10));
            if (!ch->sc16Bit) {
                int i;
                /* 12 bit message, 8 bit ID */
                id = ((ch->scShift3 >> 1) & 0x0f) |
                     ((ch->scShift3 >> 2) & 0xf0);
                uint16_t data = ch->scShift2 & 0x0fff; /* 12 bit */

                /* TODO: add crc check */
                /* Find free mainbox or mailbox with same ID */
                /* TODO: allow message box freeing */
                for (i = 0; i < 16; i++) {
                    if (((ch->scMsgFlags & (1 << i)) == 0) ||
                        (ch->scMsg[i].id == id)) {
                        ch->scMsg[i].data = data;
                        ch->scMsg[i].id = id;
                        ch->scMsgFlags |= (1 << i);
                        return 0;
                    }
                }
            } else {
                /* 16 bit message, 4 bit ID */
                uint16_t data;
                data = (ch->scShift2 & 0x0fff) |
                       (((ch->scShift3 >> 1) & 0x0f) << 12);
                id = (ch->scShift3 >> 6) & 0x0f;

                /* TODO: add crc check */
                ch->scMsg[id].data = data; /* 16 bit */
                ch->scMsg[id].id = id; /* straight mapping */
                ch->scMsgFlags |= (1 << id);
            }
        }
    }

    return 0;
}

/* This CRC works for Si7215 for WHOLE message expect last nibble (CRC) */
uint8_t sent_crc4(uint8_t* pdata, uint16_t ndata)
{
    size_t i;
    uint8_t crc = SENT_CRC_SEED; // initialize checksum with seed "0101"
    const uint8_t CrcLookup[16] = {0, 13, 7, 10, 14, 3, 9, 4, 1, 12, 6, 11, 15, 2, 8, 5};

    for (i = 0; i < ndata; i++)
    {
        crc = crc ^ pdata[i];
        crc = CrcLookup[crc];
    }

    return crc;
}

/* This CRC works for GM pressure sensor for message minus status nibble and minus CRC nibble */
/* TODO: double check and use same CRC routine? */
uint8_t sent_crc4_gm(uint8_t* pdata, uint16_t ndata)
{
    const uint8_t CrcLookup[16] = {0, 13, 7, 10, 14, 3, 9, 4, 1, 12, 6, 11, 15, 2, 8, 5};
    uint8_t calculatedCRC, i;

    calculatedCRC = SENT_CRC_SEED; // initialize checksum with seed "0101"

    for (i = 0; i < ndata; i++)
    {
        calculatedCRC = CrcLookup[calculatedCRC];
        calculatedCRC = (calculatedCRC ^ pdata[i]) & 0x0F;
    }
    // One more round with 0 as input
    calculatedCRC = CrcLookup[calculatedCRC];

    return calculatedCRC;
}


uint8_t SENT_IsRawData(void)
{
    return sentRawData;
}

void SENT_SetRawDataProp(void)
{
  sentRawData = 1;
}

void SENT_ResetRawDataProp(void)
{
  sentRawData = 0;
}

uint16_t SENT_GetOpenThrottleVal(void)
{
    return sentOpenThrottleVal;
}

uint16_t SENT_GetClosedThrottleVal(void)
{
    return sentClosedThrottleVal;
}

/* Stat counters */
uint32_t SENT_GetShortIntervalErrCnt(void)
{
    #if SENT_STATISTIC_COUNTERS
        return channels[0].ShortIntervalErr;
    #else
        return 0;
    #endif
}

uint32_t SENT_GetLongIntervalErrCnt(void)
{
    #if SENT_STATISTIC_COUNTERS
        return channels[0].LongIntervalErr;
    #else
        return 0;
    #endif
}

uint32_t SENT_GetCrcErrCnt(void)
{
    #if SENT_STATISTIC_COUNTERS
        return channels[0].CrcErrCnt;
    #else
        return 0;
    #endif
}

uint32_t SENT_GetSyncErrCnt(void)
{
    #if SENT_STATISTIC_COUNTERS
        return channels[0].SyncErr;
    #else
        return 0;
    #endif
}

uint32_t SENT_GetSyncCnt(void)
{
    #if SENT_STATISTIC_COUNTERS
        return channels[0].PulseCnt;
    #else
        return 0;
    #endif
}

uint32_t SENT_GetFrameCnt(uint32_t n)
{
    #if SENT_STATISTIC_COUNTERS
        return channels[n].FrameCnt;
    #else
        (void)n;
        return 0;
    #endif
}

uint32_t SENT_GetErrPercent(void)
{
    #if SENT_STATISTIC_COUNTERS
        return 100 * channels[0].SyncErr / channels[0].PulseCnt;
    #else
        return 0;
    #endif
}

uint32_t SENT_GetTickTimeNs(void)
{
    return channels[0].tickClocks * 1000 / 72;
}

/* Debug */
void SENT_GetRawNibbles(uint8_t* buf)
{
    for(uint8_t i = 0; i < SENT_MSG_PAYLOAD_SIZE; i++)
    {
        buf[i] = channels[0].nibbles[i];
    }
}

uint8_t SENT_GetThrottleValPrec(void)
{
    return (100 - ((sentOpenThrottleVal - SENT_THROTTLE_OPEN_VAL) * 100)/(SENT_THROTTLE_CLOSE_VAL - SENT_THROTTLE_OPEN_VAL));
}

#endif

/* Slow Channel */
uint16_t SENT_GetSlowMessagesFlags(uint32_t n)
{
    return channels[n].scMsgFlags;
}

uint16_t SENT_GetSlowMessage(uint32_t n, uint32_t i)
{
    return channels[n].scMsg[i].data;
}

uint16_t SENT_GetSlowMessageID(uint32_t n, uint32_t i)
{
    return channels[n].scMsg[i].id;
}

/* Si7215 decoded data */
int32_t Si7215_GetMagneticField(uint32_t n)
{
    return si7215_magnetic[n];
}

int32_t Si7215_GetCounter(uint32_t n)
{
    return si7215_counter[n];
}

/* GM DI fuel pressure, temperature sensor data */
int32_t gm_GetSig0(uint32_t n)
{
    return gm_sig0[n];
}

int32_t gm_GetSig1(uint32_t n)
{
    return gm_sig1[n];
}

int32_t gm_GetStat(uint32_t n)
{
    return gm_stat[n];
}

int32_t gm_GetPressure(uint32_t n)
{
    /* two pressure signals:
     * Sig0 occupie 3 first nibbles in MSB..LSB order
     * Sig1 occupit next 3 nibbles in LSB..MSB order
     * Signals are close, but not identical.
     * Sig0 shows about 197..198 at 1 Atm (open air) and 282 at 1000 KPa (9.86 Atm)
     * Sig1 shows abour 202..203 at 1 Atm (open air) and 283 at 1000 KPa (9.86 Atm)
     * So for 8.86 Atm delta there are:
     * 84..85 units for sig0
     * 80..81 units for sig1
     * Measurements are not ideal, so lets ASSUME 10 units per 1 Atm
     * Offset is 187..188 for Sig0 and 192..193 for Sig1.
     * Average offset is 180.
     */

    /* in 0.001 Atm */
    return ((gm_GetSig0(n) - 198 + 10 + gm_GetSig1(n) - 202 + 10) * 100 / 2);
}

/* 4 per channel should be enougth */
#define SENT_MB_SIZE        (4 * SENT_CH_MAX)

static msg_t sent_mb_buffer[SENT_MB_SIZE];
static MAILBOX_DECL(sent_mb, sent_mb_buffer, SENT_MB_SIZE);

static THD_WORKING_AREA(waSentDecoderThread, 256);

void SENT_ISR_Handler(uint8_t ch, uint16_t clocks)
{
    /* encode to fin msg_t */
    msg_t msg = (ch << 16) | clocks;
    chMBPostI(&sent_mb, msg);
}

static void SentDecoderThread(void*)
{
    msg_t msg;
    while(true)
    {
        msg_t ret;

        ret = chMBFetchTimeout(&sent_mb, &msg, TIME_INFINITE);
        /* TODO: handle ret */
        if (ret == MSG_OK) {
            uint16_t tick = msg & 0xffff;
            uint8_t n = (msg >> 16) & 0xff;
            struct sent_channel *ch = &channels[n];

            if (SENT_Decoder(ch, tick) > 0) {
                /* decode Si7215 packet */
                if (((~ch->nibbles[1 + 5]) & 0x0f) == ch->nibbles[1 + 0]) {
                    si7215_magnetic[n] =
                        ((ch->nibbles[1 + 0] << 8) |
                         (ch->nibbles[1 + 1] << 4) |
                         (ch->nibbles[1 + 2] << 0)) - 2048;
                    si7215_counter[n] =
                        (ch->nibbles[1 + 3] << 4) |
                        (ch->nibbles[1 + 4] << 0);
                }
                /* decode GM DI fuel pressure, temperature sensor */
                if (1) {
                    /* Sig0 occupie first 3 nibbles in MSB..LSB order
                     * Sig1 occupit next 3 nibbles in LSB..MSB order */
                    gm_sig0[n] =
                        (ch->nibbles[1 + 0] << 8) |
                        (ch->nibbles[1 + 1] << 4) |
                        (ch->nibbles[1 + 2] << 0);
                    gm_sig1[n] =
                        (ch->nibbles[1 + 3] << 0) |
                        (ch->nibbles[1 + 4] << 4) |
                        (ch->nibbles[1 + 5] << 8);
                    gm_stat[n] =
                        ch->nibbles[0];
                }
            }
        }
    }
}

void SentDecoder_Init(void)
{
    /* init interval mailbox */
    chMBObjectInit(&sent_mb, sent_mb_buffer, SENT_MB_SIZE);

    chThdCreateStatic(waSentDecoderThread, sizeof(waSentDecoderThread), NORMALPRIO, SentDecoderThread, nullptr);
}
