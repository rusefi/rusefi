package com.rusefi.ui;

import com.opensr5.ConfigurationImageTestHelper;
import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.IniFileMetaInfoImpl;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.RawIniFile;
import com.rusefi.ini.reader.IniFileReaderUtil;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import javax.swing.AbstractButton;
import javax.swing.JButton;
import javax.swing.JTable;
import javax.swing.SwingUtilities;
import java.awt.Component;
import java.awt.Container;
import java.io.ByteArrayInputStream;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.atomic.AtomicReference;
import java.util.stream.Collectors;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;

import static org.junit.jupiter.api.Assertions.*;

/** Assigning a pin from the Pinout tab: the setting picked moves to the pin, whatever used it is cleared. */
class PinoutAssignTest {
    private static final String B6 = "B6 Low Side output";
    private static final String B7 = "B7 Low Side output";
    private static final String B8 = "B8 Low Side output";

    @TempDir
    Path tempDir;

    @Test
    void candidatesAreTheDialogSettingsWhoseChoicesIncludeThePin() throws Throwable {
        IniFileModel ini = readIni();
        ConfigurationImage image = image(1, 2, 0);

        List<PinAssignments.Candidate> candidates = PinAssignments.candidates(ini, image, B8);
        // in Tuning's order; the switch input cannot choose B8, and a setting no dialog shows is left out
        assertEquals(Arrays.asList("fanPin", "hiddenPin", "fuelPumpPin"),
            candidates.stream().map(c -> c.fieldKey).collect(Collectors.toList()));
        PinAssignments.Candidate fuelPump = candidates.get(2);
        assertEquals("Fuel Pump output", fuelPump.label);
        assertEquals("Fuel Pump", fuelPump.section);
        assertEquals(B6, fuelPump.current);
        assertNull(candidates.get(1).current);
        assertTrue(candidates.get(1).hidden, "Tuning disables it until its feature is on");
        assertFalse(fuelPump.hidden);

        assertEquals(Collections.singletonList("flexPin"), PinAssignments.candidates(ini, image, "D5 Switch input")
            .stream().map(c -> c.fieldKey).collect(Collectors.toList()));
        assertEquals(Collections.singletonList("fanPin"),
            PinAssignments.users(PinAssignments.candidates(ini, image, B7), B7).stream()
                .map(c -> c.fieldKey).collect(Collectors.toList()));
    }

    @Test
    void assignMovesTheSettingAndClearsTheOldUser() throws Throwable {
        IniFileModel ini = readIni();
        ConfigurationImage image = image(1, 2, 0);

        // the fan moves from B7 to B6, which the fuel pump used
        PinAssignments.assign(ini, image, "fanPin", B6);
        assertEquals("NONE", value(ini, image, "fuelPumpPin"));
        assertEquals(B6, value(ini, image, "fanPin"));

        PinAssignments.unassign(ini, image, B6);
        assertEquals("NONE", value(ini, image, "fanPin"));

        assertThrows(IllegalArgumentException.class, () -> PinAssignments.assign(ini, image, "flexPin", B6),
            "a switch input cannot use an output pin");
    }

    @Test
    void swapExchangesEverySettingOfTheTwoPins() throws Throwable {
        IniFileModel ini = readIni();
        // fuel pump on B6, fan on B7, the switch input on D5, and a setting no dialog shows also on B6
        ConfigurationImage image = ConfigurationImageTestHelper.newLittleEndian(new byte[]{1, 2, 1, 0, 0, 1});

        assertNull(PinAssignments.swapProblem(ini, image, B6, B7));
        assertEquals(Arrays.asList("Fuel Pump output", "unlistedPin"), PinAssignments.userLabels(ini, image, B6));
        PinAssignments.swap(ini, image, B6, B7);
        assertEquals(B7, value(ini, image, "fuelPumpPin"));
        assertEquals(B7, value(ini, image, "unlistedPin"), "every setting on the pin moves, shown or not");
        assertEquals(B6, value(ini, image, "fanPin"));

        // with a free pin, the swap is a move
        PinAssignments.swap(ini, image, B6, B8);
        assertEquals(B8, value(ini, image, "fanPin"));
        assertEquals(PinAssignments.BOTH_FREE, PinAssignments.swapProblem(ini, image, B6, "B9 Low Side output"));

        assertEquals("Fuel Pump output cannot use it", PinAssignments.swapProblem(ini, image, B7, "D5 Switch input"));
        byte[] before = image.getContent().clone();
        assertThrows(IllegalArgumentException.class, () -> PinAssignments.swap(ini, image, B7, "D5 Switch input"));
        assertArrayEquals(before, image.getContent(), "a refused swap changes nothing");
    }

