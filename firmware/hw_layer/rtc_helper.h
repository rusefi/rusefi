/**
 * @file rtc_helper.h
 * @brief Real Time Clock helper
 *
 * @date Feb 5, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

void printDateTime();
void setDateTime(const char *strDate);
void initRtc();
void date_set_tm(struct tm *);
void date_get_tm(struct tm *);
void dateToString(char *buffer);
bool dateToStringShort(char *lcd_str);
