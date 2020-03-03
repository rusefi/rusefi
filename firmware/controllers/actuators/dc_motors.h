/**
 * @file dc_motors.h
 *
 * @date March 3, 2020
 * @author Matthew Kennedy (c) 2020
 */

#pragma once

#include <cstddef>
#include "global.h"

class DcMotor;
class Logger;

DcMotor* initDcMotor(size_t index DECLARE_ENGINE_PARAMETER_SUFFIX);

// Manual control of motors for use by console commands
void setDcMotorFrequency(size_t index, int hz);
void setDcMotorDuty(size_t index, float duty);

#if EFI_PROD_CODE
void showDcMotorInfo(Logging* logger);
#endif
