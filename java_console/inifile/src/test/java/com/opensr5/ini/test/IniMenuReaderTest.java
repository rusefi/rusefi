package com.opensr5.ini.test;

import com.opensr5.ini.*;
import com.rusefi.ini.reader.IniFileReaderUtil;
import org.junit.jupiter.api.Test;

import java.io.FileNotFoundException;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

public class IniMenuReaderTest {
    @Test
    public void testMenuFileReading() throws FileNotFoundException {
        // yes, it's a bit lazy to access an existing, not very relevant test data file. Guilty!
        String iniPath = "../../java_console/io/src/test/java/com/rusefi/io/pin_output_mode_with_and_without_dollar/test_data/rusefi_uaefi.ini";
        IniFileModel model = IniFileReaderUtil.readIniFile(iniPath);
        assertNotNull(model);

        List<MenuModel> menus = model.getMenus();
        assertNotNull(menus);
        assertFalse(menus.isEmpty(), "Menus should not be empty");

        // 1. Verify "Setup" menu exists
        MenuModel setupMenu = menus.stream().filter(m -> m.getName().equals("Setup")).findFirst().orElse(null);
        assertNotNull(setupMenu, "Setup menu should exist");

        // 2. Verify "Vehicle Information" sub-menu exists in Setup
        assertSubMenu(setupMenu.getItems(), "engineChars", "Vehicle Information");

        // 3. Verify "Limits and protection" group exists in Setup
        GroupMenuModel limitsGroup = (GroupMenuModel) setupMenu.getItems().stream()
                .filter(item -> item instanceof GroupMenuModel && ((GroupMenuModel) item).getName().equals("Limits and protection"))
                .findFirst().orElse(null);
        assertNotNull(limitsGroup, "Limits and protection group should exist");

        // 4. Verify "Limits and fallbacks" exists inside that group
        assertSubMenu(limitsGroup.getItems(), "limitsAndFallback", "Limits and fallbacks");

        // 5. Verify "Fuel" menu exists
        MenuModel fuelMenu = menus.stream().filter(m -> m.getName().equals("Fuel")).findFirst().orElse(null);
        assertNotNull(fuelMenu, "Fuel menu should exist");

        // 6. Verify "injectorConfig" sub-menu exists in Fuel
        assertSubMenu(fuelMenu.getItems(), "injectorConfig", "Injection configuration");

        // 7. Verify "VE" exists in Fuel, NOT inside "Cylinder fuel trims" group
        // The group is defined before VE in the INI
        assertSubMenu(fuelMenu.getItems(), "veTableDialog", "VE");

        // 8. subMenu with both enable and visible expressions
        //    subMenu = trigger_advanced, "Advanced Trigger", { 1 }, { uiMode == 0 || uiMode == 1 }
        SubMenuModel triggerAdvanced = findSubMenu(setupMenu.getItems(), "trigger_advanced");
        assertNotNull(triggerAdvanced, "trigger_advanced subMenu should exist");
        assertNotNull(triggerAdvanced.getEnableExpression(), "trigger_advanced should have enableExpression");
        assertNotNull(triggerAdvanced.getVisibleExpression(), "trigger_advanced should have visibleExpression");

        // 9. subMenu with only a visible expression (0 placeholder for enable)
        //    subMenu = stagedInjection, "Staged injection", 0, {isInjectionEnabled}
        SubMenuModel stagedInjection = findSubMenu(fuelMenu.getItems(), "stagedInjection");
        assertNotNull(stagedInjection, "stagedInjection subMenu should exist");
        assertNull(stagedInjection.getEnableExpression(), "stagedInjection should have no enableExpression (0 placeholder)");
        assertNotNull(stagedInjection.getVisibleExpression(), "stagedInjection should have visibleExpression");

        // 10. subMenu with no expressions
        //    subMenu = engineChars, "Vehicle Information"
        SubMenuModel engineChars = findSubMenu(setupMenu.getItems(), "engineChars");
        assertNotNull(engineChars, "engineChars subMenu should exist");
        assertNull(engineChars.getEnableExpression(), "engineChars should have no enableExpression");
        assertNull(engineChars.getVisibleExpression(), "engineChars should have no visibleExpression");

        // 11. groupChildMenu with visible expression (0 placeholder for enable)
        //    groupChildMenu = lambdaProtection, "Lambda protection", 0, { isInjectionEnabled }
        SubMenuModel lambdaProtection = findSubMenu(limitsGroup.getItems(), "lambdaProtection");
        assertNotNull(lambdaProtection, "lambdaProtection groupChildMenu should exist");
        assertNull(lambdaProtection.getEnableExpression(), "lambdaProtection should have no enableExpression (0 placeholder)");
        assertNotNull(lambdaProtection.getVisibleExpression(), "lambdaProtection should have visibleExpression");

        // 12. groupChildMenu with no expressions
        //    groupChildMenu = limitsAndFallback, "Limits and fallbacks"
        SubMenuModel limitsAndFallback = findSubMenu(limitsGroup.getItems(), "limitsAndFallback");
        assertNotNull(limitsAndFallback, "limitsAndFallback groupChildMenu should exist");
        assertNull(limitsAndFallback.getEnableExpression(), "limitsAndFallback should have no enableExpression");
        assertNull(limitsAndFallback.getVisibleExpression(), "limitsAndFallback should have no visibleExpression");

        // 13. "subMenu = std_separator" becomes a SeparatorMenuItem, never a SubMenuModel
        //    subMenu = triggerConfiguration_gap, "Trigger Gap Override", ...
        //    subMenu = std_separator
        //    subMenu = energySystems, "Battery and alternator"
        List<MenuItem> setupItems = setupMenu.getItems();
        int gapIndex = indexOfSubMenu(setupItems, "triggerConfiguration_gap");
        assertTrue(gapIndex >= 0, "triggerConfiguration_gap should exist in Setup");
        assertInstanceOf(SeparatorMenuItem.class, setupItems.get(gapIndex + 1),
                "item after triggerConfiguration_gap should be a separator");
        assertSubMenu(setupItems, "energySystems", "Battery and alternator");
        for (MenuModel menu : menus) {
            assertNull(findSubMenu(menu.getItems(), "std_separator"),
                    "std_separator must not leak into menu '" + menu.getName() + "' as a SubMenuModel");
        }
    }

