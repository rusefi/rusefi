package com.rusefi.ui.wizard;

import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.Component;
import java.awt.Container;
import java.awt.Rectangle;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

class DfuBoardSelectionPanelTest {
    @Test
    void cardTracksSeventyPercentOfViewAndStaysCentered() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            DfuBoardSelectionPanel panel = new DfuBoardSelectionPanel(ignored -> {});
            panel.setBoards(Arrays.asList("proteus_f7", "uaefi"), "uaefi");
            for (int[] size : new int[][]{{1920, 1080}, {3840, 2160}, {1280, 800}}) {
                panel.setSize(size[0], size[1]);
                layoutTree(panel);
                Container card = panel.confirmForTests().getParent().getParent();
                Rectangle actual = SwingUtilities.convertRectangle(card.getParent(), card.getBounds(), panel);
                int width = (int) Math.round(size[0] * 0.7);
                int height = (int) Math.round(size[1] * 0.7);
                Rectangle expected = new Rectangle((size[0] - width) / 2, (size[1] - height) / 2, width, height);
                assertEquals(expected, actual);
            }
        });
    }

    private static void layoutTree(Container container) {
        container.doLayout();
        for (Component child : container.getComponents()) {
            if (child instanceof Container) {
                layoutTree((Container) child);
            }
        }
    }

    @Test
    void loadingAndUnknownTargetRequireSelectionBeforeConfirming() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            List<String> chosen = new ArrayList<>();
            DfuBoardSelectionPanel panel = new DfuBoardSelectionPanel(chosen::add);
            assertFalse(panel.confirmForTests().isEnabled());
            assertTrue(panel.cancelForTests().isEnabled());
            panel.setBoards(Arrays.asList("proteus_f7", "uaefi"), "universal");
            assertNull(panel.boardsForTests().getSelectedValue());
            assertFalse(panel.confirmForTests().isEnabled());
            panel.boardsForTests().setSelectedValue("uaefi", true);
            panel.confirmForTests().doClick();
            panel.confirmForTests().doClick();
            assertEquals(Arrays.asList("uaefi"), chosen);
        });
    }

    @Test
    void rememberedBoardCanBeChangedUsingSearch() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            List<String> chosen = new ArrayList<>();
            DfuBoardSelectionPanel panel = new DfuBoardSelectionPanel(chosen::add);
            panel.setBoards(Arrays.asList("proteus_f7", "uaefi"), "proteus_f7");
            assertEquals("proteus_f7", panel.boardsForTests().getSelectedValue());
            assertTrue(chosen.isEmpty(), "a suggestion never confirms itself");
            panel.searchForTests().setText("UAEFI");
            assertEquals(1, panel.boardsForTests().getModel().getSize());
            assertFalse(panel.confirmForTests().isEnabled(), "filtering out the selected board must clear confirmation");
            panel.boardsForTests().setSelectedIndex(0);
            panel.confirmForTests().doClick();
            assertEquals(Arrays.asList("uaefi"), chosen);
        });
    }

    @Test
    void cancelDuringLoadingIgnoresLateCatalogAndEscapeOnlyCompletesOnce() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            List<String> chosen = new ArrayList<>();
            DfuBoardSelectionPanel panel = new DfuBoardSelectionPanel(chosen::add);
            panel.cancelForTests().doClick();
            panel.setBoards(Arrays.asList("uaefi"), "uaefi");
            panel.showLoadError("late failure");
            panel.getActionForKeyStroke(KeyStroke.getKeyStroke("ESCAPE")).actionPerformed(null);
            assertEquals(1, chosen.size());
            assertNull(chosen.get(0));
            assertFalse(panel.confirmForTests().isEnabled());
            assertEquals(0, panel.boardsForTests().getModel().getSize());
        });
    }

    @Test
    void catalogFailureStillAllowsReturningWithoutSelection() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            List<String> chosen = new ArrayList<>();
            DfuBoardSelectionPanel panel = new DfuBoardSelectionPanel(chosen::add);
            panel.showLoadError("Unavailable");
            assertFalse(panel.confirmForTests().isEnabled());
            panel.cancelForTests().doClick();
            assertEquals(1, chosen.size());
            assertNull(chosen.get(0));
        });
    }
}
