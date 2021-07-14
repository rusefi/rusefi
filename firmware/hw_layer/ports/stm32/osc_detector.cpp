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

static void useHsi() {
	// clear SW to use HSI
	RCC->CFGR &= ~RCC_CFGR_SW;
}

static void useHse() {
	// Switch to HSE clock
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_HSE;
}

static void usePll() {
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while ((RCC->CFGR & RCC_CFGR_SWS) != (STM32_SW << 2));
}

static uint32_t getOneCapture() {
	// wait for input capture
	while ((TIM5->SR & TIM_SR_CC4IF) == 0);

	// Return captured count
	return TIM5->CCR4;
}

static uint32_t getAverageLsiCounts() {
	// Burn one count
	getOneCapture();

	uint32_t firstCapture = getOneCapture();
	uint32_t lastCapture;

	for (size_t i = 0; i < 20; i++)
	{
		lastCapture = getOneCapture();
	}

	return lastCapture - firstCapture;
}

// This only works if you're using the PLL as the configured clock source!
static_assert(STM32_SW == RCC_CFGR_SW_PLL);

// These clocks must all be enabled for this to work
static_assert(STM32_HSI_ENABLED);
static_assert(STM32_LSI_ENABLED);
static_assert(STM32_HSE_ENABLED);

static void reprogramPll(uint8_t pllM) {
	// Switch back to HSI to configure PLL
	useHsi();

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
	usePll();
}

// __late_init runs after bss/zero initialziation, but before static constructors and main
extern "C" void __late_init() {
	// Turn on timer 5
	RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;

	// Remap to connect LSI to input capture channel 4
	TIM5->OR = TIM_OR_TI4_RMP_0;

	// Enable capture on channel 4
	TIM5->CCMR2 = TIM_CCMR2_CC4S_0;
	TIM5->CCER = TIM_CCER_CC4E;

	// Start TIM5
	TIM5->CR1 |= TIM_CR1_CEN;

	// Use HSI
	useHsi();

	// Measure LSI against HSI
	auto hsiCounts = getAverageLsiCounts();

	useHse();

	// Measure LSI against HSE
	auto hseCounts = getAverageLsiCounts();

	// Turn off timer 5 now that we're done with it
	RCC->APB1ENR &= ~RCC_APB1ENR_TIM5EN;

	// The external clocks's frequency is the ratio of the measured LSI speed, times HSI's speed (16MHz)
	constexpr float hsiMhz = STM32_HSICLK * 1e-6;
	float hseFrequencyMhz = hsiMhz * hseCounts / hsiCounts;

	uint8_t pllMValue = efiRound(hseFrequencyMhz, 1);

	reprogramPll(pllMValue);
}

#endif // defined ENABLE_AUTO_DETECT_HSE
