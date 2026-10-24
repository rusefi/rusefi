package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.DialogModel;
import com.opensr5.ini.IniFileMetaInfoImpl;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.RawIniFile;
import com.opensr5.ini.field.EnumIniField;
import com.rusefi.config.FieldType;
import com.rusefi.ini.reader.IniFileReaderUtil;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.Component;
import java.io.InputStream;
import java.util.Arrays;
import java.util.Map;
import java.util.TreeMap;
import java.util.concurrent.atomic.AtomicInteger;

import static org.junit.jupiter.api.Assertions.*;

/** Display ordering must preserve the firmware ordinals used to read and write tunes. */
public class EnumCalibrationFieldRuntimeTest {
    private static IniFileModel ini;

    @BeforeAll
    static void readIni() throws Throwable {
        try (InputStream input = EnumCalibrationFieldRuntimeTest.class.getResourceAsStream("/26october.ini")) {
            assertNotNull(input);
            RawIniFile raw = IniFileReaderUtil.read(input, "26october.ini");
            ini = IniFileReaderUtil.readIniFile(raw, "26october.ini", new IniFileMetaInfoImpl(raw));
        }
    }

    @Test
    void pinModesAreAlphabeticalWithInvalidEntriesHidden() throws Exception {
        // pin_mode_e_enum: visible row 2 is firmware ordinal 64, not 2.
        checkEnum(field("spi1SckMode"), new int[]{0, 4, 64, 32},
            "default", "opendrain", "PULLDOWN", "PULLUP");
    }

    @Test
    void inputModesAreAlphabeticalWithInvalidEntriesHidden() throws Exception {
        // pin_input_mode_e_enum has holes at ordinals 3 and 7.
        checkEnum(field("clutchDownPinMode"), new int[]{0, 4, 6, 5, 2, 1},
            "DEFAULT", "INVERTED DEFAULT", "INVERTED PULLDOWN", "INVERTED PULLUP", "PULLDOWN", "PULLUP");
    }

    @Test
    void spiDevicesKeepOffAndEveryBusWithTheirOriginalOrdinals() throws Exception {
        // spi_device_e_enum is already alphabetical; Off must still encode zero.
        checkEnum(field("sdCardSpiDevice"), new int[]{0, 1, 2, 3, 4, 5, 6},
            "Off", "SPI1", "SPI2", "SPI3", "SPI4", "SPI5", "SPI6");
    }

    @Test
    void gppwmChannelsAreAlphabeticalWithTheirOriginalOrdinals() throws Exception {
        checkEnum(field("gppwm1_loadAxis"), new int[]{
            9, 16, 17, 29, 30, 7, 8, 26, 10, 3, 27, 28, 15, 5, 34, 25, 18, 19,
            20, 21, 4, 6, 22, 23, 2, 32, 33, 24, 1, 31, 12, 11, 14, 13, 0
        }, "Accel Pedal", "Aux Linear 1", "Aux Linear 2", "Aux Linear 3", "Aux Linear 4",
            "Aux Temp 1", "Aux Temp 2", "Baro pressure", "Battery Voltage", "CLT",
            "EGT 1", "EGT 2", "Ethanol (Flex) %", "Fuel Load", "Fuel Pressure", "Gear (detected)",
            "GPPWM Output 1", "GPPWM Output 2", "GPPWM Output 3", "GPPWM Output 4",
            "IAT", "Ignition Load", "Lua Gauge 1", "Lua Gauge 2", "MAP", "Oil pressure", "Oil temp",
            "RPM", "TPS", "Vehicle speed", "VVT 1 Ex Actual", "VVT 1 In Actual",
            "VVT 2 Ex Actual", "VVT 2 In Actual", "Zero");
    }

    @Test
    void sparseEnumSelectionsPreserveSurroundingBitsAndUseKeysInsteadOfRows() throws Exception {
        Map<Integer, String> values = new TreeMap<>();
        values.put(0, "Off");
        values.put(2, "Zulu");
        values.put(5, "Alpha");
        EnumIniField field = new EnumIniField("sparseMode", 1, FieldType.UINT8,
            new EnumIniField.EnumKeyValueMap(values), 2, 2);
        checkEnum(field, new int[]{0, 2, 5}, "Off", "Zulu", "Alpha");
    }

    @Test
    void unrelatedEnumRetainsItsDeclaredOrder() throws Exception {
        checkEnum(ordinaryEnum(), new int[]{0, 1, 2}, "Third", "First", "Second");
    }

    @Test
    void openingAnInvalidStoredModeDoesNotRewriteItToTheFirstVisibleChoice() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            EnumIniField field = field("spi1SckMode");
            byte[] original = new byte[field.getOffset() + 2];
            Arrays.fill(original, (byte) 0xa5);
            original[field.getOffset()] = (byte) 0x83; // Reserved ordinal 3, with the unused high bit set.
            ConfigurationImage source = new ConfigurationImage(original.clone());
            ConfigurationImage working = source.clone();
            AtomicInteger changes = new AtomicInteger();
            JComboBox<?> combo = component(createRow(field, source, working, changes), JComboBox.class);

