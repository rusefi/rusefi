/**
 * @file	can_tx.cpp
 *
 * CAN transmission handling.  This file handles the dispatch of various outgoing regularly scheduled CAN message types.
 *
 * @date Mar 19, 2020
 * @author Matthew Kennedy, (c) 2020
 */

#include "globalaccess.h"
#if EFI_CAN_SUPPORT

#include "engine.h"
#include "can.h"
#include "can_hw.h"
#include "can_dash.h"
#include "obd2.h"
#include "can_sensor.h"
#include "thread_priority.h"

EXTERN_ENGINE;

extern CanListener* canListeners_head;


CanWrite::CanWrite()
	: PeriodicController("CAN TX", PRIO_CAN_TX, 5)
{
}

void CanWrite::PeriodicTask(efitime_t nowNt) {
	UNUSED(nowNt);
	static uint16_t cycleCount = 0;
	uint16_t cycleMask;
	

	if (CONFIG(enableVerboseCanTx)) {
		uint16_t period = CONFIG(canSleepPeriodMs)/5;
		if ((cycleCount%period) == 0) {
			void sendCanVerbose();
			sendCanVerbose();
		}
	}

	CanListener* current = canListeners_head;

	while (current) {
		current = current->request();
	}

	//calculate cycle mask 
	cycleMask = CAM_5ms;
	cycleCount++;
	if ((cycleCount % 2) == 0) {
		cycleMask |= CAM_10ms;
	}
	if ((cycleCount % 4) == 0) {
		cycleMask |= CAM_20ms;
	}
	if ((cycleCount % 10) == 0) {
		cycleMask |= CAM_50ms;
	}
	if ((cycleCount % 20) == 0) {
		cycleMask |= CAM_100ms;
	}
	if ((cycleCount % 40) == 0) {
		cycleMask |= CAM_200ms;
	}
	if ((cycleCount % 50) == 0) {
		cycleMask |= CAM_250ms;
	}
	if ((cycleCount % 100) == 0) {
		cycleMask |= CAM_500ms;
	}
	if ((cycleCount % 200) == 0) {
		cycleMask |= CAM_1000ms;
		cycleCount = 0;
	}

	// Transmit dash data, if enabled
	switch (CONFIG(canNbcType)) {
	case CAN_BUS_NBC_BMW:
		canDashboardBMW(cycleMask);
		break;
	case CAN_BUS_NBC_FIAT:
		canDashboardFiat(cycleMask);
		break;
	case CAN_BUS_NBC_VAG:
		canDashboardVAG(cycleMask);
		break;
	case CAN_BUS_MAZDA_RX8:
		canMazdaRX8(cycleMask);
		break;
	case CAN_BUS_W202_C180:
		canDashboardW202(cycleMask);
		break;
	case CAN_BUS_BMW_E90:
		canDashboardBMWE90(cycleMask);
		break;
	case CAN_BUS_Haltech:
		canDashboardHaltech(cycleMask);
		break;
	case CAN_BUS_MQB:
		canDashboardVagMqb(cycleMask);
		break;
	default:
		break;
	}
}

#endif // EFI_CAN_SUPPORT
