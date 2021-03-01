#include "hal.h"
#include "stm32_gpio.h"

#define PROMETHEUS_LED 13

// BLIIINK!!!!!!!!!!
void BLIIINK(int t) {
	int i, j;
	palSetPadMode(GPIOA, PROMETHEUS_LED, PAL_MODE_OUTPUT_PUSHPULL);
	//while (true) {
	for (j = 0; j < 2; j++) {
		palClearPad(GPIOA, PROMETHEUS_LED);
    	for (i = 0; i < 2*t; i++) {
	    	palTogglePad(GPIOA, PROMETHEUS_LED);
    		chThdSleepMilliseconds(250);
    	}
    	chThdSleepMilliseconds(1000);
    }
}

