package com.rusefi;

import com.devexperts.logging.FileLogger;
import com.devexperts.logging.Logging;
import com.rusefi.autoupdate.Autoupdate;
import com.rusefi.core.rusEFIVersion;
import com.rusefi.tools.ConsoleTools;
import com.rusefi.ui.engine.EngineSnifferPanel;
import com.rusefi.core.preferences.storage.PersistentConfiguration;

import javax.swing.*;
import java.util.Date;
import java.util.concurrent.atomic.AtomicReference;
import java.util.function.Consumer;

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

        // If this process was launched from rusefi_console_pending.jar, copy it over
        // rusefi_console.jar now that the previous instance has fully exited.
        Autoupdate.finalizePendingUpdate();

        if (ConsoleTools.runTool(args)) {
            return;
        }

        ConsoleTools.printTools();

        AtomicReference<Consumer<String>> bannerCallback = new AtomicReference<>();
        Thread updateThread = new Thread(() ->
            Autoupdate.runSilentUpdate(msg -> {
                Consumer<String> cb = bannerCallback.get();
                if (cb != null && msg != null)
                    SwingUtilities.invokeLater(() -> cb.accept(msg));
            }), "autoupdate-background");
        updateThread.setDaemon(true);
        updateThread.start();

        ConsoleUI.startUi(args, bannerCallback);
    }
}
