#include "pch.h"

#if EFI_USB_SERIAL

#include "usbconsole.h"
#include "thread_controller.h"
#include "tunerstudio.h"


// Assert that the USB tx/rx buffers are large enough to fit one full packet
static_assert(SERIAL_USB_BUFFERS_SIZE >= BLOCKING_FACTOR + 10);

extern SerialUSBDriver EFI_CONSOLE_USB_DEVICE;

class UsbChannel : public TsChannelBase {
public:
	UsbChannel(SerialUSBDriver& driver)
		: TsChannelBase("USB"), m_channel(reinterpret_cast<BaseChannel*>(&driver))
	{
	}

	bool isReady() const override {
		return is_usb_serial_ready();
	}

	void write(const uint8_t* buffer, size_t size) override {
		chnWriteTimeout(m_channel, buffer, size, BINARY_IO_TIMEOUT);
	}

	size_t readTimeout(uint8_t* buffer, size_t size, int timeout) override {
		return chnReadTimeout(m_channel, buffer, size, timeout);
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
	usbConsole.Start();
}

#endif // EFI_USB_SERIAL
