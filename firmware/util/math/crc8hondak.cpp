#include <stddef.h>
#include <stdint.h>

#include "crc8hondak.h"

static inline uint8_t crc_init(void)
{
	return 0xff;
}

static inline uint8_t crc_next(uint8_t crc, uint8_t data)
{
	uint8_t eor;
	unsigned int i = 8;

	crc ^= data;
	do {
		/* This might produce branchless code */
		eor = crc & 1 ? 0xb8 : 0;
		crc >>= 1;
		crc ^= eor;
	} while (--i);

	return crc;
}

static inline uint8_t crc_final(uint8_t crc) {
	return ~crc;
}

uint8_t crc_hondak_calc(const uint8_t *data, size_t len) {
	uint8_t crc = crc_init();

	if (len) do {
		crc = crc_next(crc, *data++);
	} while (--len);

	return crc_final(crc);
}