    @Test
    void setPinGivesThePinOrClearsJustThatSetting() throws Throwable {
        IniFileModel ini = readIni();
        ConfigurationImage image = image(1, 2, 0);

        assertEquals(Arrays.asList("fanPin", "hiddenPin", "fuelPumpPin", "flexPin"),
            PinAssignments.settings(ini, image).stream().map(c -> c.fieldKey).collect(Collectors.toList()),
            "every pin setting a dialog shows, in Tuning order");

        PinAssignments.setPin(ini, image, "fanPin", null);
        assertEquals("NONE", value(ini, image, "fanPin"));
        assertEquals(B6, value(ini, image, "fuelPumpPin"), "only that setting is cleared");

        PinAssignments.setPin(ini, image, "fanPin", B6);
        assertEquals(B6, value(ini, image, "fanPin"));
        assertEquals("NONE", value(ini, image, "fuelPumpPin"), "a pin has one function");
    }

    @Test
    void functionsViewListsSettingsWithTheBoardPinsEachCanChoose() throws Throwable {
        IniFileModel ini = readIni();
        PinoutFunctionsPage page = new PinoutFunctionsPage();
        page.setBoard(Collections.singletonList(new PinoutConnectorPage("Connector B", null, Arrays.asList(
            new PinoutPin("B6", "Low side 1", "outputs", "", B6, ""),
            new PinoutPin("B7", "Low side 2", "outputs", "", B7, ""),
            new PinoutPin("B8", "Low side 3", "outputs", "", B8, ""),
            new PinoutPin("GND", "Ground", "ground", "", "", "")),
            PinoutDrawing.ColorMode.TYPE, pin -> null, assignment -> { })));
        // fuel pump on B6, fan on B7, the switch input on a pin this board's pinout does not list
        page.setTune(ini, image(1, 2, 1));
        List<String[]> setPins = new ArrayList<>();
        page.setPinSetter((field, pin) -> setPins.add(new String[]{field, pin}));

        assertEquals(Arrays.asList("fanPin", "hiddenPin", "fuelPumpPin", "flexPin"),
            page.model.rows.stream().map(c -> c.fieldKey).collect(Collectors.toList()));
        assertEquals("All  4", page.filterButtons.get(PinoutFunctionsPage.Filter.ALL).getText());
        assertEquals("Assigned  3", page.filterButtons.get(PinoutFunctionsPage.Filter.ASSIGNED).getText());
        assertEquals("Not set  1", page.filterButtons.get(PinoutFunctionsPage.Filter.NOT_SET).getText());

        PinAssignments.Candidate fan = page.model.rows.get(0);
        assertEquals("B7", page.model.getValueAt(0, PinoutFunctionsPage.PIN_COLUMN));
        assertEquals("Low side 2", page.model.getValueAt(0, 3));
        assertEquals(Arrays.asList("Not set", "B6", "B7", "B8"),
            page.choicesFor(fan).stream().map(Object::toString).collect(Collectors.toList()),
            "board pins the setting can choose; ground has no tune name");
        PinAssignments.Candidate flex = page.model.rows.get(3);
        assertEquals(Arrays.asList("Not set", "D5 Switch input"),
            page.choicesFor(flex).stream().map(Object::toString).collect(Collectors.toList()),
            "a choice not on the board stays listed while selected");

        page.setFilter(PinoutFunctionsPage.Filter.NOT_SET);
        assertEquals(1, page.table.getRowCount());
        page.setFilter(PinoutFunctionsPage.Filter.ALL);
        assertEquals(0, page.setQuery("low side 3"), "B8's function: no setting uses B8");
        assertEquals(2, page.setQuery("fuel"), "the fuel pump, and the flex input by its Tuning page");
        assertEquals(1, page.setQuery("fuel pump output"));
        assertEquals(1, page.table.getRowCount());
        page.setQuery("");

        assertTrue(page.model.isCellEditable(0, PinoutFunctionsPage.PIN_COLUMN));
        assertFalse(page.model.isCellEditable(0, PinoutFunctionsPage.SETTING_COLUMN));
        page.model.setValueAt(page.choicesFor(fan).get(3), 0, PinoutFunctionsPage.PIN_COLUMN);
        page.model.setValueAt(PinoutFunctionsPage.PinChoice.NOT_SET, 2, PinoutFunctionsPage.PIN_COLUMN);
        page.model.setValueAt(page.choicesFor(fan).get(2), 0, PinoutFunctionsPage.PIN_COLUMN);
        assertEquals(2, setPins.size(), "choosing the current pin changes nothing");
        assertArrayEquals(new String[]{"fanPin", B8}, setPins.get(0));
        assertArrayEquals(new String[]{"fuelPumpPin", null}, setPins.get(1));

        page.setPinSetter(null);
        assertFalse(page.model.isCellEditable(0, PinoutFunctionsPage.PIN_COLUMN), "read-only without a tune to edit");
    }

