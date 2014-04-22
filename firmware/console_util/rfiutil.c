/**
 * @file    rfiutil.c
 * @brief   Number to string conversion code
 *
 * @date Nov 15, 2012
 * @author Andrey Belomutskiy, (c) 2012-2014
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

#include <string.h>
#include "rfiutil.h"

int mylog10(int param) {
	if (param < 10)
		return 0;
	if (param < 100)
		return 1;
	if (param < 1000)
		return 2;
	if (param < 10000)
		return 3;
	if (param < 100000)
		return 4;
	if (param < 1000000)
		return 5;
	if (param < 10000000)
		return 6;
	if (param < 100000000)
		return 7;
	return mylog10(param / 10) + 1;
}

static char *ltoa_internal(char *p, long num, unsigned radix) {
	int i;
	char *q;

	q = p + _MAX_FILLER;
	do {
		i = (int) (num % radix);
		i += '0';
		if (i > '9')
			i += 'A' - '0' - 10;
		*--q = i;
	} while ((num /= radix) != 0);

	i = (int) (p + _MAX_FILLER - q);
	do
		*p++ = *q++;
	while (--i);

	return p;
}

static char* itoa_signed(uint8_t *p, int num, unsigned radix) {
	if (num < 0) {
		*p++ = '-';
		char *end = ltoa_internal(p, -num, radix);
		*end = 0;
		return end;
	}
	char *end = ltoa_internal(p, num, radix);
	*end = 0;
	return end;
}

/**
 * Integer to string
 */
char* itoa10(uint8_t *p, int num) {
// todo: unit test
	return itoa_signed(p, num, 10);
}

char hexChar(int v) {
	v = v & 0xF;
	if (v < 10)
		return '0' + v;
	return 'A' - 10 + v;
}

// todo: why does it not compile if I make this function 'inline'?
int isIsrContext(void) {
	/**
	 * Unfortunately ChibiOS has two versions of methods for different
	 * contexts.
	 */
	return dbg_isr_cnt > 0;
}

void chVTSetAny(VirtualTimer *vtp, systime_t time, vtfunc_t vtfunc, void *par) {
	if (isIsrContext()) {
		chSysLockFromIsr()
		;

		if (chVTIsArmedI(vtp))
			chVTResetI(vtp);

		chVTSetI(vtp, time, vtfunc, par);
		chSysUnlockFromIsr()
		;
	} else {
		chSysLock()
		;
		if (chVTIsArmedI(vtp))
			chVTResetI(vtp);
		chVTSetI(vtp, time, vtfunc, par);
		chSysUnlock()
		;
	}
}

/**
 * @brief This function knows how to print a histogram_s summary
 */
void printHistogram(Logging *logging, histogram_s *histogram) {
	int report[5];
	int len = hsReport(histogram, report);

	resetLogging(logging);
	appendMsgPrefix(logging);
	appendPrintf(logging, "histogram %s *", histogram->name);
	for (int i = 0; i < len; i++)
		appendPrintf(logging, "%d ", report[i]);
	appendPrintf(logging, "*");
	appendMsgPostfix(logging);
	scheduleLogging(logging);
}