    @Test
    public void testGroupChildSeparator() throws com.rusefi.ini.reader.IniParsingException {
        String string = "[Menu]\n" +
                "\tmenu = \"Fuel\"\n" +
                "\t\tsubMenu = veTableDialog, \"VE\"\n" +
                "\t\tsubMenu = std_separator\n" +
                "\t\tgroupMenu = \"Cylinder fuel trims\"\n" +
                "\t\t\tgroupChildMenu = fuelTrim1, \"Trim 1\"\n" +
                "\t\t\tgroupChildMenu = std_separator\n" +
                "\t\t\tgroupChildMenu = fuelTrim2, \"Trim 2\"\n";

        IniFileMetaInfo metaInfo = new IniFileMetaInfo() {
            @Override public int getnPages() { return 1; }
            @Override public int getOchBlockSize() { return 0; }
            @Override public String getSignature() { return "test"; }
            @Override public String getPageReadCommand(int pageIndex) { return "r"; }
            @Override public int getPageSize(int pageIndex) { return 100; }
        };
        RawIniFile lines = IniFileReaderUtil.read(new java.io.ByteArrayInputStream(string.getBytes()));
        IniFileModel model = IniFileReaderUtil.readIniFile(lines, "test", metaInfo);

        assertEquals(1, model.getMenus().size());
        MenuModel fuelMenu = model.getMenus().get(0);
        List<MenuItem> items = fuelMenu.getItems();
        assertEquals(3, items.size(), "Fuel menu should have VE, separator, group");
        assertSubMenu(items, "veTableDialog", "VE");
        assertInstanceOf(SeparatorMenuItem.class, items.get(1));

        GroupMenuModel group = (GroupMenuModel) items.get(2);
        List<MenuItem> groupItems = group.getItems();
        assertEquals(3, groupItems.size(), "group should have trim1, separator, trim2");
        assertSubMenu(groupItems, "fuelTrim1", "Trim 1");
        assertInstanceOf(SeparatorMenuItem.class, groupItems.get(1));
        assertSubMenu(groupItems, "fuelTrim2", "Trim 2");
    }

