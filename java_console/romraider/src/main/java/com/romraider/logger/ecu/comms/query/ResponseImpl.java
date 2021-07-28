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

package com.romraider.logger.ecu.comms.query;

import com.romraider.logger.ecu.definition.LoggerData;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.Set;

public class ResponseImpl implements Response {
    private static final double ZERO = 0.0;
    private final Map<LoggerData, Double> dataValues = new LinkedHashMap<LoggerData, Double>();
    private final long timestamp;

    public ResponseImpl() {
        timestamp = System.currentTimeMillis();
    }

    public void setDataValue(LoggerData data, double value) {
        dataValues.put(data, value);
    }

    public Set<LoggerData> getData() {
        return dataValues.keySet();
    }

    public double getDataValue(LoggerData data) {
        final Double value = dataValues.get(data);
        return value == null ? ZERO : value;
    }

    public long getTimestamp() {
        return timestamp;
    }
}
