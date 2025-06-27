/*
 * LogLine.h
 *
 *  Created on: Jun 22, 2025
 *      Author: rusefi
 */

#pragma once

#include <map>
#include "MlgRecord.h"

class LogLine {
public:
    // Using a map with `const Record*` as key.
    // This is similar to Java's behavior where keys are objects, and their identity matters.
    // If you need `Record` values as keys, you'd need to define `operator<` for `Record`
    // or use `std::unordered_map` with a custom hash function for `Record`.
    std::map<const Record*, float> snapshot;

    LogLine(const std::map<const Record*, float>& snapshot) : snapshot(snapshot) {}
    // If you prefer to move the map to avoid copy (C++11 and later):
    // LogLine(std::map<const Record*, float>&& snapshot) : snapshot(std::move(snapshot)) {}
};


