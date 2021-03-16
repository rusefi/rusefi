#include "usbconsole.h"
#include "thread_controller.h"
#include "tunerstudio.h"

// Assert that the USB tx/rx buffers are large enough to fit one full packet
static_assert(SERIAL_USB_BUFFERS_SIZE >= BLOCKING_FACTOR + 10);

extern SerialUSBDriver EFI_CONSOLE_USB_DEVICE;
static BaseChannelTsChannel usbChannel((BaseChannel*)&EFI_CONSOLE_USB_DEVICE);

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
