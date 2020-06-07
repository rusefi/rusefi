package com.rusefi;

import com.rusefi.tools.ConsoleTools;
import com.rusefi.ui.engine.EngineSnifferPanel;
import com.rusefi.ui.storage.PersistentConfiguration;

import java.util.Date;

/**
 * this is the main entry point of rusEfi ECU console
 * <p/>
 * <p/>
 * 12/25/12
 * (c) Andrey Belomutskiy 2013-2019
 *
 * @see StartupFrame
 * @see EngineSnifferPanel
 */
public class Launcher extends rusEFIVersion {
    public static final String INPUT_FILES_PATH = System.getProperty("input_files_path", "..");
    public static final String TOOLS_PATH = System.getProperty("tools_path", ".");

    /**
     * rusEfi console entry point
     *
     * @see StartupFrame if no parameters specified
     */
    public static void main(final String[] args) throws Exception {
        System.out.println("rusEfi UI console " + CONSOLE_VERSION);
        System.out.println("Compiled " + new Date(ConsoleTools.classBuildTimeMillis()));
        System.out.println("\n\n");
        PersistentConfiguration.registerShutdownHook();

        if (ConsoleTools.runTool(args)) {
            return;
        }

        ConsoleTools.printTools();

        ConsoleUI.startUi(args);
    }
}
