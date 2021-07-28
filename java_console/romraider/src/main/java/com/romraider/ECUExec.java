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

package com.romraider;

import com.romraider.editor.ecu.ECUEditor;
import com.romraider.util.JREChecker;
import org.apache.log4j.Logger;

import java.io.File;
import java.text.DateFormat;

import static com.romraider.Version.*;
import static com.romraider.editor.ecu.ECUEditorManager.getECUEditor;
import static com.romraider.swing.LookAndFeelManager.initLookAndFeel;
import static com.romraider.util.LogManager.initDebugLogging;
import static javax.swing.JOptionPane.*;
import static javax.swing.SwingUtilities.invokeLater;
import static org.apache.log4j.Logger.getLogger;

public class ECUExec {
    private static final Logger LOGGER = getLogger(ECUExec.class);
    private static final String START_LOGGER_ARG = "-logger";
    private static final String START_LOGGER_FULLSCREEN_ARG = "-logger.fullscreen";

    private ECUExec() {
        throw new UnsupportedOperationException();
    }

    public static void main(String args[]) {
        // init debug logging
        initDebugLogging();
        // dump the system properties to the log file as early as practical to
        // help debugging/support
        LOGGER.info(PRODUCT_NAME + " " + VERSION + " Build: " + BUILDNUMBER);
        LOGGER.info("When requesting assistance at " + SUPPORT_URL + " please include the System Properties information below:");
        LOGGER.info(DateFormat.getDateTimeInstance(DateFormat.FULL, DateFormat.LONG).format(System.currentTimeMillis()));
        LOGGER.info("System Properties: \n\t"
                + System.getProperties().toString().replace(",", "\n\t"));

        // 64-bit won't work with the native libs (e.g. serial rxtx) but won't
        // fail until we actually try to use them we'll just warn here
        if (!JREChecker.is32bit() &&
                !containsLoggerArg(args)) {
            showMessageDialog(null,
                    "Incompatible JRE detected.\n" +
                            PRODUCT_NAME +
                            " requires a 32-bit JRE for some operations.\nSome features may be unavailable.",
                            "JRE Incompatibility Warning",
                            WARNING_MESSAGE);
        }

        // check for dodgy threading - dev only
        //RepaintManager.setCurrentManager(new ThreadCheckingRepaintManager(true));

        // set look and feel
        initLookAndFeel();

        // check if already running
                openEditor(args);
    }

    private static void showAlreadyRunningMessage() {
        showMessageDialog(null, PRODUCT_NAME + " is already running.", PRODUCT_NAME, INFORMATION_MESSAGE);
    }

    private static boolean containsLoggerArg(String[] args) {
        for (String arg : args) {
            if (arg.equalsIgnoreCase(START_LOGGER_ARG) || arg.equalsIgnoreCase(START_LOGGER_FULLSCREEN_ARG)) {
                return true;
            }
        }
        return false;
    }

    public static void openRom(final ECUEditor editor, final String rom) {
        invokeLater(new Runnable() {
            @Override
            public void run() {
                try {
                    File file = new File(rom);
                    editor.openImage(file);
                } catch (Exception ex) {
                    LOGGER.error("Error opening rom", ex);
                }
            }
        });
    }

    private static void openEditor(String[] args) {
        ECUEditor editor = getECUEditor();
        editor.initializeEditorUI();
        editor.checkDefinitions();

        if (args.length > 0) {
            openRom(editor, args[0]);
        }
        //startRomListener(editor);
    }

//    private static void startRomListener(ECUEditor editor) {
//        try {
//            while (true) {
//                String rom = waitForRom();
//                openRom(editor, rom);
//            }
//        } catch (Throwable e) {
//            LOGGER.error("Error occurred", e);
//        }
//    }
}
