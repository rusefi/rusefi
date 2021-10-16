/**
 * @file	can_vss.h
 *
 * @date Apr 19, 2020
 * @author Alex Miculescu, (c) 2020
 */

#pragma once

float getVehicleCanSpeed(void);
void initCanVssSupport();
void setCanVss(int type);

#if EFI_CAN_SUPPORT
void processCanRxVss(const CANRxFrame& frame, efitick_t nowNt);
#endif
