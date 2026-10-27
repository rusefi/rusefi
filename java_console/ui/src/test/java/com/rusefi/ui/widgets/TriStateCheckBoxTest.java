package com.rusefi.ui.widgets;

import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import java.util.concurrent.atomic.AtomicInteger;

import static org.junit.jupiter.api.Assertions.*;

public class TriStateCheckBoxTest {
    @Test
    public void testInitialState() {
        TriStateCheckBox cbDefault = new TriStateCheckBox();
        assertEquals(TriStateCheckBox.State.UNCHECKED, cbDefault.getState());
        assertFalse(cbDefault.isSelected());
        assertFalse(cbDefault.isIndeterminate());

        TriStateCheckBox cbText = new TriStateCheckBox("Bus 1");
        assertEquals("Bus 1", cbText.getText());
        assertEquals(TriStateCheckBox.State.UNCHECKED, cbText.getState());

        TriStateCheckBox cbIndeterminate = new TriStateCheckBox("Bus 2", TriStateCheckBox.State.INDETERMINATE);
        assertEquals(TriStateCheckBox.State.INDETERMINATE, cbIndeterminate.getState());
        assertFalse(cbIndeterminate.isSelected());
        assertTrue(cbIndeterminate.isIndeterminate());

        TriStateCheckBox cbChecked = new TriStateCheckBox("Bus 3", TriStateCheckBox.State.CHECKED);
        assertEquals(TriStateCheckBox.State.CHECKED, cbChecked.getState());
        assertTrue(cbChecked.isSelected());
        assertFalse(cbChecked.isIndeterminate());
    }

    @Test
    public void testSetState() {
        TriStateCheckBox cb = new TriStateCheckBox();

        cb.setState(TriStateCheckBox.State.CHECKED);
        assertEquals(TriStateCheckBox.State.CHECKED, cb.getState());
        assertTrue(cb.isSelected());
        assertFalse(cb.isIndeterminate());

        cb.setState(TriStateCheckBox.State.INDETERMINATE);
        assertEquals(TriStateCheckBox.State.INDETERMINATE, cb.getState());
        assertFalse(cb.isSelected());
        assertTrue(cb.isIndeterminate());

        cb.setState(TriStateCheckBox.State.UNCHECKED);
        assertEquals(TriStateCheckBox.State.UNCHECKED, cb.getState());
        assertFalse(cb.isSelected());
        assertFalse(cb.isIndeterminate());

        cb.setSelected(true);
        assertEquals(TriStateCheckBox.State.CHECKED, cb.getState());
        assertTrue(cb.isSelected());

        cb.setSelected(false);
        assertEquals(TriStateCheckBox.State.UNCHECKED, cb.getState());
        assertFalse(cb.isSelected());
    }

    @Test
    public void testClickTransitions() {
        TriStateCheckBox cb = new TriStateCheckBox("Test");
        AtomicInteger actionCount = new AtomicInteger();
        cb.addActionListener(e -> actionCount.incrementAndGet());

        // UNCHECKED -> doClick() -> CHECKED
        cb.setState(TriStateCheckBox.State.UNCHECKED);
        cb.doClick();
        assertEquals(TriStateCheckBox.State.CHECKED, cb.getState());
        assertTrue(cb.isSelected());
        assertEquals(1, actionCount.get());

        // CHECKED -> doClick() -> UNCHECKED
        cb.doClick();
        assertEquals(TriStateCheckBox.State.UNCHECKED, cb.getState());
        assertFalse(cb.isSelected());
        assertEquals(2, actionCount.get());

        // INDETERMINATE -> doClick() -> CHECKED
        cb.setState(TriStateCheckBox.State.INDETERMINATE);
        cb.doClick();
        assertEquals(TriStateCheckBox.State.CHECKED, cb.getState());
        assertTrue(cb.isSelected());
        assertEquals(3, actionCount.get());
    }

    @Test
    public void testRendering() {
        TriStateCheckBox cb = new TriStateCheckBox("Bus 1");
        cb.setSize(120, 30);

        BufferedImage img = new BufferedImage(120, 30, BufferedImage.TYPE_INT_ARGB);
        Graphics2D g2 = img.createGraphics();

        for (TriStateCheckBox.State state : TriStateCheckBox.State.values()) {
            cb.setState(state);
            cb.setEnabled(true);
            cb.paint(g2);

            cb.setEnabled(false);
            cb.paint(g2);
        }

        g2.dispose();
    }
}
