/**
 * @file	can_tx.cpp
 *
 * CAN transmission handling.  This file handles the dispatch of various outgoing regularly scheduled CAN message types.
 *
 * @date Mar 19, 2020
 * @author Matthew Kennedy, (c) 2020
 */

#include "pch.h"

#if EFI_CAN_SUPPORT
#include "can.h"
#include "can_hw.h"
#include "can_dash.h"
#include "obd2.h"
#include "can_sensor.h"
#include "rusefi_wideband.h"

extern CanListener* canListeners_head;


CanWrite::CanWrite()
	: PeriodicController("CAN TX", PRIO_CAN_TX, CAN_CYCLE_FREQ)
{
}

void CanWrite::PeriodicTask(efitime_t nowNt) {
	UNUSED(nowNt);
	static uint16_t cycleCount = 0;
	CanCycle cycle(cycleCount);

	//in case we have Verbose Can enabled, we should keep user configured period
	if (CONFIG(enableVerboseCanTx)) {
		uint16_t cycleCountsPeriodMs = cycleCount * CAN_CYCLE_PERIOD;
		if (0 != CONFIG(canSleepPeriodMs)) {
			if (cycleCountsPeriodMs % CONFIG(canSleepPeriodMs)) {
				void sendCanVerbose();
				sendCanVerbose();
			}
		}
	}

	CanListener* current = canListeners_head;

	while (current) {
		current = current->request();
	}

	if (cycle.isInterval(CI::_MAX_Cycle)) {
		//we now reset cycleCount since we reached max cycle count
		cycleCount = 0;
	}

	updateDash(cycle);

	if (CONFIG(enableAemXSeries) && cycle.isInterval(CI::_50ms)) {
		sendWidebandInfo();
	}

	cycleCount++;
}

CanInterval CanCycle::computeFlags(uint32_t cycleCount) {
	CanInterval cycleMask = CanInterval::_5ms;

	if ((cycleCount % 2) == 0) {
		cycleMask |= CI::_10ms;
	}

	if ((cycleCount % 4) == 0) {
		cycleMask |= CI::_20ms;
	}

	if ((cycleCount % 10) == 0) {
		cycleMask |= CI::_50ms;
	}

	if ((cycleCount % 20) == 0) {
		cycleMask |= CI::_100ms;
	}

	if ((cycleCount % 40) == 0) {
		cycleMask |= CI::_200ms;
	}

	if ((cycleCount % 50) == 0) {
		cycleMask |= CI::_250ms;
	}

	if ((cycleCount % 100) == 0) {
		cycleMask |= CI::_500ms;
	}

	if ((cycleCount % 200) == 0) {
		cycleMask |= CI::_1000ms;
	}

	return cycleMask;
}

#endif // EFI_CAN_SUPPORT
