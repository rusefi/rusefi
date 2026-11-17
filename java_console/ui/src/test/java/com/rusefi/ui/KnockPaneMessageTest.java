package com.rusefi.ui;

import com.rusefi.core.SensorCentral;
import com.rusefi.ui.util.UiUtils;
import org.junit.jupiter.api.Test;

import javax.swing.*;

import static org.junit.jupiter.api.Assertions.*;

/**
 * Before the plots have data a message card explains the state; once a line arrives the plots
 * stay, so the last capture remains visible after Stop.
 */
class KnockPaneMessageTest {
    @Test
    void messageExplainsEachStateUntilTheFirstLineArrives() throws Throwable {
        KnockPane[] pane = new KnockPane[1];
        SwingUtilities.invokeAndWait(() -> pane[0] = new KnockPane(new UIContext()));
        SwingUtilities.invokeAndWait(() -> { });
        assertEquals(KnockPane.EMPTY_NOT_CONNECTED, message(pane[0]));
        assertFalse(offersStart(pane[0]));

        SwingUtilities.invokeAndWait(() -> pane[0].simulateConnected(1));
        assertEquals(KnockPane.EMPTY_WAITING, message(pane[0]), "Capturing, no complete line yet");

        SwingUtilities.invokeAndWait(() -> pane[0].setStartState(false));
        assertEquals(KnockPane.EMPTY_STOPPED, message(pane[0]));
        assertTrue(offersStart(pane[0]), "Stopped: the card offers Start capture");
        SwingUtilities.invokeAndWait(() -> {
            JButton start = (JButton) UiUtils.getAllComponents(pane[0].getContent()).stream()
                .filter(c -> c instanceof JButton && c.isVisible() && c.isEnabled()
                    && KnockPane.START_CAPTURE.equals(((JButton) c).getText())
                    && SwingUtilities.getAncestorOfClass(com.rusefi.ui.widgets.ToolbarStrip.class, c) == null)
                .findFirst().orElseThrow(() -> new AssertionError("No enabled Start capture on the card"));
            assertNotNull(start.getToolTipText());
        });

        SwingUtilities.invokeAndWait(() -> pane[0].setStartState(true));
        sendLine((int) (System.nanoTime() & 0x3f) + 1);
        assertNull(message(pane[0]), "A line arrived: the plots are shown");

        SwingUtilities.invokeAndWait(() -> pane[0].setStartState(false));
        assertNull(message(pane[0]), "Stopping keeps the last capture visible");
    }

    private static String message(KnockPane pane) throws Exception {
        String[] text = new String[1];
        SwingUtilities.invokeAndWait(() -> text[0] = pane.getMessageForTests());
        return text[0];
    }

    private static boolean offersStart(KnockPane pane) throws Exception {
        boolean[] offered = new boolean[1];
        SwingUtilities.invokeAndWait(() -> offered[0] = pane.isMessageStartOfferedForTests());
        return offered[0];
    }

    private static void sendLine(int value) throws Exception {
        for (int i = 0; i < 16; i++) {
            long b = (value + i) & 0xFF;
            SensorCentral.getInstance().setValue((b << 24) | (b << 16) | (b << 8) | b, "m_knockSpectrum" + (i + 1));
        }
        SwingUtilities.invokeAndWait(() -> { });
    }
}
