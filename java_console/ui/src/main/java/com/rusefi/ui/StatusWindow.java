package com.rusefi.ui;

import com.rusefi.FileLog;
import com.rusefi.ui.util.FrameHelper;
import com.rusefi.ui.util.UiUtils;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.awt.*;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 * 3/7/2015
 */
public class StatusWindow implements StatusConsumer {
    // todo: extract driver from console bundle? find a separate driver bundle?
    private final JTextArea logTextArea = new JTextArea();
    private final JPanel content = new JPanel(new BorderLayout());
    private final JLabel bottomStatusLabel = new JLabel();
    private final JScrollPane messagesScroll = new JScrollPane(logTextArea, JScrollPane.VERTICAL_SCROLLBAR_ALWAYS, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED) {
        @Override
        public Dimension getPreferredSize() {
            return new Dimension(400, 400);
        }
    };
    @NotNull
    protected final FrameHelper frameHelper = new FrameHelper();

    public StatusWindow() {
        logTextArea.setLineWrap(true);
        content.add(messagesScroll, BorderLayout.CENTER);
        content.add(bottomStatusLabel, BorderLayout.SOUTH);
    }

    @NotNull
    public FrameHelper getFrameHelper() {
        return frameHelper;
    }

    public JPanel getContent() {
        return content;
    }

    public JFrame getFrame() {
        return frameHelper.getFrame();
    }

    public void showFrame(String title) {
        frameHelper.getFrame().setTitle(title);
        frameHelper.showFrame(content, false);
        UiUtils.centerWindow(frameHelper.getFrame());
        logTextArea.setText(""); // let's remove stuff from previous invocation
    }

    @Override
    public void appendMsg(final String string) {
        SwingUtilities.invokeLater(() -> {
            String s = string.replaceAll(Character.toString((char)219), "");
            FileLog.MAIN.logLine(s);
            logTextArea.append(s + "\r\n");
            UiUtils.trueLayout(logTextArea);
        });
    }

    public void setStatus(String status) {
        bottomStatusLabel.setText(status);
    }
}
