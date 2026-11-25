package com.rusefi;

import com.rusefi.autoupdate.Autoupdate;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.basic.ApplicationUpdatePrompt;
import com.rusefi.ui.basic.SoftwareUpdateCoordinator;
import org.junit.jupiter.api.AfterEach;
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
 * [tag:update_restart_prompt] Once an application update has downloaded, "Restart now" was only a banner in the
 * startup window's main tabbed view. The window opens on "Scanning for ECU…" (and stays there while it looks for
 * or auto-connects to the ECU), where that banner is not shown, so a tuner only got the restart prompt after
 * loading a tune. The window in use now asks to restart.
 */
class StartupUpdateRestartPromptDefectTest {
    @AfterEach
    void realDialogBack() {
        ApplicationUpdatePrompt.setRestartQuestionForTests(null);
    }

    @Test
    void downloadedUpdateAsksToRestartWhileTheStartupWindowIsScanning() throws Exception {
        assumeFalse(GraphicsEnvironment.isHeadless(), "Opens the startup window: requires a desktop or Xvfb");
        PortScanner scanner = mock(PortScanner.class);
        when(scanner.getCurrentHardware()).thenReturn(new AvailableHardware(List.of(), false, false, false));
        ConnectivityContext connectivity = new ConnectivityContext(scanner);
        // the update has already downloaded: the coordinator starts in RESTART_REQUIRED
        SoftwareUpdateCoordinator coordinator =
            new SoftwareUpdateCoordinator(CompletableFuture.completedFuture(Autoupdate.UpdateOutcome.UPDATED));
        assertEquals(SoftwareUpdateCoordinator.State.RESTART_REQUIRED, coordinator.getState());
        List<Component> asked = new ArrayList<>();
        // Later: nothing restarts
        ApplicationUpdatePrompt.setRestartQuestionForTests(parent -> {
            asked.add(parent);
            return false;
        });

        StartupFrame[] startup = new StartupFrame[1];
        SwingUtilities.invokeAndWait(() -> {
            startup[0] = new StartupFrame(connectivity, new UIContext(), coordinator);
            startup[0].showUi();
        });
        // the coordinator tells its listeners on a later event, which then asks on the next one
        SwingUtilities.invokeAndWait(() -> { });
        SwingUtilities.invokeAndWait(() -> { });
        try {
            SwingUtilities.invokeAndWait(() -> {
                JFrame frame = startup[0].getFrameForTests();
                assertTrue(texts(frame.getContentPane()).contains("Scanning for ECU…"), "still scanning");
                assertEquals(List.of(frame), asked, "asked once, over the startup window");

                // a second window, e.g. the main console opened later, does not ask again
                ApplicationUpdatePrompt.showRestart(new JFrame(), coordinator, () -> fail("not asked"));
                assertEquals(1, asked.size());
            });
        } finally {
            SwingUtilities.invokeAndWait(() -> startup[0].getFrameForTests().dispose());
        }
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
