package com.rusefi.ui;

import com.rusefi.ui.storage.Node;
import com.rusefi.ui.util.UiUtils;
import com.rusefi.ui.widgets.AnyCommand;

import javax.swing.*;
import java.awt.*;

/**
 * This panel displays plain-text 'msg' plain-text debug messages
 * <p/>
 * <p/>
 * Date: 4/27/13
 * Andrey Belomutskiy, (c) 2013-2020
 *
 * @see AnyCommand
 * @see MessagesView
 */
public class MessagesPanel {
    private static final String FONT_SIZE = "font_size";
    private static final String FONT_NAME = "font_name";

    private final MessagesView messagesView;

    private final JPanel buttonPanel = new JPanel(new FlowLayout(FlowLayout.CENTER, 5, 0));
    private final JButton pauseButton = UiUtils.createPauseButton();

    public MessagesPanel(JComponent extraButton, Node config) {
        JPanel middlePanel = new JPanel(new BorderLayout());
        messagesView = new MessagesView(config);
        middlePanel.add(messagesView.messagesScroll, BorderLayout.CENTER);
//        buttonPanel.setBorder(BorderFactory.createLineBorder(Color.cyan));

        pauseButton.addActionListener(event -> setPaused(!messagesView.isPaused()));

        JButton clearButton = UiUtils.createClearButton();
        clearButton.addActionListener(event -> {
            messagesView.clear();
            setPaused(false);
        });

        buttonPanel.add(clearButton);
        buttonPanel.add(pauseButton);
        if (extraButton != null)
            buttonPanel.add(extraButton);
    }

    public void setPaused(boolean isPaused) {
        messagesView.setPaused(isPaused);
        UiUtils.setPauseButtonText(pauseButton, messagesView.isPaused());
    }

    public JPanel getButtonPanel() {
        return buttonPanel;
    }

    public JScrollPane getMessagesScroll() {
        return messagesView.messagesScroll;
    }

    public Font getFont() {
        return messagesView.messages.getFont();
    }

    public void setFont(Font font, Node config) {
        messagesView.messages.setFont(font);
        config.setProperty(FONT_SIZE, font.getSize());
        config.setProperty(FONT_NAME, font.getName());
    }

    public void loadFont(Node config) {
        Font f = getFont();
        int size = config.getIntProperty(FONT_SIZE, f.getSize());
        String name = config.getProperty(FONT_NAME, f.getName());
        setFont(new Font(name, f.getStyle(), size), config);
    }
}
