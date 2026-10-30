package com.rusefi.ui.widgets.tune;

import com.opensr5.ini.GroupMenuModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.MenuModel;
import com.opensr5.ini.SubMenuModel;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.lua.LuaTextEditor;
import com.rusefi.ui.widgets.tune.SettingsPageIndex.Page;
import org.junit.jupiter.api.Test;
import org.fife.ui.rsyntaxtextarea.RSyntaxTextArea;

import com.rusefi.ui.widgets.JoinedButtons;

import javax.swing.*;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;
import java.util.*;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

class SettingsSearchPopupTest {
    /** An INI whose menu pages are all dialogs, so each one can open as a window. */
    private static SettingsPageIndex index() {
        IniFileModel ini = mock(IniFileModel.class);
        MenuModel idle = new MenuModel("Idle");
        idle.getItems().add(new SubMenuModel("idleSettings", "Idle settings"));
        idle.getItems().add(new SubMenuModel("idlePid", "Idle PID"));
        GroupMenuModel advanced = new GroupMenuModel("Advanced idle");
        advanced.getItems().add(new SubMenuModel("idleTiming", "Idle timing"));
        idle.getItems().add(advanced);
        MenuModel fuel = new MenuModel("Fuel");
        fuel.getItems().add(new SubMenuModel("injectors", "Injectors"));
        when(ini.getMenus()).thenReturn(Arrays.asList(idle, fuel));
        Map<String, com.opensr5.ini.DialogModel> dialogs = new HashMap<>();
        for (String key : new String[]{"idleSettings", "idlePid", "idleTiming", "injectors"}) {
            dialogs.put(key, mock(com.opensr5.ini.DialogModel.class));
        }
        when(ini.getDialogs()).thenReturn(dialogs);
        return SettingsPageIndex.build(ini, null);
    }

    private static void press(JComponent component, int key) {
        Object name = component.getInputMap(JComponent.WHEN_FOCUSED).get(KeyStroke.getKeyStroke(key, 0));
        assertNotNull(name, KeyEvent.getKeyText(key) + " is bound");
        component.getActionMap().get(name).actionPerformed(new ActionEvent(component, ActionEvent.ACTION_PERFORMED, ""));
    }

    /** The box's own listeners, not the look and feel's. */
    private static <T> List<T> ownListeners(T[] listeners) {
        List<T> own = new ArrayList<>();
        for (T listener : listeners) {
            if (listener.getClass().getName().startsWith(SettingsSearchPopup.class.getName())) {
                own.add(listener);
            }
        }
        assertEquals(1, own.size());
        return own;
    }

    private static List<String> shown(SettingsSearchPopup popup) {
        List<String> keys = new ArrayList<>();
        for (int i = 0; i < popup.results.size(); i++) {
            keys.add(popup.results.get(i).getKey());
        }
        return keys;
    }

    @Test
    void typingNarrowsTheListAndEnterOpensTheChosenPage() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            List<String> opened = new ArrayList<>();
            AtomicInteger closed = new AtomicInteger();
            SettingsSearchPopup popup = new SettingsSearchPopup(index(), page -> opened.add(page.getKey()),
                closed::incrementAndGet);
            assertEquals(Arrays.asList("idleSettings", "idlePid", "idleTiming", "injectors"), shown(popup),
                "every page before typing");
            assertEquals(0, popup.list.getSelectedIndex(), "the first one is ready for Enter");
            assertEquals(SettingsSearchPopup.HINT, popup.status.getText());

