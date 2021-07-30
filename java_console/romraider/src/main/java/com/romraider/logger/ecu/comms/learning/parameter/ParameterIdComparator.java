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

import java.util.Comparator;

import com.romraider.logger.ecu.comms.query.EcuQuery;

/**
 * A custom comparator to sort EcuQuery items based on an encapsulated ID field. 
 */
public final class ParameterIdComparator implements Comparator<EcuQuery> {

    public int compare(EcuQuery ecuQuery1, EcuQuery ecuQuery2) {
        return ecuQuery1.getLoggerData().getId().compareTo(
                ecuQuery2.getLoggerData().getId());
    }
}
