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
	: PeriodicController("CAN TX", PRIO_CAN_TX, 50)
{
}

void CanWrite::PeriodicTask(efitime_t nowNt) {
	UNUSED(nowNt);

	if (CONFIG(enableVerboseCanTx)) {
		void sendCanVerbose();
		sendCanVerbose();
	}

	CanListener* current = canListeners_head;

	while (current) {
		current = current->request();
	}

	// Transmit dash data, if enabled
	switch (CONFIG(canNbcType)) {
	case CAN_BUS_NBC_BMW:
		canDashboardBMW();
		break;
	case CAN_BUS_NBC_FIAT:
		canDashboardFiat();
		break;
	case CAN_BUS_NBC_VAG:
		canDashboardVAG();
		break;
	case CAN_BUS_MAZDA_RX8:
		canMazdaRX8();
		break;
	case CAN_BUS_W202_C180:
		canDashboardW202();
		break;
	case CAN_BUS_BMW_E90:
		canDashboardBMWE90();
		break;
	case CAN_BUS_MQB:
		canDashboardVagMqb();
		break;
	default:
		break;
	}
}

#endif // EFI_CAN_SUPPORT
