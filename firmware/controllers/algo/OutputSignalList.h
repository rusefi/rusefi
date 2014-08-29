/*
 * @file OutputSignalList.h
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

#ifndef OUTPUTSIGNALLIST_H_
#define OUTPUTSIGNALLIST_H_

#include "signal_executor.h"
#include "io_pins.h"

// todo: this value is too low for 6 cyl engine, get it back to 60
#define OUTPUT_SIGNAL_MAX_SIZE 90

class OutputSignalList {
public:
	OutputSignalList();
	void clear();
	OutputSignal * add(io_pin_e ioPin);
	int getSize();

private:
	int size;
	OutputSignal signals[OUTPUT_SIGNAL_MAX_SIZE];

};


#endif /* OUTPUTSIGNALLIST_H_ */
