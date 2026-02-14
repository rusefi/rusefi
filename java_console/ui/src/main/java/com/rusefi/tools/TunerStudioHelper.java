package com.rusefi.tools;

import com.rusefi.FileLog;
import com.rusefi.NamedThreadFactory;

import com.rusefi.StartupFrame;
import com.rusefi.core.preferences.storage.PersistentConfiguration;
import com.rusefi.ui.TunerStudioPanel;

import javax.swing.*;
import java.awt.*;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class TunerStudioHelper {
    private static final String FIND_TS_PROCESS = "Get-Process | Where-Object {$_.MainWindowTitle -like \\\"*TunerStudio*\\\"}";

    public static NamedThreadFactory factory = new NamedThreadFactory("TSScanner");

    public static void checkTunerStudio(Container container, Runnable restoreContent) {
        factory.newThread(new Runnable() {
            @Override
            public void run() {
                if (!PersistentConfiguration.getBoolProperty(StartupFrame.CHECK_TS_RUNNING, true)) {
                    return;
                }
                boolean isTsRunning = TunerStudioHelper.isTsRunning();
                if (isTsRunning) {
                    if (PersistentConfiguration.getBoolProperty(StartupFrame.AUTO_CLOSE_TS, false)) {
                        TunerStudioHelper.attemptClosingTunerStudio();
                        return;
                    }

                    SwingUtilities.invokeLater(new Runnable() {
                        @Override
                        public void run() {
                            container.removeAll();
                            container.add(new TunerStudioPanel(restoreContent));
                            container.revalidate();
                            container.repaint();
                        }
                    });
                }
            }
        }).start();
    }

    public static void attemptClosingTunerStudio() {
        try {
            Process powerShellProcess = Runtime.getRuntime().exec("powershell \"" + FIND_TS_PROCESS + " | Stop-Process -Force \"");
            // Getting the results
            powerShellProcess.getOutputStream().close();
        } catch (IOException e) {
        }
    }

    public static boolean isTsRunning() {
        if (!FileLog.isWindows())
            return false;
        try {
            Process powerShellProcess = Runtime.getRuntime().exec("powershell \"" + FIND_TS_PROCESS + "\"");
            // Getting the results
            powerShellProcess.getOutputStream().close();

            String line;
            BufferedReader stdout = new BufferedReader(new InputStreamReader(powerShellProcess.getInputStream()));
            while ((line = stdout.readLine()) != null) {
                String lowerCase = line.toLowerCase();
                if (lowerCase.contains("javaw"))
                    return true;
            }
            stdout.close();
            return false;
        } catch (IOException e) {
            return false;
        }
    }
}
