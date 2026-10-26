package com.rusefi.ui.widgets.tune;

import com.formdev.flatlaf.FlatLightLaf;
import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.IniFileMetaInfo;
import com.opensr5.ini.IniFileMetaInfoImpl;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.RawIniFile;
import com.rusefi.core.preferences.storage.Node;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.util.UiUtils;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.awt.event.InputEvent;
import java.awt.event.MouseEvent;
import java.awt.event.WindowEvent;
import java.beans.XMLDecoder;
import java.beans.XMLEncoder;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.InputStream;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicReference;
import java.util.function.Consumer;

import static org.junit.jupiter.api.Assertions.*;
import static org.junit.jupiter.api.Assumptions.assumeFalse;
import static org.mockito.Mockito.*;

class CalibrationPanelWorkspaceTest {
    @Test
    void softwareKnockSplitsOneLayoutLevelAtATime() throws Throwable {
        IniFileModel ini = exampleIni();
        SwingUtilities.invokeAndWait(() -> {
            Node config = new Node();
            try (Fixture fixture = new Fixture(ini, config)) {
                fixture.primary.update("softwareKnock", ini, new ConfigurationImage(ini.getMetaInfo().getPageSize(0)));
                fixture.workspace.detachCurrentPanel();
                FakeWindow original = fixture.windows.get(0);
                assertTrue(original.canSplit);
                original.move(new Rectangle(120, 100, 900, 700));
                original.split.run();
                assertTrue(original.disposed);
                assertEquals(3, fixture.windows.size());
                assertEquals("softwareKnockLeft", config.getChild("panel0").getProperty("key"));
                assertEquals("knockThresholdCurve", config.getChild("panel1").getProperty("key"));
                FakeWindow left = fixture.windows.get(1);
                assertEquals("Software Knock — Left", left.title);
                assertEquals(120, left.bounds.x);
                assertTrue(left.bounds.y <= 100, "Keep a tall child panel within the available screen");
                assertTrue(new Rectangle(0, 0, 1280, 960).contains(left.bounds));
                assertTrue(left.canSplit);
                assertFalse(fixture.windows.get(2).canSplit);
                left.split.run();
                assertTrue(left.disposed);
                assertEquals(3, config.getIntProperty("count", 0));
                FakeWindow sense = fixture.windows.get(3);
                FakeWindow response = fixture.windows.get(4);
                assertEquals("Sense", sense.title);
                assertEquals("Response", response.title);
                assertTrue(sense.canSplit, "Sense has its own controls and a nested cylinder-mapping panel");
                assertFalse(response.canSplit);
                find(sense.content, JCheckBox.class).doClick();
                find(sense.content, JTextField.class).setText("92");
                assertEquals("92.0", ConfigurationImageGetterSetter.getStringValue(
                    ini.findIniField("cylinderBore").get(), fixture.primary.getWorkingImage()));
            }
            try (Fixture restored = new Fixture(ini, roundTrip(config))) {
                restored.primary.update("softwareKnock", ini, new ConfigurationImage(ini.getMetaInfo().getPageSize(0)));
                restored.workspace.restoreWindows();
                assertEquals(3, restored.windows.size());
                assertTrue(restored.windows.stream().noneMatch(window -> "Software Knock".equals(window.title)));
            }
        });
    }

    @Test
    void splitPreservesParentControlsReusesOpenChildrenAndRestoresGeneralWindow() throws Throwable {
        IniFileModel ini = ini();
        SwingUtilities.invokeAndWait(() -> {
            Node config = new Node();
            try (Fixture fixture = new Fixture(ini, config)) {
                fixture.select("first");
                fixture.workspace.detachCurrentPanel();
                FakeWindow first = fixture.windows.get(0);
                fixture.select("combined");
                fixture.workspace.detachCurrentPanel();
                FakeWindow combined = fixture.windows.get(1);
                combined.split.run();
                assertTrue(combined.disposed);
                assertFalse(first.disposed);
                assertEquals(4, fixture.windows.size(), "Reuse the already-open first panel");
                assertEquals(3, config.getIntProperty("count", 0));
                FakeWindow general = fixture.windows.get(2);
                assertEquals("Combined — General", general.title);
                assertFalse(general.canSplit);
                assertNull(find(general.content, JTable.class), "General excludes the child curve");
                find(general.content, JTextField.class).setText("44");
                assertEquals("44", find(first.content, JTextField.class).getText());
                assertEquals("44", find(fixture.primary.getContentPane(), JTextField.class).getText());
                assertTrue(config.getChild("panel1").getBoolProperty("ownContent"));
            }
            try (Fixture restored = new Fixture(ini, roundTrip(config))) {
                restored.workspace.restoreWindows();
                assertEquals(3, restored.windows.size());
                FakeWindow general = restored.windows.get(1);
                assertEquals("Combined — General", general.title);
                assertFalse(general.canSplit);
                assertNull(find(general.content, JTable.class));
            }
        });
    }

