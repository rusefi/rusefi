#include "pch.h"

#if EFI_ETHERNET

#include "lwipthread.h"

#include "lwip/sockets.h"

#include "thread_controller.h"
#include "tunerstudio.h"

static int listenerSocket = -1;
static int connectionSocket = -1;

static void do_connection() {
	if (connectionSocket != -1) {
		auto localCopy = connectionSocket;
		connectionSocket = -1;

		lwip_close(localCopy);
	}

	sockaddr_in remote;
	socklen_t size = sizeof(remote);
	connectionSocket = lwip_accept(listenerSocket, (sockaddr*)&remote, &size);
}

class EthernetChannel : public TsChannelBase {
public:
	EthernetChannel()
		: TsChannelBase("Ethernet")
	{
	}

	bool isReady() const override {
		return connectionSocket != -1;
	}

	void write(const uint8_t* buffer, size_t size, bool isEndOfPacket) override {
		auto flags = isEndOfPacket ? 0 : MSG_MORE;
		lwip_send(connectionSocket, buffer, size, flags);
	}

	size_t readTimeout(uint8_t* buffer, size_t size, int /*timeout*/) override {
		auto result = lwip_recv(connectionSocket, buffer, size, /*flags =*/ 0);

		if (result == -1) {
			do_connection();
			return 0;
		}

		return result;
	}
};

static EthernetChannel ethChannel;

struct EthernetThread : public TunerstudioThread {
	EthernetThread() : TunerstudioThread("Ethernet Console") { }

	TsChannelBase* setupChannel() override {
		lwipInit(nullptr);

		sockaddr_in address;
		address.sin_family = AF_INET;
		address.sin_port = htons(29000);
		address.sin_addr.s_addr = INADDR_ANY;

		listenerSocket = lwip_socket(AF_INET, SOCK_STREAM, 0);
		lwip_bind(listenerSocket, (sockaddr*)&address, sizeof(address));
		lwip_listen(listenerSocket, 1);

		do_connection();

		return &ethChannel;
	}
};

static EthernetThread ethernetConsole;

void startEthernetConsole() {
#ifndef STM32H7
	// TODO: why does this break H7? I thought the pins were the same?
	efiSetPadMode("ethernet", GPIOA_1, PAL_MODE_ALTERNATE(0xb));
	efiSetPadMode("ethernet", GPIOA_2, PAL_MODE_ALTERNATE(0xb));
	efiSetPadMode("ethernet", GPIOA_7, PAL_MODE_ALTERNATE(0xb));

	efiSetPadMode("ethernet", GPIOC_1, PAL_MODE_ALTERNATE(0xb));
	efiSetPadMode("ethernet", GPIOC_4, PAL_MODE_ALTERNATE(0xb));
	efiSetPadMode("ethernet", GPIOC_5, PAL_MODE_ALTERNATE(0xb));

	efiSetPadMode("ethernet", GPIOD_5, PAL_MODE_ALTERNATE(0xb));

	efiSetPadMode("ethernet", GPIOG_11, PAL_MODE_ALTERNATE(0xb));
	efiSetPadMode("ethernet", GPIOG_13, PAL_MODE_ALTERNATE(0xb));
	efiSetPadMode("ethernet", GPIOG_14, PAL_MODE_ALTERNATE(0xb));
#endif // STM32H7

	ethernetConsole.Start();
}

#endif // EFI_ETHERNET
