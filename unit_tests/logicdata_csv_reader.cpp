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

void CsvReader::open(const char *fileName, const int* columnIndeces) {
	printf("Reading from %s\r\n", fileName);
	fp = fopen(fileName, "r");
	this->columnIndeces = columnIndeces;
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

void CsvReader::processLine(EngineTestHelper *eth) {
	Engine *engine = &eth->engine;
	EXPAND_Engine

	const char s[2] = ",";
	char *line = buffer;

	char *timeStampstr = trim(strtok(line, s));

	bool newState[TRIGGER_INPUT_PIN_COUNT];
	bool newVvtState[CAM_INPUTS_COUNT];

	for (size_t i = 0;i<m_triggerCount;i++) {
		char * triggerToken = trim(strtok(NULL, s));
		newState[columnIndeces[i]] = triggerToken[0] == '1';
	}

	for (size_t i = 0;i<m_vvtCount;i++) {
		char *vvtToken = trim(strtok(NULL, s));
		newVvtState[i] = vvtToken[0] == '1';
	}

	if (timeStampstr == nullptr) {
		firmwareError(OBD_PCM_Processor_Fault, "End of File");
		return;
	}

	double timeStamp = std::stod(timeStampstr);

	timeStamp += m_timestampOffset;

	eth->setTimeAndInvokeEventsUs(1'000'000 * timeStamp);
	for (size_t index = 0; index < m_triggerCount; index++) {
		if (currentState[index] == newState[index]) {
			continue;
		}

		efitick_t nowNt = getTimeNowNt();
		// todo: we invert VVT but we do not invert trigger input!!!
		hwHandleShaftSignal(index, newState[index], nowNt PASS_ENGINE_PARAMETER_SUFFIX);

		currentState[index] = newState[index];
	}

	for (size_t vvtIndex = 0; vvtIndex < m_vvtCount ; vvtIndex++) {
		if (currentVvtState[vvtIndex] == newVvtState[vvtIndex]) {
			continue;
		}

		efitick_t nowNt = getTimeNowNt();
		trigger_value_e event = newVvtState[vvtIndex] ^ engineConfiguration->invertCamVVTSignal ? TV_RISE : TV_FALL;
		// todo: configurable selection of vvt mode - dual bank or dual cam single bank
		int bankIndex = vvtIndex;
		int camIndex = 0;
		hwHandleVvtCamSignal(event, nowNt, bankIndex *2 + camIndex PASS_ENGINE_PARAMETER_SUFFIX);

		currentVvtState[vvtIndex] = newVvtState[vvtIndex];

	}
}

void CsvReader::readLine(EngineTestHelper *eth) {
	if (!haveMore())
		return;
	processLine(eth);
}