    @Test
    void splitCommitsAnActiveTableEditorBeforeReplacingItsWindow() throws Throwable {
        IniFileModel ini = ini();
        SwingUtilities.invokeAndWait(() -> {
            try (Fixture fixture = new Fixture(ini, new Node())) {
                fixture.select("charts");
                fixture.workspace.detachCurrentPanel();
                FakeWindow original = fixture.windows.get(0);
                JTable table = find(original.content, JTable.class);
                table.changeSelection(0, 1, false, false);
                assertTrue(table.editCellAt(0, 1));
                ((JTextField) table.getEditorComponent()).setText("72");
                original.split.run();
                assertTrue(original.disposed);
                assertEquals("72", find(fixture.windows.get(1).content, JTable.class).getValueAt(0, 1).toString());
                assertEquals("72", find(fixture.primary.getContentPane(), JTable.class).getValueAt(0, 1).toString());
            }
        });
    }

    @Test
    void leavesAndUnresolvableLayoutsCannotSplit() throws Throwable {
        IniFileModel ini = ini();
        assertTrue(new CalibrationPanelKey("first").splitParts(ini).isEmpty());
        assertTrue(new CalibrationPanelKey("curve").splitParts(ini).isEmpty());
        assertTrue(new CalibrationPanelKey("combined", true).splitParts(ini).isEmpty());
        assertTrue(new CalibrationPanelKey("incomplete").splitParts(ini).isEmpty());
        assertTrue(new CalibrationPanelKey("recursive").splitParts(ini).isEmpty());
        assertEquals(2, new CalibrationPanelKey("charts").splitParts(ini).size());
    }

    @Test
    void multisparkDetachesAtItsFormCornerAndPreferredSize() throws Throwable {
        IniFileModel ini = exampleIni();
        SwingUtilities.invokeAndWait(() -> {
            try (Fixture fixture = new Fixture(ini, new Node())) {
                fixture.primary.update("multisparkSettings", ini,
                    new ConfigurationImage(ini.getMetaInfo().getPageSize(0)));
                DetachPanelLayer ui = (DetachPanelLayer) ((JLayer<?>) fixture.workspace.getContent()).getUI();
                JLayer<JScrollPane> layer = ui.getContent();
                layer.setSize(1500, 900);
                layout(layer);
                // The form's deferred wrapping/layout pass is also used when shown on a desktop.
                layout(layer);
                JComponent form = fixture.primary.getPanelFrame();
                Rectangle bounds = SwingUtilities.convertRectangle(form, new Rectangle(form.getSize()), layer);
                assertTrue(bounds.width < 1500);
                assertTrue(bounds.height < 900);
                layer.getUI().eventDispatched(new MouseEvent(form, MouseEvent.MOUSE_ENTERED,
                    0, 0, 20, 20, 0, false), layer);
                JButton detach = find(layer.getGlassPane(), JButton.class);
                assertTrue(detach.isVisible());
                assertEquals(bounds.x + bounds.width - 34, detach.getX());
                assertEquals(bounds.y + 6, detach.getY());
                detach.doClick();
                FakeWindow window = fixture.windows.get(0);
                assertEquals("Multispark", window.title);
                assertEquals(window.content.getPreferredSize(), window.bounds.getSize());
                assertTrue(window.bounds.width < 1500);
                assertTrue(window.bounds.height < 900);
            }
        });
    }

    private static void layout(Container container) {
        container.doLayout();
        for (Component child : container.getComponents()) {
            if (child instanceof Container) {
                layout((Container) child);
            }
        }
    }

    @Test
    void freshDetachedWindowFitsCompactContent() throws Throwable {
        IniFileModel ini = ini();
        SwingUtilities.invokeAndWait(() -> {
            try (Fixture fixture = new Fixture(ini, new Node())) {
                fixture.select("first");
                fixture.workspace.getContent().setSize(1000, 800);
                fixture.workspace.getContent().doLayout();
                fixture.workspace.detachCurrentPanel();
                FakeWindow window = fixture.windows.get(0);
                assertEquals(window.content.getPreferredSize(), window.bounds.getSize());
                assertTrue(window.bounds.width < 1000);
                assertTrue(window.bounds.height < 400, "A compact form must not inherit the tile height");
            }
        });
    }