            assertEquals(4, combo.getItemCount());
            // Current Swing behavior: the filtered-out selection leaves the first option displayed.
            assertEquals("default", combo.getSelectedItem());
            assertArrayEquals(original, working.getContent());
            assertEquals(0, changes.get());

            combo.setSelectedItem("PULLDOWN");
            byte[] expected = original.clone();
            expected[field.getOffset()] = (byte) 0xc0; // Firmware ordinal 64, preserving the high bit.
            assertArrayEquals(expected, working.getContent());
            assertEquals(1, changes.get());
            assertArrayEquals(original, source.getContent());
        });
    }

    @Test
    void booleanEnumsRemainCheckboxesAndWriteTheirOriginalOrdinals() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            for (String[] labels : new String[][]{{"No", "Yes"}, {"Yes", "No"},
                {"Disabled", "Enabled"}, {"Enabled", "Disabled"}}) {
                Map<Integer, String> values = new TreeMap<>();
                values.put(0, labels[0]);
                values.put(1, labels[1]);
                EnumIniField field = new EnumIniField("enabled", 1, FieldType.UINT8,
                    new EnumIniField.EnumKeyValueMap(values), 2, 0);
                byte[] original = {(byte) 0x5a, (byte) 0xf3, (byte) 0xa5};
                ConfigurationImage source = new ConfigurationImage(original.clone());
                ConfigurationImage working = source.clone();
                AtomicInteger changes = new AtomicInteger();
                JPanel row = createRow(field, source, working, changes);
                JCheckBox checkbox = component(row, JCheckBox.class);
                assertEquals(labels[0].equals("Yes") || labels[0].equals("Enabled"), checkbox.isSelected());
                assertEquals(0, changes.get(), "Opening the editor must not mark the tune dirty");
                assertArrayEquals(original, working.getContent());
                checkbox.doClick(0);
                assertArrayEquals(new byte[]{(byte) 0x5a, (byte) 0xf7, (byte) 0xa5}, working.getContent());
                assertEquals(1, changes.get());
                assertArrayEquals(original, source.getContent());
            }
        });
    }

    private static EnumIniField ordinaryEnum() {
        Map<Integer, String> values = new TreeMap<>();
        values.put(0, "Third");
        values.put(1, "First");
        values.put(2, "Second");
        return new EnumIniField("ordinaryMode", 1, FieldType.UINT8,
            new EnumIniField.EnumKeyValueMap(values), 0, 1);
    }

    private static EnumIniField field(String name) {
        return (EnumIniField) ini.findIniField(name).get();
    }

    private static void checkEnum(EnumIniField field, int[] ordinals, String... labels) throws Exception {
        assertEquals(labels.length, ordinals.length);
        SwingUtilities.invokeAndWait(() -> {
            // These four production fields are U08. Preserve every byte and the unused bits,
            // with a different initial selection on each iteration to exercise decoding too.
            assertEquals(FieldType.UINT8, field.getType());
            int mask = ((1 << (field.getBitSize0() + 1)) - 1) << field.getBitPosition();
            for (int i = 0; i < labels.length; i++) {
                byte[] original = new byte[field.getOffset() + 2];
                Arrays.fill(original, (byte) 0xa5);
                original[field.getOffset()] = (byte) ((0xa5 & ~mask) | (ordinals[i] << field.getBitPosition()));
                ConfigurationImage source = new ConfigurationImage(original.clone());
                ConfigurationImage working = source.clone();
                AtomicInteger changes = new AtomicInteger();
                JComboBox<?> combo = component(createRow(field, source, working, changes), JComboBox.class);

                String[] options = new String[combo.getItemCount()];
                for (int row = 0; row < options.length; row++) {
                    options[row] = (String) combo.getItemAt(row);
                }
                // Each displayed label remains paired with its original firmware ordinal in the test data.
                assertArrayEquals(labels, options, field.getName());
                assertEquals(labels[i], combo.getSelectedItem(), field.getName());
                assertEquals(labels[i], combo.getToolTipText());
                assertArrayEquals(original, working.getContent());
                assertEquals(0, changes.get(), "Opening the editor must not mark the tune dirty");

                int next = (i + 1) % labels.length;
                combo.setSelectedIndex(next);
                byte[] expected = original.clone();
                expected[field.getOffset()] = (byte) ((0xa5 & ~mask) | (ordinals[next] << field.getBitPosition()));
                assertArrayEquals(expected, working.getContent(), field.getName() + ": " + labels[next]);
                assertEquals(labels[next], combo.getToolTipText());
                assertEquals(1, changes.get(), "One selection should notify the dialog once");
                assertArrayEquals(original, source.getContent(), "Editing must not mutate the source image");
            }
        });
    }

    private static JPanel createRow(EnumIniField field, ConfigurationImage source,
                                   ConfigurationImage working, AtomicInteger changes) {
        return CalibrationFieldFactory.createFieldRow(new DialogModel.Field(field.getName(), field.getName()),
            field, source, working, changes::incrementAndGet);
    }

    private static <T extends Component> T component(JPanel row, Class<T> type) {
        for (Component component : row.getComponents()) {
            if (type.isInstance(component)) {
                return type.cast(component);
            }
        }
        throw new AssertionError("Missing " + type.getSimpleName());
    }
}
