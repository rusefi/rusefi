/**
 * @file	can_hw.h
 *
 * @date Dec 11, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef CAN_HW_H_
#define CAN_HW_H_

#include "tunerstudio_configuration.h"

// CAN Bus ID for broadcast
/**
 * e46 data is from http://forums.bimmerforums.com/forum/showthread.php?1887229
 *
 * Same for Mini Cooper? http://vehicle-reverse-engineering.wikia.com/wiki/MINI
 */
#define CAN_BMW_E46_SPEED 0x153
#define CAN_BMW_E46_RPM 0x316
#define CAN_BMW_E46_DME2 0x329
#define CAN_BMW_E46_CLUSTER_STATUS 0x613
#define CAN_BMW_E46_CLUSTER_STATUS_2 0x615
#define CAN_FIAT_MOTOR_INFO 0x561
#define CAN_MAZDA_RX_RPM_SPEED 0x201
#define CAN_MAZDA_RX_STEERING_WARNING 0x300
#define CAN_MAZDA_RX_STATUS_1 0x212
#define CAN_MAZDA_RX_STATUS_2 0x420
#define CAN_VAG_RPM 0x280
#define CAN_VAG_CLT 0x289

void initCan(void);
void stopCanPins(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void startCanPins(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void commonTxInit(int eid);
void sendCanMessage();
void setCanType(int type);
void setTxBit(int offset, int index);
void enableFrankensoCan(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void postCanState(TunerStudioOutputChannels *tsOutputChannels);

#endif /* CAN_HW_H_ */
