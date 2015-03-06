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

package com.romraider.logger.ecu.comms.controller;

import static com.romraider.util.ParamChecker.checkNotNull;
import static com.romraider.util.ThreadUtil.runAsDaemon;

import org.apache.log4j.Logger;

import com.romraider.logger.ecu.comms.manager.QueryManager;
import com.romraider.logger.ecu.comms.manager.QueryManagerImpl;
import com.romraider.logger.ecu.comms.query.EcuInitCallback;
import com.romraider.logger.ecu.definition.LoggerData;
import com.romraider.logger.ecu.ui.MessageListener;
import com.romraider.logger.ecu.ui.StatusChangeListener;
import com.romraider.logger.ecu.ui.handler.DataUpdateHandler;
import com.romraider.logger.ecu.ui.handler.file.FileLoggerControllerSwitchMonitor;

public final class LoggerControllerImpl implements LoggerController {
    private static final Logger LOGGER = Logger.getLogger(LoggerControllerImpl.class);
    private final QueryManager queryManager;

    public LoggerControllerImpl(EcuInitCallback ecuInitCallback, MessageListener messageListener,
            DataUpdateHandler... dataUpdateHandlers) {
        checkNotNull(ecuInitCallback, messageListener, dataUpdateHandlers);
        queryManager = new QueryManagerImpl(ecuInitCallback, messageListener, dataUpdateHandlers);
    }

    @Override
    public synchronized void addListener(StatusChangeListener listener) {
        checkNotNull(listener, "listener");
        queryManager.addListener(listener);
    }

    @Override
    public void setFileLoggerSwitchMonitor(FileLoggerControllerSwitchMonitor monitor) {
        checkNotNull(monitor);
        LOGGER.debug("Setting file logger switch monitor: [" + monitor.getEcuSwitch().getId() + "] " + monitor.getEcuSwitch().getName());
        queryManager.setFileLoggerSwitchMonitor(monitor);
    }

    @Override
    public void addLogger(String callerId, LoggerData loggerData) {
        checkNotNull(loggerData);
        LOGGER.debug("Adding logger:   [" + loggerData.getId() + "] " + loggerData.getName());
        queryManager.addQuery(callerId, loggerData);
    }

    @Override
    public void removeLogger(String callerId, LoggerData loggerData) {
        checkNotNull(loggerData, "ecuParam");
        LOGGER.debug("Removing logger: [" + loggerData.getId() + "] " + loggerData.getName());
        queryManager.removeQuery(callerId, loggerData);
    }

    @Override
    public synchronized boolean isStarted() {
        return queryManager.isRunning();
    }

    @Override
    public synchronized void start() {
        if (!isStarted()) runAsDaemon(queryManager);
    }

    @Override
    public synchronized void stop() {
        if (isStarted() && queryManager.getThread().isAlive()) {
            queryManager.stop();
            try {
                LOGGER.debug(String.format(
                        "%s - Stopping QueryManager: %s",
                        this.getClass().getSimpleName(),
                        queryManager.getThread().getName()));
                queryManager.getThread().interrupt();
                LOGGER.debug(String.format(
                        "%s - Waiting for QueryManager %s to terminate",
                        this.getClass().getSimpleName(),
                        queryManager.getThread().getName()));
                queryManager.getThread().join(5000);
            }
            catch (InterruptedException e) {
                e.printStackTrace();
            }
            finally {
                LOGGER.debug(String.format(
                        "%s - QueryManager %s state: %s",
                        this.getClass().getSimpleName(),
                        queryManager.getThread().getName(),
                        queryManager.getThread().getState()));
            }
        }
    }
}
