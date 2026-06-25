/**
 * @file	This file implements CAN-to-TS bridge.
 *
 * @date Apr 24, 2021
 * @author andreika <prometheus.pcb@gmail.com>
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#include "pch.h"
#include "tunerstudio.h"
#include "tunerstudio_io.h"

#if EFI_CAN_SERIAL
#include "serial_can.h"
#include "can_hw.h"
#include "can_common.h"

#if !EFI_CAN_SUPPORT
#error "EFI_CAN_SERIAL requires EFI_CAN_SUPPORT"
#endif


class CanTsChannel final : public TsChannelBase {
public:
	CanTsChannel() : TsChannelBase("CAN") {

	}
	void start();

	// TsChannelBase implementation
	void write(const uint8_t* buffer, size_t size, bool) override;
	size_t readTimeout(uint8_t* buffer, size_t size, int timeout) override;
	void flush() override;
	bool isReady() const override;
	void stop() override;

	// Special override for writeCrcPacket for small packets
	void writeCrcPacket(uint8_t responseCode, const uint8_t* buf, size_t size, bool allowLongPackets = false) override;
};


void CanTsChannel::start() {
	if (!getIsCanEnabled()) {
		warning(ObdCode::CUSTOM_ERR_CAN_CONFIGURATION, "CAN not enabled");
		return;
	}

	if (!engineConfiguration->canReadEnabled || !engineConfiguration->canWriteEnabled) {
		warning(ObdCode::CUSTOM_ERR_CAN_CONFIGURATION, "CAN read or write not enabled");
	}
}

void CanTsChannel::stop() {
}

void CanTsChannel::writeCrcPacket(uint8_t responseCode, const uint8_t* buf, size_t size, bool allowLongPackets) {
#ifdef TS_CAN_DEVICE_SHORT_PACKETS_IN_ONE_FRAME
	// a special case for short packets: we can send them in 1 frame, without CRC & size,
	// because the CAN protocol is already protected by its own checksum.
	if ((size + 1) <= 7) {
		write(&responseCode, 1, false);      // header without size
		if (size > 0) {
			write(buf, size, false);      // body
		}
		flush();
		return;
	}
#endif /* TS_CAN_DEVICE_SHORT_PACKETS_IN_ONE_FRAME */

	// Packet too large, use default implementation
	TsChannelBase::writeCrcPacket(responseCode, buf, size, allowLongPackets);
}

void CanTsChannel::write(const uint8_t* buffer, size_t size, bool) {
	canStreamAddToTxTimeout(&size, buffer, BINARY_IO_TIMEOUT);
}

size_t CanTsChannel::readTimeout(uint8_t* buffer, size_t size, int timeout) {
	canStreamReceiveTimeout(&size, buffer, timeout);
	return size;
}

void CanTsChannel::flush() {
	canStreamFlushTx(BINARY_IO_TIMEOUT);
}

bool CanTsChannel::isReady() const {
	// this channel is always ready
	return true;
}

static CanTsChannel canChannel;

struct CanTsThread : public TunerstudioThread {
	CanTsThread() : TunerstudioThread("CAN TS Channel") { }

	TsChannelBase* setupChannel() override {
		canChannel.start();
		return &canChannel;
	}
};

static CanTsThread canTsThread;

void startCanConsole() {
	canTsThread.start();
	tsOverCanInit();
}

struct tsCanConsoleAnnounceMsg {
	uint32_t txId;	// ECU to host CAN ID
	uint32_t rxId;	// Host to ECU CAN ID
};

void announceCanConsole(CanCycle cycle) {
	// every 250mS?
	if (cycle.isInterval(CI::_250ms)) {
		CanTxTyped<tsCanConsoleAnnounceMsg> msg(CanCategory::SERIAL, (int)bench_test_packet_ids_e::ECU_ISO_TP_SETTINGS, true, /* channel */ 0);
		// TODO: add flag(s) if extended IDs are used!
		msg->txId = CAN_ECU_SERIAL_TX_ID;
		msg->rxId = CAN_ECU_SERIAL_RX_ID;
	}
}

#endif // EFI_CAN_SERIAL
