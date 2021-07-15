/**
 * @file	This file initializes the hardware serial ports that run the TS protocol.
 *
 * @date Mar 26, 2021
 */

#include "engine.h"

#if EFI_PROD_CODE || EFI_SIMULATOR
#include "tunerstudio.h"
#include "tunerstudio_io.h"
#include "connector_uart_dma.h"

EXTERN_ENGINE;

#if (!defined(TS_NO_PRIMARY) && (defined(TS_PRIMARY_UART) || defined(TS_PRIMARY_SERIAL)))
	#define HAS_PRIMARY true
#else
	#define HAS_PRIMARY false
#endif

#if (!defined(TS_NO_SECONDARY) && (defined(TS_SECONDARY_UART) || defined(TS_SECONDARY_SERIAL)))
	#define HAS_SECONDARY true
#else
	#define HAS_SECONDARY false
#endif

#if HAS_PRIMARY
	#ifdef TS_PRIMARY_UART
		#if EFI_USE_UART_DMA
			UartDmaTsChannel primaryChannel(TS_PRIMARY_UART);
		#else
			UartTsChannel primaryChannel(TS_PRIMARY_UART);
		#endif
	#elif defined(TS_PRIMARY_SERIAL)
		SerialTsChannel primaryChannel(TS_PRIMARY_SERIAL);
	#endif

	struct PrimaryChannelThread : public TunerstudioThread {
		PrimaryChannelThread() : TunerstudioThread("Primary TS Channel") { }

		TsChannelBase* setupChannel() {
#if EFI_PROD_CODE
			efiSetPadMode("Primary Channel RX", EFI_CONSOLE_RX_BRAIN_PIN, PAL_MODE_ALTERNATE(EFI_CONSOLE_AF));
			efiSetPadMode("Primary Channel TX", EFI_CONSOLE_TX_BRAIN_PIN, PAL_MODE_ALTERNATE(EFI_CONSOLE_AF));
#endif /* EFI_PROD_CODE */

			primaryChannel.start(CONFIG(uartConsoleSerialSpeed));

			return &primaryChannel;
		}
	};

	static PrimaryChannelThread primaryChannelThread;
#endif // HAS_PRIMARY

#if HAS_SECONDARY
	#ifdef TS_SECONDARY_UART
		#if EFI_USE_UART_DMA
			UartDmaTsChannel secondaryChannel(TS_SECONDARY_UART);
		#else
			UartTsChannel secondaryChannel(TS_SECONDARY_UART);
		#endif
	#elif defined(TS_SECONDARY_SERIAL)
		SerialTsChannel secondaryChannel(TS_SECONDARY_SERIAL);
	#endif

	struct SecondaryChannelThread : public TunerstudioThread {
		SecondaryChannelThread() : TunerstudioThread("Secondary TS Channel") { }

		TsChannelBase* setupChannel() {
#if EFI_PROD_CODE
			efiSetPadMode("Secondary Channel RX", engineConfiguration->binarySerialRxPin, PAL_MODE_ALTERNATE(TS_SERIAL_AF));
			efiSetPadMode("Secondary Channel TX", engineConfiguration->binarySerialTxPin, PAL_MODE_ALTERNATE(TS_SERIAL_AF));
#endif /* EFI_PROD_CODE */

			secondaryChannel.start(CONFIG(uartConsoleSerialSpeed));

			return &secondaryChannel;
		}
	};

	static SecondaryChannelThread secondaryChannelThread;
#endif // HAS_SECONDARY

void startSerialChannels() {
#if HAS_PRIMARY
	primaryChannelThread.Start();
#endif

#if HAS_SECONDARY
	secondaryChannelThread.Start();
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
