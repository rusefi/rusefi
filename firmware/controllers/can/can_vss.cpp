/**
 * @file	can_vss.cpp
 *
 * This file handles incoming vss values from can.
 *
 * @date Apr 19, 2020
 * @author Alex Miculescu, (c) 2020
 */

#include "pch.h"

#if EFI_CAN_SUPPORT
#include "can.h"
#include "dynoview.h"
#include "stored_value_sensor.h"

static bool isInit = false;
static uint16_t filterCanID = 0;

expected<uint16_t> look_up_can_id(can_vss_nbc_e type) {
	switch (type) {
		case BMW_e46:
			return 0x01F0; /* BMW e46 ABS Message */
		case W202:
			return 0x0200; /* W202 C180 ABS signal */
		default:
			firmwareError(OBD_Vehicle_Speed_SensorB, "Wrong Can DBC selected: %d", type);
			return unexpected;
	}
}

/* Module specitifc processing functions */
/* source: http://z4evconversion.blogspot.com/2016/07/completely-forgot-but-it-does-live-on.html */
float processBMW_e46(const CANRxFrame& frame) {
	/* left front wheel speed is used here */
	uint16_t tmp = ((frame.data8[1] & 0x0f) << 8 );
	tmp |= frame.data8[0];

	return tmp / 16.0f;
}

float processW202(const CANRxFrame& frame) {
	uint16_t tmp = (frame.data8[2] << 8);
	tmp |= frame.data8[3];
	return tmp * 0.0625;
}

/* End of specific processing functions */

void canVssInfo(void) {
	efiPrintf("vss using can option selected %x", CONFIG(canVssNbcType));
	efiPrintf("vss filter for %x canID", filterCanID);
	efiPrintf("Vss module is %d", isInit);
	efiPrintf("CONFIG_enableCanVss is %d", CONFIG(enableCanVss));
}

expected<float> processCanRxVssImpl(const CANRxFrame& frame) {
	switch (CONFIG(canVssNbcType)){
		case BMW_e46:
			return processBMW_e46(frame);
		case W202:
			return processW202(frame);
		default:
			efiPrintf("vss unsupported can option selected %x", CONFIG(canVssNbcType) );
	}

	return unexpected;
}

static StoredValueSensor canSpeed(SensorType::VehicleSpeed, MS2NT(500));

void processCanRxVss(const CANRxFrame& frame, efitick_t nowNt) {
	if ((!CONFIG(enableCanVss)) || (!isInit)) {
		return;
	}

	//filter it we need to process the can message or not
	if (CAN_SID(frame) != filterCanID ) {
		return;
	}

	if (auto speed = processCanRxVssImpl(frame)) {
		canSpeed.setValidValue(speed.Value, nowNt);

#if EFI_DYNO_VIEW
		updateDynoViewCan();
#endif
	}
}

void initCanVssSupport() {
	addConsoleAction("canvssinfo", canVssInfo);

	if (CONFIG(enableCanVss)) {
		if (auto canId = look_up_can_id(CONFIG(canVssNbcType))) {
			filterCanID = canId.Value;
			canSpeed.Register();
			isInit = true;
		} else {
			isInit = false;
		}
	}
}

void setCanVss(int type) {
	engineConfiguration->canVssNbcType = (can_vss_nbc_e)type;
	canVssInfo();
}

#endif // EFI_CAN_SUPPORT