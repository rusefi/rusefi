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

import com.romraider.logger.ecu.definition.EcuData;
import com.romraider.logger.ecu.definition.LoggerData;
import static com.romraider.util.HexUtil.asHex;
import static com.romraider.util.ParamChecker.checkNotNull;

public final class EcuQueryImpl implements EcuQuery {
    private final EcuData ecuData;
    private final byte[] bytes;
    private final String hex;
    private double response;

    public EcuQueryImpl(EcuData ecuData) {
        checkNotNull(ecuData);
        this.ecuData = ecuData;
        bytes = ecuData.getAddress().getBytes();
        hex = asHex(bytes);
    }

    public LoggerData getLoggerData() {
        return ecuData;
    }

    public String[] getAddresses() {
        return ecuData.getAddress().getAddresses();
    }

    public byte[] getBytes() {
        return bytes;
    }

    public String getHex() {
        return hex;
    }

    public double getResponse() {
        return response;
    }

    public void setResponse(byte[] bytes) {
        this.response = ecuData.getSelectedConvertor().convert(bytes);
    }

    public boolean equals(Object object) {
        return object instanceof EcuQueryImpl && getHex().equals(((EcuQueryImpl) object).getHex());
    }

    public int hashCode() {
        return getHex().hashCode();
    }

    public String toString() {
        return "0x" + getHex();
    }
}
