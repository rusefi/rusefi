/*
 * sent_hw_icu.cpp
 *
 * SENT protocol glue to ChibiOS ICU driver
 *
 * @date Oct 01, 2022
 * @author Andrey Gusakov <dron0gus@gmail.com>, (c) 2022
 */

#if EFI_PROD_CODE

#include "pch.h"

#if EFI_SENT_SUPPORT

#include "sent.h"
#include "sent_hw_icu.h"

#include "io_pins.h"
#include "pin_repository.h"

/* This SENT HW driver is based on ChibiOS ICU driver */
#if (HAL_USE_ICU == TRUE)

/* TODO: do we care about scaling abstract timer ticks to some time base? */
/* TODO: get at runtime */
/* Max timer clock for most timers on STM32 is CPU clock / 2 */
#define SENT_TIMER_CLOCK_DIV	2
#define SENT_ICU_FREQ			(STM32_SYSCLK / SENT_TIMER_CLOCK_DIV) // == CPU freq / 2

static uint16_t lastPulse[SENT_INPUT_COUNT];
static bool overcapture[SENT_INPUT_COUNT];

static void icuperiodcb(ICUDriver *icup, size_t index)
{
	uint16_t clocks;
	uint8_t flags = 0;
	const ICUConfig *icucfg = icup->config;

	if ((icucfg->channel == ICU_CHANNEL_1) || (icucfg->channel == ICU_CHANNEL_2)) {
		/* channel 1 and channel 2 supports period measurements */
		clocks = icuGetPeriodX(icup);
	} else {
		/* this is freerunnig timer and we need to calculate period using just captured timer value and previous one */
		/* TODO: support 32 bit timers too? */
		uint16_t val = icuGetWidthX(icup);

		/* can overflow */
		clocks = val - lastPulse[index];

		lastPulse[index] = val;
	}

	if (overcapture[index]) {
		flags |= SENT_FLAG_HW_OVERFLOW;
		overcapture[index] = false;
	}

	SENT_ISR_Handler(index, clocks, flags);
}

static void icuovercapture(ICUDriver *icup, size_t index)
{
	UNUSED(icup);
	overcapture[index] = true;
}

/* ICU callbacks */
static void icuperiodcb_in1(ICUDriver *icup)
{
	icuperiodcb(icup, 0);
}

static void icuovercapture_in1(ICUDriver *icup)
{
	icuovercapture(icup, 0);
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
		.overcapture_cb = icuovercapture_in1,
	}
};

void startSent() {
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
			/* this pin has no ICU functionality or ICU driver is not enabled for TIM on this pin */
			criticalError("No ICU on selected SENT pin");
			continue;
		}

		efiSetPadMode("SENT", sentPin, PAL_MODE_ALTERNATE(pinAF));

		icuStart(icu, cfg);
		icuStartCapture(icu);
		icuEnableNotifications(icu);
	}
}

void stopSent() {
	for (int i = 0; i < SENT_INPUT_COUNT; i++) {
		brain_input_pin_e sentPin = activeConfiguration.sentInputPins[i];

		if (!isBrainPinValid(sentPin)) {
			continue;
		}

		ICUDriver *icu;

		if (getIcuParams(sentPin, NULL, &icu, NULL, NULL) != true) {
			/* this pin has no ICU functionality or ICU driver is not enabled for TIM on this pin */
			/* throw error? */
			continue;
		}

		icuDisableNotifications(icu);
		icuStopCapture(icu);
		icuStop(icu);

		efiSetPadUnused(sentPin);
	}
}

#endif /* EFI_SENT_SUPPORT */

#endif /* HAL_USE_ICU */

#endif /* EFI_PROD_CODE */
