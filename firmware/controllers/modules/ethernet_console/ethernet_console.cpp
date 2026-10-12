#include "pch.h"

#if EFI_ETHERNET

#include "lwipthread.h"

#include "lwip/sockets.h"

#include "thread_controller.h"
#include "tunerstudio.h"

RUSEFI_STACK_FOREIGN_ROOT(lwIP_driver, "lwip_thread", LWIP_THREAD_STACK_SIZE);
RUSEFI_STACK_FOREIGN_ROOT(lwIP_TCP__IP, "tcpip_thread", TCPIP_THREAD_STACKSIZE);

#ifndef Ethernet_Connection_Count
#define Ethernet_Connection_Count 1
#endif

static_assert(Ethernet_Connection_Count > 0, "At least one Ethernet connection is required");
static int listenerSocket = -1;
// lwIP does not support concurrent accepts on the same socket. Only accepting
// (and initial listener setup) takes this lock; client I/O stays independent.
static chibios_rt::Mutex listenerMutex;

static bool initializeListener() {
	chibios_rt::MutexLocker lock(listenerMutex);
	static bool initialized = false;
	if (!initialized) {
		initialized = true;
		lwipInit(nullptr);

		sockaddr_in address{};
		address.sin_family = AF_INET;
		address.sin_port = htons(ETHERNET_PORT);
		address.sin_addr.s_addr = INADDR_ANY;

		listenerSocket = lwip_socket(AF_INET, SOCK_STREAM, 0);
		if (listenerSocket != -1 &&
			(lwip_bind(listenerSocket, (sockaddr*)&address, sizeof(address)) != 0 ||
			 lwip_listen(listenerSocket, Ethernet_Connection_Count) != 0)) {
			lwip_close(listenerSocket);
			listenerSocket = -1;
		}
	}
	return listenerSocket != -1;
}

class EthernetChannel final : public TsChannelBase {
public:
	EthernetChannel(const char* name)
		: TsChannelBase(name)
	{
	}

	bool isReady() const override {
		// A listening channel can accept a replacement client in readTimeout().
		return listenerSocket != -1;
	}

	void write(const uint8_t* buffer, size_t size, bool isEndOfPacket) override {
		if (connectionSocket == -1) {
			return;
		}

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
				if (connectionSocket == -1) {
					return;
				}
			}
		}
		if (isEndOfPacket) {
			flush();
		}
	}

	void flush() override {
		size_t sent = 0;
		while (sent < txPos) {
			auto result = lwip_send(connectionSocket, txBuffer + sent, txPos - sent, 0);
			if (result <= 0) {
				disconnect();
				return;
			}
			sent += static_cast<size_t>(result);
		}
		txPos = 0;
	}

	size_t readTimeout(uint8_t* buffer, size_t size, int /*timeout*/) override {
		if (connectionSocket == -1) {
			connect();
		}

		size_t received = 0;
		while (received < size) {
			auto result = lwip_recv(connectionSocket, buffer + received, size - received, /*flags =*/ 0);
			if (result <= 0) {
				disconnect();
				return 0;
			}
			received += static_cast<size_t>(result);
		}

		return received;
	}

private:
	int connectionSocket = -1;
	uint8_t txBuffer[1536];
	size_t txPos = 0;

	void disconnect() {
		in_sync = false;
		settingsBurnPending = false;
		txPos = 0;
		if (connectionSocket != -1) {
			lwip_close(connectionSocket);
			connectionSocket = -1;
		}
	}

	void connect() {
		{
			chibios_rt::MutexLocker lock(listenerMutex);
			while (connectionSocket == -1) {
				sockaddr_in remote{};
				socklen_t size = sizeof(remote);
				connectionSocket = lwip_accept(listenerSocket, (sockaddr*)&remote, &size);
				if (connectionSocket == -1) {
					chThdSleepMilliseconds(100);
				}
			}
		}

		int one = 1;
		lwip_setsockopt(connectionSocket, IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));
		lwip_setsockopt(connectionSocket, SOL_SOCKET, SO_KEEPALIVE, &one, sizeof(one));

		int keepIdle = 5;
		int keepInterval = 1;
		int keepCount = 3;
		lwip_setsockopt(connectionSocket, IPPROTO_TCP, TCP_KEEPIDLE, &keepIdle, sizeof(keepIdle));
		lwip_setsockopt(connectionSocket, IPPROTO_TCP, TCP_KEEPINTVL, &keepInterval, sizeof(keepInterval));
		lwip_setsockopt(connectionSocket, IPPROTO_TCP, TCP_KEEPCNT, &keepCount, sizeof(keepCount));
	}
};

struct EthernetThread : public TunerstudioThread {
	EthernetThread() : TunerstudioThread("Ethernet Console"), channel("Ethernet") { }

	TsChannelBase* setupChannel() override {
		return initializeListener() ? &channel : nullptr;
	}

private:
	EthernetChannel channel;
};

static EthernetThread ethernetConsoles[Ethernet_Connection_Count];

void EthernetConsoleModule::initNoConfiguration() {
	for (auto& console : ethernetConsoles) {
		console.start();
	}
}

#endif // EFI_ETHERNET
