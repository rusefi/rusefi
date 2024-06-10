/**
 * @file rtc_helper.cpp
 * @brief Real Time Clock helper
 *
 * @date Feb 5, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include <string.h>

#include "rtc_helper.h"

#if EFI_RTC
#include "rusefi_types.h"
#endif // EFI_RTC

#if EFI_PROD_CODE
#include <sys/time.h>

// Lua needs this function, but we don't necessarily have to implement it
extern "C" int _gettimeofday(timeval* tv, void* tzvp) {
	(void)tv; (void)tzvp;
	return 0;
}
#endif // EFI_PROD_CODE

#if EFI_RTC
void initRtc() {
	efiPrintf("initRtc()");
	printDateTime(); // this would test RTC, see #311
}

static const char * const monthAbbrs[] = {
	"Jan", "Feb", "Mar",
	"Apr", "May", "Jun",
	"Jul", "Aug", "Sep",
	"Oct", "Nov", "Dec"
};

void printRtcDateTime() {
	efidatetime_t dateTime = getRtcDateTime();
	// prints the date like: 19 sep 2022 21:19:55
	efiPrintf("Current RTC time: %02u %s %04lu %02u:%02u:%02u",
			dateTime.day, monthAbbrs[dateTime.month - 1], dateTime.year,
			dateTime.hour, dateTime.minute, dateTime.second);
}

void setRtcDateTime(efidatetime_t const * const dateTime) {
	RTCDateTime timespec = convertRtcDateTimeFromEfi(dateTime);
	rtcSetTime(&RTCD1, &timespec);
}

efidatetime_t getRtcDateTime() {
	RTCDateTime timespec;
	rtcGetTime(&RTCD1, &timespec);
	return convertRtcDateTimeToEfi(&timespec);
}

efidatetime_t convertRtcDateTimeToEfi(RTCDateTime const * const timespec) {
	uint32_t second = timespec->millisecond / 1000;
	uint16_t minute = second / 60;
	second -= minute * 60;
	uint8_t hour = minute / 60;
	minute -= hour * 60;

	efidatetime_t const dateTime = {
		.year = timespec->year + RTC_BASE_YEAR,
		.month = (uint8_t)timespec->month,
		.day = (uint8_t)timespec->day,
		.hour = hour,
		.minute = (uint8_t)minute,
		.second = (uint8_t)second,
	};
	return dateTime;
}

RTCDateTime convertRtcDateTimeFromEfi(efidatetime_t const * const dateTime) {
	RTCDateTime timespec;
	timespec.year = dateTime->year - RTC_BASE_YEAR; // ChibiOS year origin is e.g. 1980
	timespec.month = dateTime->month; // [1..12]
	timespec.day = dateTime->day; // [1..31]
	timespec.millisecond = (((dateTime->hour * 60) + dateTime->minute) * 60 + dateTime->second) * 1000; // ms since midnight
	timespec.dayofweek = RTC_DAY_CATURDAY; // CATURDAY: 0 ... ?
	timespec.dstflag = 0; // 0 ... ?
	return timespec;
}

// TODO(nms): move to e.g. efitime ?

static void put2(int offset, char *lcd_str, int value) {
	static char buff[_MAX_FILLER];
	efiAssertVoid(ObdCode::CUSTOM_ERR_6666, value >=0 && value <100, "value");
	itoa10(buff, value);
	if (value < 10) {
		lcd_str[offset] = '0';
		lcd_str[offset + 1] = buff[0];
	} else {
		lcd_str[offset] = buff[0];
		lcd_str[offset + 1] = buff[1];
	}
}
#endif // EFI_RTC


/**
 * @return true if we seem to know current date, false if no valid RTC state
 */
bool dateToStringShort(char *lcd_str) {
#if EFI_RTC
	strcpy(lcd_str, "000000_000000\0");
	efidatetime_t dateTime = getRtcDateTime();
	if (dateTime.year < 2016 || dateTime.year > 2030) {
		// 2016 to 2030 is the valid range
		lcd_str[0] = 0;
		return false;
	}

	put2(0, lcd_str, dateTime.year % 100);		// year, format as just the last two digits
	put2(2, lcd_str, dateTime.month);		    // month	1-12
	put2(4, lcd_str, dateTime.day);				// day of the month	1-31

	put2(7, lcd_str, dateTime.hour);			// hours since midnight	0-23
	put2(9, lcd_str, dateTime.minute);			// minutes
	put2(11, lcd_str, dateTime.second);			// seconds

	return true;
#else // EFI_RTC
	lcd_str[0] = 0;
	return false;
#endif // EFI_RTC
}

void dateToString(char *lcd_str) {
#if EFI_RTC
	// todo:
	// re-implement this along the lines of 	chvprintf("%04u-%02u-%02u %02u:%02u:%02u\r\n", timp.tm_year + 1900, timp.tm_mon + 1, timp.tm_mday, timp.tm_hour,
	// timp.tm_min, timp.tm_sec);
	// this would require a temporary mem stream - see datalogging and other existing usages

	strcpy(lcd_str, "00/00 00:00:00\0");
	efidatetime_t dateTime = getRtcDateTime();
	
	put2(0, lcd_str, dateTime.month);
	put2(3, lcd_str, dateTime.day);
	put2(6, lcd_str, dateTime.hour);
	put2(9, lcd_str, dateTime.minute);
	put2(12, lcd_str, dateTime.second);
#else // EFI_RTC
	lcd_str[0] = 0;
#endif // EFI_RTC
}
