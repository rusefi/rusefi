#include "pch.h"
#include "crc8hondak.h"


TEST(util, crc8hondak) {
	const uint8_t data[] = {0x1, 0, 0};

	ASSERT_EQ(140, crc_hondak_calc(data, 3));
}

// https://rusefi.com/forum/viewtopic.php?p=47283#p47283
TEST(util, crc8hondak_2) {
	const uint8_t data[] = {0, 0xD0, 0x06, 0};
	ASSERT_EQ(0x62, crc_hondak_calc(data, 4));
}
