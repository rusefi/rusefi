#include "pch.h"
#include "past_codes.h"
#include <cstdio>

bool PastCodeHistory::contains(ObdCode code) const {
	chibios_rt::CriticalSectionLocker lock;
	for (size_t i = 0; i < m_state.count; i++) {
		if (m_state.entries[i].code == code) {
			return true;
		}
	}
	return false;
}

bool PastCodeHistory::addLocked(ObdCode code, uint32_t timestamp) {
	if (code == ObdCode::None) {
		return false;
	}
	for (size_t i = 0; i < m_state.count; i++) {
		if (m_state.entries[i].code == code) {
			return false;
		}
	}
	if (m_state.count == Capacity) {
		for (size_t i = 1; i < Capacity; i++) {
			m_state.entries[i - 1] = m_state.entries[i];
		}
		m_state.count--;
	}
	m_state.entries[m_state.count++] = {code, timestamp};
	m_state.revision++;
	return true;
}

bool PastCodeHistory::add(ObdCode code, uint32_t timestamp) {
	chibios_rt::CriticalSectionLocker lock;
	return addLocked(code, timestamp);
}

static bool parseNumber(const char*& cursor, const char* end, uint32_t limit, uint32_t& value) {
	const char* start = cursor;
	value = 0;
	while (cursor != end && *cursor >= '0' && *cursor <= '9') {
		const uint32_t digit = *cursor++ - '0';
		if (value > (limit - digit) / 10) {
			return false;
		}
		value = value * 10 + digit;
	}
	return cursor != start;
}

bool PastCodeHistory::load(const char* csv, size_t size) {
	Snapshot loaded;
	const char* cursor = csv;
	const char* end = csv + size;
	while (cursor != end) {
		uint32_t code, timestamp = 0;
		if (loaded.count == Capacity || !parseNumber(cursor, end, UINT16_MAX, code) || code == 0) {
			return false;
		}
		if (cursor != end && *cursor == ',') {
			cursor++;
			if (cursor != end && *cursor >= '0' && *cursor <= '9'
				&& !parseNumber(cursor, end, UINT32_MAX, timestamp)) {
				return false;
			}
		}
		if (cursor != end && *cursor == '\r') {
			cursor++;
		}
		if (cursor != end && *cursor++ != '\n') {
			return false;
		}
		for (size_t i = 0; i < loaded.count; i++) {
			if (loaded.entries[i].code == static_cast<ObdCode>(code)) {
				return false;
			}
		}
		loaded.entries[loaded.count++] = {static_cast<ObdCode>(code), timestamp};
	}

	chibios_rt::CriticalSectionLocker lock;
	const Snapshot pending = m_state;
	m_state = loaded;
	m_savedRevision = m_state.revision;
	for (size_t i = 0; i < pending.count; i++) {
		bool alreadyOnDisk = false;
		for (size_t j = 0; j < loaded.count; j++) {
			alreadyOnDisk |= loaded.entries[j].code == pending.entries[i].code;
		}
		if (!alreadyOnDisk) {
			addLocked(pending.entries[i].code, pending.entries[i].timestamp);
		}
	}
	return true;
}

PastCodeHistory::Snapshot PastCodeHistory::snapshot() const {
	chibios_rt::CriticalSectionLocker lock;
	return m_state;
}

bool PastCodeHistory::snapshotIfDirty(Snapshot& snapshot) const {
	chibios_rt::CriticalSectionLocker lock;
	if (m_state.revision == m_savedRevision) {
		return false;
	}
	snapshot = m_state;
	return true;
}

void PastCodeHistory::markSaved(const Snapshot& snapshot) {
	chibios_rt::CriticalSectionLocker lock;
	// Codes arriving during SD I/O must still cause another write.
	m_savedRevision = snapshot.revision;
}

size_t PastCodeHistory::format(const Snapshot& snapshot, char (&csv)[CsvBufferSize]) {
	size_t size = 0;
	for (size_t i = 0; i < snapshot.count; i++) {
		const auto& entry = snapshot.entries[i];
		if (entry.timestamp) {
			size += snprintf(csv + size, CsvBufferSize - size, "%u,%lu\n",
				static_cast<unsigned>(entry.code), static_cast<unsigned long>(entry.timestamp));
		} else {
			size += snprintf(csv + size, CsvBufferSize - size, "%u,\n", static_cast<unsigned>(entry.code));
		}
	}
	return size;
}
