/**
 * @file rtc_helper.cpp
 * @brief Real Time Clock helper
 *
 * @date Feb 5, 2014
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include <string.h> 
#include <time.h>
#include "main.h"
#include "rfiutil.h"
#include "rtc_helper.h"

#if EFI_RTC || defined(__DOXYGEN__)
static LoggingWithStorage logger("RTC");
static RTCDateTime timespec;

extern bool rtcWorks;

#endif /* EFI_RTC */

void date_set_tm(struct tm *timp) {
	(void)timp;
#if EFI_RTC || defined(__DOXYGEN__)
        rtcConvertStructTmToDateTime(timp, 0, &timespec);
	rtcSetTime(&RTCD1, &timespec);
#endif /* EFI_RTC */
}

void date_get_tm(struct tm *timp) {
#if EFI_RTC || defined(__DOXYGEN__)
	rtcGetTime(&RTCD1, &timespec);
        rtcConvertDateTimeToStructTm(&timespec, timp, NULL);
#endif /* EFI_RTC */
}

static time_t GetTimeUnixSec(void) {
#if EFI_RTC || defined(__DOXYGEN__)
  struct tm tim;

  rtcGetTime(&RTCD1, &timespec);
  rtcConvertDateTimeToStructTm(&timespec, &tim, NULL);
  return mktime(&tim);
#else
  return (time_t)0;
#endif
}

static void SetTimeUnixSec(time_t unix_time) {
#if EFI_RTC || defined(__DOXYGEN__)
  struct tm tim;

#if defined __GNUC__
  struct tm *canary;
  /* If the conversion is successful the function returns a pointer
     to the object the result was written into.*/
  canary = localtime_r(&unix_time, &tim);
  osalDbgCheck(&tim == canary);
#else
  struct tm *t = localtime(&unix_time);
  memcpy(&tim, t, sizeof(struct tm));
#endif

  rtcConvertStructTmToDateTime(&tim, 0, &timespec);
  rtcSetTime(&RTCD1, &timespec);
#endif
}

static void put2(int offset, char *lcd_str, int value) {
	static char buff[_MAX_FILLER];
	efiAssertVoid(value >=0 && value <100, "value");
	itoa10(buff, value);
	if (value < 10) {
		lcd_str[offset] = '0';
		lcd_str[offset + 1] = buff[0];
	} else {
		lcd_str[offset] = buff[0];
		lcd_str[offset + 1] = buff[1];
	}
}

/**
 * @return true if we seem to know current date, false if no valid RTC state
 */
bool dateToStringShort(char *lcd_str) {
#if EFI_RTC || defined(__DOXYGEN__)
	strcpy(lcd_str, "0000_000000\0");
	struct tm timp;
	date_get_tm(&timp);
	if (timp.tm_year < 116 || timp.tm_year > 130) {
		// 2016 to 2030 is the valid range
		lcd_str[0] = 0;
		return false;
	}
	put2(0, lcd_str, timp.tm_mon + 1); // months since January	0-11
	put2(2, lcd_str, timp.tm_mday); // day of the month	1-31
	put2(5, lcd_str, timp.tm_hour); // hours since midnight	0-23
	put2(7, lcd_str, timp.tm_min);
	put2(9, lcd_str, timp.tm_sec);

	return true;
#else
	lcd_str[0] = 0;
	return false;
#endif
}

void dateToString(char *lcd_str) {
#if EFI_RTC || defined(__DOXYGEN__)
	// todo:
	// re-implement this along the lines of 	chvprintf("%04u-%02u-%02u %02u:%02u:%02u\r\n", timp.tm_year + 1900, timp.tm_mon + 1, timp.tm_mday, timp.tm_hour,
	// timp.tm_min, timp.tm_sec);
	// this would require a temporary mem stream - see datalogging and other existing usages

	strcpy(lcd_str, "00/00 00:00:00\0");
	struct tm timp;
	date_get_tm(&timp);			// get RTC date/time
	
	put2(0, lcd_str, timp.tm_mon + 1);
	put2(3, lcd_str, timp.tm_mday);
	put2(6, lcd_str, timp.tm_hour);
	put2(9, lcd_str, timp.tm_min);
	put2(12, lcd_str, timp.tm_sec);

#else
	lcd_str[0] = 0;
#endif /* EFI_RTC */
}

#if EFI_RTC || defined(__DOXYGEN__)
void printDateTime(void) {
	static time_t unix_time;
	struct tm timp;
	
	unix_time = GetTimeUnixSec();
	if (unix_time == -1) {
		scheduleMsg(&logger, "incorrect time in RTC cell");
	} else {
		scheduleMsg(&logger, "%D - unix time", unix_time);
		date_get_tm(&timp);

		appendMsgPrefix(&logger);
		appendPrintf(&logger, "Current RTC localtime is: %04u-%02u-%02u %02u:%02u:%02u w=%d", timp.tm_year + 1900, timp.tm_mon + 1, timp.tm_mday, timp.tm_hour,
				timp.tm_min, timp.tm_sec, rtcWorks);
		appendMsgPostfix(&logger);
		scheduleLogging(&logger);
	}
}

void setDateTime(const char *strDate) {
	if (strlen(strDate) > 0) {
		time_t unix_time = atoi(strDate);
		if (unix_time > 0) {
			SetTimeUnixSec(unix_time);
			printDateTime();
			return;
		}
	}
	scheduleMsg(&logger, "date_set Date parameter %s is wrong\r\n", strDate);
}
#endif /* EFI_RTC */

void initRtc(void) {
#if EFI_RTC || defined(__DOXYGEN__)
	GetTimeUnixSec(); // this would test RTC, see 'rtcWorks' variable, see #311
	printMsg(&logger, "initRtc()");
#endif /* EFI_RTC */
}
