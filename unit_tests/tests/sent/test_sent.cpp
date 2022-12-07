#include "pch.h"
#include "logicdata_csv_reader.h"
#include "sent_logic.h"

// On STM32 we are running timer on 1/4 of cpu clock. Cpu clock is 168 MHz
#define CORE_CLOCK				168'000'000
#define TIMER_CLOCK				(CORE_CLOCK / 4)
#define TicksToUs(ticks)		((float)(ticks) * 1000.0 * 1000.0 / TIMER_CLOCK)

static int sentTest_feedWithFile(sent_channel &channel, const char *file)
{
	int lineCount = 0;
	int printDebug = 0;
	CsvReader reader(1, 0);

	reader.open(file);

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

			channel.Decoder(diff * TIMER_CLOCK);

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

	if (printDebug) {
		uint8_t stat;
		uint16_t sig0, sig1;

		printf("Unit time %f uS\n", TicksToUs(channel.getTickTime()));

		if (channel.GetSignals(&stat, &sig0, &sig1) == 0) {
			printf("Last valid fast msg Status 0x%01x Sig0 0x%03x Sig1 0x%03x\n", stat, sig0, sig1);
		}

		printf("Slow channels:\n");
		/* run for all possible slow channel IDs (8 bit) */
		for (int i = 0; i < 256; i++) {
			int value;
			value = channel.GetSlowChannelValue(i);
			if (value < 0)
				continue;
			printf(" ID %d: %d\n", i, value);
		}

		#if SENT_STATISTIC_COUNTERS
			sent_channel_stat &statistic = channel.statistic;
			printf("Restarts %d\n", statistic.RestartCnt);
			printf("Interval errors %d short, %d long\n", statistic.ShortIntervalErr, statistic.LongIntervalErr);
			printf("Total frames %d with CRC error %d (%f%%)\n", statistic.FrameCnt, statistic.CrcErrCnt, statistic.CrcErrCnt * 100.0 / statistic.FrameCnt);
			printf("Total slow channel messages %d with crc6 errors %d (%f%%)\n", statistic.sc, statistic.scCrcErr, statistic.scCrcErr * 100.0 / statistic.sc);
			printf("Sync errors %d\n", statistic.SyncErr);
		#endif
	}

	return lineCount;
}

TEST(sent, testFordIdle) {
	static sent_channel channel;
	int lineCount = sentTest_feedWithFile(channel, "tests/sent/resources/ford-sent-idle.csv");
	ASSERT_TRUE(lineCount > 100);
	#if SENT_STATISTIC_COUNTERS
		sent_channel_stat &statistic = channel.statistic;
		ASSERT_TRUE(statistic.RestartCnt == 0);
	#endif
}

TEST(sent, testFordClosed) {
	static sent_channel channel;
	int lineCount = sentTest_feedWithFile(channel, "tests/sent/resources/ford-sent-closed.csv");
	ASSERT_TRUE(lineCount > 100);
	#if SENT_STATISTIC_COUNTERS
		sent_channel_stat &statistic = channel.statistic;
		/* TODO: bad captured data or real problem? */
		ASSERT_TRUE(statistic.RestartCnt <= 1);
	#endif
}

TEST(sent, testFuelPressure) {
	static sent_channel channel;
	int lineCount = sentTest_feedWithFile(channel, "tests/sent/resources/SENT-fuel-pressure.csv");
	ASSERT_TRUE(lineCount > 100);
	#if SENT_STATISTIC_COUNTERS
		sent_channel_stat &statistic = channel.statistic;
		ASSERT_TRUE(statistic.RestartCnt == 0);
		/* TODO: add more checks? Check data? */
	#endif
}

