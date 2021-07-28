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

package com.romraider.net;

import org.apache.log4j.Logger;
import java.io.IOException;
import java.lang.reflect.Method;
import java.net.URI;

public class BrowserControl {
    private static final Logger LOGGER = Logger.getLogger(BrowserControl.class);

    private BrowserControl() {
        throw new UnsupportedOperationException();
    }

    public static void displayURL(String url) {
        try {
            Class<?> display = Class.forName("java.awt.Desktop");
            Object getDesktopMethod = display.getDeclaredMethod("getDesktop").invoke(null);
            Method browseMethod = display.getDeclaredMethod("browse", java.net.URI.class);
            browseMethod.invoke(getDesktopMethod, new URI(url));
        } catch (Exception e) {
            LOGGER.debug("Failed to display URL via java.awt.Desktop. Calling by OS depended method.", e);
            displayURLtraditional(url);
        }
    }

    private static void displayURLtraditional(String url) {
        boolean windows = isWindowsPlatform();
        String cmd = null;
        try {
            if (windows) {
                // cmd = 'rundll32 url.dll,FileProtocolHandler http://...'
                cmd = WIN_PATH + " " + WIN_FLAG + " " + url;
                Runtime.getRuntime().exec(cmd);
            } else {
                cmd = UNIX_PATH + " " + UNIX_FLAG + "(" + url + ")";
                Process p = Runtime.getRuntime().exec(cmd);
                try {
                    int exitCode = p.waitFor();
                    if (exitCode != 0) {
                        cmd = UNIX_PATH + " " + url;
                        Runtime.getRuntime().exec(cmd);
                    }
                }
                catch (InterruptedException x) {
                    LOGGER.error("Error bringing up browser, command=" + cmd, x);
                }
            }
        }
        catch (IOException x) {
            // couldn't exec browser
            LOGGER.error("Could not invoke browser, command=" + cmd, x);
        }
    }

    public static boolean isWindowsPlatform() {
        String os = System.getProperty("os.name");
        if (os != null && os.startsWith(WIN_ID)) {
            return true;
        } else {
            return false;
        }
    }

    private static final String WIN_ID = "Windows";
    private static final String WIN_PATH = "rundll32";
    private static final String WIN_FLAG = "url.dll,FileProtocolHandler";
    private static final String UNIX_PATH = "netscape";
    private static final String UNIX_FLAG = "-remote openURL";
}
