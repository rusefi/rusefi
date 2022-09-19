/**
 * @file	tunerstudio_io.h
 * @file TS protocol commands and methods are here
 *
 * @date Mar 8, 2015
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once
#include "global.h"
#include "tunerstudio_impl.h"

#if (!TS_NO_PRIMARY && defined(TS_PRIMARY_PORT))
	#define HAS_PRIMARY true
#else
	#define HAS_PRIMARY false
#endif

#if (!TS_NO_SECONDARY && defined(TS_SECONDARY_PORT))
	#define HAS_SECONDARY true
#else
	#define HAS_SECONDARY false
#endif

#if EFI_USB_SERIAL
#include "usbconsole.h"
#endif // EFI_USB_SERIAL

#if EFI_PROD_CODE
#include "pin_repository.h"
#endif

#define SCRATCH_BUFFER_PREFIX_SIZE 3

class TsChannelBase {
public:
	TsChannelBase(const char *name);
	// Virtual functions - implement these for your underlying transport
	virtual void write(const uint8_t* buffer, size_t size, bool isEndOfPacket = false) = 0;
	virtual size_t readTimeout(uint8_t* buffer, size_t size, int timeout) = 0;

	// These functions are optional to implement, not all channels need them
	virtual void flush() { }
	virtual bool isConfigured() const { return true; }
	virtual bool isReady() const { return true; }
	virtual void stop() { }

	// Base functions that use the above virtual implementation
	size_t read(uint8_t* buffer, size_t size);

#ifdef EFI_CAN_SERIAL
	virtual	// CAN device needs this function to be virtual for small-packet optimization
#endif
	void writeCrcPacket(uint8_t responseCode, const uint8_t* buf, size_t size, bool allowLongPackets = false);
	void sendResponse(ts_response_format_e mode, const uint8_t * buffer, int size, bool allowLongPackets = false);

	/**
	 * See 'blockingFactor' in rusefi.ini
	 */
	char scratchBuffer[BLOCKING_FACTOR + 30];
	const char *name;

	void assertPacketSize(size_t size, bool allowLongPackets);
	void crcAndWriteBuffer(uint8_t responseCode, size_t size);
	void copyAndWriteSmallCrcPacket(uint8_t responseCode, const uint8_t* buf, size_t size);

	/* When TsChannel is in "not in sync" state tsProcessOne will silently try to find
	 * begining of packet.
	 * As soon as tsProcessOne was able to receive valid packet with valid size and crc
	 * TsChannel becomes "in sync". That means it will react on any futher errors: it will
	 * emit packet with error code and switch back to "not in sync" mode.
	 * This insures that RE will send only one error message after lost of syncronization
	 * with TS.
	 * Also while in "not in sync" state - tsProcessOne will not try to receive whole packet
	 * by one read. Instead after getting packet size it will try to receive one byte of
	 * command and check if it is supported. */
	bool in_sync = false;

private:
	void writeCrcPacketLarge(uint8_t responseCode, const uint8_t* buf, size_t size);
};

// This class represents a channel for a physical async serial poart
class SerialTsChannelBase : public TsChannelBase {
public:
	SerialTsChannelBase(const char *name) : TsChannelBase(name) {};
	// Open the serial port with the specified baud rate
	virtual void start(uint32_t baud) = 0;
};

#if HAL_USE_SERIAL
// This class implements a ChibiOS Serial Driver
class SerialTsChannel : public SerialTsChannelBase {
public:
	SerialTsChannel(SerialDriver& driver) : SerialTsChannelBase("Serial"), m_driver(&driver) { }

	void start(uint32_t baud) override;
	void stop() override;

	void write(const uint8_t* buffer, size_t size, bool isEndOfPacket) override;
	size_t readTimeout(uint8_t* buffer, size_t size, int timeout) override;

private:
	SerialDriver* const m_driver;
};
#endif // HAL_USE_SERIAL

#if HAL_USE_UART
// This class implements a ChibiOS UART Driver
class UartTsChannel : public SerialTsChannelBase {
public:
	UartTsChannel(UARTDriver& driver) : SerialTsChannelBase("UART"), m_driver(&driver) { }

	void start(uint32_t baud) override;
	void stop() override;

	void write(const uint8_t* buffer, size_t size, bool isEndOfPacket) override;
	size_t readTimeout(uint8_t* buffer, size_t size, int timeout) override;

protected:
	UARTDriver* const m_driver;
	UARTConfig m_config;
};
#endif // HAL_USE_UART

#define CRC_VALUE_SIZE 4
// todo: double-check this
#define CRC_WRAPPING_SIZE (CRC_VALUE_SIZE + 3)

// that's 1 second
#define BINARY_IO_TIMEOUT TIME_MS2I(1000)

// that's 1 second
#define SR5_READ_TIMEOUT TIME_MS2I(1000)

void startSerialChannels();
SerialTsChannelBase* getBluetoothChannel();

void startCanConsole();

void sendOkResponse(TsChannelBase *tsChannel, ts_response_format_e mode);
