package com.rusefi;

import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;
import java.util.List;

public class TunerStudioIntegration {
    /**
     * these magic class names are specific to TS build
     * At the moment we use 3.1.02 names
     * http://www.tunerstudio.com/downloads2/TunerStudioMS_Setup_v3.1.02.exe
     */
    private static final String VERSION = "3.1.02";
    private static final String TS_DIALOG = "com.efiAnalytics.ui.dg";
    private static final String TS_POPUP_MAIN_MENU = "aH.gc";
    private static final String TUNER_STUDIO = "TunerStudio";

    static Frame findMainFrame() throws InterruptedException {
        while (true) {
            Frame[] all = JFrame.getFrames();

            for (Frame frame : all) {
                String frameTitle = frame.getTitle();
                System.out.println("I see " + frameTitle);
                if (frameTitle.contains(TUNER_STUDIO)) {
                    if (!frameTitle.contains(VERSION)) {
                        System.out.println("Title does not look right " + frameTitle + " not " + VERSION);
                        System.exit(-1);
                    }
                    return frame;
                }
            }
            Thread.sleep(1000);
        }
    }

    static boolean isTopLevelMenuButton(Component component) {
        return component instanceof bi.b;
    }

    static java.util.List<JMenuItem> findMenuItems(Frame frame) {
        List<JMenuItem> menuItems = new ArrayList<>();
        UiUtils.visitComponents(frame, "Just clicked ", (parent, component) -> {
            if (component instanceof JMenuItem && component.getClass().getName().endsWith(TS_POPUP_MAIN_MENU)) {
                JMenuItem menuItem = (JMenuItem) component;
                System.out.println("Menu item " + menuItem.getText());
                menuItems.add(menuItem);
            }
        });
        return menuItems;
    }

    static JDialog findDynamicDialog() {
        for (Window d : Dialog.getWindows()) {
            if (d.getClass().getName().equals(TS_DIALOG) && d.isVisible()) {
                return (JDialog) d;
            }
        }
        return null;
    }
}