    @Test
    void newWindowOpensOnOwnersScreen() throws Throwable {
        IniFileModel ini = ini();
        List<Rectangle> screens = Arrays.asList(
            new Rectangle(0, 0, 1920, 1040), new Rectangle(-1280, 0, 1280, 984));
        SwingUtilities.invokeAndWait(() -> {
            try (Fixture fixture = new Fixture(ini, new Node(), screens)) {
                Window owner = mock(Window.class);
                when(owner.getBounds()).thenReturn(new Rectangle(-1100, 100, 900, 700));
                fixture.workspace.restore(owner);
                fixture.select("first");
                fixture.workspace.detachCurrentPanel();
                assertEquals(new Point(-1240, 40), fixture.windows.get(0).bounds.getLocation(),
                    "A fresh detached window should open on the owner's display");
            }
        });
    }

    @Test
    void nativeWindowsHoverEditAndRestore() throws Throwable {
        checkNativeWindows(false);
    }

    @Test
    void nativeWindowsWithConsoleTheme() throws Throwable {
        checkNativeWindows(true);
    }

    @Test
    void nativeTitleBarSplitsIntoCompactWindowsAndStillCloses() throws Throwable {
        assumeFalse(GraphicsEnvironment.isHeadless());
        IniFileModel ini = ini();
        Node config = new Node();
        JFrame[] owner = new JFrame[1];
        CalibrationPanelWorkspace[] workspace = new CalibrationPanelWorkspace[1];
        JDialog[] original = new JDialog[1];
        LookAndFeel previous = UIManager.getLookAndFeel();
        try {
            SwingUtilities.invokeAndWait(() -> {
                FlatLightLaf.setup();
                owner[0] = new JFrame("Split workspace test");
                workspace[0] = new CalibrationPanelWorkspace(new UIContext(), config);
                workspace[0].getPrimary().update("combined", ini, initialImage());
                owner[0].add(workspace[0].getContent());
                owner[0].setBounds(30, 30, 700, 450);
                owner[0].setVisible(true);
                workspace[0].detachCurrentPanel();
                original[0] = (JDialog) Arrays.stream(owner[0].getOwnedWindows())
                    .filter(Window::isShowing).findFirst().get();
                original[0].setBounds(100, 70, 1100, 850);
            });
            Robot robot = new Robot();
            robot.setAutoDelay(60);
            robot.setAutoWaitForIdle(true);
            robot.waitForIdle();
            Point[] point = new Point[1];
            SwingUtilities.invokeAndWait(() -> {
                JButton split = namedButton(original[0], "Split panel");
                JButton close = namedButton(original[0], "Close");
                assertNotNull(split);
                assertNotNull(close);
                assertTrue(split.isShowing());
                assertSame(close.getParent(), split.getParent());
                assertEquals(close.getX(), split.getX() + split.getWidth(), "Split sits immediately beside Close");
                assertNotNull(split.getIcon());
                assertTrue(split.getText() == null || split.getText().isEmpty());
                point[0] = split.getLocationOnScreen();
                point[0].translate(split.getWidth() / 2, split.getHeight() / 2);
            });
            robot.mouseMove(point[0].x, point[0].y);
            robot.mousePress(InputEvent.BUTTON1_DOWN_MASK);
            robot.mouseRelease(InputEvent.BUTTON1_DOWN_MASK);
            SwingUtilities.invokeAndWait(() -> {
                assertFalse(original[0].isDisplayable());
                Window[] children = Arrays.stream(owner[0].getOwnedWindows()).filter(Window::isShowing)
                    .toArray(Window[]::new);
                assertEquals(3, children.length);
                assertEquals(3, config.getIntProperty("count", 0));
                for (Window child : children) {
                    assertTrue(child.getWidth() < 1100);
                    assertTrue(child.getHeight() < 850);
                    assertFalse(namedButton(child, "Split panel").isShowing(), "These child panels are leaves");
                }
                JDialog general = (JDialog) Arrays.stream(children)
                    .filter(child -> "Combined — General".equals(((JDialog) child).getTitle())).findFirst().get();
                find(general, JTextField.class).setText("48");
                assertEquals("48", find(workspace[0].getPrimary().getContentPane(), JTextField.class).getText());
                general.toFront();
                JButton close = namedButton(general, "Close");
                point[0] = close.getLocationOnScreen();
                point[0].translate(close.getWidth() / 2, close.getHeight() / 2);
            });
            robot.mouseMove(point[0].x, point[0].y);
            robot.mousePress(InputEvent.BUTTON1_DOWN_MASK);
            robot.mouseRelease(InputEvent.BUTTON1_DOWN_MASK);
            SwingUtilities.invokeAndWait(() -> assertEquals(2, config.getIntProperty("count", 0)));
        } finally {
            SwingUtilities.invokeAndWait(() -> {
                if (workspace[0] != null) {
                    workspace[0].destroy();
                }
                if (owner[0] != null) {
                    owner[0].dispose();
                }
                try {
                    UIManager.setLookAndFeel(previous);
                } catch (UnsupportedLookAndFeelException e) {
                    throw new AssertionError(e);
                }
            });
        }
    }

