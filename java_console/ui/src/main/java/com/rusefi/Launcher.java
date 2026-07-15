package com.rusefi;

import com.devexperts.logging.FileLogger;
import com.devexperts.logging.Logging;
import com.rusefi.autoupdate.Autoupdate;
import com.rusefi.core.rusEFIVersion;
import com.rusefi.tools.ConsoleTools;
import com.rusefi.ui.engine.EngineSnifferPanel;
import com.rusefi.core.preferences.storage.PersistentConfiguration;

import javax.swing.*;
import java.util.Arrays;
import java.util.Date;
import java.util.concurrent.CompletableFuture;

import static com.devexperts.logging.Logging.getLogging;

/**
 * this is the main entry point of rusEfi ECU console
 * <p/>
 * <p/>
 * 12/25/12
 * Andrey Belomutskiy, (c) 2013-2020
 *
 * @see Autoupdate#COM_RUSEFI_LAUNCHER which invokes this class via reflection!
 * @see StartupFrame
 * @see EngineSnifferPanel
 */
public class Launcher implements rusEFIVersion {
    private static final Logging log = getLogging(Launcher.class);
    public static final String TOOLS_PATH = System.getProperty("tools_path", ".");

    /**
     * rusEfi console entry point
     *
     * @see StartupFrame if no parameters specified
     */
    public static void main(final String[] args) throws Exception {
        if (!JavaVersionHelper.isAtLeastJava11()) {
            String version = System.getProperty("java.version");
            String message = "Java 11 or newer is required to run rusEFI console\n" +
                    "Your current version is " + version;
            JOptionPane.showMessageDialog(null, message, "Error", JOptionPane.ERROR_MESSAGE);
            System.err.println(message);
            System.exit(-1);
        }
        FileLogger.init();
        log.info("rusEFI UI console " + UiVersion.CONSOLE_VERSION);
        log.info("Compiled " + new Date(rusEFIVersion.classBuildTimeMillis()));
        log.info("\n\n");
        PersistentConfiguration.registerShutdownHook();
        com.rusefi.maintenance.ManualIniFilePicker.register();

        // If this process was launched from rusefi_console_pending.jar, copy it over
        // rusefi_console.jar now that the previous instance has fully exited.
        Autoupdate.finalizePendingUpdate();

        // Older rusefi_updater.exe launchers pass "basic-ui" — that mode is gone, fall through to the full UI.
        String[] effectiveArgs = (args.length > 0 && "basic-ui".equalsIgnoreCase(args[0]))
            ? Arrays.copyOfRange(args, 1, args.length) // we want the rest of the args
            : args; // or all the args if we don't have basic-ui as the first one

        if (ConsoleTools.runTool(effectiveArgs)) {
            return;
        }

        ConsoleTools.printTools();

        CompletableFuture<Autoupdate.UpdateOutcome> updateOutcome = new CompletableFuture<>();
        Thread updateThread = new Thread(() -> updateOutcome.complete(Autoupdate.runSilentUpdate()),
            "autoupdate-background");
        updateThread.setDaemon(true);
        updateThread.start();

        ConsoleUI.startUi(effectiveArgs, updateOutcome);
    }
}
