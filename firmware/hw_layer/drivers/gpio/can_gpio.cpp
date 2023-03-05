/**
 * can_gpio.cpp
 */

#include "pch.h"
#include "can_gpio.h"

static THD_WORKING_AREA(canGpio_thread_1_wa, 256);

static THD_FUNCTION(canGpio_driver_thread, p) {

	(void)p;

	chRegSetThreadName("CanGpio");
}

#if EFI_CAN_GPIO
void initCanGpio() {
    chThdCreateStatic(canGpio_thread_1_wa, sizeof(canGpio_thread_1_wa),
						  PRIO_GPIOCHIP, canGpio_driver_thread, NULL);
}
#endif // EFI_CAN_GPIO