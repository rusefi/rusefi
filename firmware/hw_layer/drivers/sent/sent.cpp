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

/* use 3 full frames + one additional pulse for unit time calibration */
#define SENT_CALIBRATION_PULSES	(1 + 3 * SENT_MSG_PAYLOAD_SIZE)

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

/* Helpers for Msg manipulations */
/* nibbles order: status, sig0_MSN, sig0_MidN, sig0_LSN, sig1_MSN, sig1_MidN, sig1_LSN, CRC */
/* we shift rxReg left for 4 bits on each neable received and put newest nibble
 * in [3:0] bits of rxReg, so when full message is received:
 * CRC is [3:0] - nibble 7
 * status is [31:28] - nibble 0
 * sig0 is [27:16], sig1 is [15:4] */
#define MsgGetNibble(msg, n)	(((msg) >> (4 * (7 - (n)))) & 0xf)
#define MsgGetStat(msg)			MsgGetNibble(msg, 0)
#define MsgGetSig0(msg)			(((msg) >> (4 * 4)) & 0xfff)
#define MsgGetSig1(msg)			(((msg) >> (1 * 4)) & 0xfff)
#define MsgGetCrc(msg)			MsgGetNibble(msg, 7)

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

	/* fast channel shift register*/
	uint32_t rxReg;
	/* fast channel last received valid message */
	uint32_t rxLast;

	/* slow channel shift registers and flags */
	uint16_t scMsgFlags;
	uint32_t scShift2;	/* shift register for bit 2 from status nibble */
	uint32_t scShift3;	/* shift register for bit 3 from status nibble */
	/* Slow channel decoder */
	int SlowChannelStore(uint8_t id, uint16_t data);
	int SlowChannelDecoder(void);

	/* CRC */
	uint8_t crc4(uint32_t data);
	uint8_t crc4_gm(uint32_t data);
	uint8_t crc4_gm_v2(uint32_t data);

	void restart(void);

