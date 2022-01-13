/**
 * @file dc_motors.h
 *
 * @date March 3, 2020
 * @author Matthew Kennedy (c) 2020
 */

#pragma once

#include <cstddef>

class DcMotor;

DcMotor* initDcMotor(const dc_io& io, size_t index, bool useTwoWires);
DcMotor* initDcMotor(brain_pin_e coil_p, brain_pin_e coil_m, size_t index);

// Manual control of motors for use by console commands
void setDcMotorFrequency(size_t index, int hz);
void setDcMotorDuty(size_t index, float duty);

void showDcMotorInfo(int i);

