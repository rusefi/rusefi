#include "pch.h"

#include "tooth_logger.h"

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

} // namespace

TEST(ToothLogger, WriteCsvHeader) {
	StringWriter w;
	ToothLoggerWriteCsvHeader(w);

	EXPECT_EQ(w.data,
		"Time[s], Primary, Cam 1, Cam 2, Cam 3, Cam 4, Sync, TDC, Coils, Injectors, ACR, VBatt, ET, InstantMAP, TPS\r\n");
}

TEST(ToothLogger, WriteCsvRows) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	// Seed mock sensor values so we get deterministic VBatt/ET/TPS columns
	Sensor::setMockValue(SensorType::BatteryVoltage, 12.34f);
	Sensor::setMockValue(SensorType::Clt, 56.78f);
	Sensor::setMockValue(SensorType::Tps1, 43.21f);

	engine->outputChannels.instantMAPValue = 0;

	CompositeBuffer buf{};
	buf.nextIdx = 2;
	buf.startTime.reset(0);

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
		// On-the-wire format is byte-reversed; ToothLoggerWriteCsv swaps it back.
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
	int total = ToothLoggerWriteCsv(w, &buf);

	EXPECT_GT(total, 0);
	EXPECT_EQ((size_t)total, w.data.size());

	// Each row ends with the per-row VBatt and ET (formatted with %.2f)
	// and contains the timestamp prefix.
	EXPECT_NE(w.data.find("1.000002,"), std::string::npos);
	EXPECT_NE(w.data.find("2.000500,"), std::string::npos);
	EXPECT_NE(w.data.find("12.34"), std::string::npos);
	EXPECT_NE(w.data.find("56.78"), std::string::npos);
	EXPECT_NE(w.data.find("43.21"), std::string::npos);

	// Number of CSV rows (CRLF-terminated) must match the buffer entries.
	size_t crlfCount = 0;
	for (size_t i = 0; i + 1 < w.data.size(); i++) {
		if (w.data[i] == '\r' && w.data[i + 1] == '\n') {
			crlfCount++;
		}
	}
	EXPECT_EQ(crlfCount, 2u);
}
