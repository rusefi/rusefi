/*
 * can_bmw.h
 *
 *  Created on: Dec 19, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#pragma once

/**
 * e46 data is from http://forums.bimmerforums.com/forum/showthread.php?1887229
 *
 * Same for Mini Cooper? http://vehicle-reverse-engineering.wikia.com/wiki/MINI
 *
 * All the below packets are using 500kb/s
 *
 */
#define CAN_BMW_E46_SPEED             0x153
#define CAN_BMW_E46_RPM               0x316
#define CAN_BMW_E46_DME2              0x329
// unused #define CAN_BMW_E46_CLUSTER_STATUS    0x613
// unused #define CAN_BMW_E46_CLUSTER_STATUS_2  0x615

#define CAN_BMW_E90_TORQUE_BRAKE      0x0A8
// 10ms period
#define CAN_BMW_E90_TORQUE_2          0x0A9

// https://www.loopybunny.co.uk/CarPC/can/0AA.html
// 10ms period
#define CAN_BMW_E90_RPM_THROTTLE      0x0AA

// MECH
#define CAN_BMW_E90_TORQUE_DEMAND2    0x0B5
// 20ms period
#define CAN_BMW_E90_TORQUE_DEMAND     0x0B6
// MECH
#define CAN_BMW_E90_TRANSMISSION_DATA 0x0BA
// Requirement wheel torque 0x0BF

#define CAN_BMW_E90_STEERING_WHEEL    0x0C4
#define CAN_BMW_E90_WHEEL_SPEEDS      0x0CE
// Terminal Status, 100ms period
#define CAN_BMW_E90_IGNITION_KEY      0x130


#define CAN_BMW_E65_GEAR_SELECTOR     0x192
#define CAN_BMW_E90_BRAKING           0x19E
// MECH Getriebedaten 2
#define CAN_BMW_E90_GEARBOX_DATA_2    0x1A2

// Engine data
// https://github.com/HeinrichG-V12/E65_ReverseEngineering/blob/main/docs/0x1D0.md
#define CAN_BMW_E90_COOLANT           0x1D0
// MECH Anzeige Getriebedaten
#define CAN_BMW_E90_TRANSMISSION_DISP 0x1D2

// SEAT_BELT 2FA
// DOOR_STATUS 2FC

// MECH Status Gang
#define CAN_BMW_E90_GANG_STATUS       0x304

// EXTERNAL_TEMP_TIME 0x310

// Fahreranzeige Drehzahlbereich
#define CAN_BMW_E90_DASH_ON           0x332

// MILEAGE_RANGE 0x310
//#define CAN_BMW_E90_HAND_BRAKE        0x34F
// WHEEL_TOLERANCE 0x374
// MECH Getriebedaten 3 0x3B1

// https://www.loopybunny.co.uk/CarPC/can/3B4.html
// Powermanagement Batteriespannung
#define CAN_BMW_E90_VOLTAGE       0x3B4
