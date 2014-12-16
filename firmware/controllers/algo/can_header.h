/**
 * @file	can_header.h
 *
 * @date Dec 21, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef CAN_HEADER_H_
#define CAN_HEADER_H_

#include "rusefi_enums.h"

/**
 * Net Body Computer types
 */
typedef enum {
	CAN_BUS_NBC_BMW = 0,
	CAN_BUS_NBC_FIAT = 1,
	CAN_BUS_NBC_VAG = 2,
	CAN_BUS_MAZDA_RX8 = 3,

	Internal_ForceMyEnumIntSize_can_nbc = ENUM_SIZE_HACK,
} can_nbc_e;

#endif /* CAN_HEADER_H_ */
