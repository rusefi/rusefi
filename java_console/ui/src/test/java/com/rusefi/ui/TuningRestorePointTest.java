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

/** [tag:restore_points] Restore as changes is one edit: unburned, undoable. */
class TuningRestorePointTest {
    private static final String INI = "[Constants]\n"
        + "signature = \"restore-test\"\n"
        + "nPages = 1\npageSize = 8\npageReadCommand = \"r\"\nochBlockSize = 0\npage = 1\n"
        + "injectorFlow = scalar, F32, 0, \"cc/min\", 1, 0, 0, 2000, 1\n"
        + "[Menu]\nmenu = \"Fuel\"\nsubMenu = injection, \"Injection\"\n"
        + "[UserDefined]\ndialog = injection, \"Injection\"\n"
        + "field = \"Injector flow\", injectorFlow\n";

    @Test
    void restoringIsAnUndoableUnburnedEdit() throws Throwable {
        RawIniFile raw = IniFileReaderUtil.read(new ByteArrayInputStream(INI.getBytes(StandardCharsets.UTF_8)));
        IniFileModel ini = IniFileReaderUtil.readIniFile(raw, "restore-test.ini", new IniFileMetaInfoImpl(raw));
        UIContext context = new UIContext();
        AtomicReference<TuningPane> tuning = new AtomicReference<>();
        AtomicReference<Throwable> failure = new AtomicReference<>();
        SwingUtilities.invokeAndWait(() -> {
            context.setOfflineMode(true);
            context.setIniFileModelForOffline(ini);
            TuningPane pane = new TuningPane(context);
            tuning.set(pane);
            pane.seedOfflineImage(flow(440), "injection");
        });
        SwingUtilities.invokeAndWait(() -> {
            try {
                TuningPane pane = tuning.get();
                assertTrue(pane.restoreTune(flow(470)));
                assertEquals(470, pane.getTuneImage().getByteBuffer(0, 4).getFloat(), 1e-6);
                assertTrue(labels(pane).contains("Restore point put back as unburned changes"));
                assertEquals(java.util.Collections.singleton("injection"), pane.getUnburnedPageKeysForTests(),
                    "counted as unburned");

                button(pane, "Undo").doClick();
                assertEquals(440, pane.getTuneImage().getByteBuffer(0, 4).getFloat(), 1e-6, "one undo step");
            } catch (Throwable t) {
                failure.set(t);
            }
        });
        if (failure.get() != null) {
            throw failure.get();
        }
    }

    /** Laid out like the real .ini: Setup › Vehicle Information holds the Engine Metadata panel. */
    private static final String VEHICLE_INI = "[Constants]\n"
        + "signature = \"restore-test\"\n"
        + "nPages = 1\npageSize = 40\npageReadCommand = \"r\"\nochBlockSize = 0\npage = 1\n"
        + "injectorFlow = scalar, F32, 0, \"cc/min\", 1, 0, 0, 2000, 1\n"
        + "vehicleName = string, ASCII, 8, 32\n"
        + "[Menu]\nmenu = \"&Setup\"\nsubMenu = " + TuningPane.VEHICLE_INFORMATION_PAGE + ", \"Vehicle Information\"\n"
        + "[UserDefined]\ndialog = " + TuningPane.ENGINE_METADATA_PAGE + ", \"Engine Metadata\"\n"
        + "field = \"Vehicle Name\", vehicleName\n"
        + "dialog = " + TuningPane.VEHICLE_INFORMATION_PAGE + ", \"Vehicle Information\"\n"
        + "panel = " + TuningPane.ENGINE_METADATA_PAGE + "\n";

    @Test
    void theVehicleInformationPageOpensFromRestorePoints() throws Throwable {
        org.junit.jupiter.api.Assumptions.assumeFalse(GraphicsEnvironment.isHeadless(),
            "Opens a settings window: requires a desktop or Xvfb");
        RawIniFile raw = IniFileReaderUtil.read(new ByteArrayInputStream(VEHICLE_INI.getBytes(StandardCharsets.UTF_8)));
        IniFileModel ini = IniFileReaderUtil.readIniFile(raw, "vehicle-test.ini", new IniFileMetaInfoImpl(raw));
        UIContext context = new UIContext();
        AtomicReference<Throwable> failure = new AtomicReference<>();
        SwingUtilities.invokeAndWait(() -> {
            JFrame owner = new JFrame();
            try {
                context.setOfflineMode(true);
                context.setIniFileModelForOffline(ini);
                TuningPane pane = new TuningPane(context);
                pane.seedOfflineImage(ConfigurationImageTestHelper.newLittleEndian(new byte[40]),
                    TuningPane.VEHICLE_INFORMATION_PAGE);
                assertTrue(pane.openSettingsWindow(owner, TuningPane.VEHICLE_INFORMATION_PAGE,
                    com.rusefi.util.RestorePointStore.VEHICLE_NAME), "the page the note's button opens");
            } catch (Throwable t) {
                failure.set(t);
            } finally {
                for (Window window : owner.getOwnedWindows()) {
                    window.dispose();
                }
                owner.dispose();
            }
        });
        if (failure.get() != null) {
            throw failure.get();
        }
    }

    private static ConfigurationImage flow(float value) {
        ConfigurationImage image = ConfigurationImageTestHelper.newLittleEndian(new byte[8]);
        image.getByteBuffer(0, 4).putFloat(value);
        return image;
    }

    private static List<String> labels(TuningPane pane) {
        List<String> texts = new ArrayList<>();
        for (Component c : all(pane.getContent())) {
            if (c instanceof JLabel) {
                texts.add(((JLabel) c).getText());
            }
        }
        return texts;
    }

    private static JButton button(TuningPane pane, String text) {
        for (Component c : all(pane.getContent())) {
            if (c instanceof JButton && text.equals(((JButton) c).getText())) {
                return (JButton) c;
            }
        }
        throw new AssertionError("no button " + text);
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
}
