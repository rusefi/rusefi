/**
 * @file    board.c
 * @brief   Board initialization file.
 * @author  andreika <prometheus.pcb@gmail.com>
 */

/* This is a template for board specific configuration created by MCUXpresso IDE Project Wizard.*/

#include <stdint.h>
#include "pdl_header.h"

void delay(void)
{
    volatile uint32_t i = 0;
    for (i = 0; i < 800000; ++i)
    {
        __asm("NOP"); /* delay */
    }
}

void __early_init(void) {
	SystemInit();
}

void __late_init(void) {
	// we need static variables to be already initialized
	// to configure the clock properly and save its state
	//ke1xf_clock_init(KINETIS_DEFAULT_CLK);
}

void boardInit(void) {
}
