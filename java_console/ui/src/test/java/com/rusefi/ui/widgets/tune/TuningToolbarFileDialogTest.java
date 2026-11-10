package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModelMocks;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.basic.UiHelper;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.Timeout;

import javax.swing.*;
import java.awt.*;
import java.awt.event.AWTEventListener;
import java.awt.event.WindowEvent;
import java.awt.event.KeyEvent;
import java.awt.event.InputEvent;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicReference;

import static org.junit.jupiter.api.Assertions.*;
import static org.junit.jupiter.api.Assumptions.assumeFalse;
import static org.junit.jupiter.api.Assumptions.assumeTrue;

/** Real Swing dialogs; run with a desktop or Xvfb. No ECU or tune writes. */
@Timeout(20)
class TuningToolbarFileDialogTest {
    private LookAndFeel previousLookAndFeel;
    private boolean frameDecorated;
    private boolean dialogDecorated;

    @BeforeEach
    void configureConsoleTheme() throws Exception {
        assumeFalse(GraphicsEnvironment.isHeadless(), "Requires a desktop or Xvfb");
        SwingUtilities.invokeAndWait(() -> {
            previousLookAndFeel = UIManager.getLookAndFeel();
            frameDecorated = JFrame.isDefaultLookAndFeelDecorated();
            dialogDecorated = JDialog.isDefaultLookAndFeelDecorated();
            UiHelper.configureLaf(false);
        });
    }

    @AfterEach
    void restoreTheme() throws Exception {
        if (previousLookAndFeel != null) {
            SwingUtilities.invokeAndWait(() -> {
                UiHelper.unregisterThemeForUnitTest();
                assertDoesNotThrow(() -> UIManager.setLookAndFeel(previousLookAndFeel));
                JFrame.setDefaultLookAndFeelDecorated(frameDecorated);
                JDialog.setDefaultLookAndFeelDecorated(dialogDecorated);
            });
        }
    }

    @Test
    void loadTuneChooserOwnership() throws Exception {
        checkChooser(false, false);
    }

    @Test
    void loadTuneChooserOwnershipFromAnotherTab() throws Exception {
        checkChooser(false, true);
    }

    @Test
    void saveAsChooserOwnershipFromAnotherTab() throws Exception {
        checkChooser(true, true);
    }

    @Test
    void loadTuneBeforeTuningTabIsAttached() throws Exception {
        checkChooser(false, true, false);
    }

    @Test
    void saveAsBeforeTuningTabIsAttached() throws Exception {
        checkChooser(true, true, false);
    }

    // Opt in on an isolated desktop with a window manager and another application at (900, 650).
    @Test
    void loadTuneBeforeTuningTabIsAttachedAfterAltTab() throws Exception {
        assumeTrue(Boolean.getBoolean("rusefi.test.altTab"));
        checkChooser(false, true, false, true);
    }

    @Test
    void saveAsBeforeTuningTabIsAttachedAfterAltTab() throws Exception {
        assumeTrue(Boolean.getBoolean("rusefi.test.altTab"));
        checkChooser(true, true, false, true);
    }

    private void checkChooser(boolean save, boolean otherTab) throws Exception {
        checkChooser(save, otherTab, true);
    }

    private void checkChooser(boolean save, boolean otherTab, boolean attached) throws Exception {
        checkChooser(save, otherTab, attached, false);
    }

