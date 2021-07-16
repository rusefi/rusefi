/**
 * @file	mmc_card_access.cpp
 *
 *
 * Here we have code related to file transfer from rusEFI ECU to desktop
 * We are mostly compatible with TS MS3 transder protocol as described at Megasquirt_Serial_Protocol-2014-10-28.pdf
 * rusEFI simulator reads local files.
 *
 */

#include "engine.h"

#if EFI_SIMULATOR
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif

#if EFI_FILE_LOGGING
#include "ff.h"
#define ROOT_DIR "/"
#endif // EFI_FILE_LOGGING

#define DIR_RESPONSE_SIZE 512
#define DIR_ENTRY_SIZE 32

#define DIR_RESPONSE_BUFFER_SIZE (DIR_RESPONSE_SIZE + 2)

#define DOT '.'

#if EFI_FILE_LOGGING || EFI_SIMULATOR || EFI_UNIT_TEST

#include "mmc_card.h"
#include "efilib.h"
#include "hardware.h"

/**
 * for funny reasons file name has to be at least 4 symbols before the dot
 */
bool isLogFile(const char *fileName) {
	int dotIndex = indexOf(fileName, DOT);
	if (dotIndex == -1) {
		return false;
	}
	if (dotIndex < 4) {
		return false;
	}
	return 0 == strncmp(fileName + dotIndex, DOT_MLG, 4);
}
#endif // EFI_FILE_LOGGING || EFI_SIMULATOR || EFI_UNIT_TEST

// Enable when logging is enabled, but not USB mass storage
#if (EFI_FILE_LOGGING && !HAL_USE_USB_MSD) || EFI_SIMULATOR
#include "mmc_card.h"

#if EFI_SIMULATOR
static FILE *uploading;
#endif // EFI_SIMULATOR

#if EFI_FILE_LOGGING
static FIL uploading NO_CACHE;
#endif // EFI_FILE_LOGGING

#define TRANSFER_SIZE 2048

static uint8_t buffer[TRANSFER_SIZE + 2];

static void setFileEntry(uint8_t *buffer, int index, const char *fileName,
		int fileSize) {
	int offset = 32 * index;

	int dotIndex = indexOf(fileName, DOT);
	// assert dotIndex != -1
	memcpy(buffer + offset, fileName, dotIndex);
	for (int i = dotIndex; i < 8; i++) {
		buffer[offset + i] = 0;
	}
	memcpy(buffer + offset + 8, &fileName[dotIndex + 1], 3);

	buffer[offset + 11] = 1;

	for (int i = 0; i < 4; i++) {
		buffer[offset + 18 + i] = fileName[dotIndex + i - 4];
	}

	*(uint32_t*) (&buffer[offset + 28]) = fileSize;
}

