package com.rusefi.ui;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageTestHelper;
import com.opensr5.ini.IniFileMetaInfoImpl;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.RawIniFile;
import com.rusefi.ini.reader.IniFileReaderUtil;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.io.ByteArrayInputStream;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.atomic.AtomicReference;

import static org.junit.jupiter.api.Assertions.*;

/**
 * Offline, a menu page gated on a tune setting (VE table on isInjectionEnabled) follows edits to that setting.
 */
public class TuningOfflineMenuConditionTest {
    private static final String INI = "[Constants]\n"
        + "signature = \"offline-menu-test\"\n"
        + "nPages = 1\npageSize = 8\npageReadCommand = \"r\"\nochBlockSize = 0\npage = 1\n"
        + "isInjectionEnabled = bits, U32, 0, [0:0], \"disabled\", \"enabled\"\n"
        + "veValue = scalar, F32, 4, \"%\", 1, 0, 0, 200, 1\n"
        + "[Menu]\nmenu = \"Fuel\"\n"
        + "subMenu = injectorConfig, \"Injection configuration\"\n"
        + "subMenu = veTableDialog, \"VE\", 0, {isInjectionEnabled == 1}\n"
        + "[UserDefined]\n"
        + "dialog = injectorConfig, \"Injection configuration\"\n"
        + "field = \"Injection\", isInjectionEnabled\n"
        + "dialog = veTableDialog, \"VE\"\n"
        + "field = \"VE\", veValue\n";

    @Test
    void tuneLoadedOfflineFollowsMenuConditions() throws Throwable {
        AtomicReference<TuningPane> tuning = loadOfflineTuneWithInjectionOff();
        AtomicReference<Throwable> failure = new AtomicReference<>();
        SwingUtilities.invokeAndWait(() -> {
            try {
                // the INI gates the VE page on isInjectionEnabled == 1
                assertTrue(tuning.get().isMenuPageDisabledForTests("veTableDialog"),
                    "a tune loaded offline follows the INI menu conditions");
            } catch (Throwable t) {
                failure.set(t);
            }
        });
        if (failure.get() != null) {
            throw failure.get();
        }
    }

    @Test
    void enablingInjectionOfflineUnlocksVePage() throws Throwable {
        AtomicReference<TuningPane> tuning = loadOfflineTuneWithInjectionOff();
        AtomicReference<Throwable> failure = new AtomicReference<>();
        SwingUtilities.invokeAndWait(() -> {
            try {
                TuningPane pane = tuning.get();
                JCheckBox injection = checkbox(pane, "isInjectionEnabled");
                injection.doClick();
                injection.doClick();
                assertTrue(pane.isMenuPageDisabledForTests("veTableDialog"),
                    "injection turned off offline greys out the VE page");

                injection.doClick();
                assertFalse(pane.isMenuPageDisabledForTests("veTableDialog"),
                    "injection re-enabled offline unlocks the VE page");
            } catch (Throwable t) {
                failure.set(t);
            }
        });
        if (failure.get() != null) {
            throw failure.get();
        }
    }

    private static AtomicReference<TuningPane> loadOfflineTuneWithInjectionOff() throws Throwable {
        IniFileModel ini = readIni();
        UIContext context = new UIContext();
        AtomicReference<TuningPane> tuning = new AtomicReference<>();
        SwingUtilities.invokeAndWait(() -> {
            context.setOfflineMode(true);
            context.setIniFileModelForOffline(ini);
            TuningPane pane = new TuningPane(context);
            tuning.set(pane);
            // all zero: injection off
            pane.seedOfflineImage(ConfigurationImageTestHelper.newLittleEndian(new byte[8]), "injectorConfig");
        });
        return tuning;
    }

    private static JCheckBox checkbox(TuningPane pane, String fieldKey) {
        for (Component c : all(pane.getDialogForTests().getContentPane())) {
            if (c instanceof JComponent && fieldKey.equals(((JComponent) c).getClientProperty("rusefi.fieldKey"))) {
                for (Component child : all((Container) c)) {
                    if (child instanceof JCheckBox) {
                        return (JCheckBox) child;
                    }
                }
            }
        }
        throw new AssertionError("no checkbox for " + fieldKey);
    }

    private static List<Component> all(Container parent) {
        List<Component> result = new ArrayList<>();
        for (Component c : parent.getComponents()) {
            result.add(c);
            if (c instanceof Container) {
                result.addAll(all((Container) c));
            }
        }
        return result;
    }

    private static IniFileModel readIni() throws Throwable {
        RawIniFile raw = IniFileReaderUtil.read(new ByteArrayInputStream(INI.getBytes(StandardCharsets.UTF_8)));
        return IniFileReaderUtil.readIniFile(raw, "offline-menu-test.ini", new IniFileMetaInfoImpl(raw));
    }
}
