package com.rusefi.ui;

import com.rusefi.core.SensorCentral;
import com.rusefi.ui.util.UiUtils;
import com.rusefi.ui.widgets.StatusChip;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.event.MouseEvent;

import static org.junit.jupiter.api.Assertions.assertEquals;

/**
 * The magnitudes plot follows the newest spectrum line while the pointer is off the
 * spectrograms, shows the slice under the pointer while hovering, and goes back to live on exit.
 */
class KnockPaneMagnitudeSourceTest {
    @Test
    void magnitudesFollowTheNewestLineUnlessHovering() throws Throwable {
        KnockPane[] pane = new KnockPane[1];
        SwingUtilities.invokeAndWait(() -> pane[0] = new KnockPane(new UIContext()));
        // after the connection listener's own invokeLater, which marks the pane disconnected
        SwingUtilities.invokeAndWait(() -> pane[0].simulateConnected(1));
        SwingUtilities.invokeAndWait(() -> { });
        // distinct from any earlier test's values: SensorCentral only notifies on change
        int line = (int) (System.nanoTime() & 0x3f) + 1;

        sendLine(line);
        assertEquals(line, firstShownBin(pane[0]), "Not hovering: the newest line is shown");
        assertEquals(KnockPane.SOURCE_LATEST, sourceChip(pane[0]).getText());

        JComponent spectrogram = pane[0].getSpectrogramComponentsForTests().get(0);
        SwingUtilities.invokeAndWait(() -> {
            spectrogram.setSize(800, 200);
            dispatch(spectrogram, MouseEvent.MOUSE_MOVED);
        });
        assertEquals(KnockPane.SOURCE_AT_CURSOR, sourceChip(pane[0]).getText());

        sendLine(line + 80);
        assertEquals(line, firstShownBin(pane[0]), "Hovering: new lines do not replace the inspected slice");

        SwingUtilities.invokeAndWait(() -> dispatch(spectrogram, MouseEvent.MOUSE_EXITED));
        assertEquals(KnockPane.SOURCE_LATEST, sourceChip(pane[0]).getText());

        sendLine(line + 160);
        assertEquals(line + 160, firstShownBin(pane[0]), "After leaving: live again");
    }

    /** One complete spectrum line: 16 packets of four bins, each byte {@code value + packet}. */
    private static void sendLine(int value) throws Exception {
        for (int i = 0; i < 16; i++) {
            long b = (value + i) & 0xFF;
            long raw = (b << 24) | (b << 16) | (b << 8) | b;
            SensorCentral.getInstance().setValue(raw, "m_knockSpectrum" + (i + 1));
        }
        // the line is applied to the plots on the EDT
        SwingUtilities.invokeAndWait(() -> { });
    }

    private static int firstShownBin(KnockPane pane) throws Exception {
        float[][] shown = new float[1][];
        SwingUtilities.invokeAndWait(() -> shown[0] = pane.getShownMagnitudesForTests());
        return (int) shown[0][0];
    }

    private static StatusChip sourceChip(KnockPane pane) {
        return (StatusChip) UiUtils.getAllComponents(pane.getContent()).stream()
            .filter(c -> c instanceof StatusChip && (KnockPane.SOURCE_LATEST.equals(((StatusChip) c).getText())
                || KnockPane.SOURCE_AT_CURSOR.equals(((StatusChip) c).getText())))
            .findFirst().orElseThrow(AssertionError::new);
    }

    private static void dispatch(JComponent target, int id) {
        // the middle of the plot: an old, still empty column
        target.dispatchEvent(new MouseEvent(target, id, System.currentTimeMillis(), 0, 400, 100, 0, false));
    }
}