    private static JButton namedButton(Component component, String name) {
        if (component instanceof JButton
                && name.equals(component.getAccessibleContext().getAccessibleName())) {
            return (JButton) component;
        }
        if (component instanceof Container) {
            for (Component child : ((Container) component).getComponents()) {
                JButton found = namedButton(child, name);
                if (found != null) {
                    return found;
                }
            }
        }
        return null;
    }

    private void checkNativeWindows(boolean flatLaf) throws Throwable {
        assumeFalse(GraphicsEnvironment.isHeadless());
        IniFileModel ini = ini();
        Node config = new Node();
        JFrame[] owner = new JFrame[1];
        CalibrationPanelWorkspace[] workspace = new CalibrationPanelWorkspace[1];
        LookAndFeel previous = UIManager.getLookAndFeel();
        Rectangle savedBounds = new Rectangle(330, 160, 640, 400);
        try {
            SwingUtilities.invokeAndWait(() -> {
                if (flatLaf) {
                    FlatLightLaf.setup();
                }
                owner[0] = new JFrame("Calibration workspace test");
                workspace[0] = new CalibrationPanelWorkspace(new UIContext(), config);
                workspace[0].getPrimary().update("first", ini, initialImage());
                owner[0].add(workspace[0].getContent());
                owner[0].setBounds(30, 30, 700, 450);
                owner[0].setVisible(true);
            });
            Robot robot = new Robot();
            robot.setAutoDelay(60);
            robot.setAutoWaitForIdle(true);
            Point[] point = new Point[1];
            SwingUtilities.invokeAndWait(() -> {
                JTextField field = find(workspace[0].getPrimary().getContentPane(), JTextField.class);
                point[0] = field.getLocationOnScreen();
            });
            robot.mouseMove(point[0].x + 5, point[0].y + 5);
            SwingUtilities.invokeAndWait(() -> {
                JLayer<?> layer = (JLayer<?>) workspace[0].getContent();
                JButton detach = find(layer.getGlassPane(), JButton.class);
                assertTrue(detach.isShowing(), "Native hover reveals the button over nested editors");
                JComponent form = workspace[0].getPrimary().getPanelFrame();
                Rectangle bounds = SwingUtilities.convertRectangle(form, new Rectangle(form.getSize()), layer);
                assertEquals(bounds.x + bounds.width - 34, detach.getX());
                assertEquals(bounds.y + 6, detach.getY());
                point[0] = detach.getLocationOnScreen();
            });
            robot.mouseMove(point[0].x + 14, point[0].y + 14);
            robot.mousePress(InputEvent.BUTTON1_DOWN_MASK);
            robot.mouseRelease(InputEvent.BUTTON1_DOWN_MASK);
            SwingUtilities.invokeAndWait(() -> {
                Window detached = Arrays.stream(owner[0].getOwnedWindows()).filter(Window::isShowing).findFirst().get();
                assertTrue(detached.getWidth() < owner[0].getWidth());
                assertTrue(detached.getHeight() < owner[0].getHeight() / 2);
                JScrollPane scroll = find(detached, JScrollPane.class);
                assertEquals(scroll.getPreferredSize(), ((JDialog) detached).getContentPane().getSize(),
                    "Pack around the form, with native window decorations outside the preferred content size");
                find(detached, JTextField.class).setText("58");
                assertEquals("58", find(workspace[0].getPrimary().getContentPane(), JTextField.class).getText());
                detached.setBounds(savedBounds);
            });
            robot.waitForIdle();
            SwingUtilities.invokeAndWait(() -> {
                workspace[0].destroy();
                owner[0].dispose();
                assertEquals(1, config.getIntProperty("count", 0));

                // Restore before the owner opens, with the primary page unselected and its tab hidden.
                owner[0] = new JFrame("Other tab");
                owner[0].add(new JLabel("Another tab is selected"));
                owner[0].setBounds(30, 30, 700, 450);
                workspace[0] = new CalibrationPanelWorkspace(new UIContext(), roundTrip(config));
                workspace[0].getPrimary().update((String) null, ini, initialImage());
                workspace[0].restore(owner[0]);
                owner[0].setVisible(true);
            });
            robot.waitForIdle();
            SwingUtilities.invokeAndWait(() -> {
                Window detached = Arrays.stream(owner[0].getOwnedWindows()).filter(Window::isShowing).findFirst().get();
                assertEquals(savedBounds, detached.getBounds());
                detached.dispatchEvent(new WindowEvent(detached, WindowEvent.WINDOW_CLOSING));
                assertFalse(detached.isDisplayable());
            });
        } finally {
            SwingUtilities.invokeAndWait(() -> {
                if (workspace[0] != null) {
                    workspace[0].destroy();
                }
                if (owner[0] != null) {
                    owner[0].dispose();
                }
                try {
                    UIManager.setLookAndFeel(previous);
                } catch (UnsupportedLookAndFeelException e) {
                    throw new AssertionError(e);
                }
            });
        }
    }