    @Test
    void rebootOnlySettingsAreFlaggedWhereverTheyChange() throws Throwable {
        IniFileModel ini = readIni();
        // fuel pump on B6, fan on B7; the fan output needs a restart
        ConfigurationImage image = image(1, 2, 0);
        List<PinAssignments.Candidate> forB6 = PinAssignments.candidates(ini, image, B6);
        PinAssignments.Candidate fan = forB6.stream().filter(c -> c.fieldKey.equals("fanPin")).findFirst().get();
        PinAssignments.Candidate fuelPump = forB6.stream().filter(c -> c.fieldKey.equals("fuelPumpPin")).findFirst().get();
        assertTrue(fan.needsRestart);
        assertFalse(fuelPump.needsRestart);
        assertEquals("Fan output  (restart)", PinoutFunctionsPage.settingText(fan));

        PinoutPin b6 = new PinoutPin("B6", "Low side", "outputs", "outputs", B6, "");
        PinAssignPanel toB6 = new PinAssignPanel(b6, "Connector B", forB6, ts -> ts.substring(0, 2));
        assertTrue(toB6.select("fanPin"));
        assertTrue(toB6.effect.getText().endsWith("Takes effect after you burn and restart the ECU: Fan output."),
            toB6.effect.getText());

        // giving B7 to the fuel pump clears the fan, which needs a restart as much as setting it
        PinoutPin b7 = new PinoutPin("B7", "Low side", "outputs", "outputs", B7, "");
        PinAssignPanel toB7 = new PinAssignPanel(b7, "Connector B", PinAssignments.candidates(ini, image, B7),
            ts -> ts.substring(0, 2));
        assertTrue(toB7.select("fuelPumpPin"));
        assertTrue(toB7.effect.getText().contains("restart the ECU: Fan output."), toB7.effect.getText());
        assertTrue(toB7.unassignButton.getToolTipText().contains("restart the ECU: Fan output."));

        // a swap involving the fan says so; one without it does not
        assertEquals(Collections.singletonList("Fan output"), PinAssignments.swapRestartLabels(ini, image, B6, B7));
        assertEquals(Collections.emptyList(), PinAssignments.swapRestartLabels(ini, image, B6, B8));
        PinSwapPanel swap = new PinSwapPanel(b6, "Connector B", Collections.singletonList("Fuel Pump output"),
            Collections.singletonList(new PinSwapPanel.Target(b7, "Connector B",
                Collections.singletonList("Fan output"), null, Collections.singletonList("Fan output"))));
        assertTrue(swap.select("B7"));
        assertTrue(swap.effect.getText().endsWith("restart the ECU: Fan output."), swap.effect.getText());
    }

