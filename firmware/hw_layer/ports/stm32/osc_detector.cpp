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

#include "pch.h"

#ifdef ENABLE_AUTO_DETECT_HSE

float hseFrequencyMhz;
uint8_t autoDetectedRoundedMhz;

#ifdef STM32H7XX
#define TIMER TIM17
#else // not H7
#define TIMER TIM11
#endif

static uint32_t getOneCapture() {
	// wait for input capture
	while ((TIMER->SR & TIM_SR_CC1IF) == 0);

	// Return captured count
	return TIMER->CCR1;
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

// These clocks must all be enabled for this to work
static_assert(STM32_HSI_ENABLED);
static_assert(STM32_HSE_ENABLED);

#ifdef STM32H7XX
static constexpr float rtcpreDivider = 63;

static void enableTimer() {
	RCC->APB2ENR |= RCC_APB2ENR_TIM17EN;
}

static void disableTimer() {
	RCC->APB2ENR &= RCC_APB2ENR_TIM17EN;
}

static void reprogramPll(uint8_t roundedHseMhz) {
	// Switch system clock to HSI to configure PLL (SW = 0)
	RCC->CFGR &= ~RCC_CFGR_SW;

	// Stop all 3 PLLs
	RCC->CR &= ~(RCC_CR_PLL1ON | RCC_CR_PLL2ON | RCC_CR_PLL3ON);

	// H7 is configured for 2MHz input to PLL
	auto pllm = roundedHseMhz / 2;
	
	// Set PLLM for all 3 PLLs to the new value, and select HSE as the clock source
	RCC->PLLCKSELR = 
		pllm << RCC_PLLCKSELR_DIVM1_Pos |
		pllm << RCC_PLLCKSELR_DIVM2_Pos |
		pllm << RCC_PLLCKSELR_DIVM3_Pos |
		RCC_PLLCKSELR_PLLSRC_HSE;

	// Enable PLLs
	RCC->CR |= RCC_CR_PLL1ON | RCC_CR_PLL2ON | RCC_CR_PLL3ON;

	// Wait for PLLs to lock
	auto readyMask = RCC_CR_PLL1RDY | RCC_CR_PLL2RDY | RCC_CR_PLL3RDY;
	while ((RCC->CR & readyMask) != readyMask) ;

	// Switch system clock source back to PLL
	RCC->CFGR |= RCC_CFGR_SW_PLL1;
}
#else // not STM32H7

static constexpr float rtcpreDivider = 31;

// This only works if you're using the PLL as the configured clock source!
static_assert(STM32_SW == RCC_CFGR_SW_PLL);

static void enableTimer() {
	RCC->APB2ENR |= RCC_APB2ENR_TIM11EN;
}

static void disableTimer() {
	RCC->APB2ENR &= ~RCC_APB2ENR_TIM11EN;
}

static void reprogramPll(uint8_t roundedHseMhz) {
	// Switch back to HSI to configure PLL
	// clear SW to use HSI
	RCC->CFGR &= ~RCC_CFGR_SW;

	// Stop the PLL
	RCC->CR &= ~RCC_CR_PLLON;

	// Mask out the old PLLM and PLLSRC
	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLM_Msk | RCC_PLLCFGR_PLLSRC_Msk);

	// Stick in the new PLLM value
	RCC->PLLCFGR |= (roundedHseMhz << RCC_PLLCFGR_PLLM_Pos) & RCC_PLLCFGR_PLLM_Msk;
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
#endif

// __late_init runs after bss/zero initialziation, but before static constructors and main
extern "C" void __late_init() {
	// Set RTCPRE to /31 - just set all the bits
	RCC->CFGR |= RCC_CFGR_RTCPRE_Msk;

	// Turn on timer
	enableTimer();

	// Remap to connect HSERTC to CH1
#ifdef STM32H7XX
	// TI1SEL = 2, HSE_1MHz
	TIMER->TISEL = TIM_TISEL_TI1SEL_1;
#elif defined(STM32F4XX)
	TIMER->OR = TIM_OR_TI1_RMP_1;
#else
	// the definition has a different name on F7 for whatever reason
	TIMER->OR = TIM11_OR_TI1_RMP_1;
#endif

	// Enable capture on channel 1
	TIMER->CCMR1 = TIM_CCMR1_CC1S_0;
	TIMER->CCER = TIM_CCER_CC1E;

	// Start timer
	TIMER->CR1 |= TIM_CR1_CEN;

	// Measure HSE against SYSCLK
	auto hseCounts = getTimerCounts(10);

	// Turn off timer now that we're done with it
	disableTimer();

	float hseFrequencyHz = 10 * rtcpreDivider * STM32_TIMCLK2 / hseCounts;

	hseFrequencyMhz = hseFrequencyHz / 1e6;
	autoDetectedRoundedMhz = efiRound(hseFrequencyMhz, 1);

	reprogramPll(autoDetectedRoundedMhz);
}

#endif // defined ENABLE_AUTO_DETECT_HSE
