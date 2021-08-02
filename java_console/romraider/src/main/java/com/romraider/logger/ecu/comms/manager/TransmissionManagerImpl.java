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

package com.romraider.logger.ecu.comms.manager;

//import static com.romraider.logger.ecu.comms.io.connection.LoggerConnectionFactory.getConnection;
import static com.romraider.util.ParamChecker.checkNotNull;
import static org.apache.log4j.Logger.getLogger;

import java.util.Collection;

import org.apache.log4j.Logger;

import com.romraider.Settings;
//import com.romraider.logger.ecu.comms.io.connection.LoggerConnection;
import com.romraider.logger.ecu.comms.query.EcuQuery;
import com.romraider.logger.ecu.exception.NotConnectedException;
import com.romraider.util.SettingsManager;

public final class TransmissionManagerImpl implements TransmissionManager {
    private static final Logger LOGGER = getLogger(TransmissionManagerImpl.class);
 //   private LoggerConnection connection;

    public TransmissionManagerImpl() {
    }

    @Override
    public void start() {
        try {
            Settings settings = SettingsManager.getSettings();
      //      connection = getConnection(settings.getLoggerProtocol(), settings.getLoggerPort(), settings.getLoggerConnectionProperties());
            LOGGER.info("TX Manager Started.");
        } catch (Throwable e) {
            stop();
        }
    }

    @Override
    public void sendQueries(Collection<EcuQuery> queries, PollingState pollState) {
        checkNotNull(queries, "queries");
        checkNotNull(pollState, "pollState");
//        if (connection == null) throw new NotConnectedException("TransmissionManager must be started before queries can be sent!");
//        connection.sendAddressReads(queries, SettingsManager.getSettings().getDestinationId(), pollState);
    }

    @Override
    public void endQueries() {
//        if (connection == null) throw new NotConnectedException("TransmissionManager must be started before ending queries!");
//        connection.clearLine();
    }

    @Override
    public void stop() {
//        if (connection != null) {
//            endQueries();
////            connection.close();
//        }
        LOGGER.info("TX Manager Stopped.");
    }
}
