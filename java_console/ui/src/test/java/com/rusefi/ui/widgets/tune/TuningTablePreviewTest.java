package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.TableModel;
import com.opensr5.ini.field.ArrayIniField;
import com.rusefi.config.FieldType;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;
import java.util.Optional;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.*;

class TuningTablePreviewTest {
    @Test
    void previewFollowsAvailableSpaceAndViewMode() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            TuningTableView view = new TuningTableView("VE");
            JPanel cards = (JPanel) view.getContent().getComponent(1);
            JPanel grid = (JPanel) cards.getComponent(0);
            Surface3DView preview = (Surface3DView) grid.getComponent(1);
            loadTable(view, true);
            resize(cards, 600, 400);
            assertTrue(preview.isVisible());
            JCheckBox toggle = findToggle(view.getContent());
            assertNotNull(toggle);
            toggle.doClick();
            assertFalse(preview.isVisible());
            toggle.doClick();
            assertTrue(preview.isVisible());
            resize(cards, 600, 180);
            assertFalse(preview.isVisible());
            resize(cards, 600, 400);
            assertTrue(preview.isVisible());
            resize(cards, 250, 400);
            assertFalse(preview.isVisible());
            resize(cards, 600, 400);
            loadTable(view, false);
            assertFalse(preview.isVisible(), "Missing axes must not enable a misleading preview");
        });
    }

    private static void resize(JPanel cards, int width, int height) {
        cards.setSize(width, height);
        cards.doLayout();
        for (ComponentListener listener : cards.getComponentListeners()) {
            listener.componentResized(new ComponentEvent(cards, ComponentEvent.COMPONENT_RESIZED));
        }
    }

    private static JCheckBox findToggle(Container container) {
        for (Component component : container.getComponents()) {
            if (component instanceof JCheckBox) {
                return (JCheckBox) component;
            }
            if (component instanceof Container) {
                JCheckBox found = findToggle((Container) component);
                if (found != null) {
                    return found;
                }
            }
        }
        return null;
    }

    private static void loadTable(TuningTableView view, boolean withAxes) {
        IniFileModel ini = mock(IniFileModel.class);
        TableModel table = mock(TableModel.class);
        ArrayIniField z = new ArrayIniField("z", 0, FieldType.FLOAT, 3, 2, "", 1, "0", "100", "1");
        ArrayIniField x = new ArrayIniField("x", 24, FieldType.FLOAT, 3, 1, "", 1, "0", "8000", "0");
        ArrayIniField y = new ArrayIniField("y", 36, FieldType.FLOAT, 2, 1, "", 1, "0", "100", "0");
        ConfigurationImage image = new ConfigurationImage(44);
        ConfigurationImageGetterSetter.setArrayValues(z, image, new Double[][]{{10.0, 20.0, 30.0}, {40.0, 50.0, 60.0}});
        ConfigurationImageGetterSetter.setArrayValues(x, image, new Double[][]{{1000.0, 1500.0, 6000.0}});
        ConfigurationImageGetterSetter.setArrayValues(y, image, new Double[][]{{20.0, 100.0}});
        when(ini.getTable("table")).thenReturn(table);
        when(table.getZBinsConstant()).thenReturn("z");
        when(table.getXBinsConstant()).thenReturn("x");
        when(table.getYBinsConstant()).thenReturn("y");
        when(ini.findIniField("z")).thenReturn(Optional.of(z));
        when(ini.findIniField("x")).thenReturn(withAxes ? Optional.of(x) : Optional.empty());
        when(ini.findIniField("y")).thenReturn(withAxes ? Optional.of(y) : Optional.empty());
        view.displayTable(ini, "table", image);
    }
}
