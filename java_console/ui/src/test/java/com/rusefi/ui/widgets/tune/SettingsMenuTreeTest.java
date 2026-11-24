package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImageTestHelper;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileMetaInfo;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.SubMenuModel;
import com.rusefi.ini.reader.IniFileReaderUtil;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.TreePath;
import java.io.ByteArrayInputStream;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Enumeration;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

/**
 * {@link SettingsMenuTree} used on its own, without the Tuning tab's {@link MainMenuTreeWidget}.
 */
class SettingsMenuTreeTest {
    private static IniFileModel ini() throws Throwable {
        String text = "[Constants]\npage = 1\n"
            + "idleMode = bits, U08, 0, [0:0], \"Open loop\", \"Closed loop\"\n"
            + "[Menu]\n"
            + "menu = \"Idle\"\n"
            + "subMenu = idleSettings, \"Idle settings\"\n"
            + "subMenu = std_separator\n"
            + "subMenu = idlePid, \"Idle PID\", 0, { idleMode }\n"
            + "menu = \"Fuel\"\n"
            + "subMenu = injectors, \"Injectors\"\n"
            + "[UserDefined]\n"
            + "dialog = idleSettings, \"Idle settings\"\nfield = \"Mode\", idleMode\n"
            + "dialog = idlePid, \"Idle PID\"\nfield = \"Mode\", idleMode\n"
            + "dialog = injectors, \"Injectors\"\n";
        IniFileMetaInfo meta = mock(IniFileMetaInfo.class);
        when(meta.getnPages()).thenReturn(1);
        when(meta.getPageSize(0)).thenReturn(8);
        when(meta.getSignature()).thenReturn("menu-tree-test");
        return IniFileReaderUtil.readIniFile(IniFileReaderUtil.read(
            new ByteArrayInputStream(text.getBytes(StandardCharsets.US_ASCII))), "menu-tree-test.ini", meta);
    }

    @Test
    void buildsFiltersAndGreysOutWithoutAHost() throws Throwable {
        IniFileModel ini = ini();
        SwingUtilities.invokeAndWait(() -> {
            SettingsMenuTree menuTree = new SettingsMenuTree(() -> ini);
            JTree tree = menuTree.getTree();
            assertEquals(2, menuTree.getRoot().getChildCount(), "one node per INI menu");

            menuTree.filter("pid");
            DefaultMutableTreeNode filtered = (DefaultMutableTreeNode) tree.getModel().getRoot();
            assertEquals(1, filtered.getChildCount(), "only the category holding a match stays");
            DefaultMutableTreeNode idle = (DefaultMutableTreeNode) filtered.getChildAt(0);
            assertEquals("Idle", idle.getUserObject());
            assertEquals(1, idle.getChildCount());
            assertTrue(tree.isExpanded(new TreePath(idle.getPath())), "matches are shown expanded");

            menuTree.filter("");
            assertSame(menuTree.getRoot(), tree.getModel().getRoot(), "empty text restores the full tree");

            SubMenuModel pid = (SubMenuModel) menuTree.findSubMenuNode("idlePid").getUserObject();
            assertFalse(menuTree.isDisabled(pid));
            assertEquals("idlePid", menuTree.findAvailablePageWithField(ini, "idleMode", "idleSettings").getKey(),
                "[tag:settings_summary_line] a page can look for another page showing its setting");
            menuTree.refreshExpressions(ini, ConfigurationImageTestHelper.newLittleEndian(new byte[8]));
            assertTrue(menuTree.isDisabled(pid), "idleMode is off in an empty tune");
            assertEquals("<html>Not available with the current settings. Depends on:"
                + "<br>&bull; Mode (in Idle settings)</html>", menuTree.getUnlockHint(pid));
            assertFalse(menuTree.isSelectable(new TreePath(menuTree.findSubMenuNode("idlePid").getPath())));
            assertEquals("idleSettings", menuTree.findAvailablePageWithField(ini, "idleMode").getKey(),
                "a field opens on the first page the user can open");
        });
    }

