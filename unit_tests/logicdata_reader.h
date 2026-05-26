/*
 * @file logicdata_reader.h
 *
 * Inverse of logicdata.h's writeLogicDataFile().
 */

#pragma once

#include "logicdata.h"
#include <string>
#include <vector>

std::vector<CompositeEvent> readLogicDataFile(const char* fileName);

// Lightweight header-only inspection. Works for both:
//   - files produced by our writeLogicDataFile() (BLOCK=0x15, no version byte,
//     numChannels=6, names "Primary","Secondary",...);
//   - real Saleae / Java LogicdataStreamFile-produced files (BLOCK=0x18, a
//     leading version=0x13 byte before the title length, numChannels read from
//     header, default names "Channel 0","Channel 1",...).
//
// Throws std::runtime_error on parse errors.
struct LogicDataHeaderInfo {
    std::string title;             // e.g. "Data save2"
    int numChannels = 0;           // channels declared in header
    std::vector<std::string> channelNames;
    uint64_t frequency = 0;        // sample frequency from header
    bool hasVersionByte = false;   // true => "real" Saleae format variant
    int blockMarker = 0;           // 0x15 for our writer, 0x18 for real format
};

LogicDataHeaderInfo inspectLogicDataHeader(const char* fileName);
