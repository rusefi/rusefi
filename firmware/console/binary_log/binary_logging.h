/**
 * @file binary_logging.h
 */

#pragma once

#include "buffered_writer.h"

int getSdCardFieldsCount();

void writeSdLogLine(Writer& buffer);
void resetFileLogging();
