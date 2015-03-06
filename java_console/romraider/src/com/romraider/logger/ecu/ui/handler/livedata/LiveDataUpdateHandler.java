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

package com.romraider.logger.ecu.ui.handler.livedata;

import com.romraider.logger.ecu.comms.query.Response;
import com.romraider.logger.ecu.definition.ConvertorUpdateListener;
import com.romraider.logger.ecu.definition.LoggerData;
import com.romraider.logger.ecu.ui.handler.DataUpdateHandler;
import javax.swing.SwingUtilities;

public final class LiveDataUpdateHandler implements DataUpdateHandler, ConvertorUpdateListener {
    private final LiveDataTableModel dataTableModel;

    public LiveDataUpdateHandler(LiveDataTableModel dataTableModel) {
        this.dataTableModel = dataTableModel;
    }

    public synchronized void registerData(LoggerData loggerData) {
        // add to datatable
        dataTableModel.addParam(loggerData);
    }

    public synchronized void handleDataUpdate(final Response response) {
        for (final LoggerData loggerData : response.getData()) {
            SwingUtilities.invokeLater(new Runnable() {
                public void run() {
                    dataTableModel.updateParam(loggerData, response.getDataValue(loggerData));
                }
            });
        }
    }

    public synchronized void deregisterData(LoggerData loggerData) {
        // remove from datatable
        dataTableModel.removeParam(loggerData);
    }

    public synchronized void cleanUp() {
    }

    public synchronized void reset() {
        dataTableModel.reset();
    }

    public synchronized void notifyConvertorUpdate(LoggerData updatedLoggerData) {
        dataTableModel.resetRow(updatedLoggerData);
    }
}
