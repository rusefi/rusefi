package com.rusefi.ui;

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
        assertTrue(panel.effect.getText().contains("turn its feature on"));

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
        return new ConfigurationImage(new byte[]{(byte) fuelPump, (byte) fan, (byte) flex, 0, 0, 0});
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
            + "dialog = outputsDialog, \"Outputs\"\npanel = fuelPumpDialog\n";
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
