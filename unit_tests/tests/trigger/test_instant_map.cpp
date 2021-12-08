/*
 * test_instant_map.cpp
 *
 *  Created on: Nov 30, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#include "pch.h"

static char buffer[255];
static FILE *fp;
static int m_lineIndex = -1;

bool haveMore() {
	bool result = fgets(buffer, sizeof(buffer), fp) != nullptr;
	m_lineIndex++;
	if (m_lineIndex == 0) {
		// skip header
		return haveMore();
	}

	return result;
}

TEST(trigger, instantMap) {
	const char s[2] = ",";

	fp = fopen("tests/resources/instant_map.csv", "r");
	ASSERT_TRUE(fp != nullptr);

	MapState mapState;
	EngineTestHelper eth(TEST_ENGINE);

	while (haveMore()) {
		strtok(buffer, s);
		strtok(NULL, s);
		char *map = strtok(NULL, s);
		if (map == nullptr) {
			break;
		}
		double mapValue = std::stod(map);
		printf("map %f\n", mapValue);

		if (m_lineIndex != 105 && m_lineIndex != 183 && m_lineIndex < 289) {
			ASSERT_FALSE(mapState.isPeak(false)) << m_lineIndex << " " << mapValue;
		}

		mapState.add(mapValue);
	}

	ASSERT_TRUE(m_lineIndex > 10);

}
