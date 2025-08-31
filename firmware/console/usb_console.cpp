#include "pch.h"

#if EFI_USB_SERIAL

#include "usbconsole.h"
#include "thread_controller.h"
#include "tunerstudio.h"

// Assert that the USB tx/rx buffers are large enough to fit one full packet
// Lets don't care about underlying driver. If driver has no enough free
// space in buffers it will block chnWriteTimeout() until buffers available
// or timeout happens.
//static_assert(SERIAL_USB_BUFFERS_TX_SIZE >= BLOCKING_FACTOR + 10);

extern SerialUSBDriver EFI_CONSOLE_USB_DEVICE;

class UsbChannel final : public TsChannelBase {
public:
	UsbChannel(SerialUSBDriver& driver)
		: TsChannelBase("USB"), m_channel(reinterpret_cast<BaseChannel*>(&driver))
	{
	}

	bool isReady() const override {
		return is_usb_serial_ready();
	}

	void write(const uint8_t* buffer, size_t size, bool /*isEndOfPacket*/) override {
		size_t transferred = chnWriteTimeout(m_channel, buffer, size, BINARY_IO_TIMEOUT);
		bytesOut += transferred;
	}

	size_t readTimeout(uint8_t* buffer, size_t size, int timeout) override {
	    size_t transferred = chnReadTimeout(m_channel, buffer, size, timeout);
		bytesIn += transferred;
		return transferred;
	}

private:
	BaseChannel* const m_channel;
};

static UsbChannel usbChannel(EFI_CONSOLE_USB_DEVICE);

struct UsbThread : public TunerstudioThread {
	UsbThread() : TunerstudioThread("USB Console") { }

	TsChannelBase* setupChannel() override {
		// Start the port's USB stack
		usb_serial_start();

		return &usbChannel;
	}
};

static UsbThread usbConsole;

void startUsbConsole() {
	usbConsole.start();
}

void printUsbConnectorStats() {
}

#endif // EFI_USB_SERIAL
