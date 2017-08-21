/*
 * @file fl_protocol.cpp
 *
 * @date Mar 15, 2017
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#include "fl_protocol.h"

bool isStartOfFLProtocol(uint8_t firstByte) {
	return firstByte == START_TAG;
}

