package com.rusefi.ui.widgets;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

public class PopupMenuButton extends JButton {

    // Is the popup showing or not?
    private boolean isShowingPopup = false;

    public PopupMenuButton(String text, final JPopupMenu menu) {
        super(text);
        // FocusListener on the JPopupMenu
        menu.addFocusListener(new FocusListener() {
            @Override
            public void focusLost(FocusEvent e) {
                System.out.println("LOST FOCUS");
                isShowingPopup = false;
            }

            @Override
            public void focusGained(FocusEvent e) {
                System.out.println("GAINED FOCUS");
            }
        });

        // ComponentListener on the JPopupMenu
        menu.addComponentListener(new ComponentListener() {
            @Override
            public void componentShown(ComponentEvent e) {
                System.out.println("SHOWN");
            }

            @Override
            public void componentResized(ComponentEvent e) {
                System.out.println("RESIZED");
            }

            @Override
            public void componentMoved(ComponentEvent e) {
                System.out.println("MOVED");
            }

            @Override
            public void componentHidden(ComponentEvent e) {
                System.out.println("HIDDEN");
            }
        });

        // ActionListener on the JButton
        addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                System.out.println("isShowingPopup: " + isShowingPopup);
                if (isShowingPopup) {
                    menu.requestFocus();
                    isShowingPopup = false;
                } else {
                    Component c = (Component) e.getSource();
                    menu.show(c, -1, c.getHeight());
                    isShowingPopup = true;
                }
            }
        });

        // Skip when navigating with TAB.
        setFocusable(true); // Was false first and should be false in the end.

        menu.setFocusable(true);


    }


}