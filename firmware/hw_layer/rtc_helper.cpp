/**
 * @file rtc_helper.cpp
 * @brief Real Time Clock helper
 *
 * @date Feb 5, 2014
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#include <string.h> 
#include <time.h>
#include "main.h"
#include "rfiutil.h"
#include "chrtclib.h"
#include "rtc_helper.h"

#if EFI_RTC || defined(__DOXYGEN__)
static LoggingWithStorage logger("RTC");

#endif /* EFI_RTC */

void date_set_tm(struct tm *timp) {
	(void)timp;
#if EFI_RTC || defined(__DOXYGEN__)
	rtcSetTimeTm(&RTCD1, timp);
#endif /* EFI_RTC */
}

void date_get_tm(struct tm *timp) {
	(void)timp;
#if EFI_RTC || defined(__DOXYGEN__)
	rtcGetTimeTm(&RTCD1, timp);
#endif /* EFI_RTC */
}

void dateToString(char *lcd_str) {
#if EFI_RTC || defined(__DOXYGEN__)
	// todo:
	// re-implement this along the lines of 	chvprintf("%04u-%02u-%02u %02u:%02u:%02u\r\n", timp.tm_year + 1900, timp.tm_mon + 1, timp.tm_mday, timp.tm_hour,
	// timp.tm_min, timp.tm_sec);
	// this would require a temporary mem stream - see datalogging and other existing usages

	strcpy(lcd_str, "00/00 00:00:00\0");
	static char buff[4];
	struct tm timp;
	rtcGetTimeTm(&RTCD1, &timp);			// get RTC date/time
	
	itoa10(buff, timp.tm_mon + 1);
	if(timp.tm_mon < 9) { 
		lcd_str[0] = '0';
		lcd_str[1] = buff[0];
	} else {
		lcd_str[0] = buff[0];
		lcd_str[1] = buff[1];
	}
	itoa10(buff, timp.tm_mday);
	if(timp.tm_mday < 10) { 
		lcd_str[3] = '0';
		lcd_str[4] = buff[0];
	} else {
		lcd_str[3] = buff[0];
		lcd_str[4] = buff[1];
	}
	itoa10(buff, timp.tm_hour);
	if(timp.tm_hour < 10) { 
		lcd_str[6] = '0';
		lcd_str[7] = buff[0];
	} else {
		lcd_str[6] = buff[0];
		lcd_str[7] = buff[1];
	}
	itoa10(buff, timp.tm_min);
	if(timp.tm_min < 10) { 
		lcd_str[9] = '0';
		lcd_str[10] = buff[0];
	} else {
		lcd_str[9] = buff[0];
		lcd_str[10] = buff[1];
	}
	itoa10(buff, timp.tm_sec);
	if(timp.tm_sec < 10) { 
		lcd_str[12] = '0';
		lcd_str[13] = buff[0];
	} else {
		lcd_str[12] = buff[0];
		lcd_str[13] = buff[1];
	}
#else
	lcd_str[0] = 0;
#endif /* EFI_RTC */
}

#if EFI_RTC || defined(__DOXYGEN__)
void printDateTime(void) {
	static time_t unix_time;
	struct tm timp;
	
	unix_time = rtcGetTimeUnixSec(&RTCD1);
	if (unix_time == -1) {
		scheduleMsg(&logger, "incorrect time in RTC cell");
	} else {
		scheduleMsg(&logger, "%D - unix time", unix_time);
		rtcGetTimeTm(&RTCD1, &timp);

		appendMsgPrefix(&logger);
		appendPrintf(&logger, "Current RTC time in GMT is: %04u-%02u-%02u %02u:%02u:%02u", timp.tm_year + 1900, timp.tm_mon + 1, timp.tm_mday, timp.tm_hour,
				timp.tm_min, timp.tm_sec);
		appendMsgPostfix(&logger);
		scheduleLogging(&logger);
	}
}

void setDateTime(const char *strDate) {
	if (strlen(strDate) > 0) {
		time_t unix_time = (double) atoff(strDate);
		if (unix_time > 0) {
			rtcSetTimeUnixSec(&RTCD1, unix_time);
			printDateTime();
			return;
		}
	}
	scheduleMsg(&logger, "date_set Date parameter %s is wrong\r\n", strDate);
}
#endif /* EFI_RTC */

void initRtc(void) {
#if EFI_RTC || defined(__DOXYGEN__)
	printMsg(&logger, "initRtc()");

	// yes, it's my begin time  and we always start from this one 1391894433 - 2014-02-08 21:20:03
//	rtcSetTimeUnixSec(&RTCD1, 1391894433);
#endif
}
