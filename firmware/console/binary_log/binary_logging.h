/**
 * @file binary_logging.h
 */

#pragma once

#include "buffered_writer.h"

int getSdCardFieldsCount();

size_t writeSdLogLine(Writer& buffer);
void resetFileLogging();