    @Test
    void swapPickerOnlyChoosesPinsThatCanSwap() {
        PinoutPin b6 = new PinoutPin("B6", "Low side", "outputs", "outputs", B6, "");
        PinoutPin b7 = new PinoutPin("B7", "Low side", "outputs", "outputs", B7, "");
        PinoutPin b8 = new PinoutPin("B8", "Low side", "outputs", "outputs", B8, "");
        PinoutPin d5 = new PinoutPin("D5", "Flex", "switch inputs", "switch_inputs", "D5 Switch input", "");
        PinSwapPanel panel = new PinSwapPanel(b6, "Connector B", Collections.singletonList("Fuel Pump output"),
            Arrays.asList(
                new PinSwapPanel.Target(b7, "Connector B", Collections.singletonList("Fan output"), null),
                new PinSwapPanel.Target(b8, "Connector B", Collections.emptyList(), null),
                new PinSwapPanel.Target(d5, "Connector D", Collections.emptyList(), "Fuel Pump output cannot use it")));
        List<String> swapped = new ArrayList<>();
        panel.setOnSwap(target -> swapped.add(target.pin.name));

        assertNull(panel.selected());
        assertFalse(panel.swapButton.isEnabled());
        assertFalse(panel.select("D5"), "a pin that cannot take the settings is not choosable");

        assertTrue(panel.select("B7"));
        assertEquals("Fuel Pump output moves to B7; Fan output moves to B6.", panel.effect.getText());
        assertTrue(panel.select("B8"));
        assertEquals("Fuel Pump output moves to B8. B6 becomes free.", panel.effect.getText());

        panel.search.setText("fan");
        assertEquals("B7", panel.selected().pin.name, "the first match is selected while searching");
        panel.swapButton.doClick();
        assertEquals(Collections.singletonList("B7"), swapped);
    }

    @Test
    void pickerDescribesTheMoveAndOnlyAssignsAChange() {
        PinoutPin pin = new PinoutPin("B6", "Low side", "outputs", "outputs", B6, "");
        List<PinAssignments.Candidate> candidates = Arrays.asList(
            new PinAssignments.Candidate("fuelPumpPin", "Fuel Pump output", "Fuel Pump", "fuelPumpDialog", B6, false),
            new PinAssignments.Candidate("fanPin", "Fan output", "Fan", "fanSettings", B7, false),
            new PinAssignments.Candidate("hiddenPin", "Hidden output", "Fan", "fanSettings", null, true));
        PinAssignPanel panel = new PinAssignPanel(pin, "Connector B", candidates, ts -> ts.substring(0, 2));
        List<PinAssignments.Candidate> assigned = new ArrayList<>();
        panel.setOnAssign(assigned::add);

        assertEquals("fuelPumpPin", panel.selected().fieldKey, "the current user is selected first");
        assertFalse(panel.assignButton.isEnabled());
        assertTrue(panel.unassignButton.isVisible());

        assertTrue(panel.select("fanPin"));
        assertTrue(panel.assignButton.isEnabled());
        assertEquals("Fan output moves from B7 to B6. Fuel Pump output is set to None.", panel.effect.getText());

        assertTrue(panel.select("hiddenPin"));
        assertTrue(panel.effect.getText().startsWith("Hidden output will use B6."));
        assertTrue(panel.effect.getText().contains("assigning a pin does not turn it on"));
        assertTrue(panel.assignAndOpenButton.isVisible(), "a hidden setting offers to open Tuning");

        // titles are not selectable: moving down onto the Fan title lands on the setting below it
        assertTrue(panel.select("fuelPumpPin"));
        panel.list.setSelectedIndex(2);
        assertEquals("fanPin", panel.selected().fieldKey);

        panel.search.setText("hidden");
        assertEquals(Arrays.asList("FAN", "Hidden output"), listed(panel));
        assertEquals("hiddenPin", panel.selected().fieldKey, "the first match is selected while searching");

        panel.assignButton.doClick();
        assertEquals(Collections.singletonList("hiddenPin"),
            assigned.stream().map(c -> c.fieldKey).collect(Collectors.toList()));
    }

