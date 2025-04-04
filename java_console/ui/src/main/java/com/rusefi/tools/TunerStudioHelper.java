package com.rusefi.tools;

import com.rusefi.ConsoleUI;
import com.rusefi.FileLog;

import javax.swing.*;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

import static com.rusefi.ui.util.UiUtils.createOnTopParent;

public class TunerStudioHelper {
    private static final String FIND_TS_PROCESS = "Get-Process | Where-Object {$_.MainWindowTitle -like \\\"*TunerStudio*\\\"}";

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

    public static void maybeCloseTs() {
        boolean isTsRunning = isTsRunning();
        if (isTsRunning) {
            int result = JOptionPane.showConfirmDialog(createOnTopParent(), "Looks like TunerStudio is running, shall we close it?",
                ConsoleUI.TITLE, JOptionPane.YES_NO_OPTION);
            if (result == JOptionPane.YES_OPTION)
                attemptClosingTunerStudio();
        }
    }
}
