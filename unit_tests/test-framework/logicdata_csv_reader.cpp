/*
 * @file logicdata_csv_reader.cpp
 *
 * @date Jun 26, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#include "pch.h"
#include "logicdata_csv_reader.h"
#include "unit_test_logger.h"

static char* trim(char *str) {
	while (str != nullptr && str[0] == ' ') {
		str++;
	}
	return str;
}

CsvReader::~CsvReader() {
	if (fp) {
		fclose(fp);
	}
}

void CsvReader::open(const char *fileName, const int* triggerColumnIndeces, const int *vvtColumnIndeces) {
	printf("Reading from %s\r\n", fileName);
	fp = fopen(fileName, "r");
	this->triggerColumnIndeces = triggerColumnIndeces;
	this->vvtColumnIndeces = vvtColumnIndeces;
	ASSERT_TRUE(fp != nullptr);
}

bool CsvReader::haveMore() {
	if (fp == nullptr) {
		throw std::runtime_error("No file");
	}
	bool result = fgets(buffer, sizeof(buffer), fp) != nullptr;
	m_lineIndex++;
	if (m_lineIndex == 0) {
		// skip header
		return haveMore();
	}

	return result;
}

static const char COMMA_SEPARATOR[2] = ",";

#define readFirstTokenAndRememberInputString(input) trim(strtok(input, COMMA_SEPARATOR))
#define readNextToken() trim(strtok(nullptr, COMMA_SEPARATOR))

/**
 * @param values reference of values array to modify
 * @return timestamp of current line
 */
double CsvReader::readTimestampAndValues(double *values) {
	char *timeStampstr = readFirstTokenAndRememberInputString(buffer);
	double timeStamp = std::atof(timeStampstr);

	for (size_t i = 0; i < m_triggerCount; i++) {
		char *triggerToken = readNextToken();
		values[i] = std::atof(triggerToken);
	}

	return timeStamp;
}

// this is about TS logs generated during trigger tests and viewing these files by humans.
// Emulate 500Hz refresh rate
#define TIME_DELTA	(1.0/500.0)

// todo: separate trigger handling from csv file processing, maybe reuse 'readTimestampAndValues'?
void CsvReader::processLine(EngineTestHelper *eth) {
	Engine *engine = &eth->engine;

	bool newTriggerState[TRIGGER_INPUT_PIN_COUNT];
	bool newVvtState[CAM_INPUTS_COUNT];

	char *timeStampstr;

	if (timestampColumnIndex < 0) {
		// legacy layout: first column is the timestamp, then optional skipped
		// columns, then the trigger and vvt channels
		timeStampstr = readFirstTokenAndRememberInputString(buffer);

		for (int i = 0; i < readingOffset; i++) {
			readNextToken();
		}

		for (size_t i = 0; i < m_triggerCount; i++) {
			char *triggerToken = readNextToken();
			newTriggerState[triggerColumnIndeces[i]] = triggerToken != nullptr && triggerToken[0] == '1';
		}

		for (size_t i = 0; i < m_vvtCount; i++) {
			char *vvtToken = readNextToken();
			if (vvtToken == nullptr) {
				criticalError("Null token in [%s]", buffer);
			}
			newVvtState[vvtColumnIndeces[i]] = vvtToken[0] == '1';
		}
	} else {
		// explicit layout: tokenize the whole line into columns, then pick the
		// timestamp column and the trigger/vvt channels which start at
		// 'readingOffset'
		static const size_t MAX_COLUMNS = 32;
		char *columns[MAX_COLUMNS] = {};
		size_t columnCount = 0;

		char *token = readFirstTokenAndRememberInputString(buffer);
		while (token != nullptr && columnCount < MAX_COLUMNS) {
			columns[columnCount++] = token;
			token = readNextToken();
		}

		if ((size_t)timestampColumnIndex >= columnCount
				|| readingOffset + m_triggerCount + m_vvtCount > columnCount) {
			// not a data row (e.g. a "MARK ###" marker line) - skip it
			return;
		}
		timeStampstr = columns[timestampColumnIndex];

		// skip header/units rows where the timestamp column is not numeric
		// (e.g. "Time" or "ms"); this recording carries several header lines
		if (timeStampstr == nullptr
				|| !(isdigit((unsigned char)timeStampstr[0]) || timeStampstr[0] == '-' || timeStampstr[0] == '+' || timeStampstr[0] == '.')) {
			return;
		}

		for (size_t i = 0; i < m_triggerCount; i++) {
			char *triggerToken = columns[readingOffset + i];
			newTriggerState[triggerColumnIndeces[i]] = triggerToken != nullptr && triggerToken[0] == '1';
		}

		for (size_t i = 0; i < m_vvtCount; i++) {
			char *vvtToken = columns[readingOffset + m_triggerCount + i];
			if (vvtToken == nullptr) {
				criticalError("Null token in [%s]", buffer);
			}
			newVvtState[vvtColumnIndeces[i]] = vvtToken[0] == '1';
		}
	}

	if (timeStampstr == nullptr) {
		criticalError("End of File");
		return;
	}

	double timeStamp = std::atof(timeStampstr) * timestampScale;
	history.add(timeStamp);

	if (normalizeTimestamps) {
		if (!m_haveTimestampBase) {
			m_timestampBase = timeStamp;
			m_haveTimestampBase = true;
		}
		timeStamp -= m_timestampBase;
	}

	timeStamp += m_timestampOffset;

#ifdef TIME_DELTA
	// Fill the gap
	while (lastTimeStamp + TIME_DELTA < timeStamp) {
		lastTimeStamp += TIME_DELTA;
		eth->setTimeAndInvokeEventsUs(1'000'000 * lastTimeStamp);
		writeUnitTestLogLine();
	}
#endif

	eth->setTimeAndInvokeEventsUs(1'000'000 * timeStamp);
	for (size_t index = 0; index < m_triggerCount; index++) {
		if (currentState[index] == newTriggerState[index]) {
			continue;
		}

		efitick_t nowNt = getTimeNowNt();
		bool state = newTriggerState[index] ^ flipOnRead;
		hwHandleShaftSignal(index, state, nowNt);

		currentState[index] = newTriggerState[index];
	}

	for (size_t vvtIndex = 0; vvtIndex < m_vvtCount ; vvtIndex++) {
		if (currentVvtState[vvtIndex] == newVvtState[vvtIndex]) {
			continue;
		}

		efitick_t nowNt = getTimeNowNt();
		bool state = newVvtState[vvtIndex] ^ flipVvtOnRead;

		// todo: configurable selection of vvt mode - dual bank or dual cam single bank
		int bankIndex;
		int camIndex;
		if (twoBanksSingleCamMode) {
			bankIndex = vvtIndex;
			camIndex = 0;
		} else {
			bankIndex = vvtIndex / 2;
			camIndex = vvtIndex % 2;
		}
		hwHandleVvtCamSignal(state, nowNt, bankIndex *2 + camIndex);

		currentVvtState[vvtIndex] = newVvtState[vvtIndex];

	}
	writeUnitTestLogLine();
	lastTimeStamp = timeStamp;
}

void CsvReader::readLine(EngineTestHelper *eth) {
	if (!haveMore())
		return;
	processLine(eth);
	engine->periodicSlowCallback();
}
