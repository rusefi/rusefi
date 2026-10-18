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
import java.util.IdentityHashMap;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.function.Consumer;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.*;

/** Removing a sniffer tab must release every bus subscription to the live UIContext. */
class SlcanTabLifecycleDefectTest {
    @Test
    void disconnectedSnifferTabsReleaseCallbacksAndOnlyCurrentCheckboxesUpdate() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            CountingContext context = new CountingContext();
            IniFileModel ini = mock(IniFileModel.class);
            Map<String, DialogModel> dialogs = new LinkedHashMap<>();
            for (int bus = 1; bus <= 3; bus++) {
                String dialogKey = "canHw" + bus;
                dialogs.put(dialogKey, new DialogModel(dialogKey, "CAN " + bus,
                    Collections.emptyList(), Collections.emptyList()));
                String listenOurs = "canSniffer" + bus + "_listenOurs";
                String read = "canSniffer" + bus + "_read";
                when(ini.getIniField(listenOurs)).thenReturn(new ScalarIniField(
                    listenOurs, (bus - 1) * 2, "", FieldType.UINT8, 1, "0", 0));
                when(ini.getIniField(read)).thenReturn(new ScalarIniField(
                    read, (bus - 1) * 2 + 1, "", FieldType.UINT8, 1, "0", 0));
            }
            when(ini.getDialogs()).thenReturn(dialogs);
            context.iniFileState.setIniFileModelForTest(ini);
            JTabbedPane tabs = new JTabbedPane();
            List<JCheckBox> removedCheckboxes = new ArrayList<>();
            List<SlcanTab> createdTabs = new ArrayList<>();
            SlcanTabController controller = new SlcanTabController(tabs,
                () -> {
                    SlcanTab tab = new SlcanTab(context, message -> fail(message), false);
                    createdTabs.add(tab);
                    return tab;
                });
            // Numeric reads require a four-byte window even for a UINT8 field.
            ConfigurationImage enabled = new ConfigurationImage(new byte[]{1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0});
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
                    List<JCheckBox> currentCheckboxes = new ArrayList<>();
                    findCheckboxes(lazyTab, currentCheckboxes);
                    assertEquals(3, currentCheckboxes.size());
                    context.deliveries = 0;
                    context.fireConfigImageChanged(new ConfigurationImage(12));
                    assertEquals(3, context.deliveries, "Only the current tab's three buses receive updates");
                    assertTrue(currentCheckboxes.stream().noneMatch(AbstractButton::isSelected));
                    assertTrue(removedCheckboxes.stream().allMatch(AbstractButton::isSelected));
                    context.fireConfigImageChanged(enabled);
                    assertEquals(6, context.deliveries);
                    assertTrue(currentCheckboxes.stream().allMatch(AbstractButton::isSelected));
                    removedCheckboxes.addAll(currentCheckboxes);
                    controller.update(null, false); // ConsoleUI's disconnect path calls SlcanTab.close().
                    assertEquals(0, tabs.getTabCount());

                    context.deliveries = 0;
                    createdTabs.get(createdTabs.size() - 1).close(); // Repeated close is harmless.
                    context.fireConfigImageChanged(new ConfigurationImage(12));
                    assertEquals(0, context.deliveries, "Closed tabs must unsubscribe from the live UIContext");
                    assertTrue(removedCheckboxes.stream().allMatch(AbstractButton::isSelected),
                        "Removed controls must retain their last state instead of processing later tune images");
                }
            } finally {
                controller.update(null, false);
                context.getLinkManager().setNotifyGlobalStatusOnClose(false).close();
            }
        });
    }

    private static void findCheckboxes(Component component, List<JCheckBox> checkboxes) {
        if (component instanceof JCheckBox) {
            checkboxes.add((JCheckBox) component);
        }
        if (component instanceof Container) {
            for (Component child : ((Container) component).getComponents()) {
                findCheckboxes(child, checkboxes);
            }
        }
    }

    private static class CountingContext extends UIContext {
        int deliveries;
        private final Map<Consumer<ConfigurationImage>, Consumer<ConfigurationImage>> wrappers = new IdentityHashMap<>();

        @Override
        public void addConfigImageListener(Consumer<ConfigurationImage> listener) {
            Consumer<ConfigurationImage> wrapper = image -> {
                deliveries++;
                listener.accept(image);
            };
            wrappers.put(listener, wrapper);
            super.addConfigImageListener(wrapper);
        }

        @Override
        public void removeConfigImageListener(Consumer<ConfigurationImage> listener) {
            super.removeConfigImageListener(wrappers.remove(listener));
        }
    }
}
