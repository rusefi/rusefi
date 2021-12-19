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
 * for verbose use "set debug_mode 26" command in console
 *
 */
#define CAN_BMW_E46_SPEED             0x153
#define CAN_BMW_E46_RPM               0x316
#define CAN_BMW_E46_DME2              0x329
#define CAN_BMW_E46_CLUSTER_STATUS    0x613
#define CAN_BMW_E46_CLUSTER_STATUS_2  0x615

#define CAN_BMW_E90_TORQUE_BRAKE      0x0A8
// 10ms period
#define CAN_BMW_E90_TORQUE_2          0x0A9
// 10ms period
#define CAN_BMW_E90_RPM_THROTTLE      0x0AA
// 20ms period
#define CAN_BMW_E90_TORQUE_DEMAND     0x0B6

#define CAN_BMW_E90_STEERING_WHEEL    0x0C4
#define CAN_BMW_E90_WHEEL_SPEEDS      0x0CE
// Terminal Status, 100ms period
#define CAN_BMW_E90_IGNITION_KEY      0x130

#define CAN_BMW_E90_BRAKING           0x19E
// Engine data
#define CAN_BMW_E90_COOLANT           0x1D0

//#define CAN_BMW_E90_HAND_BRAKE        0x34F
