package com.rusefi.ui.widgets;

import com.formdev.flatlaf.FlatClientProperties;
import org.junit.jupiter.api.Test;

import javax.swing.*;

import static org.junit.jupiter.api.Assertions.*;

/** Gauges toolbar controls keep plain Swing behaviour under their new look. */
public class ToolbarControlsTest {
    @Test
    public void segmentsStayExclusiveAndKeepTheirWidth() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            JToggleButton grid = SegmentedControl.segment("Grid");
            JToggleButton free = SegmentedControl.segment("Free");
            ButtonGroup group = new ButtonGroup();
            group.add(grid);
            group.add(free);
            SegmentedControl control = new SegmentedControl(grid, free);
            assertEquals(2, control.getComponentCount());

            grid.setSelected(true);
            int freeWidth = free.getPreferredSize().width;
            assertTrue(grid.getFont().isBold());
            assertFalse(free.getFont().isBold());

            free.setSelected(true);
            assertFalse(grid.isSelected(), "the caller's ButtonGroup still makes the options exclusive");
            assertTrue(free.getFont().isBold());
            assertEquals(freeWidth, free.getPreferredSize().width, "selecting does not resize the control");
        });
    }

    @Test
    public void joinedGroupFollowsItsButtonsVisibilityAndSharedHeight() throws Exception {
        JButton add = new JButton("Add Gauge");
        JToggleButton edit = new JToggleButton("Edit Layout");
        JoinedButtons[] group = new JoinedButtons[1];
        SwingUtilities.invokeAndWait(() -> {
            group[0] = new JoinedButtons(add, edit);
            assertEquals(JoinedButtons.HEIGHT, group[0].getPreferredSize().height);
            add.setVisible(false);
            edit.setVisible(false);
        });
        // visibility changes arrive as queued component events
        SwingUtilities.invokeAndWait(() -> assertFalse(group[0].isVisible(), "no empty outline in Grid mode"));
        SwingUtilities.invokeAndWait(() -> edit.setVisible(true));
        SwingUtilities.invokeAndWait(() -> {
            assertTrue(group[0].isVisible());
            assertEquals(edit.getPreferredSize().width + 2, group[0].getPreferredSize().width,
                "hidden buttons take no space");
        });
    }

    @Test
    public void joinedButtonsKeepTheirActions() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            JButton save = new JButton("Save…");
            int[] clicks = new int[1];
            save.addActionListener(e -> clicks[0]++);
            JoinedButtons group = new JoinedButtons(save, new JButton("Load…"), new JButton("Reset"));
            assertEquals(3, group.getComponentCount());
            assertEquals(FlatClientProperties.BUTTON_TYPE_TOOLBAR_BUTTON, save.getClientProperty(FlatClientProperties.BUTTON_TYPE));
            save.doClick();
            assertEquals(1, clicks[0]);
        });
    }
}