    @Test
    void settingsWhoseLabelMatchesAreRowsUnderTheirPage() throws Throwable {
        IniFileModel ini = ini();
        SwingUtilities.invokeAndWait(() -> {
            SettingsMenuTree menuTree = new SettingsMenuTree(() -> ini);
            // idleMode is off: Idle PID is greyed out
            menuTree.refreshExpressions(ini, ConfigurationImageTestHelper.newLittleEndian(new byte[8]));
            JTree tree = menuTree.getTree();

            menuTree.filter("MODE");
            DefaultMutableTreeNode idle = (DefaultMutableTreeNode) ((DefaultMutableTreeNode) tree.getModel().getRoot())
                .getChildAt(0);
            assertEquals(2, idle.getChildCount(), "both pages holding the setting, Fuel has none");
            DefaultMutableTreeNode settingsPage = (DefaultMutableTreeNode) idle.getChildAt(0);
            DefaultMutableTreeNode pidPage = (DefaultMutableTreeNode) idle.getChildAt(1);
            SettingsMenuTree.SettingRow mode = (SettingsMenuTree.SettingRow)
                ((DefaultMutableTreeNode) settingsPage.getChildAt(0)).getUserObject();
            assertEquals("Mode", mode.getLabel());
            assertEquals("idleMode", mode.getFieldKey());
            assertEquals("idleSettings", mode.getPage().getKey());

            TreePath pidMode = new TreePath(((DefaultMutableTreeNode) pidPage.getChildAt(0)).getPath());
            assertFalse(menuTree.isSelectable(pidMode), "a setting of a greyed-out page cannot be chosen");
            assertEquals(UIManager.getColor("Label.disabledForeground"), tree.getCellRenderer()
                .getTreeCellRendererComponent(tree, pidMode.getLastPathComponent(), false, false, true, 0, false)
                .getForeground());
            assertSame(mode, ((DefaultMutableTreeNode) menuTree.bestMatch("MODE").getLastPathComponent())
                .getUserObject());
            assertTrue(menuTree.getUnlockHint(((SettingsMenuTree.SettingRow) ((DefaultMutableTreeNode)
                pidMode.getLastPathComponent()).getUserObject()).getPage()) != null, "the hover hint is the page's");

            menuTree.filter("idle settings");
            assertEquals(0, menuTree.findSubMenuNode("idleSettings").getChildCount(), "the full tree is untouched");
            DefaultMutableTreeNode shown = (DefaultMutableTreeNode) ((DefaultMutableTreeNode)
                ((DefaultMutableTreeNode) tree.getModel().getRoot()).getChildAt(0)).getChildAt(0);
            assertEquals(0, shown.getChildCount(), "a page matched by name lists only settings matching too");
        });
    }

    /** Pages whose names share words, in two menus and a group; Idle PID needs closed loop. */
    private static IniFileModel searchIni() throws Throwable {
        String text = "[Constants]\npage = 1\n"
            + "idleMode = bits, U08, 0, [0:0], \"Open loop\", \"Closed loop\"\n"
            + "[Menu]\n"
            + "menu = \"&Idle\"\n"
            + "subMenu = idleSettings, \"Idle settings\"\n"
            + "subMenu = idlePid, \"Idle PID\", 0, { idleMode }\n"
            + "subMenu = idleTable, \"Idle target table\"\n"
            + "groupMenu = \"Advanced idle\"\n"
            + "groupChildMenu = idleTiming, \"Idle timing\"\n"
            + "menu = \"Fuel\"\n"
            + "subMenu = injectors, \"Injectors\"\n"
            + "subMenu = fuelPid, \"PID fuel trim\"\n"
            + "subMenu = fuelIdle, \"Idle\"\n";
        IniFileMetaInfo meta = mock(IniFileMetaInfo.class);
        when(meta.getnPages()).thenReturn(1);
        when(meta.getPageSize(0)).thenReturn(8);
        when(meta.getSignature()).thenReturn("menu-search-test");
        return IniFileReaderUtil.readIniFile(IniFileReaderUtil.read(
            new ByteArrayInputStream(text.getBytes(StandardCharsets.US_ASCII))), "menu-search-test.ini", meta);
    }

    private static SettingsMenuTree searchTree(boolean closedLoop) {
        try {
            IniFileModel ini = searchIni();
            SettingsMenuTree menuTree = new SettingsMenuTree(() -> ini);
            ConfigurationImage image = ConfigurationImageTestHelper.newLittleEndian(8);
            image.getContent()[0] = (byte) (closedLoop ? 1 : 0);
            menuTree.refreshExpressions(ini, image);
            return menuTree;
        } catch (Throwable e) {
            throw new AssertionError(e);
        }
    }

    /** The keys of the pages the filtered tree shows, in menu order. */
    private static List<String> shown(SettingsMenuTree menuTree, String query) {
        menuTree.filter(query);
        List<String> keys = new ArrayList<>();
        Enumeration<?> nodes = ((DefaultMutableTreeNode) menuTree.getTree().getModel().getRoot()).preorderEnumeration();
        while (nodes.hasMoreElements()) {
            Object userObject = ((DefaultMutableTreeNode) nodes.nextElement()).getUserObject();
            if (userObject instanceof SubMenuModel) {
                keys.add(((SubMenuModel) userObject).getKey());
            }
        }
        return keys;
    }

