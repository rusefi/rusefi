package com.rusefi.ui;

import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.ui.widgets.JSplitButton;

import javax.swing.*;
import java.awt.*;

public class JSplitButtonSandbox {
    public static void main(String[] args) {
        JFrame frame = new JFrame("JSplitButton Sandbox");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(400, 200);
        frame.setLayout(new FlowLayout());

        JSplitButton splitButton = new JSplitButton("Default Action", AutoupdateUtil.loadIcon("upload48.png"));
        splitButton.addActionListener(e -> System.out.println("Main action clicked!"));

        JPopupMenu menu = new JPopupMenu();
        menu.add(new JMenuItem("Advanced Action 1")).addActionListener(e -> System.out.println("Advanced 1 clicked!"));
        menu.add(new JMenuItem("Advanced Action 2")).addActionListener(e -> System.out.println("Advanced 2 clicked!"));
        splitButton.setPopupMenu(menu);

        frame.add(splitButton);

        frame.setVisible(true);
    }
}
