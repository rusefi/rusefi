package com.rusefi;

import com.rusefi.ui.UIContext;
import com.rusefi.ui.basic.SingleAsyncJobExecutor;
import com.rusefi.ui.basic.StatusPanelWithProgressBar;
import com.rusefi.ui.util.UiUtils;
import com.rusefi.ui.widgets.JSplitButton;
import com.rusefi.ui.widgets.JoinedButtons;
import com.rusefi.ui.widgets.ToolbarStrip;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.Component;
import java.util.Collections;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

class DevicePaneLayoutTest {
    /** Builds the pane as ConsoleUI does, with a fake scanner reporting one serial ECU. */
    static DevicePane createPane() {
        FakePortScanner scanner = new FakePortScanner();
        ConnectivityContext connectivity = new ConnectivityContext(scanner);
        scanner.fireHardwareChange(new AvailableHardware(
            Collections.singletonList(new PortResult("COM4", SerialPortType.Ecu)), false, false, false));
        StatusPanelWithProgressBar status = new StatusPanelWithProgressBar();
        return new DevicePane(new UIContext(), connectivity,
            new DeviceSessionManager(connectivity, null, () -> { }, () -> { }), new JTabbedPane(),
            new SingleAsyncJobExecutor(status), status, panel -> { }, () -> { });
    }

    @Test
    void toolbarHoldsTheUpdateControlsAndOptionsAboveTheActivityLog() throws Throwable {
        SwingUtilities.invokeAndWait(() -> {
            List<Component> all = UiUtils.getAllComponents((JComponent) createPane().getContent());

            JSplitButton update = (JSplitButton) all.stream().filter(c -> c instanceof JSplitButton)
                .findFirst().orElseThrow(AssertionError::new);
            assertEquals(JoinedButtons.HEIGHT, update.getPreferredSize().height, "As tall as the other toolbar controls");
            assertTrue(SwingUtilities.getAncestorOfClass(ToolbarStrip.class, update) != null);

            for (String option : new String[]{"Auto-update Software", "Try to keep my current tune"}) {
                JCheckBox box = (JCheckBox) all.stream()
                    .filter(c -> c instanceof JCheckBox && option.equals(((JCheckBox) c).getText()))
                    .findFirst().orElseThrow(() -> new AssertionError(option));
                assertNotNull(box.getToolTipText(), option + " explains itself");
                assertTrue(SwingUtilities.getAncestorOfClass(ToolbarStrip.class, box) != null);
            }

            assertTrue(all.stream().anyMatch(c -> c instanceof JLabel && DevicePane.ACTIVITY.equals(((JLabel) c).getText())));
            JButton copy = (JButton) all.stream()
                .filter(c -> c instanceof JButton && DevicePane.COPY_LOG.equals(((JButton) c).getText()))
                .findFirst().orElseThrow(AssertionError::new);
            assertTrue(copy.getParent() instanceof JoinedButtons);
        });
    }

    /** The update button sits in the middle of the toolbar, as before the restyle, and never covers the options. */
    @Test
    void updateButtonIsCentredAndKeepsClearOfTheOptions() throws Throwable {
        SwingUtilities.invokeAndWait(() -> {
            List<Component> all = UiUtils.getAllComponents((JComponent) createPane().getContent());
            JSplitButton update = (JSplitButton) all.stream().filter(c -> c instanceof JSplitButton)
                .findFirst().orElseThrow(AssertionError::new);
            ToolbarStrip toolbar = (ToolbarStrip) SwingUtilities.getAncestorOfClass(ToolbarStrip.class, update);
            JCheckBox autoUpdate = (JCheckBox) all.stream()
                .filter(c -> c instanceof JCheckBox && "Auto-update Software".equals(((JCheckBox) c).getText()))
                .findFirst().orElseThrow(AssertionError::new);
            Component middle = toolbar.getComponent(0);
            Component options = toolbar.getComponent(1);

            toolbar.setSize(1600, toolbar.getPreferredSize().height);
            toolbar.doLayout();
            int centre = middle.getX() + middle.getWidth() / 2;
            assertTrue(Math.abs(centre - 800) <= 1, "centred on the whole width: " + centre);
            assertTrue(options.getX() + options.getWidth() <= 1600);
            assertTrue(SwingUtilities.isDescendingFrom(autoUpdate, options));

            int narrow = middle.getPreferredSize().width + options.getPreferredSize().width + 20;
            toolbar.setSize(narrow, toolbar.getPreferredSize().height);
            toolbar.doLayout();
            assertTrue(middle.getX() + middle.getWidth() <= options.getX(), "moves left rather than overlap");
            assertEquals(new java.awt.Color(0x16A34A), getUploadColour());
        });
    }

    /** The update icon keeps the earlier upload arrow's green. */
    private static java.awt.Color getUploadColour() {
        java.awt.image.BufferedImage image = new java.awt.image.BufferedImage(16, 16, java.awt.image.BufferedImage.TYPE_INT_ARGB);
        java.awt.Graphics2D g = image.createGraphics();
        com.rusefi.ui.widgets.CommandIcon.UPLOAD.paintIcon(null, g, 0, 0);
        g.dispose();
        return new java.awt.Color(image.getRGB(8, 10), true);
    }
}
