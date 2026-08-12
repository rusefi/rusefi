#include "pch.h"

#include "tooth_logger.h"
#include "board_overrides.h"

#include <string>

namespace {

class StringWriter : public Writer {
public:
	std::string data;

	size_t write(const char* buffer, size_t count) override {
		data.append(buffer, count);
		return count;
	}

	size_t flush() override {
		return 0;
	}
};

// Clears the board CSV hooks on scope exit so a failing test cannot leak them
// into later tests.
struct CsvBoardHookGuard {
	~CsvBoardHookGuard() {
		custom_board_toothLogCsvHeader = std::nullopt;
		custom_board_toothLogCsvLine = std::nullopt;
		custom_board_toothLogSample = std::nullopt;
	}
};

} // namespace

TEST(ToothLogger, WriteCsvHeader) {
	StringWriter w;
	ToothLoggerWriteCsvHeader(w);

	EXPECT_EQ(w.data,
		"Time[s], Primary, Cam 1, Cam 2, Cam 3, Cam 4, Sync, TDC, Coils, Injectors, ACR, VBatt, ET, InstantMAP, TPS\r\n");
}

TEST(ToothLogger, WriteCsvRows) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	// Live sensor values at CSV-write (flush) time. These must NOT appear in
	// the output - rows carry the per-event sensorSnapshot values instead.
	Sensor::setMockValue(SensorType::BatteryVoltage, 99.99f);
	Sensor::setMockValue(SensorType::Clt, 88.88f);
	Sensor::setMockValue(SensorType::Tps1, 77.77f);

	CompositeBuffer buf{};
	buf.nextIdx = 2;
	buf.startTime.reset(0);

	buf.sensorSnapshot[0].vbatt = 12.34f;
	buf.sensorSnapshot[0].et = 56.78f;
	buf.sensorSnapshot[0].instantMap = 101.25f;
	buf.sensorSnapshot[0].tps = 43.21f;

	buf.sensorSnapshot[1].vbatt = 13.5f;
	buf.sensorSnapshot[1].et = 57.25f;
	buf.sensorSnapshot[1].instantMap = 45.75f;
	buf.sensorSnapshot[1].tps = 100.0f;

	// Row 0: timestamp 1.000002s, all flags zero
	{
		composite_logger_s c{};
		c.timestamp = 1000002; // microseconds
		c.priLevel = true;
		c.cam1 = false;
		c.sync = true;
		c.tdc = false;
		c.coil = 3;
		c.injector = 5;
		c.acr = true;
		// On-the-wire format is byte-reversed; ToothLoggerWriteBufferCsv swaps it back.
		buf.buffer[0].x = SWAP_UINT64(c.x);
	}

	// Row 1
	{
		composite_logger_s c{};
		c.timestamp = 2000500;
		c.priLevel = false;
		c.cam1 = true;
		c.cam2 = true;
		c.sync = false;
		c.tdc = true;
		c.coil = 0;
		c.injector = 0;
		c.acr = false;
		buf.buffer[1].x = SWAP_UINT64(c.x);
	}

	StringWriter w;
	int total = ToothLoggerWriteBufferCsv(w, &buf);

	EXPECT_GT(total, 0);
	EXPECT_EQ((size_t)total, w.data.size());

	// Each row contains the timestamp prefix and ends with ITS OWN snapshot
	// values (formatted with %.2f), not the flush-time sensor readings.
	size_t row1 = w.data.find("\r\n");
	ASSERT_NE(row1, std::string::npos);
	EXPECT_NE(w.data.find("1.000002,"), std::string::npos);
	EXPECT_NE(w.data.find("2.000500,", row1), std::string::npos);

	EXPECT_LT(w.data.find("12.34, 56.78, 101.25, 43.21"), row1);
	EXPECT_NE(w.data.find("13.50, 57.25, 45.75, 100.00", row1), std::string::npos);

	// flush-time (live) sensor values must not leak into any row
	EXPECT_EQ(w.data.find("99.99"), std::string::npos);
	EXPECT_EQ(w.data.find("88.88"), std::string::npos);
	EXPECT_EQ(w.data.find("77.77"), std::string::npos);

	// Number of CSV rows (CRLF-terminated) must match the buffer entries.
	size_t crlfCount = 0;
	for (size_t i = 0; i + 1 < w.data.size(); i++) {
		if (w.data[i] == '\r' && w.data[i + 1] == '\n') {
			crlfCount++;
		}
	}
	EXPECT_EQ(crlfCount, 2u);
}

