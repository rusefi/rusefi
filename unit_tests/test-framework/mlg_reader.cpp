/*
 * mlg_reader.cpp
 *
 */

#include <iostream>
#include <fstream>
#include <cstdint> // For fixed-width integer types like int32_t
#include <stdexcept> // For std::runtime_error
#include <chrono>    // For time-related operations
#include <ctime>     // For std::put_time and std::localtime
#include "mlg_reader.h"
#define FIXED_HEADER_SIZE 24

int readSwappedInt(std::ifstream *ifs) {
	int32_t value;
	if (!ifs->read(reinterpret_cast<char*>(&value), sizeof(value))) {
		throw std::runtime_error("Error reading value");
	}
	uint32_t swapped_value = ((value & 0x000000FF) << 24)
			| ((value & 0x0000FF00) << 8) | ((value & 0x00FF0000) >> 8)
			| ((value & 0xFF000000) >> 24);
	return static_cast<int32_t>(swapped_value);
}

float readSwappedFloat(std::ifstream *ifs) {
	int value;
	if (!ifs->read(reinterpret_cast<char*>(&value), sizeof(value))) {
		throw std::runtime_error("Error reading value");
	}
	union {
		float f;
		uint32_t i;
	} converter;
	uint32_t swapped_bytes = ((value & 0x000000FF) << 24)
			| ((value & 0x0000FF00) << 8) | ((value & 0x00FF0000) >> 8)
			| ((value & 0xFF000000) >> 24);

	converter.i = swapped_bytes;
	return converter.f;
}

int16_t readSwappedShort(std::ifstream *ifs) {
	int16_t value;
	if (!ifs->read(reinterpret_cast<char*>(&value), sizeof(value))) {
		throw std::runtime_error("Error reading value");
	}
	uint16_t swapped_value = ((value & 0x00FF) << 8) | ((value & 0xFF00) >> 8);
	return static_cast<int16_t>(swapped_value);
}

int8_t readByte(std::ifstream *ifs) {
	int8_t value;
	if (!ifs->read(reinterpret_cast<char*>(&value), sizeof(value))) {
		throw std::runtime_error("Error reading value");
	}
	return value;
}

static std::string readFixedSizeString(std::ifstream &ifs, int size) {
	std::string s;
	s.reserve(size); // Pre-allocate memory
	bool terminated = false;
	for (int i = 0; i < size; ++i) {
		char c;
		if (!ifs.read(&c, 1)) {
			throw std::runtime_error(
					"Error reading fixed size string byte "
							+ std::to_string(i));
		}
		if (c == 0) {
			terminated = true;
		}
		if (!terminated) {
			s += c;
		}
	}
	return s;
}

int BinarySensorReader::readRecordsMetadata(std::ifstream &ifs,
		int numberOfFields) {
	int lineTotalSize = 0;
	for (int i = 0; i < numberOfFields; ++i) {
		int8_t typeCode = readByte(&ifs);
		std::string fieldName = readFixedSizeString(ifs, 34);
		std::string units = readFixedSizeString(ifs, 10);

//		std::cout << "fieldName [" << fieldName << "]" << std::endl;
//		std::cout << "units [" << units << "]" << std::endl;

		/*int8_t style = */readByte(&ifs);
		float scale = readSwappedFloat(&ifs);
		float transform = readSwappedFloat(&ifs);
		int8_t digits = readByte(&ifs);
		/*category*/readFixedSizeString(ifs, 34);

		MlgDataType type = findByOrdinal(typeCode);
		lineTotalSize += getRecordSize(type);

		Record *record = new Record(fieldName, type, scale);
		recordByName[fieldName] = record;
		records.emplace_back(record);
	}

	if (afterHeaderCallback) {
		afterHeaderCallback();
	}

	return lineTotalSize;
}

