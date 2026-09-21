package com.rusefi.ui;

import com.opensr5.ini.GroupMenuModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.MenuItem;
import com.opensr5.ini.MenuModel;
import com.opensr5.ini.SubMenuModel;

import javax.swing.*;
import java.util.List;
import java.util.function.Supplier;

/** Manages the lazy sniffer tab as the connected board changes. All calls run on the EDT. */
public class SlcanTabController {
    private final JTabbedPane tabs;
    private final Supplier<SlcanTab> factory;
    private IniFileModel currentIni;
    private JComponent content;
    private SlcanTab sniffer;

    public SlcanTabController(JTabbedPane tabs, Supplier<SlcanTab> factory) {
        this.tabs = tabs;
        this.factory = factory;
    }

    public void update(IniFileModel connectedIni, boolean forceShow) {
        boolean visible = forceShow || supportsSniffer(connectedIni);
        if (content != null && (!visible || currentIni != connectedIni)) {
            if (sniffer != null) {
                sniffer.close();
                sniffer = null;
            }
            tabs.remove(content);
            content = null;
        }
        currentIni = connectedIni;
        if (visible && content == null) {
            content = new InitOnFirstPaintPanel() {
                @Override
                protected JPanel createContent() {
                    sniffer = factory.get();
                    return sniffer.getContent();
                }
            }.getContent();
            tabs.addTab("SLCAN Sniffer", content);
        }
    }

    static boolean supportsSniffer(IniFileModel ini) {
        if (ini == null) {
            return false;
        }
        // The dialog/fields exist even on unsupported boards. Only the menu entry
        // is emitted conditionally by ts_show_canbus_sniffer in the board prepend.
        for (MenuModel menu : ini.getMenus()) {
            if (containsSniffer(menu.getItems())) {
                return true;
            }
        }
        return false;
    }

    private static boolean containsSniffer(List<MenuItem> items) {
        for (MenuItem item : items) {
            if (item instanceof SubMenuModel
                && "canBusSniffer".equals(((SubMenuModel) item).getKey())) {
                return true;
            }
            if (item instanceof GroupMenuModel
                && containsSniffer(((GroupMenuModel) item).getItems())) {
                return true;
            }
        }
        return false;
    }
}
