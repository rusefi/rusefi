#include "pch.h"

#include "efitime.h"

static uint32_t epochOf(uint32_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second) {
	efidatetime_t dateTime;
	dateTime.year = year;
	dateTime.month = month;
	dateTime.day = day;
	dateTime.hour = hour;
	dateTime.minute = minute;
	dateTime.second = second;
	return dateTimeToEpochTime(dateTime);
}

// expected values from GNU date -u -d '...' +%s
TEST(util, dateTimeToEpochTime) {
	// epoch origin
	EXPECT_EQ(0u, epochOf(1970, 1, 1, 0, 0, 0));
	EXPECT_EQ(1u, epochOf(1970, 1, 1, 0, 0, 1));
	// last second of the epoch's first year
	EXPECT_EQ(31535999u, epochOf(1970, 12, 31, 23, 59, 59));

	// leap day of a regular leap year
	EXPECT_EQ(1709164800u, epochOf(2024, 2, 29, 0, 0, 0));
	// 2000 is a leap year despite being divisible by 100 (divisible by 400)
	EXPECT_EQ(951782400u, epochOf(2000, 2, 29, 0, 0, 0));
	EXPECT_EQ(951868800u, epochOf(2000, 3, 1, 0, 0, 0));

	// around a non-leap Feb: 2023-02-28 -> 2023-03-01 is one day
	EXPECT_EQ(epochOf(2023, 2, 28, 0, 0, 0) + 86400, epochOf(2023, 3, 1, 0, 0, 0));
	// around a leap Feb: 2024-02-28 -> 2024-03-01 is two days
	EXPECT_EQ(epochOf(2024, 2, 28, 0, 0, 0) + 2 * 86400, epochOf(2024, 3, 1, 0, 0, 0));

	// arbitrary modern timestamps
	EXPECT_EQ(1455323051u, epochOf(2016, 2, 13, 0, 24, 11));
	EXPECT_EQ(1786635296u, epochOf(2026, 8, 13, 15, 34, 56));
	// full hour/minute/second contribution
	EXPECT_EQ(epochOf(2026, 8, 13, 0, 0, 0) + 23 * 3600 + 59 * 60 + 59, epochOf(2026, 8, 13, 23, 59, 59));

	// year 2038 is fine since we return unsigned: valid until 2106
	EXPECT_EQ(2147483648u, epochOf(2038, 1, 19, 3, 14, 8));
	EXPECT_EQ(4102444800u, epochOf(2100, 1, 1, 0, 0, 0));
	// 2100 is NOT a leap year (divisible by 100, not by 400)
	EXPECT_EQ(epochOf(2100, 2, 28, 0, 0, 0) + 86400, epochOf(2100, 3, 1, 0, 0, 0));
}
