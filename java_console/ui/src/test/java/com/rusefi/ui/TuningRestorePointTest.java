package com.rusefi.ui;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageTestHelper;
import com.opensr5.ini.IniFileMetaInfoImpl;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.RawIniFile;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.tune.xml.MsqFactory;
import com.rusefi.util.RestorePointStore;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import javax.swing.*;
import java.awt.*;
import java.io.ByteArrayInputStream;
import java.nio.charset.StandardCharsets;
import java.nio.file.Path;
import java.time.Clock;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicReference;

import static org.junit.jupiter.api.Assertions.*;

/** [tag:restore_points] Restore as changes is one edit: unburned, undoable. */
class TuningRestorePointTest {
    private static final String INI = "[Constants]\n"
        + "signature = \"restore-test\"\n"
        + "nPages = 2\npageSize = 8, 32\npageReadCommand = \"r\", \"r\"\n"
        + "pageIdentifier = \"\\x00\\x00\", \"\\x00\\x04\"\nochBlockSize = 0\npage = 1\n"
        + "injectorFlow = scalar, F32, 0, \"cc/min\", 1, 0, 0, 2000, 1\n"
        + "trimValue = scalar, F32, 4, \"\", 1, 0, 0, 2000, 2\n"
        + "page = 2\nluaScript = string, ASCII, 0, 32\n"
        + "[Menu]\nmenu = \"Fuel\"\nsubMenu = injection, \"Injection\"\n"
        + "[UserDefined]\ndialog = injection, \"Injection\"\n"
        + "field = \"Injector flow\", injectorFlow\n";

    @Test
    void restoringReviewCancelsSafelyAndAppliesOneUndoableUnburnedEdit(@TempDir Path directory) throws Throwable {
        RawIniFile raw = IniFileReaderUtil.read(new ByteArrayInputStream(INI.getBytes(StandardCharsets.UTF_8)));
        IniFileModel ini = IniFileReaderUtil.readIniFile(raw, "restore-test.ini", new IniFileMetaInfoImpl(raw));
        UIContext context = new UIContext();
        IniFileModel replacementIni = IniFileReaderUtil.readIniFile(raw, "refreshed.ini", new IniFileMetaInfoImpl(raw));
        RestorePointStore store = new RestorePointStore(directory, 50, Clock.systemUTC());
        ConfigurationImage target = flow(470);
        target.getByteBuffer(4, 4).putFloat(0.12f);
        RestorePointStore.RestorePoint legacy = store.saveAlways(MsqFactory.valueOf(target, ini),
            RestorePointStore.Reason.MANUAL, "Legacy point");
        RestorePointStore.RestorePoint point = store.saveAlways(MsqFactory.valueOf(Map.of(0, target, 0x0400, lua("return 2")), ini),
            RestorePointStore.Reason.MANUAL, "Test point");
        AtomicReference<TuningPane> tuning = new AtomicReference<>();
        ArrayBlockingQueue<JComponent> reviews = new ArrayBlockingQueue<>(1);
        SwingUtilities.invokeAndWait(() -> {
            context.setOfflineMode(true);
            context.setIniFileModelForOffline(ini);
            TuningPane pane = new TuningPane(context);
            tuning.set(pane);
            ConfigurationImage original = flow(440);
            original.getByteBuffer(4, 4).putFloat(0.1234f);
            pane.seedOfflineImage(original, "injection");
            context.setWorkingSecondaryTunePages(Map.of(0x0400, lua("return 1")));
            context.setTuneFilePath("working.msq");
            pane.setTuneReviewHost(card -> {
                reviews.add(card);
                return () -> { };
            });
        });
        TuningPane pane = tuning.get();
        try {
            for (int attempt = 0; attempt < 5; attempt++) {
                boolean cancel = attempt == 0, stale = attempt == 1, mainOnly = attempt == 2, skipLua = attempt == 3;
                CountDownLatch finished = new CountDownLatch(1);
                SwingUtilities.invokeAndWait(() -> {
                    pane.getLoadTuneAction().addPropertyChangeListener(e -> {
                        if ("enabled".equals(e.getPropertyName()) && Boolean.TRUE.equals(e.getNewValue())) {
                            finished.countDown();
                        }
                    });
                    assertTrue(pane.reviewRestorePoint(mainOnly ? legacy : point));
                });
                JComponent review = reviews.poll(10, TimeUnit.SECONDS);
                assertNotNull(review);
                SwingUtilities.invokeAndWait(() -> {
                    assertEquals(440, pane.getTuneImage().getByteBuffer(0, 4).getFloat(), 1e-6, "no edit before consent");
                    assertTrue(all(review).stream().noneMatch(c -> c instanceof JButton
                        && "Apply and Burn".equals(((JButton) c).getText())));
                    if (stale) {
                        context.setIniFileModelForOffline(replacementIni);
                    }
                    if (skipLua) {
                        JTable changes = all(review).stream().filter(c -> c instanceof JTable)
                            .map(c -> (JTable) c).filter(t -> t.getColumnClass(0) == Boolean.class)
                            .findFirst().orElseThrow(AssertionError::new);
                        assertEquals(2, changes.getRowCount(), "main setting and Lua, not the rounding-only field");
                        int luaRow = "luaScript".equals(changes.getValueAt(0, 1)) ? 0 : 1;
                        assertEquals("luaScript", changes.getValueAt(luaRow, 1));
                        changes.setValueAt(false, luaRow, 0);
                    }
                    button(review, cancel ? "Cancel" : "Restore selected as changes").doClick(0);
                });
                assertTrue(finished.await(10, TimeUnit.SECONDS));
                SwingUtilities.invokeAndWait(() -> {
                    boolean applied = !cancel && !stale;
                    assertEquals(applied ? 470 : 440, pane.getTuneImage().getByteBuffer(0, 4).getFloat(), 1e-6);
                    assertEquals(0.1234f, pane.getTuneImage().getByteBuffer(4, 4).getFloat(), 0f, "do not apply only rounding");
                    assertEquals("working.msq", context.getTuneFilePath());
                    assertArrayEquals(lua(applied && !mainOnly && !skipLua ? "return 2" : "return 1").getContent(),
                        context.getWorkingSecondaryTunePages().get(0x0400).getContent());
                    assertEquals(applied, button(pane.getContent(), "Undo").isEnabled());
                    if (applied) {
                        assertEquals(java.util.Collections.singleton("injection"), pane.getUnburnedPageKeysForTests());
                        button(pane.getContent(), "Undo").doClick(0);
                        assertEquals(440, pane.getTuneImage().getByteBuffer(0, 4).getFloat(), 1e-6, "one undo step");
                        assertFalse(button(pane.getContent(), "Undo").isEnabled());
                        assertArrayEquals(lua("return 1").getContent(),
                            context.getWorkingSecondaryTunePages().get(0x0400).getContent());
                    }
                });
            }
        } finally {
            SwingUtilities.invokeAndWait(pane::destroy);
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

    private static ConfigurationImage lua(String script) {
        byte[] bytes = new byte[32];
        byte[] text = script.getBytes(StandardCharsets.US_ASCII);
        System.arraycopy(text, 0, bytes, 0, text.length);
        return ConfigurationImageTestHelper.newLittleEndian(bytes);
    }

    private static JButton button(Container parent, String text) {
        for (Component c : all(parent)) {
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