    // [tag:detached_calibration_sync] Exercise real editors in multiple views, including overlapping
    // fields and replacements from the toolbar; comparing images alone misses stale rendered controls.
    @Test
    void editsPropagateInBothDirectionsWithoutRebuildingTheSource() throws Throwable {
        IniFileModel ini = ini();
        SwingUtilities.invokeAndWait(() -> {
            try (Fixture fixture = new Fixture(ini, new Node())) {
                fixture.select("first");
                fixture.workspace.detachCurrentPanel();
                FakeWindow first = fixture.windows.get(0);
                fixture.select("second");
                fixture.workspace.detachCurrentPanel();
                FakeWindow second = fixture.windows.get(1);
                AtomicInteger edits = new AtomicInteger();
                fixture.workspace.setOnConfigChange(image -> edits.incrementAndGet());

                JTextField source = find(first.content, JTextField.class);
                source.setText("37");
                assertSame(source, find(first.content, JTextField.class));
                assertEquals("37", find(second.content, JTextField.class).getText());
                assertEquals("37", find(fixture.primary.getContentPane(), JTextField.class).getText());
                assertEquals(1, edits.get(), "Peer refreshes must not emit edits");

                find(fixture.primary.getContentPane(), JCheckBox.class).doClick();
                assertTrue(find(first.content, JCheckBox.class).isSelected());
                assertTrue(find(second.content, JCheckBox.class).isSelected());
                find(second.content, JComboBox.class).setSelectedItem("Gamma");
                assertEquals("Gamma", find(first.content, JComboBox.class).getSelectedItem());
                assertEquals("Gamma", find(fixture.primary.getContentPane(), JComboBox.class).getSelectedItem());

                fixture.select("other");
                find(fixture.primary.getContentPane(), JTextField.class).setText("62");
                find(first.content, JTextField.class).setText("41");
                assertEquals("62", find(fixture.primary.getContentPane(), JTextField.class).getText());
                assertEquals(62, fixture.primary.getWorkingImage().getContent()[3]);
                fixture.select("first");
                assertEquals("41", find(fixture.primary.getContentPane(), JTextField.class).getText());
                assertTrue(find(fixture.primary.getContentPane(), JCheckBox.class).isSelected());
            }
        });
    }

    @Test
    void primaryNavigationDoesNotRebuildDetachedViewsWhenImageContentsAreUnchanged() throws Throwable {
        IniFileModel ini = ini();
        SwingUtilities.invokeAndWait(() -> {
            try (Fixture fixture = new Fixture(ini, new Node())) {
                fixture.select("first");
                fixture.workspace.detachCurrentPanel();
                JTextField beforeNavigation = find(fixture.windows.get(0).content, JTextField.class);

                fixture.select("second");

                assertSame(beforeNavigation, find(fixture.windows.get(0).content, JTextField.class),
                    "Primary navigation should preserve detached controls when image data is unchanged");
            }
        });
    }

    @Test
    void primaryNavigationRefreshesDetachedViewsWhenInPlaceImageContentsChanged() throws Throwable {
        IniFileModel ini = ini();
        SwingUtilities.invokeAndWait(() -> {
            try (Fixture fixture = new Fixture(ini, new Node())) {
                fixture.select("first");
                fixture.workspace.detachCurrentPanel();
                ConfigurationImage working = fixture.primary.getWorkingImage();
                working.getContent()[0] = 82;

                fixture.select("second");

                assertEquals("82", find(fixture.windows.get(0).content, JTextField.class).getText(),
                    "Detached controls must refresh when mutable working-image contents change");
            }
        });
    }

    @Test
    void primaryViewRefreshesDetachedViewsWhenIniModelChangesWithSameImage() throws Throwable {
        IniFileModel ini = ini();
        IniFileModel replacement = ini();
        SwingUtilities.invokeAndWait(() -> {
            try (Fixture fixture = new Fixture(ini, new Node())) {
                fixture.select("first");
                fixture.workspace.detachCurrentPanel();
                JTextField beforeReplacement = find(fixture.windows.get(0).content, JTextField.class);

                fixture.primary.update("second", replacement, fixture.primary.getWorkingImage());

                assertNotSame(beforeReplacement, find(fixture.windows.get(0).content, JTextField.class),
                    "Detached controls must refresh when their INI model instance changes");
            }
        });
    }

