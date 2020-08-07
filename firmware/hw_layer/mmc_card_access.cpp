/**
 * @file	mmc_card_access.cpp
 *
 */

#include "global.h"

#if EFI_FILE_LOGGING || EFI_SIMULATOR || EFI_UNIT_TEST

#include "mmc_card.h"
#include "efilib.h"

/**
 * for funny reasons file name has to be at least 4 symbols before the dot
 */
bool isLogFile(const char *fileName) {
	int dotIndex = indexOf(fileName, '.');
	if (dotIndex == -1) {
		return false;
	}
	if (dotIndex < 4) {
		return false;
	}
	return 0 == strncmp(fileName + dotIndex, DOT_MLG, 4);
}

#endif


#if EFI_FILE_LOGGING || EFI_SIMULATOR
#include "mmc_card.h"

extern LoggingWithStorage sharedLogger;

#define TRANSFER_SIZE 2048

static uint8_t buffer[TRANSFER_SIZE + 2];

static void setFileEntry(uint8_t *buffer, int index, const char *fileName, int fileSize) {
	int offset = 32 * index;
	memcpy(buffer + offset, fileName, 11);
	buffer[offset + 11] = 1;

	*(uint32_t *) (&buffer[offset + 28]) = fileSize;
}

void handleTsR(ts_channel_s *tsChannel, char *input) {
	const uint16_t* data16 = reinterpret_cast<uint16_t*>(input);

	if (input[0] == 0 && input[1] == TS_SD_PROTOCOL_RTC) {
		scheduleMsg(&sharedLogger, "TS_SD: RTC read command");
		memset(buffer, 0, 9);
		sr5SendResponse(tsChannel, TS_CRC, buffer, 9);

	} else if (input[0] == 0 && input[1] == TS_SD_PROTOCOL_FETCH_INFO) {
		int length = data16[2];
		scheduleMsg(&sharedLogger, "TS_SD: fetch buffer command, length=%d", length);


		if (length == 16) {
			buffer[0] = 1 + 4; // Card present + Ready
			buffer[1] = 0; // Y - error code

			buffer[2] = 2; // higher byte of '512' sector size
			buffer[3] = 0; // lower byte

			buffer[4] = 0;
			buffer[5] = 0x20; // 0x20 00 00 of 512 is 1G virtual card
            buffer[6] = 0;
            buffer[7] = 0;

            buffer[8] = 0;
            buffer[9] = 1; // number of files

			sr5SendResponse(tsChannel, TS_CRC, buffer, 16);
        } else if (length == 0x202) {
            // SD read directory command
        	memset(buffer, 0, 512);


#if EFI_SIMULATOR

        	setFileEntry(buffer, 0, "hello123.msq", 123123);
        	setFileEntry(buffer, 1, "hello222.msq", 123123);

#endif // EFI_SIMULATOR
        	sr5SendResponse(tsChannel, TS_CRC, buffer, 0x202);
		}
	} else if (input[0] == 0 && input[1] == TS_SD_PROTOCOL_FETCH_DATA) {
		int blockNumber = data16[1];
		scheduleMsg(&sharedLogger, "TS_SD: fetch data command blockNumber=%d", blockNumber);

		int offset = blockNumber * TRANSFER_SIZE;

	} else {
		scheduleMsg(&sharedLogger, "TS_SD: unexpected r");
	}
}

void handleTsW(ts_channel_s *tsChannel, char *input) {
	const uint16_t* data16 = reinterpret_cast<uint16_t*>(input);

    if (input[0] == 0 && input[1] == TS_SD_PROTOCOL_FETCH_INFO) {
		int code = data16[2];
		scheduleMsg(&sharedLogger, "TS_SD: w, code=%d", code);


		if (input[5] == TS_SD_PROTOCOL_DO) {
			sendOkResponse(tsChannel, TS_CRC);
		} else if (input[5] == TS_SD_PROTOCOL_READ_DIR) {
			sendOkResponse(tsChannel, TS_CRC);
		} else if (input[5] == TS_SD_PROTOCOL_REMOVE_FILE) {
			// todo

		} else if (input[5] == TS_SD_PROTOCOL_FETCH_COMPRESSED) {
		}


    } else {
		scheduleMsg(&sharedLogger, "TS_SD: unexpected w");
    }
}


#endif // EFI_FILE_LOGGING
