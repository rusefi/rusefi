/**
 * @file	can_tx.cpp
 *
 * CAN transmission handling.  This file handles the dispatch of various outgoing regularly scheduled CAN message types.
 *
 * @date Mar 19, 2020
 * @author Matthew Kennedy, (c) 2020
 */

#include "pch.h"

#if EFI_CAN_SUPPORT || EFI_UNIT_TEST
#include "can.h"
#include "can_dash.h"
#endif

#if EFI_CAN_SUPPORT
#include "can_hw.h"
#include "obd2.h"
#include "can_sensor.h"
#include "can_bench_test.h"
#include "rusefi_wideband.h"

extern CanListener* canListeners_head;


CanWrite::CanWrite()
	: PeriodicController("CAN TX", PRIO_CAN_TX, CAN_CYCLE_FREQ)
{
}

static CI roundTxPeriodToCycle(uint16_t period) {
	if (period < 10) return CI::_5ms;
	else if (period < 20) return CI::_10ms;
	else if (period < 50) return CI::_20ms;
	else if (period < 100) return CI::_50ms;
	else if (period < 200) return CI::_100ms;
	else if (period < 250) return CI::_200ms;
	else if (period < 500) return CI::_250ms;
	else if (period < 1000) return CI::_500ms;
	else return CI::_1000ms;
}

PUBLIC_API_WEAK bool boardEnableSendWidebandInfo() { return true; }

static uint16_t m_cycleCount = 0;

/* public API for custom boards */ void resetCanWriteCycle() {
  m_cycleCount = 0;
}

// this is invoked at CAN_CYCLE_FREQ frequency
void CanWrite::PeriodicTask(efitick_t) {
	ScopePerf pc(PE::CanThreadTx);
	CanCycle cycle(m_cycleCount);

	//in case we have Verbose Can enabled, we should keep user configured period
	if (engineConfiguration->enableVerboseCanTx) {
	  // slow down verbose CAN while in serial CAN
    int canSleepPeriodMs = (engine->pauseCANdueToSerial ? 5 : 1) * engineConfiguration->canSleepPeriodMs;

		auto roundedInterval = roundTxPeriodToCycle(canSleepPeriodMs);
		if (cycle.isInterval(roundedInterval)) {
				void sendCanVerbose();
				sendCanVerbose();
		}
	}

	CanListener* current = canListeners_head;

	while (current) {
		current = current->request();
	}

	if (cycle.isInterval(CI::_MAX_Cycle)) {
		//we now reset cycleCount since we reached max cycle count
		m_cycleCount = 0;
	}

	updateDash(cycle);

  if (engineConfiguration->enableExtendedCanBroadcast || isHwQcMode()) {
	  if (cycle.isInterval(CI::_100ms)) {
  		sendQcBenchEventCounters();
  		sendQcBenchRawAnalogValues();
#ifdef HW_HELLEN_8CHAN
  		sendQcBenchEventCounters(/*bus*/1);
  		sendQcBenchRawAnalogValues(/*bus*/1);
#endif
	  }

	  if (cycle.isInterval(CI::_250ms)) {
		  sendQcBenchBoardStatus();
#ifdef HW_HELLEN_8CHAN
		  sendQcBenchBoardStatus(/*bus*/1);
#endif
		  sendQcBenchButtonCounters();
		  sendQcBenchAuxDigitalCounters();
	  }
	}

	if (engineConfiguration->enableAemXSeries && cycle.isInterval(CI::_50ms) && boardEnableSendWidebandInfo()) {
		sendWidebandInfo();
	}

	m_cycleCount++;
}

#endif

#if EFI_CAN_SUPPORT || EFI_UNIT_TEST

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