void handleTsR(TsChannelBase* tsChannel, char *input) {
#if EFI_SIMULATOR
	printf("TS_SD r %d\n", input[1]);
#endif // EFI_SIMULATOR

	const uint16_t *data16 = reinterpret_cast<uint16_t*>(input);

	if (input[0] == 0 && input[1] == TS_SD_PROTOCOL_RTC) {
		efiPrintf("TS_SD: RTC read command");
		memset(buffer, 0, 9);
		tsChannel->sendResponse(TS_CRC, buffer, 9);

	} else if (input[0] == 0 && input[1] == TS_SD_PROTOCOL_FETCH_INFO) {
		uint16_t length = SWAP_UINT16(data16[2]);
		efiPrintf("TS_SD: fetch buffer command, length=%d",
				length);

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

			tsChannel->sendResponse(TS_CRC, buffer, 16);
		} else if (length == DIR_RESPONSE_BUFFER_SIZE) {
			// SD read directory command
			memset(buffer, 0, DIR_RESPONSE_BUFFER_SIZE);

#if EFI_SIMULATOR
			DIR *dr = opendir(".");
			if (dr == NULL) {
				// opendir returns NULL if couldn't open directory
				printf("Could not open current directory" );
				return;
			}

			int index = 0;
			struct dirent *de;  // Pointer for directory entry
			while ((de = readdir(dr)) != NULL) {
				const char * fileName = de->d_name;
				printf("%s\n", fileName);
				if (index >= DIR_RESPONSE_SIZE / DIR_ENTRY_SIZE) {
					break;
				}
				if (isLogFile(fileName)) {
					struct stat statBuffer;
					int status;

					int fileSize = 0;
					status = stat(fileName, &statBuffer);
					if (status == 0) {
						fileSize = statBuffer.st_size;
					}
					setFileEntry(buffer, index, fileName, fileSize);
					index++;
				}
			}
			closedir(dr);

#endif // EFI_SIMULATOR

#if EFI_FILE_LOGGING
			DIR dir;
			FRESULT res = f_opendir(&dir, ROOT_DIR);
			if (res != FR_OK) {
				efiPrintf("Error opening directory");
			} else {
				int index = 0;
				while (true) {
					if (index >= DIR_RESPONSE_SIZE / DIR_ENTRY_SIZE) {
						break;
					}
					FILINFO fno;
					res = f_readdir(&dir, &fno);
					char *fileName = fno.fname;
					if (res != FR_OK || fileName[0] == 0) {
						break;
					}

					if (isLogFile(fileName)) {
//      				        	    	struct stat statBuffer;
//      				        	    	int         status;
//
//      				        	    	int fileSize = 0;
//      				        	    	status = stat(fileName, &statBuffer);
//      				        	    	if (status == 0) {
//      				        	    	   fileSize = statBuffer.st_size;
//      				        	    	}

						FILINFO fileInfo;
						// todo: handle return value?
						f_stat(fileName, &fileInfo);

						setFileEntry(buffer, index, fileName,
								(int) fileInfo.fsize);
						index++;
					}

				}
			}

#endif // EFI_FILE_LOGGING

			tsChannel->sendResponse(TS_CRC, buffer,
					DIR_RESPONSE_BUFFER_SIZE);
		}
	} else if (input[0] == 0 && input[1] == TS_SD_PROTOCOL_FETCH_DATA) {
		uint16_t blockNumber = SWAP_UINT16(data16[1]);
		efiPrintf("TS_SD: fetch data command blockNumber=%d",
				blockNumber);

//		int offset = blockNumber * TRANSFER_SIZE;

		buffer[0] = input[2];
		buffer[1] = input[3];

		int got;
#if EFI_SIMULATOR
		got = fread(&buffer[2], 1, TRANSFER_SIZE, uploading);
#endif // EFI_SIMULATOR

#if EFI_FILE_LOGGING
		got = 0;
		f_read(&uploading, (void*) &buffer[2], TRANSFER_SIZE, (UINT*) &got);
#endif // EFI_FILE_LOGGING

		tsChannel->sendResponse(TS_CRC, buffer, 2 + got);
	} else {
		efiPrintf("TS_SD: unexpected r");
	}
}

