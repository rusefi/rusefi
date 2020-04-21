/**
 * @file	can_vss.h
 *
 * @date Apr 19, 2020
 * @author Alex Miculescu, (c) 2020
 */

#pragma once
#include "can.h"

float getVehicleCanSpeed(void);
void processCanRxVss(const CANRxFrame& frame, efitick_t nowNt);
void initCanVssSupport(Logging *l);
void setCanVss(int type);