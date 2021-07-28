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

package com.romraider.logger.ecu.profile.xml;

import com.romraider.logger.ecu.profile.UserProfile;
import com.romraider.logger.ecu.profile.UserProfileImpl;
import com.romraider.logger.ecu.profile.UserProfileItem;
import com.romraider.logger.ecu.profile.UserProfileItemImpl;
import org.xml.sax.Attributes;
import org.xml.sax.helpers.DefaultHandler;
import java.util.HashMap;
import java.util.Map;

public final class UserProfileHandler extends DefaultHandler {
    private static final String SELECTED = "selected";
    private static final String TAG_PROFILE = "profile";
    private static final String TAG_PARAMETER = "parameter";
    private static final String TAG_SWITCH = "switch";
    private static final String TAG_EXTERNAL = "external";
    private static final String ATTR_PROTOCOL = "protocol";
    private static final String ATTR_ID = "id";
    private static final String ATTR_UNITS = "units";
    private static final String ATTR_LIVE_DATA = "livedata";
    private static final String ATTR_GRAPH = "graph";
    private static final String ATTR_DASH = "dash";
    private Map<String, UserProfileItem> params;
    private Map<String, UserProfileItem> switches;
    private Map<String, UserProfileItem> external;
    private String protocol;

    public void startDocument() {
        params = new HashMap<String, UserProfileItem>();
        switches = new HashMap<String, UserProfileItem>();
        external = new HashMap<String, UserProfileItem>();
    }

    public void startElement(String uri, String localName, String qName, Attributes attributes) {
        if (TAG_PROFILE.equals(qName)) {
            protocol = attributes.getValue(ATTR_PROTOCOL);
        } else if (TAG_PARAMETER.equals(qName)) {
            params.put(attributes.getValue(ATTR_ID), getUserProfileItem(attributes));
        } else if (TAG_SWITCH.equals(qName)) {
            switches.put(attributes.getValue(ATTR_ID), getUserProfileItem(attributes));
        } else if (TAG_EXTERNAL.equals(qName)) {
            external.put(attributes.getValue(ATTR_ID), getUserProfileItem(attributes));
        }
    }

    public UserProfile getUserProfile() {
        return new UserProfileImpl(params, switches, external, protocol);
    }

    private UserProfileItem getUserProfileItem(Attributes attributes) {
        return new UserProfileItemImpl(
                attributes.getValue(ATTR_UNITS),
                SELECTED.equalsIgnoreCase(attributes.getValue(ATTR_LIVE_DATA)),
                SELECTED.equalsIgnoreCase(attributes.getValue(ATTR_GRAPH)),
                SELECTED.equalsIgnoreCase(attributes.getValue(ATTR_DASH))
        );
    }

}
