package com.rusefi.ui.wizard;

import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.Cursor;
import java.awt.event.MouseEvent;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.concurrent.atomic.AtomicInteger;

import static org.junit.jupiter.api.Assertions.*;

public class WizardProgressPanelTest {
    @Test
    public void onlyCompletedPillsSelectSteps() throws Exception {
        AtomicInteger selected = new AtomicInteger(-1);
        WizardProgressPanel panel = onEdt(() -> {
            WizardProgressPanel result = new WizardProgressPanel();
            result.setOnCompletedSelected(selected::set);
            result.setItems(Arrays.asList(
                item(3, WizardProgressPanel.State.COMPLETED),
                item(4, WizardProgressPanel.State.CURRENT),
                item(5, WizardProgressPanel.State.UPCOMING)));
            return result;
        });

        onEdt(() -> panel.pillsForTests().get(0).doClick());
        assertEquals(3, selected.get());
        assertEquals(SwingConstants.CENTER, panel.pillsForTests().get(0).getHorizontalAlignment());
        assertEquals(Cursor.HAND_CURSOR, panel.pillsForTests().get(0).getCursor().getType());
        onEdt(() -> panel.pillsForTests().get(1).doClick());
        assertEquals(3, selected.get());
        assertFalse(panel.pillsForTests().get(2).isEnabled());
    }

    @Test
    public void overflowButtonsStayWithinBoundsAndCurrentIsRevealed() throws Exception {
        WizardProgressPanel panel = createOverflowPanel(true);
        onEdt(() -> { });

        assertTrue(panel.nextForTests().isVisible());
        assertTrue(panel.viewportForTests().getViewPosition().x > 0, "current pill should be revealed");

        onEdt(() -> {
            for (int i = 0; i < 20; i++) panel.previousForTests().doClick();
        });
        assertEquals(0, panel.viewportForTests().getViewPosition().x);
        assertFalse(panel.previousForTests().isEnabled());
    }

    @Test
    public void dragScrollsTheStrip() throws Exception {
        WizardProgressPanel panel = createOverflowPanel(false);
        onEdt(() -> { });

        JViewport viewport = panel.viewportForTests();
        onEdt(() -> {
            long now = System.currentTimeMillis();
            viewport.dispatchEvent(new MouseEvent(viewport, MouseEvent.MOUSE_PRESSED, now, 0, 180, 20, 1, false));
            viewport.dispatchEvent(new MouseEvent(viewport, MouseEvent.MOUSE_DRAGGED, now + 1, 0, 40, 20, 0, false));
        });
        assertTrue(viewport.getViewPosition().x > 0);
    }

    @Test
    public void arrowsStayHiddenWhenContentFitsWithoutThem() throws Exception {
        WizardProgressPanel panel = onEdt(() -> {
            WizardProgressPanel result = new WizardProgressPanel();
            result.setItems(Arrays.asList(item(0, WizardProgressPanel.State.CURRENT)));
            int width = result.pillsForTests().get(0).getPreferredSize().width + 10;
            result.setSize(width, 70);
            result.doLayout();
            return result;
        });
        onEdt(() -> { });

        assertFalse(panel.previousForTests().isVisible());
        assertFalse(panel.nextForTests().isVisible());
    }

    private static WizardProgressPanel createOverflowPanel(boolean currentAtEnd) throws Exception {
        return onEdt(() -> {
            WizardProgressPanel result = new WizardProgressPanel();
            List<WizardProgressPanel.Item> items = new ArrayList<>();
            for (int i = 0; i < 8; i++) {
                WizardProgressPanel.State state = currentAtEnd && i == 7
                    ? WizardProgressPanel.State.CURRENT : WizardProgressPanel.State.COMPLETED;
                items.add(item(i, state));
            }
            result.setItems(items);
            result.setSize(330, 70);
            result.doLayout();
            result.viewportForTests().setSize(230, 60);
            result.viewportForTests().doLayout();
            return result;
        });
    }

    private static WizardProgressPanel.Item item(int index, WizardProgressPanel.State state) {
        return new WizardProgressPanel.Item(index, index + 1, "Step " + index, state);
    }

    private static void onEdt(Runnable runnable) throws Exception {
        SwingUtilities.invokeAndWait(runnable);
    }

    @SuppressWarnings("unchecked")
    private static <T> T onEdt(java.util.concurrent.Callable<T> callable) throws Exception {
        final Object[] result = new Object[1];
        SwingUtilities.invokeAndWait(() -> {
            try {
                result[0] = callable.call();
            } catch (Exception e) {
                throw new RuntimeException(e);
            }
        });
        return (T) result[0];
    }
}