    @Test
    void navigationAfterDetachedEditPreservesDetachedEditor() throws Throwable {
        IniFileModel ini = ini();
        SwingUtilities.invokeAndWait(() -> {
            try (Fixture fixture = new Fixture(ini, new Node())) {
                fixture.select("first");
                fixture.workspace.detachCurrentPanel();
                JTextField edited = find(fixture.windows.get(0).content, JTextField.class);
                edited.setText("38");

                fixture.select("second");

                assertSame(edited, find(fixture.windows.get(0).content, JTextField.class));
                assertEquals("38", edited.getText());
            }
        });
    }

    @Test
    void tableAndCurveEditsReachEveryViewAndPreserveOtherFields() throws Throwable {
        IniFileModel ini = ini();
        SwingUtilities.invokeAndWait(() -> {
            try (Fixture fixture = new Fixture(ini, new Node())) {
                fixture.select("table");
                fixture.workspace.detachCurrentPanel();
                FakeWindow tableWindow = fixture.windows.get(0);
                fixture.select("curve");
                fixture.workspace.detachCurrentPanel();
                FakeWindow curveWindow = fixture.windows.get(1);
                fixture.select("table");
                JTable detached = find(tableWindow.content, JTable.class);
                edit(detached, 0, 1, "72");
                assertSame(detached, find(tableWindow.content, JTable.class));
                assertEquals("72", find(fixture.primary.getContentPane(), JTable.class).getValueAt(0, 1).toString());
                edit(find(curveWindow.content, JTable.class), 0, 1, "9");
                fixture.select("curve");
                assertEquals("9", find(fixture.primary.getContentPane(), JTable.class).getValueAt(0, 1).toString());
                fixture.select("table");
                assertEquals("72", find(fixture.primary.getContentPane(), JTable.class).getValueAt(0, 1).toString());
            }
        });
    }

    @Test
    void undoRedoDiscardAndImageReplacementSynchronizeDetachedPanels() throws Throwable {
        IniFileModel ini = ini();
        SwingUtilities.invokeAndWait(() -> {
            try (Fixture fixture = new Fixture(ini, new Node())) {
                fixture.select("first");
                fixture.workspace.detachCurrentPanel();
                FakeWindow detached = fixture.windows.get(0);
                AtomicReference<ConfigurationImage> session = new AtomicReference<>(fixture.primary.getWorkingImage().clone());
                AtomicReference<String> key = new AtomicReference<>("first");
                TuningToolbarWidget toolbar = new TuningToolbarWidget(fixture.context, fixture.primary, key,
                    session, session.get().clone(), UpdateOperationCallbacks.DUMMY, () -> { }, failed -> { });
                fixture.workspace.setOnConfigChange(image -> {
                    toolbar.onEdit(session.get());
                    session.set(image.clone());
                });
                try {
                    find(detached.content, JTextField.class).setText("45");
                    toolbar.flushBeforeNavigate();
                    button(toolbar.getPanel(), "Undo").doClick();
                    assertEquals("10", find(detached.content, JTextField.class).getText());
                    button(toolbar.getPanel(), "Redo").doClick();
                    assertEquals("45", find(detached.content, JTextField.class).getText());
                    button(toolbar.getPanel(), "Discard changes").doClick();
                    assertEquals("10", find(detached.content, JTextField.class).getText());

                    ConfigurationImage loaded = initialImage();
                    loaded.getContent()[0] = 81;
                    fixture.primary.update("first", ini, loaded);
                    assertEquals("81", find(detached.content, JTextField.class).getText());
                    // Restored detached windows also work before a primary menu page is selected.
                    key.set(null);
                    fixture.primary.update((String) null, ini, session.get());
                    find(detached.content, JTextField.class).setText("29");
                    toolbar.flushBeforeNavigate();
                    button(toolbar.getPanel(), "Undo").doClick();
                    assertEquals("10", find(detached.content, JTextField.class).getText());
                } finally {
                    toolbar.onDisconnect();
                }
            }
        });
    }

