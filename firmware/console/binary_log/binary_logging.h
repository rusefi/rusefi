/**
 * @file binary_logging.h
 */

#include <cstddef>

struct Writer;
void writeFileHeader(Writer& buffer);
void writeSdLogLine(Writer& buffer);
void writeSdBlock(Writer& outBuffer);
