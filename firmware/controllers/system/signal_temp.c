/**
 * @file	signal_temp.c
 *
 * Work in progress. https://sourceforge.net/p/rusefi/tickets/24/
 *
 * Here we have a 1MHz timer dedicated to event scheduling. We are using one of the 32-bit timers here,
 * so this timer can schedule events up to 4B/100M = 4000 seconds from now.
 *
 * @date Apr 14, 2014
 * @author Andrey Belomutskiy, (c) 2012-2013
 */

#include "main.h"
#include "signal_executor.h"
#include "signal_temp.h"

// https://my.st.com/public/STe2ecommunities/mcu/Lists/cortex_mx_stm32/Flat.aspx?RootFolder=https%3a%2f%2fmy.st.com%2fpublic%2fSTe2ecommunities%2fmcu%2fLists%2fcortex_mx_stm32%2fInterrupt%20on%20CEN%20bit%20setting%20in%20TIM7&FolderCTID=0x01200200770978C69A1141439FE559EB459D7580009C4E14902C3CDE46A77F0FFD06506F5B&currentviews=474

#if EFI_PROD_CODE

int globalCounter = 0;

static TIM_TypeDef *TIM = TIM5;

schfunc_t globalTimerCallback;

void setTimer(int timeUs) {
	TIM->ARR = timeUs - 1;
	TIM->EGR |= TIM_EGR_UG; // generate an update event to reload timer's counter value
	TIM->CR1 |= TIM_CR1_CEN; // restart timer
}

static void callback(void) {
	GPIOD ->ODR ^= (1 << 13);   // Toggle D13

	globalCounter++;

	if (globalTimerCallback == NULL) {
		firmwareError("NULL globalTimerCallback");
		return;
	}
	globalTimerCallback(NULL);
//	if (globalCounter < 6) {
//	setTimer(100000);
//	}
}

// if you decide to move this to .cpp do not forget to make that a C method
CH_IRQ_HANDLER(STM32_TIM5_HANDLER) {
	CH_IRQ_PROLOGUE();
	if (((TIM->SR & 0x0001) != 0) && ((TIM->DIER & 0x0001) != 0)) {
		callback();
	}
	TIM->SR = (int) ~STM32_TIM_SR_UIF;   // Interrupt has been handled
	CH_IRQ_EPILOGUE();
}

void TIM_Init(void) {
//	if (1==1)
//		return; // something is not right with this code :(

	RCC ->APB1ENR |= RCC_APB1ENR_TIM5EN;   // Enable TIM5 clock
//	NVIC_EnableIRQ(TIM5_IRQn);   // Enable TIM5 IRQ
	nvicEnableVector(TIM5_IRQn, CORTEX_PRIORITY_MASK(12));
	TIM->DIER |= TIM_DIER_UIE;   // Enable interrupt on update event
	TIM->CR1 |= TIM_CR1_OPM; // one pulse mode: count down ARR and stop
	TIM->CR1 &= ~TIM_CR1_ARPE; /* ARR register is NOT buffered, allows to update timer's period on-fly. */

	TIM->PSC = 84 - 1;   // 168MHz / 2 / 84 = 1MHz, each tick is a microsecond

//	setTimer(100000);
}

#endif /* EFI_PROD_CODE */
