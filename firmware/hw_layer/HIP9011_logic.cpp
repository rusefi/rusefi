/*
 * @file HIP9011_logic.cpp
 *
 *  Created on: Jan 3, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#include "HIP9011_logic.h"

HIP9011::HIP9011() {
	needToInit = true;
	state = NOT_READY;
}

