package com.rusefi;

import com.rusefi.io.CommandQueue;
import com.rusefi.ui.util.UiUtils;
import org.jetbrains.annotations.NotNull;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionListener;

/**
 * (c) Andrey Belomutskiy 2013-2019
 */
abstract class CommandControl {
    public static final String TEST = "Test";
    public static final String SET = "Set";
    protected final JPanel panel = new JPanel(new FlowLayout(FlowLayout.RIGHT, 5, 0));
    final JButton test;

    public CommandControl(String labelText, String iconFileName, String buttonText, JComponent... components) {
        ImageIcon icon = UiUtils.loadIcon(iconFileName);
        JPanel rightVerticalPanel = new JPanel(new VerticalFlowLayout());
        rightVerticalPanel.add(new JLabel(labelText));
        for (JComponent component : components)
            rightVerticalPanel.add(component);
        test = new JButton(buttonText);
        rightVerticalPanel.add(test);

        panel.add(new JLabel(icon));
        panel.add(rightVerticalPanel);

        int GAP = 3;

        panel.setBorder(BorderFactory.createCompoundBorder(BorderFactory.createLineBorder(Color.black), BorderFactory.createEmptyBorder(GAP, GAP, GAP, GAP)));

        test.addActionListener(createButtonListener());
    }

    @NotNull
    protected ActionListener createButtonListener() {
        return e -> CommandQueue.getInstance().write(getCommand());
    }

    protected abstract String getCommand();

    public Component getContent() {
        return UiUtils.wrap(panel);
    }
}
