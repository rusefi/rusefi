/*
 * sent.cpp
 *
 * SENT protocol decoder
 *
 * @date Oct 01, 2022
 * @author Andrey Gusakov <dron0gus@gmail.com>, (c) 2022
 */

#if EFI_PROD_CODE || EFI_UNIT_TEST

#include "pch.h"

#include "sent.h"
#include "sent_logic.h"

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

/* convert CPU ticks to float Us */
#define TicksToUs(ticks)		((float)(ticks) * 1000.0 * 1000.0 / CORE_CLOCK)

void sent_channel::restart(void) {
	state = SENT_STATE_CALIB;
	pulseCounter = 0;
	currentStatePulseCounter = 0;
	hasPausePulse = false;
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
		statistic.sc = 0;
		statistic.scCrcErr = 0;
		statistic.RestartCnt++;
	#endif
}

uint32_t sent_channel::calcTickPerUnit(uint32_t clocks) {
	/* int division with rounding */
	return (clocks + (SENT_SYNC_INTERVAL + SENT_OFFSET_INTERVAL) / 2) /
			(SENT_SYNC_INTERVAL + SENT_OFFSET_INTERVAL);
}

float sent_channel::getTickTime(void) {
	return tickPerUnit;
}

int sent_channel::Decoder(uint16_t clocks) {
	int ret = 0;
	int interval;

	pulseCounter++;

	/* special case - tick time calculation */
	if (state == SENT_STATE_CALIB) {
		if (tickPerUnit == 0) {
			/* no tickPerUnit calculated yet
			 * lets assume this is sync pulse... */
			tickPerUnit = calcTickPerUnit(clocks);
		} else {
			/* some tickPerUnit calculated...
			 * Check next 1 + 6 + 1 pulses if they are valid with current tickPerUnit */
			interval = (clocks + tickPerUnit / 2) / tickPerUnit - SENT_OFFSET_INTERVAL;
			if ((interval >= 0) && (interval <= SENT_MAX_INTERVAL)) {
				currentStatePulseCounter++;
				if (currentStatePulseCounter == SENT_MSG_PAYLOAD_SIZE) {
					pulseCounter = 0;
					currentStatePulseCounter = 0;
					state = SENT_STATE_INIT;
				}
			} else {
				currentStatePulseCounter = 0;
				tickPerUnit = calcTickPerUnit(clocks);
			}
		}
		if (pulseCounter >= SENT_CALIBRATION_PULSES) {
			/* failed to calculate valid tickPerUnit, restart */
			restart();
		}
		return 0;
	}

	/* special case for out-of-sync state */
	if (state == SENT_STATE_INIT) {
		/* check if pulse looks like sync with allowed +/-20% deviation */
		int syncClocks = (SENT_SYNC_INTERVAL + SENT_OFFSET_INTERVAL) * tickPerUnit;

		if (((100 * clocks) >= (syncClocks * 80)) &&
			((100 * clocks) <= (syncClocks * 120))) {
			/* adjust unit time */
			tickPerUnit = calcTickPerUnit(clocks);
			/* we get here from calibration phase. calibration phase end with CRC nibble
			 * if we had to skip ONE pulse before we get sync - that means device sends pause
			 * pulses in between of messages */
			if (currentStatePulseCounter == 1) {
				hasPausePulse = true;
			}
			/* next state */
			currentStatePulseCounter = 0;
			state = SENT_STATE_STATUS;
		} else {
			currentStatePulseCounter++;
			/* 3 frames skipped, no SYNC detected - recalibrate */
			if (currentStatePulseCounter >= (9 * 3)) {
				restart();
			}
		}
		/* done for this pulse */
		return 0;
	}

	interval = (clocks + tickPerUnit / 2) / tickPerUnit - SENT_OFFSET_INTERVAL;

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
				tickPerUnit = calcTickPerUnit(clocks);
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
					state = hasPausePulse ? SENT_STATE_PAUSE : SENT_STATE_SYNC;
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
		case SENT_STATE_PAUSE:
			state = SENT_STATE_SYNC;
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

int sent_channel::GetMsg(uint32_t* rx) {
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

int sent_channel::StoreSlowChannelValue(uint8_t id, uint16_t data)
{
	size_t i;

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

int sent_channel::GetSlowChannelValue(uint8_t id)
{
	size_t i;

	for (i = 0; i < SENT_SLOW_CHANNELS_MAX; i++) {
		if ((scMsgFlags & BIT(i)) && (scMsg[i].id == id)) {
			return scMsg[i].data;
		}
	}

	/* not found */
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
	scCrcShift = (scCrcShift << 2) | ((uint32_t)b2 << 1) | b3;

	if (1) {
		/* Short Serial Message format */

		/* 0b1000.0000.0000.0000? */
		if ((scShift3 & 0xffff) == 0x8000) {
			/* Done receiving */

			/* TODO: add crc check */

			uint8_t id = (scShift2 >> 12) & 0x0f;
			uint16_t data = (scShift2 >> 4) & 0xff;

			return StoreSlowChannelValue(id, data);
		}
	}
	if (1) {
		/* Enhanced Serial Message format */

		/* 0b11.1111.0xxx.xx0x.xxx0 ? */
		if ((scShift3 & 0x3f821) == 0x3f000) {
			uint8_t id;

			uint8_t crc = (scShift2 >> 12) & 0x3f;
			#if SENT_STATISTIC_COUNTERS
				statistic.sc++;
			#endif
			if (crc == crc6(scCrcShift)) {
				/* C-flag: configuration bit is used to indicate 16 bit format */
				bool sc16Bit = !!(scShift3 & (1 << 10));
				if (!sc16Bit) {
					/* 12 bit message, 8 bit ID */
					id = ((scShift3 >> 1) & 0x0f) |
						 ((scShift3 >> 2) & 0xf0);
					uint16_t data = scShift2 & 0x0fff; /* 12 bit */

					/* TODO: add crc check */
					return StoreSlowChannelValue(id, data);
				} else {
					/* 16 bit message, 4 bit ID */
					id = (scShift3 >> 6) & 0x0f;
					uint16_t data = (scShift2 & 0x0fff) |
						   (((scShift3 >> 1) & 0x0f) << 12);

					return StoreSlowChannelValue(id, data);
				}
			} else {
				#if SENT_STATISTIC_COUNTERS
					statistic.scCrcErr++;
				#endif
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

uint8_t sent_channel::crc6(uint32_t data)
{
	size_t i;
	/* Seed 0x15 (21) */
	uint8_t crc = 0x15;
	/* CRC table for poly = 0x59 (x^6 + x^4 + x^3 + 1) */
	const uint8_t crc6_table[64] = {
		 0, 25, 50, 43, 61, 36, 15, 22, 35, 58, 17,  8, 30,  7, 44, 53,
		31,  6, 45, 52, 34, 59, 16,  9, 60, 37, 14, 23,  1, 24, 51, 42,
		62, 39, 12, 21,  3, 26, 49, 40, 29,  4, 47, 54, 32, 57, 18, 11,
		33, 56, 19, 10, 28,  5, 46, 55,  2, 27, 48, 41, 63, 38, 13, 20 };

	for (i = 0; i < 4; i++) {
		uint8_t tmp = (data >> (24 - 6 * (i + 1))) & 0x3f;
		crc = tmp ^ crc6_table[crc];
	}
	// Extra cound with 0 input
	crc = 0 ^ crc6_table[crc];

	return crc;
}

#endif // EFI_PROD_CODE || EFI_UNIT_TEST

#if EFI_PROD_CODE

static sent_channel channels[SENT_CHANNELS_NUM];

void sent_channel::Info(void) {
	uint8_t stat;
	uint16_t sig0, sig1;

	efiPrintf("Unit time %lu CPU ticks %f uS", tickPerUnit, TicksToUs(getTickTime()));
	efiPrintf("Total pulses %lu", pulseCounter);

	if (GetSignals(&stat, &sig0, &sig1) == 0) {
		efiPrintf("Last valid fast msg Status 0x%01x Sig0 0x%03x Sig1 0x%03x", stat, sig0, sig1);
	}

	if (scMsgFlags) {
		efiPrintf("Slow channels:");
		for (int i = 0; i < SENT_SLOW_CHANNELS_MAX; i++) {
			if (scMsgFlags & BIT(i)) {
				efiPrintf(" ID %d: %d", scMsg[i].id, scMsg[i].data);
			}
		}
	}

	#if SENT_STATISTIC_COUNTERS
		efiPrintf("Restarts %lu", statistic.RestartCnt);
		efiPrintf("Interval errors %lu short, %lu long", statistic.ShortIntervalErr, statistic.LongIntervalErr);
		efiPrintf("Total frames %lu with CRC error %lu (%f%%)", statistic.FrameCnt, statistic.CrcErrCnt, statistic.CrcErrCnt * 100.0 / statistic.FrameCnt);
		efiPrintf("Total slow channel messages %lu with crc6 errors %lu (%f%%)", statistic.sc, statistic.scCrcErr, statistic.scCrcErr * 100.0 / statistic.sc);
		efiPrintf("Sync errors %lu", statistic.SyncErr);
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

void SENT_ISR_Handler(uint8_t channel, uint16_t clocks) {
	/* encode to fit msg_t */
	msg_t msg = (channel << 16) | clocks;

	/* called from ISR */
	chSysLockFromISR();
	chMBPostI(&sent_mb, msg);
	chSysUnlockFromISR();
}

static void SentDecoderThread(void*) {
	while (true) {
		msg_t ret;
		msg_t msg;

		ret = chMBFetchTimeout(&sent_mb, &msg, TIME_INFINITE);

		if (ret == MSG_OK) {
			uint16_t tick = msg & 0xffff;
			uint8_t n = (msg >> 16) & 0xff;

			if (n < SENT_CHANNELS_NUM) {
				sent_channel &channel = channels[n];

				if (channel.Decoder(tick) > 0) {

    				uint16_t sig0, sig1;
    				channel.GetSignals(NULL, &sig0, &sig1);
    				engine->sent_state.value0 = sig0;
    				engine->sent_state.value1 = sig1;

    				#if SENT_STATISTIC_COUNTERS
    				    engine->sent_state.errorRate = channel.statistic.getErrorRate();
    				#endif // SENT_STATISTIC_COUNTERS


					/* Call high level decoder from here */
				}
			}
		}
	}
}

static void printSentInfo() {
	for (int i = 0; i < SENT_CHANNELS_NUM; i++) {
		efiPrintf("---- SENT ch %d ----", i);
		channels[i].Info();
		efiPrintf("--------------------");
	}
}

/* Don't be confused: this actually returns throttle body position */
/* TODO: remove, replace with getSentValues() */
float getSentValue(size_t index) {
	if (index < SENT_CHANNELS_NUM) {
		uint16_t sig0, sig1;

		if (channels[index].GetSignals(NULL, &sig0, &sig1) == 0) {

			// GM sig0 + sig1 == 0xfff but Ford does not
			/* scale to 0.0 .. 1.0 */
			return sig0;
		}
	}

    return NAN;
}

int getSentValues(size_t index, uint16_t *sig0, uint16_t *sig1) {
	if (index < SENT_CHANNELS_NUM) {
		return channels[index].GetSignals(NULL, sig0, sig1);
	}

	/* invalid channel */
    return -1;
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