    @Test
    void layoutSurvivesSerializationAndShutdownButExplicitCloseRemovesPanel() throws Throwable {
        IniFileModel ini = ini();
        SwingUtilities.invokeAndWait(() -> {
            Node config = new Node();
            try (Fixture first = new Fixture(ini, config)) {
                first.select("first");
                first.workspace.detachCurrentPanel();
                first.workspace.detachCurrentPanel();
                assertEquals(1, first.windows.size(), "One window per page");
                first.windows.get(0).move(new Rectangle(210, 130, 640, 410));
                first.select("curve");
                first.workspace.detachCurrentPanel();
            }
            assertEquals(2, config.getIntProperty("count", 0), "App shutdown retains the open list");
            Node restored = roundTrip(config);
            try (Fixture second = new Fixture(ini, restored)) {
                second.workspace.restoreWindows();
                assertEquals(2, second.windows.size());
                assertEquals(new Rectangle(210, 130, 640, 410), second.windows.get(0).bounds);
                assertEquals("First", second.windows.get(0).title);
                assertEquals("Curve", second.windows.get(1).title);
                second.windows.get(0).close.run();
                assertEquals(1, restored.getIntProperty("count", 0));
                assertEquals("curve", restored.getChild("panel0").getProperty("key"));
                assertTrue(second.windows.get(0).disposed);
            }
        });
    }

    @Test
    void resetAndMissingPagesSuspendWindowsWithoutForgettingTheirLayout() throws Throwable {
        IniFileModel ini = ini();
        SwingUtilities.invokeAndWait(() -> {
            Node config = new Node();
            try (Fixture fixture = new Fixture(ini, config)) {
                fixture.select("first");
                fixture.workspace.detachCurrentPanel();
                fixture.primary.reset();
                assertTrue(fixture.windows.get(0).disposed);
                assertEquals(1, config.getIntProperty("count", 0));
                IniFileModel other = mock(IniFileModel.class);
                fixture.primary.update((String) null, other, initialImage());
                assertEquals(1, fixture.windows.size(), "Do not open a page absent from the new INI");
                fixture.primary.update((String) null, ini, initialImage());
                assertEquals(2, fixture.windows.size(), "Restore when a compatible tune is available");
                assertEquals("10", find(fixture.windows.get(1).content, JTextField.class).getText());
            }
        });
    }

    @Test
    void restoredBoundsFitAvailableMonitorsIncludingNegativeOrigins() {
        List<Rectangle> screens = Arrays.asList(new Rectangle(0, 0, 1920, 1040), new Rectangle(-1280, 0, 1280, 984));
        assertEquals(new Rectangle(-1200, 100, 800, 600),
            CalibrationPanelWorkspace.fitBounds(new Rectangle(-1200, 100, 800, 600), screens));
        assertEquals(new Rectangle(1120, 440, 800, 600),
            CalibrationPanelWorkspace.fitBounds(new Rectangle(4000, 3000, 800, 600), screens));
        assertEquals(new Rectangle(0, 0, 1920, 1040),
            CalibrationPanelWorkspace.fitBounds(new Rectangle(0, 0, 4000, 3000), screens));
    }

    private static final class Fixture implements AutoCloseable {
        final UIContext context = new UIContext();
        final IniFileModel ini;
        final List<FakeWindow> windows = new ArrayList<>();
        final CalibrationPanelWorkspace workspace;
        final CalibrationDialogWidget primary;

        Fixture(IniFileModel ini, Node config) {
            this(ini, config, Collections.singletonList(new Rectangle(0, 0, 1280, 960)));
        }

        Fixture(IniFileModel ini, Node config, List<Rectangle> screens) {
            this.ini = ini;
            context.iniFileState.setIniFileModelForTest(ini);
            workspace = new CalibrationPanelWorkspace(context, config,
                (owner, title, content, close, split, canSplit, moved) -> {
                    FakeWindow window = new FakeWindow(title, content, close, split, canSplit, moved);
                    windows.add(window);
                    return window;
                }, () -> screens);
            primary = workspace.getPrimary();
            primary.update((String) null, ini, initialImage());
        }

        void select(String key) {
            primary.update(key, ini, primary.getWorkingImage());
        }

        public void close() {
            workspace.destroy();
            for (FakeWindow window : windows) {
                assertTrue(window.disposed);
            }
        }
    }

    private static final class FakeWindow implements CalibrationPanelWorkspace.PanelWindow {
        final String title;
        final JScrollPane content;
        final Runnable close;
        final Runnable split;
        final Consumer<Rectangle> moved;
        Rectangle bounds;
        boolean disposed;
        boolean canSplit;

        FakeWindow(String title, JScrollPane content, Runnable close, Runnable split, boolean canSplit,
                   Consumer<Rectangle> moved) {
            this.title = title;
            this.content = content;
            this.bounds = new Rectangle(content.getPreferredSize());
            this.close = close;
            this.split = split;
            this.canSplit = canSplit;
            this.moved = moved;
        }

        void move(Rectangle bounds) {
            this.bounds = bounds;
            moved.accept(bounds);
        }