void handleTsW(TsChannelBase* tsChannel, char *input) {
	const uint16_t *data16 = reinterpret_cast<uint16_t*>(input);

#if EFI_SIMULATOR
	printf("TS_SD w %d\n", input[1]);
#endif // EFI_SIMULATOR

	if (input[0] == 0 && input[1] == TS_SD_PROTOCOL_FETCH_INFO) {
		int code = data16[2];
		efiPrintf("TS_SD: w, code=%d", code);

		if (input[5] == TS_SD_PROTOCOL_DO) {
			efiPrintf("TS_SD_PROTOCOL_DO");
			sendOkResponse(tsChannel, TS_CRC);
		} else if (input[5] == TS_SD_PROTOCOL_READ_DIR) {
			efiPrintf("TS_SD_PROTOCOL_READ_DIR");
			sendOkResponse(tsChannel, TS_CRC);
		} else if (input[5] == TS_SD_PROTOCOL_REMOVE_FILE) {
#if EFI_SIMULATOR
        	DIR *dr = opendir(".");
        	if (dr == NULL) {
        		// opendir returns NULL if couldn't open directory
        	    printf("Could not open current directory" );
        	    return;
        	}
        	struct dirent *de;  // Pointer for directory entry
        	while ((de = readdir(dr)) != NULL) {
        		const char * fileName = de->d_name;
        	    printf("%s\n", fileName);
        	    if (isLogFile(fileName)) {
        	    	int dotIndex = indexOf(fileName, DOT);
        	    	if (0 == strncmp(input + 6, &fileName[dotIndex - 4], 4)) {
                	    printf("Removing %s\n", fileName);
                	    remove(fileName);
                	    break;
        	    	}
        	    }
        	}
        	closedir(dr);
#endif // EFI_SIMULATOR

#if EFI_FILE_LOGGING
			DIR dir;
			FRESULT res = f_opendir(&dir, ROOT_DIR);
			if (res != FR_OK) {
				efiPrintf("Error opening directory");
			} else {
				while (true) {
					FILINFO fno;
					res = f_readdir(&dir, &fno);
					char *fileName = fno.fname;
					if (res != FR_OK || fileName[0] == 0) {
						break;
					}
					if (isLogFile(fileName)) {
						int dotIndex = indexOf(fileName, DOT);
						if (0 == strncmp(input + 6, &fileName[dotIndex - 4], 4)) {
							efiPrintf("Removing %s", fileName);
							f_unlink(fileName);
							break;
						}
					}
				}
			}
#endif // EFI_FILE_LOGGING

			sendOkResponse(tsChannel, TS_CRC);

		} else if (input[5] == TS_SD_PROTOCOL_FETCH_COMPRESSED) {
#if EFI_SIMULATOR
        	DIR *dr = opendir(".");

        	if (dr == NULL) {
        		// opendir returns NULL if couldn't open directory
        	    printf("Could not open current directory" );
        	    return;
        	}

        	struct dirent *de;  // Pointer for directory entry
        	while ((de = readdir(dr)) != NULL) {
        		const char * fileName = de->d_name;
        	    printf("%s\n", fileName);
        	    if (isLogFile(fileName)) {
        	    	int dotIndex = indexOf(fileName, DOT);
        	    	if (0 == strncmp(input + 6, &fileName[dotIndex - 4], 4)) {
                	    printf("Will be uploading %s\n", fileName);
                	    uploading = fopen(fileName, "rb");
                	    if (uploading == NULL) {
                	          printf("Error opening %s\n", fileName);
                	          exit(1);
                	    }
                	    break;
        	    	}
        	    }
        	}
        	closedir(dr);
#endif // EFI_SIMULATOR

#if EFI_FILE_LOGGING
			DIR dir;
			FRESULT res = f_opendir(&dir, ROOT_DIR);

			if (res != FR_OK) {
				efiPrintf("Error opening directory");
			} else {
				memset(&uploading, 0, sizeof(FIL));			// clear the memory
				while (true) {
					FILINFO fno;
					res = f_readdir(&dir, &fno);
					char *fileName = fno.fname;
					if (res != FR_OK || fileName[0] == 0) {
						break;
					}
					if (isLogFile(fileName)) {
						int dotIndex = indexOf(fileName, DOT);
						if (0 == strncmp(input + 6, &fileName[dotIndex - 4], 4)) {
							/* FRESULT err = */
							f_open(&uploading, fileName, FA_READ);// This file has the index for next log file name
							break;
						}
					}
				}
			}

#endif // EFI_FILE_LOGGING

			sendOkResponse(tsChannel, TS_CRC);
		}

	} else {
		efiPrintf("TS_SD: unexpected w");
	}
}

#endif // (EFI_FILE_LOGGING && !HAL_USE_USB_MSD)
