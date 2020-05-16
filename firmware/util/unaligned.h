/**
 * @file    unaligned.h
 * @brief   unaligned data access helpers header file
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

#ifndef UNALIGNED_H_INCLUDED
#define UNALIGNED_H_INCLUDED

#include <stdint.h>

/* bit operations */
uint32_t bits_get(uint8_t *p, unsigned int pos, int len);
void bits_set(uint8_t *p, unsigned int pos, int len, uint32_t val);
#define bit_set(p, pos)		do {(p)[(pos) / 8] |= (1 << ((pos) % 8));} while(0)
#define bit_clr(p, pos)		do {(p)[(pos) / 8] &= ~(1 << ((pos) % 8));} while(0)
#define bit_get(p, pos)		(!!((p)[(pos) / 8] & (1 << ((pos) % 8))))

/* unaligned access */
static inline void put_be8(uint8_t *p, uint8_t v)
{
	p[0] = v;
}

static inline void put_le8(uint8_t *p, uint8_t v)
{
	p[0] = v;
}

static inline void put_be16(uint8_t *p, uint16_t v)
{
	p[0] = (uint8_t)(v >> 8);
	p[1] = (uint8_t)(v >> 0);
}

static inline void put_le16(uint8_t *p, uint16_t v)
{
	p[0] = (uint8_t)(v >> 0);
	p[1] = (uint8_t)(v >> 8);
}

static inline void put_be32(uint8_t *p, uint32_t v)
{
	p[0] = (uint8_t)(v >> 24);
	p[1] = (uint8_t)(v >> 16);
	p[2] = (uint8_t)(v >> 8);
	p[3] = (uint8_t)(v >> 0);
}

static inline void put_le32(uint8_t *p, uint32_t v)
{
	p[0] = (uint8_t)(v >> 0);
	p[1] = (uint8_t)(v >> 8);
	p[2] = (uint8_t)(v >> 16);
	p[3] = (uint8_t)(v >> 24);
}

static inline uint8_t get_be8(uint8_t *p)
{
	return p[0];
}

static inline uint8_t get_le8(uint8_t *p)
{
	return p[0];
}

static inline uint16_t get_be16(uint8_t *p)
{
	return ((uint16_t)p[0] << 8) | p[1];
}

static inline uint16_t get_le16(uint8_t *p)
{
	return ((uint16_t)p[1] << 8) | p[0];
}

static inline uint32_t get_be32(uint8_t *p)
{
	return ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) |
		   ((uint32_t)p[2] << 8) | p[3];
}

static inline uint32_t get_le32(uint8_t *p)
{
	return ((uint32_t)p[3] << 24) | ((uint32_t)p[2] << 16) |
		   ((uint32_t)p[1] << 8) | p[0];
}

#endif /* UNALIGNED_H_INCLUDED */
