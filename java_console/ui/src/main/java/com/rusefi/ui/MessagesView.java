package com.rusefi.ui;

import com.rusefi.FileLog;
import com.rusefi.core.EngineState;
import com.rusefi.core.MessagesCentral;
import com.rusefi.io.CommandQueue;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import javax.swing.text.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.text.SimpleDateFormat;
import java.util.Date;

public class MessagesView {
    private static final SimpleDateFormat DATE_FORMAT = new SimpleDateFormat(FileLog.DATE_PATTERN);
    private static final int MAX_SIZE = 50000;

    private final Style bold;
    private final Style italic;

    private boolean isPaused;

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

        JPopupMenu menu = createPopupMenu();

        messages.addMouseListener(new MouseAdapter() {
            @Override
            public void mousePressed(MouseEvent e) {
                pop(e);
            }

            @Override
            public void mouseReleased(MouseEvent e) {
                pop(e);
            }

            private void pop(MouseEvent e) {
                if (e.isPopupTrigger())
                    menu.show(e.getComponent(), e.getX(), e.getY());
            }
        });

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

    @NotNull
    private JPopupMenu createPopupMenu() {
        JPopupMenu menu = new JPopupMenu();

        /*
        JMenuItem selectAll = new JMenuItem("Select All");
        selectAll.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                messages.selectAll();
            }
        });
        menu.add(selectAll);
does not work? maybe wrong UI colors since control is not editable?
*/
        JMenuItem copy = new JMenuItem("Copy");
        copy.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                messages.copy();
            }
        });
        menu.add(copy);

        menu.add(new JPopupMenu.Separator());

        JMenuItem pause = new JMenuItem("Pause/Resume");
        pause.addActionListener(e -> setPaused(!isPaused));
        menu.add(pause);

        JMenuItem clear = new JMenuItem("Clear");
        clear.addActionListener(e -> clear());
        menu.add(clear);

        return menu;
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

    /**
     * Sets different font style depending on message source
     * <p>
     * this is ugly as hell, but that's so much better then nothing...
     */
    private AttributeSet getStyle(Class clazz) {
        if (clazz == CommandQueue.COMMAND_QUEUE_CLASS)
            return bold;
        if (clazz == EngineState.ENGINE_STATE_CLASS)
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

    public void setPaused(boolean isPaused) {
        this.isPaused = isPaused;
    }

    public boolean isPaused() {
        return isPaused;
    }
}