package com.rusefi.ui;

import com.rusefi.FileLog;
import com.rusefi.core.EngineState;
import com.rusefi.core.MessagesCentral;
import com.rusefi.io.CommandQueue;
import com.rusefi.core.preferences.storage.Node;
import com.rusefi.ui.util.UiUtils;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import javax.swing.text.*;
import java.text.SimpleDateFormat;
import java.util.Date;

public class MessagesView {
    private static final SimpleDateFormat DATE_FORMAT = new SimpleDateFormat(FileLog.DATE_PATTERN);

    private final Style bold;
    private final Style italic;
    private final Node config;

    private boolean isPaused;

    protected final JTextPane messages = new JTextPane();
    public final JScrollPane messagesScroll = new JScrollPane(messages, JScrollPane.VERTICAL_SCROLLBAR_ALWAYS, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);

    public Listener listener = Listener.VOID;

    public MessagesView(Node config) {
        this.config = config;
        messages.setEditable(false);

        UiUtils.installPopupMenu(createPopupMenu(), messages);

        StyledDocument d = (StyledDocument) messages.getDocument();
        bold = d.addStyle("StyleName", null);
        bold.addAttribute(StyleConstants.CharacterConstants.Bold, Boolean.TRUE);

        italic = d.addStyle("StyleName", null);
        italic.addAttribute(StyleConstants.CharacterConstants.Italic, Boolean.TRUE);

        MessagesCentral.getInstance().addListener(new MessagesCentral.MessageListener() {
            @Override
            public void onMessage(Class clazz, String message) {
                final String date = DATE_FORMAT.format(new Date());
                if (!isPaused) {
                    append(date + ": " + clazz.getSimpleName() + ": " + message, clazz);
                    listener.onMessage(message);
                }
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
        menu.add(UiUtils.createCopyMenu(messages));

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
        int logSizeControl = LogSizeControl.getValue(config);
        if (d.getLength() > logSizeControl)
            clearMessages(d, logSizeControl);
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
        if (clazz == CommandQueue.class)
            return bold;
        if (clazz == EngineState.class)
            return italic;
        return null;
    }


    private void clearMessages(Document d, int logSizeControl) {
        try {
            d.remove(0, d.getLength() - logSizeControl / 2);
        } catch (BadLocationException e) {
            throw new IllegalStateException(e);
        }
    }

    public void clear() {
        Document d = messages.getDocument();
        clearMessages(d, 0);
    }

    public void setPaused(boolean isPaused) {
        this.isPaused = isPaused;
    }

    public boolean isPaused() {
        return isPaused;
    }

    public interface Listener {
        Listener VOID = message -> {
        };

        void onMessage(String message);
    }
}