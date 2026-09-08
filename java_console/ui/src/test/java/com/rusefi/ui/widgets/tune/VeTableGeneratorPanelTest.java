package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.TableModel;
import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.ScalarIniField;
import com.rusefi.config.FieldType;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.util.HashMap;
import java.util.Map;
import java.util.Optional;
import java.util.TreeMap;
import java.util.concurrent.atomic.AtomicReference;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.ArgumentMatchers.anyString;
import static org.mockito.Mockito.*;

class VeTableGeneratorPanelTest {
    @Test
    void previewSizingAtLargeUiFont() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            JScrollPane scroll = (JScrollPane) VeTableGeneratorPanel.buildVeTable(
                new double[][]{{100.5, 125.0}, {90.0, 99.9}},
                new double[]{650, 20000}, new double[]{30, 160}, false);
            JTable table = (JTable) scroll.getViewport().getView();
            table.setFont(table.getFont().deriveFont(24f));
            table.getTableHeader().setFont(table.getFont());
            // Scaled UI fonts must fit both cell values and RPM headers.
            assertTrue(table.getRowHeight() > table.getFontMetrics(table.getFont()).getHeight());
            assertTrue(table.getColumnModel().getColumn(2).getMinWidth() >
                table.getFontMetrics(table.getFont()).stringWidth("20000"));
            scroll.setSize(900, 400);
            scroll.doLayout();
            assertTrue(table.getScrollableTracksViewportWidth());
        });
    }

    @Test
    void appliesFuelSettingsWithVeWithoutChangingSourceOrAxes() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            Fixture f = new Fixture();
            byte[] original = f.source.getContent().clone();
            VeTableGeneratorPanel panel = f.panel();
            editor(panel, "displacement").setText("3.2");
            editor(panel, "injector_flow").setText("650");
            editor(panel, "fuelReferencePressure").setText("350");
            editor(panel, "stoichRatioPrimary").setText("9.9");
            ((JComboBox<?>) find(panel, component -> "injectorFlowAsMassFlow".equals(component.getName()))).setSelectedIndex(1);
            button(panel, "Generate Preview").doClick();
            button(panel, "Apply to working tune").doClick();
            assertNotNull(f.applied.get());
            assertEquals(3.2, f.value("displacement"), 0.001);
            assertEquals(650, f.value("injector_flow"), 0.001);
            assertEquals(350, f.value("fuelReferencePressure"), 0.001);
            assertEquals(9.9, f.value("stoichRatioPrimary"), 0.001);
            assertEquals("\"g/s\"", ConfigurationImageGetterSetter.getStringValue(f.fields.get("injectorFlowAsMassFlow"), f.applied.get()));
            assertArrayEquals(original, f.source.getContent());
            for (int i = 0; i < original.length; i++) {
                if (i < 16 || (i >= 20 && i < 32) || (i >= 48 && i != 56)) {
                    assertEquals(original[i], f.applied.get().getContent()[i], "byte " + i);
                }
            }
            assertNotEquals(0, f.applied.get().getContent()[16]);
        });
    }

    @Test
    void olderIniWithoutEngineFieldsStillGenerates() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            Fixture f = new Fixture();
            f.fields.keySet().removeIf(name -> name.equals("displacement") || name.equals("injector_flow") ||
                name.equals("injectorFlowAsMassFlow") || name.equals("fuelReferencePressure") || name.equals("stoichRatioPrimary"));
            VeTableGeneratorPanel panel = f.panel();
            assertNull(editor(panel, "displacement"));
            button(panel, "Generate Preview").doClick();
            button(panel, "Apply to working tune").doClick();
            assertNotNull(f.applied.get());
        });
    }

    @Test
    void invalidFuelSettingBlocksApplyAndCanBeCorrected() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            Fixture f = new Fixture();
            VeTableGeneratorPanel panel = f.panel();
            button(panel, "Generate Preview").doClick();
            for (String invalid : new String[]{"", "NaN", "Infinity", "-1", "66"}) {
                editor(panel, "displacement").setText(invalid);
                button(panel, "Apply to working tune").doClick();
                assertNull(f.applied.get());
            }
            editor(panel, "displacement").setText("2.5");
            button(panel, "Apply to working tune").doClick();
            assertNotNull(f.applied.get());
        });
    }

    @Test
    void discardDoesNotApplyEditedSettings() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            Fixture f = new Fixture();
            VeTableGeneratorPanel panel = f.panel();
            editor(panel, "displacement").setText("4");
            button(panel, "Generate Preview").doClick();
            button(panel, "Discard").doClick();
            assertNull(f.applied.get());
            assertEquals(2, Double.parseDouble(ConfigurationImageGetterSetter.getStringValue(f.fields.get("displacement"), f.source)), 0.001);
        });
    }

    private static JTextField editor(Container root, String name) {
        return (JTextField) find(root, component -> name.equals(component.getName()));
    }

    private static JButton button(Container root, String text) {
        return (JButton) find(root, component -> component instanceof JButton && text.equals(((JButton) component).getText()));
    }

    private static Component find(Container root, java.util.function.Predicate<Component> predicate) {
        for (Component child : root.getComponents()) {
            if (predicate.test(child)) {
                return child;
            }
            if (child instanceof Container) {
                Component result = find((Container) child, predicate);
                if (result != null) {
                    return result;
                }
            }
        }
        return null;
    }

    private static class Fixture {
        final IniFileModel ini = mock(IniFileModel.class);
        final Map<String, IniField> fields = new HashMap<>();
        final ConfigurationImage source = new ConfigurationImage(new byte[64]);
        final AtomicReference<ConfigurationImage> applied = new AtomicReference<>();

        Fixture() {
            ArrayIniField rpm = new ArrayIniField("rpm", 0, FieldType.FLOAT, 1, 2, "RPM", 1, "0", "20000", "0");
            ArrayIniField map = new ArrayIniField("map", 8, FieldType.FLOAT, 1, 2, "kPa", 1, "0", "400", "0");
            ArrayIniField ve = new ArrayIniField("ve", 16, FieldType.UINT8, 2, 2, "%", 1, "0", "200", "1");
            fields.put("rpm", rpm);
            fields.put("map", map);
            fields.put("ve", ve);
            ConfigurationImageGetterSetter.setArrayValues(rpm, source, new Double[]{900.0, 7000.0});
            ConfigurationImageGetterSetter.setArrayValues(map, source, new Double[]{30.0, 100.0});
            scalar("displacement", 32, 2);
            scalar("injector_flow", 36, 400);
            scalar("fuelReferencePressure", 40, 300);
            scalar("stoichRatioPrimary", 44, 14.7);
            scalar("fuelAlgorithm", 48, 0);
            scalar("veOverrideMode", 52, 0);
            TreeMap<Integer, String> unitNames = new TreeMap<>();
            unitNames.put(0, "cc/min");
            unitNames.put(1, "g/s");
            fields.put("injectorFlowAsMassFlow", new EnumIniField("injectorFlowAsMassFlow", 56,
                FieldType.UINT8, new EnumIniField.EnumKeyValueMap(unitNames), 0, 0));
            TableModel table = mock(TableModel.class);
            when(table.getXBinsConstant()).thenReturn("rpm");
            when(table.getYBinsConstant()).thenReturn("map");
            when(table.getZBinsConstant()).thenReturn("ve");
            when(ini.getTable("veTableTbl")).thenReturn(table);
            when(ini.findIniField(anyString())).thenAnswer(call -> Optional.ofNullable(fields.get(call.getArgument(0))));
        }

        void scalar(String name, int offset, double value) {
            ScalarIniField field = new ScalarIniField(name, offset, "", FieldType.FLOAT, 1, "3", 0);
            fields.put(name, field);
            ConfigurationImageGetterSetter.setValue2(field, source, name, Double.toString(value));
        }

        VeTableGeneratorPanel panel() {
            return new VeTableGeneratorPanel(ini, source, applied::set, () -> {});
        }

        double value(String name) {
            return Double.parseDouble(ConfigurationImageGetterSetter.getStringValue(fields.get(name), applied.get()));
        }
    }
}
