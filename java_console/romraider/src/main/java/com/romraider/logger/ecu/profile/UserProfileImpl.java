/*
 * RomRaider Open-Source Tuning, Logging and Reflashing
 * Copyright (C) 2006-2014 RomRaider.com
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

package com.romraider.logger.ecu.profile;

import com.romraider.Settings;
import com.romraider.logger.ecu.definition.EcuDataConvertor;
import com.romraider.logger.ecu.definition.EcuParameter;
import com.romraider.logger.ecu.definition.EcuSwitch;
import com.romraider.logger.ecu.definition.ExternalData;
import com.romraider.logger.ecu.definition.LoggerData;
import com.romraider.logger.ecu.exception.ConfigurationException;
import com.romraider.util.SettingsManager;

import static com.romraider.util.ParamChecker.checkNotNull;
import static com.romraider.util.ParamChecker.isNullOrEmpty;
import java.util.Map;

public final class UserProfileImpl implements UserProfile {
    private static final String NEW_LINE = System.getProperty("line.separator");
    private final Map<String, UserProfileItem> params;
    private final Map<String, UserProfileItem> switches;
    private final Map<String, UserProfileItem> external;
    private final String protocol;

    public UserProfileImpl(
            Map<String, UserProfileItem> params,
            Map<String, UserProfileItem> switches,
            Map<String, UserProfileItem> external,
            String protocol) {

        checkNotNull(params, "params");
        checkNotNull(switches, "switches");
        checkNotNull(external, "external");
        this.params = params;
        this.switches = switches;
        this.external = external;
        this.protocol = protocol;
    }

    public boolean contains(LoggerData loggerData) {
        checkNotNull(loggerData, "loggerData");
        return getMap(loggerData).keySet().contains(loggerData.getId());
    }

    public boolean isSelectedOnLiveDataTab(LoggerData loggerData) {
        checkNotNull(loggerData, "loggerData");
        return contains(loggerData) && getUserProfileItem(loggerData).isLiveDataSelected();
    }

    public boolean isSelectedOnGraphTab(LoggerData loggerData) {
        checkNotNull(loggerData, "loggerData");
        return contains(loggerData) && getUserProfileItem(loggerData).isGraphSelected();
    }

    public boolean isSelectedOnDashTab(LoggerData loggerData) {
        checkNotNull(loggerData, "loggerData");
        return contains(loggerData) && getUserProfileItem(loggerData).isDashSelected();
    }

    public EcuDataConvertor getSelectedConvertor(LoggerData loggerData) {
        checkNotNull(loggerData, "loggerData");
        if (contains(loggerData)) {
            String defaultUnits = getUserProfileItem(loggerData).getUnits();
            if (defaultUnits != null && loggerData.getConvertors().length > 1) {
                for (EcuDataConvertor convertor : loggerData.getConvertors()) {
                    if (defaultUnits.equals(convertor.getUnits())) return convertor;
                }
                throw new ConfigurationException("Unknown default units, '" + defaultUnits + "', specified for [" + loggerData.getId() + "] " + loggerData.getName());
            }
        }
        return loggerData.getSelectedConvertor();
    }

    public byte[] getBytes() {
        return buildXml().getBytes();
    }

    @Override
    public String getProtocol() {
        return protocol;
    }

    private String buildXml() {
        final Settings settings = SettingsManager.getSettings();
        StringBuilder builder = new StringBuilder();
        builder.append("<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>").append(NEW_LINE);
        builder.append("<!DOCTYPE profile SYSTEM \"profile.dtd\">").append(NEW_LINE).append(NEW_LINE);
        builder.append("<profile protocol=\"").append(settings.getLoggerProtocol().toUpperCase()).append("\">").append(NEW_LINE);
        if (!params.isEmpty()) {
            builder.append("    <parameters>").append(NEW_LINE);
            appendLoggerDataElements(builder, "parameter", params, true);
            builder.append("    </parameters>").append(NEW_LINE);
        }
        if (!switches.isEmpty()) {
            builder.append("    <switches>").append(NEW_LINE);
            appendLoggerDataElements(builder, "switch", switches, false);
            builder.append("    </switches>").append(NEW_LINE);
        }
        if (!external.isEmpty()) {
            builder.append("    <externals>").append(NEW_LINE);
            // Comment out the next line to disable Externals from being saved
            // as there seems to be a bug when a profile is reloaded, the 
            // Logger tries to connect twice causing one or both to fail. 
            appendLoggerDataElements(builder, "external", external, true);
            builder.append("    </externals>").append(NEW_LINE);
        }
        builder.append("</profile>").append(NEW_LINE);
        return builder.toString();
    }

    private void appendLoggerDataElements(StringBuilder builder, String dataType, Map<String, UserProfileItem> dataMap, boolean showUnits) {
        for (String id : dataMap.keySet()) {
            UserProfileItem item = dataMap.get(id);
            builder.append("        <").append(dataType).append(" id=\"").append(id).append("\"");
            if (item.isLiveDataSelected()) builder.append(" livedata=\"selected\"");
            if (item.isGraphSelected()) builder.append(" graph=\"selected\"");
            if (item.isDashSelected()) builder.append(" dash=\"selected\"");
            if (showUnits && !isNullOrEmpty(item.getUnits()))
                builder.append(" units=\"").append(item.getUnits()).append("\"");
            builder.append("/>").append(NEW_LINE);
        }
    }

    private UserProfileItem getUserProfileItem(LoggerData loggerData) {
        return getMap(loggerData).get(loggerData.getId());
    }

    private Map<String, UserProfileItem> getMap(LoggerData loggerData) {
        if (loggerData instanceof EcuParameter) return params;
        else if (loggerData instanceof EcuSwitch) return switches;
        else if (loggerData instanceof ExternalData) return external;
        else throw new UnsupportedOperationException("Unknown LoggerData type: " + loggerData.getClass());
    }
}
