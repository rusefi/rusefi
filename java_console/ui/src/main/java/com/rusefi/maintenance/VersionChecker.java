package com.rusefi.maintenance;

import com.rusefi.FileLog;
import com.rusefi.models.Utils;
import com.rusefi.ui.util.URLLabel;

import javax.swing.*;
import java.awt.*;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.URL;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;

import static com.rusefi.Launcher.*;
import static com.rusefi.ui.storage.PersistentConfiguration.getConfig;

/**
 * This class checks the recommended versions numbers and compares them with current versions
 * <p/>
 * Andrey Belomutskiy, (c) 2013-2020
 * 8/10/14
 */
public class VersionChecker {
    private static final String JAVA_CONSOLE_TAG = "java_console";
    private static final String FIRMWARE_TAG = "firmware";
    private static final String VERSIONS_URL = "https://rusefi.com/console/versions.txt";

    private static final VersionChecker instance = new VersionChecker();

    private final Map<String, String> map = new HashMap<>();
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
                    FileLog.MAIN.logLine("Failed to read from " + VERSIONS_URL + e);
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
                map.put(pair[0], pair[1]);
        }

        final Integer javaVersion = parseNotNull(map.get(JAVA_CONSOLE_TAG), "VC value");
        System.out.println(new Date() + ": Server recommends java_console version " + javaVersion + " or newer");
        showUpdateWarningIfNeeded("dev console", javaVersion, CONSOLE_VERSION);
        System.out.println(new Date() + ": Server recommends firmware " + map.get(FIRMWARE_TAG) + " or newer");

        String criticalUrl = map.get("critical_url");
        if (criticalUrl != null && !criticalUrl.trim().isEmpty()) {
            JPanel panel = new JPanel(new BorderLayout());
            panel.add(new JLabel("WARNING! CRITICAL ISSUE! Are you sure you want to run rusEfi?"), BorderLayout.NORTH);
            panel.add(new URLLabel(criticalUrl, criticalUrl), BorderLayout.CENTER);
            JOptionPane.showMessageDialog(getPaneParent(), panel);
        }
    }

    private int parseNotNull(String value, String reason) throws IOException {
        if (value == null)
            throw new IOException("Unexpected file format");
        return Utils.parseIntWithReason(value, reason);
    }

    private static void showUpdateWarningIfNeeded(final String componentName, final Integer latestVersion, final int currentVersion) {
        if (latestVersion == null || currentVersion >= latestVersion)
            return;
        if (getConfig().getRoot().getProperty(componentName).equals(Integer.toString(latestVersion)))
            return; // warning was suppressed
        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                JPanel panel = new JPanel(new BorderLayout());
                String message = "<html>It's time to update " + componentName + "!<br>" +
                        "Your version: " + currentVersion + "<br>" +
                        "Latest version: " + latestVersion;
                panel.add(new JLabel(message), BorderLayout.NORTH);
                JCheckBox doNotShowForThisVersion = new JCheckBox("Do not show for this version");
                panel.add(doNotShowForThisVersion, BorderLayout.CENTER);
                JOptionPane.showMessageDialog(getPaneParent(), panel, "Update", JOptionPane.WARNING_MESSAGE);
                if (doNotShowForThisVersion.isSelected())
                    getConfig().getRoot().setProperty(componentName, latestVersion);
            }
        });
    }

    private static Component getPaneParent() {
        return JFrame.getFrames()[0];
    }

    public static VersionChecker getInstance() {
        return instance;
    }

    public void onFirmwareVersion(String firmwareString) {
        String suggestedFirmware = map.get(FIRMWARE_TAG);
        if (suggestedFirmware == null) {
            // no version file yet? nothing to bother about
            return;
        }
        int latestVersion = Utils.parseIntWithReason(suggestedFirmware, "VC value");
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
