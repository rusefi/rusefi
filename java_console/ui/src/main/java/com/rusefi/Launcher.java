package com.rusefi;

import com.devexperts.logging.Logging;
import com.rusefi.core.rusEFIVersion;
import com.rusefi.tools.ConsoleTools;
import com.rusefi.ui.engine.EngineSnifferPanel;
import com.rusefi.core.preferences.storage.PersistentConfiguration;

import java.util.Date;

import static com.devexperts.logging.Logging.getLogging;

/**
 * this is the main entry point of rusEfi ECU console
 * <p/>
 * <p/>
 * 12/25/12
 * Andrey Belomutskiy, (c) 2013-2020
 *
 * @see StartupFrame
 * @see EngineSnifferPanel
 */
public class Launcher extends rusEFIVersion {
    private static final Logging log = getLogging(Launcher.class);
    public static final String INPUT_FILES_PATH = System.getProperty("input_files_path", "..");
    public static final String TOOLS_PATH = System.getProperty("tools_path", ".");

    /**
     * rusEfi console entry point
     *
     * @see StartupFrame if no parameters specified
     */
    public static void main(final String[] args) throws Exception {
        log.info("rusEFI UI console " + CONSOLE_VERSION);
        log.info("Compiled " + new Date(rusEFIVersion.classBuildTimeMillis()));
        log.info("\n\n");
        PersistentConfiguration.registerShutdownHook();

        if (ConsoleTools.runTool(args)) {
            return;
        }

        ConsoleTools.printTools();

        ConsoleUI.startUi(args);
    }
}
