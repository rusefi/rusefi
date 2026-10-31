package com.rusefi.ui.wizard;

import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.Component;
import java.awt.Container;
import java.awt.Rectangle;
import java.awt.event.MouseEvent;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

class DfuBoardSelectionPanelTest {
    @Test
    void doubleClickConfirmsSelectedBoardOnceLikeButton() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            List<String> chosen = new ArrayList<>();
            DfuBoardSelectionPanel panel = new DfuBoardSelectionPanel(chosen::add);
            panel.setBoards(Arrays.asList("proteus_f7", "uaefi"), "proteus_f7");
            JList<String> boards = panel.boardsForTests();
            boards.setSize(400, 300);
            // Swing's mouse-press handling selects the row before mouseClicked arrives.
            boards.setSelectedIndex(1);
            Rectangle row = boards.getCellBounds(1, 1);
            click(boards, row.x + 1, row.y + 1, 2, MouseEvent.BUTTON1);
            assertEquals(Arrays.asList("uaefi"), chosen);
            click(boards, row.x + 1, row.y + 1, 2, MouseEvent.BUTTON1);
            panel.confirmForTests().doClick();
            assertEquals(Arrays.asList("uaefi"), chosen);
        });
    }

    @Test
    void singleRightAndEmptySpaceClicksDoNotConfirm() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            List<String> chosen = new ArrayList<>();
            DfuBoardSelectionPanel panel = new DfuBoardSelectionPanel(chosen::add);
            JList<String> boards = panel.boardsForTests();
            boards.setSize(400, 300);
            click(boards, 1, 1, 2, MouseEvent.BUTTON1);
            assertTrue(chosen.isEmpty(), "loading list must not confirm");
            panel.setBoards(Arrays.asList("uaefi"), "uaefi");
            Rectangle row = boards.getCellBounds(0, 0);
            click(boards, 1, 1, 1, MouseEvent.BUTTON1);
            click(boards, 1, 1, 2, MouseEvent.BUTTON3);
            click(boards, 1, row.y + row.height + 1, 2, MouseEvent.BUTTON1);
            assertTrue(chosen.isEmpty(), "only a left double-click inside a row may confirm");
            panel.confirmForTests().doClick();
            assertEquals(Arrays.asList("uaefi"), chosen);
        });
    }

    @Test
    void doubleClickRespectsDisabledConfirmationAndCancellation() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            List<String> chosen = new ArrayList<>();
            DfuBoardSelectionPanel panel = new DfuBoardSelectionPanel(chosen::add);
            panel.setBoards(Arrays.asList("uaefi"), "uaefi");
            JList<String> boards = panel.boardsForTests();
            boards.setSize(400, 300);
            panel.showLoadError("Unavailable");
            click(boards, 1, 1, 2, MouseEvent.BUTTON1);
            assertTrue(chosen.isEmpty(), "double-click must respect the disabled button");
            panel.cancelForTests().doClick();
            click(boards, 1, 1, 2, MouseEvent.BUTTON1);
            assertEquals(1, chosen.size());
            assertNull(chosen.get(0));
        });
    }

    private static void click(JList<?> list, int x, int y, int count, int button) {
        list.dispatchEvent(new MouseEvent(list, MouseEvent.MOUSE_CLICKED,
            System.currentTimeMillis(), 0, x, y, count, false, button));
    }

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
