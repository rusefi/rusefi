
#include "tunerstudio_configuration.h"
#include "log_field.h"
#include "efilib.h"
#include "efitime.h"
#include "crc.h"

static const LogField fields[] = {
	{tsOutputChannels.rpm, "RPM", "rpm", 0},
	{tsOutputChannels.vehicleSpeedKph, GAUGE_NAME_VVS, "kph", 0},
	{tsOutputChannels.internalMcuTemperature, GAUGE_NAME_CPU_TEMP, "C", 0},
	{tsOutputChannels.coolantTemperature, "CLT", "C", 1},
	{tsOutputChannels.intakeAirTemperature, "IAT", "C", 1},
	{tsOutputChannels.throttlePosition, "TPS", "%", 2},
	{tsOutputChannels.pedalPosition, GAUGE_NAME_THROTTLE_PEDAL, "%", 2},
	{tsOutputChannels.manifoldAirPressure, "MAP", "kPa", 1},
	{tsOutputChannels.airFuelRatio, GAUGE_NAME_AFR, "afr", 2},
	{tsOutputChannels.vBatt, GAUGE_NAME_VBAT, "v", 2},
	{tsOutputChannels.oilPressure, "Oil Press", "kPa", 0},
	{tsOutputChannels.vvtPosition, GAUGE_NAME_VVT, "deg", 1},
	{tsOutputChannels.chargeAirMass, GAUGE_NAME_AIR_MASS, "g", 3},
	{tsOutputChannels.currentTargetAfr, GAUGE_NAME_TARGET_AFR, "afr", 2},
	{tsOutputChannels.fuelBase, GAUGE_NAME_FUEL_BASE, "ms", 3},
	{tsOutputChannels.fuelRunning, GAUGE_NAME_FUEL_RUNNING, "ms", 3},
	{tsOutputChannels.actualLastInjection, GAUGE_NAME_FUEL_LAST_INJECTION, "ms", 3},
	{tsOutputChannels.injectorDutyCycle, GAUGE_NAME_FUEL_INJ_DUTY, "%", 0},
	{tsOutputChannels.veValue, GAUGE_NAME_FUEL_VE, "%", 1},
	{tsOutputChannels.tCharge, "tCharge", "C", 1},
	{tsOutputChannels.injectorLagMs, GAUGE_NAME_INJECTOR_LAG, "ms", 3},
	{tsOutputChannels.fuelPidCorrection, GAUGE_NAME_FUEL_PID_CORR, "ms", 3},
	{tsOutputChannels.wallFuelCorrection, GAUGE_NAME_FUEL_WALL_CORRECTION, "ms", 3},
	{tsOutputChannels.tpsAccelFuel, GAUGE_NAME_FUEL_TPS_EXTRA, "ms", 3},
	{tsOutputChannels.ignitionAdvance, GAUGE_NAME_TIMING_ADVANCE, "deg", 1},
	{tsOutputChannels.sparkDwell, GAUGE_COIL_DWELL_TIME, "ms", 1},
	{tsOutputChannels.coilDutyCycle, GAUGE_NAME_DWELL_DUTY, "%", 0},
	{tsOutputChannels.idlePosition, GAUGE_NAME_IAC, "%", 1},
	{tsOutputChannels.etbTarget, "ETB Target", "%", 2},
	{tsOutputChannels.etb1DutyCycle, "ETB Duty", "%", 1},
	{tsOutputChannels.etb1Error, "ETB Error", "%", 3},
	{tsOutputChannels.fuelTankLevel, "fuel level", "%", 0},
};

void writeHeader(char* buffer) {
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

	// Data begin index - always begin at 4096 = 0x800 bytes to allow space for header
	buffer[14] = 0;
	buffer[15] = 0;
	buffer[16] = 0x08;
	buffer[17] = 0x00;

	// Record length - length of a single data record: sum size of all fields
	uint16_t recLength = 0;
	for (size_t i = 0; i < efi::size(fields); i++) {
		recLength += fields[i].getSize();
	}

	buffer[18] = recLength >> 8;
	buffer[19] = recLength & 0xFF;

	// Number of logger fields
	buffer[20] = 0;
	buffer[21] = efi::size(fields);

	// Write the actual logger fields, offset 22
	char* entryHeaders = buffer + 22;
	for (size_t i = 0; i < efi::size(fields); i++) {
		size_t sz = fields[i].writeHeader(entryHeaders);
		entryHeaders += sz;
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
