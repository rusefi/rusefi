package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImageTestHelper;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileMetaInfo;
import com.opensr5.ini.IniFileMetaInfoImpl;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.RawIniFile;
import com.opensr5.ini.SubMenuModel;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.lua.LuaTextEditor;
import com.rusefi.ui.widgets.JoinedButtons;
import org.fife.ui.rsyntaxtextarea.RSyntaxTextArea;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.TreePath;
import java.awt.Component;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.event.ActionEvent;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.io.ByteArrayInputStream;
import java.io.InputStream;
import java.nio.charset.StandardCharsets;
import java.util.*;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.function.Predicate;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

class SettingsSearchPopupTest {
    /** Idle (with a separator, a page needing closed loop and a group), Fuel, and a Help menu of std_ pages. */
    private static IniFileModel ini() throws Throwable {
        String text = "[Constants]\npage = 1\n"
            + "idleMode = bits, U08, 0, [0:0], \"Open loop\", \"Closed loop\"\n"
            + "injectorFlow = scalar, U08, 1, \"cc\", 1, 0, 0, 100, 0\n"
            + "[Menu]\n"
            + "menu = \"Idle\"\n"
            + "subMenu = idleSettings, \"Idle settings\"\n"
            + "subMenu = std_separator\n"
            + "subMenu = idlePid, \"Idle PID\", 0, { idleMode }\n"
            + "groupMenu = \"Advanced idle\"\n"
            + "groupChildMenu = idleTiming, \"Idle timing\"\n"
            + "menu = \"Fuel\"\n"
            + "subMenu = injectors, \"Injectors\"\n"
            + "subMenu = std_separator\n"
            + "subMenu = std_ms3Rtc, \"Real-time clock\"\n"
            + "menu = \"Help\"\n"
            + "subMenu = std_help, \"About\"\n"
            + "[UserDefined]\n"
            + "dialog = idleSettings, \"Idle settings\"\nfield = \"Mode\", idleMode\n"
            + "dialog = idlePid, \"Idle PID\"\nfield = \"Mode\", idleMode\n"
            + "dialog = idleTiming, \"Idle timing\"\nfield = \"Mode\", idleMode\n"
            + "dialog = injectors, \"Injectors\"\nfield = \"Mode\", idleMode\nfield = \"Injector flow\", injectorFlow\n"
            + "field = \"!Injector flow needs a restart\", flowWarning\nfield = \"#Injector flow notes\", flowHeading\n";
        IniFileMetaInfo meta = mock(IniFileMetaInfo.class);
        when(meta.getnPages()).thenReturn(1);
        when(meta.getPageSize(0)).thenReturn(8);
        when(meta.getSignature()).thenReturn("popup-test");
        return IniFileReaderUtil.readIniFile(IniFileReaderUtil.read(
            new ByteArrayInputStream(text.getBytes(StandardCharsets.US_ASCII))), "popup-test.ini", meta);
    }

