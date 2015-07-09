/*
 * RomRaider Open-Source Tuning, Logging and Reflashing
 * Copyright (C) 2006-2013 RomRaider.com
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

package com.romraider.logger.ecu.comms.learning.parameter;

import static com.romraider.logger.ecu.comms.learning.parameter.Parameter.E1;
import static com.romraider.logger.ecu.comms.learning.parameter.Parameter.E31;
import static com.romraider.logger.ecu.comms.learning.parameter.Parameter.P11;
import static com.romraider.logger.ecu.comms.learning.parameter.Parameter.P115;
import static com.romraider.logger.ecu.comms.learning.parameter.Parameter.P118;
import static com.romraider.logger.ecu.comms.learning.parameter.Parameter.P153;
import static com.romraider.logger.ecu.comms.learning.parameter.Parameter.P17;
import static com.romraider.logger.ecu.comms.learning.parameter.Parameter.P2;
import static com.romraider.logger.ecu.comms.learning.parameter.Parameter.P24;
import static com.romraider.logger.ecu.comms.learning.parameter.Parameter.P66;

import java.util.HashMap;
import java.util.Map;

/**
 * A Map of Parameter and value specific to the Vehicle Information Table. 
 */
public class ParameterCrossReference {
    final Map<Parameter, String> map;

    public ParameterCrossReference() {
        map = new HashMap<Parameter, String>();
        map.put(P17,  "Battery");
        map.put(P24,  "ATM");
        map.put(P11,  "IAT");
        map.put(P2,   "ECT");
        map.put(E1,   "IAM");
        map.put(E31,  "IAM");
        map.put(P66,  "A/F #3");
        map.put(P118, "A/F #4");
        map.put(P115, "TPS VDC");
        map.put(P153, "Lrn Timing"); 
    }

/**
 * Retrieve the string value associated with the supplied Parameter.
 * @param parameter - Parameter to lookup value for.
 * @return the value of the Parameter.
 */
    public final String getValue(Parameter parameter) {
        return map.get(parameter);
    }
}
