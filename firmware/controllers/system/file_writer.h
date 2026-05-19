/**
 * @file file_writer.h
 */

#pragma once

#include <cstring>

#include "buffered_writer.h"

#if EFI_PROD_CODE

#include "ff.h"
#include "mmc_card_util.h"

// TODO: do we need this additioal layer of buffering?
// FIL structure already have buffer of FF_MAX_SS size
// check if it is better to increase FF_MAX_SS and drop BufferedWriter?
struct FileBufferedWriter final : public BufferedWriter<512> {
	bool failed = false;

	int init(FIL *fd) {
		if (m_fd) {
			efiPrintf("File writer already started!");
			return -1;
		}

		m_size = 0;

		m_fd = fd;

		return 0;
	}

	void stop() {
		m_fd = nullptr;

		flush();

		m_size = 0;
	}

	size_t write(const char* buffer, size_t count) override {
		size_t ret = BufferedWriter::write(buffer, count);
		m_size += ret;
		return ret;
	}

	size_t size() {
		return m_size;
	}

	size_t writeInternal(const char* buffer, size_t count) override {
		if ((!m_fd) || (failed)) {
			return 0;
		}

		size_t bytesWritten;
		efiAssert(ObdCode::CUSTOM_STACK_6627, hasLotsOfRemainingStack(), "sdlow#3", 0);
		FRESULT err = f_write(m_fd, buffer, count, &bytesWritten);

		if (err) {
			printFatFsError("file_writer write", err);
			failed = true;
			return 0;
		} else if (bytesWritten != count) {
			printFatFsError("file_writer partitial write", err);
			failed = true;
			return 0;
		}

		return bytesWritten;
	}

	void sync() {
		if ((!m_fd) || (failed)) {
			return;
		}

		f_sync(m_fd);
	}

private:
	FIL *m_fd = nullptr;
	size_t m_size = 0;
};

#else // not EFI_PROD_CODE (simulator)

#include <fstream>

class FileBufferedWriter final : public BufferedWriter<512> {
public:
	bool failed = false;

	FileBufferedWriter()
		: m_stream("rusefi_simulator_log.mlg", std::ios::binary | std::ios::trunc)
	{
		sdLoggerReady = true;
	}

	size_t writeInternal(const char* buffer, size_t count) override {
		m_stream.write(buffer, count);
		m_stream.flush();
		return count;
	}

	void sync() {
		// NOP
	}

private:
	std::ofstream m_stream;
};

#endif