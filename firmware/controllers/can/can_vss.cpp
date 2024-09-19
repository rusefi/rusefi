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
#include "can_rx.h"
#include "dynoview.h"
#include "stored_value_sensor.h"

static bool isInit = false;
static uint16_t filterVssCanID = 0;
static uint16_t filterSecondVssCanID = 0;
static uint16_t filterRpmCanID = 0;

static StoredValueSensor wheelSlipRatio(SensorType::WheelSlipRatio, MS2NT(1000));

static expected<uint16_t> look_up_rpm_can_id(can_vss_nbc_e type) {
	switch (type) {
		case HONDA_CIVIC9:
		  return 0x17C;
//		case HYUNDAI_PB:
		case NISSAN_350:
		  return 0x23D;
		case BMW_e46:
		case W202:
		case BMW_e90:
		case HYUNDAI_PB:
			firmwareError(ObdCode::OBD_Vehicle_Speed_SensorB, "RPM Can type not implemented yet: %d", type);
			return unexpected;
		default:
			firmwareError(ObdCode::OBD_Vehicle_Speed_SensorB, "Wrong RPM Can type: %d", type);
			return unexpected;
	}
}

static expected<uint16_t> look_up_vss_can_id(can_vss_nbc_e type) {
	switch (type) {
		case BMW_e46:
			return 0x01F0; /* BMW e46 ABS Message */
		case BMW_e90:
			return 0x1A0;	// BMW E90 ABS speed frame (not wheel speeds, vehicle speed)
		case NISSAN_350:
		  // decimal 640 0x280 has cluster speed but we use wheel speeds
		  return /*0x284*/644;
		case HYUNDAI_PB:
		  // decimal 1264 0x4F0 CLU1 has cluster speed but we use wheel speeds
		  return /*0x1F1*/497;
		case HONDA_CIVIC9:
		  return 0x309;
		case W202:
			return 0x0200; /* W202 C180 ABS signal */
		default:
			firmwareError(ObdCode::OBD_Vehicle_Speed_SensorB, "Wrong VSS Can type selected: %d", type);
			return unexpected;
	}
}

static uint16_t look_up_second_vss_can_id(can_vss_nbc_e type) {
	switch (type) {
		case NISSAN_350:
		  // decimal 640 0x280 has cluster speed but we use wheel speeds
		  return /*0x285*/645;
		default:
		  // it's OK not to require second packet
			return 0;
	}
}

/* Module specific processing functions */
/* source: http://z4evconversion.blogspot.com/2016/07/completely-forgot-but-it-does-live-on.html */
float processBMW_e46(const CANRxFrame& frame) {
	// average the rear wheels since those are the driven ones (more accurate gear detection!)
	uint16_t left =  getTwoBytesLsb(frame, 4);
	uint16_t right = getTwoBytesLsb(frame, 6);

	return (left + right) / (16 * 2);
}

// open question if that's front axle or rear axle?
static int nissanFrontAxle = 0;

float processNissan(const CANRxFrame& frame) {
	// todo: open question which one is left which one is right
	int left =  getTwoBytesMsb(frame, 0);
	int right = getTwoBytesMsb(frame, 2);

	nissanFrontAxle = left + right;

	return nissanFrontAxle / (100.0 * 2);
}

float processBMW_e90(const CANRxFrame& frame) {
	return 0.1f * getTwoBytesLsb(frame, 0);
}

float processW202(const CANRxFrame& frame) {
  // todo: reuse one of the getTwoBytes methods!
	uint16_t tmp = (frame.data8[2] << 8);
	tmp |= frame.data8[3];
	return tmp * 0.0625;
}

#define SLIP_RATIO(frontAxle, rearAxle) (((frontAxle) == 0 || (rearAxle) == 0) ? 1 : 1.0 * (frontAxle) / (rearAxle))

