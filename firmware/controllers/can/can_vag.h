#pragma once

#include "pch.h"
#include "can.h"

void canDashboardVAG(CanCycle cycle);

/**
 * B6
 * https://mdac.com.au/2021/04/11/dsg-control-with-rabbit-ecu/
 * https://github.com/RabbitECUProject/RabbitECUTeensyMCUXpresso/blob/main/source/Client/DIAG.c
 * https://github.com/commaai/opendbc/blob/master/vw_golf_mk4.dbc
 * https://rusefi.com/forum/download/file.php?id=9076
 */


// https://wiki.openstreetmap.org/wiki/VW-CAN
// 1056
#define CAN_VAG_CLT_V2   0x420 /* _10ms cycle */
// 976
#define CAN_VAG_IMMO     0x3D0 /* _10ms cycle */

// 0x280, 10ms period
#define CAN_VAG_Motor_1 640
// 0x284
#define CAN_VAG_Motor_Brake 644
// 0x288, 10ms period
#define CAN_VAG_Motor_2 648
// 0x362 see 4666@17
#define CAN_VAG_ACC1 866
// 0x380 10ms cycle
// see 4732@17
#define CAN_VAG_Motor_3 896
// 0x480
#define CAN_VAG_Motor_5 1152
// 0x488
#define CAN_VAG_Motor_6 1160
// 0x580
#define CAN_VAG_Motor_Flexia 1408
// 0x588
#define CAN_VAG_Motor_7 1416
