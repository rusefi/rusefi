#include "pch.h"
#include "kline.h"
#include "hellen_meta.h"


#ifdef EFI_KLINE
static SerialDriver* const klDriver = KLINE_SERIAL_DEVICE;
static THD_WORKING_AREA(klThreadStack, UTILITY_THREAD_STACK_SIZE);

static int totalBytes = 0;

void kLineThread(void*)
{
    while(1)
    {
        uint8_t ch = 0;
        chnReadTimeout(klDriver, &ch, 1, KLINE_READ_TIMEOUT);
        // to begin with just write byte to console
        if (ch != 0) {
            efiPrintf("kline: %c", ch);
            totalBytes++;
        }
    }
}
#endif

void initKLine() {

#ifdef EFI_KLINE
#if EFI_PROD_CODE
	efiSetPadMode("K-Line UART RX", KLINE_SERIAL_DEVICE_RX, PAL_MODE_ALTERNATE(TS_SERIAL_AF));
	efiSetPadMode("K-Line UART TX", KLINE_SERIAL_DEVICE_TX, PAL_MODE_ALTERNATE(TS_SERIAL_AF));
#endif /* EFI_PROD_CODE */

	static const SerialConfig cfg = {
		#if EFI_PROD_CODE
			.speed = KLINE_BAUD_RATE,
			.cr1 = 0,
			.cr2 = USART_CR2_STOP1_BITS | USART_CR2_LINEN,
			.cr3 = 0
		#endif // EFI_PROD_CODE
	};

	sdStart(klDriver, &cfg);

    chThdCreateStatic(klThreadStack, sizeof(klThreadStack), NORMALPRIO + 1, kLineThread, nullptr);
    addConsoleAction("kline", [](){
        efiPrintf("kline totalBytes %d", totalBytes);
    });
#endif
}
