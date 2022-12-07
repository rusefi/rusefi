/**
 * @file rtc_helper.h
 * @brief Real Time Clock helper
 *
 * @date Feb 5, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 * @author Nathan Schulte, (c) 2022
 */

#pragma once

#include "rusefi_types.h"

#if EFI_RTC
void initRtc();
void printRtcDateTime();
efidatetime_t getRtcDateTime();
void setRtcDateTime(const efidatetime_t * const dateTime);

efidatetime_t convertRtcDateTimeToEfi(const RTCDateTime * const timespec);
RTCDateTime convertRtcDateTimeFromEfi(const efidatetime_t * const dateTime);
#endif // EFI_RTC

void dateToString(char *buffer);
bool dateToStringShort(char *lcd_str);
