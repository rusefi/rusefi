/*
 * @file dtc_manager.cpp
 *
 * @date Jul 22, 2026
 * @author Andrey Gusakov
 */

#include "pch.h"

#include "rusefi/efistring.h"
#include "dtc_manager.h"

#if EFI_PROD_CODE && MODULE_DTC_MANAGER && EFI_FILE_LOGGING

#include "ch.hpp"

#include "mmc_card.h"
#include "binary_mlg_logging.h"

#include "board_overrides.h"

extern ToothLoggerBufferPool toothBuffers;

/**
 * @brief Manages Diagnostic Trouble Code (DTC) event logging to SD card.
 *
 * This class captures a "freeze frame" in MLG format and a circular buffer of tooth logger data in CSV format
 * when a DTC event is triggered.
 */
class DtcManager : public ThreadController<4 * UTILITY_THREAD_STACK_SIZE> {
public:
	bool moduleStart(FIL *fd, FileBufferedWriter *writer);
	void moduleStop();

	DtcManager() :
		ThreadController("DTC manager", PRIO_DTC_MANAGER),
		m_dtcTrigger(true), m_circularBufferReady(true) { }

	int onEvent(const char *reason, ObdCode code, efitick_t timestamp);
	int onToothBufferFilled(efitick_t timestamp);

private:
	void ThreadTask() override;

	FIL *m_fd;
	FileBufferedWriter *m_writer;
	const char* m_dtcDirectory = "/DTC";

	// DTC is already pending...
	bool m_pending = false;
	// flag showing that we have been triggered due to dtc
	chibios_rt::BinarySemaphore m_dtcTrigger;
	Timer m_dtcTriggerTimestamp = {};
	chibios_rt::BinarySemaphore m_circularBufferReady;
	Timer m_circularBufferReadyTimestamp = {};

	// TODO: scan FS for older items and set to +1 to last one
	size_t m_iteration = 0;
	char m_reason[32];
	ObdCode m_code = ObdCode::None;

	int createDir(const char *path);
	int createDirs();
	int createFile(FIL *fd, bool isCsv = true);

	// MLG
	int writeMlgFreezeFrame(FileBufferedWriter &writer);
	int writeMlgFile();

	// CSV
	int writeCsvComment(Writer &writer);
	/**
	 * @brief Writes the circular tooth logger buffer to CSV.
	 *
	 * This method captures both the current (partially filled) buffer and all full buffers
	 * available in the pool. It ensures the oldest data (tail of current buffer) is written first,
	 * followed by any full buffers, and finally the newest data (head of current buffer).
	 */
	int writeCsvCircular(Writer &writer);
	int writeCsvFile();
};

int DtcManager::createDir(const char *path) {
	FRESULT fr;

	fr = f_mkdir(path);

	if ((fr == FR_OK) || (fr == FR_EXIST)) {
		return 0;
	}

	efiPrintf("failed to create directory %s %s %d", path, getFatFsErrorDescription(fr), fr);

	return -fr;
}

int DtcManager::createFile(FIL *fd, bool isCsv) {
	char fileName[64];

	// clear the memory
	memset(fd, 0, sizeof(FIL));

	// generate file name
	chsnprintf(fileName, sizeof(fileName),
		"%s/%04d/%05d_%c%04d/%s",
		m_dtcDirectory, m_iteration,
		(int)(m_dtcTriggerTimestamp.get() / US_PER_SECOND), ObdCodeIsCustom(m_code) ? 'C' : 'P', (int)m_code,
		isCsv ? "trigger.csv" : "freeze.mlg");
	efiPrintf("Creating %s crush dump file %s", isCsv ? "CSV" : "mlg", fileName);
	// Create new file. If file is exist - truncate and overwrite, we need header to be at zero offset.
	FRESULT fr = f_open(fd, fileName, FA_CREATE_ALWAYS | FA_WRITE);

	if ((fr == FR_OK) || (fr == FR_EXIST)) {
		return 0;
	}

	efiPrintf("failed to create file %s %s %d", fileName, getFatFsErrorDescription(fr), fr);

	return -fr;
}

int DtcManager::writeMlgFreezeFrame(FileBufferedWriter &writer) {
	int ret = 0;
	size_t total = 0;

	ret = MLG::writeFileHeader(writer);
	if (ret < 0) {
		return ret;
	}
	total += ret;

	ret = MLG::writeSdBlock(writer);
	if (ret < 0) {
		return ret;
	}
	total += ret;

	return total;
}

