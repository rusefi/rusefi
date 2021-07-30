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

import java.util.EnumSet;
import java.util.HashMap;
import java.util.Map;

/**
 * This Emun defines all the possible parameters used to query the Learning
 * Table Values of an ECU. 
 */
public enum Parameter {
    E1("E1"),       // IAM 16 bit
    E31("E31"),     // IAM 32 bit
    E12("E12"),     // Fine Learning Knock Correction*
    E41("E41"),     // Fine Learning Knock Correction (4-byte)*
    E13("E13"),     // AF Learning 1A 16 bit
    E14("E14"),     // AF Learning 1B 16 bit
    E15("E15"),     // AF Learning 1C 16 bit
    E16("E16"),     // AF Learning 1D 16 bit
    E44("E44"),     // AF Learning 1A 32 bit
    E45("E45"),     // AF Learning 1B 32 bit
    E46("E46"),     // AF Learning 1C 32 bit
    E47("E47"),     // AF Learning 1D 32 bit
    E62("E62"),     // AF Learning 2A 32 bit
    E63("E63"),     // AF Learning 2B 32 bit
    E64("E64"),     // AF Learning 2C 32 bit
    E65("E65"),     // AF Learning 2D 32 bit
    P2("P2"),       // ECT
    P11("P11"),     // IAT
    P17("P17"),     // Battery Volts
    P24("P24"),     // ATM
    P66("P66"),     // AF Learning 3
    P115("P115"),   // Learned Throttle Closed Voltage
    P118("P118"),   // AF Learning 4
    P153("P153"),   // Whole Learning value in timing
    E173("E173");   // Fine Learning Knock Correction Table Start (4-byte)*

    private static final Map<String, Parameter> lookup
            = new HashMap<String, Parameter>();

    static {
        for(Parameter s : EnumSet.allOf(Parameter.class))
        lookup.put(s.toString(), s);
    }

    private Parameter(final String text) {
        this.text = text;
    }

    private final String text;

    @Override
    public final String toString() {
        return text;
    }

    /**
     * Retrieve the Parameter that has the given value. 
     * @param value - the value of the Parameter in String format
     * @return the Parameter that has the given value or null if undefined. 
     */
    public static Parameter fromValue(String value) {
        Parameter result = null;
        if (lookup.containsKey(value)) {
            result = lookup.get(value);
        }
        return result;
    }
}
