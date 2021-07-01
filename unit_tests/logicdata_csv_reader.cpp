/*
 * @file logicdata_csv_reader.cpp
 *
 * @date Jun 26, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#include "engine_test_helper.h"
#include "logicdata_csv_reader.h"

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
	if (secondToken != nullptr && m_triggerCount > 1) {
		newState[columnIndeces[1]] = secondToken[0] == '1';
	}

	double timeStamp = std::stod(timeStampstr);

	timeStamp += m_timestampOffset;

	eth->setTimeAndInvokeEventsUs(1'000'000 * timeStamp);
	for (int index = 0; index < m_triggerCount; index++) {
		if (currentState[index] == newState[index]) {
			continue;
		}

		efitick_t nowNt = getTimeNowNt();
		hwHandleShaftSignal(index, newState[index], nowNt PASS_ENGINE_PARAMETER_SUFFIX);

		currentState[index] = newState[index];
	}
}

void CsvReader::readLine(EngineTestHelper *eth) {
	if (!haveMore())
		return;
	processLine(eth);
}