float processHyundai(const CANRxFrame& frame, efitick_t nowNt) {
  int frontL = getBitRangeLsb(frame.data8, 16, 12);
  int frontR = getBitRangeLsb(frame.data8, 28, 12);
  int rearL = getBitRangeLsb(frame.data8, 40, 12);
  int rearR = getBitRangeLsb(frame.data8, 52, 12);

  int frontAxle = (frontL + frontR);
  int rearAxle = (rearL + rearR);

  if (engineConfiguration->verboseCan) {
    efiPrintf("processHyundai: frontL %d rearL %d", frontL, rearL);
  }

  wheelSlipRatio.setValidValue(SLIP_RATIO(frontAxle, rearAxle), nowNt);

  return frontAxle / /*average*/2 / /* scale */8;
}

/* End of specific processing functions */

expected<float> processCanRxVssImpl(const CANRxFrame& frame, efitick_t nowNt) {
	switch (engineConfiguration->canVssNbcType) {
		case BMW_e46:
			return processBMW_e46(frame);
		case BMW_e90:
			return processBMW_e90(frame);
		case W202:
			return processW202(frame);
		case HYUNDAI_PB:
			return processHyundai(frame, nowNt);
		case NISSAN_350:
			return processNissan(frame);
		default:
			efiPrintf("vss unsupported can option selected %x", engineConfiguration->canVssNbcType );
	}

	return unexpected;
}

static StoredValueSensor canSpeed(SensorType::VehicleSpeed, MS2NT(500));

static void processNissanSecondVss(const CANRxFrame& frame, efitick_t nowNt) {
	// todo: open question which one is left which one is right
  int left =  getTwoBytesMsb(frame, 0);
	int right = getTwoBytesMsb(frame, 2);

	int rearAxle = left + right;

  if (engineConfiguration->verboseCan) {
	  efiPrintf("processNissan: front %d rear %d", nissanFrontAxle, rearAxle);
	}

	wheelSlipRatio.setValidValue(SLIP_RATIO(nissanFrontAxle, rearAxle), nowNt);
}

void processCanRxSecondVss(const CANRxFrame& frame, efitick_t nowNt) {
	switch (engineConfiguration->canVssNbcType) {
		case NISSAN_350:
		  processNissanSecondVss(frame, nowNt);
		  return;
		default:
		  criticalError("Unexpected processCanRxSecondVss %d", (int)engineConfiguration->canVssNbcType);
	}
}

void processCanRxVss(const CANRxFrame& frame, efitick_t nowNt) {
	if ((!engineConfiguration->enableCanVss) || (!isInit)) {
		return;
	}

	//filter it we need to process the can message or not
	if (CAN_SID(frame) == filterVssCanID) {
	  if (auto speed = processCanRxVssImpl(frame, nowNt)) {
		  canSpeed.setValidValue(speed.Value * engineConfiguration->canVssScaling, nowNt);

#if EFI_DYNO_VIEW
	  	updateDynoViewCan();
#endif
  	}
	}

	if (CAN_SID(frame) == filterSecondVssCanID) {
	  processCanRxSecondVss(frame, nowNt);
	}


	if (CAN_SID(frame) == filterRpmCanID) {
  }

}

void initCanVssSupport() {
	if (engineConfiguration->enableCanVss) {
		if (auto canId = look_up_vss_can_id(engineConfiguration->canVssNbcType)) {
			filterVssCanID = canId.Value;
			filterSecondVssCanID = look_up_second_vss_can_id(engineConfiguration->canVssNbcType);
			canSpeed.Register();
			wheelSlipRatio.Register();
			isInit = true;
		} else {
			isInit = false;
		}


		// todo: how do we handle 'isInit' for case with only RPM without VSS for instance?
		if (engineConfiguration->canInputBCM) {
			if (auto canId = look_up_rpm_can_id(engineConfiguration->canVssNbcType))
			filterRpmCanID = canId.Value;
		}

	}
}

void setCanVss(int type) {
	engineConfiguration->canVssNbcType = (can_vss_nbc_e)type;
}

#endif // EFI_CAN_SUPPORT
