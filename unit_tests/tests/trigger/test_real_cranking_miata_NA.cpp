/*
 * @file test_real_cranking_miata_NA.cpp
 *
 * @date May 22, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#include "engine_test_helper.h"
#include <string>

static constexpr trigger_event_e riseEvents[] = { SHAFT_PRIMARY_RISING,
		SHAFT_SECONDARY_RISING, SHAFT_3RD_RISING };
static constexpr trigger_event_e fallEvents[] = { SHAFT_PRIMARY_FALLING,
		SHAFT_SECONDARY_FALLING, SHAFT_3RD_FALLING };

static char* trim(char *str) {
	while (str != nullptr && str[0] == ' ') {
		str++;
	}
	return str;
}

class CsvReader {
public:
	FILE *fp;
	char buffer[255];

	bool currentState[2];

	int lineIndex = -1;

	void open(char *fileName) {
		fp = fopen(fileName, "r");
		ASSERT_TRUE(fp != nullptr);
	}

	bool haveMore() {
		bool result = fgets(buffer, sizeof(buffer), fp) != nullptr;
		lineIndex++;
		if (lineIndex == 0) {
			// skip header
			return haveMore();
		}

		return result;
	}

	void processLine(EngineTestHelper *eth) {
		Engine *engine = &eth->engine;
		EXPAND_Engine

		const char s[2] = ",";
		char *line = buffer;

		char *timeStampstr = trim(strtok(line, s));
		bool newState[2];
		newState[0] = trim(strtok(NULL, s))[0] == '1';
		newState[1] = trim(strtok(NULL, s))[0] == '1';

		double timeStamp = std::stod(timeStampstr);

		eth->setTimeAndInvokeEventsUs(1'000'000 * timeStamp);
		for (int index = 0; index < 2; index++) {
			if (currentState[index] == newState[index]) {
				continue;
			}
			trigger_event_e event =
					(newState[index] ? riseEvents : fallEvents)[index];
			efitick_t nowNt = getTimeNowNt();
			engine->triggerCentral.handleShaftSignal(event, nowNt PASS_ENGINE_PARAMETER_SUFFIX);

			currentState[index] = newState[index];
		}

	}

	void readLine(EngineTestHelper *eth) {
		if (!haveMore())
			return;
		processLine(eth);
	}

};

TEST(cranking, realCrankingFromFile) {
	CsvReader reader;
	reader.open("tests/trigger/recourses/cranking_na_3.csv");

	WITH_ENGINE_TEST_HELPER (MIATA_NA6_MAP);

	ssize_t read;

	for (int i = 0; i < 8; i++) {
		reader.readLine(&eth);
	}

	ASSERT_EQ( 0, GET_RPM())<< reader.lineIndex;
	ASSERT_EQ( 0, eth.recentWarnings()->getCount())<< "warningCounter#got synch";

	reader.readLine(&eth);
	ASSERT_EQ( 94, GET_RPM())<< reader.lineIndex;

	for (int i = 0; i < 6; i++) {
		reader.readLine(&eth);
	}
	ASSERT_EQ( 1, eth.recentWarnings()->getCount())<< "warningCounter#with synch";

	while (reader.haveMore()) {
		reader.processLine(&eth);
	}
	ASSERT_EQ( 3, eth.recentWarnings()->getCount())<< "warningCounter#realCranking";
}
