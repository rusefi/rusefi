// file can_dash_haltech.cpp
//
// Haltech CAN bus operates at 1 MBit/s and uses 11-bit IDs
// https://support.haltech.com/portal/en/kb/articles/haltech-can-ecu-broadcast-protocol
// https://support.haltech.com/portal/en/kb/articles/haltech-can-protocol-specification

#include "pch.h"

#if EFI_CAN_SUPPORT || EFI_UNIT_TEST
#include "can.h"
#include "can_msg_tx.h"
#include "fuel_math.h"

void canDashboardHaltech(CanCycle cycle) {
  criticalAssertVoid(engineConfiguration->canBaudRate == B1MBPS, "Haltech requires 1Mbps baud rate");

	if (cycle.isInterval(CI::_20ms)) {
		/* 0x360 - 50Hz rate */
		{
			CanTxMessage msg(CanCategory::NBC, 0x360, 8);
			msg.setShortValueMsb(Sensor::getOrZero(SensorType::Rpm), 0);
			msg.setShortValueMsb(Sensor::getOrZero(SensorType::Map) * 10, 2);
			/* TPS  y = x/10 */
			msg.setShortValueMsb(Sensor::getOrZero(SensorType::Tps1) * 10, 4);
			/* Coolant pressure */
			msg[6] = 0;
			msg[7] = 0;
		}

		/* 0x361 - 50Hz rate */
		{
			CanTxMessage msg(CanCategory::NBC, 0x361, 8);
			/* Fuel pressure */
			msg.setShortValueMsb((Sensor::getOrZero(SensorType::FuelPressureLow) + STD_ATMOSPHERE) * 10, 0);
			/* Oil pressure */
			msg.setShortValueMsb((Sensor::getOrZero(SensorType::OilPressure) + STD_ATMOSPHERE) * 10, 2);
			/* Engine Demand */
			msg.setShortValueMsb(Sensor::getOrZero(SensorType::Map), 4);
			/* Wastegate Pressure */
			msg[6] = 0;
			msg[7] = 0;
		}

#if EFI_ENGINE_CONTROL
		/* 0x362 - 50Hz rate */
		{
			CanTxMessage msg(CanCategory::NBC, 0x362, 6);
			/* Injection Stage 1 Duty Cycle - y = x/10 */
			uint16_t rpm = Sensor::getOrZero(SensorType::Rpm);
			msg.setShortValueMsb(getInjectorDutyCycle(rpm) * 10, 0);
			/* Injection Stage 2 Duty Cycle */
			msg[2] = 0x00;
			msg[3] = 0x00;
			/* Ignition Angle (Leading) - y = x/10 */
			float timing = engine->engineState.timingAdvance[0];
			int16_t ignAngle = ((timing > 360 ? timing - 720 : timing) * 10);
			msg.setShortValueMsb(ignAngle, 4);
		}
#endif // EFI_ENGINE_CONTROL

		/* todo: 0x3E5 = 50Hz rate */
		{
			CanTxMessage msg(CanCategory::NBC, 0x3E5, 8);
			msg[0] = 0x00;				//Ignition Switch
			msg[1] = 0x00;				//Turbo Timer - Time Remaining
			msg[2] = 0x00;				//Turbo Timer - Engine Time Remaining
			msg[3] = 0x00;
			msg[4] = 0x00;				//4-5 Steering Wheel Angle
			msg[5] = 0x00;
			msg[6] = 0x00;				//6-7 Driveshaft RPM
			msg[7] = 0x00;
		}

		/* todo: 0x3EA = 50Hz rate */
		{
			CanTxMessage msg(CanCategory::NBC, 0x3EA, 8);
			msg[0] = 0x00;				//0-1 Gearbox Line Pressure
			msg[1] = 0x00;
			msg[2] = 0x00;				//2-3 Injection Stage 3 Duty Cycle
			msg[3] = 0x00;
			msg[4] = 0x00;				//4-5 Injection Stage 4 Duty Cycle
			msg[5] = 0x00;
			msg[6] = 0x00;				//Crank Case Pressure
			msg[7] = 0x00;
		}

		/* todo: 0x3EB = 50Hz rate */
		{
			CanTxMessage msg(CanCategory::NBC, 0x3EB, 8);
			msg[0] = 0x00;				//0-3 Race Timer
			msg[1] = 0x00;
			msg[2] = 0x00;
			msg[3] = 0x00;
			msg[4] = 0x00;				//4-5 Ignition Angle Bank 1
			msg[5] = 0x00;
			msg[6] = 0x00;				//6-7 Ignition Angle Bank 2
			msg[7] = 0x00;
		}

		/* todo: 0x3EC = 50Hz rate */
		{
			CanTxMessage msg(CanCategory::NBC, 0x3EC, 8);
			msg[0] = 0x00;				//0-1 Torque Management Driveshaft RPM Target
			msg[1] = 0x00;
			msg[2] = 0x00;				//2-3 Torque Management Driveshaft RPM Target Error
			msg[3] = 0x00;
			msg[4] = 0x00;				//4-5 Torque Management Driveshaft RPM Target Error Ignition Correction
			msg[5] = 0x00;
			msg[6] = 0x00;				//6-7 Torque Management Driveshaft RPM Timed Ignition Correction
			msg[7] = 0x00;
		}

		/* todo: 0x3ED = 50Hz rate */
		{
			CanTxMessage msg(CanCategory::NBC, 0x3ED, 2);
			msg[0] = 0x00;				//0-1 Torque Management Combined Ignition Correction
			msg[1] = 0x00;
		}

		/* todo: 0x471 = 50Hz rate */
		{
			CanTxMessage msg(CanCategory::NBC, 0x471, 6);
			msg[0] = 0x00;				//0-1 Injector Pressure Differential
			msg[1] = 0x00;
			msg.setShortValueMsb(Sensor::getOrZero(SensorType::AcceleratorPedal) * 10, 2);
			msg[4] = 0x00;				//4-5 Exhaust Manifold Pressure
			msg[5] = 0x00;
		}
	}

	if (cycle.isInterval(CI::_50ms)) {

		/* 0x363 - 20Hz rate */
		{
			CanTxMessage msg(CanCategory::NBC, 0x363, 4);
			/* Wheel Slip */
			msg[0] = 0x00;
			msg[1] = 0x00;
			/* Wheel Diff */
			msg[2] = 0x00;
			msg[3] = 0x00 ;
		}

		/* 0x368 - 20Hz rate */
		{
			CanTxMessage msg(CanCategory::NBC, 0x368, 8);
			/* Wideband Sensor 1 */
			msg.setShortValueMsb(Sensor::getOrZero(SensorType::Lambda1) * 1000, 0);
			/* Wideband Sensor 2 */
			msg.setShortValueMsb(Sensor::getOrZero(SensorType::Lambda2) * 1000, 2);
			/* Wideband Sensor 3 */
			msg[4] = 0x00;
			msg[5] = 0x00;
			/* Wideband Sensor 4 */
			msg[6] = 0x00;
			msg[7] = 0x00;
		}

#if EFI_SHAFT_POSITION_INPUT
		/* 0x369 - 20Hz rate */
		{
			CanTxMessage msg(CanCategory::NBC, 0x369, 8);
			/* Trigger System Error Count */
			msg.setShortValueMsb(engine->triggerCentral.triggerState.totalTriggerErrorCounter, 0);
			/* Trigger Counter ?? */
			msg.setShortValueMsb(engine->triggerCentral.getHwEventCounter((int)SHAFT_PRIMARY_FALLING), 2);
			/* unused */
			msg[4] = 0x00;
			msg[5] = 0x00;
			/* Trigger Sync Level ?? */
			msg[6] = 0x00;
			msg[7] = 0x00;
		}
#endif // EFI_SHAFT_POSITION_INPUT

		/* 0x36A - 20Hz rate */
		/* todo: one day we should split this */
		{
			CanTxMessage msg(CanCategory::NBC, 0x36A, 4);
			/* Knock Level 1 */
			int knock100 = engine->module<KnockController>()->m_knockLevel * 100;
			msg.setShortValueMsb(knock100, 0);
			/* Knock Level 2 */
			msg.setShortValueMsb(knock100, 2);
		}

		/* 0x36B - 20Hz rate */
		{
			CanTxMessage msg(CanCategory::NBC, 0x36B, 8);
			/* Break Pressure */
			msg[0] = 0x00;
			msg[1] = 0x00;
			/* NOS pressure Sensor 1 */
			msg[2] = 0x00;
			msg[3] = 0x00;
			/* Turbo Speed Sensor 1 */
			msg[4] = 0x00;
			msg[5] = 0x00;
			/* Lateral G */
			msg[6] = 0x00;
			msg[7] = 0x00;
		}

		/* 0x36C = 20Hz rate */
		{
			CanTxMessage msg(CanCategory::NBC, 0x36C, 8);
			/* Wheel Speed Front Left */
			auto vehicleSpeed10 = Sensor::getOrZero(SensorType::VehicleSpeed) * 10;
			msg.setShortValueMsb(vehicleSpeed10, 0);
			/* Wheel Speed Front Right */
			msg.setShortValueMsb(vehicleSpeed10, 2);
			/* Wheel Speed Read Left */
			msg.setShortValueMsb(vehicleSpeed10, 4);
			/* Wheel Speed Read Right */
			msg.setShortValueMsb(vehicleSpeed10, 6);
		}

		/* 0x36D = 20Hz rate */
		{
			CanTxMessage msg(CanCategory::NBC, 0x36D, 8);
			/* Unused */
			msg[0] = 0x00;
			msg[1] = 0x00;
			msg[2] = 0x00;
			msg[3] = 0x00;
			/* Exhaust Cam Angle 1 */
			msg[4] = 0x00;
			msg[5] = 0x00;
			/* Exhaust Cam Angle 2 */
			msg[6] = 0x00;
			msg[7] = 0x00;
		}

		/* 0x36E = 20Hz rate */
		{
			CanTxMessage msg(CanCategory::NBC, 0x36E, 8);
			/* Engine Limiting Active 0 = off/1=on*/
			msg[0] = 0x00;
			msg[1] = 0x00;
			/* Launch Control Ignition Retard */
			msg[2] = 0x00;
			msg[3] = 0x00;
			/* Launch Control Fuel Enrich */
			msg[4] = 0x00;
			msg[5] = 0x00;
			/* Longitudinal G */
			msg[6] = 0x00;
			msg[7] = 0x00;
		}

		/* 0x36F = 20Hz rate */
		{
			CanTxMessage msg(CanCategory::NBC, 0x36F, 4);
			/* Generic Output 1 Duty Cycle */
			msg[0] = 0x00;
			msg[1] = 0x00;
			/* Boost Control Output */
			msg[2] = 0x00;
			msg[3] = 0x00;
		}

		/* 0x370 = 20Hz rate */
		{
			CanTxMessage msg(CanCategory::NBC, 0x370, 8);
			/* Vehicle Speed */
			auto vehicleSpeed10 = Sensor::getOrZero(SensorType::VehicleSpeed) * 10;
			msg.setShortValueMsb(vehicleSpeed10, 0);
			/* unused */
			msg[2] = 0x00;
			msg[3] = 0x00;
			/* Intake Cam Angle 1 */
			msg[4] = 0x00;
			msg[5] = 0x00;
			/* Intake Cam Angle 2 */
			msg[6] = 0x00;
			msg[7] = 0x00;
		}

		/* todo: 0x3E6 = 20Hz rate */
		{
			CanTxMessage msg(CanCategory::NBC, 0x3E6, 8);
			msg[0] = 0x00;				//0-1 NOS Pressure Sensor 2
			msg[1] = 0x00;
			msg[2] = 0x00;				//2-3 NOS Pressure Sensor 3
			msg[3] = 0x00;
			msg[4] = 0x00;				//4-5 NOS Pressure Sensor 4
			msg[5] = 0x00;
			msg[6] = 0x00;				//6-7 Turbo Speed Sensor 2
			msg[7] = 0x00;
		}

		/* todo: 0x3E7 = 20Hz rate */
		{
			CanTxMessage msg(CanCategory::NBC, 0x3E7, 8);
			msg[0] = 0x00;				//0-1 Generic Sensor 1
			msg[1] = 0x00;
			msg[2] = 0x00;				//2-3 Generic Sensor 2
			msg[3] = 0x00;
			msg[4] = 0x00;				//4-5 Generic Sensor 3
			msg[5] = 0x00;
			msg[6] = 0x00;				//6-7 Generic Sensor 4
			msg[7] = 0x00;
		}

		/* todo: 0x3E8 = 20Hz rate */
		{
			CanTxMessage msg(CanCategory::NBC, 0x3E8, 8);
			msg[0] = 0x00;				//0-1 Generic Sensor 5
			msg[1] = 0x00;
			msg[2] = 0x00;				//2-3 Generic Sensor 6
			msg[3] = 0x00;
			msg[4] = 0x00;				//4-5 Generic Sensor 7
			msg[5] = 0x00;
			msg[6] = 0x00;				//6-7 Generic Sensor 8
			msg[7] = 0x00;
		}

		/* todo: 0x3E9 = 20Hz rate */
		{
			CanTxMessage msg(CanCategory::NBC, 0x3E9, 8);
			msg[0] = 0x00;				//0-1 Generic Sensor 9
			msg[1] = 0x00;
			msg[2] = 0x00;				//2-3 Generic Sensor 10
			msg[3] = 0x00;
			msg[4] = 0x00;				//4-5 Target Lambda
			msg[5] = 0x00;
			msg[6] = 0x00;				//Nitous Stage Output
			msg[7] = 0x00;				//Torque Management Knob
		}

		/* todo: 0x3EE = 20Hz rate */
		{
			CanTxMessage msg(CanCategory::NBC, 0x3EE, 8);
			msg[0] = 0x00;				//0-1 Wideband Sensor 5
			msg[1] = 0x00;
			msg[2] = 0x00;				//2-3 Wideband Sensor 6
			msg[3] = 0x00;
			msg[4] = 0x00;				//4-5 Wideband Sensor 7
			msg[5] = 0x00;
			msg[6] = 0x00;				//6-7 Wideband Sensor 8
			msg[7] = 0x00;
		}

		/* todo: 0x3EF = 20Hz rate */
		{
			CanTxMessage msg(CanCategory::NBC, 0x3EF, 8);
			msg[0] = 0x00;				//0-1 Wideband Sensor 9
			msg[1] = 0x00;
			msg[2] = 0x00;				//2-3 Wideband Sensor 10
			msg[3] = 0x00;
			msg[4] = 0x00;				//4-5 Wideband Sensor 11
			msg[5] = 0x00;
			msg[6] = 0x00;				//6-7 Wideband Sensor 12
			msg[7] = 0x00;
		}

		/* todo: 0x470 = 20Hz rate */
		{
			CanTxMessage msg(CanCategory::NBC, 0x470, 8);
			msg[0] = 0x00;				//0-1 Wideband Overall
			msg[1] = 0x00;
			msg[2] = 0x00;				//2-3 Wideband Bank 1
			msg[3] = 0x00;
			msg[4] = 0x00;				//4-5 Wideband Bank 2
			msg[5] = 0x00;
			// todo: open question what are Haltech Special Values for gear encoding
			msg[6] = 0x00;				//Gear Selector Position
			msg[7] = Sensor::getOrZero(SensorType::DetectedGear);
		}

		/* todo: 0x472 = 20Hz rate */
		{
			CanTxMessage msg(CanCategory::NBC, 0x472, 8);
			msg[0] = 0x00;				//0-1 Cruise Control Target Speed
			msg[1] = 0x00;
			msg[2] = 0x00;				//2-3 Cruise Control Last Target Speed
			msg[3] = 0x00;
			msg[4] = 0x00;				//4-5 Cruise Control Speed Error
			msg[5] = 0x00;
			msg[6] = 0x00;				//Cruise Control Controller state (enum)
			msg[7] = 0x00;				//Cruise Control Input State (bit-field)
		}
	}

	if (cycle.isInterval(CI::_100ms)) {

		/* 0x371 = 10Hz rate */
		{
			CanTxMessage msg(CanCategory::NBC, 0x371, 4);
			/* Fuel Flow */
			msg[0] = 0x00;
			msg[1] = 0x00;
			/* Fuel Flow Return */
			msg[2] = 0x00;
			msg[3] = 0x00;
		}

		/* 0x372 = 10Hz rate */
		{
			CanTxMessage msg(CanCategory::NBC, 0x372, 8);
			/* Battery Voltage */
			msg.setShortValueMsb(Sensor::getOrZero(SensorType::BatteryVoltage) * 10, 0);
			/* unused */
			msg[2] = 0x00;
			msg[3] = 0x00;
			/* Target Boost Level todo */
			msg[4] = 0x00;
			msg[5] = 0x00;
			/* Barometric pressure */
			msg.setShortValueMsb(Sensor::getOrZero(SensorType::BarometricPressure) * 10, 6);
		}

		/* 0x373 = 10Hz rate */
		{
			CanTxMessage msg(CanCategory::NBC, 0x373, 8);
			/* EGT1 */
			msg.setShortValueMsb(Sensor::getOrZero(SensorType::EGT1) * 10, 0);
			/* EGT2 */
			msg.setShortValueMsb(Sensor::getOrZero(SensorType::EGT2) * 10, 2);
			/* EGT3 */
			msg.setShortValueMsb(Sensor::getOrZero(SensorType::EGT3) * 10, 4);
			/* EGT4 */
			msg.setShortValueMsb(Sensor::getOrZero(SensorType::EGT4) * 10, 6);
		}

		/* 0x374 = 10Hz rate */
		{
			CanTxMessage msg(CanCategory::NBC, 0x374, 8);
			/* EGT5 */
			msg[0] = 0x00;
			msg[1] = 0x00;
			/* EGT6 */
			msg[2] = 0x00;
			msg[3] = 0x00;
			/* EGT7 */
			msg[4] = 0x00;
			msg[5] = 0x00;
			/* EGT8 */
			msg[6] = 0x00;
			msg[7] = 0x00;
		}

		/* 0x375 = 10Hz rate */
		{
			CanTxMessage msg(CanCategory::NBC, 0x375, 8);
			/* EGT9 */
			msg[0] = 0x00;
			msg[1] = 0x00;
			/* EGT10 */
			msg[2] = 0x00;
			msg[3] = 0x00;
			/* EGT11 */
			msg[4] = 0x00;
			msg[5] = 0x00;
			/* EGT12 */
			msg[6] = 0x00;
			msg[7] = 0x00;
		}

		/* 0x376 = 10Hz rate */
		{
			CanTxMessage msg(CanCategory::NBC, 0x376, 8);
			/* Ambient Air Temperature */
			msg[0] = 0x00;
			msg[1] = 0x00;
			/* Relative Humidity */
			msg[2] = 0x00;
			msg[3] = 0x00;
			/* Specific Humidity */
			msg[4] = 0x00;
			msg[5] = 0x00;
			/* Absolute Humidity */
			msg[6] = 0x00;
			msg[7] = 0x00;
		}
	}

	if (cycle.isInterval(CI::_200ms)) {
		/* 0x3E0 = 5Hz rate */
		{
			CanTxMessage msg(CanCategory::NBC, 0x3E0, 8);
			/* Coolant temperature in K y = x/10 */
			msg.setShortValueMsb((Sensor::getOrZero(SensorType::Clt) + C_K_OFFSET) * 10, 0);
			/* Air Temperature */
			msg.setShortValueMsb((Sensor::getOrZero(SensorType::Iat) + C_K_OFFSET) * 10, 2);
			/* Fuel Temperature */
			msg.setShortValueMsb((Sensor::getOrZero(SensorType::FuelTemperature) + C_K_OFFSET) * 10, 4);
			/* Oil Temperature */
			msg[6] = 0x00;
			msg[7] = 0x00;
		}

		/* 0x3E1 = 5Hz rate */
		{
			CanTxMessage msg(CanCategory::NBC, 0x3E1, 6);
			/* Gearbox Oil Temperature */
			msg[0] = 0x00;
			msg[1] = 0x00;
			/* Diff oil Temperature */
			msg[2] = 0x00;
			msg[3] = 0x00;
			/* Fuel Composition */
			msg.setShortValueMsb(Sensor::getOrZero(SensorType::FuelEthanolPercent) * 10, 4);
		}

		/* 0x3E2 = 5Hz rate */
		{
			CanTxMessage msg(CanCategory::NBC, 0x3E2, 2);
			/* Fuel Level in Liters */
			msg.setShortValueMsb(Sensor::getOrZero(SensorType::FuelLevel) * 10, 0);
		}

		/* 0x3E3 = 5Hz rate */
		{
			CanTxMessage msg(CanCategory::NBC, 0x3E3, 8);
			/* Fuel Trim Short Term Bank 1*/
			msg[0] = 0x00;
			msg[1] = 0x00;
			/* Fuel Trim Short Term Bank 2*/
			msg[2] = 0x00;
			msg[3] = 0x00;
			/* Fuel Trim Long Term Bank 1*/
			msg[4] = 0x00;
			msg[5] = 0x00;
			/* Fuel Trim Long Term Bank 2*/
			msg[6] = 0x00;
			msg[7] = 0x00;
		}

		/* todo: 0x3E4 = 5Hz rate */
		{
    			CanTxMessage msg(CanCategory::NBC, 0x3E4, 8);
    			msg[0] = 0x00; //unused
    			if (engine->engineState.brakePedalState) {
    				msg.setBit(1, 2); // Brake active
    			}
    			if (engine->engineState.clutchDownState) {
    				msg.setBit(1, 1); // Clutch active
    			}
#if EFI_LAUNCH_CONTROL
    			if (engine->launchController.isLaunchCondition) {
    				msg.setBit(2, 7); // Launch active
    			}
    			if (engine->launchController.isSwitchActivated) {
    				msg.setBit(2, 6); // Launch Switch active
    			}
#endif
    			if (engine->module<AcController>()->acButtonState){
    				msg.setBit(3, 5); // AC Request
    			}
    			if (engine->module<AcController>()->m_acEnabled){
    				msg.setBit(3, 4); // AC Output
    			}
    			if (enginePins.fanRelay2.getLogicValue()) {
    				msg.setBit(3, 1); // Fan2 active
    			}
    			if (enginePins.fanRelay.getLogicValue()) {
    				msg.setBit(3, 0); // Fan active
    			}
    			/* Switch status */
    			msg[4] = 0x00;
    			msg[5] = 0x00;
    			msg[6] = 0x00;
    			if ((Sensor::getOrZero(SensorType::Rpm)>0) && (Sensor::get(SensorType::BatteryVoltage).value_or(VBAT_FALLBACK_VALUE)<13)) {
    				msg.setBit(7, 6); // battery light
    			}
    }

	}
}

#endif
