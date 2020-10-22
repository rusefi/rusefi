/**
 * @file    common.h
 * @brief   Common macros definitions
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

#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#if !defined(MIN)
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif
#if !defined(MAX)
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif

#define CLAMP(x, low, high) ({\
	__typeof__(x) __x = (x); \
	__typeof__(low) __low = (low);\
	__typeof__(high) __high = (high);\
	(__x > __high) ? __high : ((__x < __low) ? __low : __x);\
})

#endif /* COMMON_H_INCLUDED */
