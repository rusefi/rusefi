#include "pch.h"
#include "logicdata_csv_reader.h"
#include "sent_logic.h"

TEST(sent, testFordIdle) {
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

	}
	printf("testFordIdle: Got %d lines\n", lineCount);

	ASSERT_TRUE(lineCount > 100);
}