void BinarySensorReader::readLoggerFieldData() {
	/*uint16_t timestamp =*/static_cast<uint16_t>(readSwappedShort(&ifs));

//    std::cout << "Reading for record " << recordCounter << std::endl;

	for (Record *record : records) {
		float value = record->read(ifs);
		currentSnapshot[record->getFieldName()] = value;
	}

	/*uint8_t crc = */static_cast<uint8_t>(readByte(&ifs)); // Use the new helper

	recordCounter++;
	//logContent.emplace_back(currentSnapshot);
}

std::map<const std::string, float>& BinarySensorReader::readBlock() {
	uint8_t blockType = static_cast<uint8_t>(readByte(&ifs)); // Use the new helper
	uint8_t counter = static_cast<uint8_t>(readByte(&ifs)); // Use the new helper

	if (blockType == 0) {
		readLoggerFieldData();
	} else if (blockType == 1) {
		throw std::runtime_error("todo support markers");
	} else {
		throw std::runtime_error(
				"Unexpected block type " + std::to_string(blockType));
	}

	return currentSnapshot;
}

void BinarySensorReader::openMlg(const std::string fileName) {
	ifs.open(fileName, std::ios::binary);

	if (!ifs.is_open()) {
		throw std::runtime_error("Error opening file: " + fileName);
	}

	int header = readSwappedInt(&ifs);

	std::cout << "header " << std::hex << header << std::dec << std::endl;
	if (header != 0x4d4c564c) {
		throw std::runtime_error("header " + std::to_string(header));
	}

	int32_t version = readSwappedInt(&ifs);
	std::cout << "version " << std::hex << version << std::dec << std::endl;
	if (version != 0x47000002) {
		throw std::runtime_error("version " + std::to_string(version));
	}

	int32_t timeStamp = readSwappedInt(&ifs);
	std::cout << "timeStamp " << timeStamp;

	{
		std::time_t ts_seconds = static_cast<std::time_t>(timeStamp);
		std::tm *ptm = std::localtime(&ts_seconds);
		char buffer[32];
		std::strftime(buffer, 32, "%Y-%m-%d %H:%M:%S", ptm);
		std::cout << " " << buffer << std::endl;
	}

	int32_t infoDataState = readSwappedInt(&ifs);
	std::cout << "infoDataState " << std::hex << infoDataState << "/"
			<< std::dec << infoDataState << std::endl;

	int32_t dataBeginIndex = readSwappedInt(&ifs);
	std::cout << "dataBeginIndex " << std::hex << dataBeginIndex << "/"
			<< std::dec << dataBeginIndex << std::endl;

	uint16_t recordLength = readSwappedShort(&ifs);
	uint16_t numberOfFields = readSwappedShort(&ifs);
	std::cout << "numberOfFields=" << numberOfFields << std::endl;

	int fieldsHeaderAreaSize = 89 * numberOfFields;
	std::cout << "fields area size " << fieldsHeaderAreaSize
			<< ", recordLength=" << recordLength << std::endl;

	int infoBlockExpectedSize = dataBeginIndex - FIXED_HEADER_SIZE
			- fieldsHeaderAreaSize;
	bool isInfoBlockExpected = infoBlockExpectedSize > 0;
	if (isInfoBlockExpected) {
		std::cout << "Expecting infoBlock " << infoBlockExpectedSize
				<< std::endl;
	}

	int lineTotalSize = readRecordsMetadata(ifs, numberOfFields);

	if (isInfoBlockExpected) {

		std::string infoBlock = readFixedSizeString(ifs, infoBlockExpectedSize);
		std::cout << "Skipping infoBlock length=" << infoBlock.length()
				<< std::endl;
		int sizeValidation = dataBeginIndex - infoBlock.length()
				- fieldsHeaderAreaSize - FIXED_HEADER_SIZE - 1;

		if (sizeValidation != 0)
			throw std::runtime_error(
					"Size validation failed by "
							+ std::to_string(sizeValidation));
	}
}

void BinarySensorReader::readMlg(mlg_logline_callback_t callback) {
	while (!eof()) {
		callback(readBlock());
	}

	std::cout << "Got " << recordCounter << " record(s)" << std::endl;

	ifs.close();
}

bool BinarySensorReader::eof() {
	return (ifs.peek() == EOF);
}
