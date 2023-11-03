

#include <jni.h>

// OpenBLT host library
#include "openblt.h"

#include <cstring>

JNIEXPORT void JNICALL com_rusefi_maintenance_OpenbltJni_loadFirmware(JNIEnv* env, jobject, jstring jFilename) {
	const char* filename = env->GetStringUTFChars(jFilename, 0);

	BltFirmwareInit(BLT_FIRMWARE_PARSER_SRECORD);

	if (BltFirmwareLoadFromFile(filename, 0) != BLT_RESULT_OK) {
		// todo: error handling
	}

	// Check that the file isn't empty
	if (BltFirmwareGetSegmentCount() == 0) {
		// todo: error handling
	}

	env->ReleaseStringUTFChars(jFilename, filename);
}

static tBltSessionSettingsXcpV10 xcpSettings;
static tBltTransportSettingsXcpV10Rs232 transportSettings;
static char s_portName[256];

JNIEXPORT void JNICALL com_rusefi_maintenance_OpenbltJni_sessionStart(JNIEnv* env, jobject, jstring jSerialPort) {
	xcpSettings.timeoutT1 = 1000;
	xcpSettings.timeoutT3 = 2000;
	xcpSettings.timeoutT4 = 10000;
	xcpSettings.timeoutT5 = 1000;
	xcpSettings.timeoutT6 = 50;
	xcpSettings.timeoutT7 = 2000;
	xcpSettings.seedKeyFile = nullptr;
	xcpSettings.connectMode = 0;

	const char* portName = env->GetStringUTFChars(jSerialPort, 0);
	strncpy(s_portName, portName, sizeof(s_portName));
	env->ReleaseStringUTFChars(jSerialPort, portName);

	transportSettings.portName = s_portName;
	transportSettings.baudrate = 115200;

	BltSessionInit(BLT_SESSION_XCP_V10, &xcpSettings, BLT_TRANSPORT_XCP_V10_RS232, &transportSettings);

	if (BltSessionStart() != BLT_RESULT_OK) {
		// todo: error handling
	}
}

JNIEXPORT void JNICALL com_rusefi_maintenance_OpenbltJni_erase(JNIEnv*, jobject) {
	int result = 0;

	uint32_t segmentIdx;
	uint32_t segmentLen;
	uint32_t segmentBase;
	uint8_t const * segmentData;

	/* Erase the memory segments on the target that are covered by the firmwware data. */
	for (segmentIdx = 0; segmentIdx < BltFirmwareGetSegmentCount(); segmentIdx++) 
	{
		/* Extract segment info. */
		segmentData = BltFirmwareGetSegment(segmentIdx, &segmentBase, &segmentLen);
		/* Sanity check. */
		// Only continue if sanity check passed.
		if ((segmentData == nullptr) || (segmentLen == 0))
		{
			// TODO: error handling
			return;
		}

		/* Perform erase operation. */

		/* Perform erase operation in chunks, so that a progress update can be shown
			* and no erase timeout occurs due to erasing too big of a memory range.
			*/
		uint32_t const eraseChunkSize = 32768;
		uint32_t currentEraseCnt;
		uint32_t currentEraseBase;
		uint32_t currentEraseResult;
		uint32_t stillToEraseCnt;

		stillToEraseCnt = segmentLen;
		currentEraseBase = segmentBase;
		while (stillToEraseCnt > 0)
		{
			/* Determine chunk size. */
			if (stillToEraseCnt >= eraseChunkSize)
			{
				currentEraseCnt = eraseChunkSize;
			}
			else
			{
				currentEraseCnt = stillToEraseCnt;
			}
			/* Erase the next chunk from the target's memory. */
			currentEraseResult = BltSessionClearMemory(currentEraseBase, currentEraseCnt);
			if (currentEraseResult != BLT_RESULT_OK)
			{
				// TODO: error handling
				return;
			}
			/* Update loop variables. */
			currentEraseBase += currentEraseCnt;
			stillToEraseCnt -= currentEraseCnt;

			uint8_t progressPct = (uint8_t)(((segmentLen - stillToEraseCnt) * 100ul) / segmentLen);
			// TODO: report progress
		}
	}
}

JNIEXPORT void JNICALL com_rusefi_maintenance_OpenbltJni_program(JNIEnv*, jobject) {
	uint32_t segmentIdx;
	uint32_t segmentLen;
	uint32_t segmentBase;
	uint8_t const * segmentData;

	/* Program the memory segments on the target with the firmware data. */
	for (segmentIdx = 0; segmentIdx < BltFirmwareGetSegmentCount(); segmentIdx++) 
	{
		/* Extract segment info. */
		segmentData = BltFirmwareGetSegment(segmentIdx, &segmentBase, &segmentLen);

		// Only continue if sanity check passed.
		if ((segmentData == nullptr) || (segmentLen == 0)) {
			// TODO: error handling
			return;
		}

		/* Perform write operation in chunks, so that a progress update can be shown. */
		uint32_t const writeChunkSize = 256;
		uint32_t currentWriteCnt;
		uint32_t currentWriteBase;
		uint8_t const * currentWriteDataPtr;
		uint32_t currentWriteResult;
		uint32_t stillToWriteCnt;

		stillToWriteCnt = segmentLen;
		currentWriteBase = segmentBase;
		currentWriteDataPtr = segmentData;
		while (stillToWriteCnt > 0)
		{
			/* Determine chunk size. */
			if (stillToWriteCnt >= writeChunkSize)
			{
				currentWriteCnt = writeChunkSize;
			}
			else
			{
				currentWriteCnt = stillToWriteCnt;
			}
			/* Write the next data chunk to the target's memory. */
			currentWriteResult = BltSessionWriteData(currentWriteBase, currentWriteCnt,
				currentWriteDataPtr);
			if (currentWriteResult != BLT_RESULT_OK)
			{
				// TODO: error handling
				return;
			}

			/* Update loop variables. */
			currentWriteBase += currentWriteCnt;
			currentWriteDataPtr += currentWriteCnt;
			stillToWriteCnt -= currentWriteCnt;

			uint8_t progressPct = (uint8_t)(((segmentLen - stillToWriteCnt) * 100ul) / segmentLen);
			// TODO: report progress
		}
	}
}

JNIEXPORT void JNICALL com_rusefi_maintenance_OpenbltJni_stop(JNIEnv*, jobject) {
	BltSessionStop();
	BltSessionTerminate();
	BltFirmwareTerminate();
}