// Entry timestamps are stored as microsecond offsets from the buffer's startTime;
// the CSV writer must recover absolute time as startTime + offset.
TEST(ToothLogger, WriteCsvRowsRecoverAbsoluteTime) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	CompositeBuffer buf{};
	buf.nextIdx = 3;
	// Buffer acquired 100 seconds after boot
	buf.startTime.reset(US2NT(100'000'000));

	// Offsets: first entry in the buffer, a mid-buffer entry, and one just
	// short of the 5 second buffer flush period (the largest possible offset).
	const uint32_t offsetsUs[] = { 0, 1'000'002, 4'999'999 };

	for (size_t i = 0; i < efi::size(offsetsUs); i++) {
		composite_logger_s c{};
		c.timestamp = offsetsUs[i];
		buf.buffer[i].x = SWAP_UINT64(c.x);
	}

	StringWriter w;
	int total = ToothLoggerWriteBufferCsv(w, &buf);
	EXPECT_GT(total, 0);

	// Split rows and check the absolute timestamp prefix of each
	const char* expectedPrefixes[] = { "100.000000, ", "101.000002, ", "104.999999, " };

	size_t rowStart = 0;
	for (size_t i = 0; i < efi::size(expectedPrefixes); i++) {
		ASSERT_LT(rowStart, w.data.size());
		EXPECT_EQ(w.data.compare(rowStart, strlen(expectedPrefixes[i]), expectedPrefixes[i]), 0)
			<< "row " << i << ": " << w.data.substr(rowStart, 30);

		size_t rowEnd = w.data.find("\r\n", rowStart);
		ASSERT_NE(rowEnd, std::string::npos);
		rowStart = rowEnd + 2;
	}

	// No extra rows
	EXPECT_EQ(rowStart, w.data.size());
}

// A completely full buffer (nextIdx == toothLoggerEntriesPerBuffer) must emit
// exactly one row per entry and stay within array bounds.
TEST(ToothLogger, WriteCsvFullBuffer) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	CompositeBuffer buf{};
	buf.nextIdx = toothLoggerEntriesPerBuffer;
	buf.startTime.reset(0);

	// Distinct, increasing offsets; the last one stays under the 5 s flush period
	for (size_t i = 0; i < toothLoggerEntriesPerBuffer; i++) {
		composite_logger_s c{};
		c.timestamp = i * 20'000; // 20 ms apart -> last entry at 4.980000 s
		c.priLevel = (i % 2) == 0;
		buf.buffer[i].x = SWAP_UINT64(c.x);
	}

	StringWriter w;
	int total = ToothLoggerWriteBufferCsv(w, &buf);

	EXPECT_GT(total, 0);
	EXPECT_EQ((size_t)total, w.data.size());

	// One CRLF-terminated row per entry
	size_t crlfCount = 0;
	for (size_t i = 0; i + 1 < w.data.size(); i++) {
		if (w.data[i] == '\r' && w.data[i + 1] == '\n') {
			crlfCount++;
		}
	}
	EXPECT_EQ(crlfCount, toothLoggerEntriesPerBuffer);

	// First and last rows carry the expected timestamps
	EXPECT_EQ(w.data.compare(0, 10, "0.000000, "), 0) << w.data.substr(0, 30);
	size_t lastRow = w.data.rfind("4.980000, ");
	ASSERT_NE(lastRow, std::string::npos);
	EXPECT_EQ(w.data.find("\r\n", lastRow) + 2, w.data.size());
}