    /**
     * idleMode is open loop: Idle PID is greyed out. {@code open} gets the page key, or "page@setting" for
     * a setting found.
     */
    private static SettingsSearchPopup popup(Predicate<String> open, Runnable close) {
        try {
            return new SettingsSearchPopup(null, ini(), ConfigurationImageTestHelper.newLittleEndian(new byte[8]),
                (page, field) -> open.test(field == null ? page : page + "@" + field), close);
        } catch (Throwable e) {
            throw new AssertionError(e);
        }
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

    /** The visible rows, as their text. */
    private static List<String> rows(SettingsSearchPopup popup) {
        List<String> rows = new ArrayList<>();
        for (int i = 0; i < popup.tree.getRowCount(); i++) {
            rows.add(text(popup.tree.getPathForRow(i)));
        }
        return rows;
    }

    private static String text(TreePath path) {
        if (path == null) {
            return null;
        }
        Object userObject = ((DefaultMutableTreeNode) path.getLastPathComponent()).getUserObject();
        return userObject instanceof SubMenuModel ? ((SubMenuModel) userObject).getName() : userObject.toString();
    }

    private static String selected(SettingsSearchPopup popup) {
        return text(popup.tree.getSelectionPath());
    }

    /** The keys (or texts) of a menu's rows in the full tree. */
    private static List<String> keys(DefaultMutableTreeNode root, String category) {
        List<String> keys = new ArrayList<>();
        for (int i = 0; i < root.getChildCount(); i++) {
            DefaultMutableTreeNode menu = (DefaultMutableTreeNode) root.getChildAt(i);
            if (category.equals(menu.getUserObject())) {
                for (int j = 0; j < menu.getChildCount(); j++) {
                    Object page = ((DefaultMutableTreeNode) menu.getChildAt(j)).getUserObject();
                    keys.add(page instanceof SubMenuModel ? ((SubMenuModel) page).getKey() : page.toString());
                }
            }
        }
        return keys;
    }

    @Test
    void beforeTypingItShowsTheCategoriesToBrowse() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            List<String> opened = new ArrayList<>();
            SettingsSearchPopup popup = popup(opened::add, () -> { });
            assertEquals(Arrays.asList("Idle", "Fuel"), rows(popup),
                "closed categories; Help has no page that opens as a window");
            assertEquals("Idle", selected(popup));
            assertEquals(SettingsSearchPopup.HINT, popup.status.getText());

            press(popup.search, KeyEvent.VK_ENTER);
            List<String> open = rows(popup);
            assertEquals("Idle settings", open.get(1), "Enter opens a category");
            assertEquals(Arrays.asList("Idle PID", "Advanced idle", "Fuel"), open.subList(3, 6),
                "the separator stays between the pages");
            press(popup.search, KeyEvent.VK_DOWN);
            assertEquals("Idle settings", selected(popup));
            press(popup.search, KeyEvent.VK_DOWN);
            assertEquals("Advanced idle", selected(popup), "Down skips the separator and the greyed-out page");
            press(popup.search, KeyEvent.VK_UP);
            press(popup.search, KeyEvent.VK_UP);
            assertEquals("Idle", selected(popup));
            press(popup.search, KeyEvent.VK_ENTER);
            assertEquals(Arrays.asList("Idle", "Fuel"), rows(popup), "Enter closes it again");

            assertEquals(Collections.singletonList("injectors"), keys(popup.menuTree.getRoot(), "Fuel"),
                "std_ pages and the separator left at the end are hidden");
            assertTrue(opened.isEmpty(), "categories do not open anything");
        });
    }

    @Test
    void typingKeepsTheMatchesUnderTheirCategoriesAndEnterOpensTheChosenPage() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            List<String> opened = new ArrayList<>();
            AtomicInteger closed = new AtomicInteger();
            SettingsSearchPopup popup = popup(opened::add, closed::incrementAndGet);

            popup.search.setText("idle t");
            assertEquals(Arrays.asList("Idle", "Advanced idle", "Idle timing"), rows(popup),
                "words match at the start of a word: not the t in Idle settings");
            assertEquals("Idle timing", selected(popup), "a name starting with the query is chosen first");
            press(popup.search, KeyEvent.VK_DOWN);
            assertEquals("Idle timing", selected(popup), "stops at the last row");
            press(popup.search, KeyEvent.VK_UP);
            assertEquals("Advanced idle", selected(popup));
            press(popup.search, KeyEvent.VK_DOWN);
            press(popup.search, KeyEvent.VK_ENTER);
            assertEquals(Collections.singletonList("idleTiming"), opened);
            assertEquals(1, closed.get(), "the search goes away once the page is open");

            popup.search.setText("fuel");
            assertEquals(Arrays.asList("Fuel", "Injectors"), rows(popup), "a category brings its pages");
            assertEquals("Injectors", selected(popup));

            popup.search.setText("idle pid");
            assertEquals(Arrays.asList("Idle", "Idle PID"), rows(popup), "a greyed-out page is still shown");
            assertEquals("Idle", selected(popup), "but not chosen");
        });
    }

    @Test
    void realIniFindsTheUsualPages() throws Throwable {
        IniFileModel ini;
        try (InputStream input = SettingsSearchPopupTest.class.getResourceAsStream("/26october.ini")) {
            RawIniFile raw = IniFileReaderUtil.read(input, "26october.ini");
            ini = IniFileReaderUtil.readIniFile(raw, "26october.ini", new IniFileMetaInfoImpl(raw));
        }
        SwingUtilities.invokeAndWait(() -> {
            SettingsSearchPopup popup = new SettingsSearchPopup(null, ini, null, (page, field) -> true, () -> { });
            List<String> pages = new ArrayList<>();
            Enumeration<?> nodes = popup.menuTree.getRoot().preorderEnumeration();
            while (nodes.hasMoreElements()) {
                Object userObject = ((DefaultMutableTreeNode) nodes.nextElement()).getUserObject();
                if (userObject instanceof SubMenuModel) {
                    pages.add(((SubMenuModel) userObject).getKey());
                }
            }
            assertTrue(pages.size() > 250, "nearly the whole menu: " + pages.size());
            assertTrue(pages.contains("veTableDialog"));
            assertTrue(pages.contains("veTableMap"), "the VE table's 3D view [tag:table_map3d_pages]");

            popup.search.setText("vvt");
            assertEquals("VVT outputs and PID", selected(popup));
            assertEquals("Advanced", text(popup.tree.getSelectionPath().getParentPath()));
            popup.search.setText("idle");
            assertEquals("Idle settings", selected(popup));
            assertEquals("Idle", text(popup.tree.getSelectionPath().getParentPath()));
            popup.search.setText("PID");
            assertEquals("PID Extra for low RPM", selected(popup),
                "no page is named PID: the first match from the top, not IAC PID multiplier further down");
            assertFalse(rows(popup).contains("mc33816spiDevice"), "spiDevice holds no word starting with pid");

            // laid out as in the window
            JComponent content = popup.getContent();
            content.setSize(content.getPreferredSize());
            layOut(content);
            // vvt: the page just under its category; fuel: a page far down a long group of trims
            for (String query : Arrays.asList("vvt", "fuel", "")) {
                popup.search.setText(query);
                layOut(content);
                TreePath choice = popup.tree.getSelectionPath();
                Rectangle row = popup.tree.getPathBounds(choice);
                Rectangle view = popup.scroll.getViewport().getViewRect();
                assertTrue(view.contains(row), query + ": the chosen row is in view");
                int rowsAbove = (row.y - view.y) / row.height;
                assertTrue(rowsAbove <= choice.getPathCount() - 2,
                    query + ": near the top, under its nearby headings only, not " + rowsAbove + " rows down");
            }
        });
    }

    private static void layOut(Container container) {
        container.doLayout();
        for (Component child : container.getComponents()) {
            if (child instanceof Container) {
                layOut((Container) child);
            }
        }
    }

    @Test
    void escClosesAndNothingOpensWithoutAMatch() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            AtomicBoolean opened = new AtomicBoolean();
            AtomicInteger closed = new AtomicInteger();
            SettingsSearchPopup popup = popup(page -> opened.getAndSet(true), closed::incrementAndGet);
            popup.search.setText("boost ");
            assertEquals(0, popup.tree.getRowCount());
            assertEquals("No settings page or setting matches “boost”.", popup.status.getText());
            press(popup.search, KeyEvent.VK_ENTER);
            assertFalse(opened.get());
            assertEquals(0, closed.get());
            press(popup.search, KeyEvent.VK_ESCAPE);
            assertEquals(1, closed.get());
        });
    }

    @Test
    void aSettingFoundByItsLabelOpensItsPageAtIt() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            List<String> opened = new ArrayList<>();
            AtomicInteger closed = new AtomicInteger();
            SettingsSearchPopup popup = popup(opened::add, closed::incrementAndGet);

            popup.search.setText("flow");
            assertEquals(Arrays.asList("Fuel", "Injectors", "Injector flow"), rows(popup),
                "the setting under its page, which only it brought; ! and # notes are not settings");
            assertEquals("Injector flow", selected(popup), "the setting, not the page around it");
            Component rendered = popup.tree.getCellRenderer().getTreeCellRendererComponent(popup.tree,
                popup.tree.getSelectionPath().getLastPathComponent(), true, false, true, 2, false);
            assertNull(((JLabel) rendered).getIcon(), "a setting has no page icon");

            popup.search.setText("inj");
            assertEquals(Arrays.asList("Fuel", "Injectors", "Injector flow"), rows(popup));
            assertEquals("Injectors", selected(popup), "a page named by the query comes before a setting");
            press(popup.search, KeyEvent.VK_DOWN);
            press(popup.search, KeyEvent.VK_ENTER);
            assertEquals(Collections.singletonList("injectors@injectorFlow"), opened);
            assertEquals(1, closed.get());

            popup.search.setText("mode");
            assertTrue(rows(popup).containsAll(Arrays.asList("Idle settings", "Idle PID", "Injectors")),
                "every page holding it: " + rows(popup));
            assertEquals("Idle settings", text(popup.tree.getSelectionPath().getParentPath()),
                "the first one the user can open");
        });
    }

    @Test
    void aPageThatCannotOpenSaysWhyAndStays() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            AtomicInteger closed = new AtomicInteger();
            SettingsSearchPopup popup = popup(page -> false, closed::incrementAndGet);
            popup.search.setText("injectors");
            popup.openSelected();
            assertEquals("“Injectors” cannot open now: no tune is loaded.", popup.status.getText());
            assertEquals(0, closed.get());
        });
    }

    @Test
    void withoutAnIniItSaysWhatToDo() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            SettingsSearchPopup popup = new SettingsSearchPopup(null, null, null, (page, field) -> true, () -> { });
            assertEquals(0, popup.tree.getRowCount());
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
    void theTabBarBoxIsTheSearchField() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            JTextField box = SettingsSearchPopup.createSearchBox();
            List<String> opened = new ArrayList<>();
            AtomicInteger shown = new AtomicInteger();
            AtomicInteger hidden = new AtomicInteger();
            SettingsSearchPopup.BoxSearch boxSearch = new SettingsSearchPopup.BoxSearch(box, closeAction -> {
                try {
                    return new SettingsSearchPopup(box, ini(), ConfigurationImageTestHelper.newLittleEndian(new byte[8]),
                        (page, field) -> opened.add(page), closeAction);
                } catch (Throwable e) {
                    throw new AssertionError(e);
                }
            }, popup -> shown.incrementAndGet(), hidden::incrementAndGet);
            assertEquals(JoinedButtons.HEIGHT, box.getPreferredSize().height, "lines up with the tab bar buttons");

            // events only reach an undisplayed field's listeners when handed to them directly
            for (MouseListener listener : ownListeners(box.getMouseListeners())) {
                listener.mousePressed(new MouseEvent(box, MouseEvent.MOUSE_PRESSED, 0, 0, 5, 5, 1, false,
                    MouseEvent.BUTTON1));
            }
            assertTrue(boxSearch.isOpen(), "a click drops the tree down");
            SettingsSearchPopup popup = boxSearch.getPopup();
            assertSame(box, popup.search, "no second field: the query is typed in the box");
            assertFalse(Arrays.asList(popup.getContent().getComponents()).contains(box));
            assertEquals(Arrays.asList("Idle", "Fuel"), rows(popup));

            box.setText("injectors");
            assertEquals(Arrays.asList("Fuel", "Injectors"), rows(popup), "typing in the box filters the tree");
            press(box, KeyEvent.VK_ENTER);
            assertEquals(Collections.singletonList("injectors"), opened);
            assertFalse(boxSearch.isOpen(), "the tree goes away once the page is open");
            assertEquals("", box.getText(), "and the box is emptied");
            assertEquals("notify-field-accept",
                box.getInputMap(JComponent.WHEN_FOCUSED).get(KeyStroke.getKeyStroke(KeyEvent.VK_ENTER, 0)),
                "the box takes its keys as before");

            box.setText("i");
            assertTrue(boxSearch.isOpen(), "typing drops the tree down again");
            assertEquals("i", boxSearch.getPopup().search.getText());
            press(box, KeyEvent.VK_ESCAPE);
            assertFalse(boxSearch.isOpen());
            assertEquals("", box.getText(), "Esc empties the box");

            box.setText("idle");
            for (FocusListener listener : ownListeners(box.getFocusListeners())) {
                listener.focusLost(new FocusEvent(box, FocusEvent.FOCUS_LOST));
            }
            assertFalse(boxSearch.isOpen(), "a click elsewhere closes the tree");
            assertEquals("idle", box.getText(), "but keeps what was typed");
            for (KeyListener listener : ownListeners(box.getKeyListeners())) {
                listener.keyPressed(new KeyEvent(box, KeyEvent.KEY_PRESSED, 0, 0, KeyEvent.VK_DOWN,
                    KeyEvent.CHAR_UNDEFINED));
            }
            assertTrue(boxSearch.isOpen(), "Down drops it down again");
            assertEquals("Idle settings", selected(boxSearch.getPopup()), "with the query still applied");
            assertEquals(4, shown.get());
            assertEquals(3, hidden.get());
        });
    }

    @Test
    void aClickOnAPageOpensItEvenIfTheMouseMovesALittle() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            List<String> opened = new ArrayList<>();
            SettingsSearchPopup popup = popup(opened::add, () -> { });
            popup.search.setText("injectors");
            int row = rows(popup).indexOf("Injectors");
            Rectangle bounds = popup.tree.getRowBounds(row);
            Point at = new Point(bounds.x + 10, bounds.y + bounds.height / 2);

            release(popup, at, new Point(at.x + 15, at.y));
            assertTrue(opened.isEmpty(), "a drag across the row is not a click");
            release(popup, at, new Point(at.x + 3, at.y + 1));
            assertEquals(Collections.singletonList("injectors"), opened,
                "AWT sends no click after any movement: the release opens the page");
        });
    }

    private static void release(SettingsSearchPopup popup, Point from, Point to) {
        popup.rowClick.mousePressed(new MouseEvent(popup.tree, MouseEvent.MOUSE_PRESSED, 0,
            java.awt.event.InputEvent.BUTTON1_DOWN_MASK, from.x, from.y, 1, false, MouseEvent.BUTTON1));
        popup.rowClick.mouseReleased(new MouseEvent(popup.tree, MouseEvent.MOUSE_RELEASED, 0, 0, to.x, to.y, 1,
            false, MouseEvent.BUTTON1));
    }

    @Test
    void aClickOutsideTheBoxAndTheTreeClosesIt() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            JTextField box = SettingsSearchPopup.createSearchBox();
            SettingsSearchPopup.BoxSearch boxSearch = new SettingsSearchPopup.BoxSearch(box, closeAction -> {
                try {
                    return new SettingsSearchPopup(box, ini(), null, (page, field) -> true, closeAction);
                } catch (Throwable e) {
                    throw new AssertionError(e);
                }
            }, popup -> { }, () -> { });
            box.setText("idle");
            boxSearch.pressed(box);
            boxSearch.pressed(boxSearch.getPopup().tree);
            assertTrue(boxSearch.isOpen(), "clicks in the box or the tree keep it");
            boxSearch.pressed(new JLabel("a gauge, which takes no focus"));
            assertFalse(boxSearch.isOpen());
            assertEquals("idle", box.getText(), "what was typed stays");
        });
    }

    @Test
    void nothingDropsDownWithoutPagesToSearch() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            JTextField box = SettingsSearchPopup.createSearchBox();
            AtomicInteger shown = new AtomicInteger();
            SettingsSearchPopup.BoxSearch boxSearch = new SettingsSearchPopup.BoxSearch(box, closeAction -> null,
                popup -> shown.incrementAndGet(), () -> { });
            box.setText("idle");
            assertFalse(boxSearch.isOpen(), "firmware being flashed");
            assertEquals(0, shown.get());
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
