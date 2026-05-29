#include "pch.h"

#include "table_helper.h"

TEST(tableHelper, setRpmTableBin16Float) {
	float rpmBins[16];
	setRpmTableBin(rpmBins);

	// idleRpm = 800, topRpm = DEFAULT_RPM_AXIS_HIGH_VALUE (7000)
	// array[0] = idleRpm - 150 = 650
	// rpmStep = efiRound((7000 - 800) / 14, 50) - 150 = 450 - 150 = 300
	// array[i] = 800 + 300 * (i - 1) for i = 1..14
	// array[15] = topRpm = 7000
	const float expected[16] = {
		650,
		800, 1100, 1400, 1700, 2000, 2300, 2600, 2900,
		3200, 3500, 3800, 4100, 4400, 4700,
		7000
	};

	for (int i = 0; i < 16; i++) {
		EXPECT_FLOAT_EQ(expected[i], rpmBins[i]) << "rpmBins[" << i << "]";
	}
}
