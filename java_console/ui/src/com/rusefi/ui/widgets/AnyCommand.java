package com.rusefi.ui.widgets;

import com.rusefi.io.CommandQueue;
import com.rusefi.ui.storage.Node;

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
public class AnyCommand {
    private static final int COMMAND_CONFIRMATION_TIMEOUT = 1000;
    public static final String KEY = "last_value";
    private final JTextField text = new JTextField() {
        @Override
        public Dimension getPreferredSize() {
            Dimension size = super.getPreferredSize();
            return new Dimension(200, size.height);
        }
    };

    private final Node config;
    private JPanel content = new JPanel(new FlowLayout(FlowLayout.LEFT));

    public AnyCommand(final Node config) {
        this(config, config.getProperty(KEY, ""));
    }

    public AnyCommand(final Node config, String defaultCommand) {
        this.config = config;
        text.setText(defaultCommand);
        content.setBorder(BorderFactory.createLineBorder(Color.PINK));
        content.add(new JLabel("Command: "));
        content.add(text);

        text.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String cmd = text.getText();
                if (!isValidInput(text))
                    return;
                CommandQueue.getInstance().write(cmd.toLowerCase(), COMMAND_CONFIRMATION_TIMEOUT);
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
                config.setProperty(KEY, text.getText());
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
    }

    public JTextField getText() {
        return text;
    }

//    @Override
//    public boolean requestFocusInWindow() {
//        return text.requestFocusInWindow();
//    }

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

    public JComponent getContent() {
        return content;
    }

    public void setContent(JPanel content) {
        this.content = content;
    }
}
