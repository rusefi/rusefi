/**
 * @file	microsecond_timer.c
 *
 * Here we have a 1MHz timer dedicated to event scheduling. We are using one of the 32-bit timers here,
 * so this timer can schedule events up to 4B/100M ~ 4000 seconds ~ 1 hour from current time.
 *
 * @date Apr 14, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"
#include "signal_executor.h"
#include "microsecond_timer.h"
#include "rfiutil.h"

// https://my.st.com/public/STe2ecommunities/mcu/Lists/cortex_mx_stm32/Flat.aspx?RootFolder=https%3a%2f%2fmy.st.com%2fpublic%2fSTe2ecommunities%2fmcu%2fLists%2fcortex_mx_stm32%2fInterrupt%20on%20CEN%20bit%20setting%20in%20TIM7&FolderCTID=0x01200200770978C69A1141439FE559EB459D7580009C4E14902C3CDE46A77F0FFD06506F5B&currentviews=474

#if EFI_PROD_CODE

#define GPTDEVICE GPTD5

static volatile int64_t lastSetTimerTime;
static int lastSetTimerValue;
static volatile bool_t isTimerPending = FALSE;

static volatile int timerCallbackCounter = 0;
static volatile int timerRestartCounter = 0;

schfunc_t globalTimerCallback;

/**
 * sets the alarm to the specified number of microseconds from now.
 * This function should be invoked under kernel lock which would disable interrupts.
 */
void setHardwareUsTimer(int32_t timeUs) {
	if (timeUs == 1)
		timeUs = 2; // for some reason '1' does not really work
	efiAssertVoid(timeUs > 0, "neg timeUs");
	efiAssertVoid(timeUs < 10 * US_PER_SECOND, "setHardwareUsTimer() too large");

	if (GPTDEVICE.state == GPT_ONESHOT)
		gptStopTimerI(&GPTDEVICE);
	gptStartOneShotI(&GPTDEVICE, timeUs);

	lastSetTimerTime = getTimeNowUs();
	lastSetTimerValue = timeUs;
	isTimerPending = TRUE;
	timerRestartCounter++;
}

static void callback(GPTDriver *gptp) {
	timerCallbackCounter++;
	if (globalTimerCallback == NULL) {
		firmwareError("NULL globalTimerCallback");
		return;
	}
	isTimerPending = FALSE;

//	// test code
//	setOutputPinValue(LED_CRANKING, timerCallbackCounter % 2);
//	int mod = timerCallbackCounter % 400;
//	chSysLockFromIsr()
//	;
//	setHardwareUsTimer(400 - mod);
//	chSysUnlockFromIsr()
//	;

	globalTimerCallback(NULL);
}

static THD_WORKING_AREA(mwThreadStack, UTILITY_THREAD_STACK_SIZE);

static const char * msg;

static char buff[32];

static msg_t mwThread(int param) {
	chRegSetThreadName("timer watchdog");

	while (TRUE) {
		chThdSleepMilliseconds(1000); // once a second is enough

		if (getTimeNowUs() >= lastSetTimerTime + 2 * US_PER_SECOND) {
			strcpy(buff, "no_event");
			itoa10(&buff[8], lastSetTimerValue);
			firmwareError(buff);
			return -1;
		}

		msg = isTimerPending ? "No_cb too long" : "Timer not awhile";
		// 2 seconds of inactivity would not look right
		efiAssert(getTimeNowUs() < lastSetTimerTime + 2 * US_PER_SECOND, msg, -1);
	}
#if defined __GNUC__
	return -1;
#endif        
}

//static const GPTConfig gpt5cfg;

static const GPTConfig gpt5cfg = { 1000000, /* 1 MHz timer clock.*/
callback, /* Timer callback.*/
0 };

void initMicrosecondTimer(void) {

	gptStart(&GPTDEVICE, &gpt5cfg);

	lastSetTimerTime = getTimeNowUs();
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