// Boards can append their own columns to both the header and every row via the
// custom_board_toothLogCsv* overrides.
TEST(ToothLogger, WriteCsvBoardOverrideColumns) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	CsvBoardHookGuard guard;

	custom_board_toothLogCsvHeader = [](char* buffer, size_t size) {
		return snprintf(buffer, size, ", BoardColA, BoardColB");
	};
	custom_board_toothLogCsvLine = [](char* buffer, size_t size, const void* /*payload*/) {
		return snprintf(buffer, size, ", %d, %.2f", 7, 8.25f);
	};

	StringWriter header;
	EXPECT_EQ(ToothLoggerWriteCsvHeader(header), 130);
	EXPECT_EQ(header.data,
		"Time[s], Primary, Cam 1, Cam 2, Cam 3, Cam 4, Sync, TDC, Coils, Injectors, ACR, VBatt, ET, InstantMAP, TPS"
		", BoardColA, BoardColB\r\n");

	CompositeBuffer buf{};
	buf.nextIdx = 2;
	buf.startTime.reset(0);
	for (size_t i = 0; i < buf.nextIdx; i++) {
		composite_logger_s c{};
		c.timestamp = i * 1000;
		buf.buffer[i].x = SWAP_UINT64(c.x);
	}

	StringWriter rows;
	int total = ToothLoggerWriteBufferCsv(rows, &buf);
	EXPECT_GT(total, 0);
	EXPECT_EQ((size_t)total, rows.data.size());

	// Every row must end with the board fragment
	size_t rowStart = 0;
	for (size_t i = 0; i < buf.nextIdx; i++) {
		size_t rowEnd = rows.data.find("\r\n", rowStart);
		ASSERT_NE(rowEnd, std::string::npos);

		std::string row = rows.data.substr(rowStart, rowEnd - rowStart);
		const std::string expectedTail = ", 7, 8.25";
		ASSERT_GE(row.size(), expectedTail.size());
		EXPECT_EQ(row.substr(row.size() - expectedTail.size()), expectedTail) << "row " << i << ": " << row;

		rowStart = rowEnd + 2;
	}
	EXPECT_EQ(rowStart, rows.data.size());
}

// A board fragment that does not fit the row buffer must fail the write, not truncate silently.
TEST(ToothLogger, WriteCsvBoardOverrideLineOverflow) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	CsvBoardHookGuard guard;

	custom_board_toothLogCsvLine = [](char* buffer, size_t size, const void* /*payload*/) {
		// snprintf-style: report the length the fragment would need, far beyond any row buffer
		(void)buffer;
		(void)size;
		return 10000;
	};

	CompositeBuffer buf{};
	buf.nextIdx = 1;
	buf.startTime.reset(0);
	composite_logger_s c{};
	buf.buffer[0].x = SWAP_UINT64(c.x);

	StringWriter w;
	EXPECT_EQ(ToothLoggerWriteBufferCsv(w, &buf), -1);
	EXPECT_TRUE(w.data.empty());
}

#if TOOTH_LOG_BOARD_PAYLOAD_SIZE > 0
// The line hook receives each ROW's own payload (sampled at append time by
// custom_board_toothLogSample), not a flush-time snapshot: distinct per-entry
// payloads must come out as distinct per-row column values.
TEST(ToothLogger, WriteCsvBoardPayloadIsPerRow) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	CsvBoardHookGuard guard;

	custom_board_toothLogCsvLine = [](char* buffer, size_t size, const void* payload) {
		// interpret the first 4 payload bytes as a little uint32 counter
		uint32_t value;
		memcpy(&value, payload, sizeof(value));
		return snprintf(buffer, size, ", %u", (unsigned)value);
	};

	CompositeBuffer buf{};
	buf.nextIdx = 3;
	buf.startTime.reset(0);
	for (size_t i = 0; i < buf.nextIdx; i++) {
		composite_logger_s c{};
		c.timestamp = i * 1000;
		buf.buffer[i].x = SWAP_UINT64(c.x);
		uint32_t value = 100 + i;
		memcpy(buf.boardPayload[i], &value, sizeof(value));
	}

	StringWriter w;
	int total = ToothLoggerWriteBufferCsv(w, &buf);
	EXPECT_GT(total, 0);

	size_t rowStart = 0;
	for (size_t i = 0; i < buf.nextIdx; i++) {
		size_t rowEnd = w.data.find("\r\n", rowStart);
		ASSERT_NE(rowEnd, std::string::npos);
		std::string row = w.data.substr(rowStart, rowEnd - rowStart);
		char expectedTail[16];
		snprintf(expectedTail, sizeof(expectedTail), ", %u", (unsigned)(100 + i));
		ASSERT_GE(row.size(), strlen(expectedTail));
		EXPECT_EQ(row.substr(row.size() - strlen(expectedTail)), expectedTail)
			<< "row " << i << ": " << row;
		rowStart = rowEnd + 2;
	}
}
#endif // TOOTH_LOG_BOARD_PAYLOAD_SIZE
