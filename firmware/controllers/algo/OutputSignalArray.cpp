/**
 * @file OutputSignalArray.cpp
 *
 * @date Mar 20, 2014
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

#include "OutputSignalList.h"

OutputSignalList::OutputSignalList() {
	clear();
}

void OutputSignalList::clear() {
	size = 0;
}

OutputSignal * OutputSignalList::add(io_pin_e ioPin) {
	if(size==OUTPUT_SIGNAL_MAX_SIZE) {
		firmwareError("Too many signals, adding %d", ioPin);
		return NULL;
	}

	OutputSignal *signal = &signals[size++];

	initOutputSignal(signal, ioPin);

	return signal;
}

