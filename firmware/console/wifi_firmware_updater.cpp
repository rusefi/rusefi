#include "spi_flash/include/spi_flash.h"

#include "usbcfg.h"

static bool isBigEndian() {
	uint32_t test = 0x11223344;
	uint8_t *pTest = reinterpret_cast<uint8_t *>(&test);
	return pTest[0] == 0x11;
}

static uint32_t fromNetwork32(uint32_t from) {
	static const bool be = isBigEndian();
	if (be) {
		return from;
	} else {
		uint8_t *pFrom = reinterpret_cast<uint8_t *>(&from);
		uint32_t to;
		to = pFrom[0]; to <<= 8;
		to |= pFrom[1]; to <<= 8;
		to |= pFrom[2]; to <<= 8;
		to |= pFrom[3];
		return to;
	}
}

static uint16_t fromNetwork16(uint16_t from) {
	static bool be = isBigEndian();
	if (be) {
		return from;
	} else {
		uint8_t *pFrom = reinterpret_cast<uint8_t *>(&from);
		uint16_t to;
		to = pFrom[0]; to <<= 8;
		to |= pFrom[1];
		return to;
	}
}

static uint32_t toNetwork32(uint32_t to) {
	return fromNetwork32(to);
}

static uint16_t toNetwork16(uint16_t to) {
	return fromNetwork16(to);
}


typedef struct __attribute__((__packed__)) {
	uint8_t command;
	uint32_t address;
	uint32_t arg1;
	uint16_t payloadLength;

	// payloadLenght bytes of data follows...
} UartPacket;

static const int MAX_PAYLOAD_SIZE = 256;

#define CMD_READ_FLASH        0x01
#define CMD_WRITE_FLASH       0x02
#define CMD_ERASE_FLASH       0x03
#define CMD_MAX_PAYLOAD_SIZE  0x50
#define CMD_HELLO             0x99

static int readch() {
	uint8_t buf;
	int ret = chnReadTimeout(&SDU1, &buf, 1, TIME_MS2I(1));

	if (ret == 0) {
		return -1;
	} else {
		return (int)buf;
	}
}

static void receivePacket(UartPacket *pkt, uint8_t *payload) {
	// Read command
	uint8_t *p = reinterpret_cast<uint8_t *>(pkt);
	uint16_t l = sizeof(UartPacket);
	while (l > 0) {
		int c = readch();
		if (c == -1)
		continue;
		*p++ = c;
		l--;
	}

	// Convert parameters from network byte order to cpu byte order
	pkt->address = fromNetwork32(pkt->address);
	pkt->arg1 = fromNetwork32(pkt->arg1);
	pkt->payloadLength = fromNetwork16(pkt->payloadLength);

	// Read payload
	l = pkt->payloadLength;
	while (l > 0) {
		int c = readch();
		if (c == -1)
		continue;
		*payload++ = c;
		l--;
	}
}

// Allocated statically so the compiler can tell us
// about the amount of used RAM
static UartPacket pkt;
static uint8_t payload[MAX_PAYLOAD_SIZE];

static void serialPrint(const uint8_t* data, size_t length) {
	chnWriteTimeout(&SDU1, data, length, TIME_MS2I(100));
}

static void serialPrintStr(const char* str) {
	size_t len = strlen(str);
	serialPrint(reinterpret_cast<const uint8_t*>(str), len);
}

struct WifiUpdaterThread : public ThreadController<4096> {
	WifiUpdaterThread() : ThreadController("WifiPump", NORMALPRIO - 10) {}
	void ThreadTask() override {
		if (M2M_SUCCESS != m2m_wifi_download_mode()) {
			return;
		}

		auto flashSize = spi_flash_get_size();
		(void)flashSize;

		//int ret = spi_flash_erase(0, FLASH_SECTOR_SZ);
		//(void)ret;

		usb_serial_start();

		while (true) {
			while (!is_usb_serial_ready()) {
				chThdSleepMilliseconds(1);
			}

			receivePacket(&pkt, payload);

			if (pkt.command == CMD_HELLO) {
				if (pkt.address == 0x11223344 && pkt.arg1 == 0x55667788) {
					serialPrintStr("v10000");
				}
			}

			if (pkt.command == CMD_MAX_PAYLOAD_SIZE) {
				uint16_t res = toNetwork16(MAX_PAYLOAD_SIZE);
				serialPrint(reinterpret_cast<uint8_t *>(&res), sizeof(res));
			}

			if (pkt.command == CMD_READ_FLASH) {
				uint32_t address = pkt.address;
				uint32_t len = pkt.arg1;
				if (spi_flash_read(payload, address, len) != M2M_SUCCESS) {
					serialPrintStr("ER");
				} else {
					serialPrint(payload, len);
					serialPrintStr("OK");
				}
			}

			if (pkt.command == CMD_WRITE_FLASH) {
				uint32_t address = pkt.address;
				uint32_t len = pkt.payloadLength;
				if (spi_flash_write(payload, address, len) != M2M_SUCCESS) {
					serialPrintStr("ER");
				} else {
					serialPrintStr("OK");
				}
			}

			if (pkt.command == CMD_ERASE_FLASH) {
				uint32_t address = pkt.address;
				uint32_t len = pkt.arg1;
				if (spi_flash_erase(address, len) != M2M_SUCCESS) {
					serialPrintStr("ER");
					// serialPrintStr("OK");
				} else {
					serialPrintStr("OK");
				}
			}
		}
	}
};

static WifiUpdaterThread wifiUpdater;

void startWifiUpdater() {
	wifiUpdater.start();
}

