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
        // This is what we are looking for
        assertEquals("main", model.getMenuDialog(), "menuDialog should be loaded");

        List<MenuModel> menus = model.getMenus();
        assertNotNull(menus);
        assertFalse(menus.isEmpty(), "Menus should not be empty");

        // 1. Verify "Setup" menu exists
        MenuModel setupMenu = menus.stream().filter(m -> m.getName().equals("&Setup")).findFirst().orElse(null);
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
    }

    private void assertSubMenu(List<MenuItem> items, String key, String name) {
        SubMenuModel subMenu = (SubMenuModel) items.stream()
                .filter(item -> item instanceof SubMenuModel && ((SubMenuModel) item).getKey().equals(key))
                .findFirst().orElse(null);
        assertNotNull(subMenu, "SubMenu " + key + " should exist");
        if (name != null) {
            assertEquals(name, subMenu.getName(), "SubMenu name for " + key);
        }
    }

    private void printMenuItem(MenuItem item, String indent) {
        if (item instanceof SubMenuModel) {
            SubMenuModel subMenu = (SubMenuModel) item;
            System.out.println(indent + "SubMenu: " + subMenu.getName() + " (" + subMenu.getKey() + ")");
        } else if (item instanceof GroupMenuModel) {
            GroupMenuModel group = (GroupMenuModel) item;
            System.out.println(indent + "Group: " + group.getName());
            for (MenuItem child : group.getItems()) {
                printMenuItem(child, indent + "  ");
            }
        }
    }
}
