/**
 * @file    os_util.c
 * @brief   Utility methods related to operating system usage
 *
 * @date Nov 15, 2012
 * @author Andrey Belomutskiy, (c) 2012-2019
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

#include "global.h"

#if !EFI_UNIT_TEST
#include "os_access.h"
#include "os_util.h"

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
 * TODO: refactor to new 'syssts_t sts = chSysGetStatusAndLockX();' pattern
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

/**
 * TODO: refactor to new 'chSysRestoreStatusX(sts);' pattern
 */
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
	for (int i = 0; i < size; i++) {
		if (ptr[i] != CH_DBG_STACK_FILL_VALUE) {
			return size - i;
		}
	}
#endif /* EFI_UNIT_TEST */
	return 0;
}

