/**
 * @file rtc_helper.h
 * @brief Real Time Clock helper
 *
 * @date Feb 5, 2014
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef RTC_HELPER_H_
#define RTC_HELPER_H_

void printDateTime(void);
void setDateTime(const char *strDate);
void initRtc(void);
void date_set_tm(struct tm *);
void date_get_tm(struct tm *);
void dateToString(char *buffer);
bool dateToStringShort(char *lcd_str);

#endif /* RTC_HELPER_H_ */
