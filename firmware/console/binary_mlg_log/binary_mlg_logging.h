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

    // Resolve the per-field memory offset that the unit-test build uses to
    // translate nullptr-based field addresses to the live `engine` pointer.
    // Returns nullptr in production builds.
    void* getUnitTestFieldOffset(const Entries::Field& field);
}