    @Test
    public void testConsecutiveSeparatorsCollapsed() throws com.rusefi.ini.reader.IniParsingException {
        // conditional .ini templating can emit two std_separator lines in a row once the
        // items between them are excluded from the build; only one divider should survive
        String string = "[Menu]\n" +
                "\tmenu = \"Fuel\"\n" +
                "\t\tsubMenu = veTableDialog, \"VE\"\n" +
                "\t\tsubMenu = std_separator\n" +
                "\t\tsubMenu = std_separator\n" +
                "\t\tsubMenu = lambdaDialog, \"Lambda\"\n" +
                "\t\tgroupMenu = \"Cylinder fuel trims\"\n" +
                "\t\t\tgroupChildMenu = fuelTrim1, \"Trim 1\"\n" +
                "\t\t\tgroupChildMenu = std_separator\n" +
                "\t\t\tgroupChildMenu = std_separator\n" +
                "\t\t\tgroupChildMenu = fuelTrim2, \"Trim 2\"\n";

        IniFileMetaInfo metaInfo = new IniFileMetaInfo() {
            @Override public int getnPages() { return 1; }
            @Override public int getOchBlockSize() { return 0; }
            @Override public String getSignature() { return "test"; }
            @Override public String getPageReadCommand(int pageIndex) { return "r"; }
            @Override public int getPageSize(int pageIndex) { return 100; }
        };
        RawIniFile lines = IniFileReaderUtil.read(new java.io.ByteArrayInputStream(string.getBytes()));
        IniFileModel model = IniFileReaderUtil.readIniFile(lines, "test", metaInfo);

        MenuModel fuelMenu = model.getMenus().get(0);
        List<MenuItem> items = fuelMenu.getItems();
        assertEquals(4, items.size(), "Fuel menu should have VE, one separator, Lambda, group");
        assertSubMenu(items, "veTableDialog", "VE");
        assertInstanceOf(SeparatorMenuItem.class, items.get(1));
        assertSubMenu(items, "lambdaDialog", "Lambda");

        GroupMenuModel group = (GroupMenuModel) items.get(3);
        List<MenuItem> groupItems = group.getItems();
        assertEquals(3, groupItems.size(), "group should have trim1, one separator, trim2");
        assertSubMenu(groupItems, "fuelTrim1", "Trim 1");
        assertInstanceOf(SeparatorMenuItem.class, groupItems.get(1));
        assertSubMenu(groupItems, "fuelTrim2", "Trim 2");
    }

    private int indexOfSubMenu(List<MenuItem> items, String key) {
        for (int i = 0; i < items.size(); i++) {
            MenuItem item = items.get(i);
            if (item instanceof SubMenuModel && ((SubMenuModel) item).getKey().equals(key)) {
                return i;
            }
        }
        return -1;
    }

    private SubMenuModel findSubMenu(List<MenuItem> items, String key) {
        return (SubMenuModel) items.stream()
                .filter(item -> item instanceof SubMenuModel && ((SubMenuModel) item).getKey().equals(key))
                .findFirst().orElse(null);
    }

    private void assertSubMenu(List<MenuItem> items, String key, String name) {
        SubMenuModel subMenu = findSubMenu(items, key);
        assertNotNull(subMenu, "SubMenu " + key + " should exist");
        if (name != null) {
            assertEquals(name, subMenu.getName(), "SubMenu name for " + key);
        }
    }
}
