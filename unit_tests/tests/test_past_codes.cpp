#include "pch.h"
#include "past_codes.h"
#include "malfunction_central.h"
#include <cstring>

static ObdCode code(unsigned value) {
	return static_cast<ObdCode>(value);
}

TEST(PastCodes, UniqueFirstSeenAndOldestEviction) {
	PastCodeHistory history;
	PastCodeHistory::Snapshot snapshot;
	EXPECT_FALSE(history.snapshotIfDirty(snapshot));
	EXPECT_FALSE(history.add(ObdCode::None, 100));
	for (unsigned i = 1; i <= 8; i++) {
		EXPECT_TRUE(history.add(code(i), 100 + i));
	}
	ASSERT_TRUE(history.snapshotIfDirty(snapshot));
	history.markSaved(snapshot);
	EXPECT_FALSE(history.add(code(1), 999));
	EXPECT_FALSE(history.snapshotIfDirty(snapshot));
	EXPECT_EQ(101u, history.snapshot().entries[0].timestamp);
	EXPECT_TRUE(history.add(code(9), 109));
	ASSERT_TRUE(history.snapshotIfDirty(snapshot));
	ASSERT_EQ(8u, snapshot.count);
	for (unsigned i = 0; i < 8; i++) {
		EXPECT_EQ(code(i + 2), snapshot.entries[i].code);
		EXPECT_EQ(102u + i, snapshot.entries[i].timestamp);
	}
}

TEST(PastCodes, CsvRoundTripAndUnknownRtc) {
	PastCodeHistory history;
	history.add(code(123), 1791900000);
	history.add(code(456), 0);
	// Ordering is by occurrence, even when the RTC is corrected backwards.
	history.add(code(65535), 1600000000);
	char csv[PastCodeHistory::CsvBufferSize]{};
	const size_t size = PastCodeHistory::format(history.snapshot(), csv);
	EXPECT_STREQ("123,1791900000\n456,\n65535,1600000000\n", csv);
	PastCodeHistory rebooted;
	ASSERT_TRUE(rebooted.load(csv, size));
	PastCodeHistory::Snapshot snapshot;
	EXPECT_FALSE(rebooted.snapshotIfDirty(snapshot));
	EXPECT_FALSE(rebooted.add(code(456), 1791900010));
	snapshot = rebooted.snapshot();
	ASSERT_EQ(3u, snapshot.count);
	EXPECT_EQ(code(456), snapshot.entries[1].code);
	EXPECT_EQ(0u, snapshot.entries[1].timestamp);
	EXPECT_EQ(code(65535), snapshot.entries[2].code);
}

TEST(PastCodes, StartupMergeRetainsDiskTimestampsAndBootWarnings) {
	PastCodeHistory history;
	history.add(code(2), 200);
	history.add(code(3), 201);
	const char csv[] = "1,100\r\n2,101\r\n";
	ASSERT_TRUE(history.load(csv, strlen(csv)));
	PastCodeHistory::Snapshot snapshot;
	ASSERT_TRUE(history.snapshotIfDirty(snapshot));
	ASSERT_EQ(3u, snapshot.count);
	EXPECT_EQ(code(1), snapshot.entries[0].code);
	EXPECT_EQ(code(2), snapshot.entries[1].code);
	EXPECT_EQ(101u, snapshot.entries[1].timestamp);
	EXPECT_EQ(code(3), snapshot.entries[2].code);
	EXPECT_EQ(201u, snapshot.entries[2].timestamp);
}

TEST(PastCodes, StartupDuplicatesDoNotCauseWritesOrResurrectEvictedDiskCodes) {
	const char csv[] = "1,1\n2,2\n3,3\n4,4\n5,5\n6,6\n7,7\n8,8\n";
	PastCodeHistory history;
	history.add(code(8), 100);
	ASSERT_TRUE(history.load(csv, strlen(csv)));
	PastCodeHistory::Snapshot snapshot;
	EXPECT_FALSE(history.snapshotIfDirty(snapshot));

	PastCodeHistory startup;
	startup.add(code(9), 101);
	startup.add(code(1), 102);
	ASSERT_TRUE(startup.load(csv, strlen(csv)));
	snapshot = startup.snapshot();
	ASSERT_EQ(8u, snapshot.count);
	EXPECT_EQ(code(2), snapshot.entries[0].code);
	EXPECT_EQ(code(9), snapshot.entries[7].code);
}

