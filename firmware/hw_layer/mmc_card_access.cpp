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
	const uint16_t* data16 = reinterpret_cast<uint16_t*>(input);

	if (input[0] == 0 && input[1] == 1) {
		scheduleMsg(&sharedLogger, "TS_SD: RTC read command");
		memset(buffer, 0, 9);

	} else if (input[0] == 0 && input[1] == 0x11) {
		int length = data16[2];
		scheduleMsg(&sharedLogger, "TS_SD: fetch buffer command, length=%d", length);


		if (length == 16) {

        } else {
            // SD read directory command

		}


	} else if (input[0] == 0 && input[1] == 0x14) {
		scheduleMsg(&sharedLogger, "TS_SD: fetch data command blockNumber=");

	} else {
		scheduleMsg(&sharedLogger, "TS_SD: unexpected r");
	}
}

void handleTsW(char *input) {
	const uint16_t* data16 = reinterpret_cast<uint16_t*>(input);

    if (input[0] == 0 && input[1] == 0x11) {
		int code = data16[2];
		scheduleMsg(&sharedLogger, "TS_SD: w, code=%d", code);


    } else {
		scheduleMsg(&sharedLogger, "TS_SD: unexpected w");
    }
}


#endif // EFI_FILE_LOGGING
