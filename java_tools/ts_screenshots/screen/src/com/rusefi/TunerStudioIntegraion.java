package com.rusefi;

import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;
import java.util.List;

public class TunerStudioIntegraion {
    static Frame findMainFrame() throws InterruptedException {
        while (true) {
            Frame[] all = JFrame.getFrames();

            for (Frame frame : all) {
                System.out.println("I see " + frame.getTitle());

                if (frame.getTitle().contains("TunerStudio")) {
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
            if (component instanceof JMenuItem && component.getClass().getName().endsWith("aH.gc")) {
                JMenuItem menuItem = (JMenuItem) component;
                System.out.println("Menu item " + menuItem.getText());
                menuItems.add(menuItem);
            }
        });
        return menuItems;
    }

    static JDialog findDynamicDialog() {
        for (Window d : Dialog.getWindows()) {
            if (d.getClass().getName().equals(ScreenGenerator.TS_DIALOG) && d.isVisible()) {
                return (JDialog) d;
            }
        }
        return null;
    }
}
