/**
 * @file buffered_writer.h
 */

#pragma once

#include <cstring>

struct Writer {
	virtual size_t write(const char* buffer, size_t count) = 0;
	virtual size_t flush() = 0;
};

template <int TBufferSize>
class BufferedWriter : public Writer {
public:
	size_t write(const char* buffer, size_t count) override {
		size_t bytesFlushed = 0;

		while (count) {
			if (m_bytesUsed == 0 && count >= TBufferSize) {
				// special case: write-thru, skip the copy
				bytesFlushed += writeInternal(buffer, count);
				count = 0;
			} else if (m_bytesUsed + count < TBufferSize) {
				// Write that will fit in the buffer, just copy to intermediate buffer
				memcpy(m_buffer + m_bytesUsed, buffer, count);
				m_bytesUsed += count;
				count = 0;
			} else {
				// Need to write partial, then flush buffer
				size_t bytesToWrite = TBufferSize - m_bytesUsed;
				// Copy this block in to place
				memcpy(m_buffer + m_bytesUsed, buffer, bytesToWrite);
				m_bytesUsed += bytesToWrite;

				// Flush to underlying
				bytesFlushed += flush();
				// Step the read pointer ahead
				buffer += bytesToWrite;
				// Decrement remaining bytes
				count -= bytesToWrite;
			}
		}

		return bytesFlushed;
	}

	// Flush the internal buffer to the underlying interface.
	size_t flush() override {
		size_t bytesToWrite = m_bytesUsed;
		
		if (bytesToWrite > 0) {
			m_bytesUsed = 0;
			return writeInternal(m_buffer, bytesToWrite);
		} else {
			return 0;
		}
	}

protected:
	virtual size_t writeInternal(const char* buffer, size_t count) = 0;

private:
	char m_buffer[TBufferSize];
	size_t m_bytesUsed = 0;
};
