package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageTestHelper;
import com.opensr5.ini.IniFileMetaInfoImpl;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.RawIniFile;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.tune.xml.MsqFactory;
import com.rusefi.util.RestorePointStore;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import javax.swing.*;
import java.awt.GraphicsEnvironment;
import java.io.ByteArrayInputStream;
import java.nio.charset.StandardCharsets;
import java.nio.file.Path;
import java.time.Clock;
import java.util.concurrent.atomic.AtomicReference;

import static org.junit.jupiter.api.Assertions.*;
import static org.junit.jupiter.api.Assumptions.assumeFalse;

/** [tag:restore_points] The Restore Points window and what restoring a point would change. */
class RestorePointsWindowTest {
    static final String SIGNATURE = "rusEFI master.2026.11.20.uaefi.123";
    static final String INI = "[Constants]\n"
        + "signature = \"" + SIGNATURE + "\"\n"
        + "nPages = 1\npageSize = 12\npageReadCommand = \"r\"\nochBlockSize = 0\npage = 1\n"
        + "injectorFlow = scalar, F32, 0, \"cc/min\", 1, 0, 0, 2000, 1\n"
        + "enableA = bits, U32, 4, [0:0], \"Disabled\", \"Enabled\"\n"
        + "fuelTable = array, U08, 8, [2x2], \"ms\", 1, 0, 0, 255, 0\n"
        + "fuelTableCopy = array, U08, 8, [2x2], \"ms\", 1, 0, 0, 255, 0\n"
        + "[Menu]\nmenu = \"Fuel\"\nsubMenu = injection, \"Injection\"\n"
        + "[UserDefined]\ndialog = injection, \"Injection\"\n"
        + "field = \"Injector flow\", injectorFlow\n"
        + "field = \"Enable A\", enableA\n";

    @TempDir
    Path dir;
    private RestorePointsWindow window;

