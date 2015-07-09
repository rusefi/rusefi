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

package com.romraider.logger.ecu.profile;

import com.romraider.logger.ecu.profile.xml.UserProfileHandler;
import static com.romraider.util.ParamChecker.checkNotNullOrEmpty;
import static com.romraider.util.SaxParserFactory.getSaxParser;
import org.apache.log4j.Logger;
import org.xml.sax.SAXParseException;
import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;

public final class UserProfileLoaderImpl implements UserProfileLoader {
    private static final Logger LOGGER = Logger.getLogger(UserProfileLoaderImpl.class);

    public UserProfile loadProfile(String userProfileFilePath) {
        checkNotNullOrEmpty(userProfileFilePath, "userProfileFilePath");
        LOGGER.info("Loading profile: " + userProfileFilePath);
        try {
            InputStream inputStream = new BufferedInputStream(new FileInputStream(new File(userProfileFilePath)));
            try {
                UserProfileHandler handler = new UserProfileHandler();
                getSaxParser().parse(inputStream, handler);
                return handler.getUserProfile();
            } finally {
                inputStream.close();
            }
        } catch (SAXParseException spe) {
            // catch general parsing exception - enough people don't unzip the defs that a better error message is in order
            LOGGER.error("Error loading user profile file: " + userProfileFilePath + ".  Please make sure the definition file is correct.  If it is in a ZIP archive, unzip the file and try again.");
            return null;
        } catch (Exception e) {
            LOGGER.error("Error loading user profile file: " + userProfileFilePath, e);
            return null;
        }
    }
}
