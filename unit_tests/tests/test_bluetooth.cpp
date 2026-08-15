/**
 * @file test_bluetooth.cpp
 *
 * Coverage for the Bluetooth module auto-setup helper in console/binary/bluetooth.cpp.
 */

#include "pch.h"
#include "bluetooth.h"

// set by bluetoothStart(); findBaudIndex() branches on it. Default (0) is BLUETOOTH_HC_05, which
// takes the plain "probe each baud with AT" path - no JDY-specific disconnect handshakes.
extern bluetooth_module_e btModuleType;

// getBluetoothChannel()/getTsSignature() are deliberately compiled out of the host unit-test build
// (they need real serial ports / the prod signature). bluetooth.cpp's bluetoothStart() references
// them, so provide host stubs to let the translation unit link. findBaudIndex(), the unit under
// test here, does not call either.
SerialTsChannelBase* getBluetoothChannel() {
	return nullptr;
}
const char* getTsSignature() {
	return "unit test signature";
}

namespace {

// A SerialTsChannelBase that records the last baud it was (re)started at and never answers the
// AT probe, so findBaudIndex() is forced down its "no module found" path.
class RecordingSerialChannel : public SerialTsChannelBase {
public:
	RecordingSerialChannel() : SerialTsChannelBase("test") { }

	void start(uint32_t baud) override {
		lastStartedBaud = baud;
		startCount++;
	}

	void stop() override {
		stopCount++;
	}

	void write(const uint8_t*, size_t, bool) override { }

	// always "time out" - report that zero of the requested bytes arrived
	size_t readTimeout(uint8_t*, size_t, int) override {
		return 0;
	}

	uint32_t lastStartedBaud = 0;
	int startCount = 0;
	int stopCount = 0;
};

} // namespace

/**
 * Coverage for findBaudIndex() when no Bluetooth module answers at any known baud rate.
 *
 * The function is supposed to put the serial link back to the configured console/TS speed before
 * giving up - that recovery exists, but it sits INSIDE the probe loop guarded by
 * `baudIdx == efi::size(baudRates)`, while the loop condition is `baudIdx < efi::size(baudRates)`.
 * The branch is therefore unreachable and never runs, so the channel is left started at the last
 * probed rate (57600) until the ECU is rebooted.
 *
 * This test documents CURRENT behavior. Once the recovery is reachable, expect the channel to be
 * left at engineConfiguration->tunerStudioSerialSpeed instead.
 */
TEST(bluetooth, findBaudIndexLeavesChannelAtLastProbedRate) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	btModuleType = BLUETOOTH_HC_05;

	// a distinctive configured speed that is NOT one of the probed baudRates[]
	engineConfiguration->tunerStudioSerialSpeed = 250000;

	RecordingSerialChannel channel;

	uint8_t result = findBaudIndex(&channel);

	// no module answered
	EXPECT_EQ(255, result);
	// the configured speed was never restored - the link is stuck on the last entry of baudRates[]
	EXPECT_EQ(57600u, channel.lastStartedBaud);
}
