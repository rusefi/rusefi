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

package com.romraider.logger.ecu.definition;

import static com.romraider.util.ParamChecker.checkNotNullOrEmpty;

import java.io.File;


public final class EcuDefinitionImpl implements EcuDefinition {
    private final String ecuId;
    private final String calId;
    private final String carString;
    private final String inherits;
    private final File ecuDefFile;

    public EcuDefinitionImpl(
            String ecuId, String calId, String carString,
            String inherits, File ecuDefFile) {

        checkNotNullOrEmpty(ecuId, "ecuId");
        checkNotNullOrEmpty(calId, "calId");
        checkNotNullOrEmpty(carString, "carString");
        this.ecuId = ecuId;
        this.calId = calId;
        this.carString = carString;
        this.inherits = inherits;
        this.ecuDefFile = ecuDefFile;
    }

    public String getEcuId() {
        return ecuId;
    }

    public String getCalId() {
        return calId;
    }

    public String getCarString() {
        return carString;
    }

    public String getInherits() {
        return inherits;
    }

    public File getEcuDefFile() {
        return ecuDefFile;
    }
}
