/*
 * @file logicdata_reader.cpp
 *
 * Inverse of logicdata.cpp - parses .logicdata binary files written by
 * writeLogicDataFile() back into CompositeEvent stream.
 *
 * The .logicdata writer uses a variable-length integer scheme:
 *   byte N (0, 1..4, or 8) specifies how many following little-endian bytes
 *   encode the value. N=0 means value zero (no bytes follow).
 *
 * This reader mirrors the writer step-by-step.
 */

#include "logicdata.h"
#include "logicdata_reader.h"
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>
#include <stdexcept>

namespace {

#define BLOCK 0x15
#define CHANNEL_BLOCK 0x16
#define SUB 0x54
#define FLAG_NOTEMPTY 2
#define FLAG_NOTEMPTY_LONG 3
#define FLAG_EMPTY 5

#define SIGN_FLAG 0x80000000L

static const int kNumChannels = 6;

static FILE* g_fp = nullptr;

static uint8_t readByte() {
	int c = fgetc(g_fp);
	if (c == EOF) throw std::runtime_error("unexpected EOF");
	return (uint8_t)c;
}

// Inverse of write(): read length tag and that many little-endian bytes.
static uint64_t readVar() {
	uint8_t n = readByte();
	if (n == 0) return 0;
	if (n > 8) throw std::runtime_error("bad var length");
	uint64_t v = 0;
	for (int i = 0; i < n; i++) {
		v |= ((uint64_t)readByte()) << (i * 8);
	}
	return v;
}

// Skip one variable-length value.
static void skipVar() { (void)readVar(); }

static void skipVars(int count) {
	for (int i = 0; i < count; i++) skipVar();
}

// Inverse of writeId(): three vars.
static void skipId() { skipVar(); skipVar(); skipVar(); }

// Inverse of writeString().
static void skipString() {
	uint64_t len = readVar();
	for (uint64_t i = 0; i < len; i++) (void)readByte();
}

// Inverse of writeDouble(). Written as length=8 + 8 raw bytes, but some files
// may emit shorter encodings; accept any 0..8.
static void skipDouble() {
	uint8_t n = readByte();
	if (n == 0) return;
	if (n > 8) throw std::runtime_error("bad double length");
	for (int i = 0; i < n; i++) (void)readByte();
}

static void readHeader() {
	uint8_t magic = readByte();
	if (magic != 0x7f) throw std::runtime_error("bad magic");
	skipVar();    // duplicate strlen(title), see writeHeader
	skipString(); // title

	skipVar(); // BLOCK
	skipVar(); // SUB
	skipVar(); // frequency
	skipVar(); // 0
	skipVar(); // reservedDurationInSamples
	skipVar(); // frequency/frequencyDiv
	skipVars(2);
	uint64_t nch = readVar(); // numChannels
	if ((int)nch != kNumChannels) {
		// still attempt to proceed, but we only support kNumChannels
	}

	skipVar(); // BLOCK
	skipVar(); // 0

	skipVar(); // BLOCK
	for (int i = 0; i < kNumChannels; i++) {
		skipId();
	}
	skipVar(); // 0

	skipVar(); // BLOCK
	skipId();
	skipVar();
	skipVar();
}

// Inverse of writeChannelHeader.
static void readChannelHeader(int ch) {
	skipVar(); // 0xff
	skipVar(); // ch
	skipString(); // name
	skipVars(2);
	skipDouble();
	skipVar();
	skipDouble();
	skipVar();
	skipDouble();
	if (ch == kNumChannels - 1) {
		skipVar(); // 0
	} else {
		skipId();
		for (int i = 0; i < 3; i++) skipVar();
	}
}

static void readChannelDataHeader() {
	skipVar(); // BLOCK
	skipVar(); // scaledDurationInSamples
	skipVars(5);
	skipVar(); // numChannels
	skipVars(3);
	skipId();
	skipVar();

	skipVar(); // BLOCK
	skipVars(3);

	for (int i = 0; i < kNumChannels; i++) {
		readChannelHeader(i);
	}

	skipVar(); // BLOCK
	skipVars(6); // SUB_ARRAY
	skipVars(6);

	skipVar(); // BLOCK
	skipVars(2);
	skipVar(); // realDurationInSamples
	skipVar();
	skipVar(); // SUB
	skipVar(); // reservedDurationInSamples
	skipVar(); // freq/div
	skipVars(2);
	skipVar(); // SUB
	skipVars(2);
	skipVar(); // 1
	skipVars(3);
	skipId();

	skipVar(); // BLOCK
	skipVars(3); // SAM_ARRAY
	skipVar();
	skipVar(); // SUB
	skipVar();

	skipVar(); // BLOCK
	skipVar();

	skipVar(); // BLOCK
	skipVars(4); // SUB four times
	skipVar();

	skipVar(); // BLOCK
	skipVar(); // frequency
	skipVars(3);
	skipVar(); // 1
	skipVars(3);
	skipId();
	skipVars(6); // ARR_6
	skipVar(); // SUB

	skipVar(); // BLOCK
	skipVar(); // 0
	skipVar(); // realDurationInSamples
	skipVars(2);
	skipVar(); // numChannels
	skipVars(3); // ARR_3
}

struct ChannelEdges {
	std::vector<uint32_t> timestamps; // absolute, after delta accumulation
	std::vector<int> states;          // state AFTER the edge (the new state)
	int initialState = 0;             // chPrevState before first edge (we infer)
	int lastState = 0;                // chLastState as recorded in file
};

static void readEdges(std::vector<uint32_t>& deltas, std::vector<int>& states,
                      bool useLongDeltas, int numEdges) {
	// Edges are written as raw little-endian bytes (2 or 4 per edge), followed
	// by a single 0x00 terminator byte.
	for (int e = 0; e < numEdges; e++) {
		uint8_t b0 = readByte();
		uint8_t b1 = readByte();
		uint32_t raw;
		if (useLongDeltas) {
			uint8_t b2 = readByte();
			uint8_t b3 = readByte();
			raw = (uint32_t)b0 | ((uint32_t)b1 << 8) | ((uint32_t)b2 << 16) | ((uint32_t)b3 << 24);
		} else {
			raw = (uint32_t)b0 | ((uint32_t)b1 << 8);
		}

		// Decode sign flag and recover delta and new state.
		// Writer: if state went to 0 -> delta |= SIGN_FLAG. Then for short
		// deltas, SIGN_FLAG (bit 31) was relocated to bit 15.
		int newState = 1;
		uint32_t delta;
		if (useLongDeltas) {
			if (raw & SIGN_FLAG) {
				newState = 0;
				delta = raw & ~SIGN_FLAG;
			} else {
				newState = 1;
				delta = raw;
			}
		} else {
			if (raw & 0x8000) {
				newState = 0;
				delta = raw & 0x7fff;
			} else {
				newState = 1;
				delta = raw;
			}
		}
		deltas.push_back(delta);
		states.push_back(newState);
	}
}

static void readChannelData(int ch, ChannelEdges& out) {
	skipVar(); // CHANNEL_BLOCK
	if (ch == 0) {
		skipVar(); // SUB
		skipVar(); // BLOCK
	}
	skipVar(); // ch+1
	skipVar(); // 0
	skipVar(); // realDurationInSamples
	skipVar(); // 1
	skipVar(); // lastRecord
	skipVar(); // numSamplesLeft
	out.lastState = (int)readVar(); // chLastState
	uint64_t chFlag = readVar();

	bool empty = (chFlag == FLAG_EMPTY);
	bool useLongDeltas = (chFlag == FLAG_NOTEMPTY_LONG);

	if (ch == 0) {
		skipVar(); // 0
		skipVar(); // BLOCK
		skipVars(11);
		if (useLongDeltas) {
			skipVar(); // BLOCK
			skipVars(6);
		}
		skipVar(); // BLOCK
	} else {
		skipVars(10);
		if (useLongDeltas) {
			skipVars(5);
		}
	}

	uint64_t numEdges1 = readVar();
	skipVar(); // 0
	skipVar(); // numEdges
	skipVar(); // 0
	skipVar(); // numEdges

	std::vector<uint32_t> deltas;
	std::vector<int> newStates;
	// Writer always calls writeEdges regardless of empty/non-empty, which
	// loops numEdges times then writes a single 0x00 terminator byte. For
	// empty channels numEdges is 0 so we just consume the terminator.
	readEdges(deltas, newStates, useLongDeltas, (int)numEdges1);
	(void)readByte(); // 0x00 terminator

	if (ch == 0) {
		skipVar(); // BLOCK
		skipVars(6);
		if (!useLongDeltas) {
			skipVar(); // BLOCK
			skipVars(6);
		}
		skipVar(); // BLOCK
	} else {
		skipVars(4);
		if (!useLongDeltas) {
			skipVars(5);
		}
	}

	if (empty) {
		skipVars(5);
		out.initialState = out.lastState;
		return;
	}

	skipVar(); // 1
	skipVar(); // 0
	skipVar(); // 1
	skipVar(); // 0
	skipVar(); // 1
	skipVars(16);

	for (int i = 0; i < 8; i++) (void)readByte(); // 0xFF * 8
	(void)readByte(); // chFlag raw
	for (int i = 0; i < 7; i++) (void)readByte(); // 0x00 * 7

	// Convert deltas to absolute timestamps.
	out.timestamps.reserve(deltas.size());
	uint32_t ts = 0;
	for (size_t i = 0; i < deltas.size(); i++) {
		ts += deltas[i];
		out.timestamps.push_back(ts);
		out.states.push_back(newStates[i]);
	}
	// Initial state = opposite of first edge's new state (since edges are
	// state changes), or equal to lastState if no edges.
	if (!out.states.empty()) {
		out.initialState = out.states[0] ? 0 : 1;
	} else {
		out.initialState = out.lastState;
	}
	(void)numEdges1;
}

} // namespace

