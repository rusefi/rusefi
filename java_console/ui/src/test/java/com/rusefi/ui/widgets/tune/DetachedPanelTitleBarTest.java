package com.rusefi.ui.widgets.tune;

import com.formdev.flatlaf.FlatLightLaf;
import com.formdev.flatlaf.ui.FlatNativeWindowBorder;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;

import static org.junit.jupiter.api.Assertions.*;
import static org.junit.jupiter.api.Assumptions.assumeFalse;

/** Detached calibration windows keep the split button and stay resizable. */
class DetachedPanelTitleBarTest {
    @Test
    void flatLafDialogIsResizableWithSplitButton() throws Exception {
        // Real windows need a display; CI runs headless.
        assumeFalse(GraphicsEnvironment.isHeadless());
        LookAndFeel previous = UIManager.getLookAndFeel();
        SwingUtilities.invokeAndWait(() -> {
            JDialog dialog = null;
            try {
                UIManager.setLookAndFeel(new FlatLightLaf());
                dialog = new JDialog((Window) null, "Panel", Dialog.ModalityType.MODELESS);
                DetachedPanelTitleBar.install(dialog, () -> { }, true);
                dialog.add(new JLabel("content"));
                dialog.pack();

                assertTrue(dialog.isResizable());
                assertEquals(JRootPane.PLAIN_DIALOG, dialog.getRootPane().getWindowDecorationStyle());
                if (FlatNativeWindowBorder.isSupported()) {
                    // The native border provides resizing; it is not available for undecorated dialogs.
                    assertFalse(dialog.isUndecorated());
                    assertTrue(FlatNativeWindowBorder.hasCustomDecoration(dialog));
                } else {
                    // FlatLaf's Swing resizer works on undecorated dialogs.
                    assertTrue(dialog.isUndecorated());
                }
                assertNotNull(findButton(dialog.getRootPane(), "Split into separate panels"),
                    "The title bar keeps the split action");
            } catch (UnsupportedLookAndFeelException e) {
                fail(e);
            } finally {
                if (dialog != null) {
                    dialog.dispose();
                }
                try {
                    UIManager.setLookAndFeel(previous);
                } catch (UnsupportedLookAndFeelException ignored) {
                }
            }
        });
    }

    private static JButton findButton(Container container, String toolTip) {
        for (Component component : container.getComponents()) {
            if (component instanceof JButton && toolTip.equals(((JButton) component).getToolTipText())) {
                return (JButton) component;
            }
            if (component instanceof Container) {
                JButton found = findButton((Container) component, toolTip);
                if (found != null) {
                    return found;
                }
            }
        }
        return null;
    }
}
