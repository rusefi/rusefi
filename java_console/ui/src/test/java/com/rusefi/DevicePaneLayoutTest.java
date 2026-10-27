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

            for (String option : new String[]{"Auto-update Software", "Migrate Settings"}) {
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
}
