/**
 * @file    board.c
 * @brief   Board initialization file.
 * @author  andreika <prometheus.pcb@gmail.com>
 */
 
/* This is a template for board specific configuration created by MCUXpresso IDE Project Wizard.*/

#include <stdint.h>
#include "board.h"
#include "hal.h"

/**
 * @brief Set up and initialize all required blocks and functions related to the board hardware.
 */
void BOARD_InitDebugConsole(void) {
	/* The user initialization should be placed here */
}

void delay(void)
{
    volatile uint32_t i = 0;
    for (i = 0; i < 800000; ++i)
    {
        __asm("NOP"); /* delay */
    }
}

/* Test LED blinker (Uses PD7). Should work in any conditions! */
void __blink(int n) {
#if 1
	PCC->CLKCFG[PCC_PORTD_INDEX] |= PCC_CLKCFG_CGC_MASK; // enable clock on PORT D
    PORTD->PCR[7U] = (PORTD->PCR[7U] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(1/*kPORT_MuxAsGpio*/);
	GPIOD->PSOR = 1U << 7U;
    GPIOD->PDDR |= (1U << 7U);
    int k;
    for (k = 0; k < 1; k++) {
    	GPIOD->PCOR = 1U << 7U;
   		for (int i = 0; i < 2*n; i++)
		{
	        GPIOD->PTOR = (1U << 7U);
    	    delay();
    	}
    	GPIOD->PSOR = 1U << 7U;

    	for (int kk = 0; kk < 8; kk++) {
	    	delay();
	    }
    }
/*
   	for (k = 0; k < 10; k++) {
    	delay();
    }
*/
#endif
}

void disableWatchdog(void) {
  WDOG->CNT = WDOG_UPDATE_KEY;
  WDOG->TOVAL = 0xFFFF;
  WDOG->CS = (uint32_t) ((WDOG->CS) & ~WDOG_CS_EN_MASK) | WDOG_CS_UPDATE_MASK;
}

void __early_init(void) {
	ke1xf_init();
}

void __late_init(void) {
	// we need static variables to be already initialized
	// to configure the clock properly and save its state
	ke1xf_clock_init(KINETIS_DEFAULT_CLK);
}

void boardInit(void) {
}
