/**
 * @file	This file initializes the hardware serial ports that run the TS protocol.
 *
 * @date Mar 26, 2021
 */

#include "pch.h"

#if EFI_PROD_CODE || EFI_SIMULATOR
#include "tunerstudio.h"
#include "tunerstudio_io.h"
#include "connector_uart_dma.h"
#if HW_HELLEN
#include "hellen_meta.h"
#endif // HW_HELLEN

// These may not be defined due to the HAL, but they're necessary for the compiler to do it's magic
class UARTDriver;
class UartDmaTsChannel;
class UartTsChannel;
class SerialTsChannel;

#if HAS_PRIMARY
#ifdef TS_PRIMARY_PORT

// We want to instantiate the correct channel type depending on what type of serial port we're
// using.  ChibiOS supports two - UART and Serial.  We compare the type of the port we're given
// against UartDriver and decide whether to instantiate a UART TS Channel or a Serial version.  The
// UART is further subdivided into two depending whether we support DMA or not.  We use the right
// combination of std::conditional, std::is_same, and #if to get what we want.
	std::conditional_t<
		std::is_same_v<decltype(TS_PRIMARY_PORT), UARTDriver>,
#if EFI_USE_UART_DMA
		UartDmaTsChannel,
#else // EFI_USE_UART_DMA
		UartTsChannel,
#endif // EFI_USE_UART_DMA
		SerialTsChannel> primaryChannel(TS_PRIMARY_PORT);
#endif // TS_PRIMARY_PORT

	struct PrimaryChannelThread : public TunerstudioThread {
		PrimaryChannelThread() : TunerstudioThread("Primary TS Channel") { }

		TsChannelBase* setupChannel() {
#if EFI_PROD_CODE
			// historically the idea was that primary UART has to be very hard-coded as the last line of reliability defense
			// as of 2022 it looks like sometimes we just need the GPIO on MRE for instance more than we need UART
			efiSetPadMode("Primary UART RX", EFI_CONSOLE_RX_BRAIN_PIN, PAL_MODE_ALTERNATE(EFI_CONSOLE_AF));
			efiSetPadMode("Primary UART TX", EFI_CONSOLE_TX_BRAIN_PIN, PAL_MODE_ALTERNATE(EFI_CONSOLE_AF));
#endif /* EFI_PROD_CODE */

			primaryChannel.start(engineConfiguration->uartConsoleSerialSpeed);

			return &primaryChannel;
		}
	};

	static PrimaryChannelThread primaryChannelThread;
#endif // HAS_PRIMARY

#if HAS_SECONDARY
#ifdef TS_SECONDARY_PORT
	std::conditional_t<
		std::is_same_v<decltype(TS_SECONDARY_PORT), UARTDriver>,
#if EFI_USE_UART_DMA
		UartDmaTsChannel,
#else // EFI_USE_UART_DMA
		UartTsChannel,
#endif // EFI_USE_UART_DMA
		SerialTsChannel> secondaryChannel(TS_SECONDARY_PORT);
#endif // TS_SECONDARY_PORT


	struct SecondaryChannelThread : public TunerstudioThread {
		SecondaryChannelThread() : TunerstudioThread("Secondary TS Channel") { }

		TsChannelBase* setupChannel() {
#if EFI_PROD_CODE
			efiSetPadMode("Secondary UART RX", engineConfiguration->binarySerialRxPin, PAL_MODE_ALTERNATE(TS_SERIAL_AF));
			efiSetPadMode("Secondary UART TX", engineConfiguration->binarySerialTxPin, PAL_MODE_ALTERNATE(TS_SERIAL_AF));
#endif /* EFI_PROD_CODE */

			secondaryChannel.start(engineConfiguration->tunerStudioSerialSpeed);

			return &secondaryChannel;
		}
	};

	static SecondaryChannelThread secondaryChannelThread;
#endif // HAS_SECONDARY

void startSerialChannels() {
#if HAS_PRIMARY
	// todo: invert setting one day?
	if (!engineConfiguration->disablePrimaryUart) {
		primaryChannelThread.start();
	}
#endif

#if HAS_SECONDARY
	secondaryChannelThread.start();
#endif
}

SerialTsChannelBase* getBluetoothChannel() {
#if HAS_SECONDARY
	// Prefer secondary channel for bluetooth
	return &secondaryChannel;
#elif HAS_PRIMARY
	// Use primary channel for BT if no secondary exists
	return &primaryChannel;
#endif

	// no HW serial channels on this board, fail
	return nullptr;
}

#endif // EFI_PROD_CODE
