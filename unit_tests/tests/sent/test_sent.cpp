#include "pch.h"
#include "logicdata_csv_reader.h"
#include "sent_logic.h"

TEST(sent, testFordIdle) {
	int printDebug = 0;
	CsvReader reader(1, 0);

	reader.open("tests/sent/resources/ford-sent-idle.csv");

	static sent_channel channel;

	int lineCount = 0;

	double prevTimeStamp;

	while (reader.haveMore()) {
		double value = 0;
		double stamp = reader.readTimestampAndValues(&value);
		lineCount++;

		if (lineCount == 1) {
			// get first timestamp
			prevTimeStamp = stamp;
			continue;
		}
		// we care only about falling edges
		if (value < 0.5) {
			double diff = stamp - prevTimeStamp;

			// On STM32 we are running timer on 1/4 of cpu clock. Cpu clock is 168 MHz
			channel.Decoder(diff * 168'000'000 / 4);

			prevTimeStamp = stamp;
		}

		if (((lineCount % 100) == 0) && (printDebug)) {
			int ret;
			uint8_t stat;
			uint16_t sig0, sig1;

			ret = channel.GetSignals(&stat, &sig0, &sig1);
			if (ret == 0) {
				printf("SENT status 0x%01x, signals: 0x%03x, 0x%03x\n", stat, sig0, sig1);
			}
		}
	}
	printf("testFordIdle: Got %d lines\n", lineCount);

	if (printDebug) {
		uint8_t stat;
		uint16_t sig0, sig1;

		if (channel.GetSignals(&stat, &sig0, &sig1) == 0) {
			printf("Last valid fast msg Status 0x%01x Sig0 0x%03x Sig1 0x%03x\n", stat, sig0, sig1);
		}

		printf("Slow channels:\n");
		for (int i = 0; i < SENT_SLOW_CHANNELS_MAX; i++) {
			//if (scMsgFlags & BIT(i)) {
				printf(" ID %d: %d\n", channel.scMsg[i].id, channel.scMsg[i].data);
			//}
		}

		#if SENT_STATISTIC_COUNTERS
			sent_channel_stat &statistic = channel.statistic;
			printf("Restarts %d\n", statistic.RestartCnt);
			printf("Interval errors %d short, %d long\n", statistic.ShortIntervalErr, statistic.LongIntervalErr);
			printf("Total frames %d with CRC error %d (%f%%)\n", statistic.FrameCnt, statistic.CrcErrCnt, statistic.CrcErrCnt * 100.0 / statistic.FrameCnt);
			printf("Sync errors %d\n", statistic.SyncErr);
		#endif
	}

	#if SENT_STATISTIC_COUNTERS
		sent_channel_stat &statistic = channel.statistic;
		ASSERT_TRUE(statistic.RestartCnt == 0);
	#endif

	ASSERT_TRUE(lineCount > 100);
}
