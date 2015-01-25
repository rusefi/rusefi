package com.rusefi.ui;

import com.irnems.core.MessagesCentral;
import com.rusefi.io.CommandQueue;
import com.rusefi.io.serial.PortHolder;
import com.rusefi.ui.widgets.AnyCommand;

import javax.swing.*;
import javax.swing.text.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.text.SimpleDateFormat;
import java.util.Date;

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

    private final JTextPane messages = new JTextPane();
    private boolean isPaused;
    private final Style bold;
    private final Style italic;
    private final JPanel buttonPanel = new JPanel(new FlowLayout(FlowLayout.CENTER, 5, 0));
    private final JScrollPane messagesScroll = new JScrollPane(messages, JScrollPane.VERTICAL_SCROLLBAR_ALWAYS, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);

    public MessagesPanel() {
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

        JButton resetButton = new JButton("clear");
        resetButton.setMnemonic('c');
        resetButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent event) {
                Document d = messages.getDocument();
                clearMessages(d);
            }
        });

        final JButton pauseButton = new JButton("pause");
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
        buttonPanel.add(new AnyCommand());
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
}
