/**
 * @file rtc_helper.c
 * @brief Real Time Clock helper
 *
 * @date Feb 5, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include <string.h> 
#include <time.h>
#include "main.h"
#include "rfiutil.h"
#include "chrtclib.h"
#include "rtc_helper.h"

static void date_help(void) {
	print("Usage: date_help\r\n");
	print("       date_get\r\n");
	print("       date_set N\r\n");
	print("where N is time in seconds sins Unix epoch\r\n");
	print(
			"		Human readable time   \tSeconds\r\n \
		1 hour	\t\t3600 seconds\r\n \
		1 day	\t\t86400 seconds\r\n \
		1 week	\t\t604800 seconds\r\n \
		1 month (30.44 days)\t2629743 seconds\r\n \
		1 year (365.24 days)\t31556926 seconds\r\n \
		or convert via http://www.epochconverter.com/\r\n");
}

static void date_set(char *strDate) {
	static time_t unix_time;
	if (strlen(strDate) > 0) {
		unix_time = (double) atoff(strDate);
		if (unix_time > 0) {
			rtcSetTimeUnixSec(&RTCD1, unix_time);
			return;
		}
	}
	print("date_set Date parameter %s is wrong\r\n", strDate);
}

void date_set_tm(struct tm *timp) {
	rtcSetTimeTm(&RTCD1, timp);
}

void date_get_tm(struct tm *timp) {
	rtcGetTimeTm(&RTCD1, timp);
}

void dateToString(char *lcd_str) {
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
}

static void date_get(void) {
	static time_t unix_time;
	struct tm timp;
	
	unix_time = rtcGetTimeUnixSec(&RTCD1);
	if (unix_time == -1) {
		print("incorrect time in RTC cell\r\n");
	} else {
		print("%D%s", unix_time, " - unix time\r\n");
		rtcGetTimeTm(&RTCD1, &timp);
		print("Current RTC time in GMT is: ");
		print("%04u-%02u-%02u %02u:%02u:%02u\r\n", timp.tm_year + 1900, timp.tm_mon + 1, timp.tm_mday, timp.tm_hour,
				timp.tm_min, timp.tm_sec);
		
	}
}

void initRtc(void) {
	// yes, it's my begin time  and we always start from this one 1391894433 - 2014-02-08 21:20:03
	rtcSetTimeUnixSec(&RTCD1, 1391894433);
	addConsoleAction("date_get", &date_get);
	addConsoleActionS("date_set", &date_set);
	addConsoleAction("date_help", &date_help);
}
