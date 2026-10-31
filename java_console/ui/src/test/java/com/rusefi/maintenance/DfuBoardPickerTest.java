package com.rusefi.maintenance;

import com.rusefi.ui.wizard.DfuBoardSelectionPanel;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.Component;
import java.awt.Container;
import java.util.Collections;
import java.util.concurrent.*;
import java.util.concurrent.atomic.AtomicInteger;

import static org.junit.jupiter.api.Assertions.*;

class DfuBoardPickerTest {
    @Test
    void confirmedChoiceClosesPanelBeforeResumingDfuWorker() throws Exception {
        ExecutorService executor = Executors.newSingleThreadExecutor();
        AtomicInteger closed = new AtomicInteger();
        try {
            Future<String> selected = executor.submit(() -> DfuBoardPicker.pick("uaefi", component -> {
                DfuBoardSelectionPanel panel = (DfuBoardSelectionPanel) component;
                JList<?> list = findList(panel);
                assertNotNull(list);
                assertEquals(1, list.getModel().getSize(), "local board_compatibility list must be ready before display");
                assertEquals("uaefi", list.getSelectedValue());
                clickConfirm(panel);
            }, closed::incrementAndGet, Collections.singletonList("uaefi")));
            assertEquals("uaefi", selected.get(5, TimeUnit.SECONDS));
            assertEquals(1, closed.get());
        } finally {
            executor.shutdownNow();
        }
    }

    private static void clickConfirm(Container container) {
        for (Component component : container.getComponents()) {
            if (component instanceof JButton && "Download and flash".equals(((JButton) component).getText())) {
                ((JButton) component).doClick();
            } else if (component instanceof Container) {
                clickConfirm((Container) component);
            }
        }
    }

    private static JList<?> findList(Container container) {
        for (Component component : container.getComponents()) {
            if (component instanceof JList) {
                return (JList<?>) component;
            }
            if (component instanceof Container) {
                JList<?> found = findList((Container) component);
                if (found != null) {
                    return found;
                }
            }
        }
        return null;
    }

    @Test
    void fullSizePanelCancellationReleasesWaitingJobAndClosesScreen() throws Exception {
        ExecutorService executor = Executors.newSingleThreadExecutor();
        AtomicInteger closed = new AtomicInteger();
        try {
            Future<String> selected = executor.submit(() -> DfuBoardPicker.pick("uaefi", component -> {
                assertTrue(SwingUtilities.isEventDispatchThread());
                assertInstanceOf(DfuBoardSelectionPanel.class, component);
                component.getActionForKeyStroke(KeyStroke.getKeyStroke("ESCAPE")).actionPerformed(null);
            }, () -> {
                assertTrue(SwingUtilities.isEventDispatchThread());
                closed.incrementAndGet();
            }, Collections.singletonList("uaefi")));
            assertNull(selected.get(5, TimeUnit.SECONDS));
            assertEquals(1, closed.get());
        } finally {
            executor.shutdownNow();
        }
    }
}
