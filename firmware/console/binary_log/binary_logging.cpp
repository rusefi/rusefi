/**
 * binary_logging.cpp
 *
 * See also BinarySensorLog.java
 * See also mlq_file_format.txt
 *
 * https://www.efianalytics.com/TunerStudio/docs/MLG_Binary_LogFormat_1.0.pdf
 * https://www.efianalytics.com/TunerStudio/docs/MLG_Binary_LogFormat_2.0.pdf
 */

#include "pch.h"

#include "binary_logging.h"
#include "log_field.h"
#include "buffered_writer.h"
#include "tunerstudio.h"

#if EFI_FILE_LOGGING || EFI_UNIT_TEST

#define TIME_PRECISION 1000

// floating number of seconds with millisecond precision
static scaled_channel<uint32_t, TIME_PRECISION> packedTime;

// The list of logged fields lives in a separate file so it can eventually be tool-generated
// We use angle brackets instead of quotes because for some boards we want to use header different from the one in this
// directory
#include <log_fields_generated.h>

template <typename T, size_t N>
consteval size_t getFieldsSize(const T(&)[N]) {
	return N;
}

int getSdCardFieldsCount() {
	return getFieldsSize(fields);
}

static consteval uint16_t computeFieldsRecordLength() {
	uint16_t recLength = 0;
	for (size_t i = 0; i < getFieldsSize(fields); i++) {
		recLength += fields[i].getSize();
	}

	return recLength;
}

static uint64_t binaryLogCount = 0;

static const uint16_t recordLength = computeFieldsRecordLength();

static size_t writeFileHeader(Writer& outBuffer) {
	size_t writen = 0;
	char buffer[MLQ_HEADER_SIZE];
	// File format: MLVLG\0
	strncpy(buffer, "MLVLG", 6);

	// Format version = 02
	buffer[6] = 0;
	buffer[7] = 2;

	// Timestamp
	buffer[8] = 0;
	buffer[9] = 0;
	buffer[10] = 0;
	buffer[11] = 0;

	// Info data start
	buffer[12] = 0;
	buffer[13] = 0;
	buffer[14] = 0;
	buffer[15] = 0;

	size_t headerSize = MLQ_HEADER_SIZE + efi::size(fields) * MLQ_FIELD_HEADER_SIZE;

	// Data begin index: begins immediately after the header
	buffer[16] = (headerSize >> 24) & 0xFF;
	buffer[17] = (headerSize >> 16) & 0xFF;
	buffer[18] = (headerSize >> 8) & 0xFF;
	buffer[19] = headerSize & 0xFF;

	// Record length - length of a single data record: sum size of all fields
	buffer[20] = recordLength >> 8;
	buffer[21] = recordLength & 0xFF;

	// Number of logger fields
	int fieldsCount = efi::size(fields);
	buffer[22] = fieldsCount >> 8;
	buffer[23] = fieldsCount;

	outBuffer.write(buffer, MLQ_HEADER_SIZE);
	writen += MLQ_HEADER_SIZE;

	// Write the actual logger fields, offset 22
	for (size_t i = 0; i < efi::size(fields); i++) {
		writen += fields[i].writeHeader(outBuffer);
	}

	return writen;
}

static uint8_t blockRollCounter = 0;

static size_t writeSdBlock(Writer& outBuffer) {
	size_t writen = 0;
	static char buffer[16];

	// Offset 0 = Block type, standard data block in this case
	buffer[0] = 0;

	// Offset 1 = rolling counter sequence number
	buffer[1] = blockRollCounter++;

	// Offset 2, size 2 = Timestamp at 10us resolution
	efitimeus_t nowUs = getTimeNowUs();
	uint16_t timestamp = nowUs / 10;
	buffer[2] = timestamp >> 8;
	buffer[3] = timestamp & 0xFF;

	// TODO: check ret value!
	outBuffer.write(buffer, 4);
	writen += 4;

	// todo: add a log field for SD card period
//	prevSdCardLineTime = nowUs;

	packedTime = getTimeNowMs() * 1.0 / TIME_PRECISION;

	uint8_t sum = 0;
	for (size_t fieldIndex = 0; fieldIndex < efi::size(fields); fieldIndex++) {
		#if EFI_UNIT_TEST
			// dark magic: most elements of log_fields_generated.h were const-evaluated against 'nullptr' engine, let's add it!
			void *offset = fields[fieldIndex].needsEngineOffsetHack(sizeof(*engine)) ? engine : nullptr;
		#else
			void *offset = nullptr;
		#endif

		size_t entrySize = fields[fieldIndex].writeData(buffer, offset);

		for (size_t byteIndex = 0; byteIndex < entrySize; byteIndex++) {
			// "CRC" at the end is just the sum of all bytes
			sum += buffer[byteIndex];
		}
		// TODO: check ret value!
		outBuffer.write(buffer, entrySize);
		writen += entrySize;
	}

	buffer[0] = sum;
	// 1 byte checksum footer
	outBuffer.write(buffer, 1);
	writen += 1;

	return writen;
}

size_t writeSdLogLine(Writer& bufferedWriter) {
#if EFI_PROD_CODE
extern bool main_loop_started;
	if (!main_loop_started)
		return 0;
#endif //EFI_PROD_CODE

	if (binaryLogCount == 0) {
		binaryLogCount++;

		return writeFileHeader(bufferedWriter);
	} else {
		binaryLogCount++;

		updateTunerStudioState();
		return writeSdBlock(bufferedWriter);
	}
}

void resetFileLogging() {
	binaryLogCount = 0;
	blockRollCounter = 0;
}

#endif /* EFI_FILE_LOGGING */
