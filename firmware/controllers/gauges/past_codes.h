#pragma once

#include "obd_error_codes.h"
#include <cstddef>

// First occurrences, oldest first. Zero timestamp means no valid RTC.
class PastCodeHistory {
public:
	static constexpr size_t Capacity = 8;
	static constexpr size_t CsvBufferSize = 160;
	struct Entry {
		ObdCode code = ObdCode::None;
		uint32_t timestamp = 0;
	};
	struct Snapshot {
		Entry entries[Capacity]{};
		size_t count = 0;
		uint32_t revision = 0;
	};

	bool contains(ObdCode code) const;
	bool add(ObdCode code, uint32_t timestamp);
	// Merge disk history with codes already seen this boot. Invalid files are
	// rejected without changing memory, so an unreadable history is not overwritten.
	bool load(const char* csv, size_t size);
	bool snapshotIfDirty(Snapshot& snapshot) const;
	Snapshot snapshot() const;
	void markSaved(const Snapshot& snapshot);
	static size_t format(const Snapshot& snapshot, char (&csv)[CsvBufferSize]);

private:
	bool addLocked(ObdCode code, uint32_t timestamp);
	Snapshot m_state;
	uint32_t m_savedRevision = 0;
};
