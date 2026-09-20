package com.rusefi.tune;

import com.opensr5.ini.DialogModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.PanelModel;
import com.rusefi.ini.reader.IniFileReaderUtil;
import org.junit.jupiter.api.Test;

import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertTrue;

/**
 * "Controller -> Injector test" menu opens the 'injTest' dialog (secondary_panels.ini).
 * The dialog is expected to expose the per-injector bench-test command buttons
 * (dialog 'testInjectors', cmd_test_inj1..N) next to the iteration/on/off settings.
 */
public class InjectorTestDialogTest {
    private static final String CURRENT_INI = "../../firmware/tunerstudio/generated/rusefi_uaefi.ini";

    @Test
    public void injectorTestDialogHasNoInjectorButtons() throws FileNotFoundException {
        IniFileModel ini = IniFileReaderUtil.readIniFile(CURRENT_INI);
        DialogModel injTest = ini.getDialogs().get("injTest");
        assertNotNull(injTest, "injTest dialog referenced from Controller menu");

        // sanity: the injector button dialog itself exists and does carry the buttons
        DialogModel testInjectors = ini.getDialogs().get("testInjectors");
        assertNotNull(testInjectors);
        assertTrue(collectCommands(ini, testInjectors).contains("cmd_test_inj1"));

        List<String> commands = collectCommands(ini, injTest);
        // TODO: this is the bug - Injector test dialog shows only Iterations/On Time/Off Time
        // and reference gauges, without any injector button (see .ini screenshot in report)
        assertFalse(commands.contains("cmd_test_inj1"), "current (broken) state: no injector #1 button in Injector test dialog, got " + commands);
        assertFalse(commands.contains("cmd_test_cancel"), "current (broken) state: no Abort test button in Injector test dialog, got " + commands);
    }

    /**
     * @return names of all controller commands reachable from the dialog, recursively via panels
     */
    static List<String> collectCommands(IniFileModel ini, DialogModel dialog) {
        List<String> result = new ArrayList<>();
        collectCommands(ini, dialog, result, new HashSet<>());
        return result;
    }

    private static void collectCommands(IniFileModel ini, DialogModel dialog, List<String> result, Set<String> visited) {
        if (dialog == null || !visited.add(dialog.getKey())) {
            return;
        }
        for (DialogModel.Command command : dialog.getCommandsOfCurrentDialog()) {
            result.add(command.getCommand());
        }
        for (PanelModel panel : dialog.getPanels()) {
            collectCommands(ini, panel.resolveDialog(ini), result, visited);
        }
    }
}
