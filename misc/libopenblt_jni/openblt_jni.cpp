

#include <jni.h>

// OpenBLT host library
#include "openblt.h"

#include <cstring>

class Callbacks {
public:
	Callbacks(JNIEnv* env, jobject jCallbacks, const char* phaseName, bool hasProgress)
		: m_env(env)
		, m_obj(jCallbacks)
		, m_class(env->FindClass("com/rusefi/maintenance/OpenbltJni$OpenbltCallbacks"))
		, m_log(env->GetMethodID(m_class, "log", "(Ljava/lang/String;)V"))
		, m_phase(env->GetMethodID(m_class, "setPhase", "(Ljava/lang/String;Z)V"))
		, m_updateProgress(env->GetMethodID(m_class, "updateProgress", "(I)V"))
		, m_error(env->GetMethodID(m_class, "error", "(Ljava/lang/String;)V"))
	{
		phase(phaseName, hasProgress);
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
	jmethodID const m_phase;
	jmethodID const m_updateProgress;
	jmethodID const m_error;

	void phase(const char* name, bool hasProgress) {
		jstring jName = m_env->NewStringUTF(name);
		m_env->CallObjectMethod(m_obj, m_phase, jName, hasProgress);
		m_env->DeleteLocalRef(jName);
	}
};

static bool loadFirmware(JNIEnv* env, jstring jFilename, jobject jCallbacks) {
	Callbacks cb(env, jCallbacks, "Load firmware file", false);

	const char* filename = env->GetStringUTFChars(jFilename, 0);

	BltFirmwareInit(BLT_FIRMWARE_PARSER_SRECORD);

	if (BltFirmwareLoadFromFile(filename, 0) != BLT_RESULT_OK) {
		cb.error("BltFirmwareLoadFromFile() not OK, failed to load firmware file.");
		return false;
	}

	env->ReleaseStringUTFChars(jFilename, filename);

	// Check that the file isn't empty
	if (BltFirmwareGetSegmentCount() == 0) {
		cb.error("BltFirmwareGetSegmentCount() returned 0");
		return false;
	}

	return true;
}

static tBltSessionSettingsXcpV10 xcpSettings;
static tBltTransportSettingsXcpV10Rs232 transportSettings;
static char s_portName[256];

static bool setupSerial(JNIEnv* env, jstring jSerialPort, jobject jCallbacks) {
	Callbacks cb(env, jCallbacks, "Start session", false);

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
		return false;
	}

	return true;
}

static bool setupCan(JNIEnv* env, jobject jCallbacks) {
	Callbacks cb(env, jCallbacks, "Setup CAN", false);

	cb.error("CAN not supported yet!");
	return false;
}

static bool erase(JNIEnv* env, jobject jCallbacks) {
	Callbacks cb(env, jCallbacks, "Erase", true);

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
			cb.error("BltFirmwareGetSegment not OK");
			return false;
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
				cb.error("BltSessionClearMemory not OK");
				return false;
			}
			/* Update loop variables. */
			currentEraseBase += currentEraseCnt;
			stillToEraseCnt -= currentEraseCnt;

			uint8_t progressPct = (uint8_t)(((segmentLen - stillToEraseCnt) * 100ul) / segmentLen);
			cb.updateProgress(progressPct);
		}
	}

	return true;
}

static bool program(JNIEnv* env, jobject jCallbacks) {
	Callbacks cb(env, jCallbacks, "Program", true);

	uint32_t segmentIdx;
	uint32_t segmentLen;
	uint32_t segmentBase;
	uint8_t const * segmentData;

	int lastPercent = -1;

	/* Program the memory segments on the target with the firmware data. */
	for (segmentIdx = 0; segmentIdx < BltFirmwareGetSegmentCount(); segmentIdx++) 
	{
		/* Extract segment info. */
		segmentData = BltFirmwareGetSegment(segmentIdx, &segmentBase, &segmentLen);

		// Only continue if sanity check passed.
		if ((segmentData == nullptr) || (segmentLen == 0)) {
			cb.error("BltFirmwareGetSegment not OK");
			return false;
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
				cb.error("BltSessionWriteData not OK");
				return false;
			}

			/* Update loop variables. */
			currentWriteBase += currentWriteCnt;
			currentWriteDataPtr += currentWriteCnt;
			stillToWriteCnt -= currentWriteCnt;

			int progressPct = (int)(((segmentLen - stillToWriteCnt) * 100ul) / segmentLen);
			if (progressPct != lastPercent) {
				cb.updateProgress(progressPct);
			}
			lastPercent = progressPct;
		}
	}

	return true;
}

extern "C" JNIEXPORT void JNICALL Java_com_rusefi_maintenance_OpenbltJni_flashSerialNative(JNIEnv* env, jobject, jstring jFirmwareFile, jstring jSerialPort, jobject jCallbacks) {
	if (!loadFirmware(env, jFirmwareFile, jCallbacks)) {
		return;
	}

	if (!setupSerial(env, jSerialPort, jCallbacks)) {
		return;
	}

	if (!erase(env, jCallbacks)) {
		return;
	}

	if (!program(env, jCallbacks)) {
		return;
	}
}

extern "C" JNIEXPORT void JNICALL Java_com_rusefi_maintenance_OpenbltJni_flashCanNative(JNIEnv * env, jobject, jstring jFirmwareFile, jobject jCallbacks) {
	if (!loadFirmware(env, jFirmwareFile, jCallbacks)) {
		return;
	}

	if (!setupCan(env, jCallbacks)) {
		return;
	}

	if (!erase(env, jCallbacks)) {
		return;
	}

	if (!program(env, jCallbacks)) {
		return;
	}
}

extern "C" JNIEXPORT void JNICALL Java_com_rusefi_maintenance_OpenbltJni_stopNative(JNIEnv* env, jobject, jobject jCallbacks) {
	Callbacks cb(env, jCallbacks, "Cleanup", false);

	BltSessionStop();
	BltSessionTerminate();
	BltFirmwareTerminate();
}