    private void checkChooser(boolean save, boolean otherTab, boolean attached, boolean switchWindows) throws Exception {
        assumeFalse(GraphicsEnvironment.isHeadless(), "Requires a desktop or Xvfb");
        SwingUtilities.invokeAndWait(() -> {
            UIContext context = new UIContext();
            context.iniFileState.setIniFileModelForTest(IniFileModelMocks.withGauges());
            CalibrationDialogWidget calibration = new CalibrationDialogWidget(context);
            ConfigurationImage image = new ConfigurationImage(16);
            AtomicBoolean loadStarted = new AtomicBoolean();
            TuningToolbarWidget toolbar = new TuningToolbarWidget(context, calibration,
                new AtomicReference<>(), new AtomicReference<>(image), image.clone(),
                UpdateOperationCallbacks.DUMMY, () -> loadStarted.set(true), failed -> { });
            JFrame frame = new JFrame("Tune dialog ownership test");
            if (!attached) {
                toolbar.setDialogOwner(frame);
            }
            JTabbedPane tabs = new JTabbedPane();
            // File menu actions may build Tuning before its lazy tab has ever painted.
            tabs.addTab("Tuning", attached ? toolbar.getPanel() : new JPanel());
            tabs.addTab("Gauges", new JPanel());
            tabs.setSelectedIndex(otherTab ? 1 : 0);
            frame.add(tabs);
            JMenuItem item = new JMenuItem(save ? toolbar.getSaveTuneAsAction() : toolbar.getLoadTuneAction());
            JMenu menu = new JMenu("File");
            menu.add(item);
            JMenuBar bar = new JMenuBar();
            bar.add(menu);
            frame.setJMenuBar(bar);
            frame.setSize(600, 300);

            AtomicReference<JDialog> opened = new AtomicReference<>();
            AtomicBoolean timedOut = new AtomicBoolean();
            AtomicReference<Throwable> focusFailure = new AtomicReference<>();
            AWTEventListener listener = event -> {
                if (event.getID() != WindowEvent.WINDOW_OPENED || !(event.getSource() instanceof JDialog)) {
                    return;
                }
                JDialog dialog = (JDialog) event.getSource();
                JFileChooser chooser = findChooser(dialog);
                if (chooser != null) {
                    opened.set(dialog);
                    if (switchWindows) {
                        new Thread(() -> {
                            try {
                                Robot robot = new Robot();
                                robot.setAutoDelay(60);
                                robot.waitForIdle();
                                robot.delay(300);
                                robot.mouseMove(900, 650);
                                robot.mousePress(InputEvent.BUTTON1_DOWN_MASK);
                                robot.mouseRelease(InputEvent.BUTTON1_DOWN_MASK);
                                robot.delay(500);
                                SwingUtilities.invokeAndWait(() -> assertNull(
                                    KeyboardFocusManager.getCurrentKeyboardFocusManager().getFocusedWindow(),
                                    "Switching away must focus the other application"));
                                altTab(robot);
                                SwingUtilities.invokeAndWait(() -> assertSame(dialog,
                                    KeyboardFocusManager.getCurrentKeyboardFocusManager().getFocusedWindow(),
                                    "Returning to the console must focus the modal chooser"));
                            } catch (Throwable failure) {
                                focusFailure.set(failure);
                            } finally {
                                SwingUtilities.invokeLater(chooser::cancelSelection);
                            }
                        }, "chooser-alt-tab-test").start();
                    } else {
                        // Cancel through the real chooser so the modal event loop exits normally.
                        SwingUtilities.invokeLater(chooser::cancelSelection);
                    }
                }
            };
            Timer watchdog = new Timer(5000, event -> {
                timedOut.set(true);
                for (Window window : Window.getWindows()) {
                    if (window instanceof JDialog && findChooser((Container) window) != null) {
                        window.dispose();
                    }
                }
            });
            watchdog.setRepeats(false);
            Toolkit.getDefaultToolkit().addAWTEventListener(listener, AWTEvent.WINDOW_EVENT_MASK);
            try {
                frame.setVisible(true);
                watchdog.start();
                item.doClick();
                assertFalse(timedOut.get(), "Chooser must close normally on cancel");
                if (focusFailure.get() != null) {
                    throw new AssertionError("Native window switching failed", focusFailure.get());
                }
                JDialog dialog = opened.get();
                assertNotNull(dialog, "The menu action opens a real file chooser");
                assertTrue(dialog.isModal());
                assertSame(frame, dialog.getOwner(), "The modal chooser must belong to the console window");
                assertFalse(dialog.isShowing());
                assertFalse(loadStarted.get(), "Cancel must not start a load");
                assertNull(context.getTuneFilePath(), "Cancel must not change the current tune");
            } finally {
                watchdog.stop();
                Toolkit.getDefaultToolkit().removeAWTEventListener(listener);
                if (opened.get() != null) {
                    opened.get().dispose();
                }
                frame.dispose();
                toolbar.onDisconnect();
                calibration.destroy();
            }
        });
    }

    private static void altTab(Robot robot) {
        robot.keyPress(KeyEvent.VK_ALT);
        robot.keyPress(KeyEvent.VK_TAB);
        robot.keyRelease(KeyEvent.VK_TAB);
        robot.keyRelease(KeyEvent.VK_ALT);
        robot.delay(500);
        robot.waitForIdle();
    }

    private static JFileChooser findChooser(Container container) {
        for (Component child : container.getComponents()) {
            if (child instanceof JFileChooser) {
                return (JFileChooser) child;
            }
            if (child instanceof Container) {
                JFileChooser found = findChooser((Container) child);
                if (found != null) {
                    return found;
                }
            }
        }
        return null;
    }
}
