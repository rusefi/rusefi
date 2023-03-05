/**
 * can_gpio.cpp
 */

#include "pch.h"
#include "gpio/gpio_ext.h"
#include "can_gpio.h"

#if EFI_CAN_GPIO

static SEMAPHORE_DECL(wakeSemaphore, 3 /* todo: is that right number? probably not. */);
static THD_WORKING_AREA(canGpio_thread_1_wa, 256);

struct CanOutputs : public GpioChip {
    uint8_t state[4];

    CanOutputs() {
        memset(&state, 0, sizeof(state));
    }

    void wakeThread() {
	    /* Entering a reentrant critical zone.*/
	    chibios_rt::CriticalSectionLocker csl;

	    chSemSignalI(&wakeSemaphore);
    }

	/* pin argument is pin number within gpio chip, not a global number */
    int writePad(size_t pin, int value) override {
        state[pin] = value;
        wakeThread();
    	return 0;
    }
};

static THD_FUNCTION(canGpio_driver_thread, p) {

	(void)p;

	chRegSetThreadName("CanGpio");
    while (1) {
       	chSemWaitTimeout(&wakeSemaphore, TIME_MS2I(1200 /*random number, ms*/));
    }
}

void initCanGpio() {
// CAN_PIN_0

    chThdCreateStatic(canGpio_thread_1_wa, sizeof(canGpio_thread_1_wa),
						  PRIO_GPIOCHIP, canGpio_driver_thread, NULL);
}
#endif // EFI_CAN_GPIO