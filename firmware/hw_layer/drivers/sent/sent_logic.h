/**
 * sent_logic.h
 */

#pragma once

#ifndef SENT_CHANNELS_NUM
#define SENT_CHANNELS_NUM		4 // Number of sent channels
#endif

/* collect statistic */
#define SENT_STATISTIC_COUNTERS	1

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
