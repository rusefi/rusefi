package com.rusefi.ui;

import com.rusefi.core.MessagesCentral;
import com.rusefi.io.CommandQueue;
import com.rusefi.io.serial.PortHolder;
import com.rusefi.ui.storage.Node;
import com.rusefi.ui.util.UiUtils;
import com.rusefi.ui.widgets.AnyCommand;

import javax.swing.*;
import javax.swing.text.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.text.SimpleDateFormat;
import java.util.Date;

import static com.rusefi.ui.util.LocalizedMessages.CLEAR;
import static com.rusefi.ui.util.LocalizedMessages.PAUSE;

/**
 * This panel displays plain-text 'msg' plain-text debug messages
 * <p/>
 * <p/>
 * Date: 4/27/13
 * (c) Andrey Belomutskiy
 *
 * @see AnyCommand
 */
public class MessagesPanel {
    private static final SimpleDateFormat DATE_FORMAT = new SimpleDateFormat("yyyy-MM-dd HH_mm");
    private static final int MAX_SIZE = 50000;
    private static final String FONT_SIZE = "font_size";
    private static final String FONT_NAME = "font_name";

    private final AnyCommand anyCommand;
    private final JTextPane messages = new JTextPane() {
        @Override
        public void setVisible(boolean aFlag) {
            super.setVisible(aFlag);
// todo: get focus on startup somehow
//            anyCommand.getText().requestFocus();
        }
    };
    private boolean isPaused;
    private final Style bold;
    private final Style italic;
    private final JPanel buttonPanel = new JPanel(new FlowLayout(FlowLayout.CENTER, 5, 0));
    private final JScrollPane messagesScroll = new JScrollPane(messages, JScrollPane.VERTICAL_SCROLLBAR_ALWAYS, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);

    public MessagesPanel(Node config, boolean listenToCommands) {
        JPanel middlePanel = new JPanel(new BorderLayout());
        middlePanel.add(messagesScroll, BorderLayout.CENTER);
        buttonPanel.setBorder(BorderFactory.createLineBorder(Color.red));

        StyledDocument d = (StyledDocument) messages.getDocument();
        bold = d.addStyle("StyleName", null);
        bold.addAttribute(StyleConstants.CharacterConstants.Bold, Boolean.TRUE);

        italic = d.addStyle("StyleName", null);
        italic.addAttribute(StyleConstants.CharacterConstants.Italic, Boolean.TRUE);

        MessagesCentral.getInstance().addListener(new MessagesCentral.MessageListener() {
            @Override
            public void onMessage(Class clazz, String message) {
                final String date = DATE_FORMAT.format(new Date());
                if (!isPaused)
                    append(date + ": " + clazz.getSimpleName() + ": " + message, clazz);
            }
        });

        JButton resetButton = new JButton(CLEAR.getMessage());
        resetButton.setMnemonic('c');
        resetButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent event) {
                Document d = messages.getDocument();
                clearMessages(d);
            }
        });

        final JButton pauseButton = new JButton(PAUSE.getMessage());
        pauseButton.setMnemonic('p');
        pauseButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                isPaused = !isPaused;
                UiUtils.setPauseButtonText(pauseButton, isPaused);
            }
        });

        buttonPanel.add(resetButton);
        buttonPanel.add(pauseButton);
        anyCommand = new AnyCommand(config, listenToCommands);
        buttonPanel.add(anyCommand.getContent());
    }

    private void clearMessages(Document d) {
        try {
            d.remove(0, d.getLength());
        } catch (BadLocationException e) {
            throw new IllegalStateException(e);
        }
    }

    private void append(String line, Class clazz) {
        Document d = messages.getDocument();
        if (d.getLength() > MAX_SIZE)
            clearMessages(d);
        try {
            d.insertString(d.getLength(), line + "\r\n", getStyle(clazz));
            messages.select(d.getLength(), d.getLength());
        } catch (BadLocationException e) {
            throw new IllegalStateException(e);
        }
    }

    private AttributeSet getStyle(Class clazz) {
        /**
         * this is ugly as hell, but that's so much better then nothing...
         */
        if (clazz == CommandQueue.class)
            return bold;
        if (clazz == PortHolder.class)
            return italic;
        return null;
    }

    public JPanel getButtonPanel() {
        return buttonPanel;
    }

    public JScrollPane getMessagesScroll() {
        return messagesScroll;
    }

    public Font getFont() {
        return messages.getFont();
    }

    public void setFont(Font font, Node config) {
        messages.setFont(font);
        config.setProperty(FONT_SIZE, font.getSize());
        config.setProperty(FONT_NAME, font.getName());
    }

    public void loadFont(Node config) {
        Font f = getFont();
        int size = config.getIntProperty(FONT_SIZE, f.getSize());
        String name = config.getProperty(FONT_NAME, f.getName());
        setFont(new Font(f.getName(), f.getStyle(), size), config);
    }
}
