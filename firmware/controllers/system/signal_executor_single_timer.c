/**
 * @file	signal_executor_single_timer.c
 * @brief Single timer based implementation of signal executor.
 *
 * All active outputs are stored in "output_list".
 * Closest in time output event is loaded in timer TIM7.
 * In TIM7 timer's interrupt next closest in time output event is found and so on.
 * This algorithm should reduce jitter in output signal.
 *
 * @todo Add test code.
 *
 * @date Nov 27, 2013
 * @author Dobrin Georgiev
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include <stdint.h>
#include "hal.h"
#include "utlist.h"
#include "signal_executor.h"
#include "signal_executor_single_timer_algo.h"

#if EFI_WAVE_ANALYZER
#include <stm32_tim.h>
#include "wave_chart.h"
/**
 * Signal executors feed digital events right into WaveChart used by Sniffer tab of Dev Console
 */
extern WaveChart waveChart;
#endif /* EFI_WAVE_ANALYZER */

#if EFI_SIGNAL_EXECUTOR_SINGLE_TIMER || defined(__DOXYGEN__)
static Mutex mtx; /* Mutex declaration */

/**
 * @brief Output list
 *
 * List of all active output signals
 * This is actually the head of the list.
 * When the list is empty (initial state) the head of the list should be NULL.
 * This is by design.
 */
static OutputSignal *output_list = NULL;
#if 0
/**
 * @brief Schedule output
 *
 * Set new parameters to output event.
 * When to become active and how long to stay active.
 *
 * @param [in, out] signal Signal related to an output.
 * @param [in] delay the number of ticks before the output signal immediate output if delay is zero.
 * @param [in] dwell the number of ticks of output duration.
 */
void scheduleOutput(OutputSignal *signal, int delay, int duration) {
	chDbgCheck(signal->initialized, "Signal not initialized");
	if(duration < 0) {
		firmwareError("duration cannot be negative: %d", duration)
		return;
	}

	signal->duration = duration;
	signal->offset = delay;

	/* generate an update event to reload timer's counter value, according to new set of output timings */
	TIM7->EGR |= TIM_EGR_UG;
}
#endif
void scheduleTask(scheduling_s *scheduling, int delay, schfunc_t callback, void *param)
{
//	return;
	time_t now;

	chMtxLock(&mtx);
#if 0
	static bool val = 0;

	val ^= 1;
	palWritePad(GPIOE, 5, val);
#endif
#if 1
	now = chTimeNow();
	scheduling->moment = now + delay;

	/* generate an update event to reload timer's counter value, according to new set of output timings */
	TIM7->EGR |= TIM_EGR_UG;
#endif
	chMtxUnlock();
}

#define SOURCE_DIVIDER ((STM32_HCLK) / (STM32_PCLK1))
//#pragma message VAR_NAME_VALUE(DIVIDER)
#define PRESCALLER (84)
/* Convert chSys tick to CPU tick */
#define ST2CT(st) ((st) * ((STM32_SYSCLK) / (CH_FREQUENCY)))
//#pragma message VAR_NAME_VALUE(STM32_SYSCLK)
#define CT2TT(ct) ((ct) / (SOURCE_DIVIDER) / (PRESCALLER))
#define MS2TT CT2TT(ST2CT(MS2ST(1)))
//#pragma message VAR_NAME_VALUE(MS2TT)
/**
 * @brief Initialization of output scheduler.
 *
 * TIM7 timer initial settings.
 */
void initOutputScheduler(void)
{
	/**
	 * @brief TIM7 initialization. Timer should run at 42MHz and before any output event registered should generate ISR event each millisecond.
	 */
	uint32_t initial_interval = CT2TT(ST2CT(100))-7; /// in timer ticks
	//uint32_t initial_interval = CT2TT(ST2CT(MS2ST(1))); /// in timer ticks
	//uint32_t initial_interval = 42000-1; /// in timer ticks
	//uint32_t initial_interval = (42000/PRESCALLER)-1; /// in timer ticks
	//uint32_t initial_interval = (500)-1; /// in timer ticks
#if 0
#pragma message VAR_NAME_VALUE(STM32_PCLK1)" <= "VAR_NAME_VALUE(STM32_PCLK1_MAX)
#pragma message VAR_NAME_VALUE(STM32_PPRE1)
#endif
#if 0
	TIM2->PSC = STM32_PCLK1*2 / ((1<<16) * RC_PWM_FREQ); //50Hz frequency
	TIM2->CCMR1 |= TIM_CCMR1_CC1S_0;//channel 1 = input on T1
	TIM2->DIER |= TIM_DIER_CC1IE;//enable capture/compare interrupt
	TIM2->CCER |= TIM_CCER_CC1E;//Capture/Compare output enable
	nvicEnableVector(TIM2_IRQn, 12);
	TIM2->CR1 |= TIM_CR1_CEN;//counter enable
#endif
	RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;
	nvicEnableVector(TIM7_IRQn, CORTEX_PRIORITY_MASK(2));
	TIM7->ARR = initial_interval; /* Timer's period */
	TIM7->PSC = PRESCALLER;
	TIM7->CR1 &= ~STM32_TIM_CR1_ARPE; /* ARR register is NOT buffered, allows to update timer's period on-fly. */
	TIM7->DIER |= STM32_TIM_DIER_UIE; /* Interrupt enable */
	TIM7->CR1 |= STM32_TIM_CR1_CEN; /* Counter enable */
	chMtxInit(&mtx); /* Mutex initialization before use */
}

/**
 * @brief Timer7 IRQ handler
 *
 * This is the core of "Single Timer" signal executor.
 * Each time closest in time output event is found and TIM7 is loaded to generate IRQ at exact moment.
 * FAST IRQ handler is used to minimize the jitter in output signal, caused by RTOS switching threads and by busy threads.
 * Timer7 is used as output scheduler (to drive all outputs - spark plugs and fuel injectors).
 */
CH_FAST_IRQ_HANDLER(STM32_TIM7_HANDLER)
{
#define MIN(a, b) (((a) > (b)) ? (b) : (a))
#define GET_DURATION(o) ((o)->status ? (o)->signalTimerDown.moment : (o)->signalTimerUp.moment)

	OutputSignal *out;
	time_t next; /* Time to next output event */
	time_t now;
#if 1
	static bool val = 0;
	static uint8_t cnt = 0;

	cnt++;
	cnt %= 2;
	if (0 == cnt) {
		val ^= 1;
		palWritePad(GPIOE, 5, val);
	}
#else
	now = chTimeNow();
	next = 0xFFFF;
	LL_FOREACH(output_list, out) {
		time_t n = toggleSignalIfNeeded(out, now);
		/* Find closest output event in time */
		next = MIN(next, n);
	}
	if (0xFFFF != next) {
		TIM7->ARR = CT2TT(ST2CT(next)); /* Update scheduler timing */
	}
#endif
	TIM7->SR &= ~STM32_TIM_SR_UIF; /* Reset interrupt flag */
}

/**
 * @brief Initialize output signal.
 *
 * @param [in] name Signal name.
 * @param [in, out] signal Output signal.
 * @param [in] led LED
 * @param [in] xor Option to invert output signal.
 */
void initOutputSignal(OutputSignal *signal, io_pin_e ioPin)
{
	//initLogging(&signal->logging, name);

	signal->name = getPinName(ioPin);
	signal->io_pin = ioPin;
	signal->status = IDLE;
//	signal->duration = 0;
	signal->last_scheduling_time = 0;
	registerSignal(signal);
	signal->initialized = TRUE;
}
#endif /* EFI_SIGNAL_EXECUTOR_SINGLE_TIMER */
