/**
 * @file binary_mlg_logging.h
 */

#pragma once

#include "buffered_writer.h"

namespace MLG{
    int getSdCardFieldsCount();
    size_t writeSdLogLine(Writer& buffer);
    void resetFileLogging();
}
