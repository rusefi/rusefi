/*
 * @file logicdata_reader.h
 *
 * Inverse of logicdata.h's writeLogicDataFile().
 */

#pragma once

#include "logicdata.h"
#include <vector>

std::vector<CompositeEvent> readLogicDataFile(const char* fileName);
