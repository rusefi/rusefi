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
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "main.h"
#include "microsecond_timer.h"
#include "scheduler.h"
#include "rfiutil.h"

// https://my.st.com/public/STe2ecommunities/mcu/Lists/cortex_mx_stm32/Flat.aspx?RootFolder=https%3a%2f%2fmy.st.com%2fpublic%2fSTe2ecommunities%2fmcu%2fLists%2fcortex_mx_stm32%2fInterrupt%20on%20CEN%20bit%20setting%20in%20TIM7&FolderCTID=0x01200200770978C69A1141439FE559EB459D7580009C4E14902C3CDE46A77F0FFD06506F5B&currentviews=474

#if (EFI_PROD_CODE && HAL_USE_GPT) || defined(__DOXYGEN__)
#include "efilib2.h"

/**
 * Maximum duration of complete timer callback, all pending events together
 * See also 'maxEventCallbackDuration' for maximum duration of one event
 */
uint32_t maxPrecisionCallbackDuration = 0;

// must be one of 32 bit times
#define GPTDEVICE GPTD5

static volatile efitick_t lastSetTimerTimeNt;
static int lastSetTimerValue;
static volatile bool isTimerPending = FALSE;

static volatile int timerCallbackCounter = 0;
static volatile int timerRestartCounter = 0;

schfunc_t globalTimerCallback;

static THD_WORKING_AREA(mwThreadStack, UTILITY_THREAD_STACK_SIZE);

static const char * msg;

static char buff[32];

static int timerFreezeCounter = 0;

extern bool hasFirmwareErrorFlag;

/**
 * sets the alarm to the specified number of microseconds from now.
 * This function should be invoked under kernel lock which would disable interrupts.
 */
void setHardwareUsTimer(int32_t timeUs) {
	/**
	 * #259 BUG error: not positive timeUs
	 * Once in a while we night get an interrupt where we do not expect it
	 */
	if (timeUs <= 0) {
		timerFreezeCounter++;
		warning(CUSTOM_OBD_LOCAL_FREEZE, "local freeze cnt=%d", timerFreezeCounter);
	}
	if (timeUs < 2)
		timeUs = 2; // for some reason '1' does not really work
	efiAssertVoid(timeUs > 0, "not positive timeUs");
	if (timeUs >= 10 * US_PER_SECOND) {
		firmwareError(CUSTOM_ERR_TIMER_OVERFLOW, "setHardwareUsTimer() too long: %d", timeUs);
		return;
	}

	if (GPTDEVICE.state == GPT_ONESHOT)
		gptStopTimerI(&GPTDEVICE);
	efiAssertVoid(GPTDEVICE.state == GPT_READY, "hw timer");
	if (hasFirmwareError())
		return;
	gptStartOneShotI(&GPTDEVICE, timeUs);

	lastSetTimerTimeNt = getTimeNowNt();
	lastSetTimerValue = timeUs;
	isTimerPending = TRUE;
	timerRestartCounter++;
}

static void callback(GPTDriver *gptp) {
	(void)gptp;
	timerCallbackCounter++;
	if (globalTimerCallback == NULL) {
		firmwareError(CUSTOM_ERR_NULL_TIMER_CALLBACK, "NULL globalTimerCallback");
		return;
	}
	isTimerPending = false;

//	// test code
//	setOutputPinValue(LED_CRANKING, timerCallbackCounter % 2);
//	int mod = timerCallbackCounter % 400;
//	chSysLockFromISR()
//	;
//	setHardwareUsTimer(400 - mod);
//	chSysUnlockFromISR()
//	;

	uint32_t before = GET_TIMESTAMP();
	globalTimerCallback(NULL);
	uint32_t precisionCallbackDuration = GET_TIMESTAMP() - before;
	if (precisionCallbackDuration > maxPrecisionCallbackDuration) {
		maxPrecisionCallbackDuration = precisionCallbackDuration;
	}
}

static void usTimerWatchDog(void) {
	if (getTimeNowNt() >= lastSetTimerTimeNt + 2 * CORE_CLOCK) {
		strcpy(buff, "no_event");
		itoa10(&buff[8], lastSetTimerValue);
		firmwareError(CUSTOM_ERR_SCHEDULING_ERROR, buff);
		return;
	}

	msg = isTimerPending ? "No_cb too long" : "Timer not awhile";
	// 2 seconds of inactivity would not look right
	efiAssertVoid(getTimeNowNt() < lastSetTimerTimeNt + 2 * CORE_CLOCK, msg);

}

static msg_t mwThread(int param) {
	(void)param;
	chRegSetThreadName("timer watchdog");

	while (true) {
		chThdSleepMilliseconds(1000); // once a second is enough
		usTimerWatchDog();
	}
#if defined __GNUC__
	return -1;
#endif        
}

//static const GPTConfig gpt5cfg;

static const GPTConfig gpt5cfg = { 1000000, /* 1 MHz timer clock.*/
callback, /* Timer callback.*/
0, 0 };

void initMicrosecondTimer(void) {

	gptStart(&GPTDEVICE, &gpt5cfg);

	lastSetTimerTimeNt = getTimeNowNt();
#if EFI_EMULATE_POSITION_SENSORS
	chThdCreateStatic(mwThreadStack, sizeof(mwThreadStack), NORMALPRIO, (tfunc_t) mwThread, NULL);
#endif /* EFI_ENGINE_EMULATOR */

//	// test code
//	chSysLock()
//	;
//	setHardwareUsTimer(300);
//	chSysUnlock()
//	;
}

#endif /* EFI_PROD_CODE */