        public void show() { }
        public void toFront() { }
        public Rectangle getBounds() { return new Rectangle(bounds); }
        public void setBounds(Rectangle bounds) { this.bounds = new Rectangle(bounds); }
        public void setCanSplit(boolean canSplit) { this.canSplit = canSplit; }
        public void dispose() { disposed = true; }
    }

    private static Node roundTrip(Node node) {
        ByteArrayOutputStream bytes = new ByteArrayOutputStream();
        try (XMLEncoder encoder = new XMLEncoder(bytes)) {
            encoder.writeObject(node);
        }
        try (XMLDecoder decoder = new XMLDecoder(new ByteArrayInputStream(bytes.toByteArray()))) {
            return (Node) decoder.readObject();
        }
    }

    static <T extends Component> T find(Component component, Class<T> type) {
        if (type.isInstance(component)) {
            return type.cast(component);
        }
        if (component instanceof Container) {
            for (Component child : ((Container) component).getComponents()) {
                T found = find(child, type);
                if (found != null) {
                    return found;
                }
            }
        }
        return null;
    }

    private static JButton button(JPanel panel, String text) {
        // buttons sit inside JoinedButtons groups, so search the whole toolbar
        return UiUtils.getAllComponents(panel).stream().filter(component -> component instanceof JButton)
            .map(component -> (JButton) component).filter(button -> text.equals(button.getText())).findFirst().get();
    }

    private static void edit(JTable table, int row, int column, String value) {
        table.changeSelection(row, column, false, false);
        assertTrue(table.editCellAt(row, column));
        ((JTextField) table.getEditorComponent()).setText(value);
        assertTrue(table.getCellEditor().stopCellEditing());
    }

    private static ConfigurationImage initialImage() {
        return new ConfigurationImage(new byte[]{10, 0, 0, 20, 1, 10, 2, 10, 1, 2, 3, 4});
    }

    private static IniFileModel exampleIni() throws Throwable {
        try (InputStream input = CalibrationPanelWorkspaceTest.class.getResourceAsStream("/26october.ini")) {
            RawIniFile raw = IniFileReaderUtil.read(input, "26october.ini");
            return IniFileReaderUtil.readIniFile(raw, "26october.ini", new IniFileMetaInfoImpl(raw));
        }
    }

    private static IniFileModel ini() throws Throwable {
        String fields = "field = \"Value\", value\nfield = \"Enabled\", enabled\nfield = \"Mode\", mode\n";
        String text = "[Constants]\npage = 1\n"
            + "value = scalar, U08, 0, \"\", 1, 0, 0, 100, 0\n"
            + "enabled = bits, U08, 1, [0:0], \"Disabled\", \"Enabled\"\n"
            + "mode = bits, U08, 2, [0:1], \"Alpha\", \"Beta\", \"Gamma\", \"Delta\"\n"
            + "otherValue = scalar, U08, 3, \"\", 1, 0, 0, 100, 0\n"
            + "x = array, U08, 4, [2], \"\", 1, 0, 0, 100, 0\n"
            + "y = array, U08, 6, [2], \"\", 1, 0, 0, 100, 0\n"
            + "z = array, U08, 8, [2x2], \"\", 1, 0, 0, 100, 0\n"
            + "[UserDefined]\ndialog = first, \"First\"\n" + fields
            + "dialog = second, \"Second\"\n" + fields
            + "dialog = other, \"Other\"\nfield = \"Other value\", otherValue\n"
            + "dialog = combined, \"Combined\"\nfield = \"Value\", value\npanel = first\npanel = curve\n"
            + "dialog = charts, \"Charts\"\npanel = table\npanel = curve\n"
            + "dialog = incomplete, \"Incomplete\"\npanel = first\npanel = missing\n"
            + "dialog = recursive, \"Recursive\"\npanel = first\npanel = recursive\n"
            + "[TableEditor]\ntable = table, tableMap, \"Table\", 1\nxBins = x\nyBins = y\nzBins = z\n"
            + "[CurveEditor]\ncurve = curve, \"Curve\"\ncolumnLabel = \"X\", \"Y\"\n"
            + "xAxis = 0, 10, 5\nyAxis = 0, 10, 5\nxBins = x\nyBins = y\n";
        IniFileMetaInfo meta = mock(IniFileMetaInfo.class);
        when(meta.getnPages()).thenReturn(1);
        when(meta.getPageSize(0)).thenReturn(12);
        when(meta.getSignature()).thenReturn("detached-test");
        return IniFileReaderUtil.readIniFile(IniFileReaderUtil.read(
            new ByteArrayInputStream(text.getBytes(StandardCharsets.US_ASCII))), "detached-test.ini", meta);
    }
}
