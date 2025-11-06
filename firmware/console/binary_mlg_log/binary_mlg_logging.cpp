/**
 * binary_mlg_logging.cpp
 *
 * See also BinarySensorLog.java
 * See also mlq_file_format.txt
 *
 * https://www.efianalytics.com/TunerStudio/docs/MLG_Binary_LogFormat_1.0.pdf
 * https://www.efianalytics.com/TunerStudio/docs/MLG_Binary_LogFormat_2.0.pdf
 *
 * for opposite see mlg_reader.cpp
 */

#include "pch.h"

#include "binary_mlg_logging.h"
#include "mlg_field.h"
#include "tunerstudio.h"
#include "board_lookup.h"

#if EFI_FILE_LOGGING || EFI_UNIT_TEST

#define TIME_PRECISION 1000

#if EFI_PROD_CODE
extern bool main_loop_started;
#endif

// floating number of seconds with millisecond precision
static scaled_channel<uint32_t, TIME_PRECISION> packedTime;

// The list of logged fields lives in a separate file so it can eventually be tool-generated
// We use angle brackets instead of quotes because for some boards we want to use header different from the one in this
// directory
#include <log_fields_generated.h>

namespace MLG
{

int getSdCardFieldsCount() {
	return efi::size(fields);
}

static constexpr uint16_t computeFieldsRecordLength() {
	uint16_t recLength = 0;
	for (size_t i = 0; i < efi::size(fields); i++) {
		recLength += fields[i].getSize();
	}

	return recLength;
}

static uint64_t binaryLogCount = 0;

static const uint16_t recordLength = computeFieldsRecordLength();

static size_t writeFileHeader(Writer& outBuffer) {
	size_t writen = 0;
	char buffer[Types::Header::Size];
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

	size_t headerSize = Types::Header::Size + efi::size(fields) * Types::Field::DescriptorSize;

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

	outBuffer.write(buffer, Types::Header::Size);
	writen += Types::Header::Size;

	// Write the actual logger fields, offset by header size (24)
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
	// revSdCardLineTime = nowUs;

	packedTime = getTimeNowMs() * 1.0 / TIME_PRECISION;

	uint8_t sum = 0;
	for (size_t fieldIndex = 0; fieldIndex < efi::size(fields); fieldIndex++) {
		#if EFI_UNIT_TEST
			if (engine == nullptr) {
				throw std::runtime_error{"Engine pointer is nullptr in writeSdBlock"};
			}

			// For tests a global Engine pointer is initialised with the nullptr, and tests that do require it
			// create their own instance and set up the global pointer accordingly.
			// Global static const array of fields in the generated file log_fields_generated.h has fields with
			// addresses const-evaluated against 'nullptr' engine, which effectively means offsets in Engine struct,
			// so if that is the case, we need to account for the offset to whatever
			// real current engine pointer is set to.
			// In tests, we are dealing with ELF on linux, and as far as I'm aware, there are no distributions
			// where the default linker config can map smth before the 4 MB address.
			// If in doubt, check your system for a min text-segment with "ld --verbose | grep -A20 ENTRY"
			// Engine struct is lower than 4MB in size, so we can compare field address against Engine size
			// to find out whether field address was initialised against nullptr engine or not.

			constexpr auto engineObjectSize{ sizeof(Engine) };
			static_assert(engineObjectSize < 0x400000);

			auto const currentFieldAddress{ reinterpret_cast<uintptr_t>(fields[fieldIndex].getAddr()) };
			auto const fieldNeedsOffset{ currentFieldAddress < engineObjectSize };
			void* const offset{ fieldNeedsOffset ? engine : nullptr };
		#else
			void* const offset{ nullptr };
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

} /* namespace MLG */

#endif /* EFI_FILE_LOGGING */
