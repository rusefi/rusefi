/**
 * @file	microsecond_timer.cpp
 *
 * Here we have a 1MHz timer dedicated to event scheduling. We are using one of the 32-bit timers here,
 * so this timer can schedule events up to 4B/100M ~ 4000 seconds ~ 1 hour from current time.
 *
 * GPT5 timer clock: 84000000Hz
 * If only it was a better multiplier of 2 (84000000 = 328125 * 256)
 *
 * @date Apr 14, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "microsecond_timer.h"
#include "port_microsecond_timer.h"

#if EFI_PROD_CODE

#include "periodic_thread_controller.h"

// Just in case we have a mechanism to validate that hardware timer is clocked right and all the
// conversions between wall clock and hardware frequencies are done right
// delay in milliseconds
#define TEST_CALLBACK_DELAY_MS 10
// if hardware timer is 20% off we throw a critical error and call it a day
// maybe this threshold should be 5%? 10%?
#define TIMER_PRECISION_THRESHOLD 0.2

/**
 * Maximum duration of complete timer callback, all pending events together
 * See also 'maxEventCallbackDuration' for maximum duration of one event
 */
uint32_t maxPrecisionCallbackDuration = 0;

static efitick_t lastSetTimerTimeNt;
static bool isTimerPending = false;

static int timerCallbackCounter = 0;
static int timerRestartCounter = 0;

static int timerFreezeCounter = 0;
static int setHwTimerCounter = 0;
static bool hwStarted = false;

/**
 * sets the alarm to the specified number of microseconds from now.
 * This function should be invoked under kernel lock which would disable interrupts.
 */
void setHardwareSchedulerTimer(efitick_t nowNt, efitick_t setTimeNt) {
	criticalAssertVoid(hwStarted, "HW.started");

	setHwTimerCounter++;

	// How many ticks in the future is this event?
	const auto timeDeltaNt = setTimeNt - nowNt;

	/**
	 * #259 BUG error: not positive deltaTimeNt
	 * Once in a while we night get an interrupt where we do not expect it
	 */
	if (timeDeltaNt <= 0) {
		timerFreezeCounter++;
		warning(ObdCode::CUSTOM_OBD_LOCAL_FREEZE, "local freeze cnt=%d", timerFreezeCounter);
	}

	// We need the timer to fire after we return - too close to now may actually schedule in the past
	if (timeDeltaNt < US2NT(2)) {
		setTimeNt = nowNt + US2NT(2);
	} else if (timeDeltaNt >= TOO_FAR_INTO_FUTURE_NT) {
		uint32_t delta32;
		if (timeDeltaNt > UINT32_MAX) {
			delta32 = UINT32_MAX;
		} else {
			delta32 = timeDeltaNt;
		}

		// we are trying to set callback for too far into the future. This does not look right at all
		firmwareError(ObdCode::CUSTOM_ERR_TIMER_OVERFLOW, "setHardwareSchedulerTimer() too far: %lu", delta32);
		return;
	}

	// Always re-arm the hardware timer, even under a firmware error. The ISR
	// disables the channel notification (CC1IE) before invoking the callback, and
	// portSetHardwareSchedulerTimer() below is the only code that re-enables it, so
	// skipping the arm here would permanently kill the whole event scheduler (soft
	// PWM, watchdog buddy, every scheduled event) until the next reboot. Engine
	// safety after a fatal error is handled by LimpManager::fatalError() cutting
	// ignition/injection/ETB - not by stopping the scheduler.

	// Do the actual hardware-specific timer set operation
	portSetHardwareSchedulerTimer(nowNt, setTimeNt);

	lastSetTimerTimeNt = getTimeNowNt();
	isTimerPending = true;
	timerRestartCounter++;
}

void globalTimerCallback();

void portMicrosecondTimerCallback() {
	timerCallbackCounter++;
	isTimerPending = false;

	uint32_t before = getTimeNowLowerNt();
	globalTimerCallback();
	uint32_t precisionCallbackDuration = getTimeNowLowerNt() - before;
	if (precisionCallbackDuration > maxPrecisionCallbackDuration) {
		maxPrecisionCallbackDuration = precisionCallbackDuration;
	}
}

struct MicrosecondTimerWatchdogController : public PeriodicController<TIMER_WATCHDOG_THREAD_STACK_SIZE> {
	MicrosecondTimerWatchdogController()
		: PeriodicController("MstWatchdog", NORMALPRIO, 2)
	{
	}

