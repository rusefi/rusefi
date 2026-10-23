/**
 * @file binary_mlg_logging.h
 */

#pragma once

#include "buffered_writer.h"

#include <functional>

namespace MLG::Entries { class Field; }

namespace MLG{
    int getSdCardFieldsCount();
    size_t writeFileHeader(Writer& outBuffer);
    size_t writeSdBlock(Writer& outBuffer);
    size_t writeSdLogLine(Writer& buffer);
    void resetFileLogging();

    // Iterate every logger field. Used by unit-test logger to emit CSV / NDJSON.
    void forEachField(const std::function<void(const Entries::Field&)>& callback);

}
