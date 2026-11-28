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
import java.util.List;
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
    void readsTheVehicleNameThatNamesTheCarFolder() throws Throwable {
        String ini = "[Constants]\nsignature = \"" + SIGNATURE + "\"\n"
            + "nPages = 1\npageSize = 12\npageReadCommand = \"r\"\nochBlockSize = 0\npage = 1\n"
            + "vehicleName = string, ASCII, 2, 8\n";
        RawIniFile raw = IniFileReaderUtil.read(new ByteArrayInputStream(ini.getBytes(StandardCharsets.UTF_8)));
        IniFileModel model = IniFileReaderUtil.readIniFile(raw, "vehicle-name-test.ini", new IniFileMetaInfoImpl(raw));
        ConfigurationImage image = ConfigurationImageTestHelper.newLittleEndian(new byte[12]);
        assertEquals("", RestorePointsWindow.vehicleNameOf(image, model), "blank: the car goes by its ECU serial");

        byte[] name = "Miata\0xy".getBytes(StandardCharsets.US_ASCII);
        System.arraycopy(name, 0, image.getContent(), 2, name.length);
        assertEquals("Miata", RestorePointsWindow.vehicleNameOf(image, model), "up to the terminating zero");
        assertEquals("", RestorePointsWindow.vehicleNameOf(image, ini()), "no such field in this .ini");
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

    /**
     * [tag:restore_point_rounding] A restore point keeps each value at the decimals its setting shows (the .msq
     * digits), so the open tune's own value, e.g. 689.4757 shown as 689.48, comes back rounded: no change, and
     * Restore as changes keeps the finer value.
     */
    @Test
    void aValueTheRestorePointOnlyRoundedIsNoChange() throws Throwable {
        String text = "[Constants]\nsignature = \"" + SIGNATURE + "\"\n"
            + "nPages = 1\npageSize = 8\npageReadCommand = \"r\"\nochBlockSize = 0\npage = 1\n"
            + "highPressure = scalar, F32, 0, \"kPa\", 1, 0, 0, 1000, 2\n"
            + "lowValue = scalar, F32, 4, \"V\", 1, 0, 0, 5, 1\n";
        RawIniFile raw = IniFileReaderUtil.read(new ByteArrayInputStream(text.getBytes(StandardCharsets.UTF_8)));
        IniFileModel ini = IniFileReaderUtil.readIniFile(raw, "rounding-test.ini", new IniFileMetaInfoImpl(raw));
        ConfigurationImage open = ConfigurationImageTestHelper.newLittleEndian(new byte[8]);
        // 100 psi in kPa, and a value that does fit one decimal
        open.getByteBuffer(0, 4).putFloat(689.4757f);
        open.getByteBuffer(4, 4).putFloat(0.5f);

        // a restore point of the very tune that is open
        RestorePointDiff diff = RestorePointDiff.of(MsqFactory.valueOf(open, ini), open, ini);

        assertEquals(0, diff.changedValues, "rounded only: no change");
        assertEquals(List.of(), diff.rows);
        assertEquals(689.4757f, diff.restored.getByteBuffer(0, 4).getFloat(), 0f, "restoring keeps the finer value");

        // a change that shows at the setting's decimals is one
        ConfigurationImage older = open.clone();
        older.getByteBuffer(0, 4).putFloat(700.12f);
        RestorePointDiff real = RestorePointDiff.of(MsqFactory.valueOf(older, ini), open, ini);
        assertEquals(1, real.changedValues);
        assertEquals("700.12", real.rows.get(0).then);
        assertEquals("689.48", real.rows.get(0).now);
        assertEquals(700.12f, real.restored.getByteBuffer(0, 4).getFloat(), 1e-6f);
        assertEquals(0.5f, real.restored.getByteBuffer(4, 4).getFloat(), 0f, "the rest kept as it is");
    }

    @Test
    void aTableCellTheRestorePointOnlyRoundedIsNoChange() throws Throwable {
        String text = "[Constants]\nsignature = \"" + SIGNATURE + "\"\n"
            + "nPages = 1\npageSize = 8\npageReadCommand = \"r\"\nochBlockSize = 0\npage = 1\n"
            + "bins = array, F32, 0, [2], \"kPa\", 1, 0, 0, 1000, 1\n";
        RawIniFile raw = IniFileReaderUtil.read(new ByteArrayInputStream(text.getBytes(StandardCharsets.UTF_8)));
        IniFileModel ini = IniFileReaderUtil.readIniFile(raw, "rounding-table.ini", new IniFileMetaInfoImpl(raw));
        ConfigurationImage open = ConfigurationImageTestHelper.newLittleEndian(new byte[8]);
        open.getByteBuffer(0, 4).putFloat(23.888f);
        open.getByteBuffer(4, 4).putFloat(50f);
        ConfigurationImage older = open.clone();
        older.getByteBuffer(4, 4).putFloat(60f);

        RestorePointDiff diff = RestorePointDiff.of(MsqFactory.valueOf(older, ini), open, ini);

        assertEquals(1, diff.changedValues, "the 50 -> 60 cell only, not the rounded 23.9");
        assertEquals("bins (1 cell)", diff.rows.get(0).label);
        assertEquals(23.888f, diff.restored.getByteBuffer(0, 4).getFloat(), 0f);
        assertEquals(60f, diff.restored.getByteBuffer(4, 4).getFloat(), 0f);
    }

    @Test
    void listsPointsWithWhyAndDifferencesAndRestoresTheSelectedOne() throws Exception {
        assumeFalse(GraphicsEnvironment.isHeadless(), "The window is a JDialog: requires a desktop or Xvfb");
        IniFileModel ini = ini();
        RestorePointStore store = new RestorePointStore(dir, 50, Clock.systemUTC());
        RestorePointStore.RestorePoint point = store.saveAlways(MsqFactory.valueOf(olderTune(), ini),
            RestorePointStore.Reason.MANUAL, "before idle work");
        AtomicReference<RestorePointStore.RestorePoint> restored = new AtomicReference<>();
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
            // no Vehicle Name in this .ini and no serial: the open car, in the board folder
            assertEquals("This car", window.carTitle.getText());
            assertEquals("uaefi · no Vehicle Name set · 1 restore point · the newest 50 are kept",
                window.carSubtitle.getText());
            assertTrue(window.unnamedBanner.getParent().isVisible(), "says how to tell cars apart");
            assertFalse(window.carBox.isVisible(), "one car: nothing to pick");

            window.restoreButton.doClick();
            assertEquals(point.file, restored.get().file);
            assertFalse(window.getDialogForTests().isDisplayable(), "the browser closes so the review can take focus");
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
            assertTrue(window.emptyPanel.isVisible(), "says how points are made");
            assertFalse(window.restoreButton.isEnabled());
        });
        // e.g. a Burn while the window is open
        store.save(MsqFactory.valueOf(olderTune(), ini), RestorePointStore.Reason.BEFORE_BURN, null);
        RestorePointStore.RestorePoint saved = store.list(SIGNATURE, "").get(0);
        SwingUtilities.invokeAndWait(() -> { });
        SwingUtilities.invokeAndWait(() -> {
            assertEquals(1, window.pointsTable.getRowCount());
            assertEquals("Before burn", window.pointsTable.getValueAt(0, 1));
            assertFalse(window.emptyPanel.isVisible());
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
            assertTrue(window.descriptionLabel.getText().endsWith("No differences on the main tune page."),
                window.descriptionLabel.getText());
            assertTrue(window.restoreButton.isEnabled(), "the detailed review can still find changes on secondary pages");
        });
    }

    @Test
    void changingTheFolderMovesThePointsWhenAsked() throws Exception {
        assumeFalse(GraphicsEnvironment.isHeadless(), "The window is a JDialog: requires a desktop or Xvfb");
        IniFileModel ini = ini();
        Path local = dir.resolve("local");
        Path synced = dir.resolve("synced");
        RestorePointStore store = new RestorePointStore(local, 50, Clock.systemUTC());
        store.setSerialSourceForTests(java.util.OptionalLong::empty);
        store.save(MsqFactory.valueOf(olderTune(), ini), RestorePointStore.Reason.BEFORE_BURN, null);
        java.util.concurrent.atomic.AtomicInteger asked = new java.util.concurrent.atomic.AtomicInteger();
        SwingUtilities.invokeAndWait(() -> {
            window = new RestorePointsWindow(null, store, () -> ini, RestorePointsWindowTest::openTune,
                image -> true, new AbstractAction("Create") {
                    @Override
                    public void actionPerformed(java.awt.event.ActionEvent e) {
                    }
                });
            try {
                // the details read at opening hold a file open for a moment: Windows would not move it
                window.awaitDetailsForTests();
            } catch (Exception e) {
                throw new AssertionError(e);
            }
            window.setDetailWorkForTests(Runnable::run);
            assertTrue(window.folderLabel.getText().contains(local.toString()), window.folderLabel.getText());

            window.setFolderDialogsForTests(() -> synced, count -> {
                asked.set(count);
                return JOptionPane.CANCEL_OPTION;
            });
            window.changeFolder(synced);
            assertEquals(1, asked.get(), "asks about the one point kept so far");
            assertEquals(local.toAbsolutePath().normalize(), store.getRoot(), "cancelled: nothing changes");

            window.setFolderDialogsForTests(() -> synced, count -> JOptionPane.YES_OPTION);
            window.changeFolder(synced);
            assertEquals(synced.toAbsolutePath().normalize(), store.getRoot());
            assertEquals(1, window.pointsTable.getRowCount(), "the moved point is listed from the new folder");
            assertTrue(window.folderLabel.getText().contains(synced.toString()), window.folderLabel.getText());
        });
        assertEquals(1, store.list(SIGNATURE, "").size());
        assertEquals(List.of(), RestorePointStore.allPoints(local.toAbsolutePath()));
    }

    @Test
    void anotherCarsPointsCanBeShownAndRestoredAfterAsking() throws Exception {
        assumeFalse(GraphicsEnvironment.isHeadless(), "The window is a JDialog: requires a desktop or Xvfb");
        IniFileModel ini = ini();
        RestorePointStore store = new RestorePointStore(dir, 50, Clock.systemUTC());
        // this .ini has no Vehicle Name: the open car goes by its ECU's serial
        store.setSerialSourceForTests(() -> java.util.OptionalLong.of(7));
        store.save(MsqFactory.valueOf(olderTune(), ini), "Hunchback", RestorePointStore.Reason.BEFORE_BURN, null);
        AtomicReference<RestorePointStore.RestorePoint> restored = new AtomicReference<>();
        AtomicReference<String> asked = new AtomicReference<>();
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
            assertEquals(2, window.carBox.getItemCount());
            assertEquals("ecu-7 (this car)", window.carBox.getSelectedItem().toString());
            assertEquals(0, window.pointsTable.getRowCount(), "this car has none yet");

            window.carBox.setSelectedIndex(1);
            assertEquals("Hunchback", window.carBox.getSelectedItem().toString());
            assertEquals(1, window.pointsTable.getRowCount());
            assertTrue(window.descriptionLabel.getText().startsWith("From Hunchback, not this car."),
                window.descriptionLabel.getText());

            window.setOtherCarQuestionForTests(car -> {
                asked.set(car);
                return false;
            });
            window.restoreSelected();
            assertEquals("Hunchback", asked.get());
            assertNull(restored.get(), "declined: nothing restored");

            window.setOtherCarQuestionForTests(car -> true);
            window.restoreSelected();
            assertNotNull(restored.get());
        });
    }

    @Test
    void pointsNotMovedAreExplainedInPlainWordsAndCanBeTriedAgain() throws Exception {
        assumeFalse(GraphicsEnvironment.isHeadless(), "The window is a JDialog: requires a desktop or Xvfb");
        IniFileModel ini = ini();
        Path local = dir.resolve("local");
        Path synced = dir.resolve("synced");
        RestorePointStore store = new RestorePointStore(local, 50, Clock.systemUTC());
        store.setSerialSourceForTests(java.util.OptionalLong::empty);
        store.save(MsqFactory.valueOf(olderTune(), ini), RestorePointStore.Reason.BEFORE_BURN, null);
        // stands in for a file another program has open: the point cannot go there yet
        java.nio.file.Files.createDirectories(synced);
        java.nio.file.Files.writeString(synced.resolve("uaefi"), "in the way");
        List<String> shown = new java.util.ArrayList<>();
        SwingUtilities.invokeAndWait(() -> {
            window = new RestorePointsWindow(null, store, () -> ini, RestorePointsWindowTest::openTune,
                image -> true, new AbstractAction("Create") {
                    @Override
                    public void actionPerformed(java.awt.event.ActionEvent e) {
                    }
                });
            try {
                window.awaitDetailsForTests();
            } catch (Exception e) {
                throw new AssertionError(e);
            }
            window.setDetailWorkForTests(Runnable::run);
            window.setFolderDialogsForTests(() -> synced, count -> JOptionPane.YES_OPTION);
            window.setRetryQuestionForTests(message -> {
                shown.add(message);
                try {
                    // the user closes the other program
                    java.nio.file.Files.delete(synced.resolve("uaefi"));
                } catch (java.io.IOException e) {
                    throw new AssertionError(e);
                }
                return true;
            });
            window.changeFolder(synced);
            assertEquals(1, window.pointsTable.getRowCount(), "moved on the second try");
        });
        assertEquals(List.of("1 restore point could not be moved; another program may have it open.\n"
            + "It stays in " + local.toAbsolutePath().normalize() + " and is not listed.\n"
            + "Close that program and try again."), shown);
    }

    @Test
    void notMovedMessageCountsThePoints() {
        Path old = Path.of("old");
        assertTrue(RestorePointsWindow.notMovedMessage(3, old).startsWith(
            "3 restore points could not be moved; another program may have them open.\nThey stay in old and are"));
    }

    @Test
    void theVehicleNameNoteOpensItsPageWhenTheConsoleCan() throws Exception {
        assumeFalse(GraphicsEnvironment.isHeadless(), "The window is a JDialog: requires a desktop or Xvfb");
        IniFileModel ini = ini();
        RestorePointStore store = new RestorePointStore(dir, 50, Clock.systemUTC());
        store.setSerialSourceForTests(java.util.OptionalLong::empty);
        java.util.concurrent.atomic.AtomicInteger opened = new java.util.concurrent.atomic.AtomicInteger();
        SwingUtilities.invokeAndWait(() -> {
            window = new RestorePointsWindow(null, store, () -> ini, RestorePointsWindowTest::openTune,
                image -> true, new AbstractAction("Create") {
                    @Override
                    public void actionPerformed(java.awt.event.ActionEvent e) {
                    }
                });
            assertTrue(window.unnamedHint.getText().contains("Setup › Vehicle Information"), "says where");
            assertFalse(window.vehicleNameButton.isVisible(), "no way to open the page given");

            window.setOpenVehicleName(() -> {
                opened.incrementAndGet();
                return true;
            });
            assertTrue(window.vehicleNameButton.isVisible());
            window.vehicleNameButton.doClick();
        });
        assertEquals(1, opened.get());
    }

    @Test
    void aLongTextIsCutInItsCellAndShownWholeOnClick() throws Throwable {
        assumeFalse(GraphicsEnvironment.isHeadless(), "The window is a JDialog: requires a desktop or Xvfb");
        String ini = "[Constants]\nsignature = \"" + SIGNATURE + "\"\n"
            + "nPages = 1\npageSize = 200\npageReadCommand = \"r\"\nochBlockSize = 0\npage = 1\n"
            + "luaScript = string, ASCII, 0, 200\n";
        RawIniFile raw = IniFileReaderUtil.read(new ByteArrayInputStream(ini.getBytes(StandardCharsets.UTF_8)));
        IniFileModel model = IniFileReaderUtil.readIniFile(raw, "lua-test.ini", new IniFileMetaInfoImpl(raw));
        String then = "function onTick()\n  setTimingAdd(0)\nend\n-- before the dyno day, a long comment line";
        String now = "function onTick()\n  setTimingAdd(2)\nend\n-- after the dyno day, a long comment line";
        RestorePointStore store = new RestorePointStore(dir, 50, Clock.systemUTC());
        store.setSerialSourceForTests(java.util.OptionalLong::empty);
        store.save(MsqFactory.valueOf(script(then), model), RestorePointStore.Reason.MANUAL, null);
        SwingUtilities.invokeAndWait(() -> {
            window = new RestorePointsWindow(null, store, () -> model, () -> script(now),
                image -> true, new AbstractAction("Create") {
                    @Override
                    public void actionPerformed(java.awt.event.ActionEvent e) {
                    }
                });
            window.setDetailWorkForTests(Runnable::run);
            window.reload();
            assertEquals(1, window.diffTable.getRowCount());
            java.awt.Component cell = window.diffTable.prepareRenderer(window.diffTable.getCellRenderer(0, 1), 0, 1);
            assertEquals("Show text (4 lines)", ((JLabel) cell).getText(), "a link, not a cut copy");

            window.showFullValue(0);
            JDialog viewer = window.lastViewer;
            assertNotNull(viewer, "opened");
            List<String> texts = new java.util.ArrayList<>();
            collectText(viewer.getContentPane(), texts);
            assertTrue(texts.contains(then), "the whole Then text: " + texts);
            assertTrue(texts.contains(now), "the whole Now text: " + texts);
            assertTrue(window.lastViewerSummary.startsWith("2 lines differ"), window.lastViewerSummary);
            viewer.dispose();
        });
    }

    @Test
    void shortValuesStayAsTheyAre() {
        assertFalse(RestorePointsWindow.isLongValue("23.9"));
        assertEquals("23.9", RestorePointsWindow.cellText("23.9"));
        assertTrue(RestorePointsWindow.isLongValue("a\nb"));
        assertEquals("Show text (2 lines)", RestorePointsWindow.cellText("a\nb"));
    }

    @Test
    void onlyTheLinesThatDifferAreMarked() {
        // one line changed, one inserted: the lines after the insertion still match
        List<java.util.Set<Integer>> changed = RestorePointsWindow.changedLines(
            "a\nb\nc\nd\n", "a\nB\nc\nnew\nd\n");
        assertEquals(java.util.Set.of(1), changed.get(0));
        assertEquals(java.util.Set.of(1, 3), changed.get(1));
        assertEquals(List.of(java.util.Set.of(), java.util.Set.of()), RestorePointsWindow.changedLines("x", "x"));
    }

    @Test
    void quotesInsideATextAreKept() {
        assertEquals("hexstr = { \"A\", \"B\" }", RestorePointDiff.unquoted("hexstr = { \"A\", \"B\" }"));
        assertEquals("Enabled", RestorePointDiff.unquoted("\"Enabled\""), "an enum's label");
    }

    private static ConfigurationImage script(String text) {
        ConfigurationImage image = ConfigurationImageTestHelper.newLittleEndian(new byte[200]);
        byte[] bytes = text.getBytes(StandardCharsets.US_ASCII);
        System.arraycopy(bytes, 0, image.getContent(), 0, bytes.length);
        return image;
    }

    private static void collectText(java.awt.Container parent, List<String> texts) {
        for (java.awt.Component c : parent.getComponents()) {
            if (c instanceof javax.swing.text.JTextComponent) {
                texts.add(((javax.swing.text.JTextComponent) c).getText());
            }
            if (c instanceof java.awt.Container) {
                collectText((java.awt.Container) c, texts);
            }
        }
    }
}