    /** End to end: Pinout's picker edits Tuning's tune, Tune use follows, and Undo in Tuning restores it. */
    @Test
    void assignFromPinoutIsAnUndoableTuningEdit() throws Throwable {
        IniFileModel ini = readIni();
        Path archive = writeConnectorArchive();
        AtomicReference<TuningPane> tuningRef = new AtomicReference<>();
        AtomicReference<PinoutPane> pinoutRef = new AtomicReference<>();
        AtomicReference<PinAssignPanel> picker = new AtomicReference<>();
        AtomicReference<PinSwapPanel> swapPicker = new AtomicReference<>();
        UIContext context = new UIContext();

        try {
            SwingUtilities.invokeAndWait(() -> {
                context.setOfflineMode(true);
                context.setIniFileModelForOffline(ini);
                PinoutPane pinout = new PinoutPane(context, false);
                pinoutRef.set(pinout);
                TuningPane tuning = new TuningPane(context);
                tuningRef.set(tuning);
                ConfigurationImage image = image(1, 2, 0);
                tuning.seedOfflineImage(image, "fanSettings");
                context.fireConfigImageChanged(image);
                // same wiring as ConsoleUI
                pinout.setTuneEditor(tuning::applyEdit);
                pinout.assignDialogOpener = picker::set;
                pinout.swapDialogOpener = swapPicker::set;
            });

            SwingUtilities.invokeAndWait(() -> {
                PinoutPane pinout = pinoutRef.get();
                pinout.buildConnectorTabs(Collections.singletonList("connector.yaml"), archive.toFile());
                JTable table = components(pinout.getContent(), JTable.class).get(0);
                assertEquals(Arrays.asList("Fuel Pump output", "Fan output", ""), tuneUse(table));

                PinoutConnectorPage page = components(pinout.getContent(), PinoutConnectorPage.class).get(0);
                PinoutPin b8 = page.find("B8");
                page.select(b8);
                AbstractButton assign = page.card.assignButton;
                assertTrue(assign.isVisible());
                assertEquals("Assign…", assign.getText());
                assign.doClick();

                PinAssignPanel panel = picker.get();
                assertNotNull(panel, "Assign… opens the picker");
                assertTrue(panel.select("fanPin"));
                panel.assignButton.doClick();
            });

            // Tune use refreshes from the config-image notification
            SwingUtilities.invokeAndWait(() -> { });
            SwingUtilities.invokeAndWait(() -> {
                JTable table = components(pinoutRef.get().getContent(), JTable.class).get(0);
                assertEquals(Arrays.asList("Fuel Pump output", "", "Fan output"), tuneUse(table),
                    "the fan moved from B7 to B8");
                assertEquals(B8, value(ini, tuningRef.get().getDialogForTests().getWorkingImage(), "fanPin"),
                    "the open Tuning page shows the new pin");

                JButton undo = components(tuningRef.get().getContent(), JButton.class).stream()
                    .filter(b -> "Undo".equals(b.getText())).findFirst().orElseThrow(AssertionError::new);
                assertTrue(undo.isEnabled(), "the assignment is one undo step");
                undo.doClick();
                assertEquals(B7, value(ini, tuningRef.get().getDialogForTests().getWorkingImage(), "fanPin"));
            });

            // Swap with…: the fuel pump and the fan exchange B6 and B7
            SwingUtilities.invokeAndWait(() -> { });
            SwingUtilities.invokeAndWait(() -> {
                PinoutPane pinout = pinoutRef.get();
                assertEquals(Arrays.asList("Fuel Pump output", "Fan output", ""),
                    tuneUse(components(pinout.getContent(), JTable.class).get(0)),
                    "Tune use follows Undo in Tuning (TuningUndoNotificationTest)");
                PinoutConnectorPage page = components(pinout.getContent(), PinoutConnectorPage.class).get(0);
                page.select(page.find("B6"));
                assertTrue(page.card.swapButton.isVisible());
                page.card.swapButton.doClick();
                PinSwapPanel panel = swapPicker.get();
                assertNotNull(panel, "Swap with… opens the picker");
                assertTrue(panel.select("B7"));
                panel.swapButton.doClick();
            });
            SwingUtilities.invokeAndWait(() -> { });
            SwingUtilities.invokeAndWait(() -> {
                JTable table = components(pinoutRef.get().getContent(), JTable.class).get(0);
                assertEquals(Arrays.asList("Fan output", "Fuel Pump output", ""), tuneUse(table));
                ConfigurationImage tune = tuningRef.get().getDialogForTests().getWorkingImage();
                assertEquals(B7, value(ini, tune, "fuelPumpPin"));
                assertEquals(B6, value(ini, tune, "fanPin"));

                // Functions view: the fuel pump moves to B8 from the Pin column
                PinoutPane pinout = pinoutRef.get();
                pinout.functionsViewButton.doClick();
                assertTrue(pinout.isShowingFunctions());
                PinoutFunctionsPage functions = pinout.functionsPage;
                assertTrue(functions.isVisible(), "the Functions card is shown");
                int row = functions.model.rows.indexOf(functions.model.rows.stream()
                    .filter(c -> c.fieldKey.equals("fuelPumpPin")).findFirst().orElseThrow(AssertionError::new));
                PinoutFunctionsPage.PinChoice b8 = functions.choicesFor(functions.model.rows.get(row)).stream()
                    .filter(c -> "B8".equals(c.name())).findFirst().orElseThrow(AssertionError::new);
                functions.model.setValueAt(b8, row, PinoutFunctionsPage.PIN_COLUMN);
            });
            SwingUtilities.invokeAndWait(() -> { });
            SwingUtilities.invokeAndWait(() -> {
                PinoutPane pinout = pinoutRef.get();
                assertEquals(B8, value(ini, tuningRef.get().getDialogForTests().getWorkingImage(), "fuelPumpPin"));
                PinoutFunctionsPage functions = pinout.functionsPage;
                int row = 0;
                while (!functions.model.rows.get(row).fieldKey.equals("fuelPumpPin")) {
                    row++;
                }
                assertEquals("B8", functions.model.getValueAt(row, PinoutFunctionsPage.PIN_COLUMN),
                    "the list follows the tune change");
                JTable pins = components(pinout.getContent(), PinoutConnectorPage.class).get(0).table;
                assertEquals(Arrays.asList("Fan output", "", "Fuel Pump output"), tuneUse(pins));

                // a jump from Tuning to a pin shows it on its connector
                pinout.highlightByEnumValue(B8);
                assertFalse(pinout.isShowingFunctions());
                assertTrue(pinout.pinsViewButton.isSelected());
            });
        } finally {
            SwingUtilities.invokeAndWait(() -> {
                if (tuningRef.get() != null) {
                    tuningRef.get().destroy();
                }
                context.getLinkManager().close();
            });
        }
    }

