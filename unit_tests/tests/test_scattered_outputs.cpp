#include "pch.h"
#include "FragmentEntry.h"

static uint8_t buffer10[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
static uint8_t buffer5[] = {11, 12, 13, 14, 15};

static FragmentEntry fragments[] = {
		FragmentEntry(buffer10, sizeof(buffer10)),
		FragmentEntry(buffer5, sizeof(buffer5)),
};

TEST(outputs, fragments) {
	uint8_t buffer[120];
	{
		uint8_t expected[] = {9, 10, 11, 12, 13};
		copyRange(buffer, fragments, 8, 5);
		EXPECT_TRUE( 0 == std::memcmp(buffer, expected, sizeof(expected)));
	}

	{
		uint8_t expected[] = {13, 14, 15};
		copyRange(buffer, fragments, 12, 3);
		EXPECT_TRUE( 0 == std::memcmp(buffer, expected, sizeof(expected)));
	}
}
