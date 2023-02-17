#include "pch.h"
#include "kline.h"


static SerialDriver* const klDriver = KLINE_SERIAL_DEVICE;
static THD_WORKING_AREA(klThreadStack, UTILITY_THREAD_STACK_SIZE);

void kLineThread(void*)
{
    while(1)
    {
        uint8_t ch = 0;
        chnReadTimeout(klDriver, &ch, 1, KLINE_READ_TIMEOUT);
        // to begin with just write byte to console
        if (ch != 0) {
            efiPrintf("kline: %c", ch);
        }
    }
}

void initKLine() {

#ifdef EFI_KLINE
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
#endif
}