	void PeriodicTask(efitick_t nowNt) override {
		// 2 seconds of inactivity would not look right
		if (nowNt > lastSetTimerTimeNt + MS2NT(2000)) {
#ifdef SCHEDULER_TIMER_DEVICE
			// Snapshot the hardware timer state at the moment of detection to tell apart:
			// - timer clock dead (CNT frozen),
			// - compare interrupt dead (DIER bit 1 / CC1IE cleared, CNT still counting),
			// - notification never re-armed after last fire (isTimerPending false).
			uint32_t cnt1 = SCHEDULER_TIMER_DEVICE->CNT;

			// two adjacent CNT reads can be identical just because a few CPU cycles
			// elapsed between them - sleep so the counter really has time to advance:
			// cnt2 == cnt1 after 100 ms proves the counter is frozen, not just slow
			chThdSleepMilliseconds(100);
			uint32_t cnt2 = SCHEDULER_TIMER_DEVICE->CNT;

			// TIM5 peripheral clock gate: a cleared bit means someone called
			// pwmStop()/gptStop()/icuStop() on the scheduler timer handle, which
			// gates the clock via rccDisableTIM5 in the TIMv1 LLD. The RCC register
			// name differs between families (APB1LENR on H7, APB1ENR elsewhere).
			uint32_t apb1enr = 0;
			uint32_t tim5en = 0;
#if defined(AT32F4XX) || defined(STM32F4XX) || defined(STM32F7XX)
			apb1enr = RCC->APB1ENR;
			tim5en = apb1enr & RCC_APB1ENR_TIM5EN;
#elif defined(STM32H7XX)
			apb1enr = RCC->APB1LENR;
			tim5en = apb1enr & RCC_APB1LENR_TIM5EN;
#endif

			// Live register dump, printed every watchdog period while the error holds.
			// The latched firmwareError below is capped at CRITICAL_BUFFER_SIZE=120
			// bytes, so the full register picture has to go over the 256-byte console
			// path instead. ccer bit 0 (CC1E) and ccmr1 bits 6:4 (OC1M) tell whether
			// the compare channel itself is still enabled.
			efiPrintf("WDT regs: cnt=%u..%u(+%u) dier=0x%x sr=0x%x cr1=0x%x smcr=0x%x psc=0x%x arr=0x%x ccr1=0x%x ccmr1=0x%x ccer=0x%x apb1enr=0x%x tim5en=%u",
				(unsigned)cnt1, (unsigned)cnt2, (unsigned)(cnt2 - cnt1),
				(unsigned)SCHEDULER_TIMER_DEVICE->DIER, (unsigned)SCHEDULER_TIMER_DEVICE->SR,
				(unsigned)SCHEDULER_TIMER_DEVICE->CR1, (unsigned)SCHEDULER_TIMER_DEVICE->SMCR,
				(unsigned)SCHEDULER_TIMER_DEVICE->PSC, (unsigned)SCHEDULER_TIMER_DEVICE->ARR,
				(unsigned)SCHEDULER_TIMER_DEVICE->CCR1, (unsigned)SCHEDULER_TIMER_DEVICE->CCMR1,
				(unsigned)SCHEDULER_TIMER_DEVICE->CCER, (unsigned)apb1enr, (unsigned)tim5en);

			if (!m_errorReported) {
				m_errorReported = true;

				// Reconstruct the moment the ISR->reschedule chain stopped from the
				// ring buffer kept by the healthy-path branch below
				for (int i = 0; i < m_sampleCount; i++) {
					int idx = (m_sampleIndex + i) % m_sampleCount;
					if (!m_sampleValid[idx]) {
						continue;
					}
					const auto& s = m_samples[idx];
					efiPrintf("WDT hist: cnt=%u isr=%d setHw=%d freeze=%d pend=%u sr=0x%x dier=0x%x ccr1=0x%x cr1=0x%x",
						(unsigned)s.cnt, s.isr, s.setHw, s.freeze, (unsigned)s.pending,
						(unsigned)s.sr, (unsigned)s.dier, (unsigned)s.ccr1, (unsigned)s.cr1);
				}

				firmwareError(ObdCode::RUNTIME_CRITICAL_TIMER_WATCHDOG,
					"Watchdog: no events for 2s! isr=%d setHw=%d pending=%d cnt=%u..%u dier=0x%x sr=0x%x cr1=0x%x",
					timerCallbackCounter, setHwTimerCounter, (int)isTimerPending,
					(unsigned)cnt1, (unsigned)cnt2,
					(unsigned)SCHEDULER_TIMER_DEVICE->DIER, (unsigned)SCHEDULER_TIMER_DEVICE->SR,
					(unsigned)SCHEDULER_TIMER_DEVICE->CR1);
			}
#else
			firmwareError(ObdCode::RUNTIME_CRITICAL_TIMER_WATCHDOG, "Watchdog: no events for 2 seconds!");
#endif
		} else {
#ifdef SCHEDULER_TIMER_DEVICE
			// Healthy path - keep a short recent history so the error dump can show
			// exactly when the ISR->reschedule chain stopped and whether the counter
			// kept advancing afterwards
			auto& s = m_samples[m_sampleIndex % m_sampleCount];
			s.cnt = SCHEDULER_TIMER_DEVICE->CNT;
			s.isr = timerCallbackCounter;
			s.setHw = setHwTimerCounter;
			s.freeze = timerFreezeCounter;
			s.pending = isTimerPending ? 1 : 0;
			s.sr = SCHEDULER_TIMER_DEVICE->SR;
			s.dier = SCHEDULER_TIMER_DEVICE->DIER;
			s.ccr1 = SCHEDULER_TIMER_DEVICE->CCR1;
			s.cr1 = SCHEDULER_TIMER_DEVICE->CR1;
			m_sampleValid[m_sampleIndex % m_sampleCount] = true;
			m_sampleIndex++;
#endif
		}
	}

#ifdef SCHEDULER_TIMER_DEVICE
private:
	// One sample per watchdog period (500 ms) - 16 samples cover the last 8 s
	struct Sample {
		uint32_t cnt;
		int isr;
		int setHw;
		int freeze;
		uint32_t pending;
		uint32_t sr;
		uint32_t dier;
		uint32_t ccr1;
		uint32_t cr1;
	};
	static constexpr int m_sampleCount = 16;
	Sample m_samples[m_sampleCount];
	bool m_sampleValid[m_sampleCount];
	int m_sampleIndex = 0;
	bool m_errorReported = false;
#endif
};

