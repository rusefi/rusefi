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

import com.romraider.io.connection.ConnectionProperties;
import com.romraider.logger.ecu.comms.query.EcuInit;
import java.io.File;
import java.util.List;
import java.util.Map;

public interface EcuDataLoader {

    void loadEcuDefsFromXml(File ecuDefsFile);

    void loadConfigFromXml(String loggerConfigFilePath, String protocol, String fileLoggingControllerSwitchId, EcuInit ecuInit);

    Map<String, EcuDefinition> getEcuDefinitionMap();

    List<EcuParameter> getEcuParameters();

    List<EcuSwitch> getEcuSwitches();

    EcuSwitch getFileLoggingControllerSwitch();

    ConnectionProperties getConnectionProperties();

    String getDefVersion();

    List<EcuSwitch> getEcuCodes();
}
