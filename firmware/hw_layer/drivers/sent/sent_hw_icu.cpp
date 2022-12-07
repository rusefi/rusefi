/*
 * sent_hw_icu.cpp
 *
 * SENT protocol glue to ChibiOS ICU driver
 *
 * @date Oct 01, 2022
 * @author Andrey Gusakov <dron0gus@gmail.com>, (c) 2022
 */

#if EFI_PROD_CODE

#include "ch.h"
#include "hal.h"

#include "sent.h"
#include "sent_hw_icu.h"

#include "io_pins.h"
#include "pin_repository.h"

/* This SENT HW driver is based on ChibiOS ICU driver */
#if (HAL_USE_ICU == TRUE)

/* TODO: get at runtime */
/* Max timer clock for most timers on STM32 is CPU clock / 2 */
#define SENT_TIMER_CLOCK_DIV	2
#define SENT_ICU_FREQ			(CORE_CLOCK / SENT_TIMER_CLOCK_DIV) // == CPU freq / 2

/* ICU callbacks */
static void icuperiodcb_in1(ICUDriver *icup)
{
	SENT_ISR_Handler(0, icuGetPeriodX(icup) * SENT_TIMER_CLOCK_DIV);
}

/* ICU configs */
static ICUConfig icucfg[SENT_INPUT_COUNT] =
{
	{
		.mode = ICU_INPUT_ACTIVE_LOW,
		.frequency = SENT_ICU_FREQ,
		.width_cb = NULL,
		.period_cb = icuperiodcb_in1,
		.overflow_cb = NULL,
		.channel = ICU_CHANNEL_1,	/* will be overwriten on startSent() */
		.dier = 0U,
		.arr = 0xFFFFFFFFU,
	}
};

void startSent()
{
	for (int i = 0; i < SENT_INPUT_COUNT; i++) {
		brain_input_pin_e sentPin = engineConfiguration->sentInputPins[i];

		if (!isBrainPinValid(sentPin)) {
			continue;
		}

		ICUConfig *cfg = &icucfg[i];
		ICUDriver *icu;
		iomode_t pinAF;
		uint32_t baseClock;

		if (getIcuParams(sentPin, &pinAF, &icu, &cfg->channel, &baseClock) != true) {
			/* this pin has no ICU functionality, of ICU driver is not enabled for TIM on this pin */
			/* throw error? */
			continue;
		}

		efiSetPadMode("SENT", sentPin, PAL_MODE_ALTERNATE(pinAF));

		icuStart(icu, cfg);
		icuStartCapture(icu);
		icuEnableNotifications(icu);
	}
}

void stopSent()
{
	for (int i = 0; i < SENT_INPUT_COUNT; i++) {
		brain_input_pin_e sentPin = activeConfiguration.sentInputPins[i];

		if (!isBrainPinValid(sentPin)) {
			continue;
		}

		ICUDriver *icu;

		if (getIcuParams(sentPin, NULL, &icu, NULL, NULL) != true) {
			/* this pin has no ICU functionality, of ICU driver is not enabled for TIM on this pin */
			/* throw error? */
			continue;
		}

		icuDisableNotifications(icu);
		icuStopCapture(icu);
		icuStop(icu);

		efiSetPadUnused(sentPin);
	}
}

#endif /* HAL_USE_ICU */

#endif /* EFI_PROD_CODE */
