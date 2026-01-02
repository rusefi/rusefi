package com.rusefi;

import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;
import java.util.List;

import com.rusefi.wizard.*;

public class TunerStudioIntegration {

    public static boolean DISCOVERY_MODE = true;

    static Frame findMainFrame() throws InterruptedException {
        while (true) {
            Frame frame = TsAccess.findTsMainFrame();
            if (frame != null) {
                if (TsAccess.getVersion() == TsAccess.KnownVersion.UNKNOWN) {
                    System.out.println("Title does not look right " + frame.getTitle());
                    System.exit(-1);
                }
                return frame;
            }

            Thread.sleep(1000);
        }
    }

    static boolean isTopLevelMenuButton(Component component) {
        return component.getClass().getName().equals(TsAccess.getTsTopLevelButtonClass());
    }

    static java.util.List<JMenuItem> findMenuItems(Frame frame) {
        List<JMenuItem> menuItems = new ArrayList<>();
        UiUtils.visitComponents(frame, "Just clicked ", (parent, component) -> {
            if (component instanceof JMenuItem) {
                if (DISCOVERY_MODE) {
                    // search for 'Menu item Vehicle Information'
                    System.out.println("menu item " + component + " class=" + component.getClass());
                }

                boolean isOurs = component.getClass().getName().endsWith(TsAccess.getTsPopupMainMenuClassSuffix());
                //boolean isOurs = true;
                if (isOurs) {
                    JMenuItem menuItem = (JMenuItem) component;
                    System.out.println("Menu item " + menuItem.getText());
                    menuItems.add(menuItem);
                }
            }
            return true;
        });
        return menuItems;
    }

    static JDialog findDynamicDialog() {
        for (Window d : Dialog.getWindows()) {
            if (d.isVisible() && d instanceof JDialog) {
                if (DISCOVERY_MODE) {
                    System.out.println("JDialog " + d + " class=" + d.getClass());
                }

                //boolean isOk = d.getClass().getName().equals(TS_DIALOG);
                boolean isOk = true;
                if (isOk) {
                    return (JDialog) d;
                }
            }
        }
        return null;
    }
}
