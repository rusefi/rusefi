package com.rusefi.ui;

import com.irnems.core.MessagesCentral;
import com.irnems.ui.widgets.AnyCommand;
import com.irnems.ui.widgets.IdleLabel;

import javax.swing.*;
import javax.swing.text.BadLocationException;
import javax.swing.text.Document;
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
public class MsgPanel extends JPanel {
    private static final SimpleDateFormat DATE_FORMAT = new SimpleDateFormat("yyyy-MM-dd HH_mm");

    private final JTextPane msg = new JTextPane();
    private boolean isPaused;

    public MsgPanel(boolean needsRpmControl) {
        super(new BorderLayout());
        setBorder(BorderFactory.createLineBorder(Color.green));
        JScrollPane pane = new JScrollPane(msg, JScrollPane.VERTICAL_SCROLLBAR_ALWAYS, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
        add(pane, BorderLayout.CENTER);
        MessagesCentral.getInstance().addListener(new MessagesCentral.MessageListener() {
            @Override
            public void onMessage(Class clazz, String message) {
                final String date = DATE_FORMAT.format(new Date());
                if (!isPaused)
                    append(date + ": " + clazz.getSimpleName() + ": " + message);
            }
        });

        JButton resetButton = new JButton("clear");
        resetButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent event) {
                Document d = msg.getDocument();
                clearMessages(d);
            }
        });

        final JButton pauseButton = new JButton("pause");
        pauseButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                isPaused = !isPaused;
                UiUtils.setPauseButtonText(pauseButton, isPaused);
            }
        });

        JPanel buttonPanel = new JPanel(new FlowLayout(FlowLayout.CENTER, 5, 0));
        buttonPanel.add(resetButton);
        buttonPanel.add(pauseButton);
        buttonPanel.add(new AnyCommand());
        if (needsRpmControl)
            buttonPanel.add(new RpmControl().getContent());
        add(buttonPanel, BorderLayout.NORTH);

        JPanel statsPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));

        statsPanel.add(new RpmControl().getContent());
        statsPanel.add(new IdleLabel());

        add(statsPanel, BorderLayout.SOUTH);
    }

    private void clearMessages(Document d) {
        try {
            d.remove(0, d.getLength());
        } catch (BadLocationException e) {
            throw new IllegalStateException(e);
        }
    }

    private void append(String line) {
        Document d = msg.getDocument();
        if (d.getLength() > 10000)
            clearMessages(d);
        try {
            d.insertString(d.getLength(), line + "\r\n", null);
            msg.select(d.getLength(), d.getLength());
        } catch (BadLocationException e) {
            throw new IllegalStateException(e);
        }
    }

    @Override
    public Dimension getPreferredSize() {
        Dimension size = super.getPreferredSize();
        return new Dimension(250, size.height);
    }
}
