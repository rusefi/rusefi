package com.rusefi;

import com.irnems.FileLog;
import com.rusefi.io.tcp.TcpConnector;

import javax.swing.*;
import java.awt.*;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.URL;
import java.util.HashMap;
import java.util.Map;

import static com.irnems.Launcher.*;

/**
 * This class checks the recommended versions numbers and compares them with current versions
 * <p/>
 * (c) Andrey Belomutskiy
 * 8/10/14
 */
public class VersionChecker {
    private static final String JAVA_CONSOLE_TAG = "java_console";
    private static final String FIRMWARE_TAG = "firmware";
    private static final String VERSIONS_URL = "http://rusefi.com/console/versions.txt";

    private static final VersionChecker instance = new VersionChecker();


    private final Map<String, Integer> map = new HashMap<>();
    private int previousReportedVersion;

    private VersionChecker() {
    }

    public static void start() {
        Thread t = new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    instance.readAndProcess();
                } catch (IOException e) {
                    e.printStackTrace();
                    FileLog.MAIN.logLine("Failed to read from " + VERSIONS_URL);
                }
            }
        }, "version checker");
        t.setDaemon(true);
        t.start();
    }

    private void readAndProcess() throws IOException {
        URL url = new URL(VERSIONS_URL);
        BufferedReader s = new BufferedReader(new InputStreamReader(url.openStream()));

        FileLog.MAIN.logLine("Reading from " + VERSIONS_URL);

        String line;
        while ((line = s.readLine()) != null) {
            String[] pair = line.split("=");
            if (pair.length == 2)
                map.put(pair[0], TcpConnector.parseIntWithReason(pair[1], "VC value"));
        }


        final Integer javaVersion = map.get(JAVA_CONSOLE_TAG);
        System.out.println("Server recommends java_console version " + javaVersion + " or newer");
        showUpdateWarningIfNeeded("dev console", javaVersion, CONSOLE_VERSION);
        System.out.println("Server recommends firmware " + map.get(FIRMWARE_TAG) + " or newer");
    }

    private static void showUpdateWarningIfNeeded(final String componentName, final Integer latestVersion, final int currentVersion) {
        if (latestVersion == null || currentVersion >= latestVersion)
            return;
        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                Component parent = JFrame.getFrames()[0];
                String message = "It's time to update " + componentName + "!\r\n" +
                        "Your version: " + currentVersion + "\r\n" +
                        "Latest version: " + latestVersion;
                JOptionPane.showMessageDialog(parent, message, "Update", JOptionPane.WARNING_MESSAGE);
            }
        });
    }

    public static VersionChecker getInstance() {
        return instance;
    }

    public void onFirmwareVersion(String firmwareString) {
        Integer latestVersion = map.get(FIRMWARE_TAG);
        if (latestVersion == null) {
            // no version file yet? nothing to bother about
            return;
        }
        String[] tokens = firmwareString.split("[@\\s]");
        int version;
        try {
            version = Integer.parseInt(tokens[1]);
        } catch (NumberFormatException e) {
            FileLog.MAIN.logLine("Error processing version [" + firmwareString + "]");
            return;
        }
        if (version == previousReportedVersion) {
            // no reason to bother our user too often
            return;
        }
        previousReportedVersion = version;
        showUpdateWarningIfNeeded("firmware", latestVersion, version);
    }
}
