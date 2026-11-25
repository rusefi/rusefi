package com.rusefi;

import com.rusefi.autoupdate.Autoupdate;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.basic.SoftwareUpdateCoordinator;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CompletableFuture;

import static org.junit.jupiter.api.Assertions.*;
import static org.junit.jupiter.api.Assumptions.assumeFalse;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

/**
 * DEFECT, coverage first: once an application update has downloaded, "Restart now" is only a banner in the
 * startup window's main tabbed view. The window opens on "Scanning for ECU…" (and stays there while it looks
 * for or auto-connects to the ECU), where that banner is not shown, so nothing on screen offers the restart.
 * A tuner only got the restart prompt after loading a tune, which opens the main console with its own banner.
 */
class StartupUpdateRestartPromptDefectTest {
    @Test
    void downloadedUpdateOffersNoVisibleRestartWhileTheStartupWindowIsScanning() throws Exception {
        assumeFalse(GraphicsEnvironment.isHeadless(), "Opens the startup window: requires a desktop or Xvfb");
        PortScanner scanner = mock(PortScanner.class);
        when(scanner.getCurrentHardware()).thenReturn(new AvailableHardware(List.of(), false, false, false));
        ConnectivityContext connectivity = new ConnectivityContext(scanner);
        // the update has already downloaded: the coordinator starts in RESTART_REQUIRED
        SoftwareUpdateCoordinator coordinator =
            new SoftwareUpdateCoordinator(CompletableFuture.completedFuture(Autoupdate.UpdateOutcome.UPDATED));
        assertEquals(SoftwareUpdateCoordinator.State.RESTART_REQUIRED, coordinator.getState());

        StartupFrame[] startup = new StartupFrame[1];
        SwingUtilities.invokeAndWait(() -> {
            startup[0] = new StartupFrame(connectivity, new UIContext(), coordinator);
            startup[0].showUi();
        });
        // the coordinator tells its listeners on a later event
        SwingUtilities.invokeAndWait(() -> { });
        try {
            SwingUtilities.invokeAndWait(() -> {
                JFrame frame = startup[0].getFrameForTests();
                List<JButton> restart = buttons(frame.getContentPane(), "Restart now");
                assertFalse(restart.isEmpty(), "the startup window does have a Restart now button");
                assertTrue(restart.stream().noneMatch(Component::isShowing),
                    "none of them is on screen: the window still shows Scanning for ECU…");
                assertTrue(texts(frame.getContentPane()).contains("Scanning for ECU…"));
                assertTrue(windowsOfferingRestart().isEmpty(), "and no other window offers it");
            });
        } finally {
            SwingUtilities.invokeAndWait(() -> startup[0].getFrameForTests().dispose());
        }
    }

    private static List<Window> windowsOfferingRestart() {
        List<Window> offering = new ArrayList<>();
        for (Window window : Window.getWindows()) {
            if (window.isShowing() && window instanceof RootPaneContainer
                && buttons(((RootPaneContainer) window).getContentPane(), "Restart now").stream()
                .anyMatch(Component::isShowing)) {
                offering.add(window);
            }
        }
        return offering;
    }

    private static List<JButton> buttons(Container parent, String text) {
        List<JButton> found = new ArrayList<>();
        for (Component c : parent.getComponents()) {
            if (c instanceof JButton && text.equals(((JButton) c).getText())) {
                found.add((JButton) c);
            }
            if (c instanceof Container) {
                found.addAll(buttons((Container) c, text));
            }
        }
        return found;
    }

    private static List<String> texts(Container parent) {
        List<String> found = new ArrayList<>();
        for (Component c : parent.getComponents()) {
            if (c instanceof JLabel && c.isShowing()) {
                found.add(((JLabel) c).getText());
            }
            if (c instanceof Container) {
                found.addAll(texts((Container) c));
            }
        }
        return found;
    }
}
