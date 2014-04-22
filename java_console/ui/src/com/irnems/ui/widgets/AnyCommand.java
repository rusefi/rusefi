package com.irnems.ui.widgets;

import com.rusefi.io.CommandQueue;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

/**
 * Date: 3/20/13
 * (c) Andrey Belomutskiy
 */
public class AnyCommand extends JPanel {
    public AnyCommand() {
        setBorder(BorderFactory.createLineBorder(Color.PINK));
        setLayout(new FlowLayout(FlowLayout.LEFT));
        add(new JLabel("Command: "));
        final JTextField text = createCommandControl();
        add(text);
    }

    public static JTextField createCommandControl() {
        final JTextField text = new JTextField() {
            @Override
            public Dimension getPreferredSize() {
                Dimension size = super.getPreferredSize();
                return new Dimension(200, size.height);
            }
        };
        text.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String cmd = text.getText();
                int timeout = isSlowCommand(cmd) ? 5000 : 300;
                CommandQueue.getInstance().write(cmd, timeout);
            }
        });
        // todo: limit the length of text in the text field
        return text;
    }

    private static boolean isSlowCommand(String cmd) {
        String lc = cmd.toLowerCase();
        return lc.startsWith("set_engine_type") || lc.startsWith("writeconfig") || lc.startsWith("showconfig");
    }
}
