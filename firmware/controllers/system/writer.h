/**
 * @file writer.h
 */

#pragma once

#include <cstdint>

struct Writer {
	virtual size_t write(const char* buffer, size_t count) = 0;
	virtual size_t flush() = 0;
};
