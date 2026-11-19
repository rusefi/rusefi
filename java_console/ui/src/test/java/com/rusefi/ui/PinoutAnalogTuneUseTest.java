package com.rusefi.ui;

import com.opensr5.ConfigurationImageTestHelper;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.DialogModel;
import com.opensr5.ini.IniFileMetaInfoImpl;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.RawIniFile;
import com.opensr5.ini.field.EnumIniField;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.ui.widgets.tune.CalibrationFieldFactory;
import com.rusefi.ui.widgets.tune.PinoutLink;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import javax.swing.AbstractButton;
import javax.swing.JComponent;
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
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;

import static org.junit.jupiter.api.Assertions.*;

/**
 * Analog input selectors ({@code map_sensor_hwChannel}, {@code clt_adcChannel}, {@code tps1_1AdcChannel}...)
 * choose connector pins from {@code $adc_channel_e_list}, whose labels are the pins' expanded {@code ts_name}
 * ("D9 MAP" on uaEFI), but their names do not follow the {@code .*pins?\d*} convention. The pin list must count
 * ({@link PinoutLookup#isPinSetting}): such a pin shows its Tune use and its selector has a Pinout button.
 */
class PinoutAnalogTuneUseTest {
    private static final String D9_MAP = "D9 MAP";
    private static final String B6 = "B6 Low Side output";

    @TempDir
    Path tempDir;

    @Test
    void analogInputPinShowsItsTuneUseAndItsSelectorHasAPinoutButton() throws Throwable {
        IniFileModel ini = readIni();
        // fuel pump on B6, MAP sensor on D9
        ConfigurationImage image = ConfigurationImageTestHelper.newLittleEndian(new byte[]{1, 1});
        EnumIniField map = (EnumIniField) ini.getAllIniFields().get("map_sensor_hwChannel");
        assertTrue(map.isPinEnum(), "the INI marks the MAP input as choosing from a pin list");
        Path archive = writeConnectorArchive();
        UIContext context = new UIContext();

        try {
            AtomicReference<PinoutPane> pinoutRef = new AtomicReference<>();
            SwingUtilities.invokeAndWait(() -> {
                context.setOfflineMode(true);
                context.setIniFileModelForOffline(ini);
                pinoutRef.set(new PinoutPane(context, false));
                context.fireConfigImageChanged(image);
            });
            // the pane takes the tune on the next event
            SwingUtilities.invokeAndWait(() -> {
                PinoutPane pinout = pinoutRef.get();
                pinout.buildConnectorTabs(Collections.singletonList("connector.yaml"), archive.toFile());
                JTable table = components(pinout.getContent(), JTable.class).get(0);
                int tuneUse = table.getColumn("Tune use").getModelIndex();
                assertEquals("Fuel Pump output", table.getModel().getValueAt(0, tuneUse), "control: an output");
                assertEquals("MAP ADC input", table.getModel().getValueAt(1, tuneUse),
                    "D9 carries the MAP sensor");

                PinoutLink link = pin -> { };
                assertTrue(hasPinoutButton(row(ini, image, "fuelPumpPin", link)), "control: an output");
                assertTrue(hasPinoutButton(row(ini, image, "map_sensor_hwChannel", link)),
                    "the MAP input selector has a Pinout button");
            });
        } finally {
            SwingUtilities.invokeAndWait(() -> context.getLinkManager().close());
        }
    }

    /** Assign, Swap and the Functions view see analog inputs too ({@code PinAssignments} uses isPinSetting). */
    @Test
    void assignSwapAndFunctionsSeeTheAnalogInput() throws Throwable {
        IniFileModel ini = readIni();
        // fuel pump on B6, MAP sensor on D9
        ConfigurationImage image = ConfigurationImageTestHelper.newLittleEndian(new byte[]{1, 1});

        assertEquals(Collections.singletonList("map_sensor_hwChannel"), PinAssignments.candidates(ini, image, D9_MAP)
            .stream().map(c -> c.fieldKey).collect(java.util.stream.Collectors.toList()),
            "the MAP input is offered for D9 in the Assign picker");
        assertEquals(Arrays.asList("fuelPumpPin", "map_sensor_hwChannel"), PinAssignments.settings(ini, image).stream()
            .map(c -> c.fieldKey).collect(java.util.stream.Collectors.toList()), "the Functions view lists both");
        assertEquals(Collections.singletonList("MAP ADC input"), PinAssignments.userLabels(ini, image, D9_MAP),
            "a swap of D9 moves the MAP input");
        PinAssignments.unassign(ini, image, D9_MAP);
        assertNull(PinoutLookup.pinChoice(com.opensr5.ConfigurationImageGetterSetter.getStringValue(
            ini.getAllIniFields().get("map_sensor_hwChannel"), image)), "Unassign frees D9");
        // control: the output is seen everywhere
        assertEquals(Collections.singletonList("Fuel Pump output"), PinAssignments.userLabels(ini, image, B6));
    }

    private static JComponent row(IniFileModel ini, ConfigurationImage image, String key, PinoutLink link) {
        DialogModel.Field field = ini.getFieldsInUiOrder().get(key);
        return CalibrationFieldFactory.createFieldRow(field, ini.getAllIniFields().get(key), image, image.clone(),
            () -> { }, link);
    }

    private static boolean hasPinoutButton(JComponent row) {
        return components(row, AbstractButton.class).stream().anyMatch(b -> "Pinout".equals(b.getText()));
    }

    private static IniFileModel readIni() throws Throwable {
        // the pin lists as generated for uaEFI: an output list and the analog input list
        String text = "[Constants]\n"
            + "#define output_pin_e_list=\"NONE\",\"" + B6 + "\"\n"
            + "#define adc_channel_e_list=\"NONE\",\"" + D9_MAP + "\"\n"
            + "signature = \"pinout-analog-test\"\n"
            + "nPages = 1\npageSize = 2\npageReadCommand = \"r\"\nochBlockSize = 0\npage = 1\n"
            + "fuelPumpPin = bits, U08, 0, [0:0], $output_pin_e_list\n"
            + "map_sensor_hwChannel = bits, U08, 1, [0:0], $adc_channel_e_list\n"
            + "[Menu]\nmenu = \"Setup\"\nsubMenu = sensors, \"Sensors\"\n"
            + "[UserDefined]\n"
            + "dialog = sensors, \"Sensors\"\n"
            + "field = \"Fuel Pump output\", fuelPumpPin\n"
            + "field = \"MAP ADC input\", map_sensor_hwChannel\n";
        RawIniFile raw = IniFileReaderUtil.read(new ByteArrayInputStream(text.getBytes(StandardCharsets.UTF_8)));
        return IniFileReaderUtil.readIniFile(raw, "pinout-analog-test.ini", new IniFileMetaInfoImpl(raw));
    }

    private Path writeConnectorArchive() throws Exception {
        Path archive = tempDir.resolve("connectors.zip");
        String yaml = "pins:\n"
            + "  - pin: B6\n    ts_name: ___ Low Side output\n"
            + "  - pin: D9\n    ts_name: ___ MAP\n";
        try (ZipOutputStream zip = new ZipOutputStream(Files.newOutputStream(archive))) {
            zip.putNextEntry(new ZipEntry("connector.yaml"));
            zip.write(yaml.getBytes(StandardCharsets.UTF_8));
            zip.closeEntry();
        }
        return archive;
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
