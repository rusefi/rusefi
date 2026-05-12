package com.rusefi.tools;

import com.devexperts.logging.Logging;
import com.rusefi.FileLog;
import com.rusefi.NamedThreadFactory;

import com.rusefi.StartupFrame;
import com.rusefi.core.preferences.storage.PersistentConfiguration;
import com.rusefi.ui.TunerStudioPanel;

import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;
import java.awt.*;
import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Arrays;
import java.util.List;

/**
 * see TsHelper
 */
public class TunerStudioHelper {
    private static final Logging log = Logging.getLogging(TunerStudioHelper.class);
    private static final String FIND_TS_PROCESS = "Get-Process | Where-Object {$_.MainWindowTitle -like \\\"*TunerStudio*\\\"}";

    public static final String TS_EXECUTABLE_PATH = "tunerstudio_executable_path";

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

    /**
     * Launches TunerStudio as a detached process. Returns true if the process was started.
     * Resolves the executable from a saved preference, then from common install paths, then by
     * prompting the user. A user-picked path is persisted for next time.
     */
    public static boolean launchTunerStudio(Component parent) {
        File exe = resolveTunerStudioExecutable(parent);
        if (exe == null) return false;
        try {
            ProcessBuilder pb = new ProcessBuilder(exe.getAbsolutePath());
            pb.directory(exe.getParentFile());
            pb.redirectErrorStream(true);
            pb.redirectOutput(ProcessBuilder.Redirect.DISCARD);
            pb.start();
            return true;
        } catch (IOException e) {
            log.error("Failed to launch TunerStudio at " + exe, e);
            JOptionPane.showMessageDialog(parent,
                "Failed to launch TunerStudio:\n" + e.getMessage(),
                "TunerStudio", JOptionPane.ERROR_MESSAGE);
            return false;
        }
    }

    private static File resolveTunerStudioExecutable(Component parent) {
        String saved = PersistentConfiguration.getConfig().getRoot().getProperty(TS_EXECUTABLE_PATH, null);
        if (saved != null && !saved.isEmpty()) {
            File f = new File(saved);
            if (f.isFile()) return f;
        }

        for (String candidate : defaultTsCandidatePaths()) {
            File f = new File(candidate);
            if (f.isFile()) {
                PersistentConfiguration.getConfig().getRoot().setProperty(TS_EXECUTABLE_PATH, f.getAbsolutePath());
                return f;
            }
        }

        JFileChooser chooser = new JFileChooser();
        chooser.setDialogTitle("Locate TunerStudio executable");
        if (FileLog.isWindows()) {
            chooser.setFileFilter(new FileNameExtensionFilter("Executables (*.exe)", "exe"));
            File pf = new File(System.getenv().getOrDefault("ProgramFiles", "C:\\Program Files"));
            if (pf.isDirectory()) chooser.setCurrentDirectory(pf);
        }
        if (chooser.showOpenDialog(parent) != JFileChooser.APPROVE_OPTION) return null;
        File picked = chooser.getSelectedFile();
        if (picked == null || !picked.isFile()) return null;
        PersistentConfiguration.getConfig().getRoot().setProperty(TS_EXECUTABLE_PATH, picked.getAbsolutePath());
        return picked;
    }

    private static List<String> defaultTsCandidatePaths() {
        String home = System.getProperty("user.home");
        if (FileLog.isWindows()) {
            String pf = System.getenv().getOrDefault("ProgramFiles", "C:\\Program Files");
            String pf86 = System.getenv().getOrDefault("ProgramFiles(x86)", "C:\\Program Files (x86)");
            return Arrays.asList(
                pf + "\\EFIAnalytics\\TunerStudioMS\\TunerStudio.exe",
                pf86 + "\\EFIAnalytics\\TunerStudioMS\\TunerStudio.exe",
                pf + "\\TunerStudioMS\\TunerStudio.exe",
                pf86 + "\\TunerStudioMS\\TunerStudio.exe"
            );
        }
        if (FileLog.isLinux()) {
            return Arrays.asList(
                home + "/TunerStudioMS/TunerStudio.sh",
                home + "/.TunerStudioMS/TunerStudio.sh",
                "/opt/TunerStudioMS/TunerStudio.sh",
                home + "/TunerStudioMS/TunerStudioMS",
                home + "/.TunerStudioMS/TunerStudioMS",
                "/opt/TunerStudioMS/TunerStudioMS"
            );
        }
        // macOS (open works with the .app bundle)
        return Arrays.asList(
            "/Applications/TunerStudioMS.app/Contents/MacOS/TunerStudioMS"
        );
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
