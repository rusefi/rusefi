package com.rusefi.maintenance;

import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.util.concurrent.atomic.AtomicReference;

import static org.junit.jupiter.api.Assertions.*;

class ManualIniFilePickerTest {
    @Test
    void startupPickerLayoutOnAWideWindow() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            JRootPane rootPane = new JRootPane();
            JPanel connect = new JPanel();
            JTabbedPane tabs = new JTabbedPane();
            tabs.addTab("Update ECU firmware", new JPanel());
            tabs.addTab("Connect", connect);
            rootPane.setContentPane(tabs);
            ManualIniFilePicker.registerStartupHost(rootPane, tabs, connect, () -> {});

            ManualIniFilePicker.showPicker(rootPane, rootPane,
                "rusEFI noini-10540-20261119.2026.11.19.uaefi_pro.1254041395");
            tabs.setSize(2000, 1100);
            layoutRecursively(tabs);

            JFileChooser chooser = findChooser(tabs.getComponentAt(1));
            assertNotNull(chooser);
            assertTrue(chooser.getWidth() < 1000, "retain the compact chooser width");
            assertTrue(chooser.getHeight() < 700, "retain the compact chooser height");
            Container content = chooser.getParent();
            assertTrue(content.getX() > 0);
            assertTrue(content.getY() > 0);
            assertEquals(content.getParent().getWidth() / 2.0,
                content.getX() + content.getWidth() / 2.0, 1.0);
            assertEquals(content.getParent().getHeight() / 2.0,
                content.getY() + content.getHeight() / 2.0, 1.0);
            assertFalse(rootPane.getGlassPane().isVisible());
            assertNotNull(findButton(tabs.getComponentAt(1), "Go to Update ECU firmware"));
        });
    }

    @Test
    void nonStartupPickerRetainsGlassPaneFallbackAndCancelRestoresIt() {
        JRootPane rootPane = new JRootPane();
        JPanel previousGlassPane = new JPanel();
        rootPane.setGlassPane(previousGlassPane);
        previousGlassPane.setVisible(true);

        ManualIniFilePicker.showPicker(rootPane, rootPane, "test-signature");

        assertNotSame(previousGlassPane, rootPane.getGlassPane());
        assertTrue(rootPane.getGlassPane().isVisible());
        clickCancel(rootPane.getGlassPane());
        assertSame(previousGlassPane, rootPane.getGlassPane());
        assertTrue(previousGlassPane.isVisible());
    }

    @Test
    void startupPickerLeavesTabsAccessibleAndCancelRestoresConnectContent() {
        JRootPane rootPane = new JRootPane();
        JPanel connect = new JPanel();
        JTabbedPane tabs = new JTabbedPane();
        tabs.addTab("Update ECU firmware", new JPanel());
        tabs.addTab("Connect", connect);
        rootPane.setContentPane(tabs);
        boolean[] revealed = {false};
        ManualIniFilePicker.registerStartupHost(rootPane, tabs, connect, () -> revealed[0] = true);

        ManualIniFilePicker.showPicker(rootPane, rootPane, "test-signature");

        assertFalse(rootPane.getGlassPane().isVisible());
        assertTrue(revealed[0]);
        assertEquals(1, tabs.getSelectedIndex());
        Component picker = tabs.getComponentAt(1);
        assertNotSame(connect, picker);
        assertNotNull(findButton(picker, "Go to Update ECU firmware"));

        clickCancel(picker);
        assertSame(connect, tabs.getComponentAt(1));
        assertEquals(0, tabs.getSelectedIndex());
    }

    @Test
    void firmwareNavigationSelectsTheExistingTabWithoutStartingAnUpdate() {
        JRootPane rootPane = new JRootPane();
        JPanel connect = new JPanel();
        JPanel firmware = new JPanel();
        JTabbedPane tabs = new JTabbedPane();
        tabs.addTab("Update ECU firmware", firmware);
        tabs.addTab("Connect", connect);
        rootPane.setContentPane(tabs);
        ManualIniFilePicker.registerStartupHost(rootPane, tabs, connect, () -> {});

        ManualIniFilePicker.showPicker(rootPane, rootPane, "test-signature");
        findButton(tabs.getComponentAt(1), "Go to Update ECU firmware").doClick();

        assertEquals(0, tabs.getSelectedIndex());
        assertSame(firmware, tabs.getComponentAt(0));
        clickCancel(tabs.getComponentAt(1));
        assertSame(connect, tabs.getComponentAt(1));
        assertEquals(0, tabs.getSelectedIndex());
    }

    @Test
    void secondStartupPickerReplacesActivePickerAndRestoresOriginalConnectContent() {
        JRootPane rootPane = new JRootPane();
        JPanel connect = new JPanel();
        JTabbedPane tabs = new JTabbedPane();
        tabs.addTab("Update ECU firmware", new JPanel());
        tabs.addTab("Connect", connect);
        rootPane.setContentPane(tabs);
        ManualIniFilePicker.registerStartupHost(rootPane, tabs, connect, () -> {});

        ManualIniFilePicker.showPicker(rootPane, rootPane, "first-signature");
        Component firstPicker = tabs.getComponentAt(1);
        ManualIniFilePicker.showPicker(rootPane, rootPane, "second-signature");
        Component secondPicker = tabs.getComponentAt(1);

        assertFalse(rootPane.getGlassPane().isVisible());
        assertNotSame(firstPicker, secondPicker);
        clickCancel(firstPicker);
        assertSame(secondPicker, tabs.getComponentAt(1), "stale picker actions must not restore over a newer picker");
        clickCancel(secondPicker);
        assertSame(connect, tabs.getComponentAt(1));
        assertEquals(0, tabs.getSelectedIndex());
    }

    @Test
    void approvalOfReplacementPickerImportsItsOwnSignatureAndRestoresConnectTab() {
        JRootPane rootPane = new JRootPane();
        JPanel connect = new JPanel();
        JTabbedPane tabs = new JTabbedPane();
        tabs.addTab("Update ECU firmware", new JPanel());
        tabs.addTab("Connect", connect);
        rootPane.setContentPane(tabs);
        ManualIniFilePicker.registerStartupHost(rootPane, tabs, connect, () -> {});
        ManualIniFilePicker.showPicker(rootPane, rootPane, "first-signature");
        AtomicReference<String> importedSignature = new AtomicReference<>();

        ManualIniFilePicker.showPicker(rootPane, rootPane, "second-signature",
            (signature, file) -> {
                importedSignature.set(signature);
                return new Object();
            });
        findChooser(tabs.getComponentAt(1)).approveSelection();

        assertEquals("second-signature", importedSignature.get());
        assertSame(connect, tabs.getComponentAt(1));
        assertEquals(0, tabs.getSelectedIndex());
    }

    private static JButton findButton(Component component, String text) {
        if (component instanceof JButton && text.equals(((JButton) component).getText())) {
            return (JButton) component;
        }
        if (component instanceof Container) {
            for (Component child : ((Container) component).getComponents()) {
                JButton found = findButton(child, text);
                if (found != null) {
                    return found;
                }
            }
        }
        return null;
    }

    private static void clickCancel(Component component) {
        if (component instanceof JFileChooser) {
            ((JFileChooser) component).cancelSelection();
        } else if (component instanceof Container) {
            for (Component child : ((Container) component).getComponents()) {
                clickCancel(child);
            }
        }
    }

    private static JFileChooser findChooser(Component component) {
        if (component instanceof JFileChooser) {
            return (JFileChooser) component;
        }
        if (component instanceof Container) {
            for (Component child : ((Container) component).getComponents()) {
                JFileChooser found = findChooser(child);
                if (found != null) {
                    return found;
                }
            }
        }
        return null;
    }

    private static void layoutRecursively(Container container) {
        container.doLayout();
        for (Component component : container.getComponents()) {
            if (component instanceof Container) {
                layoutRecursively((Container) component);
            }
        }
    }
}
