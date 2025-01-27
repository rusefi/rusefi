/*
 * sent.cpp
 *
 * SENT protocol decoder
 *
 * TODO support MAF sensors like 04E906051 see https://github.com/rusefi/rusefi-hardware/issues/146
 *
 * @date Oct 01, 2022
 * @author Andrey Gusakov <dron0gus@gmail.com>, (c) 2022
 */

#include "pch.h"

#if EFI_PROD_CODE
#if EFI_SENT_SUPPORT

#include "sent.h"
#include "init.h"
#include "sent_decoder.h"

#ifndef SENT_CHANNELS_NUM
#define SENT_CHANNELS_NUM		4 // Number of sent channels
#endif

static sent_channel channels[SENT_CHANNELS_NUM];

void sent_channel::Info() {
	uint8_t stat;
	uint16_t sig0, sig1;

	efiPrintf("Unit time %lu timer ticks", tickPerUnit);
	efiPrintf("Pause pulse detected %s", pausePulseReceived ? "Yes" : "No");
	efiPrintf("Total pulses %lu", pulseCounter);

	if (GetSignals(&stat, &sig0, &sig1) == 0) {
		efiPrintf("Last valid fast msg Status 0x%01x Sig0 0x%03x Sig1 0x%03x", stat, sig0, sig1);
	}

	efiPrintf("Slow channels:");
	/* run for all possible slow channel IDs (8 bit) */
	for (int i = 0; i < 256; i++) {
		int value;
		value = GetSlowChannelValue(i);
		if (value < 0)
			continue;
		efiPrintf(" ID %d: %d\n", i, value);
	}

	#if SENT_STATISTIC_COUNTERS
		efiPrintf("HW overflows %lu\n", statistic.hwOverflowCnt);

		efiPrintf("Pause pulses %lu\n", statistic.PauseCnt);
		efiPrintf("Restarts %lu", statistic.RestartCnt);
		efiPrintf("Interval errors %lu short, %lu long", statistic.ShortIntervalErr, statistic.LongIntervalErr);
		efiPrintf("Total frames %lu with CRC error %lu (%f%%)", statistic.FrameCnt, statistic.CrcErrCnt, statistic.CrcErrCnt * 100.0 / statistic.FrameCnt);
		efiPrintf("Total slow channel messages %lu with crc6 errors %lu (%f%%)", statistic.sc12 + statistic.sc16, statistic.scCrcErr, (statistic.sc12 + statistic.sc16) ? statistic.scCrcErr * 100.0 / (statistic.sc12 + statistic.sc16) : 0);
		efiPrintf("Sync errors %lu", statistic.SyncErr);
	#endif
}

/*==========================================================================*/
/* Decoder thread settings.													*/
/*==========================================================================*/

/* 4 per channel should be enough */
#define SENT_MB_SIZE		(4 * SENT_CHANNELS_NUM)

static msg_t sent_mb_buffer[SENT_MB_SIZE];
static MAILBOX_DECL(sent_mb, sent_mb_buffer, SENT_MB_SIZE);

static THD_WORKING_AREA(waSentDecoderThread, 256);

void SENT_ISR_Handler(uint8_t channel, uint16_t clocks, uint8_t flags) {
	/* encode to fit msg_t */
	msg_t msg = (flags << 24) | (channel << 16) | clocks;

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
			uint8_t flags = (msg >> 24) & 0xff;

			if (n < SENT_CHANNELS_NUM) {
				sent_channel &channel = channels[n];

				if (channel.Decoder(tick, flags) > 0) {
					/* report only for first channel */
					if (n == 0) {
						uint16_t sig0, sig1;
						channel.GetSignals(NULL, &sig0, &sig1);
						engine->sent_state.value0 = sig0;
						engine->sent_state.value1 = sig1;

						#if SENT_STATISTIC_COUNTERS
						    engine->sent_state.errorRate = 100.0 * channel.statistic.getErrorRate();
						#endif // SENT_STATISTIC_COUNTERS
					}

					SentInput input = static_cast<SentInput>((size_t)SentInput::INPUT1 + n);
					/* Call high level decoder from here */
					/* TODO: implemnet subscribers, like it is done for ADC */
					sentTpsDecode(input);
					sentPressureDecode(input);
				}
			}
		}
	}
}

static void printSentInfo() {
	for (int i = 0; i < SENT_CHANNELS_NUM; i++) {
		sent_channel &channel = channels[i];

        const char * pinName = getBoardSpecificPinName(engineConfiguration->sentInputPins[i]);
		efiPrintf("---- SENT input %d ---- on %s", i + 1, pinName);
		channel.Info();
		efiPrintf("--------------------");
	}
}

/* Don't be confused: this actually returns throttle body position */
/* TODO: remove, replace with getSentValues() */
float getSentValue(SentInput input) {
	size_t index = static_cast<size_t>(input) - static_cast<size_t>(SentInput::INPUT1);

	if (index < SENT_CHANNELS_NUM) {
		uint16_t sig0, sig1;
		sent_channel &channel = channels[index];

		if (channel.GetSignals(NULL, &sig0, &sig1) == 0) {

			// GM sig0 + sig1 == 0xfff but Ford does not
			/* scale to 0.0 .. 1.0 */
			return sig0;
		}
	}

    return NAN;
}

int getSentValues(SentInput input, uint16_t *sig0, uint16_t *sig1) {
	size_t index = static_cast<size_t>(input) - static_cast<size_t>(SentInput::INPUT1);

	if (index < SENT_CHANNELS_NUM) {
		sent_channel &channel = channels[index];

		return channel.GetSignals(NULL, sig0, sig1);
	}

	/* invalid channel */
    return -1;
}

/* Should be called once */
void initSent(void) {
	/* init interval mailbox */
	chMBObjectInit(&sent_mb, sent_mb_buffer, SENT_MB_SIZE);

	chThdCreateStatic(waSentDecoderThread, sizeof(waSentDecoderThread), NORMALPRIO, SentDecoderThread, nullptr);

	/* Start HW layer */
	startSent();

	addConsoleAction("sentinfo", &printSentInfo);
}

#endif /* EFI_SENT_SUPPORT */
#endif /* EFI_PROD_CODE */
