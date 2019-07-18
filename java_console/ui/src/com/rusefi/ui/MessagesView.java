package com.rusefi.ui;

import com.rusefi.FileLog;
import com.rusefi.core.MessagesCentral;
import com.rusefi.io.CommandQueue;
import com.rusefi.io.serial.PortHolder;

import javax.swing.*;
import javax.swing.text.*;
import java.text.SimpleDateFormat;
import java.util.Date;

public class MessagesView {
    private static final SimpleDateFormat DATE_FORMAT = new SimpleDateFormat(FileLog.DATE_PATTERN);
    private static final int MAX_SIZE = 50000;
    private final Style bold;
    private final Style italic;
    protected boolean isPaused;

    protected final JTextPane messages = new JTextPane() {
        @Override
        public void setVisible(boolean aFlag) {
            super.setVisible(aFlag);
// todo: get focus on startup somehow
//            anyCommand.getText().requestFocus();
        }
    };
    public final JScrollPane messagesScroll = new JScrollPane(messages, JScrollPane.VERTICAL_SCROLLBAR_ALWAYS, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);

    public MessagesView() {
        messages.setEditable(false);
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


    private void clearMessages(Document d) {
        try {
            d.remove(0, d.getLength());
        } catch (BadLocationException e) {
            throw new IllegalStateException(e);
        }
    }

    public void clear() {
        Document d = messages.getDocument();
        clearMessages(d);
    }
}