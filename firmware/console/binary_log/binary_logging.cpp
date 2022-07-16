/**
 * See also BinarySensorLog.java
 * See also mlq_file_format.txt
 */

#include "pch.h"

#include "binary_logging.h"
#include "log_field.h"
#include "buffered_writer.h"

#define TIME_PRECISION 1000

// floating number of seconds with millisecond precision
static scaled_channel<uint32_t, TIME_PRECISION> packedTime;

// todo: we are at the edge of sdLogBuffer size and at the moment we have no code to make sure buffer does not overflow
// todo: make this logic smarter
// The list of logged fields lives in a separate file so it can eventually be tool-generated
#include "log_fields_generated.h"

static constexpr uint16_t computeFieldsRecordLength() {
	uint16_t recLength = 0;
	for (size_t i = 0; i < efi::size(fields); i++) {
		recLength += fields[i].getSize();
	}

	return recLength;
}

static constexpr uint16_t recordLength = computeFieldsRecordLength();

void writeHeader(Writer& outBuffer) {
	char buffer[MLQ_HEADER_SIZE];
	// File format: MLVLG\0
	strncpy(buffer, "MLVLG", 6);

	// Format version = 01
	buffer[6] = 0;
	buffer[7] = 1;

	// Timestamp
	buffer[8] = 0;
	buffer[9] = 0;
	buffer[10] = 0;
	buffer[11] = 0;

	// Info data start
	buffer[12] = 0;
	buffer[13] = 0;

	size_t headerSize = MLQ_HEADER_SIZE + efi::size(fields) * 55;

	// Data begin index: begins immediately after the header
	buffer[14] = 0;
	buffer[15] = 0;
	buffer[16] = (headerSize >> 8) & 0xFF;
	buffer[17] = headerSize & 0xFF;

	// Record length - length of a single data record: sum size of all fields
	buffer[18] = recordLength >> 8;
	buffer[19] = recordLength & 0xFF;

	// Number of logger fields
	buffer[20] = 0;
	buffer[21] = efi::size(fields);

	outBuffer.write(buffer, MLQ_HEADER_SIZE);

	// Write the actual logger fields, offset 22
	for (size_t i = 0; i < efi::size(fields); i++) {
		fields[i].writeHeader(outBuffer);
	}
}

static uint8_t blockRollCounter = 0;

size_t writeBlock(char* buffer) {
	// Offset 0 = Block type, standard data block in this case
	buffer[0] = 0;

	// Offset 1 = rolling counter sequence number
	buffer[1] = blockRollCounter++;

	// Offset 2, size 2 = Timestamp at 10us resolution
	uint16_t timestamp = getTimeNowUs() / 10;
	buffer[2] = timestamp >> 8;
	buffer[3] = timestamp & 0xFF;

	packedTime = currentTimeMillis() * 1.0 / TIME_PRECISION;

	// Offset 4 = field data
	const char* dataBlockStart = buffer + 4;
	char* dataBlock = buffer + 4;
	for (size_t i = 0; i < efi::size(fields); i++) {
		size_t entrySize = fields[i].writeData(dataBlock);

		// Increment pointer to next entry
		dataBlock += entrySize;
	}

	size_t dataBlockSize = dataBlock - dataBlockStart;

	// "CRC" at the end is just the sum of all bytes
	uint8_t sum = 0;
	for (size_t i = 0; i < dataBlockSize; i++) {
		sum += dataBlockStart[i];
	}
	*dataBlock = sum;

	// Total size has 4 byte header + 1 byte checksum
	return dataBlockSize + 5;
}
