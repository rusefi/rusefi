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

// Per-channel decoded edge stream. One entry per state-change event.
struct ChannelEdgeStream {
    std::string name;
    int initialState = 0;
    int lastState = 0;
    std::vector<uint32_t> timestamps; // absolute, after delta accumulation (in samples)
    std::vector<int> states;          // state AFTER each edge
};

struct LogicDataFull {
    LogicDataHeaderInfo header;
    std::vector<ChannelEdgeStream> channels;
};

// Generic reader: works for both our 6-channel writer's variant (BLOCK=0x15)
// and the real-Saleae / Java LogicdataStreamFile variant (BLOCK=0x18, any
// numChannels declared in the header).
LogicDataFull readLogicDataFull(const char* fileName);

// Convenience: write a generic CSV from a parsed file. The first column is
// time in seconds (sample_count / frequency); subsequent columns are 0/1
// for each channel at that timestamp. One row is emitted per unique edge
// timestamp; the row carries the post-edge state for every channel.
void writeLogicDataGenericCsv(const char* csvFileName, const LogicDataFull& data);
