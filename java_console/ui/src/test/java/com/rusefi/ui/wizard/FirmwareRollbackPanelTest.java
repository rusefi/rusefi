package com.rusefi.ui.wizard;

import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.event.MouseEvent;
import java.util.concurrent.atomic.AtomicInteger;

import static org.junit.jupiter.api.Assertions.assertEquals;

class FirmwareRollbackPanelTest {
    @Test
    void doubleClickConfirmsClickedBuild() throws Exception {
        AtomicInteger confirmedIndex = new AtomicInteger(-1);
        AtomicInteger confirmationCount = new AtomicInteger();
        FirmwareRollbackPanel[] panel = new FirmwareRollbackPanel[1];
        SwingUtilities.invokeAndWait(() -> panel[0] = new FirmwareRollbackPanel(
            true, new String[]{"newer", "older"}, index -> {
                confirmedIndex.set(index);
                confirmationCount.incrementAndGet();
            }, () -> { }));

        JToggleButton choice = panel[0].choiceForTests(1);
        SwingUtilities.invokeAndWait(() -> {
            long now = System.currentTimeMillis();
            choice.dispatchEvent(new MouseEvent(choice, MouseEvent.MOUSE_CLICKED,
                now, 0, 1, 1, 1, false, MouseEvent.BUTTON1));
            choice.dispatchEvent(new MouseEvent(choice, MouseEvent.MOUSE_CLICKED,
                now + 1, 0, 1, 1, 2, false, MouseEvent.BUTTON1));
        });

        assertEquals(1, confirmedIndex.get());
        assertEquals(1, confirmationCount.get());
    }
}
