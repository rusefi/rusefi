/**
 * @file    rfiutil.c
 * @brief   Number to string conversion code
 *
 * @date Nov 15, 2012
 * @author Andrey Belomutskiy, (c) 2012-2018
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
#include "global.h"
#include "rfiutil.h"

/*
 not used, not sure if we still need it. I guess we will remove it in 2015
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
 #warning This would be better without recursion
 return mylog10(param / 10) + 1;
 }
 */
/*
 char hexChar(int v) {
 v = v & 0xF;
 if (v < 10)
 return (char)('0' + v);
 return 'A' - 10 + v;
 }
 */

#if !EFI_UNIT_TEST

void chVTSetAny(virtual_timer_t *vtp, systime_t time, vtfunc_t vtfunc, void *par) {
	bool wasLocked = lockAnyContext();

	/**
	 * todo: this could be simplified once we migrate to ChibiOS 3.0
	 * See http://www.chibios.org/dokuwiki/doku.php?id=chibios:howtos:porting_from_2_to_3
	 */
	if (chVTIsArmedI(vtp)) {
		chVTResetI(vtp);
	}

	chVTSetI(vtp, time, vtfunc, par);
	if (!wasLocked) {
		unlockAnyContext();
	}
}

/**
 * @return TRUE if already in locked context
 */
bool lockAnyContext(void) {
	int alreadyLocked = isLocked();
	if (alreadyLocked)
		return true;
#if USE_PORT_LOCK
	port_lock();
#else /* #if USE_PORT_LOCK */
	if (isIsrContext()) {
		chSysLockFromISR()
		;
	} else {
		chSysLock()
		;
	}
#endif /* #if USE_PORT_LOCK */
	return false;
}

void unlockAnyContext(void) {
#if USE_PORT_LOCK
	port_unlock();
#else /* #if USE_PORT_LOCK */
	if (isIsrContext()) {
		chSysUnlockFromISR()
		;
	} else {
		chSysUnlock()
		;
	}
#endif /* #if USE_PORT_LOCK */
}

#endif /* EFI_UNIT_TEST */


/**
 * See also getRemainingStack()
 */
int getMaxUsedStack(uint8_t *ptr, int size) {
	/**
	 * maximum used stack size total stack buffer size minus position of first modified byte
	 */
#if ! EFI_UNIT_TEST
	int used = 0;
	for (int i = 0; i < size; i++) {
		if (ptr[i] != CH_DBG_STACK_FILL_VALUE) {
			return size - i;
		}
	}
#endif /* EFI_UNIT_TEST */
	return 0;
}

