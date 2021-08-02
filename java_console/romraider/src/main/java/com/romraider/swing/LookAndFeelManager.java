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

package com.romraider.swing;

import static com.romraider.Version.PRODUCT_NAME;
import static com.romraider.util.Platform.LINUX;
import static com.romraider.util.Platform.MAC_OS_X;
import static com.romraider.util.Platform.isPlatform;
import static javax.swing.UIManager.getCrossPlatformLookAndFeelClassName;
import static javax.swing.UIManager.getSystemLookAndFeelClassName;
import static javax.swing.UIManager.setLookAndFeel;

import javax.swing.JDialog;
import javax.swing.JFrame;

import org.apache.log4j.Logger;

public final class LookAndFeelManager {
    private static final Logger LOGGER = Logger.getLogger(LookAndFeelManager.class);

    private LookAndFeelManager() {
        throw new UnsupportedOperationException();
    }

    public static void initLookAndFeel() {
        try {
            if (isPlatform(MAC_OS_X)) {
                System.setProperty("apple.awt.rendering", "true");
                System.setProperty("apple.awt.brushMetalLook", "true");
                System.setProperty("apple.laf.useScreenMenuBar", "true");
                System.setProperty("apple.awt.window.position.forceSafeCreation", "true");
                System.setProperty("com.apple.mrj.application.apple.menu.about.name", PRODUCT_NAME);
            }

            setLookAndFeel(getLookAndFeel());

            // make sure we have nice window decorations.
            JFrame.setDefaultLookAndFeelDecorated(true);
            JDialog.setDefaultLookAndFeelDecorated(true);

        } catch (Exception ex) {
            LOGGER.error("Error loading system look and feel.", ex);
        }
    }

    private static String getLookAndFeel() {
//        if (true) return "com.sun.java.swing.plaf.nimbus.NimbusLookAndFeel";
        // Linux has issues with the gtk look and feel themes.
        if (isPlatform(LINUX)) return getCrossPlatformLookAndFeelClassName();
        return getSystemLookAndFeelClassName();
    }
}