std::vector<CompositeEvent> readLogicDataFile(const char* fileName) {
	g_fp = fopen(fileName, "rb");
	if (!g_fp) {
		throw std::runtime_error(std::string("cannot open ") + fileName);
	}

	std::vector<CompositeEvent> result;
	try {
		readHeader();
		readChannelDataHeader();

		ChannelEdges channels[kNumChannels];
		for (int ch = 0; ch < kNumChannels; ch++) {
			readChannelData(ch, channels[ch]);
		}

		// Merge per-channel edges into composite events sorted by timestamp.
		// At each unique timestamp, snapshot all channel states.
		struct Edge { uint32_t ts; int ch; int newState; };
		std::vector<Edge> all;
		for (int ch = 0; ch < kNumChannels; ch++) {
			for (size_t i = 0; i < channels[ch].timestamps.size(); i++) {
				all.push_back({channels[ch].timestamps[i], ch, channels[ch].states[i]});
			}
		}
		std::stable_sort(all.begin(), all.end(),
			[](const Edge& a, const Edge& b){ return a.ts < b.ts; });

		int curState[kNumChannels];
		for (int ch = 0; ch < kNumChannels; ch++) {
			curState[ch] = channels[ch].initialState;
		}

		// Emit one CompositeEvent per unique timestamp.
		size_t i = 0;
		while (i < all.size()) {
			uint32_t ts = all[i].ts;
			while (i < all.size() && all[i].ts == ts) {
				curState[all[i].ch] = all[i].newState;
				i++;
			}
			CompositeEvent ev{};
			ev.timestamp = ts;
			ev.primaryTrigger = curState[0];
			ev.secondaryTrigger = curState[1];
			ev.isTDC = curState[2];
			ev.sync = curState[3];
			ev.coil = curState[4];
			ev.injector = curState[5];
			result.push_back(ev);
		}
	} catch (const std::exception& e) {
		fclose(g_fp);
		g_fp = nullptr;
		throw;
	}

	fclose(g_fp);
	g_fp = nullptr;
	return result;
}