public:
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

	/* Get last raw message */
	int GetMsg(uint32_t* rx);
	/* Unpack last valid message to status, signal0 and signal1
	 * Note:
	 * sig0 is nibbles 0 .. 2, where nibble 0 is MSB
	 * sig1 is niblles 5 .. 3, where niblle 5 is MSB */
	int GetSignals(uint8_t *pStat, uint16_t *pSig0, uint16_t *pSig1);

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
		/* Find longes pulse */
		if (clocks > tickPerUnit) {
			tickPerUnit = clocks;
		}
		/* ...this should be SYNC pulse */
		if (pulseCounter >= SENT_CALIBRATION_PULSES) {
			/* calculate Unit time from SYNC pulse */
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
				tickPerUnit = (clocks + (SENT_SYNC_INTERVAL + SENT_OFFSET_INTERVAL) / 2) /
								(SENT_SYNC_INTERVAL + SENT_OFFSET_INTERVAL);
				rxReg = 0;
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
				rxReg = (rxReg << 4) | (uint32_t)interval;

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
					if ((MsgGetCrc(rxReg) == crc4(rxReg)) ||
						(MsgGetCrc(rxReg) == crc4_gm(rxReg)) ||
						(MsgGetCrc(rxReg) == crc4_gm_v2(rxReg)))
					{
						/* Full packet with correct CRC has been received */
						rxLast = rxReg;
						/* TODO: add timestamp? */
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

int sent_channel::GetMsg(uint32_t* rx)
{
	if (rx) {
		*rx = rxLast;
	}

	/* TODO: add check if any packet was received */
	/* TODO: add check for time since last message reseived */
	return 0;
}

int sent_channel::GetSignals(uint8_t *pStat, uint16_t *pSig0, uint16_t *pSig1)
{
	uint32_t rx;
	int ret = GetMsg(&rx);

	if (ret < 0) {
		return ret;
	}

	/* NOTE different MSB packing for sig0 and sig1
	 * is it protocol-defined or device-specific?
	 * Also looks like some devices send 16 + 8 bit, not 12 + 12 */
	if (pStat) {
		*pStat = MsgGetStat(rx);
	}

	if (pSig0) {
		uint16_t tmp = MsgGetSig0(rx);
		*pSig0 = tmp;
	}

	if (pSig1) {
		uint16_t tmp = MsgGetSig1(rx);
		/* swap */
		tmp = 	((tmp >> 8) & 0x00f) |
				((tmp << 8) & 0xf00) |
				(tmp & 0x0f0);
		*pSig1 = tmp;
	}

	return 0;
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
	bool b2 = !!(MsgGetStat(rxLast) & BIT(2));
	bool b3 = !!(MsgGetStat(rxLast) & BIT(3));

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

/* This is correct for Si7215 */
/* This CRC is calculated for WHOLE message expect last nibble (CRC) */
uint8_t sent_channel::crc4(uint32_t data)
{
	size_t i;
	uint8_t crc = SENT_CRC_SEED; // initialize checksum with seed "0101"
	const uint8_t CrcLookup[16] = {0, 13, 7, 10, 14, 3, 9, 4, 1, 12, 6, 11, 15, 2, 8, 5};

	for (i = 0; i < 7; i++) {
		crc = crc ^ MsgGetNibble(data, i);
		crc = CrcLookup[crc];
	}

	return crc;
}

/* TODO: double check two following and use same CRC routine? */

/* This is correct for GM throttle body */
/* This CRC is calculated for message expect status nibble and minus CRC nibble */
uint8_t sent_channel::crc4_gm(uint32_t data)
{
	size_t i;
	uint8_t crc = SENT_CRC_SEED; // initialize checksum with seed "0101"
	const uint8_t CrcLookup[16] = {0, 13, 7, 10, 14, 3, 9, 4, 1, 12, 6, 11, 15, 2, 8, 5};

	for (i = 1; i < 7; i++) {
		crc = CrcLookup[crc];
		crc = (crc ^ MsgGetNibble(data, i)) & 0xf;
	}

	return crc;
}

/* This is correct for GDI fuel pressure sensor */
/* This CRC is calculated for message expect status nibble and minus CRC nibble */
uint8_t sent_channel::crc4_gm_v2(uint32_t data)
{
	size_t i;
	uint8_t crc = SENT_CRC_SEED; // initialize checksum with seed "0101"
	const uint8_t CrcLookup[16] = {0, 13, 7, 10, 14, 3, 9, 4, 1, 12, 6, 11, 15, 2, 8, 5};

	for (i = 1; i < 7; i++) {
		crc = CrcLookup[crc];
		crc = (crc ^ MsgGetNibble(data, i)) & 0xf;
	}
	// One more round with 0 as input
	crc = CrcLookup[crc];

	return crc;
}

void sent_channel::Info(void)
{
	int i;

	uint8_t stat;
	uint16_t sig0, sig1;

	efiPrintf("Unit time %d timer ticks", tickPerUnit);
	efiPrintf("Total pulses %d", pulseCounter);

	if (GetSignals(&stat, &sig0, &sig1) == 0) {
		efiPrintf("Last valid fast msg Status 0x%01x Sig0 0x%03x Sig1 0x%03x", stat, sig0, sig1);
	}

	if (scMsgFlags) {
		efiPrintf("Slow channels:");
		for (i = 0; i < SENT_SLOW_CHANNELS_MAX; i++) {
			if (scMsgFlags & BIT(i)) {
				efiPrintf(" ID %d: %d", scMsg[i].id, scMsg[i].data);
			}
		}
	}

	#if SENT_STATISTIC_COUNTERS
		efiPrintf("Restarts %d", statistic.RestartCnt);
		efiPrintf("Interval errors %d short, %d long", statistic.ShortIntervalErr, statistic.LongIntervalErr);
		efiPrintf("Total frames %d with crc error %d (%f %%)", statistic.FrameCnt, statistic.CrcErrCnt, statistic.CrcErrCnt * 100.0 / statistic.FrameCnt);
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

float getSentValue(int index) {
    // todo: just return ETB 0 to 100% for now?
    return 23;
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
