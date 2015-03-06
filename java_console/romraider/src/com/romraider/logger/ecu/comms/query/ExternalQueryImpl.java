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

import com.romraider.logger.ecu.definition.ExternalData;
import com.romraider.logger.ecu.definition.LoggerData;
import static com.romraider.util.ParamChecker.checkNotNull;

public final class ExternalQueryImpl implements ExternalQuery {
    private final ExternalData externalData;
    private double response;

    public ExternalQueryImpl(ExternalData externalData) {
        checkNotNull(externalData);
        this.externalData = externalData;
    }

    public LoggerData getLoggerData() {
        return externalData;
    }

    public void setResponse(double response) {
        this.response = response;
    }

    public double getResponse() {
        return response;
    }
}
