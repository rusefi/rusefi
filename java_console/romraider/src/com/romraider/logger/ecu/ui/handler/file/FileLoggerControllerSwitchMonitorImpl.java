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

package com.romraider.logger.ecu.ui.handler.file;

import com.romraider.logger.ecu.definition.EcuSwitch;
import static com.romraider.util.ParamChecker.checkNotNull;

public final class FileLoggerControllerSwitchMonitorImpl implements FileLoggerControllerSwitchMonitor {
    private final EcuSwitch fileLoggingSwitch;
    private final FileLoggerControllerSwitchHandler handler;

    public FileLoggerControllerSwitchMonitorImpl(EcuSwitch fileLoggingSwitch, FileLoggerControllerSwitchHandler handler) {
        checkNotNull(fileLoggingSwitch, handler);
        this.fileLoggingSwitch = fileLoggingSwitch;
        this.handler = handler;
    }

    public void monitorFileLoggerSwitch(double switchValue) {
        handler.handleSwitch(switchValue);
    }

    public EcuSwitch getEcuSwitch() {
        return fileLoggingSwitch;
    }
}
