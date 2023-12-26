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

static expected<uint16_t> look_up_can_id(can_vss_nbc_e type) {
	switch (type) {
		case BMW_e46:
			return 0x01F0; /* BMW e46 ABS Message */
		case BMW_e90:
			return 0x1A0;	// BMW E90 ABS speed frame (not wheel speeds, vehicle speed)
		case W202:
			return 0x0200; /* W202 C180 ABS signal */
		default:
			firmwareError(ObdCode::OBD_Vehicle_Speed_SensorB, "Wrong Can DBC selected: %d", type);
			return unexpected;
	}
}

static int getTwoBytesLsb(const CANRxFrame& frame, int index) {
	uint8_t low = frame.data8[index];
	uint8_t high = frame.data8[index + 1] & 0x0F;
	return low | (high << 8);
}

/* Module specific processing functions */
/* source: http://z4evconversion.blogspot.com/2016/07/completely-forgot-but-it-does-live-on.html */
float processBMW_e46(const CANRxFrame& frame) {
	// average the rear wheels since those are the driven ones (more accurate gear detection!)
	uint16_t left =  getTwoBytesLsb(frame, 4);
	uint16_t right = getTwoBytesLsb(frame, 6);

	return (left + right) / (16 * 2);
}

float processBMW_e90(const CANRxFrame& frame) {
	return 0.1f * getTwoBytesLsb(frame, 0);
}

float processW202(const CANRxFrame& frame) {
	uint16_t tmp = (frame.data8[2] << 8);
	tmp |= frame.data8[3];
	return tmp * 0.0625;
}

/* End of specific processing functions */

expected<float> processCanRxVssImpl(const CANRxFrame& frame) {
	switch (engineConfiguration->canVssNbcType){
		case BMW_e46:
			return processBMW_e46(frame);
		case BMW_e90:
			return processBMW_e90(frame);
		case W202:
			return processW202(frame);
		default:
			efiPrintf("vss unsupported can option selected %x", engineConfiguration->canVssNbcType );
	}

	return unexpected;
}

static StoredValueSensor canSpeed(SensorType::VehicleSpeed, MS2NT(500));

void processCanRxVss(const CANRxFrame& frame, efitick_t nowNt) {
	if ((!engineConfiguration->enableCanVss) || (!isInit)) {
		return;
	}

	//filter it we need to process the can message or not
	if (CAN_SID(frame) != filterCanID ) {
		return;
	}

	if (auto speed = processCanRxVssImpl(frame)) {
		canSpeed.setValidValue(speed.Value * engineConfiguration->canVssScaling, nowNt);

#if EFI_DYNO_VIEW
		updateDynoViewCan();
#endif
	}
}

void initCanVssSupport() {
	if (engineConfiguration->enableCanVss) {
		if (auto canId = look_up_can_id(engineConfiguration->canVssNbcType)) {
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
}

#endif // EFI_CAN_SUPPORT
