#include "pch.h"

/**
 * problem: we have three files with bits and pieces of time API and documentation
 * 1) this implementation class
 * 2) rusEFI header efitime.h
 * 3) libfirmware header rusefi_time_types.h
 */

#if !EFI_UNIT_TEST

#include <rusefi/rusefi_time_wraparound.h>

static WrapAround62 timeNt;

/**
 * 64-bit counter CPU/timer cycles since MCU reset
 */
efitick_t getTimeNowNt() {
	return timeNt.update(getTimeNowLowerNt());
}

/**
 * 64-bit result would not overflow, but that's complex stuff for our 32-bit MCU
 */
efitimeus_t getTimeNowUs() {
	ScopePerf perf(PE::GetTimeNowUs);
	return NT2US(getTimeNowNt());
}

/**
 * 32 bit return type overflows in 23(or46?) days. tag#4554. I think we do not expect rusEFI to run for 23 days straight days any time soon?
 */
efitimems_t getTimeNowMs() {
	return US2MS(getTimeNowUs());
}

/**
 * 32 bit integer number of seconds since ECU boot.
 * 31,710 years - would not overflow during our life span.
 */
efitimesec_t getTimeNowS() {
	return getTimeNowUs() / US_PER_SECOND;
}

#endif /* !EFI_UNIT_TEST */

/**
 * Days since 1970-01-01 for a proleptic Gregorian civil date.
 * See "days_from_civil" http://howardhinnant.github.io/date_algorithms.html
 */
static int32_t daysFromCivil(int32_t y, uint32_t m, uint32_t d) {
	y -= m <= 2;
	const int32_t era = (y >= 0 ? y : y - 399) / 400;
	const uint32_t yoe = static_cast<uint32_t>(y - era * 400);            // [0, 399]
	const uint32_t doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1;  // [0, 365]
	const uint32_t doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;           // [0, 146096]
	return era * 146097 + static_cast<int32_t>(doe) - 719468;
}

// UTC date-time to Unix epoch seconds; replaces newlib mktime which drags in tzset+sscanf, see #6876
uint32_t dateTimeToEpochTime(const efidatetime_t& dateTime) {
	int32_t days = daysFromCivil(dateTime.year, dateTime.month, dateTime.day);
	return (uint32_t)days * 86400
		+ dateTime.hour * 3600
		+ dateTime.minute * 60
		+ dateTime.second;
}
