package com.rusefi.ui.wizard;

import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.util.concurrent.atomic.AtomicInteger;

import static org.junit.jupiter.api.Assertions.*;

class FirmwareProgrammingPanelTest {
    @Test
    void confirmationIsReadablePlainTextWithCancelFocusAndNoImplicitEnterConsent() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            String message = "Target <board> & channel\nSelected device: COM7";
            FirmwareProgrammingPanel panel = FirmwareProgrammingPanel.updateConfirmation(message, () -> {}, () -> {});
            assertEquals(message, panel.messageDetailsForTests().getText());
            assertFalse(panel.messageDetailsForTests().isEditable());
            assertNotNull(SwingUtilities.getAncestorOfClass(JScrollPane.class, panel.messageDetailsForTests()));
            assertSame(panel.backButtonForTests(), panel.getFocusTraversalPolicy().getDefaultComponent(panel));
            assertEquals("Cancel", panel.backButtonForTests().getText());
            JRootPane root = new JRootPane();
            root.setContentPane(panel);
            assertNull(root.getDefaultButton());
        });
    }

    @Test
    void confirmCancelAndEscapeAreMutuallyExclusiveOneShotActions() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            for (String first : new String[]{"confirm", "cancel", "escape"}) {
                AtomicInteger confirms = new AtomicInteger();
                AtomicInteger cancels = new AtomicInteger();
                FirmwareProgrammingPanel panel = FirmwareProgrammingPanel.updateConfirmation(
                    "Target details", confirms::incrementAndGet, cancels::incrementAndGet);
                JButton confirm = panel.confirmButtonForTests();
                assertNotNull(confirm);
                if (first.equals("confirm")) {
                    confirm.doClick();
                } else if (first.equals("cancel")) {
                    panel.backButtonForTests().doClick();
                } else {
                    escape(panel);
                }
                assertFalse(confirm.isEnabled());
                assertFalse(panel.backButtonForTests().isEnabled());
                confirm.setEnabled(true);
                confirm.doClick();
                panel.backButtonForTests().setEnabled(true);
                panel.backButtonForTests().doClick();
                escape(panel);
                assertEquals(first.equals("confirm") ? 1 : 0, confirms.get());
                assertEquals(first.equals("confirm") ? 0 : 1, cancels.get());
            }
        });
    }

    @Test
    void errorHasOnlyASafeOneShotBackActionAndEscape() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            AtomicInteger closes = new AtomicInteger();
            FirmwareProgrammingPanel panel = FirmwareProgrammingPanel.error(
                "Programming unavailable", "Readable <error> & details", closes::incrementAndGet);
            assertNull(panel.confirmButtonForTests());
            assertEquals("Back", panel.backButtonForTests().getText());
            assertEquals("Readable <error> & details", panel.messageDetailsForTests().getText());
            escape(panel);
            panel.backButtonForTests().setEnabled(true);
            panel.backButtonForTests().doClick();
            assertEquals(1, closes.get());
        });
    }

    @Test
    void hiddenOrRemovedConfirmationAndErrorCannotActOnTheNextScreen() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            for (boolean error : new boolean[]{false, true}) {
                for (boolean remove : new boolean[]{false, true}) {
                    AtomicInteger callbacks = new AtomicInteger();
                    FirmwareProgrammingPanel panel = error
                        ? FirmwareProgrammingPanel.error("Error", "Details", callbacks::incrementAndGet)
                        : FirmwareProgrammingPanel.updateConfirmation("Details", callbacks::incrementAndGet,
                            callbacks::incrementAndGet);
                    if (remove) {
                        panel.removeNotify();
                    } else {
                        panel.setVisible(false);
                    }
                    JButton confirm = panel.confirmButtonForTests();
                    if (confirm != null) {
                        confirm.setEnabled(true);
                        confirm.doClick();
                    }
                    panel.backButtonForTests().setEnabled(true);
                    panel.backButtonForTests().doClick();
                    escape(panel);
                    assertEquals(0, callbacks.get());
                }
            }
        });
    }

    private static void escape(JComponent panel) {
        Object key = panel.getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).get(KeyStroke.getKeyStroke("ESCAPE"));
        assertNotNull(key);
        panel.getActionMap().get(key).actionPerformed(new ActionEvent(panel, ActionEvent.ACTION_PERFORMED, "escape"));
    }
}
