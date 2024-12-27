/**
 * @file binary_logging.h
 */

#pragma once

#include "buffered_writer.h"

int getSdCardFieldsCount();
void writeFileHeader(Writer& buffer);
void writeSdLogLine(Writer& buffer);
void writeSdBlock(Writer& outBuffer);
