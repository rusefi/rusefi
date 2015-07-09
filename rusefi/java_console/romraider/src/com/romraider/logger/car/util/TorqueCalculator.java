/*
 * RomRaider Open-Source Tuning, Logging and Reflashing
 * Copyright (C) 2006-2012 RomRaider.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

package com.romraider.logger.car.util;

import static com.romraider.logger.car.util.Constants.IMPERIAL;
import static com.romraider.logger.car.util.Constants.METRIC;
import static com.romraider.logger.car.util.Constants.TQ_CONSTANT_I;
import static com.romraider.logger.car.util.Constants.TQ_CONSTANT_M;

public class TorqueCalculator {

    public static double calculateTorque(double rpm, double hp, String units) {
        double tq = 0;
        if (units.equalsIgnoreCase(IMPERIAL.value())) {
            tq = hp / rpm * Double.parseDouble(TQ_CONSTANT_I.value());
        }
        if (units.equalsIgnoreCase(METRIC.value())) {
            tq = hp / rpm * Double.parseDouble(TQ_CONSTANT_M.value());
        }
        return tq;
    }
}
