#pragma once

#include <cstddef>
#include "global.h"

class DcMotor;
class Logger;

DcMotor* initDcMotor(size_t index DECLARE_ENGINE_PARAMETER_SUFFIX);
void showDcMotorInfo(Logging* logger);
