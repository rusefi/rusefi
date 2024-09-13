/**
 * See also BinarySensorLog.java
 * See also mlq_file_format.txt
 */

#include "pch.h"

#include "binary_logging.h"
#include "log_field.h"
#include "buffered_writer.h"
#include "tunerstudio.h"

#if EFI_FILE_LOGGING

// 2^32 milliseconds is 49 days, this is plenty of time.
constexpr int TimestampCountsPerSec = 1000;
constexpr int TicksPerCount = (US_TO_NT_MULTIPLIER * 1000000) / TimestampCountsPerSec;

// Check that it's an integer number of ticks
static_assert(US_TO_NT_MULTIPLIER * 1000000 == TimestampCountsPerSec * TicksPerCount);

static scaled_channel<uint32_t, TimestampCountsPerSec> packedTime;

// The list of logged fields lives in a separate file so it can eventually be tool-generated
#include "log_fields_generated.h"

static constexpr uint16_t computeFieldsRecordLength() {
	uint16_t recLength = 0;
	for (size_t i = 0; i < efi::size(fields); i++) {
		recLength += fields[i].getSize();
	}

	return recLength;
}

static uint64_t binaryLogCount = 0;

extern bool main_loop_started;

void writeSdLogLine(Writer& bufferedWriter) {
	if (!main_loop_started)
		return;

	if (binaryLogCount == 0) {
		writeFileHeader(bufferedWriter);
	} else {
		updateTunerStudioState();
		writeSdBlock(bufferedWriter);
	}

	binaryLogCount++;
}

static constexpr uint16_t recordLength = computeFieldsRecordLength();

void writeFileHeader(Writer& outBuffer) {
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
	buffer[16] = 0;
	buffer[17] = 0;
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

	// Write the actual logger fields, offset 22
	for (size_t i = 0; i < efi::size(fields); i++) {
		fields[i].writeHeader(outBuffer);
	}
}

static uint8_t blockRollCounter = 0;

//static efitimeus_t prevSdCardLineTime = 0;

void writeSdBlock(Writer& outBuffer) {
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

	outBuffer.write(buffer, 4);

	// Sigh.
	*reinterpret_cast<uint32_t*>(&packedTime) = getTimeNowNt() / TicksPerCount;

	uint8_t sum = 0;
	for (size_t fieldIndex = 0; fieldIndex < efi::size(fields); fieldIndex++) {
		size_t entrySize = fields[fieldIndex].writeData(buffer);

		for (size_t byteIndex = 0; byteIndex < entrySize; byteIndex++) {
			// "CRC" at the end is just the sum of all bytes
			sum += buffer[byteIndex];
		}
		outBuffer.write(buffer, entrySize);
	}

	buffer[0] = sum;
	// 1 byte checksum footer
	outBuffer.write(buffer, 1);
}

#endif /* EFI_FILE_LOGGING */
