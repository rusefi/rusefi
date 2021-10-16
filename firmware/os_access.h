/*
 * @file os_access.h
 *
 * OS access is not part of global.h in order to help separate synchronous code from asynchronous
 *
 *  Created on: Jul 3, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include <ch.h>
#include <hal.h>
#include "chprintf.h"

#ifdef __cplusplus
#include "io_pins.h"
// ChibiOS c++ wrappers
#include "ch.hpp"
#endif /* __cplusplus */

#define HAS_OS_ACCESS
