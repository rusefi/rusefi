// TODO: @andreika-git finish this implementation

#include "tunerstudio_io.h"

#ifdef TS_CAN_DEVICE
#include "serial_can.h"

class CanTsChannel : public TsChannelBase {
	void start();

	// TsChannelBase implementation
	void write(const uint8_t* buffer, size_t size) override;
	size_t readTimeout(uint8_t* buffer, size_t size, int timeout) override;
	void flush() override;
	bool isReady() override;
	void stop() override;

	// Special override for writeCrcPacket for small packets
	void writeCrcPacket(uint8_t responseCode, const uint8_t* buf, size_t size) override;
};

static CANConfig tsCanConfig = { CAN_MCR_ABOM | CAN_MCR_AWUM | CAN_MCR_TXFP, CAN_BTR_500 };

void CanTsChannel::start() {
	efiSetPadMode("ts can rx", GPIOG_13/*CONFIG(canRxPin)*/, PAL_MODE_ALTERNATE(TS_CAN_AF)); // CAN2_RX2_0
	efiSetPadMode("ts can tx", GPIOG_14/*CONFIG(canTxPin)*/, PAL_MODE_ALTERNATE(TS_CAN_AF)); // CAN2_TX2_0

	canStart(&TS_CAN_DEVICE, &tsCanConfig);
	canInit(&TS_CAN_DEVICE);
}

void CanTsChannel::stop() {
	canStop(&TS_CAN_DEVICE);
}

void CanTsChannel::writeCrcPacket(uint8_t responseCode, const uint8_t* buf, size_t size) {
#ifdef TS_CAN_DEVICE_SHORT_PACKETS_IN_ONE_FRAME
	// a special case for short packets: we can sent them in 1 frame, without CRC & size,
	// because the CAN protocol is already protected by its own checksum.
	if ((size + 1) <= 7) {
		write(&responseCode, 1);      // header without size
		if (size > 0) {
			write(buf, size);      // body
		}
		flush();
		return;
	}
#endif /* TS_CAN_DEVICE */

	// Packet too large, use default implementation
	TsChannelBase::writeCrcPacket(responseCode, buf, size);
}

void CanTsChannel::write(const uint8_t* buffer, size_t size) {
	canAddToTxStreamTimeout(&TS_CAN_DEVICE, &size, buffer, BINARY_IO_TIMEOUT);
}

size_t CanTsChannel::readTimeout(uint8_t* buffer, size_t size, int timeout) {
	canStreamReceiveTimeout(&TS_CAN_DEVICE, &size, buffer, timeout);
	return size;
}

void CanTsChannel::flush() {
	canFlushTxStream(&TS_CAN_DEVICE);
}

bool CanTsChannel::isReady() {
	// this channel is always ready
	return true;
}

#endif // def TS_CAN_DEVICE
