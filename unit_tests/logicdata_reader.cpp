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

// Read a length-prefixed ASCII string into `out`.
static void readString(std::string& out) {
	uint64_t len = readVar();
	out.clear();
	out.reserve((size_t)len);
	for (uint64_t i = 0; i < len; i++) {
		out.push_back((char)readByte());
	}
}

// Inspect just the header of a .logicdata file. Auto-detects between
// our writer's variant (no version byte, BLOCK=0x15, numChannels hard-coded
// to 6) and the real Saleae / Java LogicdataStreamFile variant (leading
// version=0x13 byte, BLOCK=0x18, numChannels declared in header).
static LogicDataHeaderInfo inspectHeaderImpl() {
	LogicDataHeaderInfo info;
	uint8_t magic = readByte();
	if (magic != 0x7f) throw std::runtime_error("bad magic");

	// After magic both variants emit a single var. For the real Saleae /
	// Java writer this is `version=0x13`; for our C++ writer it's an extra
	// `write(strlen(title))` that is otherwise unused (writeString below
	// writes the length again). Distinguish by value: 0x13 = real format.
	uint64_t firstVar = readVar();
	info.hasVersionByte = (firstVar == 0x13);

	// Title: length (var) followed by raw bytes. NOTE: for our writer this is
	// `writeString(title)` (var-length-prefixed bytes); for the real format
	// the layout is also `<var:titleLen> <raw bytes>`. Identical on the wire.
	uint64_t titleLen = readVar();
	if (titleLen > 256) throw std::runtime_error("implausible title length");
	info.title.resize((size_t)titleLen);
	for (uint64_t k = 0; k < titleLen; k++) {
		info.title[(size_t)k] = (char)readByte();
	}

	info.blockMarker = (int)readVar(); // BLOCK (0x15 ours / 0x18 real)
	skipVar(); // SUB
	info.frequency = readVar();
	skipVar(); // 0
	skipVar(); // reservedDurationInSamples
	skipVar(); // frequency / frequencyDiv
	skipVars(2);
	info.numChannels = (int)readVar();
	if (info.numChannels <= 0 || info.numChannels > 64) {
		throw std::runtime_error("implausible numChannels");
	}

	skipVar(); // BLOCK
	skipVar(); // 0

	skipVar(); // BLOCK
	for (int i = 0; i < info.numChannels; i++) {
		skipVar(); skipVar(); skipVar(); // writeId(i, 1)
	}
	skipVar(); // 0

	skipVar(); // BLOCK
	skipVar(); skipVar(); skipVar(); // writeId(0, 0)
	skipVar();
	skipVar();

	// channelDataHeader prefix.
	skipVar(); // BLOCK
	skipVar(); // scaledDurationInSamples
	skipVars(5);
	skipVar(); // numChannels
	skipVars(3);
	skipVar(); skipVar(); skipVar(); // writeId(0, 1)
	skipVar();

	skipVar(); // BLOCK
	skipVars(3);

	// Per-channel headers: 0xff var, ch var, name string, then variant-
	// specific padding/doubles + optional id+flags. We only need the name,
	// then follow the writer's per-channel header skip recipe.
	for (int ch = 0; ch < info.numChannels; ch++) {
		skipVar();            // 0xff
		skipVar();            // ch
		uint64_t nameLen = readVar();
		std::string name;
		name.resize((size_t)nameLen);
		for (uint64_t k = 0; k < nameLen; k++) {
			name[(size_t)k] = (char)readByte();
		}
		info.channelNames.push_back(name);
		skipVars(2);          // padding 0, 0
		skipDouble();         // 1.0
		skipVar();            // 0
		skipDouble();         // 0.0
		skipVar();            // 1 or 2
		skipDouble();         // 0.0 or 1.0
		if (ch == info.numChannels - 1) {
			skipVar();        // 0
		} else {
			skipVar(); skipVar(); skipVar();   // writeId(1+ch, 1)
			skipVar(); skipVar(); skipVar();   // CHANNEL_FLAGS bytes (3 vars)
		}
	}
	return info;
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

namespace {

// Generalized inverse of writeChannelData(): identical in structure to the
// 6-channel C++ writer AND the Java LogicdataStreamFile writer (modulo the
// BLOCK marker numeric value, which is not validated by the reader -- both
// 0x15 and 0x18 round-trip through skipVar()).
static void readChannelDataN(int ch, int nch, ChannelEdges& out) {
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
		(void)nch;
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

	out.timestamps.reserve(deltas.size());
	uint32_t ts = 0;
	for (size_t i = 0; i < deltas.size(); i++) {
		ts += deltas[i];
		out.timestamps.push_back(ts);
		out.states.push_back(newStates[i]);
	}
	if (!out.states.empty()) {
		out.initialState = out.states[0] ? 0 : 1;
	} else {
		out.initialState = out.lastState;
	}
}

// Read the full header + per-channel data block header for the generic case
// (any numChannels, any BLOCK marker). The file position is left right at
// the start of the first channel's CHANNEL_BLOCK record.
[[maybe_unused]] static void readChannelDataHeaderN(int nch) {
	skipVar(); // BLOCK (scaledDurationInSamples block)
	skipVar(); // scaledDurationInSamples
	skipVars(5);
	skipVar(); // numChannels
	skipVars(3);
	skipId();
	skipVar();

	skipVar(); // BLOCK
	skipVars(3);

	for (int i = 0; i < nch; i++) {
		// readChannelHeader-equivalent: works for both writers since the
		// "next pointer" tail is structurally identical (writeId + 3 vars).
		skipVar(); // 0xff
		skipVar(); // ch
		skipString(); // name
		skipVars(2);
		skipDouble();
		skipVar();
		skipDouble();
		skipVar();
		skipDouble();
		if (i == nch - 1) {
			skipVar();
		} else {
			skipId();
			for (int k = 0; k < 3; k++) skipVar();
		}
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

// Read+skip the variant-agnostic file header up to (but not including) the
// channelDataHeader. Returns numChannels and stores channel names.
static int readHeaderN(LogicDataHeaderInfo& info) {
	uint8_t magic = readByte();
	if (magic != 0x7f) throw std::runtime_error("bad magic");

	uint64_t firstVar = readVar();
	info.hasVersionByte = (firstVar == 0x13);

	uint64_t titleLen = readVar();
	if (titleLen > 256) throw std::runtime_error("implausible title length");
	info.title.resize((size_t)titleLen);
	for (uint64_t k = 0; k < titleLen; k++) {
		info.title[(size_t)k] = (char)readByte();
	}

	info.blockMarker = (int)readVar(); // BLOCK
	skipVar(); // SUB
	info.frequency = readVar();
	skipVar(); // 0
	skipVar(); // reservedDurationInSamples
	skipVar(); // freq/div
	skipVars(2);
	info.numChannels = (int)readVar();
	if (info.numChannels <= 0 || info.numChannels > 64) {
		throw std::runtime_error("implausible numChannels");
	}

	skipVar(); // BLOCK
	skipVar(); // 0

	skipVar(); // BLOCK
	for (int i = 0; i < info.numChannels; i++) {
		skipId();
	}
	skipVar(); // 0

	skipVar(); // BLOCK
	skipId();
	skipVar();
	skipVar();

	return info.numChannels;
}

// Decode one channel's pre-edge prologue (everything between CHANNEL_BLOCK
// and the numEdges triplet) and return numEdges + useLongDeltas + empty flag.
// Layout has subtle differences between our C++ writer (BLOCK=0x15) and real
// Saleae files (BLOCK=0x18). The known divergences for the real format:
//   * ch == 0, useLongDeltas: 0 + BLOCK + 11 zeros + BLOCK (no extra
//     BLOCK + 6 zeros block that our writer emits).
//   * ch != 0, useLongDeltas: write(0, 13) (vs our writer's 10 + 5 = 15).
// The short-deltas paths and ch == 0 short-deltas match both writers.
static void readChannelPrologueGeneric(int ch, bool isReal,
		uint64_t& numEdges, bool& useLongDeltas, bool& empty) {
	skipVar(); // CHANNEL_BLOCK (0x16)
	if (ch == 0) {
		skipVar(); // SUB
		skipVar(); // BLOCK
	}
	skipVar(); // ch + 1
	skipVar(); // 0
	skipVar(); // realDurationInSamples
	skipVar(); // 1
	skipVar(); // lastRecord
	skipVar(); // numSamplesLeft
	skipVar(); // chLastState
	uint64_t chFlag = readVar();
	empty = (chFlag == FLAG_EMPTY);
	useLongDeltas = (chFlag == FLAG_NOTEMPTY_LONG);

	if (ch == 0 && !isReal) {
		// Our C++ writer's path for ch == 0.
		skipVar(); // 0
		skipVar(); // BLOCK
		skipVars(11);
		if (useLongDeltas) {
			skipVar(); // BLOCK
			skipVars(6);
		}
		skipVar(); // BLOCK
	} else if (ch == 0 && isReal) {
		// Real-Saleae ch == 0 has a structure of:
		//   0, BLOCK, <variable run of zeros>, [optional BLOCK + zeros for
		//   useLongDeltas in some files], numEdges-triplet.
		// Skip the leading "0, BLOCK" pair (two vars), then dynamically
		// scan zero bytes until a non-zero length-prefix byte. If we hit
		// another BLOCK marker (0x18 var = bytes "01 18") in the middle,
		// consume it and keep scanning.
		skipVar(); // 0
		skipVar(); // BLOCK
		if (empty) {
			// Empty ch == 0 case never observed; fall back to writer recipe.
			skipVars(useLongDeltas ? 18 : 11);
			skipVar(); // BLOCK
		} else {
			while (true) {
				int c = fgetc(g_fp);
				if (c == EOF) throw std::runtime_error("EOF in ch0 prologue");
				if (c == 0x00) continue;
				if (c == 0x01) {
					// Could be a BLOCK marker (01 18) or numEdges length=1.
					int c2 = fgetc(g_fp);
					if (c2 == EOF) throw std::runtime_error("EOF in ch0 prologue");
					if (c2 == 0x18) {
						// Inline BLOCK marker -- skip and keep scanning.
						continue;
					}
					// numEdges length-prefix=1, value byte = c2. Push both
					// back so readVar() below sees them.
					ungetc(c2, g_fp);
					ungetc(c, g_fp);
					break;
				}
				// Any other non-zero byte starts the numEdges var length.
				ungetc(c, g_fp);
				break;
			}
		}
	} else if (isReal) {
		// In real-Saleae files the number of zero-vars between chFlag and
		// the numEdges triplet varies for non-empty channels (observed: 5,
		// 10, 13 zeros). For non-empty channels we robustly scan over any
		// run of zero bytes, stopping at the first non-zero byte (which is
		// the length prefix of numEdges -- always >= 1 since the channel
		// has edges). For empty channels (chFlag == FLAG_EMPTY) numEdges
		// itself is 0, so the writer emits a zero byte as the length tag
		// for numEdges -- the dynamic scan cannot tell that zero apart
		// from a padding zero. Use a fixed skip of 10 vars in that case
		// (matches every observed empty real-Saleae channel so far).
		if (empty) {
			skipVars(10);
		} else {
			while (true) {
				int c = fgetc(g_fp);
				if (c == EOF) throw std::runtime_error("EOF in channel prologue");
				if (c != 0x00) {
					ungetc(c, g_fp);
					break;
				}
			}
		}
	} else {
		if (useLongDeltas) {
			skipVars(15);
		} else {
			skipVars(10);
		}
	}

	numEdges = readVar();
	skipVar(); // 0
	skipVar(); // numEdges
	skipVar(); // 0
	skipVar(); // numEdges
}

// Skip raw edge bytes: numEdges * (useLongDeltas ? 4 : 2) + 1 terminator.
static void skipEdgeBytesAndCollect(uint64_t numEdges, bool useLongDeltas,
		std::vector<uint32_t>& deltas, std::vector<int>& newStates) {
	for (uint64_t e = 0; e < numEdges; e++) {
		uint8_t b0 = readByte();
		uint8_t b1 = readByte();
		uint32_t raw;
		if (useLongDeltas) {
			uint8_t b2 = readByte();
			uint8_t b3 = readByte();
			raw = (uint32_t)b0 | ((uint32_t)b1 << 8)
				| ((uint32_t)b2 << 16) | ((uint32_t)b3 << 24);
		} else {
			raw = (uint32_t)b0 | ((uint32_t)b1 << 8);
		}
		int newState = 1;
		uint32_t delta;
		if (useLongDeltas) {
			if (raw & SIGN_FLAG) { newState = 0; delta = raw & ~SIGN_FLAG; }
			else { newState = 1; delta = raw; }
		} else {
			if (raw & 0x8000) { newState = 0; delta = raw & 0x7fff; }
			else { newState = 1; delta = raw; }
		}
		deltas.push_back(delta);
		newStates.push_back(newState);
	}
	(void)readByte(); // 0x00 terminator
}

// Scan the file forward until we find the next channel-block marker (the
// var-encoded value 0x16, which on the wire is the two bytes 0x01 0x16).
// To avoid matching false positives inside post-edge record qwords, we
// additionally require the bytes that follow the marker to look like a
// plausible channel header: `01 <expectedCh+1> 00 03 ...` (ch+1 var-encoded
// as a 1-byte value, then a 0 var, then a 3-byte length prefix introducing
// realDurationInSamples).
//
// Returns true if found; leaves g_fp positioned right before the matched
// 0x01 0x16 pair. If we hit EOF first, returns false.
static bool scanForNextChannelBlock(int expectedChPlus1) {
	int prev = -1;
	while (true) {
		long here = ftell(g_fp);
		int c = fgetc(g_fp);
		if (c == EOF) return false;
		if (prev == 0x01 && c == 0x16) {
			// Peek next bytes; require: 01 <expectedChPlus1> 00 03
			unsigned char peek[4];
			size_t got = fread(peek, 1, 4, g_fp);
			fseek(g_fp, here + 1 - (long)got, SEEK_CUR);
			// Actually simpler: just fseek back to (here + 1) which is the
			// byte right after the 0x16 byte we just consumed.
			fseek(g_fp, here + 1, SEEK_SET);
			if (got == 4
				&& peek[0] == 0x01
				&& peek[1] == (unsigned char)expectedChPlus1
				&& peek[2] == 0x00
				&& (peek[3] == 0x03 || peek[3] == 0x04)) {
				// Rewind to just before 0x01 0x16 so caller's readVar() sees it.
				fseek(g_fp, here - 1, SEEK_SET);
				return true;
			}
			// Not a real header; keep scanning from after the 0x16.
			prev = c;
			continue;
		}
		prev = c;
	}
}

} // namespace

LogicDataFull readLogicDataFull(const char* fileName) {
	g_fp = fopen(fileName, "rb");
	if (!g_fp) {
		throw std::runtime_error(std::string("cannot open ") + fileName);
	}
	LogicDataFull result;
	try {
		int nch = readHeaderN(result.header);

		// Now re-read the per-channel-header block to capture channel names.
		// readChannelDataHeaderN above already skips them; we need a variant
		// that captures names. Do it inline here (replacing the per-channel
		// header loop from readChannelDataHeaderN).
		skipVar(); // BLOCK (scaledDurationInSamples)
		skipVar(); // scaledDurationInSamples
		skipVars(5);
		skipVar(); // numChannels
		skipVars(3);
		skipId();
		skipVar();

		skipVar(); // BLOCK
		skipVars(3);

		result.channels.resize(nch);
		for (int i = 0; i < nch; i++) {
			skipVar(); // 0xff
			skipVar(); // ch
			uint64_t nameLen = readVar();
			std::string name((size_t)nameLen, '\0');
			for (uint64_t k = 0; k < nameLen; k++) {
				name[(size_t)k] = (char)readByte();
			}
			result.channels[i].name = name;
			skipVars(2);
			skipDouble();
			skipVar();
			skipDouble();
			skipVar();
			skipDouble();
			if (i == nch - 1) {
				skipVar();
			} else {
				skipId();
				for (int k = 0; k < 3; k++) skipVar();
			}
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
		skipVars(4);
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

		// Read per-channel data blocks. We use a robust strategy that
		// decodes only the pre-edge prologue + edge bytes exactly, then
		// scans forward byte-by-byte for the next channel-block marker
		// (var-encoded 0x16, i.e. the byte pair 0x01 0x16). This avoids
		// having to precisely decode the post-edge sample-index records
		// whose layout differs between writers and across Saleae versions.
		const bool isReal = result.header.hasVersionByte;
		const bool debugLog = (std::getenv("LOGICDATA_DEBUG") != nullptr);
		for (int ch = 0; ch < nch; ch++) {
			if (debugLog) {
				std::fprintf(stderr, "ch%d prologue @offset=0x%lx\n",
					ch, (long)ftell(g_fp));
			}
			uint64_t numEdges = 0;
			bool useLongDeltas = false;
			bool empty = false;
			readChannelPrologueGeneric(ch, isReal, numEdges, useLongDeltas, empty);
			if (debugLog) {
				std::fprintf(stderr, "  ch%d numEdges=%llu useLongDeltas=%d empty=%d post-prologue@0x%lx\n",
					ch, (unsigned long long)numEdges, useLongDeltas?1:0, empty?1:0,
					(long)ftell(g_fp));
			}

			std::vector<uint32_t> deltas;
			std::vector<int> newStates;
			skipEdgeBytesAndCollect(numEdges, useLongDeltas, deltas, newStates);

			// Convert deltas to absolute timestamps.
			uint32_t ts = 0;
			result.channels[ch].timestamps.reserve(deltas.size());
			for (size_t i = 0; i < deltas.size(); i++) {
				ts += deltas[i];
				result.channels[ch].timestamps.push_back(ts);
				result.channels[ch].states.push_back(newStates[i]);
			}
			if (!result.channels[ch].states.empty()) {
				result.channels[ch].initialState =
					result.channels[ch].states[0] ? 0 : 1;
			}

			// Skip the post-edge sample-index records by scanning for the
			// next channel block (matched by the next channel's expected
			// ch+1 field). For the last channel there is no next marker --
			// just leave the file pointer wherever it is.
			if (ch < nch - 1) {
				int nextChPlus1 = ch + 2; // next channel's "ch+1" value
				if (!scanForNextChannelBlock(nextChPlus1)) {
					throw std::runtime_error(
						"unexpected EOF while looking for next channel block");
				}
			}
			(void)empty;
		}
	} catch (...) {
		fclose(g_fp);
		g_fp = nullptr;
		throw;
	}
	fclose(g_fp);
	g_fp = nullptr;
	return result;
}

void writeLogicDataGenericCsv(const char* csvFileName, const LogicDataFull& data) {
	std::FILE* f = std::fopen(csvFileName, "w");
	if (!f) {
		throw std::runtime_error(std::string("cannot open ") + csvFileName + " for writing");
	}
	// Header row
	std::fprintf(f, "Time[s]");
	for (const auto& ch : data.channels) {
		std::fprintf(f, ", %s", ch.name.c_str());
	}
	std::fprintf(f, "\n");

	// Merge all edges by timestamp.
	struct Edge { uint32_t ts; int ch; int newState; };
	std::vector<Edge> all;
	for (size_t ci = 0; ci < data.channels.size(); ci++) {
		const auto& c = data.channels[ci];
		for (size_t i = 0; i < c.timestamps.size(); i++) {
			all.push_back({c.timestamps[i], (int)ci, c.states[i]});
		}
	}
	std::stable_sort(all.begin(), all.end(),
		[](const Edge& a, const Edge& b){ return a.ts < b.ts; });

	std::vector<int> curState(data.channels.size(), 0);
	for (size_t i = 0; i < data.channels.size(); i++) {
		curState[i] = data.channels[i].initialState;
	}

	double freq = (double)(data.header.frequency ? data.header.frequency : 1);

	// Emit initial row at t=0 if there is any edge data.
	if (!all.empty()) {
		std::fprintf(f, "%.9f", 0.0);
		for (size_t i = 0; i < curState.size(); i++) {
			std::fprintf(f, ", %d", curState[i] ? 1 : 0);
		}
		std::fprintf(f, "\n");
	}

	size_t i = 0;
	while (i < all.size()) {
		uint32_t ts = all[i].ts;
		while (i < all.size() && all[i].ts == ts) {
			curState[all[i].ch] = all[i].newState;
			i++;
		}
		double t = (double)ts / freq;
		std::fprintf(f, "%.9f", t);
		for (size_t k = 0; k < curState.size(); k++) {
			std::fprintf(f, ", %d", curState[k] ? 1 : 0);
		}
		std::fprintf(f, "\n");
	}

	std::fclose(f);
}

LogicDataHeaderInfo inspectLogicDataHeader(const char* fileName) {
	g_fp = fopen(fileName, "rb");
	if (!g_fp) {
		throw std::runtime_error(std::string("cannot open ") + fileName);
	}
	LogicDataHeaderInfo info;
	try {
		info = inspectHeaderImpl();
	} catch (...) {
		fclose(g_fp);
		g_fp = nullptr;
		throw;
	}
	fclose(g_fp);
	g_fp = nullptr;
	return info;
}
