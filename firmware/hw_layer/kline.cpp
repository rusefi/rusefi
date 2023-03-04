#include "pch.h"
#include "kline.h"
#include "hellen_meta.h"

#ifdef EFI_KLINE
static SerialDriver* const klDriver = KLINE_SERIAL_DEVICE;
static THD_WORKING_AREA(klThreadStack, UTILITY_THREAD_STACK_SIZE);

static int totalBytes = 0;
static bool kLineOutPending = false;
static int kLineOut;

void kLineThread(void*) {
    while (1) {
        uint8_t ch = 0;
        chnReadTimeout(klDriver, &ch, 1, KLINE_READ_TIMEOUT);
        // to begin with just write byte to console
        if (ch != 0) {
            efiPrintf("kline: 0x%02x", ch);
            totalBytes++;
        }
        if (kLineOutPending) {
            kLineOutPending = false;
            efiPrintf("kline OUT: 0x%02x", kLineOut);
            chnWrite(klDriver, (const uint8_t *)kLineOut, 1);
        }
    }
}
#endif // EFI_KLINE

void startKLine() {
#ifdef EFI_KLINE
#if EFI_PROD_CODE
	efiSetPadMode("K-Line UART RX", KLINE_SERIAL_DEVICE_RX, PAL_MODE_ALTERNATE(TS_SERIAL_AF));
	efiSetPadMode("K-Line UART TX", KLINE_SERIAL_DEVICE_TX, PAL_MODE_ALTERNATE(TS_SERIAL_AF));
#endif /* EFI_PROD_CODE */

	static SerialConfig cfg = {
		#if EFI_PROD_CODE
			.speed = 0,
			.cr1 = 0,
			.cr2 = USART_CR2_STOP1_BITS | USART_CR2_LINEN,
			.cr3 = 0
		#endif // EFI_PROD_CODE
	};

    if (engineConfiguration->kLineBaudRate < 100)
        engineConfiguration->kLineBaudRate = KLINE_BAUD_RATE;
    cfg.speed = engineConfiguration->kLineBaudRate;

	sdStart(klDriver, &cfg);
#endif // EFI_KLINE
}

void stopKLine() {
#ifdef EFI_KLINE
#if EFI_PROD_CODE
	efiSetPadUnused(KLINE_SERIAL_DEVICE_RX);
	efiSetPadUnused(KLINE_SERIAL_DEVICE_TX);

	sdStop(klDriver);

#endif /* EFI_PROD_CODE */
#endif // EFI_KLINE
}

void initKLine() {
#ifdef EFI_KLINE
	startKLine();

    chThdCreateStatic(klThreadStack, sizeof(klThreadStack), NORMALPRIO + 1, kLineThread, nullptr);
    addConsoleAction("kline", [](){
        efiPrintf("kline totalBytes %d", totalBytes);
    });
    addConsoleActionI("klinesend", [](int value){
        kLineOutPending = true;
        kLineOut = value;
    });

#endif // EFI_KLINE
}
