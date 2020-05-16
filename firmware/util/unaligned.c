/**
 * @file    unaligned.c
 * @brief   unaligned data access helpers
 *
 * @date May, 2019
 * @author Andrey Gusakov, (c) 2019
 *
 * This file is part of rusEfi - see http://rusefi.com
 *
 * rusEfi is free software; you can redistribute it and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * rusEfi is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include "unaligned.h"
#include "common.h"

/* generate mask */
static inline uint32_t bit_mask(unsigned int from, unsigned int to)
{
	uint32_t mask = 0;
	uint32_t shift;

	if (to < from)
		return 0;

	shift = to - from + 1U;

	if ((shift > 0U) && (shift <= 32U) && (from <= 31U)) {
		if (shift < 32U) {
			mask = (uint32_t)((1UL << shift) - 1UL);
			mask = mask << from;
		} else {
			mask = 0xFFFFFFFFUL;
		}
	}

	return mask;
}

/* get upto 32 bits from char array p, from bit position pos, lenght len */
uint32_t bits_get(uint8_t *p, unsigned int pos, int len)
{
	int i;
	unsigned int offset = 0;
	uint32_t res = 0;

	i = (int)pos / 8;

	while (len > 0) {
		uint32_t tmp;
		uint32_t mask;
		int nbits = 8 - ((int)pos % 8);

		/* get */
		tmp = (uint32_t)p[i];
		/* shift */
		tmp = tmp >> (8U - (uint32_t)nbits);
		/* mask */
		mask = bit_mask(0, MIN((uint32_t)len - 1U, (uint32_t)nbits - 1U));
		tmp = tmp & mask;
		res = res | ((tmp) << offset);

		/* adjust for the next iteration */
		offset += (unsigned int)nbits;
		len -= nbits;
		pos += (unsigned int)nbits;
		i++;
	}
	return res;
}

/* set upto 32 bits in char array p, from bit position pos, lenght len */
void bits_set(uint8_t *p, unsigned int pos, int len, uint32_t val)
{
	int i;
	unsigned int offset = 0;

	i = (int)pos / 8;

	while (len > 0) {
		uint32_t tmp;
		uint32_t mask;

		/* get number of bits to shift to get to the target range */
		int shift = (int)pos % 8;
		/* get next byte */
		tmp = (val >> offset) & 0xffU;
		/* shift temporary value to the start of the target range */
		tmp = tmp << (uint8_t)shift;
		/* calculate mask */
		mask = bit_mask((uint32_t)shift, MIN(8U - 1U, (unsigned int)shift + (unsigned int)len - 1U));
		/* clean all bits outside of the target range */
		tmp &= mask;
		/* pre-clean all target bits */
		p[i] = p[i] & ~((uint8_t)mask);
		/* finally set active bits */
		p[i] |= (uint8_t)tmp;

		/* adjust for the next iteration */
		offset += ((uint32_t)8U - (uint32_t)shift);
		len -= (8 - shift);
		pos += ((unsigned int)8U - (unsigned int)shift);
		i++;
	}
}