RUSEFI_STACK_ROOT(MicrosecondTimerWatchdogController, PeriodicTask);

static MicrosecondTimerWatchdogController watchdogControllerInstance;

static scheduling_s watchDogBuddy;

static void watchDogBuddyCallback() {
	/**
	 * the purpose of this periodic activity is to make watchdogControllerInstance
	 * watchdog happy by ensuring that we have scheduler activity even in case of very broken configuration
	 * without any PWM or input pins
	 */
	engine->scheduler.schedule("watch", &watchDogBuddy, getTimeNowNt() + MS2NT(1000), action_s::make<watchDogBuddyCallback>());
}

static volatile bool testSchedulingHappened = false;
static Timer testScheduling;

static void timerValidationCallback() {
	testSchedulingHappened = true;
	efitimems_t actualTimeSinceSchedulingMs = 1e3 * testScheduling.getElapsedSeconds();

	if (absI(actualTimeSinceSchedulingMs - TEST_CALLBACK_DELAY_MS) > TEST_CALLBACK_DELAY_MS * TIMER_PRECISION_THRESHOLD) {
		firmwareError(ObdCode::CUSTOM_ERR_TIMER_TEST_CALLBACK_WRONG_TIME, "hwTimer broken precision: %ld ms", actualTimeSinceSchedulingMs);
	}
}

/**
 * This method would validate that hardware timer callbacks happen with some reasonable precision
 * helps to make sure our GPT hardware settings are somewhat right
 */
static void validateHardwareTimer() {
	if (hasFirmwareError()) {
		return;
	}
	testScheduling.reset();

	// to save RAM let's use 'watchDogBuddy' here once before we enable watchdog
	engine->scheduler.schedule(
			"hw-validate",
			&watchDogBuddy,
			getTimeNowNt() + MS2NT(TEST_CALLBACK_DELAY_MS),
			action_s::make<timerValidationCallback>());

	chThdSleepMilliseconds(TEST_CALLBACK_DELAY_MS + 2);
	if (!testSchedulingHappened) {
		firmwareError(ObdCode::CUSTOM_ERR_TIMER_TEST_CALLBACK_NOT_HAPPENED, "hwTimer not alive");
	}
}

void initMicrosecondTimer() {
	portInitMicrosecondTimer();

	hwStarted = true;

	lastSetTimerTimeNt = getTimeNowNt();

	validateHardwareTimer();

	watchDogBuddyCallback();
#if EFI_EMULATE_POSITION_SENSORS
	watchdogControllerInstance.start();
#endif /* EFI_EMULATE_POSITION_SENSORS */
}

#endif /* EFI_PROD_CODE */