    // ---- fixtures

    /** Fuel pump, fan and a switch input by their ordinals; the hidden output and its feature off. */
    private static ConfigurationImage image(int fuelPump, int fan, int flex) {
        return ConfigurationImageTestHelper.newLittleEndian(new byte[]{(byte) fuelPump, (byte) fan, (byte) flex, 0, 0, 0});
    }

    private static String value(IniFileModel ini, ConfigurationImage image, String key) {
        return ConfigurationImageGetterSetter.getStringValue(ini.getAllIniFields().get(key), image)
            .replace("\"", "");
    }

    private static IniFileModel readIni() throws Throwable {
        String outputs = "\"NONE\", \"" + B6 + "\", \"" + B7 + "\", \"" + B8 + "\"";
        String text = "[Constants]\n"
            + "signature = \"pinout-assign-test\"\n"
            + "nPages = 1\npageSize = 6\npageReadCommand = \"r\"\nochBlockSize = 0\npage = 1\n"
            + "fuelPumpPin = bits, U08, 0, [0:1], " + outputs + "\n"
            + "fanPin = bits, U08, 1, [0:1], " + outputs + "\n"
            + "flexPin = bits, U08, 2, [0:0], \"NONE\", \"D5 Switch input\"\n"
            + "hiddenPin = bits, U08, 3, [0:1], " + outputs + "\n"
            + "hiddenEnabled = bits, U08, 4, [0:0], \"false\", \"true\"\n"
            + "unlistedPin = bits, U08, 5, [0:1], " + outputs + "\n"
            + "[Menu]\nmenu = \"Setup\"\n"
            + "subMenu = fanSettings, \"Fan\"\nsubMenu = outputsDialog, \"Outputs\"\n"
            + "[UserDefined]\n"
            + "dialog = fanSettings, \"Fan\"\nfield = \"Fan output\", fanPin\n"
            + "field = \"Hidden enabled\", hiddenEnabled\n"
            + "field = \"Hidden output\", hiddenPin, { hiddenEnabled }\n"
            + "dialog = fuelPumpDialog, \"Fuel Pump\"\nfield = \"Fuel Pump output\", fuelPumpPin\n"
            + "field = \"Flex input\", flexPin\n"
            + "dialog = outputsDialog, \"Outputs\"\npanel = fuelPumpDialog\n"
            // the fan output applies only after a restart, like gppwm1_pin or vvtPins1
            + "[ConstantsExtensions]\nrequiresPowerCycle = fanPin\n";
        RawIniFile raw = IniFileReaderUtil.read(new ByteArrayInputStream(text.getBytes(StandardCharsets.UTF_8)));
        return IniFileReaderUtil.readIniFile(raw, "pinout-assign-test.ini", new IniFileMetaInfoImpl(raw));
    }

