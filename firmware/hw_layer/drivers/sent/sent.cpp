/*
 * sent.cpp
 *
 * SENT protocol decoder
 *
 * @date Oct 01, 2022
 * @author Andrey Gusakov <dron0gus@gmail.com>, (c) 2022
 */

#if EFI_PROD_CODE

#include "pch.h"

#include "sent.h"

/* Maximum slow shannel mailboxes, DO NOT CHANGE */
#define SENT_SLOW_CHANNELS_MAX  16

typedef enum
{
	SM_SENT_CALIB_STATE = 0,
	SM_SENT_INIT_STATE,
	SM_SENT_SYNC_STATE,
	SM_SENT_STATUS_STATE,
	SM_SENT_SIG1_DATA1_STATE,
	SM_SENT_SIG1_DATA2_STATE,
	SM_SENT_SIG1_DATA3_STATE,
	SM_SENT_SIG2_DATA1_STATE,
	SM_SENT_SIG2_DATA2_STATE,
	SM_SENT_SIG2_DATA3_STATE,
	SM_SENT_CRC_STATE,
}SM_SENT_enum;

struct sent_channel_stat {
	uint32_t ShortIntervalErr;
	uint32_t LongIntervalErr;
	uint32_t SyncErr;
	uint32_t CrcErrCnt;
	uint32_t FrameCnt;
	uint32_t RestartCnt;
};

class sent_channel {
private:
	SM_SENT_enum state = SM_SENT_CALIB_STATE;

	/* Unit interval in timer clocks - adjusted on SYNC */
	uint32_t tickPerUnit;
	uint32_t pulseCounter;
	/* pulses skipped in init state while waiting for SYNC */
	uint32_t initStatePulseCounter;

	/* fast channel nibbles.
	 * TODO: rewrite with uint32_t shift register */
	uint8_t nibbles[SENT_MSG_PAYLOAD_SIZE];

	/* slow channel shift registers and flags */
	uint16_t scMsgFlags;
	uint32_t scShift2;	/* shift register for bit 2 from status nibble */
	uint32_t scShift3;	/* shift register for bit 3 from status nibble */
	bool sc16Bit;		/* C-flag */
	/* Decoder */
	int SlowChannelDecoder(void);

	/* CRC */
	uint8_t crc4(uint8_t* pdata, uint16_t ndata);
	uint8_t crc4_gm(uint8_t* pdata, uint16_t ndata);

	void restart(void);

public:
	/* fast channel data */
	int32_t sig0;
	int32_t sig1;
	int8_t stat;

	/* slow channel data */
	struct {
		uint16_t data;
		uint8_t id;
	} scMsg[16];

	/* Statistic counters */
#if SENT_STATISTIC_COUNTERS
	sent_channel_stat statistic;
#endif // SENT_STATISTIC_COUNTERS

	int Decoder(uint16_t clocks);

	/* Show status */
	void Info(void);
};

static sent_channel channels[SENT_CHANNELS_NUM];

void sent_channel::restart(void)
{
	state = SM_SENT_CALIB_STATE;
	pulseCounter = 0;
	initStatePulseCounter = 0;
	tickPerUnit = 0;

	/* reset slow channels */
	scMsgFlags = 0;
	scShift2 = 0;
	scShift3 = 0;

	#if SENT_STATISTIC_COUNTERS
		statistic.ShortIntervalErr = 0;
		statistic.LongIntervalErr = 0;
		statistic.SyncErr = 0;
		statistic.CrcErrCnt = 0;
		statistic.FrameCnt = 0;
		statistic.RestartCnt++;
	#endif
}