TEST(PastCodes, WritesAcknowledgeOnlyTheSavedSnapshot) {
	PastCodeHistory history;
	history.add(code(1), 100);
	PastCodeHistory::Snapshot first, second;
	ASSERT_TRUE(history.snapshotIfDirty(first));
	// An unsuccessful write is not acknowledged and must remain pending.
	ASSERT_TRUE(history.snapshotIfDirty(second));
	EXPECT_EQ(first.revision, second.revision);
	history.add(code(2), 101);
	history.markSaved(first);
	ASSERT_TRUE(history.snapshotIfDirty(second));
	EXPECT_EQ(2u, second.count);
	history.markSaved(second);
	EXPECT_FALSE(history.snapshotIfDirty(first));
}

TEST(PastCodes, EmptyMissingFilePreservesBootWarnings) {
	PastCodeHistory history;
	ASSERT_TRUE(history.load("", 0));
	PastCodeHistory::Snapshot snapshot;
	EXPECT_FALSE(history.snapshotIfDirty(snapshot));
	history.add(code(1), 0);
	ASSERT_TRUE(history.load("", 0));
	ASSERT_TRUE(history.snapshotIfDirty(snapshot));
	EXPECT_EQ(1u, snapshot.count);
}

TEST(PastCodes, InvalidCsvDoesNotModifyHistory) {
	const char* invalid[] = {
		"0,123\n", "65536,123\n", "1,4294967296\n", "1,-1\n",
		"1,123,4\n", "x,123\n", "1,123\n1,456\n", "1,123\n\n",
		"1,1\n2,2\n3,3\n4,4\n5,5\n6,6\n7,7\n8,8\n9,9\n",
		"1,999999999999999999999999\n"
	};
	PastCodeHistory history;
	history.add(code(42), 123);
	for (const char* csv : invalid) {
		EXPECT_FALSE(history.load(csv, strlen(csv))) << csv;
		const auto snapshot = history.snapshot();
		ASSERT_EQ(1u, snapshot.count);
		EXPECT_EQ(code(42), snapshot.entries[0].code);
		EXPECT_EQ(123u, snapshot.entries[0].timestamp);
	}
}

TEST(PastCodes, MaximumValuesAndOptionalFinalNewline) {
	PastCodeHistory history;
	const char csv[] = "65535,4294967295\r\n123";
	ASSERT_TRUE(history.load(csv, strlen(csv)));
	const auto snapshot = history.snapshot();
	ASSERT_EQ(2u, snapshot.count);
	EXPECT_EQ(UINT32_MAX, snapshot.entries[0].timestamp);
	EXPECT_EQ(0u, snapshot.entries[1].timestamp);
	char output[PastCodeHistory::CsvBufferSize]{};
	for (unsigned i = 1; i <= 8; i++) {
		history.add(code(65535 - i), UINT32_MAX);
	}
	const size_t size = PastCodeHistory::format(history.snapshot(), output);
	EXPECT_EQ(8u * 17, size);
	PastCodeHistory rebooted;
	EXPECT_TRUE(rebooted.load(output, size));
}

TEST(PastCodes, ActiveWarningClearAndCapacityDoNotLoseHistory) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	auto& warnings = engine->engineState.warnings;
	for (unsigned i = 1; i <= MAX_ERROR_CODES_COUNT + 1; i++) {
		warnings.addWarningCode(code(i));
	}
	const auto before = warnings.pastCodes.snapshot();
	ASSERT_EQ(8u, before.count);
	EXPECT_EQ(code(MAX_ERROR_CODES_COUNT + 1), before.entries[7].code);
	warnings.removeErrorCode(code(MAX_ERROR_CODES_COUNT));
	warnings.clear();
	const auto after = warnings.pastCodes.snapshot();
	EXPECT_EQ(before.count, after.count);
	EXPECT_EQ(before.revision, after.revision);
}