int DtcManager::createDirs() {
	int ret = 0;
	char path[64];

	// Create DTC directory
	ret = createDir(m_dtcDirectory);
	if (ret < 0) {
		return ret;
	}

	// Create currecnt session directory
	chsnprintf(path, sizeof(path),
		"%s/%04d",
		m_dtcDirectory, m_iteration);
	ret = createDir(path);
	if (ret < 0) {
		return ret;
	}

	// create current DTC directory
	chsnprintf(path, sizeof(path),
		"%s/%04d/%05d_%c%04d/",
		m_dtcDirectory, m_iteration,
		(int)(m_dtcTriggerTimestamp.get() / US_PER_SECOND), ObdCodeIsCustom(m_code) ? 'C' : 'P', (int)m_code);
	ret = createDir(path);
	if (ret < 0) {
		return ret;
	}

	efiPrintf("DTC directory %s created", path);

	return 0;
}

int DtcManager::writeMlgFile() {
	int ret = 0;

	// MLG file
	ret = createFile(m_fd, false);
	if (ret == 0) {
		ret = m_writer->init(m_fd);
		if (ret == 0) {
			ret = writeMlgFreezeFrame(*m_writer);
			if (ret >= 0) {
				efiPrintf("%d bytes writen to MLG crush dump file", ret);
			}
			m_writer->stop();
		}
		f_close(m_fd);
	}

	return ret;
}

int DtcManager::writeCsvComment(Writer &writer) {
	size_t total = 0;
	char tmp[128];

	int ret = chsnprintf(tmp, sizeof(tmp),
		"# this is event logger file created because of reason %s\r\n", m_reason);
	if (ret < 0) {
		return ret;
	}
	writer.write(tmp, ret);
	total += ret;

	ret = chsnprintf(tmp, sizeof(tmp),
		"# event happened at %.6f S uptime\r\n", NT2US(m_dtcTriggerTimestamp.get()) / 1000000.0);
	if (ret < 0) {
		return ret;
	}
	writer.write(tmp, ret);
	total += ret;

	return total;
}

int DtcManager::writeCsvCircular(Writer &writer) {
	int ret = 0;
	size_t total = 0;
	CompositeBuffer* buffer = nullptr;

	ret = ToothLoggerWriteCsvHeader(writer);
	if (ret < 0) {
		goto exit;
	}
	total += ret;

	// In circular mode, data is wrapped. The 'current' buffer contains both:
	// 1. The oldest data (from nextIdx to end)
	// 2. The newest data (from 0 to nextIdx)
	buffer = toothBuffers.getCurrent();
	if (buffer) {
		// Write tail of current buffer fisrt...
		ret = ToothLoggerWriteBufferCsv(writer, buffer, true);
		if (ret < 0) {
			goto exit;
		}
		total += ret;

		chibios_rt::CriticalSectionLocker csl;
		toothBuffers.postCurrentI();
	}

	// Now write all buffers that were marked as filled and are waiting in the queue
	while (toothBuffers.fetchFilled(&buffer, TIME_IMMEDIATE) == MSG_OK) {
		if (buffer != nullptr) {
			ret = ToothLoggerWriteBufferCsv(writer, buffer);
			if (ret < 0) {
				goto exit;
			}
			total += ret;

			ReturnToothLoggerBuffer(buffer);
			buffer = nullptr;
		}
	}

exit:
	return (ret < 0) ? ret : total;
}

int DtcManager::writeCsvFile() {
	int ret = 0;

	// CSV file
	ret = createFile(m_fd, true);
	if (ret == 0) {
		ret = m_writer->init(m_fd);
		if (ret == 0) {
			int ret_header = writeCsvComment(*m_writer);
			if (ret_header >= 0) {
				int ret_data = writeCsvCircular(*m_writer);
				if (ret_data >= 0) {
					efiPrintf("%d bytes written to CSV crush dump file", ret_header + ret_data);
				}
			}
			m_writer->stop();
		}
		f_close(m_fd);
	}

	return ret;
}

bool DtcManager::moduleStart(FIL *fd, FileBufferedWriter *writer) {
	if (isStarted()) {
		return true;
	}

	if ((fd == nullptr) || (writer == nullptr)) {
		return false;
	}

	m_fd = fd;
	m_writer = writer;

	if (EnableToothLogger(TLmode::Background)) {
		ThreadController::start();
	} else {
		efiPrintf("Failed to start DTC. ToothLogger is busy. Console?");
	}

	return isStarted();
}

