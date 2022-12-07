/**
 * @file binary_logging.h
 */

#include <cstddef>

struct Writer;
void writeFileHeader(Writer& buffer);
void writeSdLogLine(Writer& buffer);
void writeBlock(Writer& outBuffer);
