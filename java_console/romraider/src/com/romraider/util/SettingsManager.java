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

import static com.romraider.Version.VERSION;
import static javax.swing.JOptionPane.INFORMATION_MESSAGE;
import static javax.swing.JOptionPane.showMessageDialog;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;

import org.w3c.dom.Document;
import org.xml.sax.InputSource;

import com.romraider.Settings;
import com.romraider.swing.JProgressPane;
import com.romraider.xml.DOMSettingsBuilder;
import com.romraider.xml.DOMSettingsUnmarshaller;
import com.sun.org.apache.xerces.internal.parsers.DOMParser;

public class SettingsManager {
    private static final String SETTINGS_FILE = "/settings.xml";
    private static final String USER_HOME =
            System.getProperty("user.home") + "/.RomRaider";
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
            FileInputStream settingsFileIn = null;
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
        } catch (FileNotFoundException e) {
            showMessageDialog(null,
                    "Settings file not found.\nUsing default settings.",
                    "Error Loading Settings", INFORMATION_MESSAGE);
            loadedSettings = new Settings();
        } catch (Exception e) {
            throw new RuntimeException(e);
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
