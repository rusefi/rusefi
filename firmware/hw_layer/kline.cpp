#include "pch.h"
#include "kline.h"

#if EFI_PROD_CODE
static THD_WORKING_AREA(klThreadStack, UTILITY_THREAD_STACK_SIZE);

void kLineThread(void*)
{
    while(1)
    {
        // todo: read one by in loop
        // to begin with just write byte to console
        // efiPrintf
        chThdSleepMilliseconds(50);
    }
}
#endif

void initKLine() {

#ifdef EFI_KLINE
    chThdCreateStatic(klThreadStack, sizeof(klThreadStack), NORMALPRIO + 1, kLineThread, nullptr);
    // todo: conditional uart initialization matching 2003 miata k-line
#endif
}