int sent_channel::Decoder(uint16_t clocks)
{
	int ret = 0;

	pulseCounter++;

	/* special case - tick time calculation */
	if (state == SM_SENT_CALIB_STATE) {
		/* Find longes pulse ... */
		if (clocks > tickPerUnit) {
			tickPerUnit = clocks;
		}
		/* ... of 9 pulses */
		if (pulseCounter >= (SENT_MSG_PAYLOAD_SIZE + 1)) {
			tickPerUnit = (tickPerUnit + (SENT_SYNC_INTERVAL + SENT_OFFSET_INTERVAL) / 2) /
							(SENT_SYNC_INTERVAL + SENT_OFFSET_INTERVAL);
			pulseCounter = 0;
			state = SM_SENT_INIT_STATE;
		}
		return 0;
	}

	/* special case for out-of-sync state */
	if (state == SM_SENT_INIT_STATE) {
		/* check if pulse looks like sync with allowed +/-20% deviation */
		int syncClocks = (SENT_SYNC_INTERVAL + SENT_OFFSET_INTERVAL) * tickPerUnit;

		if (((100 * clocks) >= (syncClocks * 80)) &&
			((100 * clocks) <= (syncClocks * 120))) {
			initStatePulseCounter = 0;
			/* adjust unit time */
			tickPerUnit = (clocks + (SENT_SYNC_INTERVAL + SENT_OFFSET_INTERVAL) / 2) /
							(SENT_SYNC_INTERVAL + SENT_OFFSET_INTERVAL);
			/* next state */
			state = SM_SENT_STATUS_STATE;
		} else {
			initStatePulseCounter++;
			/* 3 frames skipped, no SYNC detected - recalibrate */
			if (initStatePulseCounter >= (9 * 3)) {
				restart();
			}
		}
		/* done for this pulse */
		return 0;
	}

	int interval = (clocks + tickPerUnit / 2) / tickPerUnit - SENT_OFFSET_INTERVAL;

	if (interval < 0) {
		#if SENT_STATISTIC_COUNTERS
			statistic.ShortIntervalErr++;
		#endif //SENT_STATISTIC_COUNTERS
		state = SM_SENT_INIT_STATE;
		return -1;
	}

	switch(state)
	{
		case SM_SENT_CALIB_STATE:
		case SM_SENT_INIT_STATE:
			/* handled above, should not get in here */
			ret = -1;
			break;

		case SM_SENT_SYNC_STATE:
			if (interval == SENT_SYNC_INTERVAL)
			{// sync interval - 56 ticks
				/* measured tick interval will be used until next sync pulse */
				tickPerUnit = (clocks + 56 / 2) / (SENT_SYNC_INTERVAL + SENT_OFFSET_INTERVAL);
				state = SM_SENT_STATUS_STATE;
			}
			else
			{
				#if SENT_STATISTIC_COUNTERS
					// Increment sync interval err count
					statistic.SyncErr++;
					if (interval > SENT_SYNC_INTERVAL)
					{
						statistic.LongIntervalErr++;
					}
					else
					{
						statistic.ShortIntervalErr++;
					}
				#endif // SENT_STATISTIC_COUNTERS
				/* wait for next sync and recalibrate tickPerUnit */
				state = SM_SENT_INIT_STATE;
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
				nibbles[state - SM_SENT_STATUS_STATE] = interval;

				if (state != SM_SENT_CRC_STATE)
				{
					/* TODO: refactor */
					state = (SM_SENT_enum)((int)state + 1);
				}
				else
				{
					#if SENT_STATISTIC_COUNTERS
						statistic.FrameCnt++;
					#endif // SENT_STATISTIC_COUNTERS
					/* CRC check */
					/* TODO: find correct way to calculate CRC */
					if ((nibbles[7] == crc4(nibbles, 7)) ||
						(nibbles[7] == crc4_gm(nibbles + 1, 6)))
					{
						// Full packet has been received
						sig0 =
							(nibbles[1 + 0] << 8) |
							(nibbles[1 + 1] << 4) |
							(nibbles[1 + 2] << 0);
						sig1 =
							(nibbles[1 + 3] << 0) |
							(nibbles[1 + 4] << 4) |
							(nibbles[1 + 5] << 8);
						stat =
							nibbles[0];
						ret = 1;
					}
					else
					{
						#if SENT_STATISTIC_COUNTERS
							statistic.CrcErrCnt++;
						#endif // SENT_STATISTIC_COUNTERS
						ret = -1;
					}
					state = SM_SENT_SYNC_STATE;
				}
			}
			else
			{
				#if SENT_STATISTIC_COUNTERS
					statistic.LongIntervalErr++;
				#endif

				state = SM_SENT_INIT_STATE;
				ret = -1;
			}
			break;
	}

	if (ret > 0) {
		/* valid packet received, can process slow channels */
		SlowChannelDecoder();
	} else if (ret < 0) {
		/* packet is incorrect, reset slow channel state machine */
		scShift2 = 0;
		scShift3 = 0;
		/* TODO: add error counter and restart from CALIB state? */
	}

	return ret;
}

int sent_channel::SlowChannelDecoder()
{
	/* bit 2 and bit 3 from status nibble are used to transfer short messages */
	bool b2 = !!(nibbles[0] & (1 << 2));
	bool b3 = !!(nibbles[0] & (1 << 3));

	/* shift in new data */
	scShift2 = (scShift2 << 1) | b2;
	scShift3 = (scShift3 << 1) | b3;

	if (1) {
		/* Short Serial Message format */

		/* 0b1000.0000.0000.0000? */
		if ((scShift3 & 0xffff) == 0x8000) {
			/* Done receiving */
			uint8_t id = (scShift2 >> 12) & 0x0f;

			/* TODO: add CRC check */
			scMsg[id].data = (scShift2 >> 4) & 0xff;
			scMsg[id].id = id;
			scMsgFlags |= (1 << id);
		}
	}
	if (1) {
		/* Enhanced Serial Message format */

		/* 0b11.1111.0xxx.xx0x.xxx0 ? */
		if ((scShift3 & 0x3f821) == 0x3f000) {
			uint8_t id;

			/* C: configuration bit is used to indicate 16 bit format */
			sc16Bit = !!(scShift3 & (1 << 10));
			if (!sc16Bit) {
				int i;
				/* 12 bit message, 8 bit ID */
				id = ((scShift3 >> 1) & 0x0f) |
					 ((scShift3 >> 2) & 0xf0);
				uint16_t data = scShift2 & 0x0fff; /* 12 bit */

				/* TODO: add crc check */
				/* Find free mainbox or mailbox with same ID */
				/* TODO: allow message box freeing */
				for (i = 0; i < SENT_SLOW_CHANNELS_MAX; i++) {
					if (((scMsgFlags & (1 << i)) == 0) ||
						(scMsg[i].id == id)) {
						scMsg[i].data = data;
						scMsg[i].id = id;
						scMsgFlags |= (1 << i);
						return 0;
					}
				}
			} else {
				/* 16 bit message, 4 bit ID */
				uint16_t data;
				data = (scShift2 & 0x0fff) |
					   (((scShift3 >> 1) & 0x0f) << 12);
				id = (scShift3 >> 6) & 0x0f;

				/* TODO: add crc check */
				scMsg[id].data = data; /* 16 bit */
				scMsg[id].id = id; /* straight mapping */
				scMsgFlags |= (1 << id);
			}
		}
	}

	return 0;
}