            popup.search.setText("idle t");
            assertEquals(Arrays.asList("idleTiming", "idleSettings"), shown(popup));
            press(popup.search, KeyEvent.VK_DOWN);
            press(popup.search, KeyEvent.VK_DOWN);
            assertEquals(1, popup.list.getSelectedIndex(), "stops at the last result");
            press(popup.search, KeyEvent.VK_UP);
            press(popup.search, KeyEvent.VK_UP);
            assertEquals(0, popup.list.getSelectedIndex(), "stops at the first result");
            press(popup.search, KeyEvent.VK_DOWN);
            press(popup.search, KeyEvent.VK_ENTER);
            assertEquals(Collections.singletonList("idleSettings"), opened);
            assertEquals(1, closed.get(), "the search goes away once the page is open");
        });
    }

    @Test
    void escClosesAndNothingOpensWithoutAMatch() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            AtomicBoolean opened = new AtomicBoolean();
            AtomicInteger closed = new AtomicInteger();
            SettingsSearchPopup popup = new SettingsSearchPopup(index(), page -> opened.getAndSet(true),
                closed::incrementAndGet);
            popup.search.setText("boost ");
            assertTrue(popup.results.isEmpty());
            assertEquals("No settings page matches “boost”.", popup.status.getText());
            press(popup.search, KeyEvent.VK_ENTER);
            assertFalse(opened.get());
            assertEquals(0, closed.get());
            press(popup.search, KeyEvent.VK_ESCAPE);
            assertEquals(1, closed.get());
        });
    }

    @Test
    void aPageThatCannotOpenSaysWhyAndStays() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            AtomicInteger closed = new AtomicInteger();
            SettingsSearchPopup popup = new SettingsSearchPopup(index(), page -> false, closed::incrementAndGet);
            popup.search.setText("injectors");
            popup.openSelected();
            assertEquals("“Injectors” cannot open now: no tune is loaded.", popup.status.getText());
            assertEquals(0, closed.get());
        });
    }

    @Test
    void withoutAnIniItSaysWhatToDo() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            SettingsSearchPopup popup = new SettingsSearchPopup(SettingsPageIndex.build(null, null), page -> true,
                () -> { });
            assertTrue(popup.results.isEmpty());
            assertEquals(SettingsSearchPopup.NO_PAGES, popup.status.getText());
            popup.search.setText("idle");
            assertEquals(SettingsSearchPopup.NO_PAGES, popup.status.getText(), "not \"no match\"");
        });
    }

    @Test
    void opensUnderTheBoxOrNearTheTopOfTheConsole() {
        Rectangle console = new Rectangle(100, 50, 1200, 900);
        Dimension popup = new Dimension(560, 330);
        assertEquals(new Point(100 + (1200 - 560) / 2, 50 + 120), SettingsSearchPopup.location(console, popup, null),
            "Ctrl+K: centred near the top");
        Rectangle box = new Rectangle(1000, 60, 200, 36);
        assertEquals(new Point(1200 - 560, 60 + 36 + 4), SettingsSearchPopup.location(console, popup, box),
            "right edges lined up, just below the box");
        Rectangle leftBox = new Rectangle(110, 60, 200, 36);
        assertEquals(100, SettingsSearchPopup.location(console, popup, leftBox).x, "kept inside the console");
    }

    @Test
    void theTabBarBoxOpensTheSearchWithWhatWasTyped() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            List<String> shown = new ArrayList<>();
            List<Component> anchors = new ArrayList<>();
            JTextField box = SettingsSearchPopup.createSearchBox((text, anchor) -> {
                shown.add(text);
                anchors.add(anchor);
            });
            // events only reach an undisplayed field's listeners when handed to them directly
            for (MouseListener listener : ownListeners(box.getMouseListeners())) {
                listener.mousePressed(new MouseEvent(box, MouseEvent.MOUSE_PRESSED, 0, 0, 5, 5, 1, false,
                    MouseEvent.BUTTON1));
            }
            for (KeyListener listener : ownListeners(box.getKeyListeners())) {
                listener.keyTyped(new KeyEvent(box, KeyEvent.KEY_TYPED, 0, 0, KeyEvent.VK_UNDEFINED, 'b'));
                listener.keyPressed(new KeyEvent(box, KeyEvent.KEY_PRESSED, 0, 0, KeyEvent.VK_ENTER,
                    KeyEvent.CHAR_UNDEFINED));
                listener.keyTyped(new KeyEvent(box, KeyEvent.KEY_TYPED, 0, 0, KeyEvent.VK_UNDEFINED, (char) 9));
            }
            assertEquals(Arrays.asList("", "b", ""), shown, "a click, a letter and Enter; Tab just moves on");
            assertSame(box, anchors.get(0), "the search opens under the box");
            assertEquals("", box.getText(), "the letter goes to the search, the box stays empty");
            assertEquals(JoinedButtons.HEIGHT, box.getPreferredSize().height, "lines up with the tab bar buttons");
        });
    }

    /** The script editor keeps RSyntaxTextArea's own Ctrl+K ("next occurrence"); the search waits outside it. */
    @Test
    void luaEditorKeepsItsCtrlK() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            LuaTextEditor editor = new LuaTextEditor(new UIContext());
            RSyntaxTextArea text = CalibrationPanelWorkspaceTest.find(editor.getTextArea(), RSyntaxTextArea.class);
            Object binding = text.getInputMap().get(SettingsSearchPopup.SHORTCUT);
            assertNotNull(binding);
            assertNotNull(text.getActionMap().get(binding), "Ctrl+K still acts in the editor: " + binding);
        });
    }
}
