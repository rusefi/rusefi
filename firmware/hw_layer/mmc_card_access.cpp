/**
 * @file	mmc_card_access.cpp
 *
 */

#include "global.h"

#if EFI_FILE_LOGGING
#include "mmc_card.h"

extern LoggingWithStorage sharedLogger;

static char *buffer[2048];


void handleTsR(char *input) {
	if (input[1] == 0 && input[2] == 7) {
		scheduleMsg(&sharedLogger, "TS_SD: RTC read command");
		memset(buffer, 0, 9);
	}

}

void handleTsW(char *input) {

}


#endif // EFI_FILE_LOGGING