/* This CRC works for Si7215 for WHOLE message expect last nibble (CRC) */
uint8_t sent_channel::crc4(uint8_t* pdata, uint16_t ndata)
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
uint8_t sent_channel::crc4_gm(uint8_t* pdata, uint16_t ndata)
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

void sent_channel::Info(void)
{
	int i;

	efiPrintf("Unit time %d timer ticks", tickPerUnit);
	efiPrintf("Total pulses %d", pulseCounter);
	efiPrintf("Last fast msg Status 0x%01x Sig0 0x%03x Sig1 0x%03x", stat, sig0, sig1);

	efiPrintf("Slow channels:");
	for (i = 0; i < SENT_SLOW_CHANNELS_MAX; i++) {
		if (scMsgFlags & BIT(i)) {
			efiPrintf(" ID %d: %d", scMsg[i].id, scMsg[i].data);
		}
	}

	#if SENT_STATISTIC_COUNTERS
		efiPrintf("Restarts %d", statistic.RestartCnt);
		efiPrintf("Interval errors %d short, %d long", statistic.ShortIntervalErr, statistic.LongIntervalErr);
		efiPrintf("Total frames %d with crc erorr %d", statistic.FrameCnt, statistic.CrcErrCnt);
		efiPrintf("Sync errors %d", statistic.SyncErr);
	#endif
}

#if 0
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
		return channels[0].pulseCounter;
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
		return 100 * channels[0].SyncErr / channels[0].pulseCounter;
	#else
		return 0;
	#endif
}

uint32_t SENT_GetTickTimeNs(void)
{
	return channels[0].tickPerUnit * 1000 / 72;
}

/* Debug */
void SENT_GetRawNibbles(uint8_t* buf)
{
	for(uint8_t i = 0; i < SENT_MSG_PAYLOAD_SIZE; i++)
	{
		buf[i] = channels[0].nibbles[i];
	}
}

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

/* GM DI fuel pressure, temperature sensor data */
uint32_t SENT_GetSig0(uint32_t n)
{
	return channels[n].sig0;
}

uint32_t SENT_GetSig1(uint32_t n)
{
	return channels[n].sig1;
}

uint8_t SENT_GetStat(uint32_t n)
{
	return channels[n].stat;
}
#endif

/* 4 per channel should be enougth */
#define SENT_MB_SIZE		(4 * SENT_CHANNELS_NUM)

static msg_t sent_mb_buffer[SENT_MB_SIZE];
static MAILBOX_DECL(sent_mb, sent_mb_buffer, SENT_MB_SIZE);

static THD_WORKING_AREA(waSentDecoderThread, 256);

void SENT_ISR_Handler(uint8_t ch, uint16_t clocks)
{
	/* encode to fit msg_t */
	msg_t msg = (ch << 16) | clocks;

	/* called from ISR */
	chSysLockFromISR();
	chMBPostI(&sent_mb, msg);
	chSysUnlockFromISR();
}

static void SentDecoderThread(void*)
{
	msg_t msg;
	while(true)
	{
		msg_t ret;

		ret = chMBFetchTimeout(&sent_mb, &msg, TIME_INFINITE);

		if (ret == MSG_OK) {
			uint16_t tick = msg & 0xffff;
			uint8_t n = (msg >> 16) & 0xff;

			if (n < SENT_CHANNELS_NUM) {
				sent_channel &ch = channels[n];

				if (ch.Decoder(tick) > 0) {
					/* Call high level decoder from here */
				}
			}
		}
	}
}

static void printSentInfo()
{
	int i;

	for (i = 0; i < SENT_CHANNELS_NUM; i++) {
		sent_channel &ch = channels[i];

		efiPrintf("---- SENT ch %d ----", i);
		ch.Info();
		efiPrintf("--------------------");
	}
}

/* Should be called once */
void initSent(void)
{
	/* init interval mailbox */
	chMBObjectInit(&sent_mb, sent_mb_buffer, SENT_MB_SIZE);

	chThdCreateStatic(waSentDecoderThread, sizeof(waSentDecoderThread), NORMALPRIO, SentDecoderThread, nullptr);

	/* Start HW layer */
	startSent();

	addConsoleAction("sentinfo", &printSentInfo);
}

#endif /* EFI_PROD_CODE */
