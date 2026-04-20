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

	if (connectionSocket != -1) {
		int one = 1;
		lwip_setsockopt(connectionSocket, IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));
	}
}

class EthernetChannel final : public TsChannelBase {
public:
	EthernetChannel()
		: TsChannelBase("Ethernet")
	{
	}

	bool isReady() const override {
		return connectionSocket != -1;
	}

	void write(const uint8_t* buffer, size_t size, bool isEndOfPacket) override {
		// lwIP's MSG_MORE only sets PSH, it does not coalesce sends.  Buffer locally so
		// multi-part TS responses (header + body + tail) leave as a single TCP segment.
		// Without this, small leading writes trigger delayed-ACK + Nagle on the peer
		// pair and add ~40 ms per transaction.
		size_t remaining = size;
		const uint8_t* src = buffer;
		while (remaining > 0) {
			size_t free = sizeof(txBuffer) - txPos;
			size_t chunk = remaining < free ? remaining : free;
			memcpy(txBuffer + txPos, src, chunk);
			txPos += chunk;
			src += chunk;
			remaining -= chunk;
			if (txPos == sizeof(txBuffer)) {
				flush();
			}
		}
		if (isEndOfPacket) {
			flush();
		}
	}

	void flush() override {
		if (txPos > 0) {
			lwip_send(connectionSocket, txBuffer, txPos, 0);
			txPos = 0;
		}
	}

private:
	uint8_t txBuffer[1536];
	size_t txPos = 0;
public:

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

void EthernetConsoleModule::initNoConfiguration() {
	ethernetConsole.start();
}

#endif // EFI_ETHERNET