void DtcManager::moduleStop() {
	// ask to stop
	requestStop();

	// reset both semaphores
	m_circularBufferReady.reset(true);
	m_dtcTrigger.reset(true);

	// wait
	waitStop();

	DisableToothLogger(TLmode::Background);
	// both pointers are used from thread only, safe to zero after thread is stopped
	m_fd = nullptr;
	m_writer = nullptr;
}

void DtcManager::ThreadTask() {
	efiPrintf("DTC thread started");

	while (!chThdShouldTerminateX()) {
		int ret = 0;

		// Wait for trigger event...
		if (m_dtcTrigger.wait() == MSG_OK) {
			// Phase 1: Create directories and write MLG freeze frame
			{
				FsGuard guard;
				if (!guard.isLocked()) {
					efiPrintf("DTC writer: failed to lock FS");
				} else {
					ret = createDirs();
					if (ret < 0) {
						efiPrintf("Failed to create dirs");
					} else if (ret >= 0) {
						// Write current engine state as MLG
						ret = writeMlgFile();
						if (ret < 0) {
							efiPrintf("Failed to write MLG %d", ret);
						}
					}
				}
				// FS unlocked
			}

			// Wait for circular buffer to fill or timeout
			if (m_circularBufferReady.wait(TIME_MS2I(3000)) == MSG_OK) {
				FsGuard guard;
				if (!guard.isLocked()) {
					efiPrintf("DTC writer: failed to lock FS");
				} else {
					ret = writeCsvFile();
					if (ret < 0) {
						efiPrintf("Failed to write CSV %d", ret);
					}
				}
				// FS unlocked
			} else {
				efiPrintf("Timeout waiting circle buffer to fill");
			}

			// TODO: restart tooth logger in free-running mode

		}

		// reset
		m_pending = false;
		ToothLoggerRelease();
	}

	efiPrintf("DTC thread stopped");

	chThdExit(MSG_OK);
}

int DtcManager::onEvent(const char *reason, ObdCode code, efitick_t timestamp) {
	chibios_rt::CriticalSectionLocker csl;

	if (!isStarted()) {
		return -1;
	}

	if (m_pending) {
		// already pending
		return -2;
	}

	strlncpy(m_reason, reason ? reason : "UNKNOWN", sizeof(m_reason));
	m_code = code;

	m_dtcTriggerTimestamp.reset(timestamp);
	m_dtcTrigger.signalI();
	m_pending = true;

	// Trigger point is 50%
	ToothLoggerSetLimit(toothBuffers.toothLoggerEntriesTotal / 2);

	return 0;
}

int DtcManager::onToothBufferFilled(efitick_t timestamp) {
	chibios_rt::CriticalSectionLocker csl;

	if (!isStarted()) {
		return -1;
	}

	if (!m_pending) {
		return -1;
	}

	m_circularBufferReadyTimestamp.reset(timestamp);
	m_circularBufferReady.signalI();

	return 0;
}

static DtcManager dtcManager;

int DtcManagerStart(FIL *p_fd, FileBufferedWriter *p_writer) {
	// See comment in .h file
	//return engine->module<DtcManager>()->moduleStart(p_fd, p_writer);
	return dtcManager.moduleStart(p_fd, p_writer);
}

int DtcManagerStop(void) {
	efiPrintf("DTC manager requested to stop");
	//engine->module<DtcManager>()->moduleStop();
	dtcManager.moduleStop();
	return 0;
}

int DtcTriggerEvent(const char *reason, ObdCode code, efitick_t timestamp) {
	if (engine->module<SdLogTrigger>()->shouldLog()) {
		//return engine->module<DtcManager>()->onEvent(reason, code, timestamp);
		return dtcManager.onEvent(reason, code, timestamp);
	}
	return 0;
}

int DtcToothLoggerFilled(efitick_t timestamp) {
	//return engine->module<DtcManager>()->onToothBufferFilled(timestamp);
	return dtcManager.onToothBufferFilled(timestamp);
}

#else

int DtcManagerStart(void) {
	return -999;
}

int DtcManagerStop(void) {
	return -999;
}

// no circular background logging in this build
int DtcTriggerEvent(const char* /*reason*/, ObdCode /*code*/, efitick_t /*timestamp*/) {
	return -999;
}

int DtcToothLoggerFilled(efitick_t /*timestamp*/) {
	return -999;
}

#endif // MODULE_DTC_MANAGER
