package com.rusefi;

import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;
import java.util.List;

public class TunerStudioIntegration {
    /**
     * these magic class names are specific to TS build
     * At the moment we use 3.1.02 names
     * http://www.tunerstudio.com/downloads2/TunerStudioMS_Setup_v3.3.00.exe
     * https://github.com/rusefi/rusefi_external_utils/blob/master/TunerStudio/TunerStudioMS_Setup_v3.1.02.exe
     */
    private static final String TUNER_STUDIO = "TunerStudio";

    public static boolean DISCOVERY_MODE = true;
    private static final String VERSION = "3.3.0";
    private static final String TOP_LEVEL_BUTTON_NAME = "bq.b";
    //private static final String TS_DIALOG = "com.efiAnalytics.ui.dg";
    private static final String TS_POPUP_MAIN_MENU = "aP.gX";

    static Frame findMainFrame() throws InterruptedException {
        while (true) {
            Frame[] all = JFrame.getFrames();

            for (Frame frame : all) {
                String frameTitle = frame.getTitle();
                System.out.println("findMainFrame: I see " + frameTitle);
                if (frameTitle.contains(TUNER_STUDIO)) {
                    if (!frameTitle.contains(VERSION)) {
                        System.out.println("Title does not look right " + frameTitle + " not " + VERSION);
                        System.exit(-1);
                    }
                    System.out.println("Found main frame for expected version: " + frameTitle);
                    return frame;
                }
            }
            Thread.sleep(1000);
        }
    }

    static boolean isTopLevelMenuButton(Component component) {
        return component.getClass().getName().equals(TOP_LEVEL_BUTTON_NAME);
    }

    static java.util.List<JMenuItem> findMenuItems(Frame frame) {
        List<JMenuItem> menuItems = new ArrayList<>();
        UiUtils.visitComponents(frame, "Just clicked ", (parent, component) -> {
            if (component instanceof JMenuItem) {
                if (DISCOVERY_MODE) {
                    // search for 'Menu item Vehicle Information'
                    System.out.println("menu item " + component + " class=" + component.getClass());
                }

                boolean isOurs = component.getClass().getName().endsWith(TS_POPUP_MAIN_MENU);
                //boolean isOurs = true;
                if (isOurs) {
                    JMenuItem menuItem = (JMenuItem) component;
                    System.out.println("Menu item " + menuItem.getText());
                    menuItems.add(menuItem);
                }
            }
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
