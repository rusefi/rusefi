package com.rusefi.ui;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileMetaInfoImpl;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.RawIniFile;
import com.opensr5.ini.SubMenuModel;
import com.rusefi.ini.reader.IniFileReaderUtil;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import javax.swing.*;
import javax.swing.tree.DefaultMutableTreeNode;
import java.awt.Component;
import java.awt.Container;
import java.awt.Rectangle;
import java.awt.event.MouseEvent;
import java.io.ByteArrayInputStream;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.atomic.AtomicReference;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;

import static org.junit.jupiter.api.Assertions.*;

class PinoutTuneUseNavigationTest {
    @TempDir
    Path tempDir;

    /** A pin assigned in an inner dialog must open the menu page containing that dialog. */
    @Test
    void doubleClickOnNestedTuneUseOpensContainingTuningPage() throws Throwable {
        IniFileModel ini = readIni();
        Path archive = writeConnectorArchive();
        AtomicReference<TuningPane> tuningRef = new AtomicReference<>();
        AtomicReference<PinoutPane> pinoutRef = new AtomicReference<>();
        AtomicReference<JTabbedPane> tabsRef = new AtomicReference<>();
        UIContext context = new UIContext();
        List<String> requestedFields = new ArrayList<>();

        try {
            SwingUtilities.invokeAndWait(() -> {
                context.setOfflineMode(true);
                context.setIniFileModelForOffline(ini);
                PinoutPane pinout = new PinoutPane(context, false);
                pinoutRef.set(pinout);
                TuningPane tuning = new TuningPane(context);
                tuningRef.set(tuning);
                ConfigurationImage image = new ConfigurationImage(new byte[]{1, 1});
                tuning.seedOfflineImage(image, "fanSettings");
                context.fireConfigImageChanged(image);

                JTabbedPane tabs = new JTabbedPane();
                tabsRef.set(tabs);
                tabs.addTab("Pinout", pinout.getContent());
                tabs.addTab("Tuning", tuning.getContent());
                // Same navigation wiring as ConsoleUI, without a top-level window.
                pinout.setNavigateToTune((dialogKey, fieldKey) -> {
                    requestedFields.add(fieldKey);
                    tabs.setSelectedComponent(tuning.getContent());
                    tuning.navigateToField(dialogKey, fieldKey);
                });
            });

            // Drain the config-image notification before building/clicking the real table.
            SwingUtilities.invokeAndWait(() -> {
                PinoutPane pinout = pinoutRef.get();
                TuningPane tuning = tuningRef.get();
                JTabbedPane tabs = tabsRef.get();
                pinout.buildConnectorTabs(Collections.singletonList("connector.yaml"), archive.toFile());
                JTable table = components(pinout.getContent(), JTable.class).get(0);
                assertEquals("Fuel Pump output", table.getValueAt(0, table.getColumn("Tune use").getModelIndex()));
                assertEquals("Fan output", table.getValueAt(1, table.getColumn("Tune use").getModelIndex()));

                // Establish that the destination renders and both pin assignments exist.
                tuning.navigateToField("outputsDialog", "fuelPumpPin");
                assertEquals("outputsDialog", selectedMenu(tuning));
                assertTrue(hasSelectedPin(tuning, "B6 Low Side output"));

                // Control: a Tune use click targeting a menu dialog opens the right page.
                tabs.setSelectedComponent(pinout.getContent());
                doubleClickTuneUse(table, 1);
                assertSame(tuning.getContent(), tabs.getSelectedComponent());
                assertEquals("fanSettings", selectedMenu(tuning));
                assertTrue(hasSelectedPin(tuning, "B7 Low Side output"));

                tabs.setSelectedComponent(pinout.getContent());
                doubleClickTuneUse(table, 0);
                assertEquals(Arrays.asList("fanPin", "fuelPumpPin"), requestedFields,
                    "The pin assignment was found and the actual double-click handler fired");
                assertSame(tuning.getContent(), tabs.getSelectedComponent(),
                    "The outer Tuning tab opens");

                // fuelPumpDialog is nested below outputsDialogLeft/outputsDialog and
                // is not itself a menu item. Navigate to its containing Outputs page.
                assertEquals("outputsDialog", selectedMenu(tuning));
                assertFalse(hasSelectedPin(tuning, "B7 Low Side output"));
                assertTrue(hasSelectedPin(tuning, "B6 Low Side output"),
                    "The assigned fuel-pump field must be displayed");
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

    private static IniFileModel readIni() throws Throwable {
        // Mirrors the Outputs -> outputsDialogLeft -> fuelPumpDialog hierarchy in
        // firmware/tunerstudio/tunerstudio.template.ini, with a direct Fan menu as a control.
        String text = "[Constants]\n"
            + "signature = \"pinout-navigation-test\"\n"
            + "nPages = 1\npageSize = 2\npageReadCommand = \"r\"\nochBlockSize = 0\npage = 1\n"
            + "fuelPumpPin = bits, U08, 0, [0:1], \"NONE\", \"B6 Low Side output\"\n"
            + "fanPin = bits, U08, 1, [0:1], \"NONE\", \"B7 Low Side output\"\n"
            + "[Menu]\nmenu = \"Setup\"\n"
            + "subMenu = fanSettings, \"Fan\"\nsubMenu = outputsDialog, \"Outputs\"\n"
            + "[UserDefined]\n"
            + "dialog = fanSettings, \"Fan\"\nfield = \"Fan output\", fanPin\n"
            + "dialog = fuelPumpDialog, \"Fuel Pump\"\nfield = \"Fuel Pump output\", fuelPumpPin\n"
            + "dialog = outputsDialogLeft\npanel = fuelPumpDialog\n"
            + "dialog = outputsDialog, \"Outputs\"\npanel = outputsDialogLeft, West\n";
        RawIniFile raw = IniFileReaderUtil.read(new ByteArrayInputStream(text.getBytes(StandardCharsets.UTF_8)));
        return IniFileReaderUtil.readIniFile(raw, "pinout-navigation-test.ini", new IniFileMetaInfoImpl(raw));
    }

    private Path writeConnectorArchive() throws Exception {
        Path archive = tempDir.resolve("connectors.zip");
        String yaml = "pins:\n"
            + "  - pin: B6\n    ts_name: ___ Low Side output\n"
            + "  - pin: B7\n    ts_name: ___ Low Side output\n";
        try (ZipOutputStream zip = new ZipOutputStream(Files.newOutputStream(archive))) {
            zip.putNextEntry(new ZipEntry("connector.yaml"));
            zip.write(yaml.getBytes(StandardCharsets.UTF_8));
            zip.closeEntry();
        }
        return archive;
    }

    private static void doubleClickTuneUse(JTable table, int modelRow) {
        int row = table.convertRowIndexToView(modelRow);
        int column = table.convertColumnIndexToView(table.getColumn("Tune use").getModelIndex());
        Rectangle cell = table.getCellRect(row, column, true);
        table.dispatchEvent(new MouseEvent(table, MouseEvent.MOUSE_CLICKED, 0, 0,
            cell.x + cell.width / 2, cell.y + cell.height / 2, 2, false, MouseEvent.BUTTON1));
    }

    private static String selectedMenu(TuningPane tuning) {
        JTree tree = components(tuning.getContent(), JTree.class).get(0);
        DefaultMutableTreeNode node = (DefaultMutableTreeNode) tree.getLastSelectedPathComponent();
        assertNotNull(node);
        return ((SubMenuModel) node.getUserObject()).getKey();
    }

    private static boolean hasSelectedPin(TuningPane tuning, String pin) {
        return components(tuning.getContent(), JComboBox.class).stream()
            .anyMatch(combo -> pin.equals(combo.getSelectedItem()));
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
