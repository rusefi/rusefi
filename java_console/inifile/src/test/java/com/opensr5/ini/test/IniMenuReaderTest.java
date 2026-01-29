package com.opensr5.ini.test;

import com.opensr5.ini.*;
import com.rusefi.ini.reader.IniFileReaderUtil;
import org.junit.jupiter.api.Test;

import java.io.FileNotFoundException;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertFalse;

public class IniMenuReaderTest {
    @Test
    public void testMenuFileReading() throws FileNotFoundException {
        // yes, it's a bit lazy to access an existing, not very relevant test data file. Guilty!
        String iniPath = "../../java_console/io/src/test/java/com/rusefi/io/pin_output_mode_with_and_without_dollar/test_data/rusefi_uaefi.ini";
        IniFileModel model = IniFileReaderUtil.readIniFile(iniPath);
        assertNotNull(model);
        // This is what we are looking for
        assertNotNull(model.getMenuDialog(), "menuDialog should be loaded");

        List<MenuModel> menus = model.getMenus();
        assertNotNull(menus);
        assertFalse(menus.isEmpty(), "Menus should not be empty");

        for (MenuModel menu : menus) {
            System.out.println("Menu: " + menu.getName());
            for (MenuItem item : menu.getItems()) {
                printMenuItem(item, "  ");
            }
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
