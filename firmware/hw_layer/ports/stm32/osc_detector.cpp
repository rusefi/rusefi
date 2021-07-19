/**
 * @file	osc_detector.cpp
 * @brief This logic automatically detects the speed of the
 *        oscillator or crystal connected to HSE.
 * @date 12 July 2021
 * 
 * It works by first using the reasonably-precise HSI oscillator (16MHz) to measure LSI (nominally 32khz, but wide tolerance).
 * Then, it switches the system clock source to HSE, and repeats the same measurement.  The inaccurate LSI will not drift
 * significantly in the short period of time between these two measurements, so use it as a transfer standard to compare the speed
 * of HSI and HSE.  The ratio between the measured speed of LSI when running on HSE vs. HSI will give the ratio of speeds of HSE
 * and HSI themselves.  Since we know the value of HSI (16mhz), we can compute the speed of HSE.
 * 
 * Lastly, the PLL is reconfigured to use the correct input divider such that the input frequency is 1MHz
 * (PLLM is set to N for an N-MHz HSE crystal).
 */

#include "hal.h"
#include "efilib.h"

#ifdef ENABLE_AUTO_DETECT_HSE

float hseFrequencyMhz;
uint8_t autoDetectedPllMValue;

static uint32_t getOneCapture() {
	// wait for input capture
	while ((TIM11->SR & TIM_SR_CC1IF) == 0);

	// Return captured count
	return TIM11->CCR1;
}

static uint32_t getTimerCounts(size_t count) {
	// Burn one count
	getOneCapture();

	uint32_t firstCapture = getOneCapture();
	uint32_t lastCapture;

	for (size_t i = 0; i < count; i++)
	{
		lastCapture = getOneCapture();
	}

	return lastCapture - firstCapture;
}

// This only works if you're using the PLL as the configured clock source!
static_assert(STM32_SW == RCC_CFGR_SW_PLL);

// These clocks must all be enabled for this to work
static_assert(STM32_HSI_ENABLED);
static_assert(STM32_HSE_ENABLED);

static void reprogramPll(uint8_t pllM) {
	// Switch back to HSI to configure PLL
	// clear SW to use HSI
	RCC->CFGR &= ~RCC_CFGR_SW;

	// Stop the PLL
	RCC->CR &= ~RCC_CR_PLLON;

	// Mask out the old PLLM and PLLSRC
	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLM_Msk | RCC_PLLCFGR_PLLSRC_Msk);

	// Stick in the new PLLM value
	RCC->PLLCFGR |= (pllM << RCC_PLLCFGR_PLLM_Pos) & RCC_PLLCFGR_PLLM_Msk;
	// Set PLLSRC to HSE
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE;

	// Reenable PLL, wait for lock
	RCC->CR |= RCC_CR_PLLON;
	while (!(RCC->CR & RCC_CR_PLLRDY));

	// Switch clock source back to PLL
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while ((RCC->CFGR & RCC_CFGR_SWS) != (STM32_SW << 2));
}

// __late_init runs after bss/zero initialziation, but before static constructors and main
extern "C" void __late_init() {
	// Set RTCPRE to /31 - just set all the bits
	RCC->CFGR |= RCC_CFGR_RTCPRE_Msk;

	// Turn on timer 5
	RCC->APB2ENR |= RCC_APB2ENR_TIM11EN;

	// Remap to connect HSERTC to TIM11 CH1
	TIM11->OR = TIM_OR_TI1_RMP_1;

	// Enable capture on channel 4
	TIM11->CCMR1 = TIM_CCMR1_CC1S_0;
	TIM11->CCER = TIM_CCER_CC1E;

	// Start TIM11
	TIM11->CR1 |= TIM_CR1_CEN;

	// Measure HSE against SYSCLK
	auto hseCounts = getTimerCounts(10);

	// Turn off timer 11 now that we're done with it
	RCC->APB2ENR &= ~RCC_APB2ENR_TIM11EN;

	float hseFrequencyHz = 10 * 31.0f * STM32_SYSCLK / hseCounts;

	hseFrequencyMhz = hseFrequencyHz / 1e6;
	autoDetectedPllMValue = efiRound(hseFrequencyMhz, 1);

	reprogramPll(autoDetectedPllMValue);
}

#endif // defined ENABLE_AUTO_DETECT_HSE