    private static String best(SettingsMenuTree menuTree, String query) {
        menuTree.filter(query);
        TreePath best = menuTree.bestMatch(query);
        return best == null ? null : ((SubMenuModel) ((DefaultMutableTreeNode) best.getLastPathComponent())
            .getUserObject()).getKey();
    }

    @Test
    void everyWordMustMatchThePathAndTheBestNameIsChosen() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            SettingsMenuTree menuTree = searchTree(true);
            List<String> all = shown(menuTree, "");
            assertEquals(7, all.size(), "no query: the whole tree");
            assertEquals(all, shown(menuTree, "   "), "spaces are no query");
            assertNull(best(menuTree, "  "));

            assertEquals(Arrays.asList("idleSettings", "idlePid", "idleTable", "idleTiming", "fuelIdle"),
                shown(menuTree, "IDLE"), "any case; the & mnemonic is not part of the name");
            assertEquals("fuelIdle", best(menuTree, "IDLE"), "the exact name");
            assertEquals("fuelPid", best(menuTree, "pid"), "a name starting with the query before one containing it");
            assertEquals("injectors", best(menuTree, "fuel"),
                "no name is or starts with the query: the first match from the top, not one further down");
            assertEquals(Arrays.asList("idleTable", "idleTiming"), shown(menuTree, "idle t"),
                "every word must match at the start of a word: Idle PID has no t, Idle settings none first");
            assertEquals("idleTable", best(menuTree, "idle  t"), "first in menu order among equals");
            assertEquals(Collections.singletonList("idleTable"), shown(menuTree, "table idle"), "words in any order");
            assertEquals(Collections.singletonList("idleTiming"), shown(menuTree, "advanced timing"),
                "group names count too");
            assertTrue(shown(menuTree, "dle").isEmpty(), "words match from their start only");
            assertTrue(shown(menuTree, "boost").isEmpty());
            assertNull(best(menuTree, "boost"));
        });
    }

    @Test
    void aGreyedOutPageIsShownButNeverTheBestMatch() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            SettingsMenuTree menuTree = searchTree(false);
            assertEquals(Collections.singletonList("idlePid"), shown(menuTree, "idle pid"));
            assertNull(best(menuTree, "idle pid"));
            assertEquals("fuelPid", best(menuTree, "pid"));
        });
    }

    @Test
    void matchedLettersAreBold() throws Exception {
        assertEquals("Idle [PID]", marked("Idle PID", "pid"));
        assertEquals("[I]dle [t]iming", marked("Idle timing", " I  T "), "every word, at the start of a word");
        assertEquals("[PID] min, [pid] max", marked("PID min, pid max", "pid"), "every occurrence");
        assertEquals("mc33816spiDevice", marked("mc33816spiDevice", "pid"), "not across the words spi and Device");
        assertEquals("mc33816[spiDevice]", marked("mc33816spiDevice", "spi device"),
            "a word starts after a digit and at a capital");
        assertEquals("[A/C] idle", marked("A/C idle", "a/c"));
        assertEquals("A[/C] idle", marked("A/C idle", "/c"), "punctuation first: anywhere");
        assertEquals("Idle", marked("Idle", "fuel"), "a row shown for its menu keeps its plain name");
        assertEquals("Idle", marked("Idle", ""));

        SwingUtilities.invokeAndWait(() -> {
            SettingsMenuTree menuTree = searchTree(true);
            JTree tree = menuTree.getTree();
            DefaultMutableTreeNode pid = menuTree.findSubMenuNode("fuelPid");
            int plainWidth = rendered(tree, pid).getPreferredSize().width;
            menuTree.filter("pid");
            JLabel label = rendered(tree, pid);
            assertEquals("PID fuel trim", label.getText(), "the name stays plain text, the renderer draws the bold");
            assertTrue(label.getPreferredSize().width > plainWidth, "the row grows by the width of the bold letters");
            menuTree.filter("");
            assertEquals(plainWidth, rendered(tree, pid).getPreferredSize().width,
                "plain again once the search is cleared");
        });
    }

    /** The name with its bold letters in brackets. */
    private static String marked(String name, String query) {
        boolean[] bold = SettingsMenuTree.boldLetters(name, SettingsMenuTree.words(query));
        if (bold == null) {
            return name;
        }
        StringBuilder text = new StringBuilder();
        for (int i = 0; i < name.length(); i++) {
            if (bold[i] && (i == 0 || !bold[i - 1])) {
                text.append('[');
            }
            text.append(name.charAt(i));
            if (bold[i] && (i == name.length() - 1 || !bold[i + 1])) {
                text.append(']');
            }
        }
        return text.toString();
    }

    private static JLabel rendered(JTree tree, DefaultMutableTreeNode node) {
        return (JLabel) tree.getCellRenderer().getTreeCellRendererComponent(tree, node, false, false, true, 0, false);
    }
}
