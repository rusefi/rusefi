package com.rusefi.ui.widgets;

import javax.swing.*;
import javax.swing.plaf.basic.BasicArrowButton;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

/**
 * A button with a default action on the left and a dropdown for advanced actions on the right.
 */
public class JSplitButton extends JPanel {
    private final JButton mainButton;
    private final JButton arrowButton;
    private JPopupMenu popupMenu;

    public JSplitButton(String text) {
        this(text, null);
    }

    public JSplitButton(String text, Icon icon) {
        setLayout(new BorderLayout());

        mainButton = new JButton(text, icon);
        arrowButton = new BasicArrowButton(SwingConstants.SOUTH);

        add(mainButton, BorderLayout.CENTER);
        add(arrowButton, BorderLayout.EAST);

        arrowButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                showPopup();
            }
        });
    }

    public void setIcon(Icon icon) {
        mainButton.setIcon(icon);
    }

    public void setPopupMenu(JPopupMenu popupMenu) {
        this.popupMenu = popupMenu;
    }

    public void addActionListener(ActionListener listener) {
        mainButton.addActionListener(listener);
    }

    private void showPopup() {
        if (popupMenu != null) {
            popupMenu.show(this, 0, getHeight());
        }
    }

    @Override
    public void setToolTipText(String text) {
        mainButton.setToolTipText(text);
        arrowButton.setToolTipText(text);
    }

    @Override
    public void setEnabled(boolean enabled) {
        super.setEnabled(enabled);
        mainButton.setEnabled(enabled);
        arrowButton.setEnabled(enabled);
    }
}
