package com.rusefi.ui;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.DialogModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.ScalarIniField;
import com.rusefi.config.FieldType;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.function.Consumer;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.*;

/** Passing characterization: closed SLCAN tabs retain callbacks. Update expectations with the fix. */
class SlcanTabLifecycleDefectTest {
    @Test
    void disconnectedSnifferTabsRetainCallbacksAndStillUpdateRemovedCheckboxes() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            CountingContext context = new CountingContext();
            IniFileModel ini = mock(IniFileModel.class);
            when(ini.getDialogs()).thenReturn(Collections.singletonMap("canHw1",
                new DialogModel("canHw1", "CAN 1", Collections.emptyList(), Collections.emptyList())));
            when(ini.getIniField("canSniffer1_listenOurs")).thenReturn(new ScalarIniField(
                "canSniffer1_listenOurs", 0, "", FieldType.UINT8, 1, "0", 0));
            when(ini.getIniField("canSniffer1_read")).thenReturn(new ScalarIniField(
                "canSniffer1_read", 1, "", FieldType.UINT8, 1, "0", 0));
            context.iniFileState.setIniFileModelForTest(ini);
            JTabbedPane tabs = new JTabbedPane();
            List<JCheckBox> removedCheckboxes = new ArrayList<>();
            SlcanTabController controller = new SlcanTabController(tabs,
                () -> new SlcanTab(context, message -> fail(message), false));
            try {
                for (int cycle = 1; cycle <= 10; cycle++) {
                    controller.update(ini, true);
                    Component lazyTab = tabs.getComponentAt(0);
                    Graphics graphics = new BufferedImage(100, 100, BufferedImage.TYPE_INT_RGB).getGraphics();
                    try {
                        lazyTab.paint(graphics);
                    } finally {
                        graphics.dispose();
                    }
                    JCheckBox checkbox = findCheckbox(lazyTab);
                    assertNotNull(checkbox);
                    removedCheckboxes.add(checkbox);
                    controller.update(null, false); // ConsoleUI's disconnect path calls SlcanTab.close().
                    assertEquals(0, tabs.getTabCount());

                    context.deliveries = 0;
                    context.fireConfigImageChanged(new ConfigurationImage(new byte[]{1, 1, 0, 0, 0, 0, 0, 0}));
                    assertEquals(cycle, context.deliveries,
                        "Current defect: every closed tab remains subscribed to the live UIContext");
                    assertTrue(removedCheckboxes.stream().allMatch(AbstractButton::isSelected));
                    context.fireConfigImageChanged(new ConfigurationImage(8));
                    assertTrue(removedCheckboxes.stream().noneMatch(AbstractButton::isSelected));
                }
            } finally {
                controller.update(null, false);
                context.getLinkManager().setNotifyGlobalStatusOnClose(false).close();
            }
        });
    }

    private static JCheckBox findCheckbox(Component component) {
        if (component instanceof JCheckBox) {
            return (JCheckBox) component;
        }
        if (component instanceof Container) {
            for (Component child : ((Container) component).getComponents()) {
                JCheckBox found = findCheckbox(child);
                if (found != null) {
                    return found;
                }
            }
        }
        return null;
    }

    private static class CountingContext extends UIContext {
        int deliveries;

        @Override
        public void addConfigImageListener(Consumer<ConfigurationImage> listener) {
            super.addConfigImageListener(image -> {
                deliveries++;
                listener.accept(image);
            });
        }
    }
}
