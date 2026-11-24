package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageTestHelper;
import com.opensr5.ini.IniFileMetaInfo;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.IniField;
import com.rusefi.ini.reader.IniFileReaderUtil;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import javax.swing.tree.DefaultMutableTreeNode;
import java.awt.*;
import java.io.ByteArrayInputStream;
import java.nio.charset.StandardCharsets;
import java.util.Arrays;
import java.util.Collections;
import java.util.Enumeration;
import java.util.HashSet;
import java.util.List;
import java.util.stream.Collectors;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

/**
 * [tag:unburned_marks] Menu pages showing a setting, table or curve that differs from the last burned
 * tune get a dot, and so do the groups and categories above them (#10671).
 */
public class SettingsMenuTreeUnburnedTest {
    private static final String INI = "[Constants]\npage = 1\n"
        + "flow = scalar, F32, 0, \"cc/min\", 1, 0, 0, 2000, 1\n"
        + "pumpOn = bits, U32, 4, [0:0], \"Disabled\", \"Enabled\"\n"
        + "z = array, U08, 8, [2x2], \"\", 1, 0, 0, 255, 0\n"
        + "x = array, U08, 12, [2], \"\", 1, 0, 0, 255, 0\n"
        + "y = array, U08, 14, [2], \"\", 1, 0, 0, 255, 0\n"
        + "cx = array, U08, 16, [2], \"\", 1, 0, 0, 255, 0\n"
        + "cy = array, U08, 18, [2], \"\", 1, 0, 0, 255, 0\n"
        + "[Menu]\n"
        + "menu = \"Fuel\"\n"
        + "subMenu = injection, \"Injection\"\n"
        + "subMenu = fuelTable, \"Fuel table\"\n"
        + "groupMenu = \"Advanced\"\n"
        + "groupChildMenu = pumpPage, \"Pump\"\n"
        + "menu = \"Ignition\"\n"
        + "subMenu = curvePage, \"Curve\"\n"
        + "[UserDefined]\n"
        + "dialog = injection, \"Injection\"\nfield = \"Flow\", flow\n"
        + "dialog = pumpInner, \"Inner\"\nfield = \"Pump\", pumpOn\n"
        + "dialog = pumpPage, \"Pump\"\npanel = pumpInner\npanel = fuelTable\n"
        + "[TableEditor]\ntable = fuelTable, fuelTableMap, \"Fuel\", 1\nxBins = x\nyBins = y\nzBins = z\n"
        + "[CurveEditor]\ncurve = curvePage, \"Curve\"\ncolumnLabel = \"X\", \"Y\"\n"
        + "xAxis = 0, 255, 5\nyAxis = 0, 255, 5\nxBins = cx\nyBins = cy\n";

    private static IniFileModel ini() throws Throwable {
        IniFileMetaInfo meta = mock(IniFileMetaInfo.class);
        when(meta.getnPages()).thenReturn(1);
        when(meta.getPageSize(0)).thenReturn(20);
        when(meta.getSignature()).thenReturn("unburned-menu");
        return IniFileReaderUtil.readIniFile(IniFileReaderUtil.read(
            new ByteArrayInputStream(INI.getBytes(StandardCharsets.US_ASCII))), "unburned-menu.ini", meta);
    }

    private static ConfigurationImage burned() {
        return ConfigurationImageTestHelper.newLittleEndian(new byte[20]);
    }

    @Test
    void pageFieldsFollowPanelsToDialogsAndTables() throws Throwable {
        IniFileModel ini = ini();
        assertEquals(Arrays.asList("pumpOn", "z", "x", "y"), names(SettingsMenuTree.pageFields(ini, "pumpPage")));
        assertEquals(Arrays.asList("z", "x", "y"), names(SettingsMenuTree.pageFields(ini, "fuelTable")));
        assertEquals(Arrays.asList("cx", "cy"), names(SettingsMenuTree.pageFields(ini, "curvePage")));
        assertEquals(Collections.singletonList("flow"), names(SettingsMenuTree.pageFields(ini, "injection")));
    }

