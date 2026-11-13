package com.rusefi.ui.wizard;

import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;
import java.util.List;
import com.rusefi.ui.basic.UiHelper;

import static org.junit.jupiter.api.Assertions.*;

class FirmwareBranchSwitchPanelTest {
    @Test
    void limaDevChooserOffersOnlyDevWithNoReturnWarning() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            FirmwareBranchSwitchPanel panel = new FirmwareBranchSwitchPanel("Lima-Dev", ignored -> {}, () -> {});
            panel.setSize(800, 600);
            layoutRecursively(panel);
            assertEquals(2, buttons(panel).stream().filter(button -> !button.getText().isEmpty()).count());
            assertTrue(warning(panel).getText().contains("Development firmware is unstable"));
            assertTrue(warning(panel).getText().contains("Settings may need migration"));
            assertTrue(warning(panel).getText().contains("cannot return to Lima-Dev"));
        });
    }

    @Test
    void branchChoicesShareOneRow() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            FirmwareBranchSwitchPanel panel = new FirmwareBranchSwitchPanel("Lima", ignored -> {}, () -> {});
            panel.setSize(800, 600);
            layoutRecursively(panel);
            List<JButton> buttons = buttons(panel);
            Rectangle lima = SwingUtilities.convertRectangle(buttons.get(0).getParent(), buttons.get(0).getBounds(), panel);
            Rectangle dev = SwingUtilities.convertRectangle(buttons.get(1).getParent(), buttons.get(1).getBounds(), panel);
            assertEquals(lima.y, dev.y);
            assertTrue(dev.x > lima.getMaxX());
        });
    }

    @Test
    void wideWindowKeepsBranchActionsCompact() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            FirmwareBranchSwitchPanel panel = new FirmwareBranchSwitchPanel("Lima", ignored -> {}, () -> {});
            panel.setSize(1966, 1200);
            layoutRecursively(panel);
            List<JButton> buttons = buttons(panel);
            assertEquals(3, buttons.stream().filter(button -> !button.getText().isEmpty()).count());
            assertTrue(buttons.get(0).getWidth() < 700);
            assertTrue(buttons.get(0).getHeight() < 100);
            assertTrue(buttons.get(2).getWidth() < 200);
            assertTrue(buttons.get(0).getWidth() > 120);
            assertTrue(buttons.get(0).getHeight() > 24);
        });
    }

    @Test
    void warningFitsWhenWideFontWrapsBeyondFourLines() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            UIDefaults defaults = UIManager.getLookAndFeelDefaults();
            Object previousFont = defaults.get("Label.font");
            try {
                defaults.put("Label.font", new Font(Font.MONOSPACED, Font.PLAIN, 18));
                FirmwareBranchSwitchPanel panel = new FirmwareBranchSwitchPanel("Lima", ignored -> {}, () -> {});
                panel.setSize(480, 600);
                layoutRecursively(panel);
                JTextArea warning = warning(panel);
                try {
                    java.awt.geom.Rectangle2D end = warning.modelToView2D(warning.getDocument().getLength());
                    assertNotNull(end);
                    assertTrue(end.getMaxY() <= warning.getHeight(), "The entire warning must remain readable with wider fonts");
                } catch (javax.swing.text.BadLocationException error) {
                    throw new AssertionError(error);
                }
            } finally {
                defaults.put("Label.font", previousFont);
            }
        });
    }

    @Test
    void narrowWindowsKeepWarningsAndActionsReadableInBothThemes() throws Exception {
        checkNarrowWindowsInBothThemes("Lima");
        checkNarrowWindowsInBothThemes("Lima-Dev");
    }

    private static void checkNarrowWindowsInBothThemes(String sourceBranch) throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            LookAndFeel previous = UIManager.getLookAndFeel();
            boolean decoratedFrames = JFrame.isDefaultLookAndFeelDecorated();
            boolean decoratedDialogs = JDialog.isDefaultLookAndFeelDecorated();
            try {
                for (boolean dark : new boolean[]{false, true}) {
                    for (float scale : new float[]{1f, 1.5f}) {
                        UiHelper.configureLaf(dark);
                        Font font = UIManager.getLookAndFeelDefaults().getFont("Label.font");
                        UIManager.getLookAndFeelDefaults().put("Label.font", font.deriveFont(font.getSize2D() * scale));
                        UIManager.getLookAndFeelDefaults().put("Button.font", font.deriveFont(font.getSize2D() * scale));
                        for (int width : new int[]{480, 800, 1966}) {
                            FirmwareBranchSwitchPanel panel = new FirmwareBranchSwitchPanel(sourceBranch, ignored -> {}, () -> {});
                            panel.setSize(width, 600);
                            layoutRecursively(panel);
                            for (JButton button : buttons(panel)) {
                                if (!button.getText().isEmpty()) {
                                    Rectangle bounds = SwingUtilities.convertRectangle(button.getParent(), button.getBounds(), panel);
                                    assertTrue(bounds.x >= 0 && bounds.getMaxX() <= width,
                                        button.getText() + " must not require horizontal scrolling at " + width
                                            + " (dark=" + dark + ", scale=" + scale + ", bounds=" + bounds + ")");
                                    assertTrue(button.getHeight() > 24 && button.getHeight() < 100);
                                    for (Container parent = button.getParent(); !(parent instanceof JViewport);
                                         parent = parent.getParent()) {
                                        Rectangle local = SwingUtilities.convertRectangle(button.getParent(), button.getBounds(), parent);
                                        assertTrue(new Rectangle(0, 0, parent.getWidth(), parent.getHeight()).contains(local),
                                            button.getText() + " must not be clipped by " + parent.getClass().getSimpleName());
                                    }
                                }
                            }
                            JTextArea warning = warning(panel);
                            assertNotNull(warning);
                            assertTrue(warning.getParent().getParent().getWidth() <= 700,
                                "The card must stay bounded, including with larger fonts");
                            try {
                                java.awt.geom.Rectangle2D end = warning.modelToView2D(warning.getDocument().getLength());
                                assertNotNull(end);
                                assertTrue(end.getMaxY() <= warning.getHeight(),
                                    "The entire warning must remain readable (source=" + sourceBranch + ", dark=" + dark
                                        + ", scale=" + scale + ", width=" + width + ", font=" + warning.getFont()
                                        + ", neededHeight=" + end.getMaxY() + ", height=" + warning.getHeight() + ")");
                            } catch (javax.swing.text.BadLocationException error) {
                                throw new AssertionError(error);
                            }
                        }
                    }
                }
            } finally {
                UiHelper.unregisterThemeForUnitTest();
                try {
                    UIManager.setLookAndFeel(previous);
                } catch (UnsupportedLookAndFeelException error) {
                    throw new AssertionError(error);
                }
                JFrame.setDefaultLookAndFeelDecorated(decoratedFrames);
                JDialog.setDefaultLookAndFeelDecorated(decoratedDialogs);
            }
        });
    }

    @Test
    void resizingShortWindowKeepsWrappedChoicesInsideTheirContainer() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            FirmwareBranchSwitchPanel panel = new FirmwareBranchSwitchPanel("Lima", ignored -> {}, () -> {});
            for (int width = 700; width >= 450; width -= 10) {
                panel.setSize(width, 260);
                for (int pass = 0; pass < 3; pass++) {
                    layoutRecursively(panel);
                }
                for (JButton button : buttons(panel)) {
                    if (button.getText().startsWith("Switch to")) {
                        Container choices = button.getParent().getParent().getParent();
                        Rectangle local = SwingUtilities.convertRectangle(button.getParent(), button.getBounds(), choices);
                        assertTrue(new Rectangle(0, 0, choices.getWidth(), choices.getHeight()).contains(local),
                            button.getText() + " must remain inside the choices at width " + width);
                    }
                }
            }
        });
    }

    @Test
    void branchActionsAndEscapeKeepTheirCallbacks() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            List<String> branches = new ArrayList<>();
            int[] cancels = {0};
            FirmwareBranchSwitchPanel panel = new FirmwareBranchSwitchPanel("Lima", branches::add, () -> cancels[0]++);

            panel.clickLimaDevForTests();
            panel.clickDevelopmentForTests();
            panel.clickCancelForTests();
            Object escapeActionKey = panel.getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW)
                .get(KeyStroke.getKeyStroke("ESCAPE"));
            panel.getActionMap().get(escapeActionKey).actionPerformed(null);

            assertEquals(List.of("lts-26lima-dev", "development"), branches);
            assertEquals(2, cancels[0]);
        });
    }

    private static List<JButton> buttons(Container root) {
        List<JButton> result = new ArrayList<>();
        for (Component child : root.getComponents()) {
            if (child instanceof JButton) {
                result.add((JButton) child);
            } else if (child instanceof Container) {
                result.addAll(buttons((Container) child));
            }
        }
        return result;
    }

    private static JTextArea warning(Container root) {
        for (Component child : root.getComponents()) {
            if (child instanceof JTextArea) {
                return (JTextArea) child;
            }
            if (child instanceof Container) {
                JTextArea found = warning((Container) child);
                if (found != null) {
                    return found;
                }
            }
        }
        return null;
    }

    private static void layoutRecursively(Container root) {
        root.doLayout();
        for (Component child : root.getComponents()) {
            if (child instanceof Container) {
                layoutRecursively((Container) child);
            }
        }
    }
}
