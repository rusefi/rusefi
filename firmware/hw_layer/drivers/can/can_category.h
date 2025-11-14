/**
 * can_category.h
 */

#pragma once

enum class CanCategory : uint16_t {
	NBC = 0,
	VERBOSE = 1,
	LUA = 2,
	DOWNSTREAM_FLASHING = 3,
	SERIAL = 4,
	WBO_SERVICE = 5,
	OBD = 6,
	BENCH_TEST = 7,
	CAN_IOBOX = 8,
	GDI = 9,
	TCU = 10,
};
