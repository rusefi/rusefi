/*
 * @file logicdata_csv_reader.cpp
 *
 * @date Jun 26, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#include "engine_test_helper.h"
#include "logicdata_csv_reader.h"

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

void CsvReader::open(const char *fileName, int *columnIndeces) {
	printf("Reading from %s\r\n", fileName);
	fp = fopen(fileName, "r");
	this->columnIndeces = columnIndeces;
	ASSERT_TRUE(fp != nullptr);
}

bool CsvReader::haveMore() {
	bool result = fgets(buffer, sizeof(buffer), fp) != nullptr;
	lineIndex++;
	if (lineIndex == 0) {
		// skip header
		return haveMore();
	}

	return result;
}

void CsvReader::processLine(EngineTestHelper *eth) {
	Engine *engine = &eth->engine;
	EXPAND_Engine

	const char s[2] = ",";
	char *line = buffer;

	char *timeStampstr = trim(strtok(line, s));
	bool newState[2];
	char *firstToken = trim(strtok(NULL, s));
	char *secondToken = trim(strtok(NULL, s));

	newState[columnIndeces[0]] = firstToken[0] == '1';
	if (secondToken != nullptr && triggerCount > 1) {
		newState[columnIndeces[1]] = secondToken[0] == '1';
	}

	double timeStamp = std::stod(timeStampstr);

	eth->setTimeAndInvokeEventsUs(1'000'000 * timeStamp);
	for (int index = 0; index < 2; index++) {
		bool current = newState[index];
		if (lastState[index] == current) {
			continue;
		}

		trigger_event_e event =
				(newState[index] ? riseEvents : fallEvents)[index];
		efitick_t nowNt = getTimeNowNt();
		engine->triggerCentral.handleShaftSignal(event, nowNt PASS_ENGINE_PARAMETER_SUFFIX);
	}

	// Copy the current state to the last state
	std::copy(std::begin(newState), std::end(newState), std::begin(lastState));
}

void CsvReader::readLine(EngineTestHelper *eth) {
	if (!haveMore())
		return;
	processLine(eth);
}
