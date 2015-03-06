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

package com.romraider.logger.ecu.ui;

import com.romraider.logger.ecu.comms.controller.LoggerController;
import com.romraider.logger.ecu.definition.LoggerData;
import com.romraider.logger.ecu.ui.handler.DataUpdateHandlerManager;
import static com.romraider.util.ParamChecker.checkNotNull;
import static java.util.Collections.synchronizedList;
import java.util.ArrayList;
import java.util.List;

public final class DataRegistrationBrokerImpl implements DataRegistrationBroker {
    private final List<LoggerData> registeredLoggerData = synchronizedList(new ArrayList<LoggerData>());
    private final LoggerController controller;
    private final DataUpdateHandlerManager handlerManager;
    private final String id;

    public DataRegistrationBrokerImpl(LoggerController controller, DataUpdateHandlerManager handlerManager) {
        checkNotNull(controller, handlerManager);
        this.controller = controller;
        this.handlerManager = handlerManager;
        id = System.currentTimeMillis() + "_" + hashCode();
    }

    public synchronized void registerLoggerDataForLogging(final LoggerData loggerData) {
        if (!registeredLoggerData.contains(loggerData)) {
            // register param with handlers
            handlerManager.registerData(loggerData);

            // add logger
            controller.addLogger(id, loggerData);

            // add to registered parameters list
            registeredLoggerData.add(loggerData);
        }
    }

    public synchronized void deregisterLoggerDataFromLogging(LoggerData loggerData) {
        if (registeredLoggerData.contains(loggerData)) {
            // deregister from dependant objects
            deregisterLoggerDataFromDependants(loggerData);

            // remove from registered list
            registeredLoggerData.remove(loggerData);
        }
    }

    public synchronized void clear() {
        for (LoggerData loggerData : registeredLoggerData) {
            deregisterLoggerDataFromDependants(loggerData);
        }
        registeredLoggerData.clear();
    }

    public synchronized void connecting() {
    }

    public synchronized void readingData() {
    }

    public synchronized void loggingData() {
    }

    public synchronized void stopped() {
    }

    private void deregisterLoggerDataFromDependants(LoggerData loggerData) {
        // remove logger
        controller.removeLogger(id, loggerData);

        // deregister param from handlers
        handlerManager.deregisterData(loggerData);
    }

}
