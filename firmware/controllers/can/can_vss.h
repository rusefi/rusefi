/**
 * @file	can_vss.h
 *
 * @date Apr 19, 2020
 * @author Alex Miculescu, (c) 2020
 */

#pragma once

float getVehicleCanSpeed(void);
void processCanRxVss(const CANRxFrame& frame, efitick_t nowNt);
void initCanVssSupport(Logging *logger_ptr);
void setCanVss(int type);