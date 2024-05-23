/**
 * @file	This file initializes the hardware serial ports that run the TS protocol.
 *
 * @date Mar 26, 2021
 */

#include "pch.h"

#if (EFI_PROD_CODE || EFI_SIMULATOR) && EFI_TUNER_STUDIO
#include "tunerstudio.h"
#include "tunerstudio_io.h"
#include "connector_uart_dma.h"
#if defined(HW_HELLEN)
#include "hellen_meta.h"
#endif // HW_HELLEN

#ifdef TS_PRIMARY_UxART_PORT

#if EFI_TS_PRIMARY_IS_SERIAL
    SerialTsChannel
#elif EFI_USE_UART_DMA
	UartDmaTsChannel
#else
	UartTsChannel
#endif
    primaryChannel(TS_PRIMARY_UxART_PORT);

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
#endif // defined(TS_PRIMARY_UxART_PORT)

#ifdef TS_SECONDARY_UxART_PORT
#if EFI_TS_SECONDARY_IS_SERIAL
    SerialTsChannel
#elif EFI_USE_UART_DMA
	UartDmaTsChannel
#else
	UartTsChannel
#endif
    secondaryChannel(TS_SECONDARY_UxART_PORT);

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
#endif // defined(TS_SECONDARY_UxART_PORT)

void startSerialChannels() {
#if defined(TS_PRIMARY_UxART_PORT)
	primaryChannelThread.start();
#endif

#if defined(TS_SECONDARY_UxART_PORT)
    // do not start thread if not configured - give user a chance to use same peripheral for kline
    if (isBrainPinValid(engineConfiguration->binarySerialTxPin)) {
	    secondaryChannelThread.start();
	}
#endif
}

SerialTsChannelBase* getBluetoothChannel() {
#if defined(TS_SECONDARY_UxART_PORT)
	// Prefer secondary channel for bluetooth
	return &secondaryChannel;
#elif defined(TS_PRIMARY_UxART_PORT)
	// Use primary channel for BT if no secondary exists
	return &primaryChannel;
#endif

	// no HW serial channels on this board, fail
	return nullptr;
}

#endif // EFI_PROD_CODE
