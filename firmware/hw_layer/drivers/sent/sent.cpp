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

/*==========================================================================*/
/* Protocol definitions.													*/
/*==========================================================================*/

#define SENT_OFFSET_INTERVAL	12
#define SENT_SYNC_INTERVAL		(56 - SENT_OFFSET_INTERVAL) // 56 ticks - 12

#define SENT_MIN_INTERVAL		12
#define SENT_MAX_INTERVAL		15

#define SENT_CRC_SEED           0x05

#define SENT_MSG_DATA_SIZE      6
/* Status + two 12-bit signals + CRC */
#define SENT_MSG_PAYLOAD_SIZE   (1 + SENT_MSG_DATA_SIZE + 1)  // Size of payload

/*==========================================================================*/
/* Decoder configuration													*/
/*==========================================================================*/

#ifndef SENT_CHANNELS_NUM
#define SENT_CHANNELS_NUM		4 // Number of sent channels
#endif

/* collect statistic */
#define SENT_STATISTIC_COUNTERS	1

/* Maximum slow shannel mailboxes, DO NOT CHANGE */
#define SENT_SLOW_CHANNELS_MAX  16

/*==========================================================================*/
/* Decoder																	*/
/*==========================================================================*/

typedef enum
{
	SENT_STATE_CALIB = 0,
	SENT_STATE_INIT,
	SENT_STATE_SYNC,
	SENT_STATE_STATUS,
	SENT_STATE_SIG1_DATA1,
	SENT_STATE_SIG1_DATA2,
	SENT_STATE_SIG1_DATA3,
	SENT_STATE_SIG2_DATA1,
	SENT_STATE_SIG2_DATA2,
	SENT_STATE_SIG2_DATA3,
	SENT_STATE_CRC,
} SENT_STATE_enum;

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
	SENT_STATE_enum state = SENT_STATE_CALIB;

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
	/* Slow channel decoder */
	int SlowChannelStore(uint8_t id, uint16_t data);
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

	/* Decoder */
	int Decoder(uint16_t clocks);

	/* Show status */
	void Info(void);
};

static sent_channel channels[SENT_CHANNELS_NUM];

void sent_channel::restart(void)
{
	state = SENT_STATE_CALIB;
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
	if (state == SENT_STATE_CALIB) {
		/* Find longes pulse ... */
		if (clocks > tickPerUnit) {
			tickPerUnit = clocks;
		}
		/* ... of 9 pulses */
		if (pulseCounter >= (SENT_MSG_PAYLOAD_SIZE + 1)) {
			tickPerUnit = (tickPerUnit + (SENT_SYNC_INTERVAL + SENT_OFFSET_INTERVAL) / 2) /
							(SENT_SYNC_INTERVAL + SENT_OFFSET_INTERVAL);
			pulseCounter = 0;
			state = SENT_STATE_INIT;
		}
		return 0;
	}

	/* special case for out-of-sync state */
	if (state == SENT_STATE_INIT) {
		/* check if pulse looks like sync with allowed +/-20% deviation */
		int syncClocks = (SENT_SYNC_INTERVAL + SENT_OFFSET_INTERVAL) * tickPerUnit;

		if (((100 * clocks) >= (syncClocks * 80)) &&
			((100 * clocks) <= (syncClocks * 120))) {
			initStatePulseCounter = 0;
			/* adjust unit time */
			tickPerUnit = (clocks + (SENT_SYNC_INTERVAL + SENT_OFFSET_INTERVAL) / 2) /
							(SENT_SYNC_INTERVAL + SENT_OFFSET_INTERVAL);
			/* next state */
			state = SENT_STATE_STATUS;
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
		state = SENT_STATE_INIT;
		return -1;
	}

	switch(state)
	{
		case SENT_STATE_CALIB:
		case SENT_STATE_INIT:
			/* handled above, should not get in here */
			ret = -1;
			break;

		case SENT_STATE_SYNC:
			if (interval == SENT_SYNC_INTERVAL)
			{// sync interval - 56 ticks
				/* measured tick interval will be used until next sync pulse */
				tickPerUnit = (clocks + 56 / 2) / (SENT_SYNC_INTERVAL + SENT_OFFSET_INTERVAL);
				state = SENT_STATE_STATUS;
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
				state = SENT_STATE_INIT;
			}
			break;

		case SENT_STATE_STATUS:
		case SENT_STATE_SIG1_DATA1:
		case SENT_STATE_SIG1_DATA2:
		case SENT_STATE_SIG1_DATA3:
		case SENT_STATE_SIG2_DATA1:
		case SENT_STATE_SIG2_DATA2:
		case SENT_STATE_SIG2_DATA3:
		case SENT_STATE_CRC:
			if(interval <= SENT_MAX_INTERVAL)
			{
				nibbles[state - SENT_STATE_STATUS] = interval;

				if (state != SENT_STATE_CRC)
				{
					/* TODO: refactor */
					state = (SENT_STATE_enum)((int)state + 1);
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
					state = SENT_STATE_SYNC;
				}
			}
			else
			{
				#if SENT_STATISTIC_COUNTERS
					statistic.LongIntervalErr++;
				#endif

				state = SENT_STATE_INIT;
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

int sent_channel::SlowChannelStore(uint8_t id, uint16_t data)
{
	int i;

	/* Update already allocated messagebox? */
	for (i = 0; i < SENT_SLOW_CHANNELS_MAX; i++) {
		if ((scMsgFlags & BIT(i)) && (scMsg[i].id == id)) {
			scMsg[i].data = data;
			return 0;
		}
	}

	/* New message? Allocate messagebox */
	for (i = 0; i < SENT_SLOW_CHANNELS_MAX; i++) {
		if (!(scMsgFlags & BIT(i)))
		 {
			scMsg[i].data = data;
			scMsg[i].id = id;
			scMsgFlags |= (1 << i);
			return 0;
		}
	}

	/* No free mailboxes for new ID */
	return -1;
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

			/* TODO: add crc check */

			uint8_t id = (scShift2 >> 12) & 0x0f;
			uint16_t data = (scShift2 >> 4) & 0xff;

			return SlowChannelStore(id, data);
		}
	}
	if (1) {
		/* Enhanced Serial Message format */

		/* 0b11.1111.0xxx.xx0x.xxx0 ? */
		if ((scShift3 & 0x3f821) == 0x3f000) {
			uint8_t id;

			/* C-flag: configuration bit is used to indicate 16 bit format */
			bool sc16Bit = !!(scShift3 & (1 << 10));
			if (!sc16Bit) {
				/* 12 bit message, 8 bit ID */

				/* TODO: add crc check */

				id = ((scShift3 >> 1) & 0x0f) |
					 ((scShift3 >> 2) & 0xf0);
				uint16_t data = scShift2 & 0x0fff; /* 12 bit */

				/* TODO: add crc check */
				return SlowChannelStore(id, data);
			} else {
				/* 16 bit message, 4 bit ID */

				/* TODO: add crc check */

				id = (scShift3 >> 6) & 0x0f;
				uint16_t data = (scShift2 & 0x0fff) |
					   (((scShift3 >> 1) & 0x0f) << 12);

				return SlowChannelStore(id, data);
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

/*==========================================================================*/
/* Decoder thread settings.													*/
/*==========================================================================*/

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
