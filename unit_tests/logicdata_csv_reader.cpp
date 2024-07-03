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
	m_triggerColumnIndeces = triggerColumnIndeces;
	m_vvtColumnIndeces = vvtColumnIndeces;
	ASSERT_TRUE(fp != nullptr);
}

bool CsvReader::haveMore() {
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
	const char s[2] = ",";
	char *timeStampstr = trim(strtok(buffer, s));

	bool newTriggerState[TRIGGER_INPUT_PIN_COUNT];
	bool newVvtState[CAM_INPUTS_COUNT];

	for (size_t i = 0; i < m_triggerCount; i++) {
		char * triggerToken = trim(strtok(nullptr, s));
		newTriggerState[m_triggerColumnIndeces[i]] = triggerToken[0] == '1';
	}

	for (size_t i = 0; i < m_vvtCount; i++) {
		char *vvtToken = trim(strtok(nullptr, s));
		newVvtState[m_vvtColumnIndeces[i]] = vvtToken[0] == '1';
	}

	if (timeStampstr == nullptr) {
		firmwareError(ObdCode::OBD_PCM_Processor_Fault, "End of File");
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
		// todo: we invert VVT but we do not invert trigger input!!!
		hwHandleShaftSignal(index, newTriggerState[index], nowNt);

		currentState[index] = newTriggerState[index];
	}

	for (size_t vvtIndex = 0; vvtIndex < m_vvtCount ; vvtIndex++) {
		if (currentVvtState[vvtIndex] == newVvtState[vvtIndex]) {
			continue;
		}

		efitick_t nowNt = getTimeNowNt();
		bool isRising = newVvtState[vvtIndex];
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
		hwHandleVvtCamSignal(isRising, nowNt, bankIndex *2 + camIndex);

		currentVvtState[vvtIndex] = newVvtState[vvtIndex];

	}
}

void CsvReader::readLine(EngineTestHelper *eth) {
	if (!haveMore())
		return;
	processLine(eth);
}
