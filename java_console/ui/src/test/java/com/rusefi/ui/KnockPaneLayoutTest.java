package com.rusefi.ui;

import com.rusefi.ts_plugin.knock.KnockPlotStyle;
import com.rusefi.ui.util.UiUtils;
import com.rusefi.ui.widgets.JoinedButtons;
import com.rusefi.ui.widgets.SegmentedControl;
import com.rusefi.ui.widgets.StatusChip;
import com.rusefi.ui.widgets.ToolbarStrip;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.Component;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

class KnockPaneLayoutTest {
    @Test
    void toolbarHoldsCaptureResetAndViewAndTheChipFollowsTheState() throws Throwable {
        SwingUtilities.invokeAndWait(() -> {
            KnockPane pane = new KnockPane(new UIContext());
            List<Component> all = UiUtils.getAllComponents(pane.getContent());

            assertFalse(all.stream().anyMatch(c -> c instanceof JTabbedPane), "The view is a segmented control now");
            JButton capture = button(all, KnockPane.START_CAPTURE);
            JButton reset = button(all, KnockPane.RESET_PEAK);
            assertTrue(capture.getParent() instanceof JoinedButtons);
            assertTrue(reset.getParent() instanceof JoinedButtons);
            assertNotNull(capture.getToolTipText(), "Explains that capture changes an ECU setting");
            assertFalse(capture.isEnabled(), "Nothing to capture without an ECU");
            assertFalse(reset.isEnabled());
            for (String view : new String[]{KnockPane.VIEW_ALL, KnockPane.VIEW_SENSORS, KnockPane.VIEW_CYLINDERS}) {
                AbstractButton segment = button(all, view);
                assertTrue(segment.getParent() instanceof SegmentedControl);
                assertTrue(SwingUtilities.getAncestorOfClass(ToolbarStrip.class, segment) != null);
            }
            assertTrue(button(all, KnockPane.VIEW_ALL).isSelected());
            AbstractButton light = button(all, KnockPane.PLOT_LIGHT);
            AbstractButton dark = button(all, KnockPane.PLOT_DARK);
            assertTrue(light.getParent() instanceof SegmentedControl);
            assertSame(light.getParent(), dark.getParent());
            assertTrue(light.isSelected() != dark.isSelected(), "One plot theme is chosen");
            assertTrue(all.stream().anyMatch(c -> c instanceof JLabel && KnockPane.MAGNITUDES.equals(((JLabel) c).getText())));

            StatusChip chip = (StatusChip) all.stream().filter(c -> c instanceof StatusChip)
                .findFirst().orElseThrow(AssertionError::new);
            assertEquals(KnockPane.STATUS_NOT_CONNECTED, chip.getText());
            assertEquals(StatusChip.Tone.BAD, chip.getTone());

            pane.simulateConnected(4);
            assertEquals(KnockPane.STATUS_CAPTURING, chip.getText());
            assertEquals(StatusChip.Tone.GOOD, chip.getTone());
            assertEquals(KnockPane.STOP_CAPTURE, capture.getText());
            assertTrue(capture.isEnabled());
            assertTrue(reset.isEnabled());

            pane.setStartState(false);
            assertEquals(KnockPane.STATUS_STOPPED, chip.getText());
            assertEquals(KnockPane.START_CAPTURE, capture.getText());
        });
    }

    /** The plots follow the Log Viewer's light and dark palettes, in the Log Viewer layout. */
    @Test
    void plotStylesUseTheLogViewerPalettes() {
        KnockPlotStyle light = KnockPane.plotStyle(false);
        KnockPlotStyle dark = KnockPane.plotStyle(true);
        assertTrue(light.logViewerLayout);
        assertTrue(dark.logViewerLayout);
        assertNotNull(light.valueFontFamily, "Values use the console's monospaced font");
        assertEquals(new java.awt.Color(0xffffff), new java.awt.Color(light.background.getRGB()));
        assertEquals(new java.awt.Color(0x151b26), new java.awt.Color(dark.background.getRGB()));
        assertNotEquals(light.text, dark.text, "Text stays readable on each background");
    }

    private static <T extends AbstractButton> T button(List<Component> all, String text) {
        @SuppressWarnings("unchecked")
        T found = (T) all.stream()
            .filter(c -> c instanceof AbstractButton && text.equals(((AbstractButton) c).getText()))
            .findFirst().orElseThrow(() -> new AssertionError("No button " + text));
        return found;
    }
}
