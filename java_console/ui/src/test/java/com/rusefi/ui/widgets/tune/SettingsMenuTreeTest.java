package com.rusefi.ui.widgets.tune;

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
            menuTree.refreshExpressions(ini, new ConfigurationImage(new byte[8]));
            assertTrue(menuTree.isDisabled(pid), "idleMode is off in an empty tune");
            assertEquals("<html>Not available with the current settings. Depends on:"
                + "<br>&bull; Mode (in Idle settings)</html>", menuTree.getUnlockHint(pid));
            assertFalse(menuTree.isSelectable(new TreePath(menuTree.findSubMenuNode("idlePid").getPath())));
            assertEquals("idleSettings", menuTree.findAvailablePageWithField(ini, "idleMode").getKey(),
                "a field opens on the first page the user can open");
        });
    }
}
