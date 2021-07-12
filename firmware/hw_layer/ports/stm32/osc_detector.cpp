
#include "hal.h"

unsigned char detected_hse_mhz;

static void startClocksUseHsi() {
	/* PWR clock enable.*/
	RCC->APB1ENR = RCC_APB1ENR_PWREN;

  /* PWR initialization.*/
#if defined(STM32F4XX) || defined(__DOXYGEN__)
	PWR->CR = STM32_VOS;
#else
	PWR->CR = 0;
#endif

	/* HSI setup, it enforces the reset situation in order to handle possible
		problems with JTAG probes and re-initializations.*/
	RCC->CR |= RCC_CR_HSION;                  /* Make sure HSI is ON.         */
	while (!(RCC->CR & RCC_CR_HSIRDY))
	;                                       /* Wait until HSI is stable.    */

	/* HSI is selected as new source without touching the other fields in
		CFGR. Clearing the register has to be postponed after HSI is the
		new source.*/
	RCC->CFGR &= ~RCC_CFGR_SW;                /* Reset SW, selecting HSI.     */
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI)
	;                                       /* Wait until HSI is selected.  */

	/* Registers finally cleared to reset values.*/
	RCC->CR &= RCC_CR_HSITRIM | RCC_CR_HSION; /* CR Reset value.              */
	RCC->CFGR = 0;                            /* CFGR reset value.            */

	// Start LSI
	RCC->CSR |= RCC_CSR_LSION;
	while ((RCC->CSR & RCC_CSR_LSIRDY) == 0)
		;                           /* Waits until LSI is stable.               */

	// Start HSE
	RCC->CR |= RCC_CR_HSEON;
	while ((RCC->CR & RCC_CR_HSERDY) == 0)
		;                           /* Waits until HSE is stable.               */
}

uint32_t getOneCapture() {
	// wait for input capture
	while ((TIM5->SR & TIM_SR_CC1IF) == 0);

	// Return captured count
	return TIM5->CCR1;
}

uint32_t getAverageLsiCounts() {
	// Burn one count
	getOneCapture();

	uint32_t sum = 0;

	for (size_t i = 0; i < 16; i++)
	{
		sum += getOneCapture();
	}

	return sum / 16;
}

extern "C" void __core_init() {
	// Start clocks running, switch to use HSI
	startClocksUseHsi();

	// Turn on timer 5
	RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;

	// Remap to connect LSI to input capture channel 4
	TIM5->OR = TIM_OR_TI4_RMP;

	// Start TIM5
	TIM5->CR |= TIM_CR1_CEN;

	auto hsiCounts = getAverageLsiCounts();

	useHse();

	auto hseCounts = getAverageLsiCounts();

	// The external clocks's frequency is the ratio of the measured LSI speed, times HSI's speed (16MHz)
	uint32_t internalClockMhz = 16 * hseCounts / hsiCounts;

	
}