    @Test
    void changedPagesAndTheirGroupsAreMarked() throws Throwable {
        IniFileModel ini = ini();
        SwingUtilities.invokeAndWait(() -> {
            SettingsMenuTree menuTree = new SettingsMenuTree(() -> ini);
            ConfigurationImage burned = burned();
            ConfigurationImage current = burned.clone();

            current.getByteBuffer(0, 4).putFloat(470);
            menuTree.refreshUnburned(ini, current, burned);
            assertEquals(new HashSet<>(Collections.singletonList("injection")), menuTree.getUnburnedPageKeys());
            assertTrue(menuTree.hasUnburned(node(menuTree, "Fuel")), "the category above the page");
            assertFalse(menuTree.hasUnburned(node(menuTree, "Advanced")));
            assertFalse(menuTree.hasUnburned(node(menuTree, "Ignition")));

            current.getByteBuffer(0, 4).putFloat(0);
            current.getContent()[9] = 5; // a cell of the table, also shown on the Pump page through a panel
            menuTree.refreshUnburned(ini, current, burned);
            assertEquals(new HashSet<>(Arrays.asList("fuelTable", "pumpPage")), menuTree.getUnburnedPageKeys());
            assertTrue(menuTree.hasUnburned(node(menuTree, "Advanced")), "the group holding the Pump page");

            current.getContent()[9] = 0;
            current.getContent()[17] = 3; // a curve point
            menuTree.refreshUnburned(ini, current, burned);
            assertEquals(new HashSet<>(Collections.singletonList("curvePage")), menuTree.getUnburnedPageKeys());
            assertTrue(menuTree.hasUnburned(node(menuTree, "Ignition")));
            assertFalse(menuTree.hasUnburned(node(menuTree, "Fuel")));

            menuTree.refreshUnburned(ini, current, current.clone()); // as after a burn
            assertTrue(menuTree.getUnburnedPageKeys().isEmpty());
        });
    }

    @Test
    void withoutABurnedTuneNothingIsMarked() throws Throwable {
        IniFileModel ini = ini();
        SwingUtilities.invokeAndWait(() -> {
            SettingsMenuTree menuTree = new SettingsMenuTree(() -> ini);
            ConfigurationImage current = burned();
            current.getByteBuffer(0, 4).putFloat(470);
            menuTree.refreshUnburned(ini, current, null);
            assertTrue(menuTree.getUnburnedPageKeys().isEmpty());
        });
    }

    @Test
    void theDotWidensTheRowAndShowsInSearchResults() throws Throwable {
        IniFileModel ini = ini();
        SwingUtilities.invokeAndWait(() -> {
            SettingsMenuTree menuTree = new SettingsMenuTree(() -> ini);
            JTree tree = menuTree.getTree();
            DefaultMutableTreeNode injection = menuTree.findSubMenuNode("injection");
            int plainWidth = rendererWidth(tree, injection);

            ConfigurationImage burned = burned();
            ConfigurationImage current = burned.clone();
            current.getByteBuffer(0, 4).putFloat(470);
            menuTree.refreshUnburned(ini, current, burned);
            assertTrue(rendererWidth(tree, injection) > plainWidth, "room for the dot after the text");

            menuTree.filter("inj");
            DefaultMutableTreeNode filteredRoot = (DefaultMutableTreeNode) tree.getModel().getRoot();
            DefaultMutableTreeNode found = find(filteredRoot, "Injection");
            assertNotSame(injection, found, "search results are copied nodes");
            assertTrue(menuTree.hasUnburned(found));
        });
    }

    @Test
    void marksAppearWhenTheTreeIsFilledLater() throws Throwable {
        IniFileModel ini = ini();
        SwingUtilities.invokeAndWait(() -> {
            SettingsMenuTree menuTree = new SettingsMenuTree(() -> null);
            ConfigurationImage burned = burned();
            ConfigurationImage current = burned.clone();
            current.getByteBuffer(0, 4).putFloat(470);
            menuTree.refreshUnburned(ini, current, burned);
            assertTrue(menuTree.getUnburnedPageKeys().isEmpty(), "no pages yet");

            menuTree.populateIfEmpty(ini);
            assertEquals(new HashSet<>(Collections.singletonList("injection")), menuTree.getUnburnedPageKeys());
        });
    }

    private static int rendererWidth(JTree tree, DefaultMutableTreeNode node) {
        Component c = tree.getCellRenderer().getTreeCellRendererComponent(tree, node, false, false, true, 0, false);
        return c.getPreferredSize().width;
    }

    private static DefaultMutableTreeNode node(SettingsMenuTree menuTree, String name) {
        return find(menuTree.getRoot(), name);
    }

    private static DefaultMutableTreeNode find(DefaultMutableTreeNode root, String name) {
        Enumeration<?> nodes = root.preorderEnumeration();
        while (nodes.hasMoreElements()) {
            DefaultMutableTreeNode node = (DefaultMutableTreeNode) nodes.nextElement();
            Object userObject = node.getUserObject();
            String text = userObject instanceof com.opensr5.ini.SubMenuModel
                ? ((com.opensr5.ini.SubMenuModel) userObject).getName() : String.valueOf(userObject);
            if (name.equals(text)) {
                return node;
            }
        }
        throw new AssertionError("no node " + name);
    }

    private static List<String> names(List<IniField> fields) {
        return fields.stream().map(IniField::getName).collect(Collectors.toList());
    }
}