    private Path writeConnectorArchive() throws Exception {
        Path archive = tempDir.resolve("connectors.zip");
        String yaml = "pins:\n"
            + "  - pin: B6\n    ts_name: ___ Low Side output\n"
            + "  - pin: B7\n    ts_name: ___ Low Side output\n"
            + "  - pin: B8\n    ts_name: ___ Low Side output\n";
        try (ZipOutputStream zip = new ZipOutputStream(Files.newOutputStream(archive))) {
            zip.putNextEntry(new ZipEntry("connector.yaml"));
            zip.write(yaml.getBytes(StandardCharsets.UTF_8));
            zip.closeEntry();
        }
        return archive;
    }

    private static List<String> tuneUse(JTable table) {
        List<String> uses = new ArrayList<>();
        int column = table.getColumn("Tune use").getModelIndex();
        for (int row = 0; row < table.getModel().getRowCount(); row++) {
            uses.add((String) table.getModel().getValueAt(row, column));
        }
        return uses;
    }

    private static List<String> listed(PinAssignPanel panel) {
        List<String> items = new ArrayList<>();
        for (int i = 0; i < panel.model.size(); i++) {
            Object item = panel.model.get(i);
            items.add(item instanceof PinAssignPanel.Header ? item.toString().toUpperCase() : item.toString());
        }
        return items;
    }

    private static <T extends Component> List<T> components(Component root, Class<T> type) {
        List<T> result = new ArrayList<>();
        if (type.isInstance(root)) {
            result.add(type.cast(root));
        }
        if (root instanceof Container) {
            for (Component child : ((Container) root).getComponents()) {
                result.addAll(components(child, type));
            }
        }
        return result;
    }
}
