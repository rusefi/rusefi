/*
 * @file logicdata_csv_reader.cpp
 *
 * @date Jun 26, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#include "pch.h"
#include "logicdata_csv_reader.h"

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

/**
 * @param values reference of values array to modify
 * @return timestamp of current line
 */
double CsvReader::readTimestampAndValues(double *values) {
	const char s[2] = ",";
	char *line = buffer;

	char *timeStampstr = trim(strtok(line, s));
	double timeStamp = std::stod(timeStampstr);

	for (size_t i = 0; i < m_triggerCount; i++) {
		char *triggerToken = trim(strtok(nullptr, s));
		values[i] = std::stod(triggerToken);
	}

	return timeStamp;
}

// todo: separate trigger handling from csv file processing
void CsvReader::processLine(EngineTestHelper *eth) {
	Engine *engine = &eth->engine;

	const char s[2] = ",";
	char *timeStampstr = trim(strtok(buffer, s));

	bool newTriggerState[TRIGGER_INPUT_PIN_COUNT];
	bool newVvtState[CAM_INPUTS_COUNT];

	for (size_t i = 0;i<m_triggerCount;i++) {
		char * triggerToken = trim(strtok(nullptr, s));
		newTriggerState[triggerColumnIndeces[i]] = triggerToken[0] == '1';
	}

	for (size_t i = 0;i<m_vvtCount;i++) {
		char *vvtToken = trim(strtok(nullptr, s));
		if (vvtToken == nullptr) {
			criticalError("Null token in [%s]", buffer);
		}
		bool state = vvtToken[0] == '1';
		newVvtState[vvtColumnIndeces[i]] = state;
	}

	if (timeStampstr == nullptr) {
		criticalError("End of File");
		return;
	}

	double timeStamp = std::stod(timeStampstr);

	timeStamp += m_timestampOffset;

	eth->setTimeAndInvokeEventsUs(1'000'000 * timeStamp);
	for (size_t index = 0; index < m_triggerCount; index++) {
		if (currentState[index] == newTriggerState[index]) {
			continue;
		}

		efitick_t nowNt = getTimeNowNt();
        bool state;
		if (index == 0) {
		    state = newTriggerState[index] ^ flipOnRead ^ engineConfiguration->invertPrimaryTriggerSignal;
		} else {
		    state = newTriggerState[index] ^ flipOnRead ^ engineConfiguration->invertSecondaryTriggerSignal;
		}
		hwHandleShaftSignal(index, state, nowNt);

		currentState[index] = newTriggerState[index];
	}

	for (size_t vvtIndex = 0; vvtIndex < m_vvtCount ; vvtIndex++) {
		if (currentVvtState[vvtIndex] == newVvtState[vvtIndex]) {
			continue;
		}

		efitick_t nowNt = getTimeNowNt();
		TriggerValue event = newVvtState[vvtIndex] ^ flipVvtOnRead ^ engineConfiguration->invertCamVVTSignal ? TriggerValue::RISE : TriggerValue::FALL;
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
		hwHandleVvtCamSignal(event, nowNt, bankIndex *2 + camIndex);

		currentVvtState[vvtIndex] = newVvtState[vvtIndex];

	}
}

void CsvReader::readLine(EngineTestHelper *eth) {
	if (!haveMore())
		return;
	processLine(eth);
}
