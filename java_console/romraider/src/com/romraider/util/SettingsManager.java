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

package com.romraider.util;

import com.romraider.Settings;
import com.romraider.swing.JProgressPane;
import com.romraider.xml.DOMSettingsBuilder;
import com.romraider.xml.DOMSettingsUnmarshaller;
import com.rusefi.FileLog;
import com.sun.org.apache.xerces.internal.parsers.DOMParser;
import org.w3c.dom.Document;
import org.xml.sax.InputSource;
import org.xml.sax.SAXException;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;

import static com.romraider.Version.VERSION;

public class SettingsManager {
    private static final String SETTINGS_FILE = "/romraider_settings.xml";
    private static final String USER_HOME =
            System.getProperty("user.home") + "/.rusEFI";
    private static final String START_DIR = System.getProperty("user.dir");
    private static String settingsDir = USER_HOME;

    private static Settings settings = null;

    public static Settings getSettings() {
        if(null == settings) {
            settings = load();
        }
        return settings;
    }

    private static Settings load() {
        Settings loadedSettings;
        try {
            FileInputStream settingsFileIn;
            try {
                final File sf = new File(USER_HOME + SETTINGS_FILE);
                settingsFileIn = new FileInputStream(sf);
            }
            catch (Exception e) {
                final File sf = new File(START_DIR + SETTINGS_FILE);
                settingsFileIn = new FileInputStream(sf);
                settingsDir = START_DIR;
            }
            final InputSource src = new InputSource(settingsFileIn);
            final DOMSettingsUnmarshaller domUms = new DOMSettingsUnmarshaller();
            final DOMParser parser = new DOMParser();
            parser.parse(src);
            final Document doc = parser.getDocument();
            loadedSettings = domUms.unmarshallSettings(doc.getDocumentElement());
        } catch (SAXException | IOException e) {
            FileLog.MAIN.logLine("Settings file not found or damaged. Using default settings.");
            loadedSettings = new Settings();
        }
        return loadedSettings;
    }

    public static void save(Settings newSettings) {
        save(newSettings, new JProgressPane());
    }

    public static void save(Settings newSettings, JProgressPane progress) {
        final DOMSettingsBuilder builder = new DOMSettingsBuilder();
        try {
            final File newDir = new File(settingsDir);
            newDir.mkdir();		// Creates directory if it does not exist
            final File sf = new File(settingsDir + SETTINGS_FILE);
            builder.buildSettings(newSettings, sf, progress, VERSION);
            settings = newSettings;
        } catch (Exception e) {
            // Load the settings from disk.
            settings = load();
            throw new RuntimeException(e);
        }
    }
}
