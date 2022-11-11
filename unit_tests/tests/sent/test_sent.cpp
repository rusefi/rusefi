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
			prevTimeStamp = stamp;
			continue;
		}
		double diff = stamp - prevTimeStamp;

		// todo: proper mult
		channel.Decoder(diff * 10'000'000);

		prevTimeStamp = stamp;

	}
	printf("testFordIdle: Got %d lines\n", lineCount);

	ASSERT_TRUE(lineCount > 100);
}
