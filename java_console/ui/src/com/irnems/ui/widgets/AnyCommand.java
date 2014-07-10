package com.irnems.ui.widgets;

import com.rusefi.io.CommandQueue;

import javax.swing.*;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

/**
 * Date: 3/20/13
 * (c) Andrey Belomutskiy
 */
public class AnyCommand extends JPanel {
    public AnyCommand() {
//        setBorder(BorderFactory.createLineBorder(Color.PINK));
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
                if (!isValidInput(text))
                    return;
                int timeout = isSlowCommand(cmd) ? 5000 : 300;
                CommandQueue.getInstance().write(cmd.toLowerCase(), timeout);
            }
        });
        text.getDocument().addDocumentListener(new DocumentListener() {
            @Override
            public void insertUpdate(DocumentEvent e) {
                changedUpdate(null);
            }

            @Override
            public void removeUpdate(DocumentEvent e) {

            }

            @Override
            public void changedUpdate(DocumentEvent e) {
                boolean isOk = isValidInput(text);
                text.setBorder(isOk ? null : BorderFactory.createLineBorder(Color.red));
            }
        });

//        text.setInputVerifier(new InputVerifier() {
//            @Override
//            public boolean verify(JComponent input) {
//                boolean isOk = isValidInput(text);
//                text.setBorder(isOk ? null : BorderFactory.createLineBorder(Color.red));
//                return isOk;
//            }
//        });
        // todo: limit the length of text in the text field
        return text;
    }

    private static boolean isValidInput(JTextField text) {
        boolean isOk = true;
        for (char c : text.getText().toCharArray()) {
            if (c > 127) {
                /**
                 * https://sourceforge.net/p/rusefi/tickets/63/
                 * only English characters are accepted - we need to reject two-byte unicode stuff
                 */
                isOk = false;
                break;
            }
        }
        return isOk;
    }

    private static boolean isSlowCommand(String cmd) {
        String lc = cmd.toLowerCase();
        return lc.startsWith("set_engine_type") || lc.startsWith("writeconfig") || lc.startsWith("showconfig")
                || lc.startsWith("perftest");
    }
}