    @AfterEach
    void close() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            if (window != null) {
                window.getDialogForTests().dispose();
            }
        });
    }

    static IniFileModel ini() {
        try {
            RawIniFile raw = IniFileReaderUtil.read(new ByteArrayInputStream(INI.getBytes(StandardCharsets.UTF_8)));
            return IniFileReaderUtil.readIniFile(raw, "restore-window-test.ini", new IniFileMetaInfoImpl(raw));
        } catch (Throwable e) {
            throw new AssertionError("test .ini", e);
        }
    }

    /** Flow 440 cc/min, A disabled, an empty table. */
    static ConfigurationImage openTune() {
        ConfigurationImage image = ConfigurationImageTestHelper.newLittleEndian(new byte[12]);
        image.getByteBuffer(0, 4).putFloat(440);
        return image;
    }

    /** Flow 470, A enabled, one table cell at 5. */
    static ConfigurationImage olderTune() {
        ConfigurationImage image = openTune();
        image.getByteBuffer(0, 4).putFloat(470);
        image.getContent()[4] = 1;
        image.getContent()[8] = 5;
        return image;
    }

    @Test
    void diffListsEachChangedSettingOnce() {
        IniFileModel ini = ini();
        RestorePointDiff diff = RestorePointDiff.of(MsqFactory.valueOf(olderTune(), ini), openTune(), ini);
        assertEquals(3, diff.changedValues, "flow, A and one cell");
        assertEquals(3, diff.rows.size(), "the table and its copy are one row: " + diff.rows);
        assertEquals("Injector flow", diff.rows.get(0).label);
        assertTrue(diff.rows.get(0).then.startsWith("470"), diff.rows.get(0).then);
        assertTrue(diff.rows.get(0).now.startsWith("440"), diff.rows.get(0).now);
        assertEquals("Enable A", diff.rows.get(1).label);
        assertEquals("Enabled", diff.rows.get(1).then);
        assertEquals("Disabled", diff.rows.get(1).now);
        assertEquals("fuelTable (1 cell)", diff.rows.get(2).label, "no page shows it: its .ini name");
        assertArrayEquals(olderTune().getContent(), diff.restored.getContent());
    }

    @Test
    void listsPointsWithWhyAndDifferencesAndRestoresTheSelectedOne() throws Exception {
        assumeFalse(GraphicsEnvironment.isHeadless(), "The window is a JDialog: requires a desktop or Xvfb");
        IniFileModel ini = ini();
        RestorePointStore store = new RestorePointStore(dir, 50, Clock.systemUTC());
        store.saveAlways(MsqFactory.valueOf(olderTune(), ini), RestorePointStore.Reason.MANUAL, "before idle work");
        AtomicReference<ConfigurationImage> restored = new AtomicReference<>();
        SwingUtilities.invokeAndWait(() -> {
            window = new RestorePointsWindow(null, store, () -> ini, RestorePointsWindowTest::openTune,
                image -> {
                    restored.set(image);
                    return true;
                }, new AbstractAction("Create") {
                    @Override
                    public void actionPerformed(java.awt.event.ActionEvent e) {
                    }
                });
            window.setDetailWorkForTests(Runnable::run);
            window.reload();

            assertEquals(1, window.pointsTable.getRowCount());
            assertEquals("Manual: before idle work", window.pointsTable.getValueAt(0, 1));
            assertEquals("3", window.pointsTable.getValueAt(0, 2), "values that differ from the open tune");
            assertEquals(0, window.pointsTable.getSelectedRow(), "the newest is selected");
            assertTrue(window.titleLabel.getText().startsWith("Manual · "), window.titleLabel.getText());
            assertTrue(window.descriptionLabel.getText().contains("3 values differ"), window.descriptionLabel.getText());
            assertEquals(3, window.diffTable.getRowCount());
            assertTrue(window.restoreButton.isEnabled());
            assertTrue(window.getDialogForTests().getTitle().endsWith("uaefi"));

            window.restoreButton.doClick();
            assertArrayEquals(olderTune().getContent(), restored.get().getContent());
            assertFalse(window.getDialogForTests().isDisplayable(), "the window closes once restored");
        });
    }

    @Test
    void theListFollowsTheStore() throws Exception {
        assumeFalse(GraphicsEnvironment.isHeadless(), "The window is a JDialog: requires a desktop or Xvfb");
        IniFileModel ini = ini();
        RestorePointStore store = new RestorePointStore(dir, 50, Clock.systemUTC());
        SwingUtilities.invokeAndWait(() -> {
            window = new RestorePointsWindow(null, store, () -> ini, RestorePointsWindowTest::openTune,
                image -> true, new AbstractAction("Create") {
                    @Override
                    public void actionPerformed(java.awt.event.ActionEvent e) {
                    }
                });
            window.setDetailWorkForTests(Runnable::run);
            assertEquals(0, window.pointsTable.getRowCount());
            assertTrue(window.emptyLabel.isVisible(), "says how points are made");
            assertFalse(window.restoreButton.isEnabled());
        });
        // e.g. a Burn while the window is open
        store.save(MsqFactory.valueOf(olderTune(), ini), RestorePointStore.Reason.BEFORE_BURN, null);
        RestorePointStore.RestorePoint saved = store.list(SIGNATURE).get(0);
        SwingUtilities.invokeAndWait(() -> { });
        SwingUtilities.invokeAndWait(() -> {
            assertEquals(1, window.pointsTable.getRowCount());
            assertEquals("Before burn", window.pointsTable.getValueAt(0, 1));
            assertFalse(window.emptyLabel.isVisible());
            window.deletePoint(saved);
        });
        SwingUtilities.invokeAndWait(() -> assertEquals(0, window.pointsTable.getRowCount(), "deleted"));
    }

    @Test
    void aPointLikeTheOpenTuneHasNothingToRestore() throws Exception {
        assumeFalse(GraphicsEnvironment.isHeadless(), "The window is a JDialog: requires a desktop or Xvfb");
        IniFileModel ini = ini();
        RestorePointStore store = new RestorePointStore(dir, 50, Clock.systemUTC());
        store.save(MsqFactory.valueOf(openTune(), ini), RestorePointStore.Reason.BEFORE_LOAD, null);
        SwingUtilities.invokeAndWait(() -> {
            window = new RestorePointsWindow(null, store, () -> ini, RestorePointsWindowTest::openTune,
                image -> true, new AbstractAction("Create") {
                    @Override
                    public void actionPerformed(java.awt.event.ActionEvent e) {
                    }
                });
            window.setDetailWorkForTests(Runnable::run);
            window.reload();
            assertEquals("0", window.pointsTable.getValueAt(0, 2));
            assertTrue(window.descriptionLabel.getText().endsWith("It is the same as the tune open now."),
                window.descriptionLabel.getText());
            assertFalse(window.restoreButton.isEnabled());
        });
    }
}
