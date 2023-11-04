

#include <jni.h>

// OpenBLT host library
#include "openblt.h"

#include <cstring>

class Callbacks {
public:
	Callbacks(JNIEnv* env, jobject jCallbacks)
		: m_env(env)
		, m_obj(jCallbacks)
		, m_class(env->FindClass("OpenbltJni.Callbacks"))
		, m_log(env->GetMethodID(m_class, "log", "(java/lang/String)V"))
		, m_updateProgress(env->GetMethodID(m_class, "updateProgress", "(I)V"))
		, m_error(env->GetMethodID(m_class, "error", "(java/lang/String)V"))
	{
	}

	void log(const char* line) {
		jstring jLine = m_env->NewStringUTF(line);
		m_env->CallObjectMethod(m_obj, m_error, jLine);
		m_env->DeleteLocalRef(jLine);
	}

	void updateProgress(int percent) {
		m_env->CallIntMethod(m_obj, m_updateProgress, percent);
	}

	void error(const char* err) {
		jstring jErr = m_env->NewStringUTF(err);
		m_env->CallObjectMethod(m_obj, m_error, jErr);
		m_env->DeleteLocalRef(jErr);
	}

private:
	JNIEnv* const m_env;
	jobject const m_obj;

	jclass const m_class;

	jmethodID const m_log;
	jmethodID const m_updateProgress;
	jmethodID const m_error;
};

extern "C" JNIEXPORT void JNICALL Java_com_rusefi_maintenance_OpenbltJni_loadFirmware(JNIEnv* env, jobject, jstring jFilename, jobject jCallbacks) {
	Callbacks cb(env, jCallbacks);

	const char* filename = env->GetStringUTFChars(jFilename, 0);

	BltFirmwareInit(BLT_FIRMWARE_PARSER_SRECORD);

	if (BltFirmwareLoadFromFile(filename, 0) != BLT_RESULT_OK) {
		cb.error("BltFirmwareLoadFromFile() not OK, failed to load firmware file.");
		return;
	}

	env->ReleaseStringUTFChars(jFilename, filename);

	// Check that the file isn't empty
	if (BltFirmwareGetSegmentCount() == 0) {
		cb.error("BltFirmwareGetSegmentCount() returned 0");
		return;
	}
}

static tBltSessionSettingsXcpV10 xcpSettings;
static tBltTransportSettingsXcpV10Rs232 transportSettings;
static char s_portName[256];

extern "C" JNIEXPORT void JNICALL Java_com_rusefi_maintenance_OpenbltJni_sessionStart(JNIEnv* env, jobject, jstring jSerialPort, jobject jCallbacks) {
	Callbacks cb(env, jCallbacks);

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
		cb.error("BltSessionStart() failed");
	}
}

extern "C" JNIEXPORT void JNICALL Java_com_rusefi_maintenance_OpenbltJni_erase(JNIEnv* env, jobject, jobject jCallbacks) {
	Callbacks cb(env, jCallbacks);

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
			cb.updateProgress(progressPct);
		}
	}
}

extern "C" JNIEXPORT void JNICALL Java_com_rusefi_maintenance_OpenbltJni_program(JNIEnv* env, jobject, jobject jCallbacks) {
	Callbacks cb(env, jCallbacks);

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
			cb.updateProgress(progressPct);
		}
	}
}

extern "C" JNIEXPORT void JNICALL Java_com_rusefi_maintenance_OpenbltJni_stop(JNIEnv*, jobject, jobject jCallbacks) {
	BltSessionStop();
	BltSessionTerminate();
	BltFirmwareTerminate();
}
